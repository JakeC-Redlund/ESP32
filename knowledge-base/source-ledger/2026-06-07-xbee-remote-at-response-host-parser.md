# XBee Remote AT Response Host Parser Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-REMOTE-AT-RESPONSE-HOST-PARSER-2026-06-07`

## Verified Facts

- Task 0243 adds a pure host-only parser for already-decoded Digi XBee Remote
  AT Command Response `0x97` frame data.
- Official Digi 900HP/XSC documentation identifies `0x97` as the Remote AT
  Command Response emitted for Remote AT Command Request `0x17`, with fields
  for frame ID, 64-bit source address, 16-bit reserved field, AT command,
  command status, and optional command data.
- The parser requires frame type `0x97`, at least 15 decoded frame-data bytes,
  no more than `FR_XBEE_MAX_FRAME_DATA` bytes, and optional command data no
  longer than `FR_XBEE_AT_VALUE_MAX`.
- The parser preserves frame ID, source64 byte order, the 900HP/XSC
  big-endian `reserved16` field, two-byte AT command, raw remote command
  status, and optional command data bytes.
- The remote-specific label helper covers source-listed statuses `0x00`,
  `0x01`, `0x02`, `0x03`, `0x04`, and `0x0c` without changing the local
  `0x88` status helper.

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
  `.agents/TASK_LOG/0243-xbee-remote-at-response-host-parser.md`
- Handoff:
  `.agents/handoffs/0180-xbee-remote-at-response-host-parser-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-REMOTE-AT-RESPONSE-HOST-PARSER-2026-06-07`
- Official source row:
  `SRC-DIGI-XBEE-REMOTE-AT-RESPONSE-0X97-2026-06-07`
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
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 243`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- Scoped `git diff --check` over the Task 0243 code and record boundary
  passed.
- Pre-acceptance pending-marker scan found only the expected Task 0243
  acceptance-status fields.
- Closed-surface review found only authority-limit/open-evidence wording and
  no authorized serial, RF, flash, relay, release, commit, push, PR, or deploy
  action.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This source record does not accept or authorize remote setting reads, Remote AT
Command Request `0x17` generation, API transmit-frame generation, serial
opens, Tier B AT reads, XCTU or XBee Studio discovery, XBee setting writes,
`WR`, `AC`, `KY`, API transmit to hardware, RF/range/throughput tests, live
`0x97` behavior claims, live bridge dispatch, relay payloads, relay/load/mains
work, wiring, flashing, monitor, framework selection, release, publication,
commit, push, PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter
  delta.
- Current local radio settings and whether local radios can emit `0x97`.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Live RF behavior, range, throughput, bridge-dispatch, relay-payload, and
  deployment proof.
