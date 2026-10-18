#include "four_relay_core.h"

static bool needs_escape(uint8_t value)
{
    return value == 0x7eu || value == 0x7du || value == 0x11u || value == 0x13u;
}

static fr_status_t append_escaped(
    uint8_t value,
    uint8_t *wire,
    size_t wire_capacity,
    size_t *offset
)
{
    if (needs_escape(value)) {
        if (*offset + 2u > wire_capacity) {
            return FR_REJECT_PAYLOAD_INVALID;
        }
        wire[(*offset)++] = 0x7du;
        wire[(*offset)++] = value ^ 0x20u;
        return FR_OK;
    }
    if (*offset + 1u > wire_capacity) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    wire[(*offset)++] = value;
    return FR_OK;
}

fr_status_t fr_xbee_encode_api2(
    const uint8_t *frame_data,
    size_t frame_len,
    uint8_t *wire,
    size_t wire_capacity,
    size_t *wire_len
)
{
    if (frame_data == 0 || wire == 0 || wire_len == 0 || frame_len > FR_XBEE_MAX_FRAME_DATA) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (wire_capacity < 4u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    size_t offset = 0;
    uint8_t checksum_sum = 0;
    wire[offset++] = 0x7eu;

    fr_status_t status = append_escaped((uint8_t)((frame_len >> 8u) & 0xffu), wire, wire_capacity, &offset);
    if (status != FR_OK) {
        return status;
    }
    status = append_escaped((uint8_t)(frame_len & 0xffu), wire, wire_capacity, &offset);
    if (status != FR_OK) {
        return status;
    }

    for (size_t index = 0; index < frame_len; ++index) {
        checksum_sum = (uint8_t)(checksum_sum + frame_data[index]);
        status = append_escaped(frame_data[index], wire, wire_capacity, &offset);
        if (status != FR_OK) {
            return status;
        }
    }

    status = append_escaped((uint8_t)(0xffu - checksum_sum), wire, wire_capacity, &offset);
    if (status != FR_OK) {
        return status;
    }
    *wire_len = offset;
    return FR_OK;
}

fr_status_t fr_xbee_decode_api2(
    const uint8_t *wire,
    size_t wire_len,
    fr_xbee_frame_t *frame
)
{
    if (wire == 0 || frame == 0 || wire_len < 4u || wire[0] != 0x7eu) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    uint8_t decoded[FR_XBEE_MAX_FRAME_DATA + 3u];
    size_t decoded_len = 0;
    for (size_t index = 1u; index < wire_len; ++index) {
        uint8_t value = wire[index];
        if (value == 0x7du) {
            if (index + 1u >= wire_len) {
                return FR_REJECT_PAYLOAD_INVALID;
            }
            value = wire[++index] ^ 0x20u;
        }
        if (decoded_len >= sizeof(decoded)) {
            return FR_REJECT_PAYLOAD_INVALID;
        }
        decoded[decoded_len++] = value;
    }

    if (decoded_len < 3u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    size_t frame_len = ((size_t)decoded[0] << 8u) | decoded[1];
    if (frame_len > FR_XBEE_MAX_FRAME_DATA || decoded_len != frame_len + 3u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    uint8_t checksum_sum = 0;
    for (size_t index = 0; index < frame_len; ++index) {
        frame->data[index] = decoded[index + 2u];
        checksum_sum = (uint8_t)(checksum_sum + frame->data[index]);
    }
    checksum_sum = (uint8_t)(checksum_sum + decoded[decoded_len - 1u]);
    if (checksum_sum != 0xffu) {
        frame->data_len = 0;
        return FR_REJECT_PAYLOAD_INVALID;
    }

    frame->data_len = frame_len;
    return FR_OK;
}

fr_status_t fr_xbee_parse_at_response(
    const fr_xbee_frame_t *frame,
    fr_xbee_at_response_t *response
)
{
    if (frame == 0 || response == 0 || frame->data_len < 5u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x88u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    size_t value_len = frame->data_len - 5u;
    if (value_len > FR_XBEE_AT_VALUE_MAX) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    response->frame_id = frame->data[1];
    response->command[0] = (char)frame->data[2];
    response->command[1] = (char)frame->data[3];
    response->command[2] = '\0';
    response->command_status = frame->data[4];
    response->value_len = value_len;
    for (size_t index = 0; index < value_len; ++index) {
        response->value[index] = frame->data[index + 5u];
    }
    return FR_OK;
}

const char *fr_xbee_at_command_status_label(uint8_t command_status)
{
    switch (command_status) {
    case 0x00u:
        return "ok";
    case 0x01u:
        return "error";
    case 0x02u:
        return "invalid_command";
    case 0x03u:
        return "invalid_parameter";
    default:
        return "unknown_at_command_status";
    }
}

fr_status_t fr_xbee_parse_remote_at_response(
    const fr_xbee_frame_t *frame,
    fr_xbee_remote_at_response_t *response
)
{
    if (
        frame == 0
        || response == 0
        || frame->data_len < 15u
        || frame->data_len > FR_XBEE_MAX_FRAME_DATA
    ) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x97u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    size_t value_len = frame->data_len - 15u;
    if (value_len > FR_XBEE_AT_VALUE_MAX) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    response->frame_id = frame->data[1];
    for (size_t index = 0; index < 8u; ++index) {
        response->source64[index] = frame->data[index + 2u];
    }
    response->reserved16 = ((uint16_t)frame->data[10] << 8u) | frame->data[11];
    response->command[0] = (char)frame->data[12];
    response->command[1] = (char)frame->data[13];
    response->command[2] = '\0';
    response->command_status = frame->data[14];
    response->value_len = value_len;
    for (size_t index = 0; index < value_len; ++index) {
        response->value[index] = frame->data[index + 15u];
    }
    return FR_OK;
}

const char *fr_xbee_remote_at_command_status_label(uint8_t command_status)
{
    switch (command_status) {
    case 0x00u:
        return "ok";
    case 0x01u:
        return "error";
    case 0x02u:
        return "invalid_command";
    case 0x03u:
        return "invalid_parameter";
    case 0x04u:
        return "transmission_failure";
    case 0x0cu:
        return "encryption_error";
    default:
        return "unknown_remote_at_command_status";
    }
}

fr_status_t fr_xbee_parse_modem_status(
    const fr_xbee_frame_t *frame,
    fr_xbee_modem_status_t *status
)
{
    if (frame == 0 || status == 0 || frame->data_len != 2u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x8au) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    status->status = frame->data[1];
    return FR_OK;
}

const char *fr_xbee_modem_status_label(uint8_t status)
{
    switch (status) {
    case 0x00u:
        return "hardware_reset_or_power_up";
    case 0x01u:
        return "watchdog_timer_reset";
    case 0x02u:
        return "joined_network";
    case 0x03u:
        return "left_network";
    case 0x06u:
        return "coordinator_started";
    case 0x07u:
        return "network_security_key_updated";
    case 0x0bu:
        return "network_woke_up";
    case 0x0cu:
        return "network_went_to_sleep";
    case 0x0du:
        return "voltage_supply_limit_exceeded";
    case 0x0eu:
        return "digi_remote_manager_connected";
    case 0x0fu:
        return "digi_remote_manager_disconnected";
    case 0x11u:
        return "modem_configuration_changed_while_join_in_progress";
    case 0x12u:
        return "access_fault";
    case 0x13u:
        return "fatal_error";
    case 0x32u:
        return "ble_connect";
    case 0x33u:
        return "ble_disconnect";
    case 0x34u:
        return "bandmask_configuration_failed";
    case 0x35u:
        return "cellular_component_update_started";
    case 0x36u:
        return "cellular_component_update_failed";
    case 0x37u:
        return "cellular_component_update_completed";
    case 0x38u:
        return "xbee_firmware_update_started";
    case 0x39u:
        return "xbee_firmware_update_failed";
    case 0x3au:
        return "xbee_firmware_update_applying";
    case 0x3bu:
        return "secure_session_established";
    case 0x3cu:
        return "secure_session_ended";
    case 0x3du:
        return "secure_session_authentication_failed";
    case 0x3eu:
        return "coordinator_pan_id_conflict_no_action";
    case 0x3fu:
        return "coordinator_changed_pan_id_due_to_conflict";
    case 0x40u:
        return "router_pan_id_changed_by_coordinator_due_to_conflict";
    case 0x42u:
        return "network_watchdog_timeout_expired";
    default:
        if (status >= 0x80u) {
            return "stack_error";
        }
        return "unknown_modem_status";
    }
}

fr_status_t fr_xbee_parse_receive_packet(
    const fr_xbee_frame_t *frame,
    fr_xbee_receive_packet_t *packet
)
{
    if (
        frame == 0
        || packet == 0
        || frame->data_len <= 12u
        || frame->data_len > FR_XBEE_MAX_FRAME_DATA
    ) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x90u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    for (size_t index = 0; index < 8u; ++index) {
        packet->source64[index] = frame->data[index + 1u];
    }
    packet->source16 = ((uint16_t)frame->data[9] << 8u) | frame->data[10];
    packet->receive_options = frame->data[11];

    packet->payload_len = frame->data_len - 12u;
    if (packet->payload_len > FR_XBEE_MAX_FRAME_DATA) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    for (size_t index = 0; index < packet->payload_len; ++index) {
        packet->payload[index] = frame->data[index + 12u];
    }
    return FR_OK;
}

fr_status_t fr_xbee_parse_explicit_receive_packet(
    const fr_xbee_frame_t *frame,
    fr_xbee_explicit_receive_packet_t *packet
)
{
    if (
        frame == 0
        || packet == 0
        || frame->data_len <= 18u
        || frame->data_len > FR_XBEE_MAX_FRAME_DATA
    ) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x91u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    for (size_t index = 0; index < 8u; ++index) {
        packet->source64[index] = frame->data[index + 1u];
    }
    packet->reserved16 = ((uint16_t)frame->data[9] << 8u) | frame->data[10];
    packet->source_endpoint = frame->data[11];
    packet->destination_endpoint = frame->data[12];
    packet->cluster_id = ((uint16_t)frame->data[13] << 8u) | frame->data[14];
    packet->profile_id = ((uint16_t)frame->data[15] << 8u) | frame->data[16];
    packet->receive_options = frame->data[17];

    packet->payload_len = frame->data_len - 18u;
    for (size_t index = 0; index < packet->payload_len; ++index) {
        packet->payload[index] = frame->data[index + 18u];
    }
    return FR_OK;
}

bool fr_xbee_receive_options_acknowledged(uint8_t options)
{
    return (options & 0x01u) != 0u;
}

bool fr_xbee_receive_options_broadcast(uint8_t options)
{
    return (options & 0x02u) != 0u;
}

const char *fr_xbee_receive_options_delivery_method_label(uint8_t options)
{
    switch (options & 0xc0u) {
    case 0x40u:
        return "point_multipoint";
    case 0x80u:
        return "directed_broadcast";
    case 0xc0u:
        return "digimesh";
    default:
        return "invalid_delivery_method";
    }
}

fr_status_t fr_xbee_parse_route_information(
    const fr_xbee_frame_t *frame,
    fr_xbee_route_information_t *route
)
{
    if (
        frame == 0
        || route == 0
        || frame->data_len < 42u
        || frame->data_len > FR_XBEE_MAX_FRAME_DATA
    ) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x8du) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    size_t route_data_len = frame->data[2];
    if (route_data_len != frame->data_len - 3u || route_data_len < 39u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    route->source_event = frame->data[1];
    route->data_length = frame->data[2];
    route->timestamp_us =
        ((uint32_t)frame->data[3] << 24u)
        | ((uint32_t)frame->data[4] << 16u)
        | ((uint32_t)frame->data[5] << 8u)
        | frame->data[6];
    route->ack_timeout_count = frame->data[7];
    route->tx_blocked_count = frame->data[8];
    route->reserved = frame->data[9];

    for (size_t index = 0; index < 8u; ++index) {
        route->destination64[index] = frame->data[index + 10u];
        route->source64[index] = frame->data[index + 18u];
        route->responder64[index] = frame->data[index + 26u];
        route->receiver64[index] = frame->data[index + 34u];
    }
    route->trailing_len = route_data_len - 39u;
    return FR_OK;
}

const char *fr_xbee_route_information_source_event_label(uint8_t source_event)
{
    switch (source_event) {
    case 0x11u:
        return "nack";
    case 0x12u:
        return "trace_route";
    default:
        return "unknown_route_information_source_event";
    }
}

fr_status_t fr_xbee_parse_aggregate_addressing_update(
    const fr_xbee_frame_t *frame,
    fr_xbee_aggregate_addressing_update_t *update
)
{
    if (frame == 0 || update == 0 || frame->data_len != 18u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x8eu) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    update->reserved = frame->data[1];
    for (size_t index = 0; index < 8u; ++index) {
        update->new_address64[index] = frame->data[index + 2u];
        update->old_address64[index] = frame->data[index + 10u];
    }
    return FR_OK;
}

fr_status_t fr_xbee_parse_transmit_status(
    const fr_xbee_frame_t *frame,
    fr_xbee_transmit_status_t *status
)
{
    if (frame == 0 || status == 0 || frame->data_len != 7u) {
        return FR_REJECT_PAYLOAD_INVALID;
    }
    if (frame->data[0] != 0x8bu) {
        return FR_REJECT_PAYLOAD_INVALID;
    }

    status->frame_id = frame->data[1];
    status->reserved16 = ((uint16_t)frame->data[2] << 8u) | frame->data[3];
    status->retry_count = frame->data[4];
    status->delivery_status = frame->data[5];
    status->discovery_status = frame->data[6];
    return FR_OK;
}

const char *fr_xbee_delivery_status_label(uint8_t delivery_status)
{
    switch (delivery_status) {
    case 0x00u:
        return "success";
    case 0x01u:
        return "mac_ack_failure";
    case 0x02u:
        return "cca_lbt_failure";
    case 0x03u:
        return "indirect_message_unrequested_or_no_spectrum";
    case 0x21u:
        return "network_ack_failure";
    case 0x25u:
        return "route_not_found";
    case 0x31u:
        return "internal_resource_error";
    case 0x32u:
        return "resource_error";
    case 0x74u:
        return "data_payload_too_large";
    case 0x75u:
        return "indirect_message_unrequested";
    default:
        return "unknown_delivery_status";
    }
}

const char *fr_xbee_discovery_status_label(uint8_t discovery_status)
{
    switch (discovery_status) {
    case 0x00u:
        return "no_discovery_overhead";
    case 0x02u:
        return "route_discovery";
    default:
        return "unknown_discovery_status";
    }
}
