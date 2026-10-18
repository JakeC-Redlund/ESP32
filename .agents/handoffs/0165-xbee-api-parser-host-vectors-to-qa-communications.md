# Handoff 0165: XBee API Parser Host Vectors To QA Communications

Status: active

Task:
[Task 0228](../TASK_LOG/0228-xbee-api-parser-host-vectors.md)

Source coverage:
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`,
`SRC-LOCAL-XBEE-HUB-SPOKE-HOST-PLAN-2026-06-05`, and
`SRC-LOCAL-XBEE-TIER-A-NO-SERIAL-IDENTITY-EVIDENCE-2026-06-05`.

## Verified Facts

- Task 0228 adds host-only XBee API2 codec/parser boundary vectors.
- Task 0228 adds a receive-packet parser guard for direct host frames whose
  `data_len` is larger than the fixed parser buffer.
- Focused safe-core host tests passed after the parser/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat these vectors as local parser coverage, not live radio proof.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, Tier B read
  safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput,
  relay payload readiness, deployment readiness, release, publication, commit,
  push, PR, and deploy remain open.

## Continue With

- Review any future parser additions against the fixed host buffer size before
  adding direct parser vectors.
- Keep new XBee frame claims source-index-backed or mark them as unresolved.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core parser changes.

## Do Not Do

- Do not treat Task 0228 as serial, RF, bridge-dispatch, relay-payload, range,
  throughput, XBee setting-write, flash, live bench, hardware, release,
  publication, commit, push, PR, or deploy authority.
