#!/usr/bin/env python3
"""Firmware skeleton and safe-core contract audits."""

from __future__ import annotations

from pathlib import Path

from scaffold_audit_data import (
    FIRMWARE_SOURCE_SCAN_ROOT,
    FORBIDDEN_FIRMWARE_MARKERS,
    ROOT,
)
from scaffold_audit_docs import require_markers

UART_BRIDGE_SOURCE = "firmware/projects/four-relay-xbee-wifi/main/main.c"


def audit_firmware_readme(root: Path = ROOT) -> list[str]:
    firmware_readme = (
        root / "firmware/projects/four-relay-xbee-wifi/README.md"
    ).read_text(encoding="utf-8")
    return require_markers(firmware_readme, [
        "## Verified facts",
        "## Assumptions",
        "## Unknowns",
        "## Hard gates",
        "No GPIO writes",
        "Encoder GPIO reads are allowed only for this input-only menu/diagnostic work",
        "No expander writes",
        "No XBee setting writes",
        "UART bridge writes are allowed only for the normal bridge feature",
        "LCD I2C writes are allowed only for this display-only menu feature",
        "No flash or monitor step",
        "No live bench mutation outside accepted named gates",
        "encoder menu gate,",
        "completed COM6-only write/verify",
        "PF0530F",
        "PF0530G",
        "PF0530H",
        "PF0530I",
        "PF0530J",
        "PF0530K",
        "PF0530L",
        "PF0530M",
        "PF0530N",
        "PF0530O",
        "PF0530P",
        "PF0530Q",
        "PF0530R",
        "PF0530T",
        "PF0530U",
        "PF0530V",
        "PF0530W",
        "FR_DIAG_XBEE_BRIDGE_CLOSED 1",
        "SRC-LOCAL-FOUR-RELAY-KY040-ENCODER-MENU-PF0530F-2026-05-30",
        "SRC-LOCAL-FOUR-RELAY-KY040-LCD-INIT-DIAG-PF0530G-2026-05-30",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530H-2026-05-31",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530I-2026-05-31",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530J-2026-05-31",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530K-2026-05-31",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530L-2026-05-31",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530M-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530N-SCROLLING-XML-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530O-REAL-MENU-CAL-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530O-LIVE-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530P-DEBOUNCE-CAL-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530P-LIVE-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530Q-QUIET-CAL-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530Q-LIVE-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530R-DETENT-CAL-2026-06-01",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530T-RESPONSIVE-2026-06-02",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530U-RESPONSIVE-V7-2026-06-02",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530V-PCNT-SOURCE-BUILD-2026-06-02",
        "SRC-LOCAL-FOUR-RELAY-KY040-BBS-LCD-MENU-PF0530W-VISUAL-ART-2026-06-02",
        "pure-C API payload validation",
        "normalized state snapshots",
        "split host-test binaries",
        "SRC-ESP-IDF-STABLE-ESP32",
        "SRC-ESP-IDF-I2C",
        "SRC-LOCAL-ESP32-XBEE-UART-BRIDGE-FLASH-RETEST-2026-05-30",
        "SRC-LOCAL-FOUR-RELAY-LCD-I2C-TEST-FIRMWARE-2026-05-30",
        "SRC-LOCAL-FOUR-RELAY-ENCODER-MENU-FIRMWARE-2026-05-30",
        "SRC-LOCAL-FOUR-RELAY-ENCODER-RAW-DIAGNOSTICS-2026-05-30",
        "page-0 raw levels and transition",
        "pin-finder",
        "SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19",
    ], "firmware skeleton README")


def audit_safe_core_contract(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    core_header = (
        root
        / "firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h"
    ).read_text(encoding="utf-8")
    failures.extend(require_markers(core_header, [
        "FR_REJECT_HARDWARE_GATE_OPEN",
        "fr_relay_request_set",
        "fr_relay_request_set_public",
        "fr_relay_public_channel_to_index",
        "fr_safety_supervisor_accepts_change",
        "fr_config_store_default",
        "fr_http_classify_route",
        "fr_api_validate_relay_payload",
        "fr_api_build_state_snapshot",
        "fr_api_assets_manifest_default",
        "fr_api_logs_recent_empty",
        "fr_storage_status_default",
        "fr_xbee_encode_api2",
        "fr_xbee_decode_api2",
        "fr_xbee_parse_at_response",
        "fr_xbee_parse_receive_packet",
    ], "safe core header"))

    host_test_runner = (
        root / "tests/four_relay_safe_core/run_host_tests.py"
    ).read_text(encoding="utf-8")
    host_test_text = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((root / "tests/four_relay_safe_core").glob("test_*.c"))
    )
    failures.extend(require_markers(host_test_text, [
        "hardware_gate_open",
        "public relay channel zero route rejects",
        "public relay channel 4 maps to internal index three",
        "GET /api/state response snapshot includes storage",
        "POST /api/all-off payload contract accepts sequence",
        "GET /api/assets/manifest response exposes file list",
        "GET /api/logs/recent response defaults empty",
        "XBee API2 encode succeeds in memory",
        "bad checksum rejects",
        "XBee truncated escape rejects",
        "AT response frame parses command",
        "receive-packet payload parses",
    ], "safe core host tests"))
    for marker in ["-Werror", "test_relay_safety", "safe_core"]:
        if marker not in host_test_runner:
            failures.append(f"safe core test runner missing marker: {marker}")
    return failures


def audit_firmware_forbidden_markers(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    scan_root = root / FIRMWARE_SOURCE_SCAN_ROOT.relative_to(ROOT)
    for source_file in sorted(scan_root.rglob("*")):
        if source_file.suffix not in {".c", ".h", ".txt"} and source_file.name != "CMakeLists.txt":
            continue
        text = source_file.read_text(encoding="utf-8")
        rel = source_file.relative_to(root).as_posix()
        for forbidden in FORBIDDEN_FIRMWARE_MARKERS:
            if forbidden == "uart_write_bytes" and rel == UART_BRIDGE_SOURCE:
                continue
            if forbidden == "gpio_config" and rel == UART_BRIDGE_SOURCE:
                continue
            if forbidden == "i2c_master_transmit" and rel == UART_BRIDGE_SOURCE:
                continue
            if forbidden in text:
                failures.append(f"firmware skeleton contains forbidden marker {forbidden}: {rel}")
    return failures


def audit_uart_bridge_boundary(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    bridge = (root / UART_BRIDGE_SOURCE).read_text(encoding="utf-8")
    failures.extend(require_markers(bridge, [
        "FR_BRIDGE_HOST_UART UART_NUM_0",
        "FR_BRIDGE_XBEE_UART UART_NUM_2",
        "FR_BRIDGE_HOST_BAUD 115200",
        "FR_BRIDGE_XBEE_BAUD 9600",
        "FR_BRIDGE_XBEE_TX_GPIO GPIO_NUM_17",
        "FR_BRIDGE_XBEE_RX_GPIO GPIO_NUM_16",
        "UART_HW_FLOWCTRL_DISABLE",
        "esp_log_level_set(\"*\", ESP_LOG_NONE)",
        "uart_write_bytes",
        "uart_read_bytes",
        "fr_relay_state_init",
        "fr_config_store_default",
        "fr_storage_status_default",
    ], "UART bridge firmware boundary"))
    for forbidden in [
        "\"WR\"",
        "\"AC\"",
        "\"KY\"",
        "esp_wifi_start",
        "esp_vfs_fat",
        "gpio_set_level",
    ]:
        if forbidden in bridge:
            failures.append(f"UART bridge source contains forbidden marker: {forbidden}")

    main_cmake = (
        root / "firmware/projects/four-relay-xbee-wifi/main/CMakeLists.txt"
    ).read_text(encoding="utf-8")
    failures.extend(require_markers(main_cmake, [
        "PRIV_REQUIRES safe_core esp_driver_uart esp_driver_i2c esp_driver_gpio esp_driver_pcnt",
    ], "UART bridge CMake boundary"))
    return failures


def audit_lcd_test_boundary(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    bridge = (
        (root / UART_BRIDGE_SOURCE).read_text(encoding="utf-8")
        + "\n"
        + (root / "firmware/projects/four-relay-xbee-wifi/main/bbs_lcd_menu_generated.h").read_text(encoding="utf-8")
    )
    failures.extend(require_markers(bridge, [
        "FR_LCD_I2C_PORT 0",
        "FR_LCD_I2C_SDA_GPIO GPIO_NUM_21",
        "FR_LCD_I2C_SCL_GPIO GPIO_NUM_22",
        "FR_LCD_I2C_SPEED_HZ 100000",
        "FR_LCD_TASK_PRIORITY tskIDLE_PRIORITY",
        ".flags.enable_internal_pullup = false",
        "i2c_master_probe",
        "fr_lcd_probe_range(lcd->bus, 0x20, 0x27",
        "fr_lcd_probe_range(lcd->bus, 0x38, 0x3f",
        "i2c_master_transmit",
        "FR_DIAG_XBEE_BRIDGE_CLOSED 1",
        "FR_DIAG_FIRMWARE_ID \"PF0530W\"",
        "FR_DIAG_FIRMWARE_ID_VALUE \"PF0530W\"",
        "FR_GLYPH_BANK_COUNT FR_BBS_GLYPH_BANK_COUNT",
        "FR_BBS_GLYPH_BANK_COUNT 7u",
        "FR_GLYPH_BANK_SWAP_MIN_MS 250",
        "FR_MENU_AUTO_CYCLE_ENABLED 0U",
        "FR_MENU_AUTO_CYCLE_MS 7000",
        "LCD_DIAG_READY gpio=21/22 speed=%d pullups=external",
        "LCD_BUS result=ok",
        "LCD_PROBE addr=0x%02x result=ack",
        "LCD_PROBE_SUMMARY count=%u selected=0x%02x",
        "LCD_DEVICE result=ok addr=0x%02x",
        "LCD_HD44780 step=%s result=%s err=%s",
        "LCD_INIT_OK addr=0x%02x",
        "LCD_INIT_FAIL stage=",
        "LCD_DIAG_HB status=%s count=%lu addr=0x%02x stage=%s devices=%u",
        "fr_lcd_bbs_menu_task",
        "BBS_LCD_READY gpio=13/14/32 pullups=on lcd=21/22 addr=0x%02x",
        "BBS_INPUT_READY task=split poll_ms=%u render=dirty idle_ms=%u ",
        "irq=pcnt queue=%u",
        "BBS_GLYPH_BANK name=%s index=%u slots=%u rows=%u",
        "BBS_CURSOR row=%u col=%u ddram=0x%02x",
        "BBS_LCD_RENDER page=%s index=%u row0=\\\"%s\\\" row1=\\\"%s\\\"",
        "rows=%u seq=%lu dur_ms=%lu reason=%s",
        "dirty_rows=0x%02x dirty_cells=%u",
        "BBS FIELD STATUS",
        "MESSAGES",
        "PEERS",
        "QUEUE",
        "FILES",
        "MESH",
        "XBEE",
        "DIAG",
        "LOCKS",
        "BARS LINK QUEUE",
        "VERT CHART HISTORY",
        "BIG DIGITS 12:34",
        "GAUGE STATUS",
        "fr_lcd_start_task",
    ], "LCD-only I2C firmware boundary"))
    if "xTaskCreate(\n        fr_lcd_diag_task" in bridge:
        failures.append("LCD task start still targets fr_lcd_diag_task")
    if "xTaskCreate(\n        fr_lcd_bbs_menu_task" not in bridge:
        failures.append("LCD task start does not target fr_lcd_bbs_menu_task")
    return failures


def audit_encoder_menu_boundary(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    bridge = (
        (root / UART_BRIDGE_SOURCE).read_text(encoding="utf-8")
        + "\n"
        + (root / "firmware/projects/four-relay-xbee-wifi/main/bbs_lcd_menu_generated.h").read_text(encoding="utf-8")
    )
    failures.extend(require_markers(bridge, [
        "#include \"driver/gpio.h\"",
        "FR_ENCODER_CLK_GPIO GPIO_NUM_13",
        "FR_ENCODER_DT_GPIO GPIO_NUM_14",
        "FR_ENCODER_SW_GPIO GPIO_NUM_32",
        "FR_ENCODER_TRANSITIONS_PER_STEP 1",
        "FR_ENCODER_SW_DEBOUNCE_MS 30",
        "FR_ENCODER_AB_STABLE_SAMPLES 1",
        "FR_ENCODER_AB_DEBOUNCE_MS 1",
        "FR_ENCODER_AB_QUIET_MS 0",
        "FR_ENCODER_DETENT_AB 3U",
        "FR_ENCODER_STEP_LOCKOUT_MS 25",
        "FR_ENCODER_SW_GUARD_MS 40",
        "FR_ENCODER_PCNT_LOW_LIMIT -32767",
        "FR_ENCODER_PCNT_HIGH_LIMIT 32767",
        "FR_ENCODER_PCNT_RECENTER_LIMIT 30000",
        "FR_ENCODER_PCNT_GLITCH_NS 1000",
        "FR_ENCODER_PCNT_COUNTS_PER_STEP 4",
        "FR_ENCODER_PCNT_MAX_STEPS_PER_POLL 4",
        "FR_ENCODER_PCNT_DIR_SIGN 1",
        "FR_ENCODER_LONG_PRESS_MS 650",
        "FR_ENCODER_RAW_HEARTBEAT_MS 1000",
        "FR_ENCODER_RAW_EVENT_LOG_ENABLED 0U",
        "FR_ENCODER_INTERRUPT_TELEMETRY 0U",
        "FR_ENCODER_DETENT_GATED 1U",
        "FR_GPIO_SWEEP_COUNT 3",
        "FR_MENU_POLL_MS 2",
        "FR_MENU_PAGE_COUNT FR_BBS_MENU_PAGE_COUNT",
        "FR_BBS_MENU_PAGE_COUNT 15u",
        "FR_MENU_HEARTBEAT_MS 2000",
        "FR_MENU_AUTO_CYCLE_ENABLED 0U",
        "FR_MENU_AUTO_CYCLE_MS 7000",
        "FR_MENU_ANIMATION_MS 2000",
        "FR_ENCODER_EVENT_QUEUE_DEPTH 0",
        "FR_ENCODER_IRQ_DRAIN_LIMIT 0",
        "FR_DIAG_FIRMWARE_ID \"PF0530W\"",
        "FR_DIAG_FIRMWARE_ID_VALUE \"PF0530W\"",
        "FR_DIAG_XBEE_BRIDGE_CLOSED 1",
        "FR_GLYPH_BANK_COUNT FR_BBS_GLYPH_BANK_COUNT",
        "FR_BBS_GLYPH_BANK_COUNT 7u",
        "FR_GLYPH_SLOTS 8",
        "FR_GLYPH_ROWS 8",
        "FR_GLYPH_BANK_SWAP_MIN_MS 250",
        "FR_MENU_RENDER_POLL_MS 20",
        "FR_MENU_IDLE_REFRESH_MS 60000",
        "FR_MENU_INPUT_TASK_PRIORITY (tskIDLE_PRIORITY + 1)",
        "FR_MENU_MODE_PAGE_BROWSE",
        "FR_MENU_MODE_ROW_BROWSE",
        "FR_MENU_MODE_DETAIL",
        "FR_MENU_MODE_EDIT_LAB",
        "fr_delay_ticks_at_least_one",
        "fr_encoder_irq_count",
        "fr_encoder_pin_mask",
        "gpio_config(&config)",
        "gpio_dump_io_configuration(stdout, fr_encoder_pin_mask())",
        ".mode = GPIO_MODE_INPUT",
        ".pull_up_en = enable_pullup ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE",
        ".pull_down_en = GPIO_PULLDOWN_DISABLE",
        ".intr_type = FR_ENCODER_INTERRUPT_TELEMETRY != 0U",
        ": GPIO_INTR_DISABLE",
        "BBS_LCD_READY gpio=13/14/32 pullups=on lcd=21/22 addr=0x%02x",
        "BBS_INPUT_READY task=split poll_ms=%u render=dirty idle_ms=%u ",
        "irq=pcnt queue=%u",
        "modes=scroll,detail,edit actions=page,detail,edit,back",
        "step=%u stable=%u sw_debounce_ms=%u sw_guard_ms=%u long_ms=%u drain=%u",
        "cal=pcnt-v1",
        "decoder=pcnt sw=poll pcnt=1 counts_per_step=%u",
        "max_steps=%u glitch_ns=%u dir=%d recenter=%d",
        "ab_ms=%u quiet_ms=%u step_lockout_ms=%u",
        "detent=%u detent_gate=%u raw_hb_ms=%u gpio_cfg=1 poll_raw=1 ",
        "raw_log=%u poll_decoder=0 source=%s",
        "ENC_BASE levels=C%uD%uS%u raw_ab=%u pcnt_count=%ld t=%lu",
        "ENC_GPIO_CONFIG pin=%d label=%s mode=input pullup=%s",
        "ENC_PCNT_READY result=ok",
        "ENC_LEVEL_HB levels=C%uD%uS%u raw_ab=%u stable=C%uD%u",
        "ENC_PCNT_HB count=%ld delta=%ld residual=%ld steps=%lu",
        "FR_BBS_MENU_XML_SCHEMA",
        "FR_BBS_MENU_RENDER_SCHEMA",
        "FR_BBS_MENU_MARQUEE_HOLD_MS 750u",
        "FR_BBS_MENU_MARQUEE_STEP_MS 250u",
        "fr_bbs_generated_pages",
        "fr_bbs_sync_menu_view",
        "fr_bbs_render_generated_frame",
        ".name = \"table\"",
        "NODE |RSSI|Q",
        "ROUTES",
        "ENC_RAW kind=%s levels=C%uD%uS%u raw_ab=%lu raw_sw=%lu ",
        "gap_ms=%lu burst=%lu t=%lu",
        "ENC_EV pin=%d label=%s level=%u count=%lu t=%lu",
        "BBS_MENU_STEP dir=%c page=%s index=%u pos=%ld cw=%lu ccw=%lu t=%lu",
        "BBS_MENU_SELECT buttons=%lu page=%s index=%u kind=short held_ms=%lu t=%lu",
        "BBS_MENU_SELECT buttons=%lu page=%s index=%u kind=long held_ms=%lu t=%lu",
        "AB_SUPPRESS raw=%u levels=C%uD%uS%u suppressed=%lu t=%lu",
        "AB_INVALID prev=%u curr=%u invalid=%lu t=%lu",
        "ENC_FILTER reason=%s prev=%u curr=%u levels=C%uD%uS%u",
        "\"ab_debounce\"",
        "\"ab_quiet\"",
        "\"step_lockout\"",
        "\"detent_partial\"",
        "\"invalid\"",
        "\"sw_guard\"",
        "BBS_MENU_HB page=%s index=%u pos=%ld levels=C%uD%uS%u steps=%lu/%lu",
        "ab_hold=%lu ab_quiet=%lu stable_ab=%lu detent=%lu detent_step=%lu partial=%lu",
        "step_lockout=%lu invalid=%lu suppressed=%lu",
        "raw_burst=%lu gap_ms=%lu",
        "BBS_MENU_AUTO_CYCLE page=%s index=%u interval_ms=%u t=%lu",
        "irq_drop=%lu",
        "queue_drop=%lu",
        "raw_levels=C%uD%uS%u raw_ab=%u candidate=C%uD%u",
        "isr=C%luD%luS%lu queue_rx=%lu poll=%lu",
        "BBS_GLYPH_BANK name=%s index=%u slots=%u rows=%u",
        ".name = \"core_status\"",
        ".name = \"horizontal_bar\"",
        ".name = \"vertical_chart\"",
        ".name = \"big_digits\"",
        ".name = \"gauge\"",
        ".name = \"art_panel\"",
        "FR_BBS_ART_GLYPH_BANK_INDEX 6U",
        "FR_BBS_ART_PANEL_COUNT 5U",
        "fr_bbs_art_panels",
        "art_panel_index",
        "\"ART NEXT\"",
        "\"ART PREV\"",
        "ART Pixel Gallery",
        "ART",
        "fr_lcd_load_glyph_bank",
        "BBS_CURSOR row=%u col=%u ddram=0x%02x focus=%s mode=%s",
        "BBS_LCD_RENDER page=%s index=%u row0=\\\"%s\\\" row1=\\\"%s\\\"",
        "rows=%u seq=%lu dur_ms=%lu reason=%s",
        "art=%u",
        "dirty_rows=0x%02x dirty_cells=%u",
        "fr_menu_input_task",
        "fr_lcd_render_cache_t",
        "render_cache.valid",
        "runtime.lcd_dirty",
        "#include \"driver/pulse_cnt.h\"",
        "pcnt_new_unit",
        "pcnt_unit_set_glitch_filter",
        "pcnt_new_channel",
        "pcnt_channel_set_edge_action",
        "pcnt_channel_set_level_action",
        "pcnt_unit_enable",
        "pcnt_unit_clear_count",
        "pcnt_unit_start",
        "pcnt_unit_get_count",
        "fr_encoder_pcnt_accumulate_steps",
        "fr_menu_process_pcnt",
        "fr_menu_resync_pcnt",
        "FR_ENCODER_DETENT_AB",
        "raw_ab_transition_count",
        "raw_sw_transition_count",
        "poll_count",
        "queue_receive_count",
        "signal_change_count",
        "cw_step_count",
        "ccw_step_count",
        "pcnt_last_count",
        "pcnt_last_delta",
        "pcnt_residual",
        "pcnt_step_count",
        "pcnt_capped_poll_count",
        "pcnt_direction_flip_count",
        "pcnt_guard_reset_count",
        "pcnt_suppressed_delta_count",
        "pcnt_recenter_count",
        "ab_debounce_hold_count",
        "ab_quiet_hold_count",
        "accepted_stable_ab_transition_count",
        "detent_return_count",
        "detent_step_count",
        "detent_partial_count",
        "step_lockout_count",
        "raw_ab_burst_count",
        "last_raw_ab_gap_ms",
        "last_ab_held_ms",
        "last_ab_quiet_ms",
        "invalid_transition_count",
        "suppressed_transition_count",
        "switch_guard_until_ms",
        "candidate_changed_ms_a",
        "candidate_changed_ms_b",
        "last_step_ms",
        "last_heartbeat_ms",
        "last_level_heartbeat_ms",
        "{FR_ENCODER_CLK_GPIO, \"CLK\", true}",
        "{FR_ENCODER_DT_GPIO, \"DT\", true}",
        "{FR_ENCODER_SW_GPIO, \"SW\", true}",
        "fr_gpio_sweep_pins[index].enable_pullup",
        "fr_menu_sample_ab_pair",
        "fr_menu_ab_a",
        "fr_menu_ab_b",
        "fr_menu_raw_ab",
        "fr_menu_process_levels",
        "fr_menu_handle_switch_stable",
        "fr_menu_handle_long_press",
        "fr_menu_sample_inputs",
        "fr_diag_short_display_count",
        "fr_diag_short_position_magnitude",
        "fr_bbs_page_name",
        "BBS FIELD STATUS",
        "fr_menu_level_char",
        "MSG NEW:01 IN:12",
        "PEERS ACTIVE 2/3",
        "QUEUE P:2 F:0",
        "FILES Q:1 D:3",
        "MESH runtime",
        "BRIDGE LOCAL CLOSED",
        "DIAG ERRORS:0",
        "Flash:LOCK Ser:LOCK",
        "BARS LINK QUEUE",
        "VERT CHART HISTORY",
        "BIG DIGITS 12:34",
        "GAUGE STATUS",
    ], "encoder LCD menu firmware boundary"))
    for forbidden in [
        "FR_DIAG_SERIAL_PINTRACE 1",
        "SERIAL_PINTRACE READY",
        "fr_serial_pintrace_start_task",
        "fr_serial_pintrace_run();",
        "    menu->page = 0;\n    if (menu->ack",
        "gpio_pullup_en(pins[index])",
        "FR_ENCODER_CLK_GPIO GPIO_NUM_34",
        "FR_ENCODER_DT_GPIO GPIO_NUM_35",
        "FR_ENCODER_SW_GPIO GPIO_NUM_13",
        "{FR_GPIO_SWEEP_GPIO32, \"32\", false}",
        "FR_GPIO_SWEEP_GPIO16",
        "FR_GPIO_SWEEP_GPIO17",
        "FR_GPIO_SWEEP_GPIO21",
        "FR_GPIO_SWEEP_GPIO22",
        "FR_GPIO_SWEEP_GPIO25",
        "FR_GPIO_SWEEP_GPIO26",
        "FR_GPIO_SWEEP_GPIO27",
        "FR_GPIO_SWEEP_GPIO33",
        "gpio_set_level",
        "esp_partition_erase",
        "esp_wifi_start",
        "httpd_start",
        "esp_vfs_fat",
    ]:
        if forbidden in bridge:
            failures.append(f"encoder menu source contains forbidden marker: {forbidden}")
    return failures


def audit_espnow_bbs_build_only_target(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    project = root / "firmware/projects/espnow-bbs"
    readme = (project / "README.md").read_text(encoding="utf-8")
    top_cmake = (project / "CMakeLists.txt").read_text(encoding="utf-8")
    main_cmake = (project / "main/CMakeLists.txt").read_text(encoding="utf-8")
    main_c = (project / "main/main.c").read_text(encoding="utf-8")

    failures.extend(require_markers(readme, [
        "## Verified facts",
        "## Assumptions",
        "## Unknowns",
        "## Hard gates",
        "build-only ESP-IDF target",
        "No flash, erase, monitor, serial write, live mesh, BLE, Android, router/admin,",
        "deploy authority is opened by this target.",
        "SRC-ESP-IDF-STABLE-ESP32",
        "SRC-ESP-IDF-GET-STARTED",
        "SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS",
        "SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05",
    ], "ESP-NOW BBS firmware README"))
    failures.extend(require_markers(top_cmake, [
        "cmake_minimum_required(VERSION 3.16)",
        "include($ENV{IDF_PATH}/tools/cmake/project.cmake)",
        "project(espnow_bbs_build_only)",
    ], "ESP-NOW BBS top-level CMake"))
    failures.extend(require_markers(main_cmake, [
        "idf_component_register(",
        "SRCS \"main.c\"",
        "PRIV_REQUIRES mesh_discovery_contract mesh_retained_persistence mesh_startup_lifecycle",
    ], "ESP-NOW BBS main CMake"))
    failures.extend(require_markers(main_c, [
        "#include \"espnow_bbs_mesh_discovery_contract.h\"",
        "#include \"espnow_bbs_mesh_retained_persistence.h\"",
        "#include \"espnow_bbs_mesh_startup_lifecycle.h\"",
        "void app_main(void)",
        "Gate M4O wires accepted M4N startup lifecycle source into app_main",
        "Gate M4W adds source-only retained recovery",
        "Gate M4X adds source-only local",
        "Runtime/live behavior remains unaccepted until",
        "s_mesh_app_lifecycle_status = espnow_bbs_mesh_app_start();",
    ], "ESP-NOW BBS build-only main"))
    return failures


def audit_espnow_bbs_mesh_discovery_contract(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    project = root / "firmware/projects/espnow-bbs"
    component = project / "components/mesh_discovery_contract"
    component_cmake = (component / "CMakeLists.txt").read_text(encoding="utf-8")
    header = (
        component / "include/espnow_bbs_mesh_discovery_contract.h"
    ).read_text(encoding="utf-8")
    source = (
        component / "src/espnow_bbs_mesh_discovery_contract.c"
    ).read_text(encoding="utf-8")
    host_runner = (
        root / "tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py"
    ).read_text(encoding="utf-8")
    host_tests = (
        root / "tests/espnow_bbs_mesh_discovery_contract/test_mesh_discovery_contract.c"
    ).read_text(encoding="utf-8")

    failures.extend(require_markers(component_cmake, [
        "idf_component_register(",
        "\"src/espnow_bbs_mesh_discovery_contract.c\"",
        "INCLUDE_DIRS \"include\"",
    ], "ESP-NOW BBS mesh discovery contract CMake"))
    failures.extend(require_markers(header, [
        "ESPNOW_BBS_MESH_DISCOVERY_SCHEMA \"mesh_discovery.v1\"",
        "ESPNOW_BBS_MESH_DISCOVERY_TRANSPORT \"esp-wifi-mesh\"",
        "ESPNOW_BBS_MESH_DISCOVERY_MODE \"sim\"",
        "ESPNOW_BBS_MESH_DISCOVERY_ADMIN_GATE \"disabled\"",
        "ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES 512u",
        "ESPNOW_BBS_MESH_CONTRACT_EVENT_QUEUE_CAPACITY 16u",
        "ESPNOW_BBS_MESH_CONTRACT_NODE_ID_MAX_BYTES 8u",
        "ESPNOW_BBS_MESH_CONTRACT_DETAIL_MAX_BYTES 32u",
        "ESPNOW_BBS_MESH_CONTRACT_OWNER_NAME_MAX_BYTES 16u",
        "ESPNOW_BBS_MESH_CONTRACT_DISPATCH_PLAN_CAPACITY 8u",
        "ESPNOW_BBS_MESH_CONTRACT_DISPATCH_ACTION_NAME_MAX_BYTES 16u",
        "ESPNOW_BBS_MESH_CONTRACT_DISPATCH_BUDGET_MAX 16u",
        "ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_PROFILE_NAME_MAX_BYTES 16u",
        "ESPNOW_BBS_MESH_CONTRACT_SCHEDULER_TICK_MAX 64u",
        "ESPNOW_BBS_MESH_CONTRACT_EXECUTION_CYCLE_MAX 64u",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_CYCLE_MAX 64u",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_POLICY_NAME_MAX_BYTES 16u",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_PERIOD_MAX 64u",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_CORE_NONE (-1)",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY \"caller_invoked\"",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY \"task_none\"",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY \"timer_none\"",
        "espnow_bbs_mesh_contract_event_count",
        "espnow_bbs_mesh_contract_event_queue_push",
        "espnow_bbs_mesh_contract_event_queue_peek",
        "espnow_bbs_mesh_contract_event_queue_pop",
        "espnow_bbs_mesh_contract_event_queue_summary",
        "espnow_bbs_mesh_contract_owner_claim",
        "espnow_bbs_mesh_contract_owner_release",
        "espnow_bbs_mesh_contract_owner_ingest",
        "espnow_bbs_mesh_contract_owner_drain",
        "espnow_bbs_mesh_contract_owner_summary",
        "espnow_bbs_mesh_contract_dispatch_action_count",
        "espnow_bbs_mesh_contract_dispatch_plan_stage",
        "espnow_bbs_mesh_contract_dispatch_plan_from_owner",
        "espnow_bbs_mesh_contract_dispatch_plan_summary",
        "espnow_bbs_mesh_contract_scheduler_profile_count",
        "espnow_bbs_mesh_contract_scheduler_stage_tick",
        "espnow_bbs_mesh_contract_scheduler_state_summary",
        "espnow_bbs_mesh_contract_execution_accounting_apply_plan",
        "espnow_bbs_mesh_contract_execution_accounting_summary",
        "espnow_bbs_mesh_contract_runtime_cycle_step",
        "espnow_bbs_mesh_contract_runtime_cycle_summary",
        "espnow_bbs_mesh_contract_runtime_scheduler_config_status",
        "espnow_bbs_mesh_contract_runtime_scheduler_configure",
        "espnow_bbs_mesh_contract_runtime_scheduler_step",
        "espnow_bbs_mesh_contract_runtime_scheduler_summary",
        "espnow_bbs_mesh_contract_bridge_summary_state_t",
        "espnow_bbs_mesh_contract_bridge_request_router_t",
        "espnow_bbs_mesh_contract_bridge_summary",
        "espnow_bbs_mesh_contract_bridge_request_route",
        "espnow_bbs_mesh_contract_bridge_request_router_summary",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_t",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_route",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_t",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_route",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_summary",
        "espnow_bbs_mesh_contract_persistence_intent_t",
        "espnow_bbs_mesh_contract_persistence_intent_stage",
        "espnow_bbs_mesh_contract_persistence_intent_summary",
        "espnow_bbs_mesh_contract_request_count",
        "espnow_bbs_mesh_contract_gate_f_service_code",
        "espnow_bbs_mesh_contract_field_path_status",
        "espnow_bbs_mesh_contract_live_action_status",
        "espnow_bbs_mesh_contract_summary_status",
    ], "ESP-NOW BBS mesh discovery contract header"))
    failures.extend(require_markers(source, [
        "{\"node_seen\", \"ns\"}",
        "{\"node_lost\", \"nl\"}",
        "{\"parent_selected\", \"ps\"}",
        "{\"root_elected\", \"re\"}",
        "{\"root_switched\", \"rs\"}",
        "{\"route_added\", \"ra\"}",
        "{\"route_removed\", \"rr\"}",
        "{\"heal_started\", \"hs\"}",
        "{\"heal_observed\", \"ho\"}",
        "{\"service_seen\", \"ss\"}",
        "{\"capability_seen\", \"cs\"}",
        "{\"ble_client_seen\", \"bs\"}",
        "\"discovery_snapshot\"",
        "\"discovery_events\"",
        "\"service_catalog\"",
        "\"capability_report\"",
        "{\"direct_message\", 1}",
        "{\"file_chunk\", 2}",
        "{\"telemetry_report\", 3}",
        "{\"node_status\", 4}",
        "{\"custody_ack\", 5}",
        "{\"control_intent\", 6}",
        "\"mesh_start\"",
        "\"serial_write\"",
        "\"xbee_write\"",
        "normalized_secret_match",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_LINE_TOO_LONG",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_EMPTY",
        "format_queue_summary",
        "append_size_value",
        "queue->dropped",
        "\"mesh_ingress\"",
        "\"mesh_drain\"",
        "\"mesh_snapshot\"",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_OWNER_STALE",
        "format_owner_summary",
        "owner->accepted",
        "owner->rejected",
        "owner->drained",
        "{\"owner_drain\", \"od\"}",
        "{\"owner_ingest\", \"oi\"}",
        "{\"owner_snapshot\", \"os\"}",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_DISPATCH_PLAN_FULL",
        "format_dispatch_plan_summary",
        "plan->requested",
        "plan->planned",
        "plan->rejected",
        "{\"balanced\", 2u, 2u, 1u}",
        "{\"drain_first\", 1u, 4u, 1u}",
        "{\"snapshot_only\", 0u, 0u, 1u}",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_SCHEDULER_TICK_TOO_LARGE",
        "format_scheduler_state_summary",
        "state->tick_count",
        "state->requested",
        "state->planned",
        "state->rejected",
        "format_execution_accounting_summary",
        "accounting->applied",
        "accounting->skipped",
        "accounting->drain_ready",
        "accounting->ingest_ready",
        "accounting->snapshot_ready",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_EXECUTION_STEP_NOT_READY",
        "format_runtime_cycle_summary",
        "runtime->scheduled",
        "runtime->applied",
        "runtime->skipped",
        "runtime->rejected",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_CYCLE_TOO_LARGE",
        "espnow_bbs_mesh_contract_runtime_cycle_step",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_INVOKE_POLICY",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TASK_POLICY",
        "ESPNOW_BBS_MESH_CONTRACT_RUNTIME_SCHEDULER_TIMER_POLICY",
        "runtime_scheduler_config_status",
        "format_runtime_scheduler_summary",
        "runtime_scheduler->step_count",
        "runtime_scheduler->yield_due",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_RUNTIME_OPEN",
        "ESPNOW_BBS_MESH_CONTRACT_REJECT_RUNTIME_SCHEDULER_NOT_CONFIGURED",
        "espnow_bbs_mesh_contract_runtime_scheduler_step",
        "format_bridge_snapshot_summary",
        "format_bridge_events_summary",
        "format_bridge_service_catalog_summary",
        "format_bridge_capability_summary",
        "format_bridge_request_router_summary",
        "format_bridge_dispatch_envelope_summary",
        "espnow_bbs_mesh_contract_bridge_summary",
        "espnow_bbs_mesh_contract_bridge_request_router_init",
        "espnow_bbs_mesh_contract_bridge_request_route",
        "espnow_bbs_mesh_contract_bridge_request_router_summary",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_init",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_route",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary",
        "normalize_bridge_request_text_intent",
        "format_bridge_request_text_intent_summary",
        "format_persistence_intent_summary",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_init",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_route",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_summary",
        "espnow_bbs_mesh_contract_persistence_intent_init",
        "espnow_bbs_mesh_contract_persistence_intent_stage",
        "espnow_bbs_mesh_contract_persistence_intent_summary",
        "req=discovery_snapshot",
        "req=discovery_events",
        "req=service_catalog",
        "req=capability_report",
        "router=local_request",
        "router->attempted",
        "router->accepted",
        "router->rejected",
        "envelope=local_dispatch",
        "envelope->attempted",
        "envelope->would_dispatch",
        "envelope->rejected",
        "intent=request_text",
        "intent->attempted",
        "intent->accepted",
        "intent->rejected",
        "persistence=intent",
        "persistence->attempted",
        "persistence->staged",
        "persistence->rejected",
        "storage=volatile",
        "nvs=closed",
        "file=closed",
        "abi=closed",
        "serial=closed",
        "bridge=disjoint",
        "live=blocked",
    ], "ESP-NOW BBS mesh discovery contract source"))
    failures.extend(require_markers(host_runner, [
        "-Werror",
        "espnow_bbs_mesh_discovery_contract.c",
        "test_mesh_discovery_contract.c",
    ], "ESP-NOW BBS mesh discovery contract host runner"))
    failures.extend(require_markers(host_tests, [
        "schema is mesh_discovery.v1",
        "512-byte summary fits",
        "513-byte summary rejects",
        "non-ASCII summary rejects",
        "unknown event rejects",
        "caller queue initializes empty",
        "full queue rejects with backpressure",
        "queue reuses wrapped slot after pop",
        "non-ASCII node rejects at queue ingress",
        "over-limit detail rejects at queue ingress",
        "queue summary rejects small caller capacity",
        "owner role count is stable",
        "unbound owner claim rejects",
        "double owner claim rejects",
        "owner accepted counter increments",
        "owner surfaces caller queue full backpressure",
        "owner summary rejects small caller capacity",
        "dispatch action count is stable",
        "dispatch zero budget rejects",
        "dispatch plan full rejects",
        "dispatch plan builds from owner",
        "dispatch planning does not drain caller queue",
        "dispatch full-ingest planning does not increment drop count",
        "dispatch full-ingest summary is stable",
        "dispatch summary rejects small caller capacity",
        "scheduler profile count is stable",
        "scheduler balanced tick stages dispatch plan",
        "scheduler tick does not drain caller queue",
        "scheduler tick records ingest backpressure",
        "scheduler summary rejects small caller capacity",
        "execution accounting applies balanced plan",
        "execution accounting does not drain caller queue",
        "execution accounting partial backpressure plan accepts",
        "execution accounting summary rejects small caller capacity",
        "runtime cycle balanced step accepts",
        "runtime cycle does not drain caller queue",
        "runtime cycle zero cycle rejects",
        "runtime cycle partial backpressure accepts",
        "runtime cycle summary rejects small caller capacity",
        "runtime scheduler config accepts caller-invoked policy",
        "runtime scheduler caller step accepts",
        "runtime scheduler caller step does not drain caller queue",
        "runtime scheduler rejected configure does not mutate state",
        "runtime scheduler rejected step does not mutate state",
        "runtime scheduler summary rejects small caller capacity",
        "discovery request stays disjoint from Gate F bridge requests",
        "known bridge summary request accepts",
        "bridge summary rejects small caller capacity",
        "bridge summary does not step runtime scheduler",
        "known bridge request router route accepts",
        "bridge request router tracks attempts",
        "bridge request router unknown request rejects",
        "bridge request router small output rejects",
        "bridge request router does not step runtime scheduler",
        "bridge request router reset clears attempts",
        "known bridge dispatch envelope route accepts",
        "bridge dispatch envelope uses request router",
        "bridge dispatch envelope unknown request rejects",
        "bridge dispatch envelope small output rejects",
        "bridge dispatch envelope does not step runtime scheduler",
        "bridge dispatch envelope reset clears attempts",
        "known bridge request text intent route accepts",
        "malformed bridge request text intent rejects before envelope",
        "invalid request text intent does not call envelope",
        "bridge request text intent small output rejects through local route",
        "bridge request text intent does not step runtime scheduler",
        "bridge request text intent reset clears attempts",
        "known persistence intent stage accepts",
        "malformed persistence intent rejects through request text intent",
        "persistence intent small output rejects through M4S route",
        "persistence intent does not step runtime scheduler",
        "persistence intent reset clears attempts",
        "mesh_discovery is not a Gate F service",
        "configured secret marker rejects",
        "nested secret field rejects",
        "blocked live action rejects",
    ], "ESP-NOW BBS mesh discovery contract host tests"))

    forbidden_markers = [
        "malloc",
        "calloc",
        "realloc",
        "free",
        "strdup",
        "xTaskCreate",
        "xQueue",
        "xTimer",
        "StaticTask_t",
        "StackType_t",
        "StaticQueue_t",
        "StaticSemaphore_t",
        "TaskHandle",
        "QueueHandle",
        "TimerHandle",
        "SemaphoreHandle",
        "EventGroupHandle",
        "vTaskDelay",
        "esp_timer",
        "esp_event_handler",
        "(*callback",
        "(*hook",
        "callback",
        "hook",
        "handler",
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "nimble",
        "ble_gap",
        "ble_gatt",
        "nvs_",
        "uart_",
        "gpio_",
        "httpd_",
        "esp_partition",
        "esp_flash",
        "esp_ota",
        "FILE *",
        "fopen",
        "fwrite",
        "snprintf",
        "fprintf",
        "open(",
        "write(",
        "printf",
        "ESP_LOG",
        "esp_wifi_start",
        "esp_now_init",
        "nvs_flash_init",
        "uart_write_bytes",
        "uart_read_bytes",
        "gpio_set_level",
        "gpio_config",
        "httpd_start",
        "esp_partition_erase",
    ]
    allowed_task_shell_markers = {
        (
            "firmware/projects/espnow-bbs/main/main.c"
        ): {"StaticTask_t", "StackType_t"},
        (
            "firmware/projects/espnow-bbs/components/mesh_runtime_task_shell/"
            "include/espnow_bbs_mesh_runtime_task_shell.h"
        ): {"free", "StaticTask_t", "StackType_t", "TaskHandle"},
        (
            "firmware/projects/espnow-bbs/components/mesh_runtime_task_shell/"
            "src/espnow_bbs_mesh_runtime_task_shell.c"
        ): {"free", "xTaskCreate", "TaskHandle"},
        (
            "firmware/projects/espnow-bbs/components/mesh_runtime_task_shell/"
            "CMakeLists.txt"
        ): {"free"},
        (
            "firmware/projects/espnow-bbs/components/mesh_retained_persistence/"
            "CMakeLists.txt"
        ): {"nvs_"},
        (
            "firmware/projects/espnow-bbs/components/mesh_retained_persistence/"
            "src/espnow_bbs_mesh_retained_persistence_nvs.c"
        ): {"nvs_", "nvs_flash_init"},
    }
    for source_file in sorted(project.rglob("*")):
        if source_file.suffix not in {".c", ".h", ".txt"} and source_file.name != "CMakeLists.txt":
            continue
        text = source_file.read_text(encoding="utf-8")
        rel = source_file.relative_to(root).as_posix()
        for forbidden in forbidden_markers:
            if forbidden in text:
                if forbidden in allowed_task_shell_markers.get(rel, set()):
                    continue
                failures.append(
                    "ESP-NOW BBS firmware contract target contains forbidden "
                    f"marker {forbidden}: {rel}"
                )
    return failures


def audit_espnow_bbs_mesh_retained_persistence(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    component = (
        root / "firmware" / "projects" / "espnow-bbs" /
        "components" / "mesh_retained_persistence"
    )
    cmake = component / "CMakeLists.txt"
    header = component / "include" / "espnow_bbs_mesh_retained_persistence.h"
    codec = component / "src" / "espnow_bbs_mesh_retained_persistence_codec.c"
    nvs_source = component / "src" / "espnow_bbs_mesh_retained_persistence_nvs.c"
    host_runner = root / "tests" / "espnow_bbs_mesh_retained_persistence" / "run_host_tests.py"
    host_tests = root / "tests" / "espnow_bbs_mesh_retained_persistence" / "test_mesh_retained_persistence.c"
    required_files = [cmake, header, codec, nvs_source, host_runner, host_tests]
    for required_file in required_files:
        if not required_file.exists():
            failures.append(
                "missing ESP-NOW BBS retained persistence file: "
                f"{required_file.relative_to(root)}"
            )
    if failures:
        return failures

    cmake_text = cmake.read_text(encoding="utf-8")
    header_text = header.read_text(encoding="utf-8")
    codec_text = codec.read_text(encoding="utf-8")
    nvs_text = nvs_source.read_text(encoding="utf-8")
    host_runner_text = host_runner.read_text(encoding="utf-8")
    host_tests_text = host_tests.read_text(encoding="utf-8")
    codec_boundary = "\n".join([header_text, codec_text])
    component_text = "\n".join([cmake_text, header_text, codec_text, nvs_text])

    failures.extend(require_markers(cmake_text, [
        "idf_component_register(",
        "\"src/espnow_bbs_mesh_retained_persistence_codec.c\"",
        "\"src/espnow_bbs_mesh_retained_persistence_nvs.c\"",
        "REQUIRES mesh_discovery_contract",
        "PRIV_REQUIRES nvs_flash",
    ], "ESP-NOW BBS retained persistence CMake"))
    failures.extend(require_markers(header_text, [
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_STORAGE_OWNER \"mesh_ingress\"",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_NAMESPACE \"mesh_retained\"",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_KEY \"m4t_checkpoint\"",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_WRITE_POLICY \"caller_triggered\"",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_VERSION 1u",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_RECORD_BYTES 96u",
        "espnow_bbs_mesh_retained_persistence_checkpoint_from_intent",
        "espnow_bbs_mesh_retained_persistence_checkpoint_apply",
        "espnow_bbs_mesh_retained_persistence_encode",
        "espnow_bbs_mesh_retained_persistence_decode",
        "espnow_bbs_mesh_retained_persistence_recover",
        "espnow_bbs_mesh_retained_persistence_summary",
        "espnow_bbs_mesh_retained_persistence_storage_init",
        "espnow_bbs_mesh_retained_persistence_storage_deinit",
        "espnow_bbs_mesh_retained_persistence_storage_load",
        "espnow_bbs_mesh_retained_persistence_storage_save",
        "espnow_bbs_mesh_retained_persistence_storage_erase",
    ], "ESP-NOW BBS retained persistence header"))
    failures.extend(require_markers(codec_text, [
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_MAGIC",
        "ESPNOW_BBS_MESH_DISCOVERY_SCHEMA",
        "retained_validate_checkpoint",
        "retained_validate_request",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_SECRET_FIELD",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_CHECKSUM",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_VERSION",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_COUNTER_RANGE",
        "espnow_bbs_mesh_contract_persistence_intent_init(intent)",
        "espnow_bbs_mesh_contract_field_name_status",
        "espnow_bbs_mesh_contract_request_status",
        "storage=nvs",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_WRITE_POLICY",
        "reboot=unproved",
        "live=blocked",
        "serial=closed",
        "abi=closed",
    ], "ESP-NOW BBS retained persistence codec"))
    failures.extend(require_markers(nvs_text, [
        "nvs_flash_init",
        "nvs_flash_deinit",
        "nvs_open",
        "NVS_READONLY",
        "NVS_READWRITE",
        "nvs_get_blob",
        "nvs_set_blob",
        "nvs_erase_key",
        "nvs_commit",
        "nvs_close",
        "espnow_bbs_mesh_retained_persistence_storage_init",
        "espnow_bbs_mesh_retained_persistence_storage_deinit",
        "espnow_bbs_mesh_retained_persistence_storage_load",
        "espnow_bbs_mesh_retained_persistence_storage_save",
        "espnow_bbs_mesh_retained_persistence_storage_erase",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_HOST_TEST",
        "espnow_bbs_mesh_contract_persistence_intent_init(intent)",
    ], "ESP-NOW BBS retained persistence NVS adapter"))
    failures.extend(require_markers(host_runner_text, [
        "-Werror",
        "-DESPNOW_BBS_MESH_RETAINED_PERSISTENCE_HOST_TEST",
        "espnow_bbs_mesh_retained_persistence_codec.c",
        "espnow_bbs_mesh_retained_persistence_nvs.c",
        "test_mesh_retained_persistence.c",
    ], "ESP-NOW BBS retained persistence host runner"))
    failures.extend(require_markers(host_tests_text, [
        "retained checkpoint builds from M4T intent",
        "retained checkpoint encodes",
        "retained record carries schema identity",
        "retained record carries last normalized request",
        "retained record does not retain generated summary payload",
        "retained record does not retain serial bytes",
        "retained recover does not mutate queue",
        "retained recover does not mutate owner",
        "retained recover does not step runtime scheduler",
        "retained summary preserves closed surfaces",
        "missing retained record recovers as volatile empty",
        "corrupt retained record rejects by checksum",
        "unsupported retained record version rejects",
        "retained checkpoint rejects unknown request",
        "retained checkpoint rejects secret-like request field",
        "retained checkpoint rejects non-ASCII request",
        "retained checkpoint rejects impossible counters",
        "retained checkpoint rejects counters outside record range",
        "retained storage rejects load before init",
        "retained storage load reports missing record",
        "retained storage save commits caller-triggered checkpoint",
        "retained storage erase accepts existing checkpoint",
        "retained storage deinit accepts",
    ], "ESP-NOW BBS retained persistence host tests"))

    codec_forbidden_markers = [
        "nvs_",
        "nvs_flash_init",
        "esp_partition",
        "esp_flash",
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "uart_",
        "gpio_",
        "httpd_",
        "FILE *",
        "fopen",
        "fwrite",
        "snprintf",
        "fprintf",
        "open(",
        "write(",
        "printf",
        "ESP_LOG",
        "xTaskCreate",
        "xQueue",
        "xTimer",
        "esp_timer",
    ]
    for marker in codec_forbidden_markers:
        if marker in codec_boundary:
            failures.append(
                "ESP-NOW BBS retained persistence codec/header contains "
                f"forbidden marker: {marker}"
            )

    nvs_forbidden_markers = [
        "esp_partition",
        "esp_flash",
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "uart_",
        "gpio_",
        "httpd_",
        "FILE *",
        "fopen",
        "fwrite",
        "snprintf",
        "fprintf",
        "open(",
        "write(",
        "printf",
        "ESP_LOG",
        "xTaskCreate",
        "xQueue",
        "xTimer",
        "esp_timer",
    ]
    for marker in nvs_forbidden_markers:
        if marker in nvs_text:
            failures.append(
                "ESP-NOW BBS retained persistence NVS adapter contains "
                f"forbidden marker: {marker}"
            )

    closed_markers = [
        "reboot=proved",
        "runtime=persistent",
        "live=accepted",
        "serial=open",
        "abi=open",
        "bridge ABI change",
        "Gate F service",
    ]
    for marker in closed_markers:
        if marker in component_text or marker in host_tests_text:
            failures.append(
                "ESP-NOW BBS retained persistence opens closed marker: "
                f"{marker}"
            )
    return failures


def audit_espnow_bbs_mesh_runtime_task_shell(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    component = (
        root / "firmware" / "projects" / "espnow-bbs" /
        "components" / "mesh_runtime_task_shell"
    )
    cmake = component / "CMakeLists.txt"
    header = component / "include" / "espnow_bbs_mesh_runtime_task_shell.h"
    source = component / "src" / "espnow_bbs_mesh_runtime_task_shell.c"
    main = root / "firmware" / "projects" / "espnow-bbs" / "main" / "main.c"
    required_files = [cmake, header, source, main]
    for required_file in required_files:
        if not required_file.exists():
            failures.append(f"missing ESP-NOW BBS runtime task shell file: {required_file.relative_to(root)}")
    if failures:
        return failures

    cmake_text = cmake.read_text(encoding="utf-8")
    header_text = header.read_text(encoding="utf-8")
    source_text = source.read_text(encoding="utf-8")
    main_text = main.read_text(encoding="utf-8")
    joined_text = "\n".join([cmake_text, header_text, source_text])

    required_markers = [
        "idf_component_register",
        "REQUIRES mesh_discovery_contract freertos",
        "StaticTask_t *task_buffer",
        "StackType_t *stack_buffer",
        "TaskHandle_t task_handle",
        "espnow_bbs_mesh_runtime_task_config_status",
        "espnow_bbs_mesh_runtime_task_start",
        "espnow_bbs_mesh_runtime_task_request_stop",
        "xTaskCreateStaticPinnedToCore",
        "ulTaskNotifyTake",
        "xTaskNotifyGive",
        "espnow_bbs_mesh_contract_runtime_scheduler_step",
    ]
    for marker in required_markers:
        if marker not in joined_text:
            failures.append(f"ESP-NOW BBS runtime task shell missing marker: {marker}")

    forbidden_shell_markers = [
        "xTaskCreate(",
        "xQueue",
        "xTimer",
        "StaticQueue_t",
        "StaticSemaphore_t",
        "QueueHandle",
        "TimerHandle",
        "SemaphoreHandle",
        "EventGroupHandle",
        "vTaskDelay",
        "esp_timer",
        "esp_event_handler",
        "(*callback",
        "(*hook",
        "callback",
        "hook",
        "handler",
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "nimble",
        "ble_gap",
        "ble_gatt",
        "nvs_",
        "uart_",
        "gpio_",
        "httpd_",
        "esp_partition",
        "esp_flash",
        "esp_ota",
        "FILE *",
        "fopen",
        "fwrite",
        "snprintf",
        "fprintf",
        "open(",
        "write(",
        "printf",
        "ESP_LOG",
        "malloc",
        "calloc",
        "realloc",
        "free(",
        "vPortFree",
        "strdup",
    ]
    for marker in forbidden_shell_markers:
        if marker in joined_text:
            failures.append(f"ESP-NOW BBS runtime task shell contains forbidden marker: {marker}")

    main_forbidden_markers = ["espnow_bbs_mesh_runtime_task_start"]
    for marker in main_forbidden_markers:
        if marker in main_text:
            failures.append(f"ESP-NOW BBS app_main starts or includes runtime task shell: {marker}")

    return failures


def audit_espnow_bbs_mesh_startup_lifecycle(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    project = root / "firmware" / "projects" / "espnow-bbs"
    component = project / "components" / "mesh_startup_lifecycle"
    cmake = component / "CMakeLists.txt"
    header = component / "include" / "espnow_bbs_mesh_startup_lifecycle.h"
    source = component / "src" / "espnow_bbs_mesh_startup_lifecycle.c"
    main = project / "main" / "main.c"
    required_files = [cmake, header, source, main]
    for required_file in required_files:
        if not required_file.exists():
            failures.append(
                "missing ESP-NOW BBS startup lifecycle file: "
                f"{required_file.relative_to(root)}"
            )
    if failures:
        return failures

    cmake_text = cmake.read_text(encoding="utf-8")
    header_text = header.read_text(encoding="utf-8")
    source_text = source.read_text(encoding="utf-8")
    main_text = main.read_text(encoding="utf-8")
    joined_text = "\n".join([cmake_text, header_text, source_text])

    required_markers = [
        "idf_component_register",
        "REQUIRES mesh_runtime_task_shell",
        "espnow_bbs_mesh_startup_lifecycle_t",
        "espnow_bbs_mesh_runtime_task_t runtime_task",
        "espnow_bbs_mesh_runtime_task_config_t task_config",
        "ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_UNINITIALIZED",
        "ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_CONFIGURED",
        "ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STARTING",
        "ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_RUNNING",
        "ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOP_REQUESTED",
        "ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_STOPPED",
        "ESPNOW_BBS_MESH_STARTUP_LIFECYCLE_STATE_FAILED",
        "\"uninitialized\"",
        "\"configured\"",
        "\"starting\"",
        "\"running\"",
        "\"stop_requested\"",
        "\"stopped\"",
        "\"failed\"",
        "espnow_bbs_mesh_startup_lifecycle_configure",
        "espnow_bbs_mesh_startup_lifecycle_start",
        "espnow_bbs_mesh_startup_lifecycle_request_stop",
        "espnow_bbs_mesh_startup_lifecycle_poll",
        "espnow_bbs_mesh_startup_lifecycle_deinit",
        "espnow_bbs_mesh_runtime_task_config_status",
        "espnow_bbs_mesh_runtime_task_start",
        "espnow_bbs_mesh_runtime_task_request_stop",
        "espnow_bbs_mesh_runtime_task_is_running",
        "lifecycle->runtime_task.task_handle == NULL",
        "espnow_bbs_mesh_startup_lifecycle_can_deinit",
    ]
    for marker in required_markers:
        if marker not in joined_text:
            failures.append(f"ESP-NOW BBS startup lifecycle missing marker: {marker}")

    forbidden_lifecycle_markers = [
        "xTaskCreate(",
        "xTaskCreateStatic",
        "xQueue",
        "xTimer",
        "StaticQueue_t",
        "StaticSemaphore_t",
        "QueueHandle",
        "TimerHandle",
        "SemaphoreHandle",
        "EventGroupHandle",
        "vTaskDelay",
        "esp_timer",
        "esp_event_handler",
        "(*callback",
        "(*hook",
        "callback",
        "hook",
        "handler",
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "nimble",
        "ble_gap",
        "ble_gatt",
        "nvs_",
        "uart_",
        "gpio_",
        "httpd_",
        "esp_partition",
        "esp_flash",
        "esp_ota",
        "FILE *",
        "fopen",
        "fwrite",
        "snprintf",
        "fprintf",
        "open(",
        "write(",
        "printf",
        "ESP_LOG",
        "malloc",
        "calloc",
        "realloc",
        "free(",
        "vPortFree",
        "strdup",
    ]
    for marker in forbidden_lifecycle_markers:
        if marker in joined_text:
            failures.append(f"ESP-NOW BBS startup lifecycle contains forbidden marker: {marker}")

    return failures


def audit_espnow_bbs_app_main_startup_integration(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    project = root / "firmware" / "projects" / "espnow-bbs"
    main_cmake = (project / "main" / "CMakeLists.txt").read_text(encoding="utf-8")
    main_c = (project / "main" / "main.c").read_text(encoding="utf-8")

    failures.extend(require_markers(main_cmake, [
        "PRIV_REQUIRES mesh_discovery_contract mesh_retained_persistence mesh_startup_lifecycle",
    ], "ESP-NOW BBS M4O main CMake boundary"))
    failures.extend(require_markers(main_c, [
        "#include \"espnow_bbs_mesh_startup_lifecycle.h\"",
        "ESPNOW_BBS_MESH_APP_OWNER_NAME \"mesh_ingress\"",
        "ESPNOW_BBS_MESH_APP_PROFILE_NAME \"balanced\"",
        "ESPNOW_BBS_MESH_APP_TASK_PERIOD_TICKS 1u",
        "ESPNOW_BBS_MESH_APP_TASK_YIELD_INTERVAL 2u",
        "ESPNOW_BBS_MESH_APP_TASK_STACK_DEPTH 4096u",
        "ESPNOW_BBS_MESH_APP_TASK_PRIORITY (tskIDLE_PRIORITY + 1u)",
        "ESPNOW_BBS_MESH_APP_TASK_CORE ESPNOW_BBS_MESH_RUNTIME_TASK_CORE_NONE",
        "static espnow_bbs_mesh_contract_event_queue_t s_mesh_event_queue",
        "static espnow_bbs_mesh_contract_owner_t s_mesh_owner",
        "static espnow_bbs_mesh_contract_runtime_scheduler_t s_mesh_runtime_scheduler",
        "static espnow_bbs_mesh_startup_lifecycle_t s_mesh_lifecycle",
        "static StaticTask_t s_mesh_task_buffer",
        "static StackType_t s_mesh_task_stack[ESPNOW_BBS_MESH_APP_TASK_STACK_DEPTH]",
        "static volatile espnow_bbs_mesh_contract_status_t s_mesh_app_contract_status",
        "static volatile espnow_bbs_mesh_startup_lifecycle_status_t",
        "espnow_bbs_mesh_contract_event_queue_init(&s_mesh_event_queue)",
        "espnow_bbs_mesh_contract_owner_init(&s_mesh_owner, &s_mesh_event_queue)",
        "espnow_bbs_mesh_contract_runtime_scheduler_init(&s_mesh_runtime_scheduler)",
        "espnow_bbs_mesh_contract_owner_claim(",
        "espnow_bbs_mesh_contract_runtime_scheduler_configure(",
        "espnow_bbs_mesh_startup_lifecycle_init(&s_mesh_lifecycle)",
        "espnow_bbs_mesh_startup_lifecycle_configure(",
        "espnow_bbs_mesh_startup_lifecycle_start(&s_mesh_lifecycle)",
        "s_mesh_app_lifecycle_status = espnow_bbs_mesh_app_start();",
    ], "ESP-NOW BBS M4O app_main startup integration"))

    forbidden_main_markers = [
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "nimble",
        "ble_gap",
        "ble_gatt",
        "nvs_",
        "uart_",
        "gpio_",
        "httpd_",
        "esp_partition",
        "esp_flash",
        "esp_ota",
        "FILE *",
        "fopen",
        "fwrite",
        "snprintf",
        "fprintf",
        "open(",
        "write(",
        "printf",
        "ESP_LOG",
        "malloc",
        "calloc",
        "realloc",
        "free(",
        "vPortFree",
        "strdup",
        "xQueue",
        "xTimer",
        "esp_timer",
        "esp_event_handler",
        "callback",
        "hook",
        "handler",
    ]
    for marker in forbidden_main_markers:
        if marker in main_c:
            failures.append(
                "ESP-NOW BBS M4O app_main integration contains forbidden "
                f"marker: {marker}"
            )

    return failures


def audit_espnow_bbs_app_main_retained_recovery_integration(
    root: Path = ROOT,
) -> list[str]:
    failures: list[str] = []
    project = root / "firmware" / "projects" / "espnow-bbs"
    main_cmake = (project / "main" / "CMakeLists.txt").read_text(encoding="utf-8")
    main_c = (project / "main" / "main.c").read_text(encoding="utf-8")

    failures.extend(require_markers(main_cmake, [
        "PRIV_REQUIRES mesh_discovery_contract mesh_retained_persistence mesh_startup_lifecycle",
    ], "ESP-NOW BBS M4W main CMake retained recovery boundary"))
    failures.extend(require_markers(main_c, [
        "#include \"espnow_bbs_mesh_retained_persistence.h\"",
        "static espnow_bbs_mesh_contract_persistence_intent_t s_mesh_retained_intent",
        "static espnow_bbs_mesh_retained_persistence_storage_t s_mesh_retained_storage",
        "static volatile espnow_bbs_mesh_retained_persistence_status_t",
        "s_mesh_app_retained_init_status",
        "s_mesh_app_retained_load_status",
        "static void espnow_bbs_mesh_app_recover_retained_intent(void)",
        "espnow_bbs_mesh_contract_persistence_intent_init(&s_mesh_retained_intent)",
        "espnow_bbs_mesh_retained_persistence_storage_context_init(",
        "espnow_bbs_mesh_retained_persistence_storage_init(",
        "ESPNOW_BBS_MESH_RETAINED_PERSISTENCE_REJECT_NOT_INITIALIZED",
        "espnow_bbs_mesh_retained_persistence_storage_load(",
        "espnow_bbs_mesh_app_recover_retained_intent();",
        "Gate M4W adds source-only retained recovery",
        "reboot",
        "live NVS execution remain unaccepted",
    ], "ESP-NOW BBS M4W app_main retained recovery integration"))

    forbidden_main_markers = [
        "espnow_bbs_mesh_retained_persistence_storage_save",
        "espnow_bbs_mesh_retained_persistence_storage_erase",
        "nvs_",
        "nvs_commit",
        "nvs_flash_erase",
        "esp_partition",
        "esp_flash",
        "uart_",
        "printf",
        "ESP_LOG",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_route",
        "espnow_bbs_mesh_contract_bridge_request_text_ingest",
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "xTimer",
        "esp_timer",
        "httpd_",
    ]
    for marker in forbidden_main_markers:
        if marker in main_c:
            failures.append(
                "ESP-NOW BBS M4W app_main retained recovery contains "
                f"forbidden marker: {marker}"
            )

    return failures


def audit_espnow_bbs_app_main_bridge_readiness_integration(
    root: Path = ROOT,
) -> list[str]:
    failures: list[str] = []
    project = root / "firmware" / "projects" / "espnow-bbs"
    main_c = (project / "main" / "main.c").read_text(encoding="utf-8")

    failures.extend(require_markers(main_c, [
        "ESPNOW_BBS_MESH_APP_BRIDGE_READINESS_REQUEST \"discovery_snapshot\"",
        "static espnow_bbs_mesh_contract_bridge_summary_state_t",
        "s_mesh_bridge_summary_state",
        "static espnow_bbs_mesh_contract_bridge_request_router_t s_mesh_bridge_router",
        "static espnow_bbs_mesh_contract_bridge_dispatch_envelope_t s_mesh_bridge_envelope",
        "static espnow_bbs_mesh_contract_bridge_request_text_intent_t",
        "s_mesh_bridge_request_intent",
        "static char s_mesh_bridge_readiness_summary[",
        "ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u",
        "static volatile espnow_bbs_mesh_contract_status_t s_mesh_app_bridge_status",
        "static volatile size_t s_mesh_app_bridge_line_bytes",
        "static void espnow_bbs_mesh_app_capture_bridge_readiness(void)",
        "s_mesh_bridge_summary_state.event_queue = &s_mesh_event_queue",
        "s_mesh_bridge_summary_state.owner = &s_mesh_owner",
        "s_mesh_bridge_summary_state.runtime_scheduler = &s_mesh_runtime_scheduler",
        "espnow_bbs_mesh_contract_bridge_request_router_init(&s_mesh_bridge_router)",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_init(",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_init(",
        "espnow_bbs_mesh_contract_bridge_request_text_intent_route(",
        "ESPNOW_BBS_MESH_APP_BRIDGE_READINESS_REQUEST",
        "s_mesh_bridge_readiness_summary",
        "sizeof(s_mesh_bridge_readiness_summary)",
        "s_mesh_app_bridge_line_bytes = line_bytes",
        "espnow_bbs_mesh_app_capture_bridge_readiness();",
        "Gate M4X adds source-only local",
        "live bridge dispatch remain",
    ], "ESP-NOW BBS M4X app_main bridge-readiness integration"))

    forbidden_main_markers = [
        "espnow_bbs_mesh_contract_bridge_request_route(",
        "espnow_bbs_mesh_contract_bridge_dispatch_envelope_route(",
        "espnow_bbs_mesh_contract_persistence_intent_stage",
        "espnow_bbs_mesh_retained_persistence_storage_save",
        "espnow_bbs_mesh_retained_persistence_storage_erase",
        "nvs_",
        "nvs_commit",
        "nvs_flash_erase",
        "uart_",
        "printf",
        "ESP_LOG",
        "esp_wifi",
        "esp_now",
        "esp_mesh",
        "esp_ble",
        "esp_bt",
        "xTimer",
        "esp_timer",
        "httpd_",
        "fopen",
        "fwrite",
        "malloc",
        "calloc",
        "realloc",
        "free(",
        "strdup",
    ]
    for marker in forbidden_main_markers:
        if marker in main_c:
            failures.append(
                "ESP-NOW BBS M4X app_main bridge-readiness contains "
                f"forbidden marker: {marker}"
            )

    return failures


def audit_firmware(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    failures.extend(audit_firmware_readme(root))
    failures.extend(audit_safe_core_contract(root))
    failures.extend(audit_firmware_forbidden_markers(root))
    failures.extend(audit_uart_bridge_boundary(root))
    failures.extend(audit_lcd_test_boundary(root))
    failures.extend(audit_encoder_menu_boundary(root))
    failures.extend(audit_espnow_bbs_build_only_target(root))
    failures.extend(audit_espnow_bbs_mesh_discovery_contract(root))
    failures.extend(audit_espnow_bbs_mesh_retained_persistence(root))
    failures.extend(audit_espnow_bbs_mesh_runtime_task_shell(root))
    failures.extend(audit_espnow_bbs_mesh_startup_lifecycle(root))
    failures.extend(audit_espnow_bbs_app_main_startup_integration(root))
    failures.extend(audit_espnow_bbs_app_main_retained_recovery_integration(root))
    failures.extend(audit_espnow_bbs_app_main_bridge_readiness_integration(root))
    return failures
