# XBee Receive Options Helper Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-RECEIVE-OPTIONS-HELPER-2026-06-07`

## Verified Facts

- Task 0239 adds pure host-only diagnostic helpers over parsed Digi XBee
  Receive Packet `0x90` raw `receive_options` bytes.
- The helper update preserves `fr_xbee_parse_receive_packet` raw parser
  behavior and acceptance boundaries.
- The acknowledged helper checks only bit `0x01`.
- The broadcast helper checks only bit `0x02`.
- The delivery-method helper masks only `options & 0xC0u`.
- The helper labels `0x00` high bits as `invalid_delivery_method`, `0x40` as
  `point_multipoint`, `0x80` as `directed_broadcast`, and `0xC0` as
  `digimesh`.
- Host vectors cover individual and combined acknowledge/broadcast bits, each
  delivery-method value, and lower-bit combinations such as `0x41`, `0x82`,
  `0xC1`, and `0xFF`.

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
  `.agents/TASK_LOG/0239-xbee-receive-options-helper.md`
- Handoff:
  `.agents/handoffs/0176-xbee-receive-options-helper-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-RECEIVE-OPTIONS-HELPER-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-RECEIVE-PACKET-0X90-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-AO`,
  `SRC-DIGI-XBEE-900HP-AP`, and
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 239`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0239 touched files passed with no output.
- Focused code closed-surface scan over the safe-core header/source/test files
  found no serial, RF, bridge, relay, flash, monitor, release, or git behavior
  in the new receive-options helper path.
- Closed-surface review over the new Task 0239 record, handoff, and source
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
