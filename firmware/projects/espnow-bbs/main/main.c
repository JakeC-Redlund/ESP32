#include <stddef.h>

#include "espnow_bbs_mesh_discovery_contract.h"
#include "espnow_bbs_mesh_retained_persistence.h"
#include "espnow_bbs_mesh_startup_lifecycle.h"

#define ESPNOW_BBS_MESH_APP_OWNER_NAME "mesh_ingress"
#define ESPNOW_BBS_MESH_APP_PROFILE_NAME "balanced"
#define ESPNOW_BBS_MESH_APP_TASK_PERIOD_TICKS 1u
#define ESPNOW_BBS_MESH_APP_TASK_YIELD_INTERVAL 2u
#define ESPNOW_BBS_MESH_APP_TASK_STACK_DEPTH 4096u
#define ESPNOW_BBS_MESH_APP_TASK_PRIORITY (tskIDLE_PRIORITY + 1u)
#define ESPNOW_BBS_MESH_APP_TASK_CORE ESPNOW_BBS_MESH_RUNTIME_TASK_CORE_NONE
#define ESPNOW_BBS_MESH_APP_BRIDGE_READINESS_REQUEST "discovery_snapshot"

static espnow_bbs_mesh_contract_event_queue_t s_mesh_event_queue;
static espnow_bbs_mesh_contract_owner_t s_mesh_owner;
static espnow_bbs_mesh_contract_runtime_scheduler_t s_mesh_runtime_scheduler;
static espnow_bbs_mesh_contract_bridge_summary_state_t
    s_mesh_bridge_summary_state;
static espnow_bbs_mesh_contract_bridge_request_router_t s_mesh_bridge_router;
static espnow_bbs_mesh_contract_bridge_dispatch_envelope_t s_mesh_bridge_envelope;
static espnow_bbs_mesh_contract_bridge_request_text_intent_t
    s_mesh_bridge_request_intent;
static espnow_bbs_mesh_contract_persistence_intent_t s_mesh_retained_intent;
static espnow_bbs_mesh_retained_persistence_storage_t s_mesh_retained_storage;
static espnow_bbs_mesh_startup_lifecycle_t s_mesh_lifecycle;
static StaticTask_t s_mesh_task_buffer;
static StackType_t s_mesh_task_stack[ESPNOW_BBS_MESH_APP_TASK_STACK_DEPTH];
static char s_mesh_bridge_readiness_summary[
    ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u
];
static volatile espnow_bbs_mesh_contract_status_t s_mesh_app_contract_status;
static volatile espnow_bbs_mesh_contract_status_t s_mesh_app_bridge_status;
static volatile espnow_bbs_mesh_retained_persistence_status_t
    s_mesh_app_retained_init_status;
static volatile espnow_bbs_mesh_retained_persistence_status_t
    s_mesh_app_retained_load_status;
static volatile espnow_bbs_mesh_startup_lifecycle_status_t
    s_mesh_app_lifecycle_status;
static volatile size_t s_mesh_app_bridge_line_bytes;

static espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_app_configure_contract(
    size_t *claim_token
);
static void espnow_bbs_mesh_app_capture_bridge_readiness(void);
static void espnow_bbs_mesh_app_recover_retained_intent(void);
static espnow_bbs_mesh_startup_lifecycle_status_t espnow_bbs_mesh_app_start(void);

void app_main(void)
{
    /*
     * Gate M4O wires accepted M4N startup lifecycle source into app_main for
     * no-flash build proof only. Gate M4W adds source-only retained recovery
     * status capture before lifecycle start. Gate M4X adds source-only local
     * bridge-readiness status capture for a fixed accepted discovery request.
     * Runtime/live behavior remains unaccepted until a separate Tier 3 gate;
     * reboot durability and live NVS execution remain unaccepted until
     * hardware proof; live bridge dispatch remains unaccepted until hardware
     * proof.
     */
    s_mesh_app_lifecycle_status = espnow_bbs_mesh_app_start();
}

static espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_app_configure_contract(
    size_t *claim_token
)
{
    espnow_bbs_mesh_contract_runtime_scheduler_config_t scheduler_config = {
        ESPNOW_BBS_MESH_APP_OWNER_NAME,
        ESPNOW_BBS_MESH_APP_PROFILE_NAME,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY,
        0u,
        ESPNOW_BBS_MESH_APP_TASK_PERIOD_TICKS,
        ESPNOW_BBS_MESH_APP_TASK_YIELD_INTERVAL,
        0u,
        0u,
        ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE,
        false,
    };
    espnow_bbs_mesh_contract_status_t status;

    if (claim_token == NULL) {
        return ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL;
    }

    espnow_bbs_mesh_contract_event_queue_init(&s_mesh_event_queue);
    espnow_bbs_mesh_contract_owner_init(&s_mesh_owner, &s_mesh_event_queue);
    espnow_bbs_mesh_contract_runtime_scheduler_init(&s_mesh_runtime_scheduler);

    status = espnow_bbs_mesh_contract_owner_claim(
        &s_mesh_owner,
        ESPNOW_BBS_MESH_APP_OWNER_NAME,
        claim_token
    );
    if (status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return status;
    }

    scheduler_config.claim_token = *claim_token;
    return espnow_bbs_mesh_contract_runtime_scheduler_configure(
        &s_mesh_runtime_scheduler,
        &scheduler_config
    );
}

static void espnow_bbs_mesh_app_capture_bridge_readiness(void)
{
    size_t line_bytes = 0u;

    s_mesh_bridge_summary_state.event_queue = &s_mesh_event_queue;
    s_mesh_bridge_summary_state.owner = &s_mesh_owner;
    s_mesh_bridge_summary_state.runtime_scheduler = &s_mesh_runtime_scheduler;
    espnow_bbs_mesh_contract_bridge_request_router_init(&s_mesh_bridge_router);
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(
        &s_mesh_bridge_envelope
    );
    espnow_bbs_mesh_contract_bridge_request_text_intent_init(
        &s_mesh_bridge_request_intent
    );

    s_mesh_app_bridge_status =
        espnow_bbs_mesh_contract_bridge_request_text_intent_route(
            &s_mesh_bridge_request_intent,
            &s_mesh_bridge_envelope,
            &s_mesh_bridge_router,
            ESPNOW_BBS_MESH_APP_BRIDGE_READINESS_REQUEST,
            &s_mesh_bridge_summary_state,
            s_mesh_bridge_readiness_summary,
            sizeof(s_mesh_bridge_readiness_summary),
            &line_bytes
        );
    s_mesh_app_bridge_line_bytes = line_bytes;
}

static void espnow_bbs_mesh_app_recover_retained_intent(void)
{
    espnow_bbs_mesh_contract_persistence_intent_init(&s_mesh_retained_intent);
    espnow_bbs_mesh_retained_persistence_storage_context_init(
        &s_mesh_retained_storage
    );

    s_mesh_app_retained_init_status =
        espnow_bbs_mesh_retained_persistence_storage_init(
            &s_mesh_retained_storage
        );
    if (s_mesh_app_retained_init_status !=
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        s_mesh_app_retained_load_status =
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED;
        return;
    }

    s_mesh_app_retained_load_status =
        espnow_bbs_mesh_retained_persistence_storage_load(
            &s_mesh_retained_storage,
            &s_mesh_retained_intent
        );
}

static espnow_bbs_mesh_startup_lifecycle_status_t espnow_bbs_mesh_app_start(void)
{
    espnow_bbs_mesh_runtime_task_config_t task_config = {0};
    espnow_bbs_mesh_startup_lifecycle_status_t lifecycle_status;
    size_t claim_token = 0u;

    s_mesh_app_contract_status =
        espnow_bbs_mesh_app_configure_contract(&claim_token);
    if (s_mesh_app_contract_status != ESPNOW_BBS_MESH_CONTRACT_OK) {
        return ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_TASK_CONFIG;
    }

    espnow_bbs_mesh_app_recover_retained_intent();
    espnow_bbs_mesh_app_capture_bridge_readiness();

    task_config.runtime_scheduler = &s_mesh_runtime_scheduler;
    task_config.owner = &s_mesh_owner;
    task_config.task_name = ESPNOW_BBS_MESH_RUNTIME_TASK_NAME;
    task_config.stack_buffer = s_mesh_task_stack;
    task_config.task_buffer = &s_mesh_task_buffer;
    task_config.stack_depth_bytes = ESPNOW_BBS_MESH_APP_TASK_STACK_DEPTH;
    task_config.priority = ESPNOW_BBS_MESH_APP_TASK_PRIORITY;
    task_config.core_id = ESPNOW_BBS_MESH_APP_TASK_CORE;
    task_config.period_ticks = ESPNOW_BBS_MESH_APP_TASK_PERIOD_TICKS;

    espnow_bbs_mesh_startup_lifecycle_init(&s_mesh_lifecycle);
    lifecycle_status = espnow_bbs_mesh_startup_lifecycle_configure(
        &s_mesh_lifecycle,
        &task_config
    );
    if (lifecycle_status != ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK) {
        return lifecycle_status;
    }

    (void)claim_token;
    return espnow_bbs_mesh_startup_lifecycle_start(&s_mesh_lifecycle);
}
