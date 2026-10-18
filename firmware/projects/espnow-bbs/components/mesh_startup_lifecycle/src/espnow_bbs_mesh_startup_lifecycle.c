#include "espnow_bbs_mesh_startup_lifecycle.h"

static void espnow_bbs_mesh_startup_lifecycle_clear_config(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
);
static bool espnow_bbs_mesh_startup_lifecycle_can_deinit(
    espnow_bbs_mesh_startup_lifecycle_state_t state
);

const char *espnow_bbs_mesh_startup_lifecycle_state_name(
    espnow_bbs_mesh_startup_lifecycle_state_t state
)
{
    switch (state) {
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_UNINITIALIZED:
        return "uninitialized";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_CONFIGURED:
        return "configured";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STARTING:
        return "starting";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_RUNNING:
        return "running";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOP_REQUESTED:
        return "stop_requested";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOPPED:
        return "stopped";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_FAILED:
        return "failed";
    default:
        return "unknown";
    }
}

const char *espnow_bbs_mesh_startup_lifecycle_status_name(
    espnow_bbs_mesh_startup_lifecycle_status_t status
)
{
    switch (status) {
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK:
        return "ok";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_NULL:
        return "reject_null";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_TASK_CONFIG:
        return "reject_task_config";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_CONFIGURE_STATE:
        return "reject_configure_state";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_START_STATE:
        return "reject_start_state";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_STOP_STATE:
        return "reject_stop_state";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_DEINIT_STATE:
        return "reject_deinit_state";
    case ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_TASK_START_FAILED:
        return "reject_task_start_failed";
    default:
        return "reject_unknown";
    }
}

void espnow_bbs_mesh_startup_lifecycle_init(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
)
{
    if (lifecycle == NULL) {
        return;
    }

    lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_UNINITIALIZED;
    espnow_bbs_mesh_runtime_task_init(&lifecycle->runtime_task);
    espnow_bbs_mesh_startup_lifecycle_clear_config(lifecycle);
    lifecycle->configured_count = 0u;
    lifecycle->started_count = 0u;
    lifecycle->stop_requested_count = 0u;
    lifecycle->stopped_count = 0u;
    lifecycle->failed_count = 0u;
    lifecycle->last_status = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK;
    lifecycle->last_task_status = ESPNOW_BBS_MESH_RUNTIME_TASK_OK;
}

espnow_bbs_mesh_startup_lifecycle_state_t
espnow_bbs_mesh_startup_lifecycle_state(
    const espnow_bbs_mesh_startup_lifecycle_t *lifecycle
)
{
    if (lifecycle == NULL) {
        return ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_UNINITIALIZED;
    }

    return lifecycle->state;
}

espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_configure(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle,
    const espnow_bbs_mesh_runtime_task_config_t *task_config
)
{
    espnow_bbs_mesh_runtime_task_status_t task_status;

    if (lifecycle == NULL || task_config == NULL) {
        return ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_NULL;
    }
    if (
        lifecycle->state != ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_UNINITIALIZED &&
        lifecycle->state != ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOPPED
    ) {
        lifecycle->last_status =
            ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_CONFIGURE_STATE;
        return lifecycle->last_status;
    }

    task_status = espnow_bbs_mesh_runtime_task_config_status(task_config);
    lifecycle->last_task_status = task_status;
    if (task_status != ESPNOW_BBS_MESH_RUNTIME_TASK_OK) {
        lifecycle->last_status =
            ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_TASK_CONFIG;
        return lifecycle->last_status;
    }

    espnow_bbs_mesh_runtime_task_init(&lifecycle->runtime_task);
    lifecycle->task_config = *task_config;
    lifecycle->has_config = true;
    lifecycle->configured_count += 1u;
    lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_CONFIGURED;
    lifecycle->last_status = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK;
    return lifecycle->last_status;
}

espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_start(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
)
{
    espnow_bbs_mesh_runtime_task_status_t task_status;

    if (lifecycle == NULL) {
        return ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_NULL;
    }
    if (
        lifecycle->state != ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_CONFIGURED ||
        !lifecycle->has_config
    ) {
        lifecycle->last_status =
            ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_START_STATE;
        return lifecycle->last_status;
    }

    lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STARTING;
    task_status = espnow_bbs_mesh_runtime_task_start(
        &lifecycle->runtime_task,
        &lifecycle->task_config
    );
    lifecycle->last_task_status = task_status;
    if (task_status != ESPNOW_BBS_MESH_RUNTIME_TASK_OK) {
        espnow_bbs_mesh_runtime_task_init(&lifecycle->runtime_task);
        espnow_bbs_mesh_startup_lifecycle_clear_config(lifecycle);
        lifecycle->failed_count += 1u;
        lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_FAILED;
        lifecycle->last_status =
            ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_TASK_START_FAILED;
        return lifecycle->last_status;
    }

    lifecycle->started_count += 1u;
    lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_RUNNING;
    lifecycle->last_status = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK;
    return lifecycle->last_status;
}

espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_request_stop(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
)
{
    espnow_bbs_mesh_runtime_task_status_t task_status;

    if (lifecycle == NULL) {
        return ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_NULL;
    }
    if (lifecycle->state == ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOP_REQUESTED) {
        lifecycle->last_status = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK;
        return lifecycle->last_status;
    }
    if (lifecycle->state != ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_RUNNING) {
        lifecycle->last_status =
            ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_STOP_STATE;
        return lifecycle->last_status;
    }

    task_status = espnow_bbs_mesh_runtime_task_request_stop(
        &lifecycle->runtime_task
    );
    lifecycle->last_task_status = task_status;
    if (task_status != ESPNOW_BBS_MESH_RUNTIME_TASK_OK) {
        lifecycle->failed_count += 1u;
        lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_FAILED;
        lifecycle->last_status =
            ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_STOP_STATE;
        return lifecycle->last_status;
    }

    lifecycle->stop_requested_count += 1u;
    lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOP_REQUESTED;
    lifecycle->last_status = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK;
    return lifecycle->last_status;
}

espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_poll(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
)
{
    if (lifecycle == NULL) {
        return ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_NULL;
    }

    if (
        lifecycle->state == ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOP_REQUESTED &&
        lifecycle->runtime_task.task_handle == NULL &&
        !espnow_bbs_mesh_runtime_task_is_running(&lifecycle->runtime_task)
    ) {
        espnow_bbs_mesh_runtime_task_init(&lifecycle->runtime_task);
        espnow_bbs_mesh_startup_lifecycle_clear_config(lifecycle);
        lifecycle->stopped_count += 1u;
        lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOPPED;
    }

    lifecycle->last_status = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK;
    return lifecycle->last_status;
}

espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_deinit(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
)
{
    if (lifecycle == NULL) {
        return ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_startup_lifecycle_can_deinit(lifecycle->state)) {
        lifecycle->last_status =
            ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_DEINIT_STATE;
        return lifecycle->last_status;
    }

    espnow_bbs_mesh_runtime_task_init(&lifecycle->runtime_task);
    espnow_bbs_mesh_startup_lifecycle_clear_config(lifecycle);
    lifecycle->state = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_UNINITIALIZED;
    lifecycle->last_task_status = ESPNOW_BBS_MESH_RUNTIME_TASK_OK;
    lifecycle->last_status = ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK;
    return lifecycle->last_status;
}

static void espnow_bbs_mesh_startup_lifecycle_clear_config(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
)
{
    lifecycle->task_config = (espnow_bbs_mesh_runtime_task_config_t){0};
    lifecycle->has_config = false;
}

static bool espnow_bbs_mesh_startup_lifecycle_can_deinit(
    espnow_bbs_mesh_startup_lifecycle_state_t state
)
{
    return (
        state == ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_CONFIGURED ||
        state == ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOPPED ||
        state == ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_FAILED
    );
}
