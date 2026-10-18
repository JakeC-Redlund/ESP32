#ifndef ESPNOW_BBS_MESH_RUNTIME_TASK_SHELL_H
#define ESPNOW_BBS_MESH_RUNTIME_TASK_SHELL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "espnow_bbs_mesh_discovery_contract.h"

#define ESPNOW_BBS_MESH_RUNTIME_TASK_NAME "mesh_rt_sched"
#define ESPNOW_BBS_MESH_RUNTIME_TASK_PERIOD_MAX_TICKS 64u
#define ESPNOW_BBS_MESH_RUNTIME_TASK_CORE_NONE tskNO_AFFINITY

typedef enum {
    ESPNOW_BBS_MESH_RUNTIME_TASK_OK = 0,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NULL,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_STACK_DEPTH_ZERO,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PERIOD_ZERO,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PERIOD_TOO_LARGE,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_CORE,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_PRIORITY_ZERO,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_ALREADY_STARTED,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_NOT_STARTED,
    ESPNOW_BBS_MESH_RUNTIME_TASK_REJECT_CREATE_FAILED
} espnow_bbs_mesh_runtime_task_status_t;

typedef struct {
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler;
    const espnow_bbs_mesh_contract_owner_t *owner;
    const char *task_name;
    StackType_t *stack_buffer;
    StaticTask_t *task_buffer;
    uint32_t stack_depth_bytes;
    UBaseType_t priority;
    BaseType_t core_id;
    TickType_t period_ticks;
} espnow_bbs_mesh_runtime_task_config_t;

typedef struct {
    TaskHandle_t task_handle;
    espnow_bbs_mesh_contract_runtime_scheduler_t *runtime_scheduler;
    const espnow_bbs_mesh_contract_owner_t *owner;
    TickType_t period_ticks;
    size_t attempted;
    size_t completed;
    size_t rejected;
    size_t wake_count;
    volatile bool stop_requested;
    volatile bool running;
    espnow_bbs_mesh_runtime_task_status_t last_status;
    espnow_bbs_mesh_contract_status_t last_contract_status;
} espnow_bbs_mesh_runtime_task_t;

const char *espnow_bbs_mesh_runtime_task_status_name(
    espnow_bbs_mesh_runtime_task_status_t status
);
void espnow_bbs_mesh_runtime_task_init(espnow_bbs_mesh_runtime_task_t *task);
bool espnow_bbs_mesh_runtime_task_is_running(
    const espnow_bbs_mesh_runtime_task_t *task
);
size_t espnow_bbs_mesh_runtime_task_completed_count(
    const espnow_bbs_mesh_runtime_task_t *task
);
espnow_bbs_mesh_runtime_task_status_t espnow_bbs_mesh_runtime_task_config_status(
    const espnow_bbs_mesh_runtime_task_config_t *config
);
espnow_bbs_mesh_runtime_task_status_t espnow_bbs_mesh_runtime_task_start(
    espnow_bbs_mesh_runtime_task_t *task,
    const espnow_bbs_mesh_runtime_task_config_t *config
);
espnow_bbs_mesh_runtime_task_status_t espnow_bbs_mesh_runtime_task_request_stop(
    espnow_bbs_mesh_runtime_task_t *task
);

#endif
