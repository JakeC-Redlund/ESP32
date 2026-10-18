# Handoff 0177: XBee Route Information Host Parser To QA Communications

Status: active

Task:
[Task 0240](../TASK_LOG/0240-xbee-route-information-host-parser.md)

Source coverage:
`SRC-LOCAL-XBEE-ROUTE-INFORMATION-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-ROUTE-INFORMATION-0X8D-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0240 adds a pure host-only parser for already-decoded Digi XBee Route
  Information `0x8D` frame data.
- The parser requires frame type `0x8D`, at least 42 decoded frame-data bytes,
  `data_length == frame->data_len - 3`, and `data_length >= 39`.
- The parser preserves raw source event, data length, timestamp in
  microseconds, ACK timeout count, TX blocked count, one reserved byte, four
  64-bit address arrays, and trailing-byte count.
- Official Digi source reconciliation supports one raw reserved byte: the
  source lists the reserved field as 8-bit and the example body length
  `0x27` matches timestamp 4, ACK timeout 1, TX blocked 1, reserved 1, and
  four 64-bit addresses.
- Future appended bytes are accepted only through `trailing_len`; Task 0240
  assigns no meaning to trailing bytes.
- The source-event helper labels only `0x11` as `nack` and `0x12` as
  `trace_route`, with unknown fallback for unlisted bytes.
- Focused safe-core host tests passed after the parser/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat parsed route information as diagnostics over an already
  received frame, not as proof that current project hardware can emit `0x8D`,
  perform Trace Routing, request NACKs, diagnose live links, transmit to
  hardware, or accept relay payloads.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, Tier B read
  safety, XCTU/XBee Studio readiness, live `0x8D` availability, live RF
  behavior, range, throughput, API transmit-to-hardware behavior, relay
  payload readiness, deployment readiness, release, publication, commit, push,
  PR, and deploy remain open.

## Continue With

- Preserve the parser as an in-memory decoded-frame parser only.
- Keep `data_length == frame->data_len - 3` and `trailing_len = data_length -
  39` for future appended source fields.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0240 as `0x10` builder, Trace Routing request generation,
  NACK request generation, Transmit Request generation, API transmit-frame
  generation, current hardware identity, serial-open, Tier B read, XCTU/XBee
  Studio discovery, XBee setting-write, API transmit-to-hardware,
  RF/range/throughput, live bridge dispatch, relay-payload, wiring, flash,
  monitor, live bench, release, publication, commit, push, PR, or deploy
  authority.
