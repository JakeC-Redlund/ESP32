#include "espnow_bbs_mesh_retained_persistence.h"

#include <string.h>

#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MAGIC 0x4d345256u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MAGIC_OFFSET 0u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_VERSION_OFFSET 4u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_ATTEMPTED_OFFSET 8u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STAGED_OFFSET 12u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECTED_OFFSET 16u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STAGED_BYTES_OFFSET 20u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STATUS_OFFSET 24u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SCHEMA_OFFSET 28u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SCHEMA_BYTES 32u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REQUEST_OFFSET 60u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REQUEST_BYTES 33u
#define ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_CHECKSUM_OFFSET 94u

static bool append_literal(char *out, size_t capacity, size_t *used, const char *value);
static bool append_size(char *out, size_t capacity, size_t *used, size_t value);
static bool copy_fixed_text(char *out, size_t out_capacity, const uint8_t *in, size_t in_capacity);
static bool retained_contract_status_known(espnow_bbs_mesh_contract_status_t status);
static bool retained_size_to_u32(size_t value, uint32_t *out);
static uint16_t retained_checksum(const uint8_t *record, size_t bytes);
static uint16_t retained_read_u16(const uint8_t *record, size_t offset);
static uint32_t retained_read_u32(const uint8_t *record, size_t offset);
static espnow_bbs_mesh_retained_persistence_status_t retained_validate_checkpoint(
    const espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
);
static espnow_bbs_mesh_retained_persistence_status_t retained_validate_request(
    const char *request
);
static void retained_write_u16(uint8_t *record, size_t offset, uint16_t value);
static void retained_write_u32(uint8_t *record, size_t offset, uint32_t value);

const char *espnow_bbs_mesh_retained_persistence_status_name(
    espnow_bbs_mesh_retained_persistence_status_t status
)
{
    switch (status) {
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK:
        return "ok";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING:
        return "missing";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL:
        return "reject_null";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CAPACITY:
        return "reject_capacity";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_LENGTH:
        return "reject_length";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_MAGIC:
        return "reject_magic";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_VERSION:
        return "reject_version";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SCHEMA:
        return "reject_schema";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NON_ASCII:
        return "reject_non_ascii";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SECRET_FIELD:
        return "reject_secret_field";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_UNKNOWN_REQUEST:
        return "reject_unknown_request";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE:
        return "reject_counter_range";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_STATUS:
        return "reject_status";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CHECKSUM:
        return "reject_checksum";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED:
        return "reject_not_initialized";
    case ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_ERROR:
        return "storage_error";
    default:
        return "unknown_status";
    }
}

void espnow_bbs_mesh_retained_persistence_checkpoint_init(
    espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
)
{
    if (checkpoint == NULL) {
        return;
    }
    memset(checkpoint, 0, sizeof(*checkpoint));
    (void)copy_fixed_text(
        checkpoint->schema,
        sizeof(checkpoint->schema),
        (const uint8_t *)ESPNOW_BBS_MESH_DISCOVERY_SCHEMA,
        strlen(ESPNOW_BBS_MESH_DISCOVERY_SCHEMA) + 1u
    );
    checkpoint->record_version = ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_VERSION;
    checkpoint->last_status = ESPNOW_BBS_MESH_CONTRACT_OK;
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_checkpoint_from_intent(
    espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint,
    const espnow_bbs_mesh_contract_persistence_intent_t *intent
)
{
    espnow_bbs_mesh_retained_persistence_status_t request_status;

    if (checkpoint == NULL || intent == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    if (!retained_size_to_u32(intent->attempted, &checkpoint->attempted) ||
        !retained_size_to_u32(intent->staged, &checkpoint->staged) ||
        !retained_size_to_u32(intent->rejected, &checkpoint->rejected) ||
        !retained_size_to_u32(intent->staged_bytes, &checkpoint->staged_bytes)) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE;
    }
    if (intent->staged > intent->attempted ||
        intent->rejected > intent->attempted - intent->staged) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE;
    }
    if (!retained_contract_status_known(intent->last_status)) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_STATUS;
    }
    request_status = retained_validate_request(intent->last_request);
    if (request_status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        return request_status;
    }

    espnow_bbs_mesh_retained_persistence_checkpoint_init(checkpoint);
    checkpoint->attempted = (uint32_t)intent->attempted;
    checkpoint->staged = (uint32_t)intent->staged;
    checkpoint->rejected = (uint32_t)intent->rejected;
    checkpoint->staged_bytes = (uint32_t)intent->staged_bytes;
    checkpoint->last_status = intent->last_status;
    (void)copy_fixed_text(
        checkpoint->last_request,
        sizeof(checkpoint->last_request),
        (const uint8_t *)intent->last_request,
        strlen(intent->last_request) + 1u
    );
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_checkpoint_apply(
    espnow_bbs_mesh_contract_persistence_intent_t *intent,
    const espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
)
{
    espnow_bbs_mesh_retained_persistence_status_t status;

    if (intent == NULL || checkpoint == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    status = retained_validate_checkpoint(checkpoint);
    if (status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        espnow_bbs_mesh_contract_persistence_intent_init(intent);
        return status;
    }

    espnow_bbs_mesh_contract_persistence_intent_init(intent);
    intent->attempted = checkpoint->attempted;
    intent->staged = checkpoint->staged;
    intent->rejected = checkpoint->rejected;
    intent->staged_bytes = checkpoint->staged_bytes;
    intent->last_status = checkpoint->last_status;
    (void)copy_fixed_text(
        intent->last_request,
        sizeof(intent->last_request),
        (const uint8_t *)checkpoint->last_request,
        strlen(checkpoint->last_request) + 1u
    );
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_encode(
    const espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint,
    uint8_t *record,
    size_t record_capacity,
    size_t *record_bytes
)
{
    espnow_bbs_mesh_retained_persistence_status_t status;
    uint16_t checksum;

    if (checkpoint == NULL || record == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    if (record_capacity < ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CAPACITY;
    }
    status = retained_validate_checkpoint(checkpoint);
    if (status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        return status;
    }

    memset(record, 0, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES);
    retained_write_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MAGIC_OFFSET,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MAGIC
    );
    retained_write_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_VERSION_OFFSET,
        checkpoint->record_version
    );
    retained_write_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_ATTEMPTED_OFFSET,
        checkpoint->attempted
    );
    retained_write_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STAGED_OFFSET,
        checkpoint->staged
    );
    retained_write_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECTED_OFFSET,
        checkpoint->rejected
    );
    retained_write_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STAGED_BYTES_OFFSET,
        checkpoint->staged_bytes
    );
    retained_write_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STATUS_OFFSET,
        (uint32_t)checkpoint->last_status
    );
    memcpy(
        record + ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SCHEMA_OFFSET,
        checkpoint->schema,
        strlen(checkpoint->schema) + 1u
    );
    memcpy(
        record + ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REQUEST_OFFSET,
        checkpoint->last_request,
        strlen(checkpoint->last_request) + 1u
    );
    checksum = retained_checksum(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_CHECKSUM_OFFSET
    );
    retained_write_u16(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_CHECKSUM_OFFSET,
        checksum
    );
    if (record_bytes != NULL) {
        *record_bytes = ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES;
    }
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_decode(
    const uint8_t *record,
    size_t record_bytes,
    espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
)
{
    espnow_bbs_mesh_retained_persistence_checkpoint_t decoded;
    uint16_t expected_checksum;
    uint16_t actual_checksum;
    espnow_bbs_mesh_retained_persistence_status_t status;

    if (record == NULL || checkpoint == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    if (record_bytes != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_LENGTH;
    }
    if (retained_read_u32(record, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MAGIC_OFFSET) !=
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MAGIC) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_MAGIC;
    }
    if (retained_read_u32(record, ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_VERSION_OFFSET) !=
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_VERSION) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_VERSION;
    }
    expected_checksum = retained_checksum(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_CHECKSUM_OFFSET
    );
    actual_checksum = retained_read_u16(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_CHECKSUM_OFFSET
    );
    if (actual_checksum != expected_checksum) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CHECKSUM;
    }

    espnow_bbs_mesh_retained_persistence_checkpoint_init(&decoded);
    decoded.record_version = retained_read_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_VERSION_OFFSET
    );
    decoded.attempted = retained_read_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_ATTEMPTED_OFFSET
    );
    decoded.staged = retained_read_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STAGED_OFFSET
    );
    decoded.rejected = retained_read_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECTED_OFFSET
    );
    decoded.staged_bytes = retained_read_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STAGED_BYTES_OFFSET
    );
    decoded.last_status = (espnow_bbs_mesh_contract_status_t)retained_read_u32(
        record,
        ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STATUS_OFFSET
    );
    if (!copy_fixed_text(
            decoded.schema,
            sizeof(decoded.schema),
            record + ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SCHEMA_OFFSET,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_SCHEMA_BYTES
        ) ||
        !copy_fixed_text(
            decoded.last_request,
            sizeof(decoded.last_request),
            record + ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REQUEST_OFFSET,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REQUEST_BYTES
        )) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_LENGTH;
    }
    status = retained_validate_checkpoint(&decoded);
    if (status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        return status;
    }

    *checkpoint = decoded;
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_recover(
    espnow_bbs_mesh_contract_persistence_intent_t *intent,
    const uint8_t *record,
    size_t record_bytes
)
{
    espnow_bbs_mesh_retained_persistence_checkpoint_t checkpoint;
    espnow_bbs_mesh_retained_persistence_status_t status;

    if (intent == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    if (record == NULL && record_bytes == 0u) {
        espnow_bbs_mesh_contract_persistence_intent_init(intent);
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MISSING;
    }
    status = espnow_bbs_mesh_retained_persistence_decode(
        record,
        record_bytes,
        &checkpoint
    );
    if (status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        espnow_bbs_mesh_contract_persistence_intent_init(intent);
        return status;
    }
    return espnow_bbs_mesh_retained_persistence_checkpoint_apply(intent, &checkpoint);
}

espnow_bbs_mesh_retained_persistence_status_t
espnow_bbs_mesh_retained_persistence_summary(
    espnow_bbs_mesh_retained_persistence_status_t recovery_status,
    const espnow_bbs_mesh_contract_persistence_intent_t *intent,
    char *summary,
    size_t summary_capacity,
    size_t *line_bytes
)
{
    size_t used = 0u;

    if (intent == NULL || summary == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    if (summary_capacity == 0u) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CAPACITY;
    }
    summary[0] = '\0';
    if (!append_literal(summary, summary_capacity, &used, "retained=m4v") ||
        !append_literal(summary, summary_capacity, &used, " storage=nvs") ||
        !append_literal(summary, summary_capacity, &used, " owner=") ||
        !append_literal(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_OWNER
        ) ||
        !append_literal(summary, summary_capacity, &used, " namespace=") ||
        !append_literal(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_NAMESPACE
        ) ||
        !append_literal(summary, summary_capacity, &used, " key=") ||
        !append_literal(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY
        ) ||
        !append_literal(summary, summary_capacity, &used, " policy=") ||
        !append_literal(
            summary,
            summary_capacity,
            &used,
            ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_WRITE_POLICY
        ) ||
        !append_literal(summary, summary_capacity, &used, " status=") ||
        !append_literal(
            summary,
            summary_capacity,
            &used,
            espnow_bbs_mesh_retained_persistence_status_name(recovery_status)
        ) ||
        !append_literal(summary, summary_capacity, &used, " request=") ||
        !append_literal(
            summary,
            summary_capacity,
            &used,
            intent->last_request[0] != '\0' ? intent->last_request : "none"
        ) ||
        !append_literal(summary, summary_capacity, &used, " attempted=") ||
        !append_size(summary, summary_capacity, &used, intent->attempted) ||
        !append_literal(summary, summary_capacity, &used, " staged=") ||
        !append_size(summary, summary_capacity, &used, intent->staged) ||
        !append_literal(summary, summary_capacity, &used, " rejected=") ||
        !append_size(summary, summary_capacity, &used, intent->rejected) ||
        !append_literal(summary, summary_capacity, &used, " bytes=") ||
        !append_size(summary, summary_capacity, &used, intent->staged_bytes) ||
        !append_literal(summary, summary_capacity, &used, " reboot=unproved") ||
        !append_literal(summary, summary_capacity, &used, " live=blocked") ||
        !append_literal(summary, summary_capacity, &used, " serial=closed") ||
        !append_literal(summary, summary_capacity, &used, " abi=closed")) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CAPACITY;
    }
    if (line_bytes != NULL) {
        *line_bytes = used;
    }
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

void espnow_bbs_mesh_retained_persistence_storage_context_init(
    espnow_bbs_mesh_retained_persistence_storage_t *storage
)
{
    if (storage == NULL) {
        return;
    }
    memset(storage, 0, sizeof(*storage));
    storage->last_status = ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

static bool append_literal(char *out, size_t capacity, size_t *used, const char *value)
{
    size_t value_bytes;

    if (out == NULL || used == NULL || value == NULL) {
        return false;
    }
    value_bytes = strlen(value);
    if (*used + value_bytes + 1u > capacity) {
        return false;
    }
    memcpy(out + *used, value, value_bytes);
    *used += value_bytes;
    out[*used] = '\0';
    return true;
}

static bool append_size(char *out, size_t capacity, size_t *used, size_t value)
{
    char reversed[32u];
    size_t count = 0u;

    if (value == 0u) {
        return append_literal(out, capacity, used, "0");
    }
    while (value != 0u && count < sizeof(reversed)) {
        reversed[count] = (char)('0' + (value % 10u));
        value /= 10u;
        ++count;
    }
    while (count != 0u) {
        char digit[2u] = {reversed[count - 1u], '\0'};
        if (!append_literal(out, capacity, used, digit)) {
            return false;
        }
        --count;
    }
    return true;
}

static bool copy_fixed_text(char *out, size_t out_capacity, const uint8_t *in, size_t in_capacity)
{
    size_t bytes = 0u;

    if (out == NULL || out_capacity == 0u || in == NULL || in_capacity == 0u) {
        return false;
    }
    while (bytes < in_capacity && in[bytes] != 0u) {
        if (in[bytes] > 0x7eu) {
            return false;
        }
        ++bytes;
    }
    if (bytes == in_capacity || bytes + 1u > out_capacity) {
        return false;
    }
    memcpy(out, in, bytes);
    out[bytes] = '\0';
    return true;
}

static bool retained_contract_status_known(espnow_bbs_mesh_contract_status_t status)
{
    return status >= ESPNOW_BBS_MESH_CONTRACT_OK &&
        status <= ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_NOT_CONFIGURED;
}

static bool retained_size_to_u32(size_t value, uint32_t *out)
{
    if (out == NULL || value > UINT32_MAX) {
        return false;
    }
    *out = (uint32_t)value;
    return true;
}

static uint16_t retained_checksum(const uint8_t *record, size_t bytes)
{
    uint32_t sum = 0u;
    size_t index;

    for (index = 0u; index < bytes; ++index) {
        sum += record[index];
    }
    return (uint16_t)(sum & 0xffffu);
}

static uint16_t retained_read_u16(const uint8_t *record, size_t offset)
{
    return (uint16_t)record[offset] |
        ((uint16_t)record[offset + 1u] << 8u);
}

static uint32_t retained_read_u32(const uint8_t *record, size_t offset)
{
    return (uint32_t)record[offset] |
        ((uint32_t)record[offset + 1u] << 8u) |
        ((uint32_t)record[offset + 2u] << 16u) |
        ((uint32_t)record[offset + 3u] << 24u);
}

static espnow_bbs_mesh_retained_persistence_status_t retained_validate_checkpoint(
    const espnow_bbs_mesh_retained_persistence_checkpoint_t *checkpoint
)
{
    espnow_bbs_mesh_retained_persistence_status_t request_status;

    if (checkpoint == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    if (checkpoint->record_version != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_VERSION) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_VERSION;
    }
    if (strcmp(checkpoint->schema, ESPNOW_BBS_MESH_DISCOVERY_SCHEMA) != 0) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SCHEMA;
    }
    if (!retained_contract_status_known(checkpoint->last_status)) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_STATUS;
    }
    if (checkpoint->staged > checkpoint->attempted ||
        checkpoint->rejected > checkpoint->attempted - checkpoint->staged) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE;
    }
    request_status = retained_validate_request(checkpoint->last_request);
    if (request_status != ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK) {
        return request_status;
    }
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

static espnow_bbs_mesh_retained_persistence_status_t retained_validate_request(
    const char *request
)
{
    if (request == NULL) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NULL;
    }
    if (!espnow_bbs_mesh_contract_is_ascii(request)) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NON_ASCII;
    }
    if (request[0] == '\0') {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
    }
    if (espnow_bbs_mesh_contract_field_name_status(request) ==
        ESPNOW_BBS_MESH_CONTRACT_REJECT_SECRET_FIELD) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SECRET_FIELD;
    }
    if (espnow_bbs_mesh_contract_request_status(request) !=
        ESPNOW_BBS_MESH_CONTRACT_OK) {
        return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_UNKNOWN_REQUEST;
    }
    return ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_OK;
}

static void retained_write_u16(uint8_t *record, size_t offset, uint16_t value)
{
    record[offset] = (uint8_t)(value & 0xffu);
    record[offset + 1u] = (uint8_t)((value >> 8u) & 0xffu);
}

static void retained_write_u32(uint8_t *record, size_t offset, uint32_t value)
{
    record[offset] = (uint8_t)(value & 0xffu);
    record[offset + 1u] = (uint8_t)((value >> 8u) & 0xffu);
    record[offset + 2u] = (uint8_t)((value >> 16u) & 0xffu);
    record[offset + 3u] = (uint8_t)((value >> 24u) & 0xffu);
}
