# Handoff 0164: Agricultural Telemetry Evidence Intake To Hardware Communications QA

Status: active

Task:
[Task 0227](../TASK_LOG/0227-agricultural-telemetry-evidence-intake.md)

Source coverage:
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-EVIDENCE-INTAKE-2026-06-07`,
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`,
`SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`,
`SRC-METER-TEROS12-2026-05-25`,
`SRC-SENTEK-DRILLDROP-2026-05-25`,
`SRC-IRROMETER-SOIL-SENSORS-2026-05-25`,
and `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`.

## Verified Facts

- ADR-0011 and Task 0226 keep agricultural telemetry as candidate planning
  only.
- Task 0227 adds an evidence-intake packet template for future exact
  equipment/service review.
- The packet template defines required future evidence fields; it does not fill
  them or accept implementation.

## Assumptions

- Hardware and Communications owners will fill one packet per exact
  equipment/service lane before any implementation gate.
- QA will treat unfilled packet fields as unresolved gaps, not as accepted
  evidence.

## Unknowns

- Exact center-pivot controller, soil probes, adapters, loggers, GPS trackers,
  service subscriptions, API rights, owner authorization, power, voltage,
  current budget, boot-pin impact, isolation, connectors, protocol/API details,
  calibration, units, retention/redaction policy, GPS pivot positioning, live
  telemetry proof, release, publication, commit, push, PR, and deploy remain
  open.

## Continue With

- Fill a single evidence-intake packet for one exact lane before creating any
  selected profile or implementation gate.
- Attach source-index rows or same-session local evidence for every factual
  claim.
- Keep power, voltage, current, boot-pin, isolation, surge, connector,
  calibration, data-policy, and cleanup criteria visible in the filled packet.
- Run owner review before protocol/API, schema, firmware, bench, field, or live
  telemetry work.

## Do Not Do

- Do not treat the evidence-intake template as exact local hardware evidence.
- Do not wire field equipment, connect pivot controllers, attach soil probes,
  integrate APIs, add telemetry schemas, add firmware dependencies, select a
  framework, flash, open serial/RF, run live bench, release, publish, commit,
  push, open a PR, or deploy without a separately named gate and explicit
  authority.
