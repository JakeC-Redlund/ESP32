#include "four_relay_core.h"
#include "test_support.h"

#include <string.h>

static void test_api2_codec(void)
{
    const uint8_t frame_data[] = {
        0x90, 0x00, 0x13, 0xa2, 0x00, 0x41, 0x7e, 0x7d, 0x11, 0x13, 0x01
    };
    uint8_t wire[64];
    size_t wire_len = 0;
    fr_xbee_frame_t decoded;
    const uint8_t expected_wire[] = {
        0x7e, 0x00, 0x0b, 0x90, 0x00, 0x7d, 0x33, 0xa2, 0x00, 0x41,
        0x7d, 0x5e, 0x7d, 0x5d, 0x7d, 0x31, 0x7d, 0x33, 0x01, 0x59
    };

    check(fr_xbee_encode_api2(frame_data, sizeof(frame_data), wire, sizeof(wire), &wire_len) == FR_OK,
          "XBee API2 encode succeeds in memory");
    check(wire_len > sizeof(frame_data), "XBee API2 encoder escapes reserved bytes");
    check(wire_len == sizeof(expected_wire), "XBee API2 escaped wire length is exact");
    check(memcmp(wire, expected_wire, sizeof(expected_wire)) == 0,
          "XBee API2 escaped wire bytes are exact");
    check(fr_xbee_decode_api2(wire, wire_len, &decoded) == FR_OK,
          "XBee API2 decode succeeds in memory");
    check(decoded.data_len == sizeof(frame_data), "XBee decoded length matches");
    check(memcmp(decoded.data, frame_data, sizeof(frame_data)) == 0,
          "XBee decoded frame matches input");

    wire[wire_len - 1u] ^= 0x01u;
    check(fr_xbee_decode_api2(wire, wire_len, &decoded) == FR_REJECT_PAYLOAD_INVALID,
          "XBee bad checksum rejects");

    const uint8_t status_frame[] = {0x8b, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00};
    check(fr_xbee_encode_api2(status_frame, sizeof(status_frame), wire, sizeof(wire), &wire_len) == FR_OK,
          "XBee transmit status frame encodes");
    check(fr_xbee_decode_api2(wire, wire_len, &decoded) == FR_OK,
          "XBee transmit status frame decodes");
    check(decoded.data_len == sizeof(status_frame), "XBee status frame decoded length matches");
    check(decoded.data[0] == 0x8b, "XBee status frame type preserved");

    const uint8_t bad_length[] = {0x7e, 0x00, 0x02, 0x90, 0x6f};
    check(fr_xbee_decode_api2(bad_length, sizeof(bad_length), &decoded) == FR_REJECT_PAYLOAD_INVALID,
          "XBee bad length rejects");

    const uint8_t truncated_escape[] = {0x7e, 0x00, 0x01, 0x7d};
    check(fr_xbee_decode_api2(truncated_escape, sizeof(truncated_escape), &decoded) == FR_REJECT_PAYLOAD_INVALID,
          "XBee truncated escape rejects");

    const uint8_t too_short[] = {0x7e, 0x00, 0x01};
    check(fr_xbee_decode_api2(too_short, sizeof(too_short), &decoded) == FR_REJECT_PAYLOAD_INVALID,
          "XBee short frame rejects");
}

static void test_api2_codec_boundaries(void)
{
    uint8_t wire[320];
    size_t wire_len = 0;
    fr_xbee_frame_t decoded;

    const uint8_t checksum_escape_frame[] = {0xec};
    const uint8_t expected_checksum_escape_wire[] = {
        0x7e, 0x00, 0x01, 0xec, 0x7d, 0x33
    };
    check(fr_xbee_encode_api2(
              checksum_escape_frame,
              sizeof(checksum_escape_frame),
              wire,
              sizeof(wire),
              &wire_len
          ) == FR_OK,
          "XBee API2 checksum-reserved frame encodes");
    check(wire_len == sizeof(expected_checksum_escape_wire),
          "XBee API2 checksum-reserved wire length is exact");
    check(memcmp(wire, expected_checksum_escape_wire, sizeof(expected_checksum_escape_wire)) == 0,
          "XBee API2 checksum byte is escaped");
    check(fr_xbee_decode_api2(wire, wire_len, &decoded) == FR_OK,
          "XBee API2 checksum-reserved frame decodes");
    check(decoded.data_len == sizeof(checksum_escape_frame), "XBee checksum-reserved decoded length matches");
    check(decoded.data[0] == checksum_escape_frame[0], "XBee checksum-reserved payload matches");

    uint8_t length_escape_frame[0x7eu];
    memset(length_escape_frame, 0, sizeof(length_escape_frame));
    length_escape_frame[0] = 0x01u;
    check(fr_xbee_encode_api2(
              length_escape_frame,
              sizeof(length_escape_frame),
              wire,
              sizeof(wire),
              &wire_len
          ) == FR_OK,
          "XBee API2 length-reserved frame encodes");
    check(wire[0] == 0x7eu && wire[1] == 0x00u && wire[2] == 0x7du && wire[3] == 0x5eu,
          "XBee API2 escapes reserved length byte");
    check(fr_xbee_decode_api2(wire, wire_len, &decoded) == FR_OK,
          "XBee API2 length-reserved frame decodes");
    check(decoded.data_len == sizeof(length_escape_frame), "XBee length-reserved decoded length matches");
    check(decoded.data[0] == 0x01u && decoded.data[125] == 0x00u,
          "XBee length-reserved payload bytes match");

    const uint8_t capacity_frame[] = {0x7e};
    check(fr_xbee_encode_api2(
              capacity_frame,
              sizeof(capacity_frame),
              wire,
              5u,
              &wire_len
          ) == FR_REJECT_PAYLOAD_INVALID,
          "XBee API2 encode rejects insufficient escaped capacity");

    uint8_t max_frame[FR_XBEE_MAX_FRAME_DATA];
    memset(max_frame, 0xa5, sizeof(max_frame));
    check(fr_xbee_encode_api2(max_frame, sizeof(max_frame), wire, sizeof(wire), &wire_len) == FR_OK,
          "XBee API2 max frame encodes");
    check(fr_xbee_decode_api2(wire, wire_len, &decoded) == FR_OK,
          "XBee API2 max frame decodes");
    check(decoded.data_len == FR_XBEE_MAX_FRAME_DATA, "XBee API2 max frame length matches");
    check(decoded.data[0] == 0xa5u && decoded.data[FR_XBEE_MAX_FRAME_DATA - 1u] == 0xa5u,
          "XBee API2 max frame payload matches");

    uint8_t over_max_frame[FR_XBEE_MAX_FRAME_DATA + 1u];
    memset(over_max_frame, 0, sizeof(over_max_frame));
    check(fr_xbee_encode_api2(
              over_max_frame,
              sizeof(over_max_frame),
              wire,
              sizeof(wire),
              &wire_len
          ) == FR_REJECT_PAYLOAD_INVALID,
          "XBee API2 encode rejects over-max frame");

    const uint8_t missing_delimiter[] = {0x00, 0x00, 0x01, 0x00, 0xff};
    check(fr_xbee_decode_api2(missing_delimiter, sizeof(missing_delimiter), &decoded) == FR_REJECT_PAYLOAD_INVALID,
          "XBee API2 decode rejects missing delimiter");

    const uint8_t extra_trailing_byte[] = {0x7e, 0x00, 0x01, 0x00, 0xff, 0x00};
    check(fr_xbee_decode_api2(extra_trailing_byte, sizeof(extra_trailing_byte), &decoded) == FR_REJECT_PAYLOAD_INVALID,
          "XBee API2 decode rejects trailing decoded byte");

    const uint8_t over_max_length[] = {0x7e, 0x00, 0x81, 0x00};
    check(fr_xbee_decode_api2(over_max_length, sizeof(over_max_length), &decoded) == FR_REJECT_PAYLOAD_INVALID,
          "XBee API2 decode rejects over-max length field");
}

static void test_at_response_parser(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_at_response_t response;
    const uint8_t at_response[] = {0x88, 0x01, 'V', 'R', 0x00, 0x22, 0x17};

    memcpy(frame.data, at_response, sizeof(at_response));
    frame.data_len = sizeof(at_response);
    check(fr_xbee_parse_at_response(&frame, &response) == FR_OK,
          "AT response frame parses command");
    check(response.frame_id == 0x01u, "AT response frame preserves frame id");
    check(strcmp(response.command, "VR") == 0, "AT response frame parses command name");
    check(response.command_status == 0x00u, "AT response frame preserves command status");
    check(response.value_len == 2u, "AT response frame preserves value length");

    frame.data[0] = 0x90u;
    check(fr_xbee_parse_at_response(&frame, &response) == FR_REJECT_PAYLOAD_INVALID,
          "AT response parser rejects receive-packet frame");
}

static void test_at_response_parser_boundaries(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_at_response_t response;
    const uint8_t no_value_response[] = {0x88, 0x02, 'N', 'I', 0x00};

    memcpy(frame.data, no_value_response, sizeof(no_value_response));
    frame.data_len = sizeof(no_value_response);
    check(fr_xbee_parse_at_response(&frame, &response) == FR_OK,
          "AT response parser accepts no-value response");
    check(response.frame_id == 0x02u, "AT response no-value frame id preserved");
    check(strcmp(response.command, "NI") == 0, "AT response no-value command parses");
    check(response.command_status == 0x00u, "AT response no-value status preserved");
    check(response.value_len == 0u, "AT response no-value length is zero");

    memset(&frame, 0, sizeof(frame));
    frame.data[0] = 0x88u;
    frame.data[1] = 0x03u;
    frame.data[2] = 'N';
    frame.data[3] = 'D';
    frame.data[4] = 0x00u;
    for (size_t index = 0; index < FR_XBEE_AT_VALUE_MAX + 1u; ++index) {
        frame.data[index + 5u] = (uint8_t)index;
    }
    frame.data_len = 5u + FR_XBEE_AT_VALUE_MAX + 1u;
    check(fr_xbee_parse_at_response(&frame, &response) == FR_REJECT_PAYLOAD_INVALID,
          "AT response parser rejects over-max value");
}

static void test_at_command_status_labels(void)
{
    check(strcmp(fr_xbee_at_command_status_label(0x00u), "ok") == 0,
          "AT command status 0x00 label is source-backed");
    check(strcmp(fr_xbee_at_command_status_label(0x01u), "error") == 0,
          "AT command status 0x01 label is source-backed");
    check(strcmp(fr_xbee_at_command_status_label(0x02u), "invalid_command") == 0,
          "AT command status 0x02 label is source-backed");
    check(strcmp(fr_xbee_at_command_status_label(0x03u), "invalid_parameter") == 0,
          "AT command status 0x03 label is source-backed");
    check(strcmp(fr_xbee_at_command_status_label(0x04u), "unknown_at_command_status") == 0,
          "AT command status 0x04 remains unknown for local 0x88");
    check(strcmp(fr_xbee_at_command_status_label(0x0cu), "unknown_at_command_status") == 0,
          "AT command status 0x0c remains unknown for local 0x88");
    check(strcmp(fr_xbee_at_command_status_label(0xffu), "unknown_at_command_status") == 0,
          "unknown AT command status falls back");
}

static void test_remote_at_response_parser(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_remote_at_response_t response;
    const uint8_t remote_at_response[] = {
        0x97, 0x52,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0x2b, 0xaa,
        0x12, 0x7e,
        'G', 'T',
        0x00
    };
    const uint8_t remote_at_query_response[] = {
        0x97, 0x52,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0x2b, 0xaa,
        0x12, 0x7e,
        'T', 'P',
        0x00,
        0x00, 0x2f
    };
    const uint8_t expected_source64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0x2b, 0xaa
    };

    memcpy(frame.data, remote_at_response, sizeof(remote_at_response));
    frame.data_len = sizeof(remote_at_response);
    check(fr_xbee_parse_remote_at_response(&frame, &response) == FR_OK,
          "remote AT response frame parses");
    check(response.frame_id == 0x52u, "remote AT response frame id is preserved");
    check(memcmp(response.source64, expected_source64, sizeof(expected_source64)) == 0,
          "remote AT response source64 byte order is preserved");
    check(response.reserved16 == 0x127eu, "remote AT response reserved16 is big-endian");
    check(strcmp(response.command, "GT") == 0, "remote AT response command parses");
    check(response.command_status == 0x00u, "remote AT response status is raw");
    check(response.value_len == 0u, "remote AT response no-data length is zero");

    memcpy(frame.data, remote_at_query_response, sizeof(remote_at_query_response));
    frame.data_len = sizeof(remote_at_query_response);
    check(fr_xbee_parse_remote_at_response(&frame, &response) == FR_OK,
          "remote AT response query frame parses command data");
    check(strcmp(response.command, "TP") == 0, "remote AT response query command parses");
    check(response.value_len == 2u, "remote AT response query value length parses");
    check(response.value[0] == 0x00u && response.value[1] == 0x2fu,
          "remote AT response query value bytes parse");

    frame.data[0] = 0x88u;
    check(fr_xbee_parse_remote_at_response(&frame, &response) == FR_REJECT_PAYLOAD_INVALID,
          "remote AT response parser rejects local AT response frame");
}

static void test_remote_at_response_parser_boundaries(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_remote_at_response_t response;
    const size_t max_value_len = FR_XBEE_AT_VALUE_MAX;

    memset(&frame, 0, sizeof(frame));
    frame.data[0] = 0x97u;
    frame.data[1] = 0x7bu;
    for (size_t index = 0; index < 8u; ++index) {
        frame.data[index + 2u] = (uint8_t)(0xa0u + index);
    }
    frame.data[10] = 0xbeu;
    frame.data[11] = 0xefu;
    frame.data[12] = 'N';
    frame.data[13] = 'I';
    frame.data[14] = 0x04u;
    for (size_t index = 0; index < max_value_len; ++index) {
        frame.data[index + 15u] = (uint8_t)(index & 0xffu);
    }
    frame.data_len = 15u + max_value_len;

    check(fr_xbee_parse_remote_at_response(0, &response) == FR_REJECT_PAYLOAD_INVALID,
          "remote AT response parser rejects null frame");
    check(fr_xbee_parse_remote_at_response(&frame, 0) == FR_REJECT_PAYLOAD_INVALID,
          "remote AT response parser rejects null output");
    check(fr_xbee_parse_remote_at_response(&frame, &response) == FR_OK,
          "remote AT response parser accepts max command data");
    check(response.source64[0] == 0xa0u && response.source64[7] == 0xa7u,
          "remote AT response max source64 parses");
    check(response.reserved16 == 0xbeefu, "remote AT response non-fffe reserved16 is raw");
    check(strcmp(response.command, "NI") == 0, "remote AT response max command parses");
    check(response.command_status == 0x04u, "remote AT response raw status 0x04 is preserved");
    check(response.value_len == max_value_len, "remote AT response max value length parses");
    check(response.value[0] == 0x00u && response.value[max_value_len - 1u] == (uint8_t)((max_value_len - 1u) & 0xffu),
          "remote AT response max value bytes parse");

    frame.data_len = 14u;
    check(fr_xbee_parse_remote_at_response(&frame, &response) == FR_REJECT_PAYLOAD_INVALID,
          "remote AT response parser rejects short frame data");

    frame.data_len = 15u + FR_XBEE_AT_VALUE_MAX + 1u;
    check(fr_xbee_parse_remote_at_response(&frame, &response) == FR_REJECT_PAYLOAD_INVALID,
          "remote AT response parser rejects over-max command data");

    frame.data_len = FR_XBEE_MAX_FRAME_DATA + 1u;
    check(fr_xbee_parse_remote_at_response(&frame, &response) == FR_REJECT_PAYLOAD_INVALID,
          "remote AT response parser rejects over-max host frame length");
}

static void test_remote_at_command_status_labels(void)
{
    check(strcmp(fr_xbee_remote_at_command_status_label(0x00u), "ok") == 0,
          "remote AT command status 0x00 label is source-backed");
    check(strcmp(fr_xbee_remote_at_command_status_label(0x01u), "error") == 0,
          "remote AT command status 0x01 label is source-backed");
    check(strcmp(fr_xbee_remote_at_command_status_label(0x02u), "invalid_command") == 0,
          "remote AT command status 0x02 label is source-backed");
    check(strcmp(fr_xbee_remote_at_command_status_label(0x03u), "invalid_parameter") == 0,
          "remote AT command status 0x03 label is source-backed");
    check(strcmp(fr_xbee_remote_at_command_status_label(0x04u), "transmission_failure") == 0,
          "remote AT command status 0x04 label is source-backed");
    check(strcmp(fr_xbee_remote_at_command_status_label(0x0cu), "encryption_error") == 0,
          "remote AT command status 0x0c label is source-backed");
    check(strcmp(fr_xbee_remote_at_command_status_label(0xffu), "unknown_remote_at_command_status") == 0,
          "unknown remote AT command status falls back");
}

static void test_modem_status_parser(void)
{
    const uint8_t modem_frame[] = {0x8a, 0x00};
    uint8_t wire[16];
    size_t wire_len = 0;
    fr_xbee_frame_t frame;
    fr_xbee_modem_status_t status;

    check(fr_xbee_encode_api2(modem_frame, sizeof(modem_frame), wire, sizeof(wire), &wire_len) == FR_OK,
          "modem-status frame encodes through API2");
    check(fr_xbee_decode_api2(wire, wire_len, &frame) == FR_OK,
          "modem-status frame decodes through API2");
    check(fr_xbee_parse_modem_status(&frame, &status) == FR_OK,
          "modem-status frame parses");
    check(status.status == 0x00u, "modem-status raw status is preserved");

    frame.data[0] = 0x90u;
    check(fr_xbee_parse_modem_status(&frame, &status) == FR_REJECT_PAYLOAD_INVALID,
          "modem-status parser rejects receive-packet frame");

    memcpy(frame.data, modem_frame, sizeof(modem_frame));
    frame.data_len = 1u;
    check(fr_xbee_parse_modem_status(&frame, &status) == FR_REJECT_PAYLOAD_INVALID,
          "modem-status parser rejects short frame data");

    frame.data_len = 3u;
    frame.data[2] = 0x00u;
    check(fr_xbee_parse_modem_status(&frame, &status) == FR_REJECT_PAYLOAD_INVALID,
          "modem-status parser rejects long frame data");

    frame.data_len = sizeof(modem_frame);
    check(fr_xbee_parse_modem_status(0, &status) == FR_REJECT_PAYLOAD_INVALID,
          "modem-status parser rejects null frame");
    check(fr_xbee_parse_modem_status(&frame, 0) == FR_REJECT_PAYLOAD_INVALID,
          "modem-status parser rejects null output");
}

static void test_modem_status_labels(void)
{
    check(strcmp(fr_xbee_modem_status_label(0x00u), "hardware_reset_or_power_up") == 0,
          "modem status 0x00 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x01u), "watchdog_timer_reset") == 0,
          "modem status 0x01 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x02u), "joined_network") == 0,
          "modem status 0x02 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x03u), "left_network") == 0,
          "modem status 0x03 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x06u), "coordinator_started") == 0,
          "modem status 0x06 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x07u), "network_security_key_updated") == 0,
          "modem status 0x07 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x0bu), "network_woke_up") == 0,
          "modem status 0x0b label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x0cu), "network_went_to_sleep") == 0,
          "modem status 0x0c label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x0du), "voltage_supply_limit_exceeded") == 0,
          "modem status 0x0d label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x0eu), "digi_remote_manager_connected") == 0,
          "modem status 0x0e label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x0fu), "digi_remote_manager_disconnected") == 0,
          "modem status 0x0f label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x11u), "modem_configuration_changed_while_join_in_progress") == 0,
          "modem status 0x11 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x12u), "access_fault") == 0,
          "modem status 0x12 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x13u), "fatal_error") == 0,
          "modem status 0x13 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x32u), "ble_connect") == 0,
          "modem status 0x32 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x33u), "ble_disconnect") == 0,
          "modem status 0x33 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x34u), "bandmask_configuration_failed") == 0,
          "modem status 0x34 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x35u), "cellular_component_update_started") == 0,
          "modem status 0x35 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x36u), "cellular_component_update_failed") == 0,
          "modem status 0x36 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x37u), "cellular_component_update_completed") == 0,
          "modem status 0x37 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x38u), "xbee_firmware_update_started") == 0,
          "modem status 0x38 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x39u), "xbee_firmware_update_failed") == 0,
          "modem status 0x39 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x3au), "xbee_firmware_update_applying") == 0,
          "modem status 0x3a label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x3bu), "secure_session_established") == 0,
          "modem status 0x3b label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x3cu), "secure_session_ended") == 0,
          "modem status 0x3c label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x3du), "secure_session_authentication_failed") == 0,
          "modem status 0x3d label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x3eu), "coordinator_pan_id_conflict_no_action") == 0,
          "modem status 0x3e label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x3fu), "coordinator_changed_pan_id_due_to_conflict") == 0,
          "modem status 0x3f label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x40u), "router_pan_id_changed_by_coordinator_due_to_conflict") == 0,
          "modem status 0x40 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x42u), "network_watchdog_timeout_expired") == 0,
          "modem status 0x42 label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x80u), "stack_error") == 0,
          "modem status 0x80 stack-error range label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0xffu), "stack_error") == 0,
          "modem status 0xff stack-error range label is source-backed");
    check(strcmp(fr_xbee_modem_status_label(0x04u), "unknown_modem_status") == 0,
          "unlisted modem status 0x04 falls back");
    check(strcmp(fr_xbee_modem_status_label(0x41u), "unknown_modem_status") == 0,
          "unlisted modem status 0x41 falls back");
    check(strcmp(fr_xbee_modem_status_label(0x7fu), "unknown_modem_status") == 0,
          "unlisted modem status 0x7f falls back");
}

static void test_transmit_status_parser(void)
{
    const uint8_t status_frame[] = {0x8b, 0x42, 0xff, 0xfe, 0x03, 0x24, 0x40};
    uint8_t wire[64];
    size_t wire_len = 0;
    fr_xbee_frame_t frame;
    fr_xbee_transmit_status_t status;

    check(fr_xbee_encode_api2(status_frame, sizeof(status_frame), wire, sizeof(wire), &wire_len) == FR_OK,
          "transmit-status frame encodes through API2");
    check(fr_xbee_decode_api2(wire, wire_len, &frame) == FR_OK,
          "transmit-status frame decodes through API2");
    check(fr_xbee_parse_transmit_status(&frame, &status) == FR_OK,
          "transmit-status frame parses");
    check(status.frame_id == 0x42u, "transmit-status frame id parses");
    check(status.reserved16 == 0xfffeu, "transmit-status reserved16 parses");
    check(status.retry_count == 0x03u, "transmit-status retry count parses");
    check(status.delivery_status == 0x24u, "transmit-status delivery status preserved");
    check(status.discovery_status == 0x40u, "transmit-status discovery status preserved");

    frame.data[0] = 0x90u;
    check(fr_xbee_parse_transmit_status(&frame, &status) == FR_REJECT_PAYLOAD_INVALID,
          "transmit-status parser rejects receive-packet frame");

    memcpy(frame.data, status_frame, sizeof(status_frame));
    frame.data_len = 6u;
    check(fr_xbee_parse_transmit_status(&frame, &status) == FR_REJECT_PAYLOAD_INVALID,
          "transmit-status parser rejects short frame data");

    frame.data_len = 8u;
    frame.data[7] = 0x00u;
    check(fr_xbee_parse_transmit_status(&frame, &status) == FR_REJECT_PAYLOAD_INVALID,
          "transmit-status parser rejects long frame data");

    frame.data_len = sizeof(status_frame);
    check(fr_xbee_parse_transmit_status(0, &status) == FR_REJECT_PAYLOAD_INVALID,
          "transmit-status parser rejects null frame");
    check(fr_xbee_parse_transmit_status(&frame, 0) == FR_REJECT_PAYLOAD_INVALID,
          "transmit-status parser rejects null output");
}

static void test_transmit_status_labels(void)
{
    check(strcmp(fr_xbee_delivery_status_label(0x00u), "success") == 0,
          "delivery status 0x00 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x01u), "mac_ack_failure") == 0,
          "delivery status 0x01 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x02u), "cca_lbt_failure") == 0,
          "delivery status 0x02 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x03u), "indirect_message_unrequested_or_no_spectrum") == 0,
          "delivery status 0x03 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x21u), "network_ack_failure") == 0,
          "delivery status 0x21 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x25u), "route_not_found") == 0,
          "delivery status 0x25 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x31u), "internal_resource_error") == 0,
          "delivery status 0x31 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x32u), "resource_error") == 0,
          "delivery status 0x32 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x74u), "data_payload_too_large") == 0,
          "delivery status 0x74 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x75u), "indirect_message_unrequested") == 0,
          "delivery status 0x75 label is source-backed");
    check(strcmp(fr_xbee_delivery_status_label(0x24u), "unknown_delivery_status") == 0,
          "delivery status 0x24 remains unknown");
    check(strcmp(fr_xbee_delivery_status_label(0xffu), "unknown_delivery_status") == 0,
          "unknown delivery status falls back");

    check(strcmp(fr_xbee_discovery_status_label(0x00u), "no_discovery_overhead") == 0,
          "discovery status 0x00 label is source-backed");
    check(strcmp(fr_xbee_discovery_status_label(0x02u), "route_discovery") == 0,
          "discovery status 0x02 label is source-backed");
    check(strcmp(fr_xbee_discovery_status_label(0x40u), "unknown_discovery_status") == 0,
          "discovery status 0x40 remains unknown");
    check(strcmp(fr_xbee_discovery_status_label(0xffu), "unknown_discovery_status") == 0,
          "unknown discovery status falls back");
}

static void test_receive_packet_parser(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_receive_packet_t packet;
    const uint8_t receive_packet[] = {
        0x90,
        0x00, 0x13, 0xa2, 0x00, 0x41, 0x52, 0x53, 0x54,
        0xff, 0xfe,
        0x01,
        'O', 'K'
    };

    memcpy(frame.data, receive_packet, sizeof(receive_packet));
    frame.data_len = sizeof(receive_packet);
    check(fr_xbee_parse_receive_packet(&frame, &packet) == FR_OK,
          "receive-packet payload parses");
    check(packet.source64[0] == 0x00u && packet.source64[7] == 0x54u,
          "receive-packet source64 parses");
    check(packet.source16 == 0xfffeu, "receive-packet source16 parses");
    check(packet.receive_options == 0x01u, "receive-packet options parse");
    check(packet.payload_len == 2u, "receive-packet payload length parses");
    check(packet.payload[0] == 'O' && packet.payload[1] == 'K',
          "receive-packet payload bytes parse");

    frame.data_len = 12u;
    check(fr_xbee_parse_receive_packet(&frame, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "receive-packet parser rejects empty payload");
    frame.data_len = sizeof(receive_packet);
    frame.data[0] = 0x88u;
    check(fr_xbee_parse_receive_packet(&frame, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "receive-packet parser rejects AT response frame");
}

static void test_receive_packet_parser_boundaries(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_receive_packet_t packet;
    const size_t max_payload_len = FR_XBEE_MAX_FRAME_DATA - 12u;

    memset(&frame, 0, sizeof(frame));
    frame.data[0] = 0x90u;
    for (size_t index = 0; index < 8u; ++index) {
        frame.data[index + 1u] = (uint8_t)(0xa0u + index);
    }
    frame.data[9] = 0x12u;
    frame.data[10] = 0x34u;
    frame.data[11] = 0x02u;
    for (size_t index = 0; index < max_payload_len; ++index) {
        frame.data[index + 12u] = (uint8_t)(index & 0xffu);
    }
    frame.data_len = FR_XBEE_MAX_FRAME_DATA;
    check(fr_xbee_parse_receive_packet(&frame, &packet) == FR_OK,
          "receive-packet parser accepts max host frame");
    check(packet.source64[0] == 0xa0u && packet.source64[7] == 0xa7u,
          "receive-packet max source64 parses");
    check(packet.source16 == 0x1234u, "receive-packet max source16 parses");
    check(packet.receive_options == 0x02u, "receive-packet max options parse");
    check(packet.payload_len == max_payload_len, "receive-packet max payload length parses");
    check(packet.payload[0] == 0x00u && packet.payload[max_payload_len - 1u] == (uint8_t)((max_payload_len - 1u) & 0xffu),
          "receive-packet max payload bytes parse");

    frame.data_len = FR_XBEE_MAX_FRAME_DATA + 1u;
    check(fr_xbee_parse_receive_packet(&frame, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "receive-packet parser rejects over-max host frame length");
}

static void test_explicit_receive_packet_parser(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_explicit_receive_packet_t packet;
    const uint8_t explicit_receive_packet[] = {
        0x91,
        0x00, 0x13, 0xa2, 0x00, 0x41, 0xae, 0xb5, 0x4e,
        0xff, 0xfe,
        0xe8,
        0xe8,
        0x00, 0x11,
        0xc1, 0x05,
        0xc1,
        'T', 'x', 'D', 'a', 't', 'a'
    };
    const uint8_t expected_source64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x41, 0xae, 0xb5, 0x4e
    };

    memcpy(frame.data, explicit_receive_packet, sizeof(explicit_receive_packet));
    frame.data_len = sizeof(explicit_receive_packet);
    check(fr_xbee_parse_explicit_receive_packet(&frame, &packet) == FR_OK,
          "explicit-receive packet parses");
    check(memcmp(packet.source64, expected_source64, sizeof(expected_source64)) == 0,
          "explicit-receive source64 parses");
    check(packet.reserved16 == 0xfffeu, "explicit-receive reserved16 parses");
    check(packet.source_endpoint == 0xe8u, "explicit-receive source endpoint parses");
    check(packet.destination_endpoint == 0xe8u, "explicit-receive destination endpoint parses");
    check(packet.cluster_id == 0x0011u, "explicit-receive cluster id parses");
    check(packet.profile_id == 0xc105u, "explicit-receive profile id parses");
    check(packet.receive_options == 0xc1u, "explicit-receive options parse");
    check(packet.payload_len == 6u, "explicit-receive payload length parses");
    check(memcmp(packet.payload, "TxData", 6u) == 0,
          "explicit-receive payload bytes parse");

    frame.data[0] = 0x90u;
    check(fr_xbee_parse_explicit_receive_packet(&frame, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "explicit-receive parser rejects receive-packet frame");
}

static void test_explicit_receive_packet_parser_boundaries(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_explicit_receive_packet_t packet;
    const size_t max_payload_len = FR_XBEE_MAX_FRAME_DATA - 18u;

    memset(&frame, 0, sizeof(frame));
    frame.data[0] = 0x91u;
    for (size_t index = 0; index < 8u; ++index) {
        frame.data[index + 1u] = (uint8_t)(0xa0u + index);
    }
    frame.data[9] = 0x12u;
    frame.data[10] = 0x34u;
    frame.data[11] = 0x56u;
    frame.data[12] = 0x78u;
    frame.data[13] = 0xabu;
    frame.data[14] = 0xcdu;
    frame.data[15] = 0xc1u;
    frame.data[16] = 0x05u;
    frame.data[17] = 0x82u;
    for (size_t index = 0; index < max_payload_len; ++index) {
        frame.data[index + 18u] = (uint8_t)(index & 0xffu);
    }
    frame.data_len = FR_XBEE_MAX_FRAME_DATA;

    check(fr_xbee_parse_explicit_receive_packet(0, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "explicit-receive parser rejects null frame");
    check(fr_xbee_parse_explicit_receive_packet(&frame, 0) == FR_REJECT_PAYLOAD_INVALID,
          "explicit-receive parser rejects null output");
    check(fr_xbee_parse_explicit_receive_packet(&frame, &packet) == FR_OK,
          "explicit-receive parser accepts max host frame");
    check(packet.reserved16 == 0x1234u, "explicit-receive reserved16 is big-endian");
    check(packet.source_endpoint == 0x56u, "explicit-receive synthetic source endpoint parses");
    check(packet.destination_endpoint == 0x78u,
          "explicit-receive synthetic destination endpoint parses");
    check(packet.cluster_id == 0xabcdu, "explicit-receive cluster id is big-endian");
    check(packet.profile_id == 0xc105u, "explicit-receive profile id is big-endian");
    check(packet.receive_options == 0x82u, "explicit-receive synthetic options parse");
    check(packet.payload_len == max_payload_len, "explicit-receive max payload length parses");
    check(packet.payload[0] == 0x00u && packet.payload[max_payload_len - 1u] == (uint8_t)((max_payload_len - 1u) & 0xffu),
          "explicit-receive max payload bytes parse");

    frame.data_len = 17u;
    check(fr_xbee_parse_explicit_receive_packet(&frame, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "explicit-receive parser rejects short frame data");

    frame.data_len = 18u;
    check(fr_xbee_parse_explicit_receive_packet(&frame, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "explicit-receive parser rejects header-only frame data");

    frame.data_len = FR_XBEE_MAX_FRAME_DATA + 1u;
    check(fr_xbee_parse_explicit_receive_packet(&frame, &packet) == FR_REJECT_PAYLOAD_INVALID,
          "explicit-receive parser rejects over-max host frame length");
}

static void test_receive_options_helpers(void)
{
    check(!fr_xbee_receive_options_acknowledged(0x00u),
          "receive option 0x00 is not acknowledged");
    check(fr_xbee_receive_options_acknowledged(0x01u),
          "receive option 0x01 acknowledged bit is source-backed");
    check(fr_xbee_receive_options_acknowledged(0xc1u),
          "receive option acknowledged bit survives combined options");
    check(!fr_xbee_receive_options_broadcast(0x00u),
          "receive option 0x00 is not broadcast");
    check(fr_xbee_receive_options_broadcast(0x02u),
          "receive option 0x02 broadcast bit is source-backed");
    check(fr_xbee_receive_options_broadcast(0xc3u),
          "receive option broadcast bit survives combined options");
    check(fr_xbee_receive_options_acknowledged(0x03u)
              && fr_xbee_receive_options_broadcast(0x03u),
          "receive options acknowledge and broadcast bits may combine");

    check(strcmp(fr_xbee_receive_options_delivery_method_label(0x00u), "invalid_delivery_method") == 0,
          "receive options delivery bits 0x00 are invalid");
    check(strcmp(fr_xbee_receive_options_delivery_method_label(0x40u), "point_multipoint") == 0,
          "receive options delivery bits 0x40 label point-multipoint");
    check(strcmp(fr_xbee_receive_options_delivery_method_label(0x80u), "directed_broadcast") == 0,
          "receive options delivery bits 0x80 label directed broadcast");
    check(strcmp(fr_xbee_receive_options_delivery_method_label(0xc0u), "digimesh") == 0,
          "receive options delivery bits 0xc0 label DigiMesh");
    check(strcmp(fr_xbee_receive_options_delivery_method_label(0x41u), "point_multipoint") == 0,
          "receive options lower bits do not affect point-multipoint label");
    check(strcmp(fr_xbee_receive_options_delivery_method_label(0x82u), "directed_broadcast") == 0,
          "receive options lower bits do not affect directed-broadcast label");
    check(strcmp(fr_xbee_receive_options_delivery_method_label(0xc1u), "digimesh") == 0,
          "receive options acknowledged bit does not affect DigiMesh label");
    check(strcmp(fr_xbee_receive_options_delivery_method_label(0xffu), "digimesh") == 0,
          "receive options reserved lower bits do not affect DigiMesh label");
}

static void test_route_information_parser(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_route_information_t route;
    const uint8_t route_frame[] = {
        0x8d, 0x12, 0x27, 0x6b, 0xeb, 0xca, 0x93, 0x00, 0x00, 0x00,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xdd, 0xdd,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xaa, 0xaa,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xbb, 0xbb,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xcc, 0xcc
    };
    const uint8_t expected_destination64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xdd, 0xdd
    };
    const uint8_t expected_source64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xaa, 0xaa
    };
    const uint8_t expected_responder64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xbb, 0xbb
    };
    const uint8_t expected_receiver64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xcc, 0xcc
    };

    memcpy(frame.data, route_frame, sizeof(route_frame));
    frame.data_len = sizeof(route_frame);
    check(fr_xbee_parse_route_information(&frame, &route) == FR_OK,
          "route-information frame parses");
    check(route.source_event == 0x12u, "route-information source event parses");
    check(route.data_length == 0x27u, "route-information data length parses");
    check(route.timestamp_us == 0x6bebca93u, "route-information timestamp parses");
    check(route.ack_timeout_count == 0x00u, "route-information ACK timeout count parses");
    check(route.tx_blocked_count == 0x00u, "route-information TX blocked count parses");
    check(route.reserved == 0x00u, "route-information reserved byte parses");
    check(memcmp(route.destination64, expected_destination64, sizeof(expected_destination64)) == 0,
          "route-information destination64 parses");
    check(memcmp(route.source64, expected_source64, sizeof(expected_source64)) == 0,
          "route-information source64 parses");
    check(memcmp(route.responder64, expected_responder64, sizeof(expected_responder64)) == 0,
          "route-information responder64 parses");
    check(memcmp(route.receiver64, expected_receiver64, sizeof(expected_receiver64)) == 0,
          "route-information receiver64 parses");
    check(route.trailing_len == 0u, "route-information source-listed body has no trailing bytes");

    frame.data[0] = 0x90u;
    check(fr_xbee_parse_route_information(&frame, &route) == FR_REJECT_PAYLOAD_INVALID,
          "route-information parser rejects receive-packet frame");
}

static void test_route_information_parser_boundaries(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_route_information_t route;
    const uint8_t route_frame[] = {
        0x8d, 0x11, 0x27, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x00,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7
    };

    memcpy(frame.data, route_frame, sizeof(route_frame));
    frame.data_len = sizeof(route_frame);
    check(fr_xbee_parse_route_information(0, &route) == FR_REJECT_PAYLOAD_INVALID,
          "route-information parser rejects null frame");
    check(fr_xbee_parse_route_information(&frame, 0) == FR_REJECT_PAYLOAD_INVALID,
          "route-information parser rejects null output");

    frame.data_len = sizeof(route_frame) - 1u;
    frame.data[2] = 0x26u;
    check(fr_xbee_parse_route_information(&frame, &route) == FR_REJECT_PAYLOAD_INVALID,
          "route-information parser rejects below-minimum body");

    memcpy(frame.data, route_frame, sizeof(route_frame));
    frame.data_len = sizeof(route_frame);
    frame.data[2] = 0x28u;
    check(fr_xbee_parse_route_information(&frame, &route) == FR_REJECT_PAYLOAD_INVALID,
          "route-information parser rejects data-length mismatch");

    memcpy(frame.data, route_frame, sizeof(route_frame));
    frame.data_len = sizeof(route_frame) + 1u;
    frame.data[sizeof(route_frame)] = 0xeeu;
    check(fr_xbee_parse_route_information(&frame, &route) == FR_REJECT_PAYLOAD_INVALID,
          "route-information parser rejects long frame without matching data length");

    memcpy(frame.data, route_frame, sizeof(route_frame));
    frame.data[2] = 0x29u;
    frame.data[sizeof(route_frame)] = 0xeeu;
    frame.data[sizeof(route_frame) + 1u] = 0x55u;
    frame.data_len = sizeof(route_frame) + 2u;
    check(fr_xbee_parse_route_information(&frame, &route) == FR_OK,
          "route-information parser accepts future appended fields");
    check(route.trailing_len == 2u, "route-information trailing length tracks future bytes");
    check(route.destination64[0] == 0xa0u && route.receiver64[7] == 0xd7u,
          "route-information future fields do not shift source-listed addresses");
}

static void test_route_information_source_event_labels(void)
{
    check(strcmp(fr_xbee_route_information_source_event_label(0x11u), "nack") == 0,
          "route-information source event 0x11 label is source-backed");
    check(strcmp(fr_xbee_route_information_source_event_label(0x12u), "trace_route") == 0,
          "route-information source event 0x12 label is source-backed");
    check(strcmp(fr_xbee_route_information_source_event_label(0x00u), "unknown_route_information_source_event") == 0,
          "route-information source event 0x00 remains unknown");
    check(strcmp(fr_xbee_route_information_source_event_label(0xffu), "unknown_route_information_source_event") == 0,
          "unknown route-information source event falls back");
}

static void test_aggregate_addressing_update_parser(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_aggregate_addressing_update_t update;
    const uint8_t aggregate_frame[] = {
        0x8e, 0x00,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xbb, 0xbb,
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xaa, 0xaa
    };
    const uint8_t expected_new_address64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xbb, 0xbb
    };
    const uint8_t expected_old_address64[] = {
        0x00, 0x13, 0xa2, 0x00, 0x40, 0x52, 0xaa, 0xaa
    };

    memcpy(frame.data, aggregate_frame, sizeof(aggregate_frame));
    frame.data_len = sizeof(aggregate_frame);
    check(fr_xbee_parse_aggregate_addressing_update(&frame, &update) == FR_OK,
          "aggregate-addressing update frame parses");
    check(update.reserved == 0x00u, "aggregate-addressing update reserved byte parses");
    check(memcmp(update.new_address64, expected_new_address64, sizeof(expected_new_address64)) == 0,
          "aggregate-addressing update new DH/DL address parses");
    check(memcmp(update.old_address64, expected_old_address64, sizeof(expected_old_address64)) == 0,
          "aggregate-addressing update old DH/DL address parses");

    frame.data[0] = 0x90u;
    check(fr_xbee_parse_aggregate_addressing_update(&frame, &update) == FR_REJECT_PAYLOAD_INVALID,
          "aggregate-addressing update parser rejects receive-packet frame");
}

static void test_aggregate_addressing_update_parser_boundaries(void)
{
    fr_xbee_frame_t frame;
    fr_xbee_aggregate_addressing_update_t update;
    const uint8_t aggregate_frame[] = {
        0x8e, 0x7f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7
    };

    memcpy(frame.data, aggregate_frame, sizeof(aggregate_frame));
    frame.data_len = sizeof(aggregate_frame);
    check(fr_xbee_parse_aggregate_addressing_update(0, &update) == FR_REJECT_PAYLOAD_INVALID,
          "aggregate-addressing update parser rejects null frame");
    check(fr_xbee_parse_aggregate_addressing_update(&frame, 0) == FR_REJECT_PAYLOAD_INVALID,
          "aggregate-addressing update parser rejects null output");

    frame.data_len = sizeof(aggregate_frame) - 1u;
    check(fr_xbee_parse_aggregate_addressing_update(&frame, &update) == FR_REJECT_PAYLOAD_INVALID,
          "aggregate-addressing update parser rejects short frame data");

    frame.data_len = sizeof(aggregate_frame) + 1u;
    frame.data[sizeof(aggregate_frame)] = 0x00u;
    check(fr_xbee_parse_aggregate_addressing_update(&frame, &update) == FR_REJECT_PAYLOAD_INVALID,
          "aggregate-addressing update parser rejects long frame data");

    frame.data_len = sizeof(aggregate_frame);
    check(fr_xbee_parse_aggregate_addressing_update(&frame, &update) == FR_OK,
          "aggregate-addressing update parser preserves nonzero reserved byte");
    check(update.reserved == 0x7fu, "aggregate-addressing update reserved byte is raw");
    check(update.new_address64[0] == 0xa0u && update.new_address64[7] == 0xa7u,
          "aggregate-addressing update new address byte order is preserved");
    check(update.old_address64[0] == 0xb0u && update.old_address64[7] == 0xb7u,
          "aggregate-addressing update old address byte order is preserved");
}

int main(void)
{
    test_api2_codec();
    test_api2_codec_boundaries();
    test_at_response_parser();
    test_at_response_parser_boundaries();
    test_at_command_status_labels();
    test_remote_at_response_parser();
    test_remote_at_response_parser_boundaries();
    test_remote_at_command_status_labels();
    test_modem_status_parser();
    test_modem_status_labels();
    test_transmit_status_parser();
    test_transmit_status_labels();
    test_receive_packet_parser();
    test_receive_packet_parser_boundaries();
    test_explicit_receive_packet_parser();
    test_explicit_receive_packet_parser_boundaries();
    test_receive_options_helpers();
    test_route_information_parser();
    test_route_information_parser_boundaries();
    test_route_information_source_event_labels();
    test_aggregate_addressing_update_parser();
    test_aggregate_addressing_update_parser_boundaries();
    return finish_tests("test_xbee_frame_codec");
}
