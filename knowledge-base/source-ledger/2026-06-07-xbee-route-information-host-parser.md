# XBee Route Information Host Parser Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-ROUTE-INFORMATION-HOST-PARSER-2026-06-07`

## Verified Facts

- Task 0240 adds a pure host-only parser for already-decoded Digi XBee Route
  Information `0x8D` frame data.
- The parser requires frame type `0x8D`, at least 42 decoded frame-data bytes,
  `data_length == frame->data_len - 3`, and `data_length >= 39`.
- The parser preserves source event, data length, timestamp in microseconds,
  ACK timeout count, TX blocked count, one reserved byte, destination/source/
  responder/receiver 64-bit addresses, and trailing-byte count.
- The parser uses the official Digi example and `data_length = 0x27` to
  reconcile the source table offset ambiguity and preserve one raw reserved
  byte before the address arrays.
- The source-event label helper maps only `0x11` to `nack` and `0x12` to
  `trace_route`.
- Host vectors cover the official example shape, parser acceptance/rejection
  boundaries, future appended bytes, and source-event labels.

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
  `.agents/TASK_LOG/0240-xbee-route-information-host-parser.md`
- Handoff:
  `.agents/handoffs/0177-xbee-route-information-host-parser-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-ROUTE-INFORMATION-HOST-PARSER-2026-06-07`
- Official source row:
  `SRC-DIGI-XBEE-ROUTE-INFORMATION-0X8D-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-AP`, and
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 240`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0240 touched files passed with no output.
- Focused code closed-surface scan over the safe-core header/source/test files
  found no new serial, RF, bridge, relay, flash, monitor, release, or git
  behavior in the new `0x8D` parser/helper path.
- Closed-surface review over the new Task 0240 record, handoff, and source
  ledger found only explicit unknown/open-evidence or authority-limit language
  for live XBee, serial, RF, relay, flash, release, publication, and git
  surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This source record does not accept or authorize `0x10` builder work, Trace
Routing request generation, NACK request generation, Transmit Request
generation, API transmit-frame generation, serial opens, Tier B AT reads, XCTU
or XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`, API transmit
to hardware, RF/range/throughput tests, live bridge dispatch, relay payloads,
relay/load/mains work, wiring, flashing, monitor, framework selection,
release, publication, commit, push, PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter
  delta.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Live `0x8D` availability, RF behavior, range, throughput, bridge-dispatch,
  relay-payload, and deployment proof.
