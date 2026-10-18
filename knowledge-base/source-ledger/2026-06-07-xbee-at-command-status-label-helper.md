# XBee AT Command Status Label Helper Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-AT-COMMAND-STATUS-LABEL-HELPER-2026-06-07`

## Verified Facts

- Task 0237 adds a pure host-only label helper for parsed Digi XBee Local AT
  Command Response `0x88` raw command-status bytes.
- The helper preserves raw parser behavior and does not reject unlisted status
  bytes.
- The helper labels only Digi-listed local `0x88` command status values:
  `0x00`, `0x01`, `0x02`, and `0x03`.
- Unlisted bytes including `0x04`, `0x0c`, and `0xff` return
  `unknown_at_command_status`.
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
  `.agents/TASK_LOG/0237-xbee-at-command-status-label-helper.md`
- Handoff:
  `.agents/handoffs/0174-xbee-at-command-status-label-helper-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-AT-COMMAND-STATUS-LABEL-HELPER-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-LOCAL-AT-RESPONSE-0X88-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-05-29`,
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-AP`, and
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 237`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0237 touched files passed with no output.
- A broad added-line closed-surface scan over touched dirty files included
  pre-existing dirty-tree rows from earlier uncommitted work; focused code scan
  over the safe-core header/source/test files found only unknown-status test
  labels and no serial, transmit, RF, bridge, relay, flash, release, or git
  behavior in the new helper path.
- Closed-surface review over the new Task 0237 record, handoff, and source
  ledger found only explicit unknown/open-evidence or authority-limit language
  for live XBee, serial, RF, relay, flash, release, publication, and git
  surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This source record does not accept or authorize `0x10` builder work, Transmit
Request generation, API transmit-frame generation, serial opens, Tier B AT
reads, XCTU or XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`,
API transmit to hardware, RF/range/throughput tests, live bridge dispatch,
relay payloads, relay/load/mains work, wiring, flashing, monitor, framework
selection, release, publication, commit, push, PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter
  delta.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Any future live RF, range, throughput, bridge-dispatch, relay-payload, or
  deployment proof.
