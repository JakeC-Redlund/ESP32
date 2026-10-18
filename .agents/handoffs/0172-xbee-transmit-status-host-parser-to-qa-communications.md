# Handoff 0172: XBee Transmit Status Host Parser To QA Communications

Status: active

Task:
[Task 0235](../TASK_LOG/0235-xbee-transmit-status-host-parser.md)

Source coverage:
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`,
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`, and
`SRC-LOCAL-XBEE-HUB-SPOKE-HOST-PLAN-2026-06-05`.

## Verified Facts

- Task 0235 adds a host-only typed parser for decoded Digi XBee Extended
  Transmit Status `0x8B` frame data.
- The parser accepts only exactly seven decoded frame-data bytes and preserves
  `frame_id`, `reserved16`, `retry_count`, `delivery_status`, and
  `discovery_status` as raw fields.
- Focused safe-core host tests passed after the parser/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat the raw delivery and discovery status bytes as parser output,
  not as accepted radio-link health or delivery semantics.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, Tier B read
  safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput,
  relay payload readiness, deployment readiness, release, publication, commit,
  push, PR, and deploy remain open.

## Continue With

- Review any future `0x8B` handling against the official Digi frame-data shape
  and preserve raw status-byte semantics unless a separate source-backed
  interpretation task is opened.
- Keep new XBee frame claims source-index-backed or mark them as unresolved.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core parser changes.

## Do Not Do

- Do not treat Task 0235 as current hardware identity, serial-open, Tier B read,
  XCTU/XBee Studio discovery, XBee setting-write, API transmit-to-hardware,
  RF/range/throughput, live bridge dispatch, relay-payload, wiring, flash,
  monitor, live bench, release, publication, commit, push, PR, or deploy
  authority.
