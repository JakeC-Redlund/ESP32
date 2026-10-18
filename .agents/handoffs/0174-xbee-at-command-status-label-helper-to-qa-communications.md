# Handoff 0174: XBee AT Command Status Label Helper To QA Communications

Status: active

Task:
[Task 0237](../TASK_LOG/0237-xbee-at-command-status-label-helper.md)

Source coverage:
`SRC-LOCAL-XBEE-AT-COMMAND-STATUS-LABEL-HELPER-2026-06-07`,
`SRC-DIGI-XBEE-LOCAL-AT-RESPONSE-0X88-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-05-29`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0237 adds a pure host-only label helper for parsed Digi XBee Local AT
  Command Response `0x88` raw command-status bytes.
- The helper does not alter `fr_xbee_parse_at_response` or reject unlisted raw
  bytes.
- Source-listed local `0x88` status values `0x00` through `0x03` return static
  labels.
- Unlisted bytes return `unknown_at_command_status`.
- Focused safe-core host tests passed after the helper/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat labels as source-backed diagnostics over raw bytes, not as live
  radio-link health, read-query safety, command success proof, or deployment
  readiness.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, Tier B read
  safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput, API
  transmit-to-hardware behavior, relay payload readiness, deployment readiness,
  release, publication, commit, push, PR, and deploy remain open.

## Continue With

- Preserve raw `0x88` parser output and label only source-listed local
  command-status bytes.
- Keep unlisted local `0x88` command-status bytes on the unknown-label path
  unless a separate source-backed task adds coverage.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0237 as `0x10` builder, Transmit Request generation, API
  transmit-frame generation, current hardware identity, serial-open, Tier B
  read, XCTU/XBee Studio discovery, XBee setting-write, API
  transmit-to-hardware, RF/range/throughput, live bridge dispatch,
  relay-payload, wiring, flash, monitor, live bench, release, publication,
  commit, push, PR, or deploy authority.
