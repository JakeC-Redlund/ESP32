# Handoff 0173: XBee Transmit Status Label Helpers To QA Communications

Status: active

Task:
[Task 0236](../TASK_LOG/0236-xbee-transmit-status-label-helpers.md)

Source coverage:
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-LABEL-HELPERS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-HOST-PARSER-2026-06-07`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0236 adds pure host-only label helpers for decoded Digi XBee Extended
  Transmit Status `0x8B` raw status bytes.
- The helpers do not alter `fr_xbee_parse_transmit_status` or reject unlisted
  raw bytes.
- Unlisted bytes return static unknown labels.
- Focused safe-core host tests passed after the helper/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat labels as source-backed diagnostics over raw bytes, not as live
  radio-link health or delivery proof.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, Tier B read
  safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput, API
  transmit-to-hardware behavior, relay payload readiness, deployment readiness,
  release, publication, commit, push, PR, and deploy remain open.

## Continue With

- Preserve raw `0x8B` parser output and label only source-listed status bytes.
- Keep unlisted delivery or discovery bytes on the unknown-label path unless a
  separate source-backed task adds coverage.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0236 as `0x10` builder, API transmit-frame generation,
  current hardware identity, serial-open, Tier B read, XCTU/XBee Studio
  discovery, XBee setting-write, API transmit-to-hardware, RF/range/throughput,
  live bridge dispatch, relay-payload, wiring, flash, monitor, live bench,
  release, publication, commit, push, PR, or deploy authority.
