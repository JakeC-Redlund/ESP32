# XBee Transmit Status Label Helpers Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-LABEL-HELPERS-2026-06-07`

## Verified Facts

- Task 0236 adds pure host-only label helpers for decoded Digi XBee Extended
  Transmit Status `0x8B` delivery-status and discovery-status bytes.
- The helpers preserve raw parser behavior and do not reject unlisted status
  bytes.
- The delivery-status helper labels only Digi-listed values:
  `0x00`, `0x01`, `0x02`, `0x03`, `0x21`, `0x25`, `0x31`, `0x32`, `0x74`, and
  `0x75`.
- The discovery-status helper labels only Digi-listed values: `0x00` and
  `0x02`.
- Host vectors cover all listed values and required unknown fallbacks.

## Evidence

- Header:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`
- Parser/helper source:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`
- Host vectors:
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`
- Host test runner:
  `tests/four_relay_safe_core/run_host_tests.py`
- Task record:
  `.agents/TASK_LOG/0236-xbee-transmit-status-label-helpers.md`
- Handoff:
  `.agents/handoffs/0173-xbee-transmit-status-label-helpers-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-TRANSMIT-STATUS-LABEL-HELPERS-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
  `SRC-LOCAL-XBEE-TRANSMIT-STATUS-HOST-PARSER-2026-06-07`,
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-AP`, and
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 236`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0236 touched files passed with no output.
- Closed-surface review over the Task 0236 touched files found only explicit
  unknown/open-evidence or authority-limit language for live XBee, serial, RF,
  relay, flash, release, publication, and git surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This source record does not accept or authorize `0x10` builder work, API
transmit-frame generation, serial opens, Tier B AT reads, XCTU or XBee Studio
discovery, XBee setting writes, `WR`, `AC`, `KY`, API transmit to hardware,
RF/range/throughput tests, live bridge dispatch, relay payloads,
relay/load/mains work, wiring, flashing, monitor, framework selection, release,
publication, commit, push, PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter delta.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Any future live RF, range, throughput, bridge-dispatch, relay-payload, or
  deployment proof.
