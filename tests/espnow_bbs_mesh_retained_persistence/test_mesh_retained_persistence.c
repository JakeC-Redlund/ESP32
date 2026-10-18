#include <stdint.h>
#include <string.h>

#include "espnow_bbs_mesh_retained_persistence.h"
#include "test_support.h"

static bool bytes_contain(const uint8_t *haystack, size_t haystack_len, const char *needle);
static void build_persistence_fixture(
    espnow_bbs_mesh_contract_persistence_intent_t *persistence,
    espnow_bbs_mesh_contract_event_queue_t *queue,
    espnow_bbs_mesh_contract_owner_t *owner,
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
);
static void test_retained_checkpoint_round_trips_m4t_fields(void);
static void test_retained_recovery_rejects_bad_records(void);
static void test_retained_record_rejects_unsafe_fields(void);
static void test_retained_storage_host_stub_handles_missing_save_load_erase(void);

#ifdef ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_HOST_TEST
void espnow_bbs_mesh_retained_persistence_host_seed_blob_length(size_t length);
#endif

int main(void)
{
    test_retained_checkpoint_round_trips_m4t_fields();
    test_retained_recovery_rejects_bad_records();
    test_retained_record_rejects_unsafe_fields();
    test_retained_storage_host_stub_handles_missing_save_load_erase();
    return finish_tests("espnow_bbs_mesh_retained_persistence");
}

static bool bytes_contain(const uint8_t *haystack, size_t haystack_len, const char *needle)
{
    size_t needle_len = strlen(needle);
    size_t index;

    if (needle_len == 0u || needle_len > haystack_len) {
        return false;
    }
    for (index = 0u; index + needle_len <= haystack_len; ++index) {
        if (memcmp(haystack + index, needle, needle_len) == 0) {
            return true;
        }
    }
    return false;
}

static void build_persistence_fixture(
    espnow_bbs_mesh_contract_persistence_intent_t *persistence,
    espnow_bbs_mesh_contract_event_queue_t *queue,
    espnow_bbs_mesh_contract_owner_t *owner,
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
)
{
    espnow_bbs_mesh_contract_bridge_summary_state_t state;
    espnow_bbs_mesh_contract_bridge_request_router_t router;
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t envelope;
    espnow_bbs_mesh_contract_bridge_request_text_intent_t intent;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config = {
        "mesh_ingress",
        "balanced",
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY,
        0u,
        1u,
        2u,
        0u,
        0u,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE,
        false,
    };
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "n01",
        "retained",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t claim_token = 0u;

    espnow_bbs_mesh_contract_event_queue_init(queue);
    espnow_bbs_mesh_contract_owner_init(owner, queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(owner, "mesh_ingress", &claim_token) ==
            ESPNOW_BBS_MESH_CONTRACT_OK,
        "retained persistence fixture owner claim accepts"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(owner, claim_token, &event) ==
            ESPNOW_BBS_MESH_CONTRACT_OK,
        "retained persistence fixture enqueues event"
    );
    espnow_bbs_mesh_contract_runtime_scheduler_init(runtime_scheduler);
    config.claim_token = claim_token;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "retained persistence fixture scheduler config accepts"
    );
    state.event_queue = queue;
    state.owner = owner;
    state.runtime_scheduler = runtime_scheduler;
    espnow_bbs_mesh_contract_bridge_request_router_init(&router);
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(&envelope);
    espnow_bbs_mesh_contract_bridge_request_text_intent_init(&intent);
    espnow_bbs_mesh_contract_persistence_intent_init(persistence);
    check(
        espnow_bbs_mesh_contract_persistence_intent_stage(
            persistence,
            &intent,
            &envelope,
            &router,
            " discovery_snapshot ",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "retained persistence fixture stages M4T output"
    );
    check(
        !bytes_contain(
            (const uint8_t *)summary,
            strlen(summary),
            "secret"
        ),
        "retained persistence fixture summary has no secret marker"
    );
}

static void test_retained_checkpoint_round_trips_m4t_fields(void)
{
    espnow_bbs_mesh_contract_persistence_intent_t persistence;
    espnow_bbs_mesh_contract_persistence_intent_t recovered;
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_retained_persistence_checkpoint_t checkpoint;
    uint8_t record[ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES];
    char summary[ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SUMMARY_MAX_BYTES];
    size_t record_bytes = 0u;
    size_t line_bytes = 0u;
    size_t queue_count_before;
    size_t owner_accepted_before;
    size_t runtime_steps_before;

    build_persistence_fixture(&persistence, &queue, &owner, &runtime_scheduler);
    queue_count_before = queue.count;
    owner_accepted_before = owner.accepted;
    runtime_steps_before = runtime_scheduler.step_count;
    check(
        espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
            &checkpoint,
            &persistence
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained checkpoint builds from M4T intent"
    );
    check(
        espnow_bbs_mesh_retained_persistence_encode(
            &checkpoint,
            record,
            sizeof(record),
            &record_bytes
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained checkpoint encodes"
    );
    check(
        record_bytes == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES,
        "retained checkpoint record size is stable"
    );
    check(
        bytes_contain(record, record_bytes, ESPNOW_BBS_MESH_DISCOVERY_SCHEMA),
        "retained record carries schema identity"
    );
    check(
        bytes_contain(record, record_bytes, "discovery_snapshot"),
        "retained record carries last normalized request"
    );
    check(
        !bytes_contain(record, record_bytes, "node_seen"),
        "retained record does not retain generated summary payload"
    );
    check(
        !bytes_contain(record, record_bytes, "serial"),
        "retained record does not retain serial bytes"
    );
    check(
        espnow_bbs_mesh_retained_persistence_recover(
            &recovered,
            record,
            record_bytes
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained record recovers into volatile M4T state"
    );
    check(recovered.attempted == persistence.attempted, "retained recover restores attempted");
    check(recovered.staged == persistence.staged, "retained recover restores staged");
    check(recovered.rejected == persistence.rejected, "retained recover restores rejected");
    check(recovered.staged_bytes == persistence.staged_bytes, "retained recover restores byte total");
    check(
        strcmp(recovered.last_request, "discovery_snapshot") == 0,
        "retained recover restores last request"
    );
    check(queue.count == queue_count_before, "retained recover does not mutate queue");
    check(owner.accepted == owner_accepted_before, "retained recover does not mutate owner");
    check(
        runtime_scheduler.step_count == runtime_steps_before,
        "retained recover does not step runtime scheduler"
    );
    check(
        espnow_bbs_mesh_retained_persistence_summary(
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
            &recovered,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained recovery summary accepts"
    );
    check(line_bytes < sizeof(summary), "retained recovery summary is bounded");
    check(
        strstr(summary, "storage=nvs") != NULL &&
            strstr(summary, "policy=caller_triggered") != NULL &&
            strstr(summary, "reboot=unproved") != NULL &&
            strstr(summary, "live=blocked") != NULL &&
            strstr(summary, "serial=closed") != NULL &&
            strstr(summary, "abi=closed") != NULL,
        "retained summary preserves closed surfaces"
    );
}

static void test_retained_recovery_rejects_bad_records(void)
{
    espnow_bbs_mesh_contract_persistence_intent_t persistence;
    espnow_bbs_mesh_contract_persistence_intent_t recovered;
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_retained_persistence_checkpoint_t checkpoint;
    uint8_t record[ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES];
    size_t record_bytes = 0u;

    build_persistence_fixture(&persistence, &queue, &owner, &runtime_scheduler);
    check(
        espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
            &checkpoint,
            &persistence
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained bad-record fixture checkpoint builds"
    );
    check(
        espnow_bbs_mesh_retained_persistence_encode(
            &checkpoint,
            record,
            sizeof(record),
            &record_bytes
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained bad-record fixture encodes"
    );

    recovered.attempted = 99u;
    check(
        espnow_bbs_mesh_retained_persistence_recover(&recovered, NULL, 0u) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING,
        "missing retained record recovers as volatile empty"
    );
    check(recovered.attempted == 0u, "missing retained record clears attempted");

    record[32u] ^= 0x01u;
    recovered.attempted = 99u;
    check(
        espnow_bbs_mesh_retained_persistence_recover(
            &recovered,
            record,
            record_bytes
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CHECKSUM,
        "corrupt retained record rejects by checksum"
    );
    check(recovered.attempted == 0u, "corrupt retained record clears attempted");
    record[32u] ^= 0x01u;

    record[4u] = 2u;
    recovered.attempted = 99u;
    check(
        espnow_bbs_mesh_retained_persistence_recover(
            &recovered,
            record,
            record_bytes
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_VERSION,
        "unsupported retained record version rejects"
    );
    check(recovered.attempted == 0u, "unsupported retained record clears attempted");
}

static void test_retained_record_rejects_unsafe_fields(void)
{
    espnow_bbs_mesh_contract_persistence_intent_t persistence;
    espnow_bbs_mesh_retained_persistence_checkpoint_t checkpoint;

    espnow_bbs_mesh_contract_persistence_intent_init(&persistence);
    persistence.attempted = 1u;
    persistence.staged = 1u;
    persistence.staged_bytes = 10u;
    persistence.last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
    (void)strcpy(persistence.last_request, "unknown_request");
    check(
        espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
            &checkpoint,
            &persistence
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_UNKNOWN_REQUEST,
        "retained checkpoint rejects unknown request"
    );
    (void)strcpy(persistence.last_request, "pairing_token");
    check(
        espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
            &checkpoint,
            &persistence
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SECRET_FIELD,
        "retained checkpoint rejects secret-like request field"
    );
    persistence.last_request[0] = 'd';
    persistence.last_request[1] = (char)0xff;
    persistence.last_request[2] = '\0';
    check(
        espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
            &checkpoint,
            &persistence
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NON_ASCII,
        "retained checkpoint rejects non-ASCII request"
    );
    persistence.last_request[0] = '\0';
    persistence.staged = 2u;
    check(
        espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
            &checkpoint,
            &persistence
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE,
        "retained checkpoint rejects impossible counters"
    );
    persistence.staged = 0u;
    persistence.attempted = (size_t)UINT32_MAX + 1u;
    check(
        espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
            &checkpoint,
            &persistence
        ) == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE,
        "retained checkpoint rejects counters outside record range"
    );
}

static void test_retained_storage_host_stub_handles_missing_save_load_erase(void)
{
    espnow_bbs_mesh_contract_persistence_intent_t persistence;
    espnow_bbs_mesh_contract_persistence_intent_t recovered;
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_retained_persistence_storage_t storage;

    build_persistence_fixture(&persistence, &queue, &owner, &runtime_scheduler);
    espnow_bbs_mesh_retained_persistence_storage_context_init(&storage);
    check(
        espnow_bbs_mesh_retained_persistence_storage_load(&storage, &recovered) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED,
        "retained storage rejects load before init"
    );
    check(
        espnow_bbs_mesh_retained_persistence_storage_init(&storage) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained storage init accepts"
    );
    check(storage.initialized, "retained storage init marks initialized");
    recovered.attempted = 55u;
    check(
        espnow_bbs_mesh_retained_persistence_storage_load(&storage, &recovered) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING,
        "retained storage load reports missing record"
    );
    check(recovered.attempted == 0u, "retained missing storage load clears volatile state");
    check(
        espnow_bbs_mesh_retained_persistence_storage_save(&storage, &persistence) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained storage save commits caller-triggered checkpoint"
    );
    check(storage.commit_count == 1u, "retained storage save records one commit");
    check(
        espnow_bbs_mesh_retained_persistence_storage_load(&storage, &recovered) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained storage load recovers saved checkpoint"
    );
    check(
        strcmp(recovered.last_request, "discovery_snapshot") == 0,
        "retained storage load restores request"
    );
#ifdef ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_HOST_TEST
    recovered.attempted = 77u;
    espnow_bbs_mesh_retained_persistence_host_seed_blob_length(
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES + 1u
    );
    check(
        espnow_bbs_mesh_retained_persistence_storage_load(&storage, &recovered) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_LENGTH,
        "retained storage load rejects wrong-length NVS blob"
    );
    check(
        recovered.attempted == 0u,
        "retained wrong-length NVS blob clears volatile state"
    );
#endif
    check(
        espnow_bbs_mesh_retained_persistence_storage_erase(&storage) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained storage erase accepts existing checkpoint"
    );
    check(storage.commit_count == 2u, "retained storage erase records commit");
    check(
        espnow_bbs_mesh_retained_persistence_storage_load(&storage, &recovered) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING,
        "retained storage load reports missing after erase"
    );
    check(
        espnow_bbs_mesh_retained_persistence_storage_deinit(&storage) ==
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK,
        "retained storage deinit accepts"
    );
    check(!storage.initialized, "retained storage deinit clears initialized");
}
