# XBee Transmit Status Host Parser Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-HOST-PARSER-2026-06-07`

## Verified Facts

- Task 0235 adds a host-only typed parser for decoded Digi XBee Extended
  Transmit Status `0x8B` frame data.
- The parser rejects null inputs, non-`0x8B` frame types, and decoded frame data
  whose length is not exactly seven bytes.
- The parser preserves `frame_id`, `reserved16`, `retry_count`,
  `delivery_status`, and `discovery_status` as raw fields.
- Host vectors cover a decoded API2 `0x8B` frame with nonzero raw status bytes,
  wrong-type rejection, short-frame rejection, long-frame rejection, null-frame
  rejection, and null-output rejection.
- Same-session Tier 2 read-only reviewers approved the named host-only parser
  boundary after clarification.

## Evidence

- Header:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`
- Parser source:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`
- Host vectors:
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`
- Host test runner:
  `tests/four_relay_safe_core/run_host_tests.py`
- Task record:
  `.agents/TASK_LOG/0235-xbee-transmit-status-host-parser.md`
- Handoff:
  `.agents/handoffs/0172-xbee-transmit-status-host-parser-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-TRANSMIT-STATUS-HOST-PARSER-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-AP`,
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`, and
  `SRC-LOCAL-XBEE-HUB-SPOKE-HOST-PLAN-2026-06-05`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 235`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0235 touched files passed with no output.
- Closed-surface review over the Task 0235 touched files found only explicit
  authority-limit or closed-surface language for live XBee, serial, RF, relay,
  flash, release, publication, and git surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This source record does not accept or authorize serial opens, Tier B AT reads,
XCTU or XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`, API
transmit to hardware, RF/range/throughput tests, live bridge dispatch, relay
payloads, relay/load/mains work, wiring, flashing, monitor, framework
selection, release, publication, commit, push, PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter delta.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Any future live RF, range, throughput, bridge-dispatch, relay-payload, or
  deployment proof.
