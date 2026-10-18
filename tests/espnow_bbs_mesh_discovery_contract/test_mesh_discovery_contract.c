#include "espnow_bbs_mesh_discovery_contract.h"
#include "test_support.h"

#include <string.h>

static espnow_bbs_mesh_contract_runtime_scheduler_config_t runtime_scheduler_valid_config(
    size_t claim_token
)
{
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config = {
        "mesh_ingress",
        "balanced",
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY,
        claim_token,
        1u,
        2u,
        0u,
        0u,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE,
        false,
    };
    return config;
}

static void test_schema_defaults_and_summary_budget(void)
{
    char exact[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char too_long[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 2u];
    char non_ascii[] = {'o', 'k', (char)0xff, '\0'};
    size_t line_bytes = 0;

    memset(exact, 'a', sizeof(exact) - 1u);
    exact[sizeof(exact) - 1u] = '\0';
    memset(too_long, 'b', sizeof(too_long) - 1u);
    too_long[sizeof(too_long) - 1u] = '\0';

    check(
        strcmp(ESPNOW_BBS_MESH_DISCOVERY_SCHEMA, "mesh_discovery.v1") == 0,
        "schema is mesh_discovery.v1"
    );
    check(
        strcmp(ESPNOW_BBS_MESH_DISCOVERY_TRANSPORT, "esp-wifi-mesh") == 0,
        "transport is esp-wifi-mesh"
    );
    check(
        strcmp(ESPNOW_BBS_MESH_DISCOVERY_MODE, "sim") == 0,
        "mode is sim"
    );
    check(
        strcmp(ESPNOW_BBS_MESH_DISCOVERY_ADMIN_GATE, "disabled") == 0,
        "admin gate is disabled"
    );
    check(
        ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES == 512u,
        "bridge line budget is 512 bytes"
    );
    check(
        espnow_bbs_mesh_contract_summary_status(exact, &line_bytes)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "512-byte summary fits"
    );
    check(line_bytes == 512u, "summary byte count returned");
    check(
        espnow_bbs_mesh_contract_summary_status(too_long, NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_LINE_TOO_LONG,
        "513-byte summary rejects"
    );
    check(
        espnow_bbs_mesh_contract_summary_status(non_ascii, NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII summary rejects"
    );
    check(
        strcmp(
            espnow_bbs_mesh_contract_status_name(
                ESPNOW_BBS_MESH_CONTRACT_REJECT_LINE_TOO_LONG
            ),
            "line_too_long"
        ) == 0,
        "status names are stable"
    );
}

static void test_event_vocabulary_and_labels(void)
{
    const char *const names[] = {
        "node_seen",
        "node_lost",
        "parent_selected",
        "root_elected",
        "root_switched",
        "route_added",
        "route_removed",
        "heal_started",
        "heal_observed",
        "service_seen",
        "capability_seen",
        "ble_client_seen",
    };
    const char *const labels[] = {
        "ns",
        "nl",
        "ps",
        "re",
        "rs",
        "ra",
        "rr",
        "hs",
        "ho",
        "ss",
        "cs",
        "bs",
    };
    size_t index;

    check(
        espnow_bbs_mesh_contract_event_count()
            == sizeof(names) / sizeof(names[0]),
        "event count matches host fixture"
    );
    for (index = 0; index < espnow_bbs_mesh_contract_event_count(); ++index) {
        check(
            strcmp(espnow_bbs_mesh_contract_event_name(index), names[index]) == 0,
            "event name order matches host fixture"
        );
        check(
            strcmp(espnow_bbs_mesh_contract_event_label(names[index]), labels[index]) == 0,
            "event compact label matches host fixture"
        );
        check(
            espnow_bbs_mesh_contract_event_status(names[index])
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known event accepts"
        );
    }
    check(
        espnow_bbs_mesh_contract_event_status("route_rewrite")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_EVENT,
        "unknown event rejects"
    );
    check(
        espnow_bbs_mesh_contract_event_status("node_seen\xff")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII event rejects"
    );
}

static void test_event_queue_fifo_and_caller_owned_state(void)
{
    const char *const names[] = {
        "node_seen",
        "node_lost",
        "parent_selected",
        "root_elected",
        "root_switched",
        "route_added",
        "route_removed",
        "heal_started",
        "heal_observed",
        "service_seen",
        "capability_seen",
        "ble_client_seen",
    };
    const char *const labels[] = {
        "ns",
        "nl",
        "ps",
        "re",
        "rs",
        "ra",
        "rr",
        "hs",
        "ho",
        "ss",
        "cs",
        "bs",
    };
    const char *const nodes[] = {
        "n00",
        "n01",
        "n02",
        "n03",
        "n04",
        "n05",
        "n06",
        "n07",
        "n08",
        "n09",
        "n10",
        "n11",
    };
    espnow_bbs_mesh_contract_event_queue_t queue_a;
    espnow_bbs_mesh_contract_event_queue_t queue_b;
    espnow_bbs_mesh_contract_event_record_t record;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue_a);
    espnow_bbs_mesh_contract_event_queue_init(&queue_b);
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue_a) == 0u,
        "caller queue initializes empty"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_dropped(&queue_a) == 0u,
        "caller queue initializes drop counter"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_pop(&queue_a, &record)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY,
        "empty queue pop rejects"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_peek(&queue_a, 0u, &record)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY,
        "empty queue peek rejects"
    );

    for (index = 0u; index < sizeof(names) / sizeof(names[0]); ++index) {
        espnow_bbs_mesh_contract_event_input_t event = {
            names[index],
            nodes[index],
            "host detail",
        };
        check(
            espnow_bbs_mesh_contract_event_queue_push(&queue_a, &event)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known event enqueues"
        );
    }
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue_a)
            == sizeof(names) / sizeof(names[0]),
        "queue count tracks accepted events"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue_b) == 0u,
        "second caller queue remains independent"
    );

    for (index = 0u; index < sizeof(names) / sizeof(names[0]); ++index) {
        check(
            espnow_bbs_mesh_contract_event_queue_peek(&queue_a, index, &record)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "queue peek by offset accepts"
        );
        check(strcmp(record.event_name, names[index]) == 0, "peek preserves event name");
        check(strcmp(record.event_label, labels[index]) == 0, "peek preserves label");
        check(strcmp(record.node_id, nodes[index]) == 0, "peek preserves node");
        check(strcmp(record.detail, "host detail") == 0, "peek preserves detail");
        check(record.sequence == index + 1u, "peek preserves sequence");
    }

    for (index = 0u; index < sizeof(names) / sizeof(names[0]); ++index) {
        check(
            espnow_bbs_mesh_contract_event_queue_pop(&queue_a, &record)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "queue pop accepts in FIFO order"
        );
        check(strcmp(record.event_name, names[index]) == 0, "pop preserves FIFO name");
        check(record.sequence == index + 1u, "pop preserves FIFO sequence");
    }
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue_a) == 0u,
        "queue count returns to empty after pops"
    );
}

static void test_event_queue_capacity_and_wraparound(void)
{
    const char *const nodes[] = {
        "c00",
        "c01",
        "c02",
        "c03",
        "c04",
        "c05",
        "c06",
        "c07",
        "c08",
        "c09",
        "c10",
        "c11",
        "c12",
        "c13",
        "c14",
        "c15",
    };
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_event_record_t record;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "c00",
        "capacity",
    };
    espnow_bbs_mesh_contract_event_input_t overflow = {
        "route_added",
        "full",
        "overflow",
    };
    espnow_bbs_mesh_contract_event_input_t wrap = {
        "route_removed",
        "wrap",
        "reuse",
    };
    size_t index;

    check(
        ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY == 16u,
        "queue capacity constant is 16"
    );
    espnow_bbs_mesh_contract_event_queue_init(&queue);
    for (index = 0u;
         index < ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
         ++index) {
        event.node_id = nodes[index];
        check(
            espnow_bbs_mesh_contract_event_queue_push(&queue, &event)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "queue accepts until fixed capacity"
        );
    }
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue)
            == ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY,
        "queue reports fixed capacity count"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &overflow)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL,
        "full queue rejects with backpressure"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_dropped(&queue) == 1u,
        "full queue increments explicit drop counter"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue)
            == ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY,
        "full rejection does not change count"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_pop(&queue, &record)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "pop releases one queue slot"
    );
    check(strcmp(record.node_id, "c00") == 0, "first capacity item pops first");
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &wrap)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "queue reuses wrapped slot after pop"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_peek(
            &queue,
            ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY - 1u,
            &record
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "wrapped queue peeks newest record"
    );
    check(strcmp(record.event_label, "rr") == 0, "wrapped record preserves label");
    check(strcmp(record.node_id, "wrap") == 0, "wrapped record preserves node");
    check(record.sequence == 17u, "wrapped record preserves monotonic sequence");
    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 0u,
        "queue init resets count"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_dropped(&queue) == 0u,
        "queue init resets drop counter"
    );
}

static void test_event_queue_input_rejection(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_event_input_t valid = {
        "node_seen",
        "12345678",
        "12345678901234567890123456789012",
    };
    espnow_bbs_mesh_contract_event_input_t unknown = {
        "route_rewrite",
        "node",
        "detail",
    };
    char non_ascii_event[] = {'n', 'o', 'd', 'e', '_', 's', 'e', 'e', 'n', (char)0xff, '\0'};
    char non_ascii_node[] = {'n', (char)0xff, '\0'};
    char non_ascii_detail[] = {'d', (char)0xff, '\0'};

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(NULL, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null queue push rejects"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null event push rejects"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "max-size node and detail accept"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &unknown)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_EVENT,
        "unknown event rejects at queue ingress"
    );

    valid.event_name = non_ascii_event;
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII event rejects at queue ingress"
    );
    valid.event_name = "node_seen";
    valid.node_id = non_ascii_node;
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII node rejects at queue ingress"
    );
    valid.node_id = "123456789";
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NODE_TOO_LONG,
        "over-limit node rejects at queue ingress"
    );
    valid.node_id = "node";
    valid.detail = non_ascii_detail;
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII detail rejects at queue ingress"
    );
    valid.detail = "123456789012345678901234567890123";
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_DETAIL_TOO_LONG,
        "over-limit detail rejects at queue ingress"
    );
    valid.detail = NULL;
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null detail rejects at queue ingress"
    );
    valid.node_id = NULL;
    valid.detail = "detail";
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &valid)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null node rejects at queue ingress"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 1u,
        "rejected queue inputs do not enqueue"
    );
    check(
        strcmp(
            espnow_bbs_mesh_contract_status_name(
                ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL
            ),
            "queue_full"
        ) == 0,
        "queue status names are stable"
    );
}

static void test_event_queue_summary_bounds(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_event_input_t first = {
        "node_seen",
        "oldnode",
        "first",
    };
    espnow_bbs_mesh_contract_event_input_t second = {
        "route_added",
        "newnode",
        "second",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char exact[128];
    char too_small[12];
    size_t line_bytes = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_summary(
            &queue,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "empty queue summary accepts"
    );
    check(
        strcmp(summary, "schema=mesh_discovery.v1;count=0;dropped=0") == 0,
        "empty queue summary is stable"
    );
    check(line_bytes == strlen(summary), "empty summary byte count returns");

    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &first)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "summary fixture first event accepts"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &second)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "summary fixture second event accepts"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_summary(
            &queue,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "non-empty queue summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;count=2;dropped=0;old=ns:oldnode;new=ra:newnode"
        ) == 0,
        "queue summary snapshots oldest and newest"
    );
    check(line_bytes == strlen(summary), "non-empty summary byte count returns");
    check(
        line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
        "queue summary stays inside 512-byte bridge budget"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_summary(
            &queue,
            exact,
            line_bytes + 1u,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "queue summary accepts exact caller capacity"
    );
    check(strcmp(exact, summary) == 0, "exact-capacity summary copies output");
    check(
        espnow_bbs_mesh_contract_event_queue_summary(
            &queue,
            too_small,
            sizeof(too_small),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "queue summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_summary(&queue, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null queue summary output rejects"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null queue summary input rejects"
    );
}

static void test_owner_names_and_claim_release(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    size_t claim = 0u;
    size_t second_claim = 0u;
    char long_name[] = "mesh_owner_name_too_long";
    char non_ascii_name[] = {'m', 'e', 's', 'h', '_', (char)0xff, '\0'};

    check(
        ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES == 16u,
        "owner name capacity constant is 16"
    );
    check(
        espnow_bbs_mesh_contract_owner_name_count() == 3u,
        "owner role count is stable"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_owner_name(0u), "mesh_ingress") == 0,
        "owner role name 0 is stable"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_owner_name(1u), "mesh_drain") == 0,
        "owner role name 1 is stable"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_owner_name(2u), "mesh_snapshot") == 0,
        "owner role name 2 is stable"
    );
    check(
        espnow_bbs_mesh_contract_owner_name(3u) == NULL,
        "owner role out-of-range returns null"
    );
    check(
        espnow_bbs_mesh_contract_owner_name_status(NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null owner role rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_name_status(non_ascii_name)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII owner role rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_name_status(long_name)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NAME_TOO_LONG,
        "over-limit owner role rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_name_status("mesh_runtime")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_OWNER,
        "unknown owner role rejects"
    );

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, NULL);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND,
        "unbound owner claim rejects"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(owner.state == ESPNOW_BBS_MESH_CONTRACT_OWNER_RELEASED, "owner initializes released");
    check(owner.next_claim_token == 1u, "owner initializes next claim token");
    check(owner.accepted == 0u && owner.rejected == 0u && owner.drained == 0u, "owner counters initialize");
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner claim accepts known role"
    );
    check(claim == 1u, "owner claim returns token");
    check(owner.state == ESPNOW_BBS_MESH_CONTRACT_OWNER_CLAIMED, "owner becomes claimed");
    check(strcmp(owner.owner_name, "mesh_ingress") == 0, "owner stores role name");
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_drain", &second_claim)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_ALREADY_CLAIMED,
        "double owner claim rejects"
    );
    check(second_claim == 0u, "double owner claim does not overwrite token output");
    check(
        espnow_bbs_mesh_contract_owner_release(&owner, claim + 1u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE,
        "stale owner release rejects"
    );
    check(owner.state == ESPNOW_BBS_MESH_CONTRACT_OWNER_CLAIMED, "stale release keeps owner claimed");
    check(
        espnow_bbs_mesh_contract_owner_release(&owner, claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner release accepts active token"
    );
    check(owner.state == ESPNOW_BBS_MESH_CONTRACT_OWNER_RELEASED, "owner release clears claimed state");
    check(owner.owner_name[0] == '\0', "owner release clears role name");
    check(
        espnow_bbs_mesh_contract_owner_release(&owner, claim)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED,
        "double owner release rejects"
    );

    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_drain", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner can be claimed after release"
    );
    check(claim == 2u, "owner claim token advances");
    owner.accepted = 4u;
    owner.rejected = 3u;
    owner.drained = 2u;
    espnow_bbs_mesh_contract_owner_reset(&owner);
    check(owner.state == ESPNOW_BBS_MESH_CONTRACT_OWNER_RELEASED, "owner reset releases state");
    check(owner.next_claim_token == 1u, "owner reset restarts claim token");
    check(owner.accepted == 0u && owner.rejected == 0u && owner.drained == 0u, "owner reset clears counters");
    check(owner.queue == &queue, "owner reset preserves caller queue binding");
}

static void test_owner_ingest_drain_and_counters(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_event_record_t record;
    espnow_bbs_mesh_contract_event_input_t first = {
        "node_seen",
        "node1",
        "first",
    };
    espnow_bbs_mesh_contract_event_input_t second = {
        "route_added",
        "node2",
        "second",
    };
    espnow_bbs_mesh_contract_event_input_t unknown = {
        "route_rewrite",
        "node3",
        "bad",
    };
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, 1u, &first)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED,
        "owner ingest before claim rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner ingest fixture claims role"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim + 1u, &first)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE,
        "stale owner ingest rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim, &first)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner accepted event ingress"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim, &second)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner accepted second event ingress"
    );
    check(owner.accepted == 2u, "owner accepted counter increments");
    check(owner.rejected == 0u, "owner rejected counter remains zero after accepted ingress");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 2u,
        "owner ingress writes to caller queue"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim, &unknown)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_EVENT,
        "owner rejects unknown event ingress"
    );
    check(owner.rejected == 1u, "owner rejected counter increments");
    check(owner.last_status == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_EVENT, "owner tracks last reject status");

    check(
        espnow_bbs_mesh_contract_owner_drain(&owner, claim + 1u, &record)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE,
        "stale owner drain rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_drain(&owner, claim, &record)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner drain accepts"
    );
    check(strcmp(record.event_name, "node_seen") == 0, "owner drain preserves FIFO first");
    check(owner.drained == 1u, "owner drained counter increments");
    check(
        espnow_bbs_mesh_contract_owner_drain(&owner, claim, &record)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner drain accepts second event"
    );
    check(strcmp(record.event_name, "route_added") == 0, "owner drain preserves FIFO second");
    check(owner.drained == 2u, "owner drained counter increments again");
    check(
        espnow_bbs_mesh_contract_owner_drain(&owner, claim, &record)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY,
        "owner drain empty queue rejects"
    );
    check(owner.drained == 2u, "owner empty drain does not increment counter");
}

static void test_owner_capacity_and_independence(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue_a;
    espnow_bbs_mesh_contract_event_queue_t queue_b;
    espnow_bbs_mesh_contract_owner_t owner_a;
    espnow_bbs_mesh_contract_owner_t owner_b;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "o00",
        "capacity",
    };
    espnow_bbs_mesh_contract_event_input_t overflow = {
        "node_lost",
        "full",
        "overflow",
    };
    const char *const nodes[] = {
        "o00",
        "o01",
        "o02",
        "o03",
        "o04",
        "o05",
        "o06",
        "o07",
        "o08",
        "o09",
        "o10",
        "o11",
        "o12",
        "o13",
        "o14",
        "o15",
    };
    size_t claim_a = 0u;
    size_t claim_b = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue_a);
    espnow_bbs_mesh_contract_event_queue_init(&queue_b);
    espnow_bbs_mesh_contract_owner_init(&owner_a, &queue_a);
    espnow_bbs_mesh_contract_owner_init(&owner_b, &queue_b);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner_a, "mesh_ingress", &claim_a)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner A claims role"
    );
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner_b, "mesh_drain", &claim_b)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner B claims role"
    );
    for (index = 0u;
         index < ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
         ++index) {
        event.node_id = nodes[index];
        check(
            espnow_bbs_mesh_contract_owner_ingest(&owner_a, claim_a, &event)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "owner accepts until caller queue capacity"
        );
    }
    check(owner_a.accepted == 16u, "owner accepted counter reaches capacity");
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner_a, claim_a, &overflow)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL,
        "owner surfaces caller queue full backpressure"
    );
    check(owner_a.rejected == 1u, "owner rejected counter increments on full queue");
    check(queue_a.dropped == 1u, "caller queue drop counter increments on owner overflow");
    check(owner_b.accepted == 0u && owner_b.rejected == 0u, "independent owner counters remain unchanged");
    check(espnow_bbs_mesh_contract_event_queue_count(&queue_b) == 0u, "independent caller queue remains empty");

    espnow_bbs_mesh_contract_owner_init(&owner_b, &queue_a);
    check(owner_b.queue == &queue_a, "owner init can rebind to another caller queue");
    check(owner_b.accepted == 0u && owner_b.rejected == 0u, "owner rebind resets counters");
}

static void test_owner_summary_bounds(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_owner_t unbound;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "own1",
        "summary",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char exact[160];
    char too_small[20];
    size_t line_bytes = 0u;
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    espnow_bbs_mesh_contract_owner_init(&unbound, NULL);
    check(
        espnow_bbs_mesh_contract_owner_summary(&unbound, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND,
        "unbound owner summary rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner summary fixture claims role"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner summary fixture ingests event"
    );
    check(
        espnow_bbs_mesh_contract_owner_summary(
            &owner,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;owner=mesh_ingress;state=claimed;in=1;out=0;reject=0;q=1;drop=0;last=ok"
        ) == 0,
        "owner summary is stable"
    );
    check(line_bytes == strlen(summary), "owner summary byte count returns");
    check(
        line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
        "owner summary stays inside 512-byte bridge budget"
    );
    check(
        espnow_bbs_mesh_contract_owner_summary(&owner, exact, line_bytes + 1u, NULL)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "owner summary accepts exact caller capacity"
    );
    check(strcmp(exact, summary) == 0, "owner exact-capacity summary copies output");
    check(
        espnow_bbs_mesh_contract_owner_summary(
            &owner,
            too_small,
            sizeof(too_small),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "owner summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_owner_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null owner summary rejects"
    );
    check(
        espnow_bbs_mesh_contract_owner_summary(&owner, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null owner summary output rejects"
    );
}

static void test_dispatch_actions_and_stage_bounds(void)
{
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    char non_ascii_action[] = {'o', 'w', 'n', 'e', 'r', '_', (char)0xff, '\0'};
    char long_action[] = "owner_dispatch_too_long";
    char non_ascii_owner[] = {'m', 'e', 's', 'h', '_', (char)0xff, '\0'};
    size_t index;

    check(
        ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY == 8u,
        "dispatch plan capacity constant is 8"
    );
    check(
        ESPNOW_BBS_MESH_CONTRACT_DISPATCH_ACTION_NAME_MAX_BYTES == 16u,
        "dispatch action name capacity constant is 16"
    );
    check(
        ESPNOW_BBS_MESH_CONTRACT_DISPATCH_BUDGET_MAX == 16u,
        "dispatch budget max constant is 16"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_action_count() == 3u,
        "dispatch action count is stable"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_dispatch_action_name(0u), "owner_drain") == 0,
        "dispatch action 0 is stable"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_dispatch_action_name(1u), "owner_ingest") == 0,
        "dispatch action 1 is stable"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_dispatch_action_name(2u), "owner_snapshot") == 0,
        "dispatch action 2 is stable"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_action_name(3u) == NULL,
        "dispatch action out-of-range returns null"
    );
    check(
        strcmp(
            espnow_bbs_mesh_contract_dispatch_action_label("owner_ingest"),
            "oi"
        ) == 0,
        "dispatch action label is stable"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_action_status(NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null dispatch action rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_action_status(non_ascii_action)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII dispatch action rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_action_status(long_action)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_ACTION_NAME_TOO_LONG,
        "over-limit dispatch action rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_action_status("owner_flash")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_DISPATCH_ACTION,
        "unknown dispatch action rejects"
    );

    espnow_bbs_mesh_contract_dispatch_plan_init(&plan);
    check(plan.count == 0u, "dispatch plan initializes empty");
    check(plan.last_status == ESPNOW_BBS_MESH_CONTRACT_OK, "dispatch plan initializes status");
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            NULL,
            "mesh_ingress",
            "owner_ingest",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null dispatch plan stage rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            non_ascii_owner,
            "owner_ingest",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII dispatch owner rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_runtime",
            "owner_ingest",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_OWNER,
        "unknown dispatch owner rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_flash",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_DISPATCH_ACTION,
        "unknown dispatch stage action rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_ingest",
            0u,
            0u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_ZERO,
        "dispatch zero budget rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_ingest",
            ESPNOW_BBS_MESH_CONTRACT_DISPATCH_BUDGET_MAX + 1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_TOO_LARGE,
        "dispatch over-limit budget rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_ingest",
            1u,
            2u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_TOO_LARGE,
        "dispatch planned-over-requested rejects"
    );

    espnow_bbs_mesh_contract_dispatch_plan_reset(&plan);
    for (index = 0u;
         index < ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY;
         ++index) {
        check(
            espnow_bbs_mesh_contract_dispatch_plan_stage(
                &plan,
                "mesh_ingress",
                "owner_ingest",
                1u,
                1u,
                ESPNOW_BBS_MESH_CONTRACT_OK
            ) == ESPNOW_BBS_MESH_CONTRACT_OK,
            "dispatch plan accepts until fixed capacity"
        );
    }
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_ingest",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_PLAN_FULL,
        "dispatch plan full rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_count(&plan)
            == ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY,
        "dispatch plan count reaches fixed capacity"
    );
    espnow_bbs_mesh_contract_dispatch_plan_reset(&plan);
    check(plan.count == 0u && plan.requested == 0u && plan.planned == 0u, "dispatch reset clears counters");
}

static void test_dispatch_plan_from_owner_deterministic(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_dispatch_plan_t repeat;
    espnow_bbs_mesh_contract_event_input_t first = {
        "node_seen",
        "dp01",
        "first",
    };
    espnow_bbs_mesh_contract_event_input_t second = {
        "route_added",
        "dp02",
        "second",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char repeat_summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t line_bytes = 0u;
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &first)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch fixture first queue record accepts"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &second)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch fixture second queue record accepts"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch fixture owner claims role"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &owner,
            claim,
            2u,
            4u,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch plan builds from owner"
    );
    check(plan.count == 3u, "dispatch plan has three deterministic actions");
    check(strcmp(plan.steps[0u].action_name, "owner_drain") == 0, "dispatch plan drains first");
    check(strcmp(plan.steps[1u].action_name, "owner_ingest") == 0, "dispatch plan ingests second");
    check(strcmp(plan.steps[2u].action_name, "owner_snapshot") == 0, "dispatch plan snapshots third");
    check(plan.steps[0u].requested == 4u && plan.steps[0u].planned == 2u, "dispatch drain budget is bounded by queue count");
    check(plan.steps[1u].requested == 2u && plan.steps[1u].planned == 2u, "dispatch ingest budget fits available queue space");
    check(plan.steps[2u].requested == 1u && plan.steps[2u].planned == 1u, "dispatch snapshot budgets one local view");
    check(plan.requested == 7u && plan.planned == 5u && plan.rejected == 2u, "dispatch plan counters are stable");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 2u,
        "dispatch planning does not drain caller queue"
    );
    check(owner.accepted == 0u && owner.drained == 0u && owner.rejected == 0u, "dispatch planning does not mutate owner counters");
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(
            &plan,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;plan=3;req=7;ready=5;reject=2;first=od:mesh_ingress;last=os:mesh_ingress;status=ok"
        ) == 0,
        "dispatch summary is stable"
    );
    check(line_bytes == strlen(summary), "dispatch summary byte count returns");
    check(
        line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
        "dispatch summary stays inside 512-byte bridge budget"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &repeat,
            &owner,
            claim,
            2u,
            4u,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch plan rebuild is deterministic"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(
            &repeat,
            repeat_summary,
            sizeof(repeat_summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "repeat dispatch summary accepts"
    );
    check(strcmp(repeat_summary, summary) == 0, "dispatch repeated summary matches");
}

static void test_dispatch_plan_backpressure_and_owner_rejection(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_owner_t unbound;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "df00",
        "fill",
    };
    const char *const nodes[] = {
        "df00",
        "df01",
        "df02",
        "df03",
        "df04",
        "df05",
        "df06",
        "df07",
        "df08",
        "df09",
        "df10",
        "df11",
        "df12",
        "df13",
        "df14",
        "df15",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t claim = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &owner,
            1u,
            1u,
            0u,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED,
        "dispatch owner-bound plan rejects unclaimed owner"
    );
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch backpressure fixture claims owner"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &owner,
            claim + 1u,
            1u,
            0u,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE,
        "dispatch owner-bound plan rejects stale token"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &owner,
            claim,
            0u,
            0u,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_ZERO,
        "dispatch owner-bound plan rejects all-zero budgets"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &owner,
            claim,
            ESPNOW_BBS_MESH_CONTRACT_DISPATCH_BUDGET_MAX + 1u,
            0u,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_TOO_LARGE,
        "dispatch owner-bound plan rejects over-limit ingest budget"
    );
    espnow_bbs_mesh_contract_owner_init(&unbound, NULL);
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &unbound,
            1u,
            1u,
            0u,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND,
        "dispatch owner-bound plan rejects unbound owner"
    );

    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &owner,
            claim,
            0u,
            2u,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch empty-drain plan records local status"
    );
    check(plan.count == 1u, "dispatch empty-drain plan has one action");
    check(plan.steps[0u].status == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY, "dispatch empty-drain step records queue empty");
    check(plan.steps[0u].planned == 0u && plan.rejected == 2u, "dispatch empty-drain budget remains local");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 0u,
        "dispatch empty-drain planning does not mutate queue"
    );

    for (index = 0u;
         index < ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
         ++index) {
        event.node_id = nodes[index];
        check(
            espnow_bbs_mesh_contract_event_queue_push(&queue, &event)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "dispatch full-queue fixture fills caller queue"
        );
    }
    check(
        espnow_bbs_mesh_contract_dispatch_plan_from_owner(
            &plan,
            &owner,
            claim,
            3u,
            0u,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch full-ingest plan records local status"
    );
    check(plan.count == 1u, "dispatch full-ingest plan has one action");
    check(plan.steps[0u].status == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL, "dispatch full-ingest step records queue full");
    check(plan.steps[0u].planned == 0u && plan.rejected == 3u, "dispatch full-ingest budget remains local");
    check(espnow_bbs_mesh_contract_event_queue_count(&queue) == 16u, "dispatch full-ingest planning does not mutate queue count");
    check(espnow_bbs_mesh_contract_event_queue_dropped(&queue) == 0u, "dispatch full-ingest planning does not increment drop count");
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(
            &plan,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch full-ingest summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;plan=1;req=3;ready=0;reject=3;first=oi:mesh_ingress;last=oi:mesh_ingress;status=queue_full"
        ) == 0,
        "dispatch full-ingest summary is stable"
    );
}

static void test_dispatch_plan_summary_bounds(void)
{
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char exact[160];
    char too_small[18];
    size_t line_bytes = 0u;

    espnow_bbs_mesh_contract_dispatch_plan_init(&plan);
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(
            &plan,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "empty dispatch summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;plan=0;req=0;ready=0;reject=0;status=ok"
        ) == 0,
        "empty dispatch summary is stable"
    );
    check(line_bytes == strlen(summary), "empty dispatch summary byte count returns");
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_snapshot",
            "owner_snapshot",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch summary fixture stage accepts"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(
            &plan,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "non-empty dispatch summary accepts"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(
            &plan,
            exact,
            line_bytes + 1u,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "dispatch summary accepts exact caller capacity"
    );
    check(strcmp(exact, summary) == 0, "dispatch exact-capacity summary copies output");
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(
            &plan,
            too_small,
            sizeof(too_small),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "dispatch summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null dispatch summary plan rejects"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_summary(&plan, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null dispatch summary output rejects"
    );
}

static void test_scheduler_profiles_and_state_bounds(void)
{
    espnow_bbs_mesh_contract_scheduler_state_t state;
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char exact[160];
    char too_small[24];
    char non_ascii_profile[] = {'b', 'a', 'l', (char)0xff, '\0'};
    char long_profile[] = "scheduler_profile_too_long";
    size_t line_bytes = 0u;

    check(
        ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_PROFILE_NAME_MAX_BYTES == 16u,
        "scheduler profile name capacity constant is 16"
    );
    check(
        ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_TICK_MAX == 64u,
        "scheduler tick max constant is 64"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_profile_count() == 3u,
        "scheduler profile count is stable"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_scheduler_profile_name(0u), "balanced") == 0,
        "scheduler profile 0 is balanced"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_scheduler_profile_name(1u), "drain_first") == 0,
        "scheduler profile 1 is drain_first"
    );
    check(
        strcmp(espnow_bbs_mesh_contract_scheduler_profile_name(2u), "snapshot_only") == 0,
        "scheduler profile 2 is snapshot_only"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_profile_name(3u) == NULL,
        "scheduler profile out-of-range returns null"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_profile_status(NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null scheduler profile rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_profile_status(non_ascii_profile)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII scheduler profile rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_profile_status(long_profile)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_PROFILE_NAME_TOO_LONG,
        "over-limit scheduler profile rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_profile_status("runtime_loop")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_SCHEDULER_PROFILE,
        "unknown scheduler profile rejects"
    );

    espnow_bbs_mesh_contract_scheduler_state_init(&state);
    check(
        espnow_bbs_mesh_contract_scheduler_state_tick_count(&state) == 0u,
        "scheduler state initializes tick count"
    );
    check(
        state.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler state initializes status"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_state_tick_count(NULL) == 0u,
        "null scheduler state tick count returns zero"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_state_summary(
            &state,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "empty scheduler summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;sched=none;owner=none;ticks=0;last_tick=0;req=0;ready=0;reject=0;status=ok"
        ) == 0,
        "empty scheduler summary is stable"
    );
    check(line_bytes == strlen(summary), "empty scheduler summary byte count returns");
    check(
        espnow_bbs_mesh_contract_scheduler_state_summary(
            &state,
            exact,
            line_bytes + 1u,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler summary accepts exact caller capacity"
    );
    check(strcmp(exact, summary) == 0, "scheduler exact-capacity summary copies output");
    check(
        espnow_bbs_mesh_contract_scheduler_state_summary(
            &state,
            too_small,
            sizeof(too_small),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "scheduler summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_state_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null scheduler summary state rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_state_summary(&state, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null scheduler summary output rejects"
    );
}

static void test_scheduler_stage_tick_deterministic(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_scheduler_state_t state;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_event_input_t first = {
        "node_seen",
        "sc01",
        "first",
    };
    espnow_bbs_mesh_contract_event_input_t second = {
        "route_added",
        "sc02",
        "second",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t line_bytes = 0u;
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &first)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler fixture first queue record accepts"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &second)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler fixture second queue record accepts"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler fixture owner claims role"
    );
    espnow_bbs_mesh_contract_scheduler_state_init(&state);
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler balanced tick stages dispatch plan"
    );
    check(plan.count == 3u, "scheduler balanced tick stages three actions");
    check(strcmp(plan.steps[0u].action_name, "owner_drain") == 0, "scheduler tick drains first");
    check(strcmp(plan.steps[1u].action_name, "owner_ingest") == 0, "scheduler tick ingests second");
    check(strcmp(plan.steps[2u].action_name, "owner_snapshot") == 0, "scheduler tick snapshots third");
    check(plan.requested == 5u && plan.planned == 5u && plan.rejected == 0u, "scheduler balanced tick counters are stable");
    check(
        state.tick_count == 1u
            && state.last_tick == 1u
            && state.requested == 5u
            && state.planned == 5u
            && state.rejected == 0u,
        "scheduler state records first tick counters"
    );
    check(strcmp(state.profile_name, "balanced") == 0, "scheduler state records profile");
    check(strcmp(state.owner_name, "mesh_ingress") == 0, "scheduler state records owner");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 2u,
        "scheduler tick does not drain caller queue"
    );
    check(owner.accepted == 0u && owner.drained == 0u && owner.rejected == 0u, "scheduler tick does not mutate owner counters");
    check(
        espnow_bbs_mesh_contract_scheduler_state_summary(
            &state,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler summary after tick accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;sched=balanced;owner=mesh_ingress;ticks=1;last_tick=1;req=5;ready=5;reject=0;status=ok"
        ) == 0,
        "scheduler summary after tick is stable"
    );
    check(
        line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
        "scheduler summary stays inside 512-byte bridge budget"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            claim,
            "drain_first",
            2u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler drain-first tick stages dispatch plan"
    );
    check(plan.requested == 6u && plan.planned == 4u && plan.rejected == 2u, "scheduler drain-first plan counters are stable");
    check(
        state.tick_count == 2u
            && state.last_tick == 2u
            && state.requested == 11u
            && state.planned == 9u
            && state.rejected == 2u,
        "scheduler state aggregates repeated ticks"
    );
}

static void test_scheduler_stage_tick_rejection_and_backpressure(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_owner_t unbound;
    espnow_bbs_mesh_contract_scheduler_state_t state;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "sb00",
        "fill",
    };
    const char *const nodes[] = {
        "sb00",
        "sb01",
        "sb02",
        "sb03",
        "sb04",
        "sb05",
        "sb06",
        "sb07",
        "sb08",
        "sb09",
        "sb10",
        "sb11",
        "sb12",
        "sb13",
        "sb14",
        "sb15",
    };
    size_t claim = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    espnow_bbs_mesh_contract_owner_init(&unbound, NULL);
    espnow_bbs_mesh_contract_scheduler_state_init(&state);

    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            NULL,
            &plan,
            &owner,
            1u,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null scheduler state tick rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            NULL,
            &owner,
            1u,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null scheduler plan tick rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            NULL,
            1u,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null scheduler owner tick rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            1u,
            "runtime_loop",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_SCHEDULER_PROFILE,
        "scheduler unknown profile tick rejects"
    );
    check(state.tick_count == 0u, "scheduler rejected profile does not tick");
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            1u,
            "balanced",
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_ZERO,
        "scheduler zero tick rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            1u,
            "balanced",
            ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_TICK_MAX + 1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_TOO_LARGE,
        "scheduler over-limit tick rejects"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            1u,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED,
        "scheduler tick rejects unclaimed owner"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &unbound,
            1u,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND,
        "scheduler tick rejects unbound owner"
    );

    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler backpressure fixture claims owner"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            claim + 1u,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE,
        "scheduler tick rejects stale owner claim"
    );
    check(state.tick_count == 0u, "scheduler owner rejection does not tick");

    for (index = 0u;
         index < ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
         ++index) {
        event.node_id = nodes[index];
        check(
            espnow_bbs_mesh_contract_event_queue_push(&queue, &event)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "scheduler full-queue fixture fills caller queue"
        );
    }
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &state,
            &plan,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "scheduler full-queue tick stages local backpressure"
    );
    check(plan.count == 3u, "scheduler full-queue tick stages three actions");
    check(plan.steps[1u].status == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL, "scheduler tick records ingest backpressure");
    check(plan.requested == 5u && plan.planned == 3u && plan.rejected == 2u, "scheduler full-queue counters are stable");
    check(state.tick_count == 1u && state.rejected == 2u, "scheduler state records backpressure");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 16u,
        "scheduler full-queue tick does not mutate queue count"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_dropped(&queue) == 0u,
        "scheduler full-queue tick does not increment drop count"
    );
    espnow_bbs_mesh_contract_scheduler_state_reset(&state);
    check(state.tick_count == 0u && state.last_status == ESPNOW_BBS_MESH_CONTRACT_OK, "scheduler reset clears counters");
}

static void test_execution_accounting_summary_bounds(void)
{
    espnow_bbs_mesh_contract_execution_accounting_t accounting;
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char exact[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t line_bytes = 0u;

    check(
        ESPNOW_BBS_MESH_CONTRACT_EXECUTION_CYCLE_MAX == 64u,
        "execution cycle max constant is 64"
    );
    espnow_bbs_mesh_contract_execution_accounting_init(&accounting);
    check(
        espnow_bbs_mesh_contract_execution_accounting_cycle_count(&accounting) == 0u,
        "execution accounting initializes cycle count"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_cycle_count(NULL) == 0u,
        "null execution accounting cycle count returns zero"
    );
    check(
        accounting.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting initializes status"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_summary(
            &accounting,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "empty execution accounting summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;acct=0;owner=none;last_cycle=0;apply=0;skip=0;drain=0;ingest=0;snap=0;status=ok"
        ) == 0,
        "empty execution accounting summary is stable"
    );
    check(line_bytes == strlen(summary), "empty execution accounting summary byte count returns");
    check(
        espnow_bbs_mesh_contract_execution_accounting_summary(
            &accounting,
            exact,
            line_bytes + 1u,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting summary accepts exact caller capacity"
    );
    check(strcmp(exact, summary) == 0, "execution accounting exact-capacity summary copies output");
    check(
        espnow_bbs_mesh_contract_execution_accounting_summary(
            &accounting,
            exact,
            line_bytes,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "execution accounting summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null execution accounting summary state rejects"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_summary(&accounting, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null execution accounting summary output rejects"
    );
}

static void test_execution_accounting_apply_plan_deterministic(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_scheduler_state_t scheduler;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_execution_accounting_t accounting;
    espnow_bbs_mesh_contract_event_input_t first = {
        "node_seen",
        "ea01",
        "first",
    };
    espnow_bbs_mesh_contract_event_input_t second = {
        "route_added",
        "ea02",
        "second",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t line_bytes = 0u;
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &first)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting fixture first queue record accepts"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &second)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting fixture second queue record accepts"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting fixture owner claims role"
    );
    espnow_bbs_mesh_contract_scheduler_state_init(&scheduler);
    espnow_bbs_mesh_contract_execution_accounting_init(&accounting);
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &scheduler,
            &plan,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting fixture stages balanced plan"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(
            &accounting,
            &plan,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting applies balanced plan"
    );
    check(
        accounting.cycle_count == 1u
            && accounting.last_cycle == 1u
            && accounting.applied == 5u
            && accounting.skipped == 0u,
        "execution accounting records first cycle counters"
    );
    check(accounting.drain_ready == 2u, "execution accounting records drain-ready units");
    check(accounting.ingest_ready == 2u, "execution accounting records ingest-ready units");
    check(accounting.snapshot_ready == 1u, "execution accounting records snapshot-ready units");
    check(strcmp(accounting.owner_name, "mesh_ingress") == 0, "execution accounting records owner");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 2u,
        "execution accounting does not drain caller queue"
    );
    check(owner.accepted == 0u && owner.drained == 0u && owner.rejected == 0u, "execution accounting does not mutate owner counters");
    check(scheduler.tick_count == 1u && scheduler.planned == 5u, "execution accounting does not mutate scheduler counters");
    check(plan.count == 3u && plan.planned == 5u, "execution accounting does not mutate dispatch plan");
    check(
        espnow_bbs_mesh_contract_execution_accounting_summary(
            &accounting,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting summary after plan accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;acct=1;owner=mesh_ingress;last_cycle=1;apply=5;skip=0;drain=2;ingest=2;snap=1;status=ok"
        ) == 0,
        "execution accounting summary after plan is stable"
    );
    check(
        line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
        "execution accounting summary stays inside 512-byte bridge budget"
    );
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &scheduler,
            &plan,
            &owner,
            claim,
            "drain_first",
            2u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting fixture stages drain-first plan"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(
            &accounting,
            &plan,
            2u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting applies drain-first plan"
    );
    check(
        accounting.cycle_count == 2u
            && accounting.last_cycle == 2u
            && accounting.applied == 9u
            && accounting.skipped == 2u,
        "execution accounting aggregates repeated cycles"
    );
    check(accounting.drain_ready == 4u, "execution accounting aggregates drain-ready units");
    check(accounting.ingest_ready == 3u, "execution accounting aggregates ingest-ready units");
    check(accounting.snapshot_ready == 2u, "execution accounting aggregates snapshot-ready units");
    espnow_bbs_mesh_contract_execution_accounting_reset(&accounting);
    check(
        accounting.cycle_count == 0u && accounting.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting reset clears counters"
    );
}

static void test_execution_accounting_rejection_and_backpressure(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_execution_accounting_t accounting;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "eb00",
        "fill",
    };
    const char *const nodes[] = {
        "eb00",
        "eb01",
        "eb02",
        "eb03",
        "eb04",
        "eb05",
        "eb06",
        "eb07",
        "eb08",
        "eb09",
        "eb10",
        "eb11",
        "eb12",
        "eb13",
        "eb14",
        "eb15",
    };
    size_t claim = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    espnow_bbs_mesh_contract_execution_accounting_init(&accounting);
    espnow_bbs_mesh_contract_dispatch_plan_init(&plan);
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(NULL, &plan, 1u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null execution accounting apply rejects"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(&accounting, NULL, 1u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null execution accounting plan rejects"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(&accounting, &plan, 0u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_ZERO,
        "execution accounting zero cycle rejects"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(
            &accounting,
            &plan,
            ESPNOW_BBS_MESH_CONTRACT_EXECUTION_CYCLE_MAX + 1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_TOO_LARGE,
        "execution accounting over-limit cycle rejects"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(&accounting, &plan, 1u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_PLAN_EMPTY,
        "execution accounting empty plan rejects"
    );
    check(accounting.cycle_count == 0u, "execution accounting rejected plans do not cycle");

    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_drain",
            2u,
            0u,
            ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting empty-drain fixture stages"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(&accounting, &plan, 1u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_STEP_NOT_READY,
        "execution accounting all-skipped plan rejects"
    );
    check(accounting.cycle_count == 0u, "execution accounting all-skipped plan does not cycle");

    espnow_bbs_mesh_contract_dispatch_plan_reset(&plan);
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_drain",
            2u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting inconsistent step fixture stages"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(&accounting, &plan, 1u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_STEP_NOT_READY,
        "execution accounting planned non-ready step rejects"
    );

    espnow_bbs_mesh_contract_dispatch_plan_reset(&plan);
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_ingress",
            "owner_snapshot",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting mixed-owner first step stages"
    );
    check(
        espnow_bbs_mesh_contract_dispatch_plan_stage(
            &plan,
            "mesh_drain",
            "owner_snapshot",
            1u,
            1u,
            ESPNOW_BBS_MESH_CONTRACT_OK
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting mixed-owner second step stages"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(&accounting, &plan, 1u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_OWNER_MISMATCH,
        "execution accounting mixed owner plan rejects"
    );

    espnow_bbs_mesh_contract_dispatch_plan_reset(&plan);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting backpressure fixture claims owner"
    );
    for (index = 0u;
         index < ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
         ++index) {
        event.node_id = nodes[index];
        check(
            espnow_bbs_mesh_contract_event_queue_push(&queue, &event)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "execution accounting full-queue fixture fills caller queue"
        );
    }
    check(
        espnow_bbs_mesh_contract_scheduler_stage_tick(
            &(espnow_bbs_mesh_contract_scheduler_state_t){0},
            &plan,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting full-queue fixture stages plan"
    );
    check(
        espnow_bbs_mesh_contract_execution_accounting_apply_plan(&accounting, &plan, 1u)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "execution accounting partial backpressure plan accepts"
    );
    check(
        accounting.cycle_count == 1u
            && accounting.applied == 3u
            && accounting.skipped == 2u
            && accounting.ingest_ready == 0u,
        "execution accounting records partial backpressure"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 16u,
        "execution accounting backpressure does not mutate queue count"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_dropped(&queue) == 0u,
        "execution accounting backpressure does not increment drop count"
    );
}

static void test_runtime_cycle_summary_bounds(void)
{
    espnow_bbs_mesh_contract_runtime_cycle_t runtime;
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char exact[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t line_bytes = 0u;

    check(
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_CYCLE_MAX == 64u,
        "runtime cycle max constant is 64"
    );
    espnow_bbs_mesh_contract_runtime_cycle_init(&runtime);
    check(
        espnow_bbs_mesh_contract_runtime_cycle_count(&runtime) == 0u,
        "runtime cycle initializes cycle count"
    );
    check(
        espnow_bbs_mesh_contract_runtime_cycle_count(NULL) == 0u,
        "null runtime cycle count returns zero"
    );
    check(
        runtime.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle initializes status"
    );
    check(
        espnow_bbs_mesh_contract_runtime_cycle_summary(
            &runtime,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "empty runtime cycle summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;runtime=0;profile=none;owner=none;last_cycle=0;sched=0;apply=0;skip=0;reject=0;status=ok"
        ) == 0,
        "empty runtime cycle summary is stable"
    );
    check(line_bytes == strlen(summary), "empty runtime cycle summary byte count returns");
    check(
        espnow_bbs_mesh_contract_runtime_cycle_summary(
            &runtime,
            exact,
            line_bytes + 1u,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle summary accepts exact caller capacity"
    );
    check(strcmp(exact, summary) == 0, "runtime cycle exact-capacity summary copies output");
    check(
        espnow_bbs_mesh_contract_runtime_cycle_summary(
            &runtime,
            exact,
            line_bytes,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "runtime cycle summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_runtime_cycle_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime cycle summary state rejects"
    );
    check(
        espnow_bbs_mesh_contract_runtime_cycle_summary(&runtime, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime cycle summary output rejects"
    );
}

static void test_runtime_cycle_step_deterministic(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_scheduler_state_t scheduler;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_execution_accounting_t accounting;
    espnow_bbs_mesh_contract_runtime_cycle_t runtime;
    espnow_bbs_mesh_contract_event_input_t first = {
        "node_seen",
        "rc01",
        "first",
    };
    espnow_bbs_mesh_contract_event_input_t second = {
        "route_added",
        "rc02",
        "second",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t line_bytes = 0u;
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &first)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle fixture first queue record accepts"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &second)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle fixture second queue record accepts"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle fixture owner claims role"
    );
    espnow_bbs_mesh_contract_scheduler_state_init(&scheduler);
    espnow_bbs_mesh_contract_dispatch_plan_init(&plan);
    espnow_bbs_mesh_contract_execution_accounting_init(&accounting);
    espnow_bbs_mesh_contract_runtime_cycle_init(&runtime);
    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            &plan,
            &accounting,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle balanced step accepts"
    );
    check(plan.count == 3u && plan.planned == 5u, "runtime cycle stages balanced plan");
    check(
        scheduler.tick_count == 1u
            && scheduler.last_tick == 1u
            && scheduler.planned == 5u,
        "runtime cycle commits scheduler state"
    );
    check(
        accounting.cycle_count == 1u
            && accounting.last_cycle == 1u
            && accounting.applied == 5u
            && accounting.skipped == 0u,
        "runtime cycle commits execution accounting"
    );
    check(
        runtime.cycle_count == 1u
            && runtime.last_cycle == 1u
            && runtime.scheduled == 5u
            && runtime.applied == 5u
            && runtime.skipped == 0u
            && runtime.rejected == 0u,
        "runtime cycle records first cycle counters"
    );
    check(strcmp(runtime.profile_name, "balanced") == 0, "runtime cycle records profile");
    check(strcmp(runtime.owner_name, "mesh_ingress") == 0, "runtime cycle records owner");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 2u,
        "runtime cycle does not drain caller queue"
    );
    check(owner.accepted == 0u && owner.drained == 0u && owner.rejected == 0u, "runtime cycle does not mutate owner counters");
    check(
        espnow_bbs_mesh_contract_runtime_cycle_summary(
            &runtime,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle summary after step accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;runtime=1;profile=balanced;owner=mesh_ingress;last_cycle=1;sched=5;apply=5;skip=0;reject=0;status=ok"
        ) == 0,
        "runtime cycle summary after step is stable"
    );
    check(
        line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
        "runtime cycle summary stays inside 512-byte bridge budget"
    );
    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            &plan,
            &accounting,
            &owner,
            claim,
            "drain_first",
            2u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle drain-first step accepts"
    );
    check(plan.requested == 6u && plan.planned == 4u && plan.rejected == 2u, "runtime cycle drain-first plan counters are stable");
    check(
        runtime.cycle_count == 2u
            && runtime.last_cycle == 2u
            && runtime.scheduled == 9u
            && runtime.applied == 9u
            && runtime.skipped == 2u
            && runtime.rejected == 2u,
        "runtime cycle aggregates repeated cycles"
    );
    check(accounting.applied == 9u && accounting.skipped == 2u, "runtime cycle accounting aggregates repeated cycles");
    espnow_bbs_mesh_contract_runtime_cycle_reset(&runtime);
    check(
        runtime.cycle_count == 0u && runtime.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle reset clears counters"
    );
}

static void test_runtime_cycle_rejection_is_atomic(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_owner_t unclaimed;
    espnow_bbs_mesh_contract_scheduler_state_t scheduler;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_execution_accounting_t accounting;
    espnow_bbs_mesh_contract_runtime_cycle_t runtime;
    espnow_bbs_mesh_contract_scheduler_state_t scheduler_before;
    espnow_bbs_mesh_contract_dispatch_plan_t plan_before;
    espnow_bbs_mesh_contract_execution_accounting_t accounting_before;
    espnow_bbs_mesh_contract_runtime_cycle_t runtime_before;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "ra00",
        "atomic",
    };
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle atomic fixture queue record accepts"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    espnow_bbs_mesh_contract_owner_init(&unclaimed, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle atomic fixture owner claims role"
    );
    espnow_bbs_mesh_contract_scheduler_state_init(&scheduler);
    espnow_bbs_mesh_contract_dispatch_plan_init(&plan);
    espnow_bbs_mesh_contract_execution_accounting_init(&accounting);
    espnow_bbs_mesh_contract_runtime_cycle_init(&runtime);

    scheduler_before = scheduler;
    plan_before = plan;
    accounting_before = accounting;
    runtime_before = runtime;
    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            &plan,
            &accounting,
            &owner,
            claim,
            "balanced",
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_ZERO,
        "runtime cycle zero cycle rejects"
    );
    check(memcmp(&scheduler, &scheduler_before, sizeof(scheduler)) == 0, "zero runtime cycle does not mutate scheduler");
    check(memcmp(&plan, &plan_before, sizeof(plan)) == 0, "zero runtime cycle does not mutate plan");
    check(memcmp(&accounting, &accounting_before, sizeof(accounting)) == 0, "zero runtime cycle does not mutate accounting");
    check(memcmp(&runtime, &runtime_before, sizeof(runtime)) == 0, "zero runtime cycle does not mutate runtime");

    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            &plan,
            &accounting,
            &owner,
            claim,
            "balanced",
            ESPNOW_BBS_MESH_CONTRACT_RUNTIME_CYCLE_MAX + 1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_TOO_LARGE,
        "runtime cycle over-limit cycle rejects"
    );
    check(memcmp(&scheduler, &scheduler_before, sizeof(scheduler)) == 0, "over-limit runtime cycle does not mutate scheduler");
    check(memcmp(&plan, &plan_before, sizeof(plan)) == 0, "over-limit runtime cycle does not mutate plan");
    check(memcmp(&accounting, &accounting_before, sizeof(accounting)) == 0, "over-limit runtime cycle does not mutate accounting");
    check(memcmp(&runtime, &runtime_before, sizeof(runtime)) == 0, "over-limit runtime cycle does not mutate runtime");

    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            &plan,
            &accounting,
            &owner,
            claim,
            "runtime_loop",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_SCHEDULER_PROFILE,
        "runtime cycle unknown profile rejects"
    );
    check(memcmp(&scheduler, &scheduler_before, sizeof(scheduler)) == 0, "unknown-profile runtime cycle does not mutate scheduler");
    check(memcmp(&plan, &plan_before, sizeof(plan)) == 0, "unknown-profile runtime cycle does not mutate plan");
    check(memcmp(&accounting, &accounting_before, sizeof(accounting)) == 0, "unknown-profile runtime cycle does not mutate accounting");
    check(memcmp(&runtime, &runtime_before, sizeof(runtime)) == 0, "unknown-profile runtime cycle does not mutate runtime");

    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            &plan,
            &accounting,
            &unclaimed,
            1u,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED,
        "runtime cycle unclaimed owner rejects"
    );
    check(memcmp(&scheduler, &scheduler_before, sizeof(scheduler)) == 0, "unclaimed-owner runtime cycle does not mutate scheduler");
    check(memcmp(&plan, &plan_before, sizeof(plan)) == 0, "unclaimed-owner runtime cycle does not mutate plan");
    check(memcmp(&accounting, &accounting_before, sizeof(accounting)) == 0, "unclaimed-owner runtime cycle does not mutate accounting");
    check(memcmp(&runtime, &runtime_before, sizeof(runtime)) == 0, "unclaimed-owner runtime cycle does not mutate runtime");

    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            NULL,
            &scheduler,
            &plan,
            &accounting,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime cycle state rejects"
    );
    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            NULL,
            &plan,
            &accounting,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime cycle scheduler rejects"
    );
    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            NULL,
            &accounting,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime cycle plan rejects"
    );
}

static void test_runtime_cycle_backpressure_accounting(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_scheduler_state_t scheduler;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_execution_accounting_t accounting;
    espnow_bbs_mesh_contract_runtime_cycle_t runtime;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "rb00",
        "fill",
    };
    const char *const nodes[] = {
        "rb00",
        "rb01",
        "rb02",
        "rb03",
        "rb04",
        "rb05",
        "rb06",
        "rb07",
        "rb08",
        "rb09",
        "rb10",
        "rb11",
        "rb12",
        "rb13",
        "rb14",
        "rb15",
    };
    size_t claim = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    for (index = 0u;
         index < ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY;
         ++index) {
        event.node_id = nodes[index];
        check(
            espnow_bbs_mesh_contract_event_queue_push(&queue, &event)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "runtime cycle full-queue fixture fills caller queue"
        );
    }
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle backpressure fixture owner claims role"
    );
    espnow_bbs_mesh_contract_scheduler_state_init(&scheduler);
    espnow_bbs_mesh_contract_dispatch_plan_init(&plan);
    espnow_bbs_mesh_contract_execution_accounting_init(&accounting);
    espnow_bbs_mesh_contract_runtime_cycle_init(&runtime);
    check(
        espnow_bbs_mesh_contract_runtime_cycle_step(
            &runtime,
            &scheduler,
            &plan,
            &accounting,
            &owner,
            claim,
            "balanced",
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime cycle partial backpressure accepts"
    );
    check(plan.count == 3u, "runtime cycle full queue stages three actions");
    check(plan.steps[1u].status == ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL, "runtime cycle records ingest backpressure");
    check(runtime.scheduled == 3u && runtime.applied == 3u, "runtime cycle records ready units under backpressure");
    check(runtime.skipped == 2u && runtime.rejected == 2u, "runtime cycle records skipped units under backpressure");
    check(accounting.ingest_ready == 0u, "runtime cycle backpressure records no ingest-ready units");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 16u,
        "runtime cycle backpressure does not mutate queue count"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_dropped(&queue) == 0u,
        "runtime cycle backpressure does not increment drop count"
    );
}

static void test_runtime_scheduler_config_and_summary_bounds(void)
{
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config =
        runtime_scheduler_valid_config(1u);
    espnow_bbs_mesh_contract_runtime_scheduler_config_t bad;
    espnow_bbs_mesh_contract_runtime_scheduler_t before;
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char exact[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char too_small[24];
    char non_ascii_policy[] = {'c', 'a', 'l', 'l', (char)0xff, '\0'};
    char long_policy[] = "caller_invoked_policy_too_long";
    size_t line_bytes = 0u;

    check(
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_POLICY_NAME_MAX_BYTES == 16u,
        "runtime scheduler policy name capacity constant is 16"
    );
    check(
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_PERIOD_MAX == 64u,
        "runtime scheduler period max constant is 64"
    );
    check(
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE == -1,
        "runtime scheduler core-none sentinel is stable"
    );
    check(
        strcmp(
            espnow_bbs_mesh_contract_status_name(
                ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN
            ),
            "runtime_scheduler_runtime_open"
        ) == 0,
        "runtime scheduler status names are stable"
    );

    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step_count(&runtime_scheduler) == 0u,
        "runtime scheduler initializes step count"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step_count(NULL) == 0u,
        "null runtime scheduler step count returns zero"
    );
    check(
        runtime_scheduler.core_id
            == ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE,
        "runtime scheduler initializes core policy"
    );
    check(!runtime_scheduler.configured, "runtime scheduler initializes unconfigured");
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_summary(
            &runtime_scheduler,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "empty runtime scheduler summary accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;schedsrc=none;owner=none;profile=none;steps=0;last_cycle=0;sched=0;apply=0;skip=0;reject=0;yield=0;status=ok"
        ) == 0,
        "empty runtime scheduler summary is stable"
    );
    check(line_bytes == strlen(summary), "empty runtime scheduler summary byte count returns");
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_summary(
            &runtime_scheduler,
            exact,
            line_bytes + 1u,
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler summary accepts exact caller capacity"
    );
    check(strcmp(exact, summary) == 0, "runtime scheduler exact summary copies output");
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_summary(
            &runtime_scheduler,
            too_small,
            sizeof(too_small),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "runtime scheduler summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_summary(
            NULL,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime scheduler summary state rejects"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_summary(
            &runtime_scheduler,
            NULL,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime scheduler summary output rejects"
    );

    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&config)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler config accepts caller-invoked policy"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime scheduler config rejects"
    );

    before = runtime_scheduler;
    bad = config;
    bad.invoke_policy = non_ascii_policy;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &bad
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "runtime scheduler non-ASCII policy rejects"
    );
    check(
        memcmp(&runtime_scheduler, &before, sizeof(runtime_scheduler)) == 0,
        "runtime scheduler rejected configure does not mutate state"
    );
    bad = config;
    bad.invoke_policy = long_policy;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_POLICY_NAME_TOO_LONG,
        "runtime scheduler over-limit policy rejects"
    );
    bad = config;
    bad.invoke_policy = "timer_driven";
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_UNKNOWN_POLICY,
        "runtime scheduler unknown policy rejects"
    );
    bad = config;
    bad.task_policy = "task_owned";
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_UNKNOWN_POLICY,
        "runtime scheduler task policy opening rejects"
    );
    bad = config;
    bad.timer_policy = "esp_timer";
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_UNKNOWN_POLICY,
        "runtime scheduler timer policy opening rejects"
    );
    bad = config;
    bad.period_ticks = 0u;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_ZERO,
        "runtime scheduler zero period rejects"
    );
    bad = config;
    bad.yield_interval = ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_PERIOD_MAX + 1u;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_TOO_LARGE,
        "runtime scheduler over-limit yield interval rejects"
    );
    bad = config;
    bad.priority = 1u;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN,
        "runtime scheduler priority opening rejects"
    );
    bad = config;
    bad.stack_bytes = 1u;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN,
        "runtime scheduler stack opening rejects"
    );
    bad = config;
    bad.core_id = 0;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN,
        "runtime scheduler core selection rejects"
    );
    bad = config;
    bad.static_state = true;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_config_status(&bad)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN,
        "runtime scheduler static state opening rejects"
    );

    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler configure accepts caller-owned source contract"
    );
    check(runtime_scheduler.configured, "runtime scheduler stores configured flag");
    check(strcmp(runtime_scheduler.owner_name, "mesh_ingress") == 0, "runtime scheduler stores owner name");
    check(strcmp(runtime_scheduler.profile_name, "balanced") == 0, "runtime scheduler stores profile name");
    check(strcmp(runtime_scheduler.invoke_policy, "caller_invoked") == 0, "runtime scheduler stores invoke policy");
    check(strcmp(runtime_scheduler.task_policy, "task_none") == 0, "runtime scheduler stores task policy");
    check(strcmp(runtime_scheduler.timer_policy, "timer_none") == 0, "runtime scheduler stores timer policy");
}

static void test_runtime_scheduler_step_deterministic(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config;
    espnow_bbs_mesh_contract_event_input_t first = {
        "node_seen",
        "rs01",
        "first",
    };
    espnow_bbs_mesh_contract_event_input_t second = {
        "route_added",
        "rs02",
        "second",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    size_t line_bytes = 0u;
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &first)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler fixture first queue record accepts"
    );
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &second)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler fixture second queue record accepts"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler fixture owner claims role"
    );
    config = runtime_scheduler_valid_config(claim);
    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            &owner,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_NOT_CONFIGURED,
        "runtime scheduler unconfigured step rejects"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler step fixture configures"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            &owner,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler caller step accepts"
    );
    check(
        runtime_scheduler.step_count == 1u
            && runtime_scheduler.runtime.cycle_count == 1u
            && runtime_scheduler.scheduler.tick_count == 1u
            && runtime_scheduler.accounting.cycle_count == 1u,
        "runtime scheduler composes nested caller-owned states"
    );
    check(
        runtime_scheduler.scheduled == 5u
            && runtime_scheduler.applied == 5u
            && runtime_scheduler.skipped == 0u
            && runtime_scheduler.rejected == 0u,
        "runtime scheduler records first step counters"
    );
    check(runtime_scheduler.yield_due == 0u, "runtime scheduler first step does not mark yield due");
    check(
        espnow_bbs_mesh_contract_event_queue_count(&queue) == 2u,
        "runtime scheduler caller step does not drain caller queue"
    );
    check(owner.accepted == 0u && owner.drained == 0u && owner.rejected == 0u, "runtime scheduler caller step does not mutate owner counters");
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_summary(
            &runtime_scheduler,
            summary,
            sizeof(summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler summary after step accepts"
    );
    check(
        strcmp(
            summary,
            "schema=mesh_discovery.v1;schedsrc=caller_invoked;owner=mesh_ingress;profile=balanced;steps=1;last_cycle=1;sched=5;apply=5;skip=0;reject=0;yield=0;status=ok"
        ) == 0,
        "runtime scheduler summary after step is stable"
    );
    check(
        line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
        "runtime scheduler summary stays inside 512-byte bridge budget"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            &owner,
            2u
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler second caller step accepts"
    );
    check(
        runtime_scheduler.step_count == 2u
            && runtime_scheduler.scheduled == 10u
            && runtime_scheduler.applied == 10u
            && runtime_scheduler.yield_due == 1u,
        "runtime scheduler aggregates repeated caller steps"
    );
    espnow_bbs_mesh_contract_runtime_scheduler_reset(&runtime_scheduler);
    check(
        runtime_scheduler.step_count == 0u
            && !runtime_scheduler.configured
            && runtime_scheduler.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler reset clears counters and configuration"
    );
}

static void test_runtime_scheduler_rejection_is_atomic(void)
{
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_owner_t unclaimed;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_runtime_scheduler_t before;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "rx00",
        "atomic",
    };
    size_t claim = 0u;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    check(
        espnow_bbs_mesh_contract_event_queue_push(&queue, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler atomic fixture queue record accepts"
    );
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    espnow_bbs_mesh_contract_owner_init(&unclaimed, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(&owner, "mesh_ingress", &claim)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler atomic fixture owner claims role"
    );
    config = runtime_scheduler_valid_config(claim);
    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler atomic fixture configures"
    );

    before = runtime_scheduler;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            &owner,
            0u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_ZERO,
        "runtime scheduler zero cycle rejects"
    );
    check(
        memcmp(&runtime_scheduler, &before, sizeof(runtime_scheduler)) == 0,
        "runtime scheduler rejected step does not mutate state"
    );

    before = runtime_scheduler;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            &unclaimed,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED,
        "runtime scheduler unclaimed owner rejects"
    );
    check(
        memcmp(&runtime_scheduler, &before, sizeof(runtime_scheduler)) == 0,
        "runtime scheduler unclaimed owner does not mutate state"
    );

    config = runtime_scheduler_valid_config(claim + 1u);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler stale-token fixture configures"
    );
    before = runtime_scheduler;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            &owner,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE,
        "runtime scheduler stale owner claim rejects"
    );
    check(
        memcmp(&runtime_scheduler, &before, sizeof(runtime_scheduler)) == 0,
        "runtime scheduler stale owner claim does not mutate state"
    );

    config = runtime_scheduler_valid_config(claim);
    config.owner_name = "mesh_drain";
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "runtime scheduler owner-mismatch fixture configures"
    );
    before = runtime_scheduler;
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            &owner,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_OWNER_MISMATCH,
        "runtime scheduler owner mismatch rejects"
    );
    check(
        memcmp(&runtime_scheduler, &before, sizeof(runtime_scheduler)) == 0,
        "runtime scheduler owner mismatch does not mutate state"
    );

    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            NULL,
            &owner,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime scheduler step state rejects"
    );
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_step(
            &runtime_scheduler,
            NULL,
            1u
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null runtime scheduler step owner rejects"
    );
}

static void test_request_and_gate_f_invariance(void)
{
    const char *const requests[] = {
        "discovery_snapshot",
        "discovery_events",
        "service_catalog",
        "capability_report",
    };
    const char *const bridge_requests[] = {
        "msg_post",
        "download_queue",
        "telemetry_report",
        "node_status",
        "protocol_report",
        "state_get",
        "control_intent",
    };
    const char *const services[] = {
        "direct_message",
        "file_chunk",
        "telemetry_report",
        "node_status",
        "custody_ack",
        "control_intent",
    };
    const int service_codes[] = {1, 2, 3, 4, 5, 6};
    size_t request_index;
    size_t index;

    check(
        espnow_bbs_mesh_contract_request_count()
            == sizeof(requests) / sizeof(requests[0]),
        "discovery request count matches host fixture"
    );
    for (request_index = 0;
         request_index < espnow_bbs_mesh_contract_request_count();
         ++request_index) {
        check(
            strcmp(
                espnow_bbs_mesh_contract_request_name(request_index),
                requests[request_index]
            ) == 0,
            "discovery request order matches host fixture"
        );
        check(
            espnow_bbs_mesh_contract_request_status(requests[request_index])
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known discovery request accepts"
        );
    }
    check(
        espnow_bbs_mesh_contract_request_status("mesh_start")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
        "unknown discovery request rejects"
    );

    check(
        espnow_bbs_mesh_contract_bridge_request_count()
            == sizeof(bridge_requests) / sizeof(bridge_requests[0]),
        "Gate F bridge request count is stable"
    );
    for (request_index = 0;
         request_index < espnow_bbs_mesh_contract_request_count();
         ++request_index) {
        for (index = 0; index < espnow_bbs_mesh_contract_bridge_request_count(); ++index) {
            check(
                strcmp(
                    espnow_bbs_mesh_contract_request_name(request_index),
                    espnow_bbs_mesh_contract_bridge_request_name(index)
                ) != 0,
                "discovery request stays disjoint from Gate F bridge requests"
            );
        }
    }

    check(
        espnow_bbs_mesh_contract_gate_f_service_count()
            == sizeof(services) / sizeof(services[0]),
        "Gate F service count is stable"
    );
    for (index = 0; index < espnow_bbs_mesh_contract_gate_f_service_count(); ++index) {
        check(
            strcmp(espnow_bbs_mesh_contract_gate_f_service_name(index), services[index]) == 0,
            "Gate F service name is stable"
        );
        check(
            espnow_bbs_mesh_contract_gate_f_service_code(index) == service_codes[index],
            "Gate F service code is stable"
        );
        check(
            strcmp(espnow_bbs_mesh_contract_gate_f_service_name(index), "mesh_discovery")
                != 0,
            "mesh_discovery is not a Gate F service"
        );
        check(
            strcmp(espnow_bbs_mesh_contract_gate_f_service_name(index), "discovery_snapshot")
                != 0,
            "discovery_snapshot is not a Gate F service"
        );
    }
}

static void test_bridge_summary_selection_bounds_and_rejections(void)
{
    const char *const requests[] = {
        "discovery_snapshot",
        "discovery_events",
        "service_catalog",
        "capability_report",
    };
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_bridge_summary_state_t state;
    espnow_bbs_mesh_contract_bridge_summary_state_t missing;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "n01",
        "bridge",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char tiny[8];
    size_t claim_token = 0u;
    size_t queue_count_before;
    size_t queue_dropped_before;
    size_t owner_accepted_before;
    size_t owner_rejected_before;
    size_t owner_drained_before;
    size_t runtime_steps_before;
    size_t line_bytes = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(
            &owner,
            "mesh_ingress",
            &claim_token
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge summary owner claim accepts"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim_token, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge summary fixture enqueues event"
    );
    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    config = runtime_scheduler_valid_config(claim_token);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge summary runtime scheduler config accepts"
    );

    state.event_queue = &queue;
    state.owner = &owner;
    state.runtime_scheduler = &runtime_scheduler;

    queue_count_before = queue.count;
    queue_dropped_before = queue.dropped;
    owner_accepted_before = owner.accepted;
    owner_rejected_before = owner.rejected;
    owner_drained_before = owner.drained;
    runtime_steps_before = runtime_scheduler.step_count;

    for (index = 0u; index < sizeof(requests) / sizeof(requests[0]); ++index) {
        memset(summary, 0, sizeof(summary));
        line_bytes = 0u;
        check(
            espnow_bbs_mesh_contract_bridge_summary(
                requests[index],
                &state,
                summary,
                sizeof(summary),
                &line_bytes
            ) == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known bridge summary request accepts"
        );
        check(
            strstr(summary, "schema=mesh_discovery.v1") != NULL,
            "bridge summary includes schema"
        );
        check(
            strstr(summary, requests[index]) != NULL,
            "bridge summary includes request name"
        );
        check(
            line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
            "bridge summary stays inside 512-byte bridge budget"
        );
        check(
            espnow_bbs_mesh_contract_summary_status(summary, NULL)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "bridge summary output is bounded ASCII"
        );
    }

    check(
        strstr(summary, "live=blocked") != NULL,
        "capability summary keeps live action closed"
    );
    check(
        espnow_bbs_mesh_contract_bridge_summary(
            "discovery_snapshot",
            &state,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "bridge summary rejects small caller capacity"
    );
    check(
        espnow_bbs_mesh_contract_bridge_summary(NULL, &state, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge summary request rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_summary(
            "discovery_snapshot",
            NULL,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge summary state rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_summary(
            "discovery_snapshot",
            &state,
            NULL,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge summary output rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_summary(
            "route_rewrite",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
        "unknown bridge summary request rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_summary(
            "discovery_snapshot\xff",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII bridge summary request rejects"
    );

    missing = state;
    missing.event_queue = NULL;
    check(
        espnow_bbs_mesh_contract_bridge_summary(
            "discovery_events",
            &missing,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge event summary requires caller queue"
    );
    missing = state;
    missing.runtime_scheduler = NULL;
    check(
        espnow_bbs_mesh_contract_bridge_summary(
            "capability_report",
            &missing,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge capability summary requires runtime scheduler"
    );

    check(queue.count == queue_count_before, "bridge summary does not mutate queue count");
    check(queue.dropped == queue_dropped_before, "bridge summary does not mutate drops");
    check(owner.accepted == owner_accepted_before, "bridge summary does not mutate ingress");
    check(owner.rejected == owner_rejected_before, "bridge summary does not mutate rejects");
    check(owner.drained == owner_drained_before, "bridge summary does not mutate drains");
    check(
        runtime_scheduler.step_count == runtime_steps_before,
        "bridge summary does not step runtime scheduler"
    );
}

static void test_bridge_request_router_accounting_and_bounds(void)
{
    const char *const requests[] = {
        "discovery_snapshot",
        "discovery_events",
        "service_catalog",
        "capability_report",
    };
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_bridge_summary_state_t state;
    espnow_bbs_mesh_contract_bridge_summary_state_t missing;
    espnow_bbs_mesh_contract_bridge_request_router_t router;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "n01",
        "router",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char router_summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char tiny[8];
    size_t claim_token = 0u;
    size_t queue_count_before;
    size_t queue_dropped_before;
    size_t owner_accepted_before;
    size_t owner_rejected_before;
    size_t owner_drained_before;
    size_t runtime_steps_before;
    size_t line_bytes = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(
            &owner,
            "mesh_ingress",
            &claim_token
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request router owner claim accepts"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim_token, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request router fixture enqueues event"
    );
    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    config = runtime_scheduler_valid_config(claim_token);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request router runtime scheduler config accepts"
    );
    state.event_queue = &queue;
    state.owner = &owner;
    state.runtime_scheduler = &runtime_scheduler;

    espnow_bbs_mesh_contract_bridge_request_router_init(&router);
    check(router.attempted == 0u, "bridge request router init clears attempts");
    check(router.accepted == 0u, "bridge request router init clears accepts");
    check(router.rejected == 0u, "bridge request router init clears rejects");
    check(
        router.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request router init sets ok status"
    );

    queue_count_before = queue.count;
    queue_dropped_before = queue.dropped;
    owner_accepted_before = owner.accepted;
    owner_rejected_before = owner.rejected;
    owner_drained_before = owner.drained;
    runtime_steps_before = runtime_scheduler.step_count;

    for (index = 0u; index < sizeof(requests) / sizeof(requests[0]); ++index) {
        memset(summary, 0, sizeof(summary));
        line_bytes = 0u;
        check(
            espnow_bbs_mesh_contract_bridge_request_route(
                &router,
                requests[index],
                &state,
                summary,
                sizeof(summary),
                &line_bytes
            ) == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known bridge request router route accepts"
        );
        check(
            strstr(summary, "schema=mesh_discovery.v1") != NULL,
            "bridge request router route includes schema"
        );
        check(
            strstr(summary, requests[index]) != NULL,
            "bridge request router route includes request name"
        );
        check(
            line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
            "bridge request router route stays inside 512-byte bridge budget"
        );
        check(
            espnow_bbs_mesh_contract_summary_status(summary, NULL)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "bridge request router output is bounded ASCII"
        );
    }

    check(router.attempted == 4u, "bridge request router tracks attempts");
    check(router.accepted == 4u, "bridge request router tracks accepts");
    check(router.rejected == 0u, "bridge request router tracks zero rejects");
    check(
        strcmp(router.last_request, "capability_report") == 0,
        "bridge request router tracks last accepted request"
    );
    check(
        router.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request router tracks ok last status"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_router_summary(
            &router,
            router_summary,
            sizeof(router_summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request router summary accepts"
    );
    check(
        strstr(router_summary, "router=local_request") != NULL,
        "bridge request router summary is local"
    );
    check(
        strstr(router_summary, "attempt=4") != NULL,
        "bridge request router summary tracks attempts"
    );
    check(
        strstr(router_summary, "live=blocked") != NULL,
        "bridge request router summary keeps live closed"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_router_summary(
            &router,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "bridge request router summary rejects small caller capacity"
    );

    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            NULL,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge request router rejects without accounting"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            &router,
            NULL,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request router null request rejects"
    );
    check(router.attempted == 5u, "bridge request router counts null request attempt");
    check(router.rejected == 1u, "bridge request router counts null request reject");
    check(router.last_request[0] == '\0', "bridge request router clears invalid request");
    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            &router,
            "route_rewrite",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
        "bridge request router unknown request rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            &router,
            "discovery_snapshot\xff",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "bridge request router non-ASCII request rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            &router,
            "discovery_snapshot",
            NULL,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request router null summary state rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            &router,
            "discovery_snapshot",
            &state,
            NULL,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request router null output rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            &router,
            "discovery_snapshot",
            &state,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "bridge request router small output rejects"
    );
    missing = state;
    missing.event_queue = NULL;
    check(
        espnow_bbs_mesh_contract_bridge_request_route(
            &router,
            "discovery_events",
            &missing,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request router missing queue rejects"
    );

    check(router.attempted == 11u, "bridge request router tracks rejected attempts");
    check(router.accepted == 4u, "bridge request router accepts remain stable");
    check(router.rejected == 7u, "bridge request router rejected count increments");
    check(
        router.last_status == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request router tracks last reject status"
    );
    check(
        strcmp(router.last_request, "discovery_events") == 0,
        "bridge request router keeps last valid rejected request"
    );
    check(queue.count == queue_count_before, "bridge request router does not mutate queue count");
    check(queue.dropped == queue_dropped_before, "bridge request router does not mutate drops");
    check(owner.accepted == owner_accepted_before, "bridge request router does not mutate ingress");
    check(owner.rejected == owner_rejected_before, "bridge request router does not mutate owner rejects");
    check(owner.drained == owner_drained_before, "bridge request router does not mutate drains");
    check(
        runtime_scheduler.step_count == runtime_steps_before,
        "bridge request router does not step runtime scheduler"
    );

    espnow_bbs_mesh_contract_bridge_request_router_reset(&router);
    check(router.attempted == 0u, "bridge request router reset clears attempts");
    check(router.accepted == 0u, "bridge request router reset clears accepts");
    check(router.rejected == 0u, "bridge request router reset clears rejects");
    check(router.last_request[0] == '\0', "bridge request router reset clears last request");
    check(
        espnow_bbs_mesh_contract_bridge_request_router_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge request router summary rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_router_summary(&router, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge request router summary output rejects"
    );
}

static void test_bridge_dispatch_envelope_accounting_and_bounds(void)
{
    const char *const requests[] = {
        "discovery_snapshot",
        "discovery_events",
        "service_catalog",
        "capability_report",
    };
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_bridge_summary_state_t state;
    espnow_bbs_mesh_contract_bridge_summary_state_t missing;
    espnow_bbs_mesh_contract_bridge_request_router_t router;
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t envelope;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "n01",
        "envelope",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char envelope_summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char tiny[8];
    size_t claim_token = 0u;
    size_t queue_count_before;
    size_t queue_dropped_before;
    size_t owner_accepted_before;
    size_t owner_rejected_before;
    size_t owner_drained_before;
    size_t runtime_steps_before;
    size_t line_bytes = 0u;
    size_t index;

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(
            &owner,
            "mesh_ingress",
            &claim_token
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge dispatch envelope owner claim accepts"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim_token, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge dispatch envelope fixture enqueues event"
    );
    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    config = runtime_scheduler_valid_config(claim_token);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge dispatch envelope runtime scheduler config accepts"
    );
    state.event_queue = &queue;
    state.owner = &owner;
    state.runtime_scheduler = &runtime_scheduler;

    espnow_bbs_mesh_contract_bridge_request_router_init(&router);
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(&envelope);
    check(envelope.attempted == 0u, "bridge dispatch envelope init clears attempts");
    check(envelope.would_dispatch == 0u, "bridge dispatch envelope init clears would-dispatch count");
    check(envelope.rejected == 0u, "bridge dispatch envelope init clears rejects");
    check(
        envelope.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge dispatch envelope init sets ok status"
    );

    queue_count_before = queue.count;
    queue_dropped_before = queue.dropped;
    owner_accepted_before = owner.accepted;
    owner_rejected_before = owner.rejected;
    owner_drained_before = owner.drained;
    runtime_steps_before = runtime_scheduler.step_count;

    for (index = 0u; index < sizeof(requests) / sizeof(requests[0]); ++index) {
        memset(summary, 0, sizeof(summary));
        line_bytes = 0u;
        check(
            espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
                &envelope,
                &router,
                requests[index],
                &state,
                summary,
                sizeof(summary),
                &line_bytes
            ) == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known bridge dispatch envelope route accepts"
        );
        check(
            strstr(summary, "schema=mesh_discovery.v1") != NULL,
            "bridge dispatch envelope route includes schema"
        );
        check(
            strstr(summary, requests[index]) != NULL,
            "bridge dispatch envelope route includes request name"
        );
        check(
            line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
            "bridge dispatch envelope route stays inside 512-byte bridge budget"
        );
        check(
            espnow_bbs_mesh_contract_summary_status(summary, NULL)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "bridge dispatch envelope output is bounded ASCII"
        );
    }

    check(envelope.attempted == 4u, "bridge dispatch envelope tracks attempts");
    check(envelope.would_dispatch == 4u, "bridge dispatch envelope tracks local would-dispatch count");
    check(envelope.rejected == 0u, "bridge dispatch envelope tracks zero rejects");
    check(router.attempted == 4u, "bridge dispatch envelope uses request router");
    check(router.accepted == 4u, "bridge dispatch envelope preserves router accepts");
    check(
        strcmp(envelope.last_request, "capability_report") == 0,
        "bridge dispatch envelope tracks last accepted request"
    );
    check(
        envelope.last_status == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge dispatch envelope tracks ok last status"
    );
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary(
            &envelope,
            envelope_summary,
            sizeof(envelope_summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge dispatch envelope summary accepts"
    );
    check(
        strstr(envelope_summary, "envelope=local_dispatch") != NULL,
        "bridge dispatch envelope summary is local"
    );
    check(
        strstr(envelope_summary, "would=4") != NULL,
        "bridge dispatch envelope summary tracks would-dispatch count"
    );
    check(
        strstr(envelope_summary, "live=blocked") != NULL,
        "bridge dispatch envelope summary keeps live closed"
    );
    check(
        strstr(envelope_summary, "serial=closed") != NULL,
        "bridge dispatch envelope summary keeps serial closed"
    );
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary(
            &envelope,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "bridge dispatch envelope summary rejects small caller capacity"
    );

    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            NULL,
            &router,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge dispatch envelope rejects without router accounting"
    );
    check(router.attempted == 4u, "null bridge dispatch envelope does not call router");
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            &envelope,
            NULL,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge dispatch envelope null router rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            &envelope,
            &router,
            NULL,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge dispatch envelope null request rejects"
    );
    check(envelope.attempted == 6u, "bridge dispatch envelope counts rejected attempts");
    check(envelope.rejected == 2u, "bridge dispatch envelope counts null rejects");
    check(router.attempted == 5u, "bridge dispatch envelope routes null request through router");
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            &envelope,
            &router,
            "route_rewrite",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
        "bridge dispatch envelope unknown request rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            &envelope,
            &router,
            "discovery_snapshot\xff",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "bridge dispatch envelope non-ASCII request rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            &envelope,
            &router,
            "discovery_snapshot",
            &state,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "bridge dispatch envelope small output rejects"
    );
    missing = state;
    missing.event_queue = NULL;
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
            &envelope,
            &router,
            "discovery_events",
            &missing,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge dispatch envelope missing queue rejects"
    );

    check(envelope.attempted == 10u, "bridge dispatch envelope tracks all rejected attempts");
    check(envelope.would_dispatch == 4u, "bridge dispatch envelope would-dispatch count stays stable on rejects");
    check(envelope.rejected == 6u, "bridge dispatch envelope rejected count increments");
    check(
        envelope.last_status == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge dispatch envelope tracks last reject status"
    );
    check(
        strcmp(envelope.last_request, "discovery_events") == 0,
        "bridge dispatch envelope keeps last valid rejected request"
    );
    check(queue.count == queue_count_before, "bridge dispatch envelope does not mutate queue count");
    check(queue.dropped == queue_dropped_before, "bridge dispatch envelope does not mutate drops");
    check(owner.accepted == owner_accepted_before, "bridge dispatch envelope does not mutate ingress");
    check(owner.rejected == owner_rejected_before, "bridge dispatch envelope does not mutate owner rejects");
    check(owner.drained == owner_drained_before, "bridge dispatch envelope does not mutate drains");
    check(
        runtime_scheduler.step_count == runtime_steps_before,
        "bridge dispatch envelope does not step runtime scheduler"
    );

    espnow_bbs_mesh_contract_bridge_dispatch_envelope_reset(&envelope);
    check(envelope.attempted == 0u, "bridge dispatch envelope reset clears attempts");
    check(envelope.would_dispatch == 0u, "bridge dispatch envelope reset clears would-dispatch count");
    check(envelope.rejected == 0u, "bridge dispatch envelope reset clears rejects");
    check(envelope.last_request[0] == '\0', "bridge dispatch envelope reset clears last request");
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge dispatch envelope summary rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary(&envelope, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge dispatch envelope summary output rejects"
    );
}

static void test_bridge_request_text_intent_normalization_and_bounds(void)
{
    const char *const request_texts[] = {
        " discovery_snapshot",
        "\tdiscovery_events\r\n",
        "service_catalog\n",
        "\r\ncapability_report\t ",
    };
    const char *const normalized_requests[] = {
        "discovery_snapshot",
        "discovery_events",
        "service_catalog",
        "capability_report",
    };
    const char *const malformed_texts[] = {
        "",
        " \t\r\n ",
        "discovery snapshot",
        "discovery_snapshot extra",
        "discovery_snapshot\ncapability_report",
        "discovery_snapshot\r\ncapability_report",
        "{\"request\":\"discovery_snapshot\"}",
        "req=discovery_snapshot",
        "v:1",
        "type:discovery_snapshot",
        "18:discovery_snapshot",
        "msg_post",
        "download_queue",
        "flash",
        "serial_write",
        "route_rewrite",
    };
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_bridge_summary_state_t state;
    espnow_bbs_mesh_contract_bridge_summary_state_t missing;
    espnow_bbs_mesh_contract_bridge_request_router_t router;
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t envelope;
    espnow_bbs_mesh_contract_bridge_request_text_intent_t intent;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "n01",
        "intent",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char intent_summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char tiny[8];
    char non_ascii_request[] = {
        'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', 'y', '_',
        's', 'n', 'a', 'p', 's', 'h', 'o', 't', (char)0xff, '\0'
    };
    char nul_request[] = {
        '\0', 'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', 'y', '_',
        's', 'n', 'a', 'p', 's', 'h', 'o', 't', '\0'
    };
    char overlong_request[
        ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 8u
    ];
    size_t claim_token = 0u;
    size_t queue_count_before;
    size_t queue_dropped_before;
    size_t owner_accepted_before;
    size_t owner_rejected_before;
    size_t owner_drained_before;
    size_t runtime_steps_before;
    size_t envelope_attempted_before;
    size_t envelope_rejected_before;
    size_t router_attempted_before;
    size_t router_rejected_before;
    size_t line_bytes = 0u;
    size_t index;

    memset(overlong_request, 'a', sizeof(overlong_request) - 1u);
    overlong_request[sizeof(overlong_request) - 1u] = '\0';

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(
            &owner,
            "mesh_ingress",
            &claim_token
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request text intent owner claim accepts"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim_token, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request text intent fixture enqueues event"
    );
    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    config = runtime_scheduler_valid_config(claim_token);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request text intent runtime scheduler config accepts"
    );
    state.event_queue = &queue;
    state.owner = &owner;
    state.runtime_scheduler = &runtime_scheduler;

    espnow_bbs_mesh_contract_bridge_request_router_init(&router);
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(&envelope);
    espnow_bbs_mesh_contract_bridge_request_text_intent_init(&intent);
    check(intent.attempted == 0u, "bridge request text intent init clears attempts");
    check(intent.accepted == 0u, "bridge request text intent init clears accepts");
    check(intent.rejected == 0u, "bridge request text intent init clears rejects");
    check(intent.last_status == ESPNOW_BBS_MESH_CONTRACT_OK, "bridge request text intent init sets ok status");

    queue_count_before = queue.count;
    queue_dropped_before = queue.dropped;
    owner_accepted_before = owner.accepted;
    owner_rejected_before = owner.rejected;
    owner_drained_before = owner.drained;
    runtime_steps_before = runtime_scheduler.step_count;

    for (index = 0u; index < sizeof(request_texts) / sizeof(request_texts[0]); ++index) {
        memset(summary, 0, sizeof(summary));
        line_bytes = 0u;
        check(
            espnow_bbs_mesh_contract_bridge_request_text_intent_route(
                &intent,
                &envelope,
                &router,
                request_texts[index],
                &state,
                summary,
                sizeof(summary),
                &line_bytes
            ) == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known bridge request text intent route accepts"
        );
        check(
            strcmp(intent.normalized_request, normalized_requests[index]) == 0,
            "bridge request text intent stores normalized request"
        );
        check(
            strstr(summary, "schema=mesh_discovery.v1") != NULL,
            "bridge request text intent route includes schema"
        );
        check(
            strstr(summary, normalized_requests[index]) != NULL,
            "bridge request text intent route includes normalized request"
        );
        check(
            line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
            "bridge request text intent route stays inside 512-byte bridge budget"
        );
        check(
            espnow_bbs_mesh_contract_summary_status(summary, NULL)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "bridge request text intent output is bounded ASCII"
        );
    }

    check(intent.attempted == 4u, "bridge request text intent tracks attempts");
    check(intent.accepted == 4u, "bridge request text intent tracks accepts");
    check(intent.rejected == 0u, "bridge request text intent tracks zero rejects");
    check(envelope.attempted == 4u, "bridge request text intent uses local envelope");
    check(envelope.would_dispatch == 4u, "bridge request text intent records would-dispatch count");
    check(router.attempted == 4u, "bridge request text intent uses request router");
    check(router.accepted == 4u, "bridge request text intent preserves router accepts");
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_summary(
            &intent,
            intent_summary,
            sizeof(intent_summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "bridge request text intent summary accepts"
    );
    check(
        strstr(intent_summary, "intent=request_text") != NULL,
        "bridge request text intent summary names local request text intent"
    );
    check(
        strstr(intent_summary, "accept=4") != NULL,
        "bridge request text intent summary tracks accepts"
    );
    check(
        strstr(intent_summary, "last_req=capability_report") != NULL,
        "bridge request text intent summary tracks last normalized request"
    );
    check(
        strstr(intent_summary, "live=blocked") != NULL,
        "bridge request text intent summary keeps live closed"
    );
    check(
        strstr(intent_summary, "serial=closed") != NULL,
        "bridge request text intent summary keeps serial closed"
    );
    check(
        strstr(intent_summary, "abi=closed") != NULL,
        "bridge request text intent summary keeps bridge ABI closed"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_summary(
            &intent,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "bridge request text intent summary rejects small caller capacity"
    );

    envelope_attempted_before = envelope.attempted;
    envelope_rejected_before = envelope.rejected;
    router_attempted_before = router.attempted;
    router_rejected_before = router.rejected;

    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            NULL,
            &envelope,
            &router,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge request text intent rejects without envelope accounting"
    );
    check(envelope.attempted == envelope_attempted_before, "null request text intent does not call envelope");
    check(router.attempted == router_attempted_before, "null request text intent does not call router");

    for (index = 0u; index < sizeof(malformed_texts) / sizeof(malformed_texts[0]); ++index) {
        check(
            espnow_bbs_mesh_contract_bridge_request_text_intent_route(
                &intent,
                &envelope,
                &router,
                malformed_texts[index],
                &state,
                summary,
                sizeof(summary),
                NULL
            ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
            "malformed bridge request text intent rejects before envelope"
        );
        check(intent.normalized_request[0] == '\0', "malformed bridge request text intent clears normalized request");
    }
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            &envelope,
            &router,
            overlong_request,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
        "overlong bridge request text intent rejects before envelope"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            &envelope,
            &router,
            nul_request,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
        "leading NUL bridge request text intent rejects as blank request"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            &envelope,
            &router,
            non_ascii_request,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII bridge request text intent rejects before envelope"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            &envelope,
            &router,
            NULL,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge request text intent text rejects before envelope"
    );
    check(envelope.attempted == envelope_attempted_before, "invalid request text intent does not call envelope");
    check(envelope.rejected == envelope_rejected_before, "invalid request text intent does not change envelope rejects");
    check(router.attempted == router_attempted_before, "invalid request text intent does not call router");
    check(router.rejected == router_rejected_before, "invalid request text intent does not change router rejects");

    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            NULL,
            &router,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request text intent null envelope rejects after normalization"
    );
    check(router.attempted == router_attempted_before, "null envelope request text intent does not call router");
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            &envelope,
            NULL,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request text intent null router rejects after normalization"
    );
    check(envelope.attempted == envelope_attempted_before + 1u, "null router request text intent reaches envelope once");
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            &envelope,
            &router,
            "discovery_snapshot",
            &state,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "bridge request text intent small output rejects through local route"
    );
    missing = state;
    missing.event_queue = NULL;
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &intent,
            &envelope,
            &router,
            "discovery_events",
            &missing,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request text intent missing queue rejects through local route"
    );
    check(
        envelope.attempted == envelope_attempted_before + 3u,
        "valid rejected request text intent reaches envelope for downstream checks"
    );
    check(
        router.attempted == router_attempted_before + 2u,
        "valid rejected request text intent reaches router when present"
    );
    check(intent.accepted == 4u, "bridge request text intent accepts remain stable on rejects");
    check(intent.rejected > 0u, "bridge request text intent tracks rejected text and downstream rejects");
    check(
        intent.last_status == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "bridge request text intent tracks last downstream reject status"
    );
    check(
        strcmp(intent.normalized_request, "discovery_events") == 0,
        "bridge request text intent keeps last valid rejected request"
    );
    check(queue.count == queue_count_before, "bridge request text intent does not mutate queue count");
    check(queue.dropped == queue_dropped_before, "bridge request text intent does not mutate drops");
    check(owner.accepted == owner_accepted_before, "bridge request text intent does not mutate ingress");
    check(owner.rejected == owner_rejected_before, "bridge request text intent does not mutate owner rejects");
    check(owner.drained == owner_drained_before, "bridge request text intent does not mutate drains");
    check(
        runtime_scheduler.step_count == runtime_steps_before,
        "bridge request text intent does not step runtime scheduler"
    );

    espnow_bbs_mesh_contract_bridge_request_text_intent_reset(&intent);
    check(intent.attempted == 0u, "bridge request text intent reset clears attempts");
    check(intent.accepted == 0u, "bridge request text intent reset clears accepts");
    check(intent.rejected == 0u, "bridge request text intent reset clears rejects");
    check(intent.normalized_request[0] == '\0', "bridge request text intent reset clears normalized request");
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge request text intent summary rejects"
    );
    check(
        espnow_bbs_mesh_contract_bridge_request_text_intent_summary(&intent, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null bridge request text intent summary output rejects"
    );
}

static void test_persistence_intent_stages_volatile_bridge_output(void)
{
    const char *const request_texts[] = {
        " discovery_snapshot",
        "\tcapability_report\r\n",
    };
    const char *const normalized_requests[] = {
        "discovery_snapshot",
        "capability_report",
    };
    const char *const malformed_texts[] = {
        "",
        "discovery snapshot",
        "{\"request\":\"discovery_snapshot\"}",
        "msg_post",
        "flash",
    };
    espnow_bbs_mesh_contract_event_queue_t queue;
    espnow_bbs_mesh_contract_owner_t owner;
    espnow_bbs_mesh_contract_runtime_scheduler_t runtime_scheduler;
    espnow_bbs_mesh_contract_bridge_summary_state_t state;
    espnow_bbs_mesh_contract_bridge_summary_state_t missing;
    espnow_bbs_mesh_contract_bridge_request_router_t router;
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t envelope;
    espnow_bbs_mesh_contract_bridge_request_text_intent_t intent;
    espnow_bbs_mesh_contract_persistence_intent_t persistence;
    espnow_bbs_mesh_contract_runtime_scheduler_config_t config;
    espnow_bbs_mesh_contract_event_input_t event = {
        "node_seen",
        "n01",
        "persist",
    };
    char summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char persistence_summary[ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u];
    char tiny[8];
    char non_ascii_request[] = {
        'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', 'y', '_',
        's', 'n', 'a', 'p', 's', 'h', 'o', 't', (char)0xff, '\0'
    };
    char overlong_request[
        ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 8u
    ];
    size_t claim_token = 0u;
    size_t queue_count_before;
    size_t queue_dropped_before;
    size_t owner_accepted_before;
    size_t owner_rejected_before;
    size_t owner_drained_before;
    size_t runtime_steps_before;
    size_t staged_bytes = 0u;
    size_t staged_before;
    size_t envelope_attempted_before;
    size_t router_attempted_before;
    size_t line_bytes = 0u;
    size_t index;

    memset(overlong_request, 'a', sizeof(overlong_request) - 1u);
    overlong_request[sizeof(overlong_request) - 1u] = '\0';

    espnow_bbs_mesh_contract_event_queue_init(&queue);
    espnow_bbs_mesh_contract_owner_init(&owner, &queue);
    check(
        espnow_bbs_mesh_contract_owner_claim(
            &owner,
            "mesh_ingress",
            &claim_token
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "persistence intent owner claim accepts"
    );
    check(
        espnow_bbs_mesh_contract_owner_ingest(&owner, claim_token, &event)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "persistence intent fixture enqueues event"
    );
    espnow_bbs_mesh_contract_runtime_scheduler_init(&runtime_scheduler);
    config = runtime_scheduler_valid_config(claim_token);
    check(
        espnow_bbs_mesh_contract_runtime_scheduler_configure(
            &runtime_scheduler,
            &config
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "persistence intent runtime scheduler config accepts"
    );
    state.event_queue = &queue;
    state.owner = &owner;
    state.runtime_scheduler = &runtime_scheduler;

    espnow_bbs_mesh_contract_bridge_request_router_init(&router);
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(&envelope);
    espnow_bbs_mesh_contract_bridge_request_text_intent_init(&intent);
    espnow_bbs_mesh_contract_persistence_intent_init(&persistence);
    check(persistence.attempted == 0u, "persistence intent init clears attempts");
    check(persistence.staged == 0u, "persistence intent init clears staged count");
    check(persistence.rejected == 0u, "persistence intent init clears rejects");
    check(persistence.staged_bytes == 0u, "persistence intent init clears bytes");
    check(persistence.last_status == ESPNOW_BBS_MESH_CONTRACT_OK, "persistence intent init sets ok status");

    queue_count_before = queue.count;
    queue_dropped_before = queue.dropped;
    owner_accepted_before = owner.accepted;
    owner_rejected_before = owner.rejected;
    owner_drained_before = owner.drained;
    runtime_steps_before = runtime_scheduler.step_count;

    for (index = 0u; index < sizeof(request_texts) / sizeof(request_texts[0]); ++index) {
        memset(summary, 0, sizeof(summary));
        line_bytes = 0u;
        check(
            espnow_bbs_mesh_contract_persistence_intent_stage(
                &persistence,
                &intent,
                &envelope,
                &router,
                request_texts[index],
                &state,
                summary,
                sizeof(summary),
                &line_bytes
            ) == ESPNOW_BBS_MESH_CONTRACT_OK,
            "known persistence intent stage accepts"
        );
        staged_bytes += line_bytes;
        check(
            strcmp(persistence.last_request, normalized_requests[index]) == 0,
            "persistence intent stores last normalized request"
        );
        check(
            line_bytes <= ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES,
            "persistence intent staged output stays inside 512-byte bridge budget"
        );
        check(
            espnow_bbs_mesh_contract_summary_status(summary, NULL)
                == ESPNOW_BBS_MESH_CONTRACT_OK,
            "persistence intent staged output is bounded ASCII"
        );
    }

    check(persistence.attempted == 2u, "persistence intent tracks attempts");
    check(persistence.staged == 2u, "persistence intent tracks staged output");
    check(persistence.rejected == 0u, "persistence intent tracks zero rejects");
    check(persistence.staged_bytes == staged_bytes, "persistence intent accumulates staged bytes");
    check(intent.accepted == 2u, "persistence intent uses M4S request text intent");
    check(envelope.would_dispatch == 2u, "persistence intent uses local envelope");
    check(router.accepted == 2u, "persistence intent uses request router");
    check(
        espnow_bbs_mesh_contract_persistence_intent_summary(
            &persistence,
            persistence_summary,
            sizeof(persistence_summary),
            &line_bytes
        ) == ESPNOW_BBS_MESH_CONTRACT_OK,
        "persistence intent summary accepts"
    );
    check(
        strstr(persistence_summary, "persistence=intent") != NULL,
        "persistence intent summary names volatile persistence intent"
    );
    check(
        strstr(persistence_summary, "stage=2") != NULL,
        "persistence intent summary tracks staged count"
    );
    check(
        strstr(persistence_summary, "storage=volatile") != NULL,
        "persistence intent summary keeps storage volatile"
    );
    check(
        strstr(persistence_summary, "nvs=closed") != NULL,
        "persistence intent summary keeps NVS closed"
    );
    check(
        strstr(persistence_summary, "file=closed") != NULL,
        "persistence intent summary keeps file storage closed"
    );
    check(
        strstr(persistence_summary, "live=blocked") != NULL,
        "persistence intent summary keeps live closed"
    );
    check(
        strstr(persistence_summary, "serial=closed") != NULL,
        "persistence intent summary keeps serial closed"
    );
    check(
        strstr(persistence_summary, "abi=closed") != NULL,
        "persistence intent summary keeps bridge ABI closed"
    );
    check(
        espnow_bbs_mesh_contract_persistence_intent_summary(
            &persistence,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "persistence intent summary rejects small caller capacity"
    );

    staged_before = persistence.staged;
    envelope_attempted_before = envelope.attempted;
    router_attempted_before = router.attempted;

    check(
        espnow_bbs_mesh_contract_persistence_intent_stage(
            NULL,
            &intent,
            &envelope,
            &router,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null persistence intent rejects without downstream accounting"
    );
    check(envelope.attempted == envelope_attempted_before, "null persistence intent does not call envelope");
    check(router.attempted == router_attempted_before, "null persistence intent does not call router");

    check(
        espnow_bbs_mesh_contract_persistence_intent_stage(
            &persistence,
            NULL,
            &envelope,
            &router,
            "discovery_snapshot",
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null request text intent rejects before downstream persistence stage"
    );
    check(envelope.attempted == envelope_attempted_before, "null text intent does not call envelope");
    check(router.attempted == router_attempted_before, "null text intent does not call router");

    for (index = 0u; index < sizeof(malformed_texts) / sizeof(malformed_texts[0]); ++index) {
        check(
            espnow_bbs_mesh_contract_persistence_intent_stage(
                &persistence,
                &intent,
                &envelope,
                &router,
                malformed_texts[index],
                &state,
                summary,
                sizeof(summary),
                NULL
            ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
            "malformed persistence intent rejects through request text intent"
        );
    }
    check(envelope.attempted == envelope_attempted_before, "malformed persistence intent does not call envelope");
    check(router.attempted == router_attempted_before, "malformed persistence intent does not call router");
    check(
        espnow_bbs_mesh_contract_persistence_intent_stage(
            &persistence,
            &intent,
            &envelope,
            &router,
            overlong_request,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
        "overlong persistence intent rejects through request text intent"
    );
    check(
        espnow_bbs_mesh_contract_persistence_intent_stage(
            &persistence,
            &intent,
            &envelope,
            &router,
            non_ascii_request,
            &state,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII persistence intent rejects through request text intent"
    );
    check(persistence.staged == staged_before, "rejected persistence intent does not stage output");

    check(
        espnow_bbs_mesh_contract_persistence_intent_stage(
            &persistence,
            &intent,
            &envelope,
            &router,
            "discovery_snapshot",
            &state,
            tiny,
            sizeof(tiny),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
        "persistence intent small output rejects through M4S route"
    );
    missing = state;
    missing.event_queue = NULL;
    check(
        espnow_bbs_mesh_contract_persistence_intent_stage(
            &persistence,
            &intent,
            &envelope,
            &router,
            "discovery_events",
            &missing,
            summary,
            sizeof(summary),
            NULL
        ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "persistence intent missing queue rejects through M4S route"
    );
    check(persistence.staged == staged_before, "downstream rejected persistence intent does not stage output");
    check(
        strcmp(persistence.last_request, "discovery_events") == 0,
        "persistence intent keeps last valid rejected request"
    );
    check(queue.count == queue_count_before, "persistence intent does not mutate queue count");
    check(queue.dropped == queue_dropped_before, "persistence intent does not mutate drops");
    check(owner.accepted == owner_accepted_before, "persistence intent does not mutate ingress");
    check(owner.rejected == owner_rejected_before, "persistence intent does not mutate owner rejects");
    check(owner.drained == owner_drained_before, "persistence intent does not mutate drains");
    check(
        runtime_scheduler.step_count == runtime_steps_before,
        "persistence intent does not step runtime scheduler"
    );

    espnow_bbs_mesh_contract_persistence_intent_reset(&persistence);
    check(persistence.attempted == 0u, "persistence intent reset clears attempts");
    check(persistence.staged == 0u, "persistence intent reset clears staged count");
    check(persistence.rejected == 0u, "persistence intent reset clears rejects");
    check(persistence.staged_bytes == 0u, "persistence intent reset clears staged bytes");
    check(persistence.last_request[0] == '\0', "persistence intent reset clears last request");
    check(
        espnow_bbs_mesh_contract_persistence_intent_summary(NULL, summary, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null persistence intent summary rejects"
    );
    check(
        espnow_bbs_mesh_contract_persistence_intent_summary(&persistence, NULL, sizeof(summary), NULL)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
        "null persistence intent summary output rejects"
    );
}

static void test_secret_marker_and_field_path_rejection(void)
{
    const char *path[] = {"outer", "inner", "pairing_token"};
    const char *safe_path[] = {"outer", "evidence", "node"};
    size_t index;

    for (index = 0; index < espnow_bbs_mesh_contract_secret_marker_count(); ++index) {
        check(
            espnow_bbs_mesh_contract_field_name_status(
                espnow_bbs_mesh_contract_secret_marker_name(index)
            ) == ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD,
            "configured secret marker rejects"
        );
    }
    check(
        espnow_bbs_mesh_contract_field_name_status("root_key")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD,
        "suffix key rejects"
    );
    check(
        espnow_bbs_mesh_contract_field_name_status("session-token")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD,
        "suffix token rejects"
    );
    check(
        espnow_bbs_mesh_contract_field_name_status("wifi_password")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD,
        "password substring rejects"
    );
    check(
        espnow_bbs_mesh_contract_field_name_status("mesh_secret")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD,
        "secret substring rejects"
    );
    check(
        espnow_bbs_mesh_contract_field_name_status("evidence")
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "safe field accepts"
    );
    check(
        espnow_bbs_mesh_contract_field_name_status("node\xff")
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
        "non-ASCII field rejects"
    );
    check(
        espnow_bbs_mesh_contract_field_path_status(path, 3u)
            == ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD,
        "nested secret field rejects"
    );
    check(
        espnow_bbs_mesh_contract_field_path_status(safe_path, 3u)
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "safe nested field path accepts"
    );
}

static void test_live_action_rejection(void)
{
    const char *const blocked[] = {
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
    size_t index;

    for (index = 0; index < sizeof(blocked) / sizeof(blocked[0]); ++index) {
        check(
            espnow_bbs_mesh_contract_live_action_blocked(blocked[index]),
            "blocked live action is classified"
        );
        check(
            espnow_bbs_mesh_contract_live_action_status(blocked[index])
                == ESPNOW_BBS_MESH_CONTRACT_REJECT_LIVE_ACTION,
            "blocked live action rejects"
        );
    }
    check(
        espnow_bbs_mesh_contract_live_action_status("discovery_snapshot")
            == ESPNOW_BBS_MESH_CONTRACT_OK,
        "read-only discovery request is not classified as live action"
    );
}

int main(void)
{
    test_schema_defaults_and_summary_budget();
    test_event_vocabulary_and_labels();
    test_event_queue_fifo_and_caller_owned_state();
    test_event_queue_capacity_and_wraparound();
    test_event_queue_input_rejection();
    test_event_queue_summary_bounds();
    test_owner_names_and_claim_release();
    test_owner_ingest_drain_and_counters();
    test_owner_capacity_and_independence();
    test_owner_summary_bounds();
    test_dispatch_actions_and_stage_bounds();
    test_dispatch_plan_from_owner_deterministic();
    test_dispatch_plan_backpressure_and_owner_rejection();
    test_dispatch_plan_summary_bounds();
    test_scheduler_profiles_and_state_bounds();
    test_scheduler_stage_tick_deterministic();
    test_scheduler_stage_tick_rejection_and_backpressure();
    test_execution_accounting_summary_bounds();
    test_execution_accounting_apply_plan_deterministic();
    test_execution_accounting_rejection_and_backpressure();
    test_runtime_cycle_summary_bounds();
    test_runtime_cycle_step_deterministic();
    test_runtime_cycle_rejection_is_atomic();
    test_runtime_cycle_backpressure_accounting();
    test_runtime_scheduler_config_and_summary_bounds();
    test_runtime_scheduler_step_deterministic();
    test_runtime_scheduler_rejection_is_atomic();
    test_request_and_gate_f_invariance();
    test_bridge_summary_selection_bounds_and_rejections();
    test_bridge_request_router_accounting_and_bounds();
    test_bridge_dispatch_envelope_accounting_and_bounds();
    test_bridge_request_text_intent_normalization_and_bounds();
    test_persistence_intent_stages_volatile_bridge_output();
    test_secret_marker_and_field_path_rejection();
    test_live_action_rejection();
    return finish_tests("test_mesh_discovery_contract");
}
