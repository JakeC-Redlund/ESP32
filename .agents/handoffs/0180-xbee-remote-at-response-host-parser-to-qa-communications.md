# Handoff 0180: XBee Remote AT Response Host Parser To QA Communications

Status: active

Task:
[Task 0243](../TASK_LOG/0243-xbee-remote-at-response-host-parser.md)

Source coverage:
`SRC-LOCAL-XBEE-REMOTE-AT-RESPONSE-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-REMOTE-AT-RESPONSE-0X97-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0243 adds a pure host-only parser for already-decoded Digi XBee Remote
  AT Command Response `0x97` frame data.
- The parser requires frame type `0x97`, at least 15 decoded frame-data bytes,
  no more than `FR_XBEE_MAX_FRAME_DATA` bytes, and no more than
  `FR_XBEE_AT_VALUE_MAX` optional command-data bytes.
- The parser preserves frame ID, source64 byte order, the 900HP/XSC
  `reserved16` field, AT command, raw remote command status, and optional
  command data bytes.
- The remote-specific label helper covers `0x00` OK, `0x01` error, `0x02`
  invalid command, `0x03` invalid parameter, `0x04` transmission failure, and
  `0x0c` encryption error.
- Focused safe-core host tests passed after the parser/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat parsed `0x97` responses as diagnostics over an already
  received frame, not as proof that current project hardware generated remote
  AT traffic, read a remote setting, diagnosed live links, or accepted relay
  payloads.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, whether local
  radios can emit `0x97`, Tier B read safety, XCTU/XBee Studio readiness, live
  RF behavior, range, throughput, API transmit-to-hardware behavior, live
  bridge behavior, relay payload readiness, deployment readiness, release,
  publication, commit, push, PR, and deploy remain open.

## Continue With

- Preserve the parser as an in-memory decoded-frame parser only.
- Keep the 900HP/XSC 16-bit field wording as `reserved16`, not source or
  network address.
- Keep remote `0x97` status labels separate from local `0x88` status labels.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0243 as authority to read remote settings, generate
  Remote AT Command Request `0x17`, generate API transmit frames, identify
  current hardware, open serial, run Tier B reads, launch XCTU/XBee Studio
  discovery, write XBee settings, transmit to hardware, test RF/range/
  throughput, dispatch bridge payloads, operate relays, wire hardware, flash,
  monitor, run live bench, release, publish, commit, push, create a PR, or
  deploy.
