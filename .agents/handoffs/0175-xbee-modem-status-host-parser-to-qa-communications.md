# Handoff 0175: XBee Modem Status Host Parser To QA Communications

Status: active

Task:
[Task 0238](../TASK_LOG/0238-xbee-modem-status-host-parser.md)

Source coverage:
`SRC-LOCAL-XBEE-MODEM-STATUS-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-MODEM-STATUS-0X8A-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0238 adds a pure host-only parser and label helper for already-decoded
  Digi XBee Modem Status `0x8A` frame data.
- The parser requires exactly two decoded bytes: `0x8A` and the raw
  modem-status byte.
- The parser preserves the raw status byte in `fr_xbee_modem_status_t`.
- The helper labels only source-listed Digi modem-status values plus the
  source-listed stack-error range `0x80` through `0xff`.
- Unlisted bytes including `0x04`, `0x41`, and `0x7f` return
  `unknown_modem_status`.
- Focused safe-core host tests passed after the parser/helper/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat modem-status labels as diagnostics over raw received status
  bytes, not as live radio-link health, read-query safety, command success
  proof, or deployment readiness.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, Tier B read
  safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput, API
  transmit-to-hardware behavior, relay payload readiness, deployment readiness,
  release, publication, commit, push, PR, and deploy remain open.

## Continue With

- Preserve raw `0x8A` parser output and keep parser acceptance strict to the
  two decoded Modem Status frame-data bytes.
- Keep unlisted modem-status bytes on the unknown-label path unless a separate
  source-backed task adds coverage.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0238 as `0x10` builder, Transmit Request generation, API
  transmit-frame generation, current hardware identity, serial-open, Tier B
  read, XCTU/XBee Studio discovery, XBee setting-write, API
  transmit-to-hardware, RF/range/throughput, live bridge dispatch,
  relay-payload, wiring, flash, monitor, live bench, release, publication,
  commit, push, PR, or deploy authority.
