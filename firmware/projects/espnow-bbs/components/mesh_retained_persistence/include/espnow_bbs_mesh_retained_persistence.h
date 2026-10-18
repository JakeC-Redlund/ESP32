#ifndef ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_H
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "espnow_bbs_mesh_discovery_contract.h"

#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_OWNER "mesh_ingress"
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_NAMESPACE "mesh_retained"
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY "m4t_checkpoint"
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_WRITE_POLICY "caller_triggered"
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_VERSION 1u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES 96u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SCHEMA_MAX_BYTES 31u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SUMMARY_MAX_BYTES 512u

typedef enum {
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK = 0,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CAPACITY,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_LENGTH,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_MAGIC,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_VERSION,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SCHEMA,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NON_ASCII,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SECRET_FIELD,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_UNKNOWN_REQUEST,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_STATUS,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CHECKSUM,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED,
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_ERROR
} espnow_bbs_mesh_retained_persistence_status_t;

typedef struct {
    char schema[ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SCHEMA_MAX_BYTES + 1u];
    char last_request[
        ESPNOW_BBS_MESH_CONTRACT_BRIDGE_REQUEST_ROUTER_NAME_MAX_BYTES + 1u
    ];
    uint32_t record_version;
    uint32_t attempted;
    uint32_t staged;
    uint32_t rejected;
    uint32_t staged_bytes;
    espnow_bbs_mesh_contract_status_t last_status;
} espnow_bbs_mesh_retained_persistence_checkpoint_t;

typedef struct {
    bool initialized;
    size_t init_count;
    size_t deinit_count;
    size_t load_attempted;
    size_t save_attempted;
    size_t erase_attempted;
    size_t commit_count;
    size_t recovered;
    size_t missing;
    size_t rejected;
    espnow_bbs_mesh_retained_persistence_status_t last_status;
} espnow_bbs_mesh_retained_persistence_storage_t;

const char *espnow_bbs_mesh_retained_persistence_status_name(
    espnow_bbs_mesh_retained_persistence_status_t status
);
void espnow_bbs_mesh_retained_persistence_checkpoint_init(
    espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
    espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint,
    const espnow_bbs_mesh_contract_persistence_intent_t *intent
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_checkpoint_apply(
    espnow_bbs_mesh_contract_persistence_intent_t *intent,
    const espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_encode(
    const espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint,
    uint8_t *record,
    size_t record_capacity,
    size_t *record_bytes
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_decode(
    const uint8_t *record,
    size_t record_bytes,
    espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_recover(
    espnow_bbs_mesh_contract_persistence_intent_t *intent,
    const uint8_t *record,
    size_t record_bytes
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_summary(
    espnow_bbs_mesh_retained_persistence_status_t recovery_status,
    const espnow_bbs_mesh_contract_persistence_intent_t *intent,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
);
void espnow_bbs_mesh_retained_persistence_storage_context_init(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_init(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_deinit(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_load(
    espnow_bbs_mesh_retained_persistence_storage_t *storage,
    espnow_bbs_mesh_contract_persistence_intent_t *intent
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_save(
    espnow_bbs_mesh_retained_persistence_storage_t *storage,
    const espnow_bbs_mesh_contract_persistence_intent_t *intent
);
espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_erase(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
);

#endif
