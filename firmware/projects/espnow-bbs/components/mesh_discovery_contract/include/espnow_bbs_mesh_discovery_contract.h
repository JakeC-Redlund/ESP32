#ifndef ESPNOW_BBS_MESH_DISCOVERY_CONTRACT_H
#define ESPNOW_BBS_MESH_DISCOVERY_CONTRACT_H

#include <stdbool.h>
#include <stddef.h>

#define ESPNOW_BBS_MESH_DISCOVERY_SCHEMA "mesh_discovery.v1"
#define ESPNOW_BBS_MESH_DISCOVERY_TRANSPORT "esp-wifi-mesh"
#define ESPNOW_BBS_MESH_DISCOVERY_MODE "sim"
#define ESPNOW_BBS_MESH_DISCOVERY_ADMIN_GATE "disabled"
#define ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES 512u
#define ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY 16u
#define ESPNOW_BBS_MESH_CONTRACT_EVENT_NAME_MAX_BYTES 16u
#define ESPNOW_BBS_MESH_CONTRACT_NODE_ID_MAX_BYTES 8u
#define ESPNOW_BBS_MESH_CONTRACT_DETAIL_MAX_BYTES 32u
#define ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES 16u
#define ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY 8u
#define ESPNOW_BBS_MESH_CONTRACT_DISPATCH_ACTION_NAME_MAX_BYTES 16u
#define ESPNOW_BBS_MESH_CONTRACT_DISPATCH_BUDGET_MAX 16u
#define ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_PROFILE_NAME_MAX_BYTES 16u
#define ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_TICK_MAX 64u
#define ESPNOW_BBS_MESH_CONTRACT_EXECUTION_CYCLE_MAX 64u
#define ESPNOW_BBS_MESH_CONTRACT_RUNTIME_CYCLE_MAX 64u
#define ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_POLICY_NAME_MAX_BYTES 16u
#define ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_PERIOD_MAX 64u
#define ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE (-1)
#define ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY "caller_invoked"
#define ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY "task_none"
#define ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY "timer_none"
#define ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES 32u

typedef enum {
    ESPNOW_BBS_MESH_CONTRACT_OK = 0,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_EVENT,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_REQUEST,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_NON_ASCII,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_LINE_TOO_LONG,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_LIVE_ACTION,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_NODE_TOO_LONG,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_DETAIL_TOO_LONG,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_SUMMARY_CAPACITY,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_OWNER,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NAME_TOO_LONG,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_UNBOUND,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_ALREADY_CLAIMED,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_NOT_CLAIMED,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_DISPATCH_ACTION,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_ACTION_NAME_TOO_LONG,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_PLAN_FULL,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_ZERO,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_BUDGET_TOO_LARGE,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_UNKNOWN_SCHEDULER_PROFILE,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_PROFILE_NAME_TOO_LONG,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_ZERO,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_TOO_LARGE,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_ZERO,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_CYCLE_TOO_LARGE,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_PLAN_EMPTY,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_STEP_NOT_READY,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_OWNER_MISMATCH,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_ZERO,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_TOO_LARGE,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_POLICY_NAME_TOO_LONG,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_UNKNOWN_POLICY,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_ZERO,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_PERIOD_TOO_LARGE,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN,
    ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_NOT_CONFIGURED
} espnow_bbs_mesh_contract_status_t;

typedef enum {
    ESPNOW_BBS_MESH_CONTRACT_OWNER_RELEASED = 0,
    ESPNOW_BBS_MESH_CONTRACT_OWNER_CLAIMED
} espnow_bbs_mesh_contract_owner_state_t;

typedef struct {
    const char *event_name;
    const char *node_id;
    const char *detail;
} espnow_bbs_mesh_contract_event_input_t;

typedef struct {
    char event_name[ESPNOW_BBS_MESH_CONTRACT_EVENT_NAME_MAX_BYTES + 1u];
    char event_label[3u];
    char node_id[ESPNOW_BBS_MESH_CONTRACT_NODE_ID_MAX_BYTES + 1u];
    char detail[ESPNOW_BBS_MESH_CONTRACT_DETAIL_MAX_BYTES + 1u];
    size_t sequence;
} espnow_bbs_mesh_contract_event_record_t;

typedef struct {
    espnow_bbs_mesh_contract_event_record_t
        records[ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY];
    size_t head;
    size_t count;
    size_t next_sequence;
    size_t dropped;
} espnow_bbs_mesh_contract_event_queue_t;

typedef struct {
    espnow_bbs_mesh_contract_event_queue_t *queue;
    char owner_name[ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES + 1u];
    espnow_bbs_mesh_contract_owner_state_t state;
    espnow_bbs_mesh_contract_status_t last_status;
    size_t next_claim_token;
    size_t active_claim_token;
    size_t accepted;
    size_t rejected;
    size_t drained;
} espnow_bbs_mesh_contract_owner_t;

typedef struct {
    char action_name[ESPNOW_BBS_MESH_CONTRACT_DISPATCH_ACTION_NAME_MAX_BYTES + 1u];
    char action_label[3u];
    char owner_name[ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES + 1u];
    size_t requested;
    size_t planned;
    espnow_bbs_mesh_contract_status_t status;
} espnow_bbs_mesh_contract_dispatch_step_t;

typedef struct {
    espnow_bbs_mesh_contract_dispatch_step_t
        steps[ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY];
    size_t count;
    size_t requested;
    size_t planned;
    size_t rejected;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_dispatch_plan_t;

typedef struct {
    char profile_name[ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_PROFILE_NAME_MAX_BYTES + 1u];
    char owner_name[ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES + 1u];
    size_t last_tick;
    size_t tick_count;
    size_t requested;
    size_t planned;
    size_t rejected;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_scheduler_state_t;

typedef struct {
    char owner_name[ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES + 1u];
    size_t last_cycle;
    size_t cycle_count;
    size_t applied;
    size_t skipped;
    size_t drain_ready;
    size_t ingest_ready;
    size_t snapshot_ready;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_execution_accounting_t;

typedef struct {
    char profile_name[ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_PROFILE_NAME_MAX_BYTES + 1u];
    char owner_name[ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES + 1u];
    size_t last_cycle;
    size_t cycle_count;
    size_t scheduled;
    size_t applied;
    size_t skipped;
    size_t rejected;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_runtime_cycle_t;

typedef struct {
    const char *owner_name;
    const char *profile_name;
    const char *invoke_policy;
    const char *task_policy;
    const char *timer_policy;
    size_t claim_token;
    size_t period_ticks;
    size_t yield_interval;
    size_t priority;
    size_t stack_bytes;
    int core_id;
    bool static_state;
} espnow_bbs_mesh_contract_runtime_scheduler_config_t;

typedef struct {
    espnow_bbs_mesh_contract_scheduler_state_t scheduler;
    espnow_bbs_mesh_contract_dispatch_plan_t plan;
    espnow_bbs_mesh_contract_execution_accounting_t accounting;
    espnow_bbs_mesh_contract_runtime_cycle_t runtime;
    char owner_name[ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES + 1u];
    char profile_name[ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_PROFILE_NAME_MAX_BYTES + 1u];
    char invoke_policy[ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_POLICY_NAME_MAX_BYTES + 1u];
    char task_policy[ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_POLICY_NAME_MAX_BYTES + 1u];
    char timer_policy[ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_POLICY_NAME_MAX_BYTES + 1u];
    size_t claim_token;
    size_t period_ticks;
    size_t yield_interval;
    size_t step_count;
    size_t scheduled;
    size_t applied;
    size_t skipped;
    size_t rejected;
    size_t yield_due;
    int core_id;
    bool configured;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_runtime_scheduler_t;

typedef struct {
    const espnow_bbs_mesh_contract_event_queue_t *event_queue;
    const espnow_bbs_mesh_contract_owner_t *owner;
    const espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler;
} espnow_bbs_mesh_contract_bridge_summary_state_t;

typedef struct {
    char last_request[ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 1u];
    size_t attempted;
    size_t accepted;
    size_t rejected;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_bridge_request_router_t;

typedef struct {
    char last_request[ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 1u];
    size_t attempted;
    size_t would_dispatch;
    size_t rejected;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_bridge_dispatch_envelope_t;

typedef struct {
    char normalized_request[
        ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 1u
    ];
    size_t attempted;
    size_t accepted;
    size_t rejected;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_bridge_request_text_intent_t;

typedef struct {
    char last_request[ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 1u];
    size_t attempted;
    size_t staged;
    size_t rejected;
    size_t staged_bytes;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_contract_persistence_intent_t;

const char *espnow_bbs_mesh_contract_status_name(
    espnow_bbs_mesh_contract_status_t status
);

size_t espnow_bbs_mesh_contract_event_count(void);
const char *espnow_bbs_mesh_contract_event_name(size_t index);
const char *espnow_bbs_mesh_contract_event_label(const char *event_name);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_status(
    const char *event_name
);
void espnow_bbs_mesh_contract_event_queue_init(
    espnow_bbs_mesh_contract_event_queue_t *queue
);
size_t espnow_bbs_mesh_contract_event_queue_count(
    const espnow_bbs_mesh_contract_event_queue_t *queue
);
size_t espnow_bbs_mesh_contract_event_queue_dropped(
    const espnow_bbs_mesh_contract_event_queue_t *queue
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_push(
    espnow_bbs_mesh_contract_event_queue_t *queue,
    const espnow_bbs_mesh_contract_event_input_t *event
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_peek(
    const espnow_bbs_mesh_contract_event_queue_t *queue,
    size_t offset,
    espnow_bbs_mesh_contract_event_record_t *record
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_pop(
    espnow_bbs_mesh_contract_event_queue_t *queue,
    espnow_bbs_mesh_contract_event_record_t *record
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_event_queue_summary(
    const espnow_bbs_mesh_contract_event_queue_t *queue,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);

size_t espnow_bbs_mesh_contract_owner_name_count(void);
const char *espnow_bbs_mesh_contract_owner_name(size_t index);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_name_status(
    const char *owner_name
);
void espnow_bbs_mesh_contract_owner_init(
    espnow_bbs_mesh_contract_owner_t *owner,
    espnow_bbs_mesh_contract_event_queue_t *queue
);
void espnow_bbs_mesh_contract_owner_reset(
    espnow_bbs_mesh_contract_owner_t *owner
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_claim(
    espnow_bbs_mesh_contract_owner_t *owner,
    const char *owner_name,
    size_t *claim_token
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_release(
    espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_ingest(
    espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    const espnow_bbs_mesh_contract_event_input_t *event
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_drain(
    espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    espnow_bbs_mesh_contract_event_record_t *record
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_owner_summary(
    const espnow_bbs_mesh_contract_owner_t *owner,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);

size_t espnow_bbs_mesh_contract_dispatch_action_count(void);
const char *espnow_bbs_mesh_contract_dispatch_action_name(size_t index);
const char *espnow_bbs_mesh_contract_dispatch_action_label(
    const char *action_name
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_action_status(
    const char *action_name
);
void espnow_bbs_mesh_contract_dispatch_plan_init(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan
);
void espnow_bbs_mesh_contract_dispatch_plan_reset(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan
);
size_t espnow_bbs_mesh_contract_dispatch_plan_count(
    const espnow_bbs_mesh_contract_dispatch_plan_t *plan
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_plan_stage(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    const char *owner_name,
    const char *action_name,
    size_t requested,
    size_t planned,
    espnow_bbs_mesh_contract_status_t step_status
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_plan_from_owner(
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    size_t ingest_budget,
    size_t drain_budget,
    size_t snapshot_budget
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_dispatch_plan_summary(
    const espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);

size_t espnow_bbs_mesh_contract_scheduler_profile_count(void);
const char *espnow_bbs_mesh_contract_scheduler_profile_name(size_t index);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_scheduler_profile_status(
    const char *profile_name
);
void espnow_bbs_mesh_contract_scheduler_state_init(
    espnow_bbs_mesh_contract_scheduler_state_t *state
);
void espnow_bbs_mesh_contract_scheduler_state_reset(
    espnow_bbs_mesh_contract_scheduler_state_t *state
);
size_t espnow_bbs_mesh_contract_scheduler_state_tick_count(
    const espnow_bbs_mesh_contract_scheduler_state_t *state
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_scheduler_stage_tick(
    espnow_bbs_mesh_contract_scheduler_state_t *state,
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    const char *profile_name,
    size_t tick
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_scheduler_state_summary(
    const espnow_bbs_mesh_contract_scheduler_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);

void espnow_bbs_mesh_contract_execution_accounting_init(
    espnow_bbs_mesh_contract_execution_accounting_t *accounting
);
void espnow_bbs_mesh_contract_execution_accounting_reset(
    espnow_bbs_mesh_contract_execution_accounting_t *accounting
);
size_t espnow_bbs_mesh_contract_execution_accounting_cycle_count(
    const espnow_bbs_mesh_contract_execution_accounting_t *accounting
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_execution_accounting_apply_plan(
    espnow_bbs_mesh_contract_execution_accounting_t *accounting,
    const espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    size_t cycle
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_execution_accounting_summary(
    const espnow_bbs_mesh_contract_execution_accounting_t *accounting,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);

void espnow_bbs_mesh_contract_runtime_cycle_init(
    espnow_bbs_mesh_contract_runtime_cycle_t *runtime
);
void espnow_bbs_mesh_contract_runtime_cycle_reset(
    espnow_bbs_mesh_contract_runtime_cycle_t *runtime
);
size_t espnow_bbs_mesh_contract_runtime_cycle_count(
    const espnow_bbs_mesh_contract_runtime_cycle_t *runtime
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_cycle_step(
    espnow_bbs_mesh_contract_runtime_cycle_t *runtime,
    espnow_bbs_mesh_contract_scheduler_state_t *scheduler,
    espnow_bbs_mesh_contract_dispatch_plan_t *plan,
    espnow_bbs_mesh_contract_execution_accounting_t *accounting,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t claim_token,
    const char *profile_name,
    size_t cycle
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_cycle_summary(
    const espnow_bbs_mesh_contract_runtime_cycle_t *runtime,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);

void espnow_bbs_mesh_contract_runtime_scheduler_init(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
);
void espnow_bbs_mesh_contract_runtime_scheduler_reset(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
);
size_t espnow_bbs_mesh_contract_runtime_scheduler_step_count(
    const espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_config_status(
    const espnow_bbs_mesh_contract_runtime_scheduler_config_t *config
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_configure(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler,
    const espnow_bbs_mesh_contract_runtime_scheduler_config_t *config
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_step(
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler,
    const espnow_bbs_mesh_contract_owner_t *owner,
    size_t cycle
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_runtime_scheduler_summary(
    const espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_bridge_summary(
    const char *request_name,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
void espnow_bbs_mesh_contract_bridge_request_router_init(
    espnow_bbs_mesh_contract_bridge_request_router_t *router
);
void espnow_bbs_mesh_contract_bridge_request_router_reset(
    espnow_bbs_mesh_contract_bridge_request_router_t *router
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_bridge_request_route(
    espnow_bbs_mesh_contract_bridge_request_router_t *router,
    const char *request_name,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_request_router_summary(
    const espnow_bbs_mesh_contract_bridge_request_router_t *router,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
void espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope
);
void espnow_bbs_mesh_contract_bridge_dispatch_envelope_reset(
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope
);
espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(
    espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope,
    espnow_bbs_mesh_contract_bridge_request_router_t *router,
    const char *request_name,
    const espnow_bbs_mesh_contract_bridge_summary_state_t *state,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary(
    const espnow_bbs_mesh_contract_bridge_dispatch_envelope_t *envelope,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
void espnow_bbs_mesh_contract_bridge_request_text_intent_init(
    espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent
);
void espnow_bbs_mesh_contract_bridge_request_text_intent_reset(
    espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent
);
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
);
espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_bridge_request_text_intent_summary(
    const espnow_bbs_mesh_contract_bridge_request_text_intent_t *intent,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
void espnow_bbs_mesh_contract_persistence_intent_init(
    espnow_bbs_mesh_contract_persistence_intent_t *persistence
);
void espnow_bbs_mesh_contract_persistence_intent_reset(
    espnow_bbs_mesh_contract_persistence_intent_t *persistence
);
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
);
espnow_bbs_mesh_contract_status_t
espnow_bbs_mesh_contract_persistence_intent_summary(
    const espnow_bbs_mesh_contract_persistence_intent_t *persistence,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);

size_t espnow_bbs_mesh_contract_request_count(void);
const char *espnow_bbs_mesh_contract_request_name(size_t index);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_request_status(
    const char *request_name
);

size_t espnow_bbs_mesh_contract_bridge_request_count(void);
const char *espnow_bbs_mesh_contract_bridge_request_name(size_t index);

size_t espnow_bbs_mesh_contract_gate_f_service_count(void);
const char *espnow_bbs_mesh_contract_gate_f_service_name(size_t index);
int espnow_bbs_mesh_contract_gate_f_service_code(size_t index);

size_t espnow_bbs_mesh_contract_secret_marker_count(void);
const char *espnow_bbs_mesh_contract_secret_marker_name(size_t index);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_field_name_status(
    const char *field_name
);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_field_path_status(
    const char *const *field_names,
    size_t field_count
);

bool espnow_bbs_mesh_contract_live_action_blocked(const char *request_name);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_live_action_status(
    const char *request_name
);

bool espnow_bbs_mesh_contract_is_ascii(const char *value);
espnow_bbs_mesh_contract_status_t espnow_bbs_mesh_contract_summary_status(
    const char *summary,
    size_t *line_bytes
);

#endif
