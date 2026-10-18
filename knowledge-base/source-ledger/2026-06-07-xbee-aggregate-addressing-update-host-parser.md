# XBee Aggregate Addressing Update Host Parser Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-AGGREGATE-ADDRESSING-UPDATE-HOST-PARSER-2026-06-07`

## Verified Facts

- Task 0241 adds a pure host-only parser for already-decoded Digi XBee
  Aggregate Addressing Update `0x8E` frame data.
- The parser requires frame type `0x8E` and exactly 18 decoded frame-data
  bytes.
- The parser preserves the raw reserved byte.
- The parser preserves the new and old 64-bit `DH`/`DL` destination-address
  fields as byte arrays.
- Host vectors cover the official example body, parser acceptance/rejection
  boundaries, raw reserved-byte preservation, and byte-order preservation.

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
  `.agents/TASK_LOG/0241-xbee-aggregate-addressing-update-host-parser.md`
- Handoff:
  `.agents/handoffs/0178-xbee-aggregate-addressing-update-host-parser-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-AGGREGATE-ADDRESSING-UPDATE-HOST-PARSER-2026-06-07`
- Official source row:
  `SRC-DIGI-XBEE-AGGREGATE-ADDRESSING-UPDATE-0X8E-2026-06-07`
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
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 241`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- Source, docs, records, and agent-process scaffold audits passed.
- Scoped `git diff --check` passed for the Task 0241 mutation boundary.
- Closed-surface review passed: implementation changes add only host parser/test
  coverage, and live/API-transmit/serial/RF/relay/flash/release matches are
  authority-limit text in records.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This source record does not accept or authorize issuing `AG`, updating `DH` or
`DL`, `0x10` builder work, Transmit Request generation, API transmit-frame
generation, serial opens, Tier B AT reads, XCTU or XBee Studio discovery, XBee
setting writes, `WR`, `AC`, `KY`, API transmit to hardware,
RF/range/throughput tests, live DigiMesh behavior claims, live bridge dispatch,
relay payloads, relay/load/mains work, wiring, flashing, monitor, framework
selection, release, publication, commit, push, PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter
  delta.
- Whether local radios can emit `0x8E` or support DigiMesh `AG` in the local
  configuration.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Live RF behavior, range, throughput, bridge-dispatch, relay-payload, and
  deployment proof.
