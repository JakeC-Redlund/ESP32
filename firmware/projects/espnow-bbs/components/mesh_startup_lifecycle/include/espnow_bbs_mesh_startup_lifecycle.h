#ifndef ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_H
#define ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_H

#include <stdbool.h>
#include <stddef.h>

#include "espnow_bbs_mesh_runtime_task_shell.h"

typedef enum {
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_UNINITIALIZED = 0,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_CONFIGURED,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STARTING,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_RUNNING,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOP_REQUESTED,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOPPED,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_FAILED
} espnow_bbs_mesh_startup_lifecycle_state_t;

typedef enum {
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_OK = 0,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_NULL,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_TASK_CONFIG,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_CONFIGURE_STATE,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_START_STATE,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_STOP_STATE,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_DEINIT_STATE,
    ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_REJECT_TASK_START_FAILED
} espnow_bbs_mesh_startup_lifecycle_status_t;

typedef struct {
    espnow_bbs_mesh_startup_lifecycle_state_t state;
    espnow_bbs_mesh_runtime_task_t runtime_task;
    espnow_bbs_mesh_runtime_task_config_t task_config;
    bool has_config;
    size_t configured_count;
    size_t started_count;
    size_t stop_requested_count;
    size_t stopped_count;
    size_t failed_count;
    espnow_bbs_mesh_startup_lifecycle_status_t last_status;
    espnow_bbs_mesh_runtime_task_status_t last_task_status;
} espnow_bbs_mesh_startup_lifecycle_t;

const char *espnow_bbs_mesh_startup_lifecycle_state_name(
    espnow_bbs_mesh_startup_lifecycle_state_t state
);
const char *espnow_bbs_mesh_startup_lifecycle_status_name(
    espnow_bbs_mesh_startup_lifecycle_status_t status
);
void espnow_bbs_mesh_startup_lifecycle_init(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
);
espnow_bbs_mesh_startup_lifecycle_state_t
espnow_bbs_mesh_startup_lifecycle_state(
    const espnow_bbs_mesh_startup_lifecycle_t *lifecycle
);
espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_configure(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle,
    const espnow_bbs_mesh_runtime_task_config_t *task_config
);
espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_start(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
);
espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_request_stop(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
);
espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_poll(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
);
espnow_bbs_mesh_startup_lifecycle_status_t
espnow_bbs_mesh_startup_lifecycle_deinit(
    espnow_bbs_mesh_startup_lifecycle_t *lifecycle
);

#endif
