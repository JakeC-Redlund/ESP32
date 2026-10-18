#include "espnow_bbs_mesh_discovery_contract.h"

#include <ctype.h>
#include <string.h>

typedef struct {
    const char *name;
    const char *label;
} name_label_t;

typedef struct {
    const char *name;
    int code;
} name_code_t;

typedef struct {
    const char *name;
    size_t ingest_budget;
    size_t drain_budget;
    size_t snapshot_budget;
} schedule_profile_t;

static const name_label_t DISCOVERY_EVENTS[] = {
    {"node_seen", "ns"},
    {"node_lost", "nl"},
    {"parent_selected", "ps"},
    {"root_elected", "re"},
    {"root_switched", "rs"},
    {"route_added", "ra"},
    {"route_removed", "rr"},
    {"heal_started", "hs"},
    {"heal_observed", "ho"},
    {"service_seen", "ss"},
    {"capability_seen", "cs"},
    {"ble_client_seen", "bs"},
};

static const char *const DISCOVERY_REQUESTS[] = {
    "discovery_snapshot",
    "discovery_events",
    "service_catalog",
    "capability_report",
};

static const char *const GATE_F_BRIDGE_REQUESTS[] = {
    "msg_post",
    "download_queue",
    "telemetry_report",
    "node_status",
    "protocol_report",
    "state_get",
    "control_intent",
};

static const name_code_t GATE_F_SERVICE_CODES[] = {
    {"direct_message", 1},
    {"file_chunk", 2},
    {"telemetry_report", 3},
    {"node_status", 4},
    {"custody_ack", 5},
    {"control_intent", 6},
};

static const char *const SECRET_FIELD_MARKERS[] = {
    "androidid",
    "body",
    "bondingkey",
    "credential",
    "credentials",
    "key",
    "keys",
    "latitude",
    "lmk",
    "location",
    "longitude",
    "password",
    "passwd",
    "pairingkey",
    "pairingtoken",
    "pmk",
    "rawbody",
    "secret",
    "secrets",
    "token",
    "tokens",
};

static const char *const BLOCKED_LIVE_ACTIONS[] = {
    "ble_pair",
    "erase",
    "flash",
    "live_mesh",
    "mesh_start",
    "monitor",
    "pcap_start",
    "radio_set",
    "relay_set",
    "router_admin",
    "serial_write",
    "xbee_write",
};

static const char *const OWNER_NAMES[] = {
    "mesh_ingress",
    "mesh_drain",
    "mesh_snapshot",
};

static const name_label_t DISPATCH_ACTIONS[] = {
    {"owner_drain", "od"},
    {"owner_ingest", "oi"},
    {"owner_snapshot", "os"},
};

static const schedule_profile_t SCHEDULER_PROFILES[] = {
    {"balanced", 2u, 2u, 1u},
    {"drain_first", 1u, 4u, 1u},
    {"snapshot_only", 0u, 0u, 1u},
};

static const schedule_profile_t *scheduler_profile_by_name(
    const char *profile_name
)
{
    size_t index;

    if (profile_name == NULL) {
        return NULL;
    }
    for (index = 0; index < espnow_bbs_mesh_contract_scheduler_profile_count(); ++index) {
        if (strcmp(profile_name, SCHEDULER_PROFILES[index].name) == 0) {
            return &SCHEDULER_PROFILES[index];
        }
    }
    return NULL;
}

static bool string_in_table(const char *value, const char *const *table, size_t count)
{
    size_t index;

    if (value == NULL) {
        return false;
    }
    for (index = 0; index < count; ++index) {
        if (strcmp(value, table[index]) == 0) {
            return true;
        }
    }
    return false;
}

static bool normalized_secret_match(const char *field_name)
{
    char normalized[128];
    size_t out = 0;
    size_t index;

    for (index = 0; field_name[index] != '\0'; ++index) {
        unsigned char ch = (unsigned char)field_name[index];
        if (ch > 0x7f) {
            return false;
        }
        if (isalnum(ch)) {
            if (out + 1u >= sizeof(normalized)) {
                return false;
            }
            normalized[out] = (char)tolower(ch);
            ++out;
        }
    }
    normalized[out] = '\0';

    for (index = 0; index < espnow_bbs_mesh_contract_secret_marker_count(); ++index) {
        if (strcmp(normalized, SECRET_FIELD_MARKERS[index]) == 0) {
            return true;
        }
    }

    if (out >= 3u && strcmp(&normalized[out - 3u], "key") == 0) {
        return true;
    }
    if (out >= 5u && strcmp(&normalized[out - 5u], "token") == 0) {
        return true;
    }
    if (strstr(normalized, "password") != NULL) {
        return true;
    }
    if (strstr(normalized, "secret") != NULL) {
        return true;
    }
    return false;
}

static espnow_bbs_mesh_contract_status_t limited_ascii_status(
    const char *value,
    size_t max_bytes,
    espnow_bbs_mesh_contract_status_t too_long_status
)
{
    if (value == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_contract_is_ascii(value)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII;
    }
    if (strlen(value) > max_bytes) {
        return too_long_status;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

static void copy_limited(char *target, size_t target_capacity, const char *source)
{
    size_t bytes = strlen(source);

    if (bytes >= target_capacity) {
        bytes = target_capacity - 1u;
    }
    memcpy(target, source, bytes);
    target[bytes] = '\0';
}

static bool append_text(
    char *buffer,
    size_t capacity,
    size_t *used,
    const char *text
)
{
    size_t bytes;

    if (buffer == NULL || used == NULL || text == NULL || capacity == 0u) {
        return false;
    }
    bytes = strlen(text);
    if (*used + bytes >= capacity) {
        return false;
    }
    memcpy(&buffer[*used], text, bytes);
    *used += bytes;
    buffer[*used] = '\0';
    return true;
}

static bool append_size_value(
    char *buffer,
    size_t capacity,
    size_t *used,
    size_t value
)
{
    char digits[32];
    size_t digit_count = 0;

    do {
        digits[digit_count] = (char)('0' + (value % 10u));
        ++digit_count;
        value /= 10u;
    } while (value != 0u);

    while (digit_count > 0u) {
        char digit[2];

        --digit_count;
        digit[0] = digits[digit_count];
        digit[1] = '\0';
        if (!append_text(buffer, capacity, used, digit)) {
            return false;
        }
    }
    return true;
}

static size_t min_size_value(size_t left, size_t right)
{
    return left < right ? left : right;
}

static const char *owner_state_name(espnow_bbs_mesh_contract_owner_state_t state)
{
    switch (state) {
    case ESPNOW_BBS_MESH_CONTRACT_OWNER_RELEASED:
        return "released";
    case ESPNOW_BBS_MESH_CONTRACT_OWNER_CLAIMED:
        return "claimed";
    default:
        return "unknown";
    }
}

static espnow_bbs_mesh_contract_status_t owner_claim_status(
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token
)
{
    if (owner == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (owner->queue == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND;
    }
    if (owner->state != ESPNOW_BBS_MESH_CONTRACT_OWNER_CLAIMED) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED;
    }
    if (owner->active_claim_token != claim_token) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

static espnow_bbs_mesh_contract_status_t format_owner_summary(
    const espnow_bbs_mesh_contract_owner_t *owner,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";owner=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            owner->owner_name[0] != '\0' ? owner->owner_name : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";state=")
        || !append_text(summary, summary_capacity, &used, owner_state_name(owner->state))
        || !append_text(summary, summary_capacity, &used, ";in=")
        || !append_size_value(summary, summary_capacity, &used, owner->accepted)
        || !append_text(summary, summary_capacity, &used, ";out=")
        || !append_size_value(summary, summary_capacity, &used, owner->drained)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, owner->rejected)
        || !append_text(summary, summary_capacity, &used, ";q=")
        || !append_size_value(summary, summary_capacity, &used, owner->queue->count)
        || !append_text(summary, summary_capacity, &used, ";drop=")
        || !append_size_value(summary, summary_capacity, &used, owner->queue->dropped)
        || !append_text(summary, summary_capacity, &used, ";last=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(owner->last_status)
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static bool append_record_endpoint(
    char *buffer,
    size_t capacity,
    size_t *used,
    const char *name,
    const espnow_bbs_mesh_contract_event_record_t *record
)
{
    return append_text(buffer, capacity, used, ";")
        && append_text(buffer, capacity, used, name)
        && append_text(buffer, capacity, used, "=")
        && append_text(buffer, capacity, used, record->event_label)
        && append_text(buffer, capacity, used, ":")
        && append_text(buffer, capacity, used, record->node_id);
}

static espnow_bbs_mesh_contract_status_t format_queue_summary(
    const espnow_bbs_mesh_contract_event_queue_t *queue,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    espnow_bbs_mesh_contract_status_t status;
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";count=")
        || !append_size_value(summary, summary_capacity, &used, queue->count)
        || !append_text(summary, summary_capacity, &used, ";dropped=")
        || !append_size_value(summary, summary_capacity, &used, queue->dropped)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    if (queue->count != 0u) {
        espnow_bbs_mesh_contract_event_record_t oldest;
        espnow_bbs_mesh_contract_event_record_t newest;

        status = espnow_bbs_mesh_contract_event_queue_peek(queue, 0u, &oldest);
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
        if (!append_record_endpoint(
                summary,
                summary_capacity,
                &used,
                "old",
                &oldest
            )) {
            return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
        }
        status = espnow_bbs_mesh_contract_event_queue_peek(
            queue,
            queue->count - 1u,
            &newest
        );
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
        if (!append_record_endpoint(
                summary,
                summary_capacity,
                &used,
                "new",
                &newest
            )) {
            return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
        }
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static bool append_plan_endpoint(
    char *buffer,
    size_t capacity,
    size_t *used,
    const char *name,
    const espnow_bbs_mesh_contract_dispatch_step_t *step
)
{
    return append_text(buffer, capacity, used, ";")
        && append_text(buffer, capacity, used, name)
        && append_text(buffer, capacity, used, "=")
        && append_text(buffer, capacity, used, step->action_label)
        && append_text(buffer, capacity, used, ":")
        && append_text(buffer, capacity, used, step->owner_name);
}

static espnow_bbs_mesh_contract_status_t format_dispatch_plan_summary(
    const espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";plan=")
        || !append_size_value(summary, summary_capacity, &used, plan->count)
        || !append_text(summary, summary_capacity, &used, ";req=")
        || !append_size_value(summary, summary_capacity, &used, plan->requested)
        || !append_text(summary, summary_capacity, &used, ";ready=")
        || !append_size_value(summary, summary_capacity, &used, plan->planned)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, plan->rejected)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    if (plan->count != 0u) {
        if (!append_plan_endpoint(
                summary,
                summary_capacity,
                &used,
                "first",
                &plan->steps[0u]
            )
            || !append_plan_endpoint(
                summary,
                summary_capacity,
                &used,
                "last",
                &plan->steps[plan->count - 1u]
            )) {
            return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
        }
    }

    if (!append_text(summary, summary_capacity, &used, ";status=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(plan->last_status)
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_scheduler_state_summary(
    const espnow_bbs_mesh_contract_scheduler_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";sched=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            state->profile_name[0] != '\0' ? state->profile_name : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";owner=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            state->owner_name[0] != '\0' ? state->owner_name : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";ticks=")
        || !append_size_value(summary, summary_capacity, &used, state->tick_count)
        || !append_text(summary, summary_capacity, &used, ";last_tick=")
        || !append_size_value(summary, summary_capacity, &used, state->last_tick)
        || !append_text(summary, summary_capacity, &used, ";req=")
        || !append_size_value(summary, summary_capacity, &used, state->requested)
        || !append_text(summary, summary_capacity, &used, ";ready=")
        || !append_size_value(summary, summary_capacity, &used, state->planned)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, state->rejected)
        || !append_text(summary, summary_capacity, &used, ";status=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(state->last_status)
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_execution_accounting_summary(
    const espnow_bbs_mesh_contract_execution_accounting_t *accounting,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";acct=")
        || !append_size_value(summary, summary_capacity, &used, accounting->cycle_count)
        || !append_text(summary, summary_capacity, &used, ";owner=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            accounting->owner_name[0] != '\0' ? accounting->owner_name : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";last_cycle=")
        || !append_size_value(summary, summary_capacity, &used, accounting->last_cycle)
        || !append_text(summary, summary_capacity, &used, ";apply=")
        || !append_size_value(summary, summary_capacity, &used, accounting->applied)
        || !append_text(summary, summary_capacity, &used, ";skip=")
        || !append_size_value(summary, summary_capacity, &used, accounting->skipped)
        || !append_text(summary, summary_capacity, &used, ";drain=")
        || !append_size_value(summary, summary_capacity, &used, accounting->drain_ready)
        || !append_text(summary, summary_capacity, &used, ";ingest=")
        || !append_size_value(summary, summary_capacity, &used, accounting->ingest_ready)
        || !append_text(summary, summary_capacity, &used, ";snap=")
        || !append_size_value(summary, summary_capacity, &used, accounting->snapshot_ready)
        || !append_text(summary, summary_capacity, &used, ";status=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(accounting->last_status)
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_runtime_cycle_summary(
    const espnow_bbs_mesh_contract_runtime_cycle_t *runtime,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";runtime=")
        || !append_size_value(summary, summary_capacity, &used, runtime->cycle_count)
        || !append_text(summary, summary_capacity, &used, ";profile=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            runtime->profile_name[0] != '\0' ? runtime->profile_name : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";owner=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            runtime->owner_name[0] != '\0' ? runtime->owner_name : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";last_cycle=")
        || !append_size_value(summary, summary_capacity, &used, runtime->last_cycle)
        || !append_text(summary, summary_capacity, &used, ";sched=")
        || !append_size_value(summary, summary_capacity, &used, runtime->scheduled)
        || !append_text(summary, summary_capacity, &used, ";apply=")
        || !append_size_value(summary, summary_capacity, &used, runtime->applied)
        || !append_text(summary, summary_capacity, &used, ";skip=")
        || !append_size_value(summary, summary_capacity, &used, runtime->skipped)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, runtime->rejected)
        || !append_text(summary, summary_capacity, &used, ";status=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(runtime->last_status)
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t runtime_scheduler_policy_status(
    const char *policy,
    const char *expected
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = limited_ascii_status(
        policy,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_POLICY_NAME_MAX_BYTES,
        ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_POLICY_NAME_TOO_LONG
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (strcmp(policy, expected) != 0) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_UNKNOWN_POLICY;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

static espnow_bbs_mesh_contract_status_t runtime_scheduler_config_status(
    const espnow_bbs_mesh_contract_runtime_scheduler_config_t *config
)
{
    espnow_bbs_mesh_contract_status_t status;

    if (config == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }

    status = espnow_bbs_mesh_contract_owner_name_status(config->owner_name);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = espnow_bbs_mesh_contract_scheduler_profile_status(config->profile_name);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = runtime_scheduler_policy_status(
        config->invoke_policy,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = runtime_scheduler_policy_status(
        config->task_policy,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = runtime_scheduler_policy_status(
        config->timer_policy,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (config->period_ticks == 0u || config->yield_interval == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_ZERO;
    }
    if (config->period_ticks > ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_PERIOD_MAX
        || config->yield_interval > ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_PERIOD_MAX) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_TOO_LARGE;
    }
    if (config->priority != 0u || config->stack_bytes != 0u
        || config->core_id != ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE
        || config->static_state) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

static espnow_bbs_mesh_contract_status_t format_runtime_scheduler_summary(
    const espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";schedsrc=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            runtime_scheduler->configured
                ? runtime_scheduler->invoke_policy
                : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";owner=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            runtime_scheduler->owner_name[0] != '\0'
                ? runtime_scheduler->owner_name
                : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";profile=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            runtime_scheduler->profile_name[0] != '\0'
                ? runtime_scheduler->profile_name
                : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";steps=")
        || !append_size_value(summary, summary_capacity, &used, runtime_scheduler->step_count)
        || !append_text(summary, summary_capacity, &used, ";last_cycle=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            runtime_scheduler->runtime.last_cycle
        )
        || !append_text(summary, summary_capacity, &used, ";sched=")
        || !append_size_value(summary, summary_capacity, &used, runtime_scheduler->scheduled)
        || !append_text(summary, summary_capacity, &used, ";apply=")
        || !append_size_value(summary, summary_capacity, &used, runtime_scheduler->applied)
        || !append_text(summary, summary_capacity, &used, ";skip=")
        || !append_size_value(summary, summary_capacity, &used, runtime_scheduler->skipped)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, runtime_scheduler->rejected)
        || !append_text(summary, summary_capacity, &used, ";yield=")
        || !append_size_value(summary, summary_capacity, &used, runtime_scheduler->yield_due)
        || !append_text(summary, summary_capacity, &used, ";status=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(runtime_scheduler->last_status)
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static const char *configured_name(bool configured)
{
    return configured ? "yes" : "no";
}

static bool append_service_endpoint(
    char *buffer,
    size_t capacity,
    size_t *used,
    const char *name,
    const name_code_t *service
)
{
    return append_text(buffer, capacity, used, ";")
        && append_text(buffer, capacity, used, name)
        && append_text(buffer, capacity, used, "=")
        && append_text(buffer, capacity, used, service->name)
        && append_text(buffer, capacity, used, ":")
        && append_size_value(buffer, capacity, used, (size_t)service->code);
}

static espnow_bbs_mesh_contract_status_t format_bridge_snapshot_summary(
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (state->event_queue == NULL || state->owner == NULL
        || state->runtime_scheduler == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";req=discovery_snapshot")
        || !append_text(summary, summary_capacity, &used, ";transport=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_TRANSPORT
        )
        || !append_text(summary, summary_capacity, &used, ";mode=")
        || !append_text(summary, summary_capacity, &used, ESPNOW_BBS_MESH_DISCOVERY_MODE)
        || !append_text(summary, summary_capacity, &used, ";admin=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_ADMIN_GATE
        )
        || !append_text(summary, summary_capacity, &used, ";events=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            state->event_queue->count
        )
        || !append_text(summary, summary_capacity, &used, ";drop=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            state->event_queue->dropped
        )
        || !append_text(summary, summary_capacity, &used, ";owner=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            state->owner->owner_name[0] != '\0' ? state->owner->owner_name : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";ostate=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            owner_state_name(state->owner->state)
        )
        || !append_text(summary, summary_capacity, &used, ";steps=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            state->runtime_scheduler->step_count
        )
        || !append_text(summary, summary_capacity, &used, ";status=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(
                state->runtime_scheduler->last_status
            )
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_bridge_events_summary(
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    espnow_bbs_mesh_contract_status_t status;
    size_t used = 0;

    if (state->event_queue == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";req=discovery_events")
        || !append_text(summary, summary_capacity, &used, ";count=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            state->event_queue->count
        )
        || !append_text(summary, summary_capacity, &used, ";dropped=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            state->event_queue->dropped
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    if (state->event_queue->count != 0u) {
        espnow_bbs_mesh_contract_event_record_t oldest;
        espnow_bbs_mesh_contract_event_record_t newest;

        status = espnow_bbs_mesh_contract_event_queue_peek(
            state->event_queue,
            0u,
            &oldest
        );
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
        status = espnow_bbs_mesh_contract_event_queue_peek(
            state->event_queue,
            state->event_queue->count - 1u,
            &newest
        );
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
        if (!append_record_endpoint(
                summary,
                summary_capacity,
                &used,
                "old",
                &oldest
            )
            || !append_record_endpoint(
                summary,
                summary_capacity,
                &used,
                "new",
                &newest
            )) {
            return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
        }
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_bridge_service_catalog_summary(
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;
    size_t last_service_index =
        espnow_bbs_mesh_contract_gate_f_service_count() - 1u;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";req=service_catalog")
        || !append_text(summary, summary_capacity, &used, ";svc=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_gate_f_service_count()
        )
        || !append_text(summary, summary_capacity, &used, ";disc_req=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_request_count()
        )
        || !append_text(summary, summary_capacity, &used, ";gatef_req=")
        || !append_size_value(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_bridge_request_count()
        )
        || !append_service_endpoint(
            summary,
            summary_capacity,
            &used,
            "first",
            &GATE_F_SERVICE_CODES[0u]
        )
        || !append_service_endpoint(
            summary,
            summary_capacity,
            &used,
            "last",
            &GATE_F_SERVICE_CODES[last_service_index]
        )
        || !append_text(summary, summary_capacity, &used, ";bridge=disjoint")) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_bridge_capability_summary(
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (state->runtime_scheduler == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";req=capability_report")
        || !append_text(summary, summary_capacity, &used, ";transport=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_TRANSPORT
        )
        || !append_text(summary, summary_capacity, &used, ";admin=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_ADMIN_GATE
        )
        || !append_text(summary, summary_capacity, &used, ";configured=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            configured_name(state->runtime_scheduler->configured)
        )
        || !append_text(summary, summary_capacity, &used, ";invoke=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY
        )
        || !append_text(summary, summary_capacity, &used, ";task=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY
        )
        || !append_text(summary, summary_capacity, &used, ";timer=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY
        )
        || !append_text(summary, summary_capacity, &used, ";live=blocked")
        || !append_text(summary, summary_capacity, &used, ";persist=none")
        || !append_text(summary, summary_capacity, &used, ";serial=closed")) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_bridge_request_router_summary(
    const espnow_bbs_mesh_contract_bridge_request_router_t *router,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";router=local_request")
        || !append_text(summary, summary_capacity, &used, ";attempt=")
        || !append_size_value(summary, summary_capacity, &used, router->attempted)
        || !append_text(summary, summary_capacity, &used, ";accept=")
        || !append_size_value(summary, summary_capacity, &used, router->accepted)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, router->rejected)
        || !append_text(summary, summary_capacity, &used, ";last_req=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            router->last_request[0] != '\0' ? router->last_request : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";last=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(router->last_status)
        )
        || !append_text(summary, summary_capacity, &used, ";live=blocked")) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_bridge_dispatch_envelope_summary(
    const espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";envelope=local_dispatch")
        || !append_text(summary, summary_capacity, &used, ";attempt=")
        || !append_size_value(summary, summary_capacity, &used, envelope->attempted)
        || !append_text(summary, summary_capacity, &used, ";would=")
        || !append_size_value(summary, summary_capacity, &used, envelope->would_dispatch)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, envelope->rejected)
        || !append_text(summary, summary_capacity, &used, ";last_req=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            envelope->last_request[0] != '\0'
                ? envelope->last_request
                : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";last=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(envelope->last_status)
        )
        || !append_text(summary, summary_capacity, &used, ";live=blocked")
        || !append_text(summary, summary_capacity, &used, ";serial=closed")) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static bool bridge_request_text_trim_byte(unsigned char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

static espnow_bbs_mesh_contract_status_t normalize_bridge_request_text_intent(
    const char *request_text,
    char *normalized_request,
    size_t normalized_capacity
)
{
    size_t start = 0u;
    size_t end = 0u;
    size_t length;
    size_t index;

    if (request_text == NULL || normalized_request == NULL
        || normalized_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    normalized_request[0] = '\0';

    while (request_text[end] != '\0') {
        unsigned char ch = (unsigned char)request_text[end];
        if (ch > 0x7fu) {
            return ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII;
        }
        ++end;
    }

    while (start < end
        && bridge_request_text_trim_byte((unsigned char)request_text[start])) {
        ++start;
    }
    while (end > start
        && bridge_request_text_trim_byte((unsigned char)request_text[end - 1u])) {
        --end;
    }
    if (start == end) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST;
    }

    length = end - start;
    if (length + 1u > normalized_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST;
    }

    for (index = 0u; index < length; ++index) {
        unsigned char ch = (unsigned char)request_text[start + index];
        if (bridge_request_text_trim_byte(ch) || ch < 0x20u || ch == 0x7fu) {
            normalized_request[0] = '\0';
            return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST;
        }
        normalized_request[index] = (char)ch;
    }
    normalized_request[length] = '\0';
    return espnow_bbs_mesh_contract_request_status(normalized_request);
}

static espnow_bbs_mesh_contract_status_t
format_bridge_request_text_intent_summary(
    const espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0u;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";intent=request_text")
        || !append_text(summary, summary_capacity, &used, ";attempt=")
        || !append_size_value(summary, summary_capacity, &used, intent->attempted)
        || !append_text(summary, summary_capacity, &used, ";accept=")
        || !append_size_value(summary, summary_capacity, &used, intent->accepted)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, intent->rejected)
        || !append_text(summary, summary_capacity, &used, ";last_req=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            intent->normalized_request[0] != '\0'
                ? intent->normalized_request
                : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";last=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(intent->last_status)
        )
        || !append_text(summary, summary_capacity, &used, ";live=blocked")
        || !append_text(summary, summary_capacity, &used, ";serial=closed")
        || !append_text(summary, summary_capacity, &used, ";abi=closed")) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

static espnow_bbs_mesh_contract_status_t format_persistence_intent_summary(
    const espnow_bbs_mesh_contract_persistence_intent_t *persistence,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0u;

    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_text(summary, summary_capacity, &used, "schema=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_DISCOVERY_SCHEMA
        )
        || !append_text(summary, summary_capacity, &used, ";persistence=intent")
        || !append_text(summary, summary_capacity, &used, ";attempt=")
        || !append_size_value(summary, summary_capacity, &used, persistence->attempted)
        || !append_text(summary, summary_capacity, &used, ";stage=")
        || !append_size_value(summary, summary_capacity, &used, persistence->staged)
        || !append_text(summary, summary_capacity, &used, ";reject=")
        || !append_size_value(summary, summary_capacity, &used, persistence->rejected)
        || !append_text(summary, summary_capacity, &used, ";last_req=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            persistence->last_request[0] != '\0'
                ? persistence->last_request
                : "none"
        )
        || !append_text(summary, summary_capacity, &used, ";last=")
        || !append_text(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_contract_status_name(persistence->last_status)
        )
        || !append_text(summary, summary_capacity, &used, ";bytes=")
        || !append_size_value(summary, summary_capacity, &used, persistence->staged_bytes)
        || !append_text(summary, summary_capacity, &used, ";storage=volatile")
        || !append_text(summary, summary_capacity, &used, ";nvs=closed")
        || !append_text(summary, summary_capacity, &used, ";file=closed")
        || !append_text(summary, summary_capacity, &used, ";live=blocked")
        || !append_text(summary, summary_capacity, &used, ";serial=closed")
        || !append_text(summary, summary_capacity, &used, ";abi=closed")) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }

    return espnow_bbs_mesh_contract_summary_status(summary, line_bytes);
}

const char *espnow_bbs_mesh_contract_status_name(
    espnow_bbs_mesh_contract_status_t status
)
{
    switch (status) {
    case ESPNOW_BBS_MESH_CONTRACT_OK:
        return "ok";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL:
        return "null";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_EVENT:
        return "unknown_event";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST:
        return "unknown_request";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII:
        return "non_ascii";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD:
        return "secret_field_blocked";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_LINE_TOO_LONG:
        return "line_too_long";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_LIVE_ACTION:
        return "live_action_blocked";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_NODE_TOO_LONG:
        return "node_too_long";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_DETAIL_TOO_LONG:
        return "detail_too_long";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL:
        return "queue_full";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY:
        return "queue_empty";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY:
        return "summary_capacity";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_OWNER:
        return "unknown_owner";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NAME_TOO_LONG:
        return "owner_name_too_long";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND:
        return "owner_unbound";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_ALREADY_CLAIMED:
        return "owner_already_claimed";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED:
        return "owner_not_claimed";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE:
        return "owner_stale";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_DISPATCH_ACTION:
        return "unknown_dispatch_action";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_ACTION_NAME_TOO_LONG:
        return "dispatch_action_name_too_long";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_PLAN_FULL:
        return "dispatch_plan_full";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_ZERO:
        return "dispatch_budget_zero";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_TOO_LARGE:
        return "dispatch_budget_too_large";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_SCHEDULER_PROFILE:
        return "unknown_scheduler_profile";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_PROFILE_NAME_TOO_LONG:
        return "scheduler_profile_name_too_long";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_ZERO:
        return "scheduler_tick_zero";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_TOO_LARGE:
        return "scheduler_tick_too_large";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_ZERO:
        return "execution_cycle_zero";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_TOO_LARGE:
        return "execution_cycle_too_large";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_PLAN_EMPTY:
        return "execution_plan_empty";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_STEP_NOT_READY:
        return "execution_step_not_ready";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_OWNER_MISMATCH:
        return "execution_owner_mismatch";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_ZERO:
        return "runtime_cycle_zero";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_TOO_LARGE:
        return "runtime_cycle_too_large";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_POLICY_NAME_TOO_LONG:
        return "runtime_scheduler_policy_name_too_long";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_UNKNOWN_POLICY:
        return "runtime_scheduler_unknown_policy";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_ZERO:
        return "runtime_scheduler_period_zero";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_TOO_LARGE:
        return "runtime_scheduler_period_too_large";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN:
        return "runtime_scheduler_runtime_open";
    case ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_NOT_CONFIGURED:
        return "runtime_scheduler_not_configured";
    default:
        return "unknown_status";
    }
}

size_t espnow_bbs_mesh_contract_event_count(void)
{
    return sizeof(DISCOVERY_EVENTS) / sizeof(DISCOVERY_EVENTS[0]);
}

const char *espnow_bbs_mesh_contract_event_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_event_count()) {
        return NULL;
    }
    return DISCOVERY_EVENTS[index].name;
}

const char *espnow_bbs_mesh_contract_event_label(const char *event_name)
{
    size_t index;

    if (event_name == NULL) {
        return NULL;
    }
    for (index = 0; index < espnow_bbs_mesh_contract_event_count(); ++index) {
        if (strcmp(event_name, DISCOVERY_EVENTS[index].name) == 0) {
            return DISCOVERY_EVENTS[index].label;
        }
    }
    return NULL;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_status(
    const char *event_name
)
{
    if (event_name == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_contract_is_ascii(event_name)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII;
    }
    if (espnow_bbs_mesh_contract_event_label(event_name) == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_EVENT;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_event_queue_init(
    espnow_bbs_mesh_contract_event_queue_t *queue
)
{
    if (queue == NULL) {
        return;
    }
    memset(queue, 0, sizeof(*queue));
    queue->next_sequence = 1u;
}

size_t espnow_bbs_mesh_contract_event_queue_count(
    const espnow_bbs_mesh_contract_event_queue_t *queue
)
{
    if (queue == NULL) {
        return 0u;
    }
    return queue->count;
}

size_t espnow_bbs_mesh_contract_event_queue_dropped(
    const espnow_bbs_mesh_contract_event_queue_t *queue
)
{
    if (queue == NULL) {
        return 0u;
    }
    return queue->dropped;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_push(
    espnow_bbs_mesh_contract_event_queue_t *queue,
    const espnow_bbs_mesh_contract_event_input_t *event
)
{
    espnow_bbs_mesh_contract_event_record_t *record;
    espnow_bbs_mesh_contract_status_t status;
    const char *label;
    size_t write_index;

    if (queue == NULL || event == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = espnow_bbs_mesh_contract_event_status(event->event_name);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = limited_ascii_status(
        event->node_id,
        ESPNOW_BBS_MESH_CONTRACT_NODE_ID_MAX_BYTES,
        ESPNOW_BBS_MESH_CONTRACT_REJECT_NODE_TOO_LONG
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = limited_ascii_status(
        event->detail,
        ESPNOW_BBS_MESH_CONTRACT_DETAIL_MAX_BYTES,
        ESPNOW_BBS_MESH_CONTRACT_REJECT_DETAIL_TOO_LONG
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (queue->count >= ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY) {
        ++queue->dropped;
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL;
    }

    label = espnow_bbs_mesh_contract_event_label(event->event_name);
    write_index = (queue->head + queue->count)
        % ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
    record = &queue->records[write_index];
    copy_limited(record->event_name, sizeof(record->event_name), event->event_name);
    copy_limited(record->event_label, sizeof(record->event_label), label);
    copy_limited(record->node_id, sizeof(record->node_id), event->node_id);
    copy_limited(record->detail, sizeof(record->detail), event->detail);
    record->sequence = queue->next_sequence;
    ++queue->next_sequence;
    ++queue->count;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_peek(
    const espnow_bbs_mesh_contract_event_queue_t *queue,
    size_t offset,
    espnow_bbs_mesh_contract_event_record_t *record
)
{
    size_t read_index;

    if (queue == NULL || record == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (offset >= queue->count) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY;
    }
    read_index = (queue->head + offset)
        % ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
    *record = queue->records[read_index];
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_pop(
    espnow_bbs_mesh_contract_event_queue_t *queue,
    espnow_bbs_mesh_contract_event_record_t *record
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = espnow_bbs_mesh_contract_event_queue_peek(queue, 0u, record);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    queue->head = (queue->head + 1u)
        % ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
    --queue->count;
    if (queue->count == 0u) {
        queue->head = 0u;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_summary(
    const espnow_bbs_mesh_contract_event_queue_t *queue,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (queue == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_queue_summary(queue, scratch, sizeof(scratch), line_bytes);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

size_t espnow_bbs_mesh_contract_owner_name_count(void)
{
    return sizeof(OWNER_NAMES) / sizeof(OWNER_NAMES[0]);
}

const char *espnow_bbs_mesh_contract_owner_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_owner_name_count()) {
        return NULL;
    }
    return OWNER_NAMES[index];
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_name_status(
    const char *owner_name
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = limited_ascii_status(
        owner_name,
        ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES,
        ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NAME_TOO_LONG
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (!string_in_table(
            owner_name,
            OWNER_NAMES,
            espnow_bbs_mesh_contract_owner_name_count()
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_OWNER;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_owner_init(
    espnow_bbs_mesh_contract_owner_t *owner,
    espnow_bbs_mesh_contract_event_queue_t *queue
)
{
    if (owner == NULL) {
        return;
    }
    memset(owner, 0, sizeof(*owner));
    owner->queue = queue;
    owner->state = ESPNOW_BBS_MESH_CONTRACT_OWNER_RELEASED;
    owner->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    owner->next_claim_token = 1u;
}

void espnow_bbs_mesh_contract_owner_reset(
    espnow_bbs_mesh_contract_owner_t *owner
)
{
    espnow_bbs_mesh_contract_event_queue_t *queue;

    if (owner == NULL) {
        return;
    }
    queue = owner->queue;
    espnow_bbs_mesh_contract_owner_init(owner, queue);
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_claim(
    espnow_bbs_mesh_contract_owner_t *owner,
    const char *owner_name,
    size_t *claim_token
)
{
    espnow_bbs_mesh_contract_status_t status;

    if (owner == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (owner->queue == NULL) {
        owner->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND;
        return owner->last_status;
    }
    if (owner->state == ESPNOW_BBS_MESH_CONTRACT_OWNER_CLAIMED) {
        owner->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_ALREADY_CLAIMED;
        return owner->last_status;
    }
    status = espnow_bbs_mesh_contract_owner_name_status(owner_name);
    owner->last_status = status;
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    copy_limited(owner->owner_name, sizeof(owner->owner_name), owner_name);
    owner->state = ESPNOW_BBS_MESH_CONTRACT_OWNER_CLAIMED;
    owner->active_claim_token = owner->next_claim_token;
    ++owner->next_claim_token;
    if (claim_token != NULL) {
        *claim_token = owner->active_claim_token;
    }
    owner->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_release(
    espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = owner_claim_status(owner, claim_token);
    if (owner != NULL) {
        owner->last_status = status;
    }
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    owner->owner_name[0] = '\0';
    owner->state = ESPNOW_BBS_MESH_CONTRACT_OWNER_RELEASED;
    owner->active_claim_token = 0u;
    owner->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_ingest(
    espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    const espnow_bbs_mesh_contract_event_input_t *event
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = owner_claim_status(owner, claim_token);
    if (owner != NULL) {
        owner->last_status = status;
    }
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = espnow_bbs_mesh_contract_event_queue_push(owner->queue, event);
    owner->last_status = status;
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        ++owner->accepted;
    } else {
        ++owner->rejected;
    }
    return status;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_drain(
    espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    espnow_bbs_mesh_contract_event_record_t *record
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = owner_claim_status(owner, claim_token);
    if (owner != NULL) {
        owner->last_status = status;
    }
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    status = espnow_bbs_mesh_contract_event_queue_pop(owner->queue, record);
    owner->last_status = status;
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        ++owner->drained;
    }
    return status;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_summary(
    const espnow_bbs_mesh_contract_owner_t *owner,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (owner == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (owner->queue == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND;
    }
    status = format_owner_summary(owner, scratch, sizeof(scratch), line_bytes);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

size_t espnow_bbs_mesh_contract_dispatch_action_count(void)
{
    return sizeof(DISPATCH_ACTIONS) / sizeof(DISPATCH_ACTIONS[0]);
}

const char *espnow_bbs_mesh_contract_dispatch_action_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_dispatch_action_count()) {
        return NULL;
    }
    return DISPATCH_ACTIONS[index].name;
}

const char *espnow_bbs_mesh_contract_dispatch_action_label(
    const char *action_name
)
{
    size_t index;

    if (action_name == NULL) {
        return NULL;
    }
    for (index = 0; index < espnow_bbs_mesh_contract_dispatch_action_count(); ++index) {
        if (strcmp(action_name, DISPATCH_ACTIONS[index].name) == 0) {
            return DISPATCH_ACTIONS[index].label;
        }
    }
    return NULL;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_action_status(
    const char *action_name
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = limited_ascii_status(
        action_name,
        ESPNOW_BBS_MESH_CONTRACT_DISPATCH_ACTION_NAME_MAX_BYTES,
        ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_ACTION_NAME_TOO_LONG
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (espnow_bbs_mesh_contract_dispatch_action_label(action_name) == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_DISPATCH_ACTION;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_dispatch_plan_init(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan
)
{
    if (plan == NULL) {
        return;
    }
    memset(plan, 0, sizeof(*plan));
    plan->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_dispatch_plan_reset(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan
)
{
    espnow_bbs_mesh_contract_dispatch_plan_init(plan);
}

size_t espnow_bbs_mesh_contract_dispatch_plan_count(
    const espnow_bbs_mesh_contract_dispatch_plan_t *plan
)
{
    if (plan == NULL) {
        return 0u;
    }
    return plan->count;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_plan_stage(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    const char *owner_name,
    const char *action_name,
    size_t requested,
    size_t planned,
    espnow_bbs_mesh_contract_status_t step_status
)
{
    espnow_bbs_mesh_contract_dispatch_step_t *step;
    espnow_bbs_mesh_contract_status_t status;
    const char *label;

    if (plan == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = espnow_bbs_mesh_contract_owner_name_status(owner_name);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        plan->last_status = status;
        return status;
    }
    status = espnow_bbs_mesh_contract_dispatch_action_status(action_name);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        plan->last_status = status;
        return status;
    }
    if (requested == 0u) {
        plan->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_ZERO;
        return plan->last_status;
    }
    if (requested > ESPNOW_BBS_MESH_CONTRACT_DISPATCH_BUDGET_MAX
        || planned > requested) {
        plan->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_TOO_LARGE;
        return plan->last_status;
    }
    if (plan->count >= ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY) {
        plan->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_PLAN_FULL;
        return plan->last_status;
    }

    label = espnow_bbs_mesh_contract_dispatch_action_label(action_name);
    step = &plan->steps[plan->count];
    copy_limited(step->owner_name, sizeof(step->owner_name), owner_name);
    copy_limited(step->action_name, sizeof(step->action_name), action_name);
    copy_limited(step->action_label, sizeof(step->action_label), label);
    step->requested = requested;
    step->planned = planned;
    step->status = step_status;
    ++plan->count;
    plan->requested += requested;
    plan->planned += planned;
    if (planned < requested) {
        plan->rejected += requested - planned;
    }
    plan->last_status = step_status;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_plan_from_owner(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    size_t ingest_budget,
    size_t drain_budget,
    size_t snapshot_budget
)
{
    espnow_bbs_mesh_contract_status_t status;
    size_t available;
    size_t planned;

    if (plan == NULL || owner == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    espnow_bbs_mesh_contract_dispatch_plan_init(plan);
    status = owner_claim_status(owner, claim_token);
    plan->last_status = status;
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (ingest_budget == 0u && drain_budget == 0u && snapshot_budget == 0u) {
        plan->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_ZERO;
        return plan->last_status;
    }

    if (drain_budget != 0u) {
        planned = min_size_value(drain_budget, owner->queue->count);
        status = espnow_bbs_mesh_contract_dispatch_plan_stage(
            plan,
            owner->owner_name,
            "owner_drain",
            drain_budget,
            planned,
            planned == 0u
                ? ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY
                : ESPNOW_BBS_MESH_CONTRACT_OK
        );
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
    }
    if (ingest_budget != 0u) {
        available = ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY - owner->queue->count;
        planned = min_size_value(ingest_budget, available);
        status = espnow_bbs_mesh_contract_dispatch_plan_stage(
            plan,
            owner->owner_name,
            "owner_ingest",
            ingest_budget,
            planned,
            planned == 0u
                ? ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL
                : ESPNOW_BBS_MESH_CONTRACT_OK
        );
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
    }
    if (snapshot_budget != 0u) {
        status = espnow_bbs_mesh_contract_dispatch_plan_stage(
            plan,
            owner->owner_name,
            "owner_snapshot",
            snapshot_budget,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        );
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_plan_summary(
    const espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (plan == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_dispatch_plan_summary(plan, scratch, sizeof(scratch), line_bytes);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

size_t espnow_bbs_mesh_contract_scheduler_profile_count(void)
{
    return sizeof(SCHEDULER_PROFILES) / sizeof(SCHEDULER_PROFILES[0]);
}

const char *espnow_bbs_mesh_contract_scheduler_profile_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_scheduler_profile_count()) {
        return NULL;
    }
    return SCHEDULER_PROFILES[index].name;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_scheduler_profile_status(
    const char *profile_name
)
{
    espnow_bbs_mesh_contract_status_t status;

    status = limited_ascii_status(
        profile_name,
        ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_PROFILE_NAME_MAX_BYTES,
        ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_PROFILE_NAME_TOO_LONG
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (scheduler_profile_by_name(profile_name) == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_SCHEDULER_PROFILE;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_scheduler_state_init(
    espnow_bbs_mesh_contract_scheduler_state_t *state
)
{
    if (state == NULL) {
        return;
    }
    memset(state, 0, sizeof(*state));
    state->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_scheduler_state_reset(
    espnow_bbs_mesh_contract_scheduler_state_t *state
)
{
    espnow_bbs_mesh_contract_scheduler_state_init(state);
}

size_t espnow_bbs_mesh_contract_scheduler_state_tick_count(
    const espnow_bbs_mesh_contract_scheduler_state_t *state
)
{
    if (state == NULL) {
        return 0u;
    }
    return state->tick_count;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_scheduler_stage_tick(
    espnow_bbs_mesh_contract_scheduler_state_t *state,
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    const char *profile_name,
    size_t tick
)
{
    const schedule_profile_t *profile;
    espnow_bbs_mesh_contract_status_t status;

    if (state == NULL || plan == NULL || owner == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = espnow_bbs_mesh_contract_scheduler_profile_status(profile_name);
    state->last_status = status;
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (tick == 0u) {
        state->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_ZERO;
        return state->last_status;
    }
    if (tick > ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_TICK_MAX) {
        state->last_status =
            ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_TOO_LARGE;
        return state->last_status;
    }

    profile = scheduler_profile_by_name(profile_name);
    status = espnow_bbs_mesh_contract_dispatch_plan_from_owner(
        plan,
        owner,
        claim_token,
        profile->ingest_budget,
        profile->drain_budget,
        profile->snapshot_budget
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        state->last_status = status;
        return status;
    }

    copy_limited(state->profile_name, sizeof(state->profile_name), profile->name);
    copy_limited(state->owner_name, sizeof(state->owner_name), owner->owner_name);
    state->last_tick = tick;
    ++state->tick_count;
    state->requested += plan->requested;
    state->planned += plan->planned;
    state->rejected += plan->rejected;
    state->last_status = plan->last_status;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_scheduler_state_summary(
    const espnow_bbs_mesh_contract_scheduler_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (state == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_scheduler_state_summary(state, scratch, sizeof(scratch), line_bytes);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_execution_accounting_init(
    espnow_bbs_mesh_contract_execution_accounting_t *accounting
)
{
    if (accounting == NULL) {
        return;
    }
    memset(accounting, 0, sizeof(*accounting));
    accounting->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_execution_accounting_reset(
    espnow_bbs_mesh_contract_execution_accounting_t *accounting
)
{
    espnow_bbs_mesh_contract_execution_accounting_init(accounting);
}

size_t espnow_bbs_mesh_contract_execution_accounting_cycle_count(
    const espnow_bbs_mesh_contract_execution_accounting_t *accounting
)
{
    if (accounting == NULL) {
        return 0u;
    }
    return accounting->cycle_count;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_execution_accounting_apply_plan(
    espnow_bbs_mesh_contract_execution_accounting_t *accounting,
    const espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    size_t cycle
)
{
    char owner_name[ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t applied = 0u;
    size_t skipped = 0u;
    size_t drain_ready = 0u;
    size_t ingest_ready = 0u;
    size_t snapshot_ready = 0u;
    size_t index;

    if (accounting == NULL || plan == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (cycle == 0u) {
        accounting->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_ZERO;
        return accounting->last_status;
    }
    if (cycle > ESPNOW_BBS_MESH_CONTRACT_EXECUTION_CYCLE_MAX) {
        accounting->last_status =
            ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_TOO_LARGE;
        return accounting->last_status;
    }
    if (plan->count == 0u) {
        accounting->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_PLAN_EMPTY;
        return accounting->last_status;
    }
    if (plan->count > ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY) {
        accounting->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_PLAN_FULL;
        return accounting->last_status;
    }

    owner_name[0] = '\0';
    for (index = 0u; index < plan->count; ++index) {
        const espnow_bbs_mesh_contract_dispatch_step_t *step = &plan->steps[index];

        status = espnow_bbs_mesh_contract_owner_name_status(step->owner_name);
        accounting->last_status = status;
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
        if (index == 0u) {
            copy_limited(owner_name, sizeof(owner_name), step->owner_name);
        } else if (strcmp(owner_name, step->owner_name) != 0) {
            accounting->last_status =
                ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_OWNER_MISMATCH;
            return accounting->last_status;
        }

        status = espnow_bbs_mesh_contract_dispatch_action_status(step->action_name);
        accounting->last_status = status;
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
        if (step->requested == 0u) {
            accounting->last_status =
                ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_ZERO;
            return accounting->last_status;
        }
        if (step->requested > ESPNOW_BBS_MESH_CONTRACT_DISPATCH_BUDGET_MAX
            || step->planned > step->requested) {
            accounting->last_status =
                ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_TOO_LARGE;
            return accounting->last_status;
        }
        if (step->planned != 0u
            && step->status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            accounting->last_status =
                ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_STEP_NOT_READY;
            return accounting->last_status;
        }

        applied += step->planned;
        skipped += step->requested - step->planned;
        if (strcmp(step->action_name, "owner_drain") == 0) {
            drain_ready += step->planned;
        } else if (strcmp(step->action_name, "owner_ingest") == 0) {
            ingest_ready += step->planned;
        } else if (strcmp(step->action_name, "owner_snapshot") == 0) {
            snapshot_ready += step->planned;
        }
    }

    if (applied == 0u) {
        accounting->last_status =
            ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_STEP_NOT_READY;
        return accounting->last_status;
    }

    copy_limited(accounting->owner_name, sizeof(accounting->owner_name), owner_name);
    accounting->last_cycle = cycle;
    ++accounting->cycle_count;
    accounting->applied += applied;
    accounting->skipped += skipped;
    accounting->drain_ready += drain_ready;
    accounting->ingest_ready += ingest_ready;
    accounting->snapshot_ready += snapshot_ready;
    accounting->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_execution_accounting_summary(
    const espnow_bbs_mesh_contract_execution_accounting_t *accounting,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (accounting == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_execution_accounting_summary(
        accounting,
        scratch,
        sizeof(scratch),
        line_bytes
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_runtime_cycle_init(
    espnow_bbs_mesh_contract_runtime_cycle_t *runtime
)
{
    if (runtime == NULL) {
        return;
    }
    memset(runtime, 0, sizeof(*runtime));
    runtime->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_runtime_cycle_reset(
    espnow_bbs_mesh_contract_runtime_cycle_t *runtime
)
{
    espnow_bbs_mesh_contract_runtime_cycle_init(runtime);
}

size_t espnow_bbs_mesh_contract_runtime_cycle_count(
    const espnow_bbs_mesh_contract_runtime_cycle_t *runtime
)
{
    if (runtime == NULL) {
        return 0u;
    }
    return runtime->cycle_count;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_cycle_step(
    espnow_bbs_mesh_contract_runtime_cycle_t *runtime,
    espnow_bbs_mesh_contract_scheduler_state_t *scheduler,
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    espnow_bbs_mesh_contract_execution_accounting_t *accounting,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    const char *profile_name,
    size_t cycle
)
{
    espnow_bbs_mesh_contract_scheduler_state_t scheduler_next;
    espnow_bbs_mesh_contract_dispatch_plan_t plan_next;
    espnow_bbs_mesh_contract_execution_accounting_t accounting_next;
    espnow_bbs_mesh_contract_status_t status;
    size_t applied_before;
    size_t skipped_before;

    if (runtime == NULL || scheduler == NULL || plan == NULL
        || accounting == NULL || owner == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (cycle == 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_ZERO;
    }
    if (cycle > ESPNOW_BBS_MESH_CONTRACT_RUNTIME_CYCLE_MAX) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_TOO_LARGE;
    }

    scheduler_next = *scheduler;
    accounting_next = *accounting;
    status = espnow_bbs_mesh_contract_scheduler_stage_tick(
        &scheduler_next,
        &plan_next,
        owner,
        claim_token,
        profile_name,
        cycle
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    applied_before = accounting_next.applied;
    skipped_before = accounting_next.skipped;
    status = espnow_bbs_mesh_contract_execution_accounting_apply_plan(
        &accounting_next,
        &plan_next,
        cycle
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    *scheduler = scheduler_next;
    *plan = plan_next;
    *accounting = accounting_next;
    copy_limited(runtime->profile_name, sizeof(runtime->profile_name), scheduler->profile_name);
    copy_limited(runtime->owner_name, sizeof(runtime->owner_name), scheduler->owner_name);
    runtime->last_cycle = cycle;
    ++runtime->cycle_count;
    runtime->scheduled += plan->planned;
    runtime->applied += accounting->applied - applied_before;
    runtime->skipped += accounting->skipped - skipped_before;
    runtime->rejected += plan->rejected;
    runtime->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_cycle_summary(
    const espnow_bbs_mesh_contract_runtime_cycle_t *runtime,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (runtime == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_runtime_cycle_summary(
        runtime,
        scratch,
        sizeof(scratch),
        line_bytes
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_runtime_scheduler_init(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
)
{
    if (runtime_scheduler == NULL) {
        return;
    }
    memset(runtime_scheduler, 0, sizeof(*runtime_scheduler));
    espnow_bbs_mesh_contract_scheduler_state_init(&runtime_scheduler->scheduler);
    espnow_bbs_mesh_contract_dispatch_plan_init(&runtime_scheduler->plan);
    espnow_bbs_mesh_contract_execution_accounting_init(
        &runtime_scheduler->accounting
    );
    espnow_bbs_mesh_contract_runtime_cycle_init(&runtime_scheduler->runtime);
    runtime_scheduler->core_id =
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE;
    runtime_scheduler->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_runtime_scheduler_reset(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
)
{
    espnow_bbs_mesh_contract_runtime_scheduler_init(runtime_scheduler);
}

size_t espnow_bbs_mesh_contract_runtime_scheduler_step_count(
    const espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
)
{
    if (runtime_scheduler == NULL) {
        return 0u;
    }
    return runtime_scheduler->step_count;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_config_status(
    const espnow_bbs_mesh_contract_runtime_scheduler_config_t *config
)
{
    return runtime_scheduler_config_status(config);
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_configure(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler,
    const espnow_bbs_mesh_contract_runtime_scheduler_config_t *config
)
{
    espnow_bbs_mesh_contract_runtime_scheduler_t next;
    espnow_bbs_mesh_contract_status_t status;

    if (runtime_scheduler == NULL || config == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = runtime_scheduler_config_status(config);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    espnow_bbs_mesh_contract_runtime_scheduler_init(&next);
    copy_limited(next.owner_name, sizeof(next.owner_name), config->owner_name);
    copy_limited(next.profile_name, sizeof(next.profile_name), config->profile_name);
    copy_limited(
        next.invoke_policy,
        sizeof(next.invoke_policy),
        config->invoke_policy
    );
    copy_limited(next.task_policy, sizeof(next.task_policy), config->task_policy);
    copy_limited(
        next.timer_policy,
        sizeof(next.timer_policy),
        config->timer_policy
    );
    next.claim_token = config->claim_token;
    next.period_ticks = config->period_ticks;
    next.yield_interval = config->yield_interval;
    next.core_id = config->core_id;
    next.configured = true;
    next.last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    *runtime_scheduler = next;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_step(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t cycle
)
{
    espnow_bbs_mesh_contract_runtime_scheduler_t next;
    espnow_bbs_mesh_contract_status_t status;

    if (runtime_scheduler == NULL || owner == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (!runtime_scheduler->configured) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_NOT_CONFIGURED;
    }

    status = owner_claim_status(owner, runtime_scheduler->claim_token);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    if (strcmp(owner->owner_name, runtime_scheduler->owner_name) != 0) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_OWNER_MISMATCH;
    }

    next = *runtime_scheduler;
    status = espnow_bbs_mesh_contract_runtime_cycle_step(
        &next.runtime,
        &next.scheduler,
        &next.plan,
        &next.accounting,
        owner,
        next.claim_token,
        next.profile_name,
        cycle
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    ++next.step_count;
    next.scheduled = next.runtime.scheduled;
    next.applied = next.runtime.applied;
    next.skipped = next.runtime.skipped;
    next.rejected = next.runtime.rejected;
    if (next.yield_interval != 0u
        && next.step_count % next.yield_interval == 0u) {
        ++next.yield_due;
    }
    next.last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    *runtime_scheduler = next;
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_summary(
    const espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (runtime_scheduler == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_runtime_scheduler_summary(
        runtime_scheduler,
        scratch,
        sizeof(scratch),
        line_bytes
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_bridge_summary(
    const char *request_name,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (request_name == NULL || state == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = espnow_bbs_mesh_contract_request_status(request_name);
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    if (strcmp(request_name, "discovery_snapshot") == 0) {
        status = format_bridge_snapshot_summary(
            state,
            scratch,
            sizeof(scratch),
            line_bytes
        );
    } else if (strcmp(request_name, "discovery_events") == 0) {
        status = format_bridge_events_summary(
            state,
            scratch,
            sizeof(scratch),
            line_bytes
        );
    } else if (strcmp(request_name, "service_catalog") == 0) {
        status = format_bridge_service_catalog_summary(
            scratch,
            sizeof(scratch),
            line_bytes
        );
    } else {
        status = format_bridge_capability_summary(
            state,
            scratch,
            sizeof(scratch),
            line_bytes
        );
    }
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_bridge_request_router_init(
    espnow_bbs_mesh_contract_bridge_request_router_t *router
)
{
    if (router == NULL) {
        return;
    }
    memset(router, 0, sizeof(*router));
    router->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_bridge_request_router_reset(
    espnow_bbs_mesh_contract_bridge_request_router_t *router
)
{
    espnow_bbs_mesh_contract_bridge_request_router_init(router);
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_bridge_request_route(
    espnow_bbs_mesh_contract_bridge_request_router_t *router,
    const char *request_name,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    espnow_bbs_mesh_contract_status_t status;

    if (router == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    ++router->attempted;
    router->last_request[0] = '\0';

    status = espnow_bbs_mesh_contract_request_status(request_name);
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        copy_limited(
            router->last_request,
            sizeof(router->last_request),
            request_name
        );
        status = espnow_bbs_mesh_contract_bridge_summary(
            request_name,
            state,
            summary,
            summary_capacity,
            line_bytes
        );
    }

    router->last_status = status;
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        ++router->accepted;
    } else {
        ++router->rejected;
    }
    return status;
}

espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_request_router_summary(
    const espnow_bbs_mesh_contract_bridge_request_router_t *router,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (router == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_bridge_request_router_summary(
        router,
        scratch,
        sizeof(scratch),
        line_bytes
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope
)
{
    if (envelope == NULL) {
        return;
    }
    memset(envelope, 0, sizeof(*envelope));
    envelope->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_bridge_dispatch_envelope_reset(
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope
)
{
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(envelope);
}

espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope,
    espnow_bbs_mesh_contract_bridge_request_router_t *router,
    const char *request_name,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    espnow_bbs_mesh_contract_status_t request_status;
    espnow_bbs_mesh_contract_status_t status;

    if (envelope == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }

    ++envelope->attempted;
    envelope->last_request[0] = '\0';

    request_status = espnow_bbs_mesh_contract_request_status(request_name);
    if (request_status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        copy_limited(
            envelope->last_request,
            sizeof(envelope->last_request),
            request_name
        );
    }

    status = espnow_bbs_mesh_contract_bridge_request_route(
        router,
        request_name,
        state,
        summary,
        summary_capacity,
        line_bytes
    );
    envelope->last_status = status;
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        ++envelope->would_dispatch;
    } else {
        ++envelope->rejected;
    }
    return status;
}

espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary(
    const espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (envelope == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_bridge_dispatch_envelope_summary(
        envelope,
        scratch,
        sizeof(scratch),
        line_bytes
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_bridge_request_text_intent_init(
    espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent
)
{
    if (intent == NULL) {
        return;
    }
    memset(intent, 0, sizeof(*intent));
    intent->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_bridge_request_text_intent_reset(
    espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent
)
{
    espnow_bbs_mesh_contract_bridge_request_text_intent_init(intent);
}

espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_request_text_intent_route(
    espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent,
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope,
    espnow_bbs_mesh_contract_bridge_request_router_t *router,
    const char *request_text,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char normalized[
        ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 1u
    ];
    espnow_bbs_mesh_contract_status_t status;

    if (intent == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }

    ++intent->attempted;
    intent->normalized_request[0] = '\0';

    status = normalize_bridge_request_text_intent(
        request_text,
        normalized,
        sizeof(normalized)
    );
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        copy_limited(
            intent->normalized_request,
            sizeof(intent->normalized_request),
            normalized
        );
        status = espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            envelope,
            router,
            normalized,
            state,
            summary,
            summary_capacity,
            line_bytes
        );
    }

    intent->last_status = status;
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        ++intent->accepted;
    } else {
        ++intent->rejected;
    }
    return status;
}

espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_request_text_intent_summary(
    const espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (intent == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_bridge_request_text_intent_summary(
        intent,
        scratch,
        sizeof(scratch),
        line_bytes
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_persistence_intent_init(
    espnow_bbs_mesh_contract_persistence_intent_t *persistence
)
{
    if (persistence == NULL) {
        return;
    }
    memset(persistence, 0, sizeof(*persistence));
    persistence->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

void espnow_bbs_mesh_contract_persistence_intent_reset(
    espnow_bbs_mesh_contract_persistence_intent_t *persistence
)
{
    espnow_bbs_mesh_contract_persistence_intent_init(persistence);
}

espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_persistence_intent_stage(
    espnow_bbs_mesh_contract_persistence_intent_t *persistence,
    espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent,
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope,
    espnow_bbs_mesh_contract_bridge_request_router_t *router,
    const char *request_text,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t routed_line_bytes = 0u;
    size_t *route_line_bytes = line_bytes != NULL ? line_bytes : &routed_line_bytes;
    espnow_bbs_mesh_contract_status_t status;

    if (persistence == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }

    ++persistence->attempted;
    persistence->last_request[0] = '\0';

    if (intent == NULL || envelope == NULL || router == NULL || summary == NULL) {
        persistence->last_status = ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
        ++persistence->rejected;
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }

    status = espnow_bbs_mesh_contract_bridge_request_text_intent_route(
        intent,
        envelope,
        router,
        request_text,
        state,
        summary,
        summary_capacity,
        route_line_bytes
    );
    if (intent->normalized_request[0] != '\0') {
        copy_limited(
            persistence->last_request,
            sizeof(persistence->last_request),
            intent->normalized_request
        );
    }

    persistence->last_status = status;
    if (status == ESPNOW_BBS_MESH_CONTRACT_OK) {
        ++persistence->staged;
        persistence->staged_bytes += *route_line_bytes;
    } else {
        ++persistence->rejected;
    }
    return status;
}

espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_persistence_intent_summary(
    const espnow_bbs_mesh_contract_persistence_intent_t *persistence,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    char scratch[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    espnow_bbs_mesh_contract_status_t status;
    size_t bytes;

    if (persistence == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    status = format_persistence_intent_summary(
        persistence,
        scratch,
        sizeof(scratch),
        line_bytes
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }
    bytes = strlen(scratch);
    if (summary_capacity == 0u || bytes + 1u > summary_capacity) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY;
    }
    memcpy(summary, scratch, bytes + 1u);
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

size_t espnow_bbs_mesh_contract_request_count(void)
{
    return sizeof(DISCOVERY_REQUESTS) / sizeof(DISCOVERY_REQUESTS[0]);
}

const char *espnow_bbs_mesh_contract_request_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_request_count()) {
        return NULL;
    }
    return DISCOVERY_REQUESTS[index];
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_request_status(
    const char *request_name
)
{
    if (request_name == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_contract_is_ascii(request_name)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII;
    }
    if (!string_in_table(
            request_name,
            DISCOVERY_REQUESTS,
            espnow_bbs_mesh_contract_request_count()
        )) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

size_t espnow_bbs_mesh_contract_bridge_request_count(void)
{
    return sizeof(GATE_F_BRIDGE_REQUESTS) / sizeof(GATE_F_BRIDGE_REQUESTS[0]);
}

const char *espnow_bbs_mesh_contract_bridge_request_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_bridge_request_count()) {
        return NULL;
    }
    return GATE_F_BRIDGE_REQUESTS[index];
}

size_t espnow_bbs_mesh_contract_gate_f_service_count(void)
{
    return sizeof(GATE_F_SERVICE_CODES) / sizeof(GATE_F_SERVICE_CODES[0]);
}

const char *espnow_bbs_mesh_contract_gate_f_service_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_gate_f_service_count()) {
        return NULL;
    }
    return GATE_F_SERVICE_CODES[index].name;
}

int espnow_bbs_mesh_contract_gate_f_service_code(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_gate_f_service_count()) {
        return -1;
    }
    return GATE_F_SERVICE_CODES[index].code;
}

size_t espnow_bbs_mesh_contract_secret_marker_count(void)
{
    return sizeof(SECRET_FIELD_MARKERS) / sizeof(SECRET_FIELD_MARKERS[0]);
}

const char *espnow_bbs_mesh_contract_secret_marker_name(size_t index)
{
    if (index >= espnow_bbs_mesh_contract_secret_marker_count()) {
        return NULL;
    }
    return SECRET_FIELD_MARKERS[index];
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_field_name_status(
    const char *field_name
)
{
    if (field_name == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_contract_is_ascii(field_name)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII;
    }
    if (normalized_secret_match(field_name)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_field_path_status(
    const char *const *field_names,
    size_t field_count
)
{
    size_t index;

    if (field_names == NULL && field_count != 0u) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    for (index = 0; index < field_count; ++index) {
        espnow_bbs_mesh_contract_status_t status =
            espnow_bbs_mesh_contract_field_name_status(field_names[index]);
        if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
            return status;
        }
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

bool espnow_bbs_mesh_contract_live_action_blocked(const char *request_name)
{
    return string_in_table(
        request_name,
        BLOCKED_LIVE_ACTIONS,
        sizeof(BLOCKED_LIVE_ACTIONS) / sizeof(BLOCKED_LIVE_ACTIONS[0])
    );
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_live_action_status(
    const char *request_name
)
{
    if (request_name == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_contract_is_ascii(request_name)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII;
    }
    if (espnow_bbs_mesh_contract_live_action_blocked(request_name)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_LIVE_ACTION;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}

bool espnow_bbs_mesh_contract_is_ascii(const char *value)
{
    size_t index;

    if (value == NULL) {
        return false;
    }
    for (index = 0; value[index] != '\0'; ++index) {
        if ((unsigned char)value[index] > 0x7f) {
            return false;
        }
    }
    return true;
}

espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_summary_status(
    const char *summary,
    size_t *line_bytes
)
{
    size_t bytes;

    if (summary == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_contract_is_ascii(summary)) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII;
    }
    bytes = strlen(summary);
    if (line_bytes != NULL) {
        *line_bytes = bytes;
    }
    if (bytes > ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_LINE_TOO_LONG;
    }
    return ESPNOW_BBS_MESH_CONTRACT_OK;
}
