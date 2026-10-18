# XBee Modem Status Host Parser Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-MODEM-STATUS-HOST-PARSER-2026-06-07`

## Verified Facts

- Task 0238 adds a pure host-only parser for already-decoded Digi XBee Modem
  Status `0x8A` frame data.
- The parser requires exactly two decoded bytes: frame type `0x8A` and the raw
  modem-status byte.
- The parser preserves the raw status byte and rejects null pointers, wrong
  frame type, short frame data, and long frame data.
- The label helper maps only source-listed modem-status values and the
  source-listed `0x80` through `0xff` stack-error range.
- Unlisted bytes including `0x04`, `0x41`, and `0x7f` return
  `unknown_modem_status`.
- Host vectors cover the parser acceptance/rejection boundary, source-listed
  labels, stack-error range labels, and unknown fallbacks.

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
  `.agents/TASK_LOG/0238-xbee-modem-status-host-parser.md`
- Handoff:
  `.agents/handoffs/0175-xbee-modem-status-host-parser-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-MODEM-STATUS-HOST-PARSER-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-MODEM-STATUS-0X8A-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-AP`, and
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 238`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0238 touched files passed with no output.
- Focused code closed-surface scan over the safe-core header/source/test files
  found no serial, RF, bridge, relay, flash, monitor, release, or git behavior
  in the new `0x8A` parser/helper path.
- Closed-surface review over the new Task 0238 record, handoff, and source
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
