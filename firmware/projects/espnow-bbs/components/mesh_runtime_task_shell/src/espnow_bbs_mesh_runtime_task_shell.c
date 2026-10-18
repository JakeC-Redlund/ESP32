#include "espnow_bbs_mesh_runtime_task_shell.h"

#include "freertos/idf_additions.h"

static void espnow_bbs_mesh_runtime_task_main(void *argument);
static bool espnow_bbs_mesh_runtime_task_core_valid(BaseType_t core_id);
static const char *espnow_bbs_mesh_runtime_task_name(const char *configured_name);

const char *espnow_bbs_mesh_runtime_task_status_name(
    espnow_bbs_mesh_runtime_task_status_t status
)
{
    switch (status) {
    case ESPNOW_BBS_MESH_RUNTIME_TASK_OK:
        return "ok";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NULL:
        return "reject_null";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_STACK_DEPTH_ZERO:
        return "reject_stack_depth_zero";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PERIOD_ZERO:
        return "reject_period_zero";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PERIOD_TOO_LARGE:
        return "reject_period_too_large";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_CORE:
        return "reject_core";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PRIORITY_ZERO:
        return "reject_priority_zero";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_ALREADY_STARTED:
        return "reject_already_started";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NOT_STARTED:
        return "reject_not_started";
    case ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_CREATE_FAILED:
        return "reject_create_failed";
    default:
        return "reject_unknown";
    }
}

void espnow_bbs_mesh_runtime_task_init(espnow_bbs_mesh_runtime_task_t *task)
{
    if (task == NULL) {
        return;
    }

    task->task_handle = NULL;
    task->runtime_scheduler = NULL;
    task->owner = NULL;
    task->period_ticks = 0u;
    task->attempted = 0u;
    task->completed = 0u;
    task->rejected = 0u;
    task->wake_count = 0u;
    task->stop_requested = false;
    task->running = false;
    task->last_status = ESPNOW_BBS_MESH_RUNTIME_TASK_OK;
    task->last_contract_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

bool espnow_bbs_mesh_runtime_task_is_running(
    const espnow_bbs_mesh_runtime_task_t *task
)
{
    if (task == NULL) {
        return false;
    }

    return task->running;
}

size_t espnow_bbs_mesh_runtime_task_completed_count(
    const espnow_bbs_mesh_runtime_task_t *task
)
{
    if (task == NULL) {
        return 0u;
    }

    return task->completed;
}

espnow_bbs_mesh_runtime_task_status_t espnow_bbs_mesh_runtime_task_config_status(
    const espnow_bbs_mesh_runtime_task_config_t *config
)
{
    if (
        config == NULL ||
        config->runtime_scheduler == NULL ||
        config->owner == NULL ||
        config->stack_buffer == NULL ||
        config->task_buffer == NULL
    ) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NULL;
    }

    if (config->stack_depth_bytes == 0u) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_STACK_DEPTH_ZERO;
    }
    if (config->period_ticks == 0u) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PERIOD_ZERO;
    }
    if (config->period_ticks > ESPNOW_BBS_MESH_RUNTIME_TASK_PERIOD_MAX_TICKS) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PERIOD_TOO_LARGE;
    }
    if (!espnow_bbs_mesh_runtime_task_core_valid(config->core_id)) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_CORE;
    }
    if (config->priority == 0u) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PRIORITY_ZERO;
    }

    return ESPNOW_BBS_MESH_RUNTIME_TASK_OK;
}

espnow_bbs_mesh_runtime_task_status_t espnow_bbs_mesh_runtime_task_start(
    espnow_bbs_mesh_runtime_task_t *task,
    const espnow_bbs_mesh_runtime_task_config_t *config
)
{
    espnow_bbs_mesh_runtime_task_status_t status;

    if (task == NULL) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NULL;
    }
    if (task->task_handle != NULL || task->running) {
        task->last_status = ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_ALREADY_STARTED;
        return task->last_status;
    }

    status = espnow_bbs_mesh_runtime_task_config_status(config);
    if (status != ESPNOW_BBS_MESH_RUNTIME_TASK_OK) {
        task->last_status = status;
        return status;
    }

    task->runtime_scheduler = config->runtime_scheduler;
    task->owner = config->owner;
    task->period_ticks = config->period_ticks;
    task->attempted = 0u;
    task->completed = 0u;
    task->rejected = 0u;
    task->wake_count = 0u;
    task->stop_requested = false;
    task->running = false;
    task->last_contract_status = ESPNOW_BBS_MESH_CONTRACT_OK;

    task->task_handle = xTaskCreateStaticPinnedToCore(
        espnow_bbs_mesh_runtime_task_main,
        espnow_bbs_mesh_runtime_task_name(config->task_name),
        config->stack_depth_bytes,
        task,
        config->priority,
        config->stack_buffer,
        config->task_buffer,
        config->core_id
    );
    if (task->task_handle == NULL) {
        task->runtime_scheduler = NULL;
        task->owner = NULL;
        task->period_ticks = 0u;
        task->last_status = ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_CREATE_FAILED;
        return task->last_status;
    }

    task->last_status = ESPNOW_BBS_MESH_RUNTIME_TASK_OK;
    return task->last_status;
}

espnow_bbs_mesh_runtime_task_status_t espnow_bbs_mesh_runtime_task_request_stop(
    espnow_bbs_mesh_runtime_task_t *task
)
{
    TaskHandle_t task_handle;

    if (task == NULL) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NULL;
    }
    if (task->task_handle == NULL) {
        task->last_status = ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NOT_STARTED;
        return task->last_status;
    }

    task->stop_requested = true;
    task_handle = task->task_handle;
    if (task_handle != NULL) {
        xTaskNotifyGive(task_handle);
    }
    task->last_status = ESPNOW_BBS_MESH_RUNTIME_TASK_OK;
    return task->last_status;
}

static void espnow_bbs_mesh_runtime_task_main(void *argument)
{
    espnow_bbs_mesh_runtime_task_t *task =
        (espnow_bbs_mesh_runtime_task_t *)argument;

    if (task == NULL) {
        vTaskDelete(NULL);
        return;
    }

    task->running = true;
    while (!task->stop_requested) {
        (void)ulTaskNotifyTake(pdTRUE, task->period_ticks);
        if (task->stop_requested) {
            break;
        }

        task->wake_count += 1u;
        task->attempted += 1u;
        task->last_contract_status =
            espnow_bbs_mesh_contract_runtime_scheduler_step(
                task->runtime_scheduler,
                task->owner,
                task->attempted
            );
        if (task->last_contract_status == ESPNOW_BBS_MESH_CONTRACT_OK) {
            task->completed += 1u;
        } else {
            task->rejected += 1u;
        }
    }

    task->running = false;
    task->task_handle = NULL;
    vTaskDelete(NULL);
}

static bool espnow_bbs_mesh_runtime_task_core_valid(BaseType_t core_id)
{
    if (core_id == ESPNOW_BBS_MESH_RUNTIME_TASK_CORE_NONE) {
        return true;
    }

    return taskVALID_CORE_ID(core_id) == pdTRUE;
}

static const char *espnow_bbs_mesh_runtime_task_name(const char *configured_name)
{
    if (configured_name == NULL) {
        return ESPNOW_BBS_MESH_RUNTIME_TASK_NAME;
    }

    return configured_name;
}
