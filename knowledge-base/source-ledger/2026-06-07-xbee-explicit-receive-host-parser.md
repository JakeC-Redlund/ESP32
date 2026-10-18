# XBee Explicit Receive Host Parser Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-EXPLICIT-RECEIVE-HOST-PARSER-2026-06-07`

## Verified Facts

- Task 0242 adds a pure host-only parser for already-decoded Digi XBee
  Explicit Receive Indicator `0x91` frame data.
- The parser requires frame type `0x91`, more than 18 decoded frame-data bytes,
  and no more than `FR_XBEE_MAX_FRAME_DATA` bytes.
- The parser preserves the source 64-bit address byte order.
- The parser preserves the 900HP/XSC 16-bit reserved field as `reserved16`.
- The parser preserves source endpoint, destination endpoint, big-endian
  cluster ID, big-endian profile ID, raw receive options, and payload bytes.
- Host vectors cover the official example body, parser acceptance/rejection
  boundaries, local header-only rejection, max payload, raw receive-options
  preservation, byte-order preservation, and payload preservation.

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
  `.agents/TASK_LOG/0242-xbee-explicit-receive-host-parser.md`
- Handoff:
  `.agents/handoffs/0179-xbee-explicit-receive-host-parser-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-EXPLICIT-RECEIVE-HOST-PARSER-2026-06-07`
- Official source row:
  `SRC-DIGI-XBEE-EXPLICIT-RECEIVE-0X91-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
  `SRC-DIGI-XBEE-900HP-AO`, `SRC-DIGI-XBEE-900HP-AP`, and
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 242`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- Scoped `git diff --check` over the Task 0242 code and record boundary
  passed.
- Pending-marker scan before acceptance found only the expected Task 0242
  acceptance-status fields.
- Closed-surface review found only authority-limit/open-evidence wording and
  no authorized serial, RF, flash, relay, release, commit, push, PR, or deploy
  action.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This source record does not accept or authorize changing `AO`, building `0x10`
or `0x11` frames, Transmit Request generation, Explicit Addressing Command
Request generation, API transmit-frame generation, serial opens, Tier B AT
reads, XCTU or XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`,
API transmit to hardware, RF/range/throughput tests, live `0x91` behavior
claims, live bridge dispatch, relay payloads, relay/load/mains work, wiring,
flashing, monitor, framework selection, release, publication, commit, push,
PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter
  delta.
- Current local `AO` value and whether local radios can emit `0x91`.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Live RF behavior, range, throughput, bridge-dispatch, relay-payload, and
  deployment proof.
