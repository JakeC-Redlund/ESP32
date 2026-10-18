#include "espnow_bbs_mesh_retained_persistence.h"

#include <string.h>

#ifndef ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_HOST_TEST
#include "nvs.h"
#include "nvs_flash.h"
#else
typedef int esp_err_t;
typedef int nvs_handle_t;
typedef enum {
    NVS_READONLY = 1,
    NVS_READWRITE = 2
} nvs_open_mode_t;
#define ESP_OK 0
#define ESP_ERR_NVS_NOT_FOUND 0x1102
#define ESP_ERR_NVS_INVALID_LENGTH 0x110c
static bool s_host_nvs_has_blob;
static bool s_host_nvs_initialized;
static uint8_t s_host_nvs_blob[
    ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES + 8u
];
static size_t s_host_nvs_blob_length;
static esp_err_t nvs_flash_init(void);
static esp_err_t nvs_flash_deinit(void);
static esp_err_t nvs_open (
    const char *namespace_name,
    nvs_open_mode_t open_mode,
    nvs_handle_t *out_handle
);
static esp_err_t nvs_get_blob(
    nvs_handle_t handle,
    const char *key,
    void *out_value,
    size_t *length
);
static esp_err_t nvs_set_blob(
    nvs_handle_t handle,
    const char *key,
    const void *value,
    size_t length
);
static esp_err_t nvs_erase_key(nvs_handle_t handle, const char *key);
static esp_err_t nvs_commit(nvs_handle_t handle);
static void nvs_close(nvs_handle_t handle);
#endif

static espnow_bbs_mesh_retained_persistence_status_t retained_storage_finish(
    espnow_bbs_mesh_retained_persistence_storage_t *storage,
    espnow_bbs_mesh_retained_persistence_status_t status
);
static espnow_bbs_mesh_retained_persistence_status_t retained_storage_map_error(
    esp_err_t err
);

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_init(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
)
{
    esp_err_t err;

    if (storage == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    err = nvs_flash_init();
    ++storage->init_count;
    if (err != ESP_OK) {
        storage->initialized = false;
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }
    storage->initialized = true;
    return retained_storage_finish(storage, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK);
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_deinit(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
)
{
    esp_err_t err;

    if (storage == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    err = nvs_flash_deinit();
    ++storage->deinit_count;
    storage->initialized = false;
    if (err != ESP_OK) {
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }
    return retained_storage_finish(storage, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK);
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_load(
    espnow_bbs_mesh_retained_persistence_storage_t *storage,
    espnow_bbs_mesh_contract_persistence_intent_t *intent
)
{
    nvs_handle_t handle;
    uint8_t record[ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES];
    size_t record_bytes = sizeof(record);
    esp_err_t err;
    espnow_bbs_mesh_retained_persistence_status_t status;

    if (storage == NULL || intent == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    ++storage->load_attempted;
    if (!storage->initialized) {
        return retained_storage_finish(
            storage,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED
        );
    }
    err = nvs_open (
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_NAMESPACE,
        NVS_READONLY,
        &handle
    );
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        status = espnow_bbs_mesh_retained_persistence_recover(intent, NULL, 0u);
        return retained_storage_finish(storage, status);
    }
    if (err != ESP_OK) {
        espnow_bbs_mesh_contract_persistence_intent_init(intent);
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }

    err = nvs_get_blob(
        handle,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY,
        record,
        &record_bytes
    );
    nvs_close(handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        status = espnow_bbs_mesh_retained_persistence_recover(intent, NULL, 0u);
        return retained_storage_finish(storage, status);
    }
    if (err != ESP_OK) {
        espnow_bbs_mesh_contract_persistence_intent_init(intent);
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }
    status = espnow_bbs_mesh_retained_persistence_recover(
        intent,
        record,
        record_bytes
    );
    return retained_storage_finish(storage, status);
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_save(
    espnow_bbs_mesh_retained_persistence_storage_t *storage,
    const espnow_bbs_mesh_contract_persistence_intent_t *intent
)
{
    espnow_bbs_mesh_retained_persistence_checkpoint_t checkpoint;
    uint8_t record[ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES];
    size_t record_bytes = 0u;
    nvs_handle_t handle;
    esp_err_t err;
    espnow_bbs_mesh_retained_persistence_status_t status;

    if (storage == NULL || intent == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    ++storage->save_attempted;
    if (!storage->initialized) {
        return retained_storage_finish(
            storage,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED
        );
    }
    status = espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
        &checkpoint,
        intent
    );
    if (status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        return retained_storage_finish(storage, status);
    }
    status = espnow_bbs_mesh_retained_persistence_encode(
        &checkpoint,
        record,
        sizeof(record),
        &record_bytes
    );
    if (status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        return retained_storage_finish(storage, status);
    }

    err = nvs_open (
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_NAMESPACE,
        NVS_READWRITE,
        &handle
    );
    if (err != ESP_OK) {
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }
    err = nvs_set_blob(
        handle,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY,
        record,
        record_bytes
    );
    if (err == ESP_OK) {
        err = nvs_commit(handle);
        if (err == ESP_OK) {
            ++storage->commit_count;
        }
    }
    nvs_close(handle);
    if (err != ESP_OK) {
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }
    return retained_storage_finish(storage, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK);
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_storage_erase(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
)
{
    nvs_handle_t handle;
    esp_err_t err;

    if (storage == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    ++storage->erase_attempted;
    if (!storage->initialized) {
        return retained_storage_finish(
            storage,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED
        );
    }
    err = nvs_open (
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_NAMESPACE,
        NVS_READWRITE,
        &handle
    );
    if (err != ESP_OK) {
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }
    err = nvs_erase_key(handle, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY);
    if (err == ESP_OK) {
        err = nvs_commit(handle);
        if (err == ESP_OK) {
            ++storage->commit_count;
        }
    }
    nvs_close(handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return retained_storage_finish(
            storage,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING
        );
    }
    if (err != ESP_OK) {
        return retained_storage_finish(storage, retained_storage_map_error(err));
    }
    return retained_storage_finish(storage, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK);
}

static espnow_bbs_mesh_retained_persistence_status_t retained_storage_finish(
    espnow_bbs_mesh_retained_persistence_storage_t *storage,
    espnow_bbs_mesh_retained_persistence_status_t status
)
{
    if (storage == NULL) {
        return status;
    }
    storage->last_status = status;
    if (status == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        ++storage->recovered;
    } else if (status == ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING) {
        ++storage->missing;
    } else {
        ++storage->rejected;
    }
    return status;
}

static espnow_bbs_mesh_retained_persistence_status_t retained_storage_map_error(
    esp_err_t err
)
{
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING;
    }
    if (err == ESP_ERR_NVS_INVALID_LENGTH) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_LENGTH;
    }
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_ERROR;
}

#ifdef ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_HOST_TEST
void espnow_bbs_mesh_retained_persistence_host_seed_blob_length(size_t length)
{
    if (length > sizeof(s_host_nvs_blob)) {
        length = sizeof(s_host_nvs_blob);
    }
    memset(s_host_nvs_blob, 0xa5, sizeof(s_host_nvs_blob));
    s_host_nvs_blob_length = length;
    s_host_nvs_has_blob = true;
}

static esp_err_t nvs_flash_init(void)
{
    s_host_nvs_initialized = true;
    return ESP_OK;
}

static esp_err_t nvs_flash_deinit(void)
{
    s_host_nvs_initialized = false;
    return ESP_OK;
}

static esp_err_t nvs_open (
    const char *namespace_name,
    nvs_open_mode_t open_mode,
    nvs_handle_t *out_handle
)
{
    (void)open_mode;
    if (!s_host_nvs_initialized || namespace_name == NULL || out_handle == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_ERROR;
    }
    if (strcmp(namespace_name, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_NAMESPACE) != 0) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    *out_handle = 1;
    return ESP_OK;
}

static esp_err_t nvs_get_blob(
    nvs_handle_t handle,
    const char *key,
    void *out_value,
    size_t *length
)
{
    (void)handle;
    if (key == NULL || length == NULL ||
        strcmp(key, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY) != 0) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    if (!s_host_nvs_has_blob) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    if (out_value == NULL || *length < s_host_nvs_blob_length) {
        *length = s_host_nvs_blob_length;
        return ESP_ERR_NVS_INVALID_LENGTH;
    }
    memcpy(out_value, s_host_nvs_blob, s_host_nvs_blob_length);
    *length = s_host_nvs_blob_length;
    return ESP_OK;
}

static esp_err_t nvs_set_blob(
    nvs_handle_t handle,
    const char *key,
    const void *value,
    size_t length
)
{
    (void)handle;
    if (key == NULL || value == NULL ||
        strcmp(key, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY) != 0 ||
        length > sizeof(s_host_nvs_blob)) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_ERROR;
    }
    memcpy(s_host_nvs_blob, value, length);
    s_host_nvs_blob_length = length;
    s_host_nvs_has_blob = true;
    return ESP_OK;
}

static esp_err_t nvs_erase_key(nvs_handle_t handle, const char *key)
{
    (void)handle;
    if (key == NULL || strcmp(key, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY) != 0 ||
        !s_host_nvs_has_blob) {
        return ESP_ERR_NVS_NOT_FOUND;
    }
    memset(s_host_nvs_blob, 0, sizeof(s_host_nvs_blob));
    s_host_nvs_blob_length = 0u;
    s_host_nvs_has_blob = false;
    return ESP_OK;
}

static esp_err_t nvs_commit(nvs_handle_t handle)
{
    (void)handle;
    return ESP_OK;
}

static void nvs_close(nvs_handle_t handle)
{
    (void)handle;
}
#endif
