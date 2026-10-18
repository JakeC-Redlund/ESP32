# Handoff 0163: Agricultural Telemetry Profile Boundary To Hardware Communications QA

Status: active

Task:
[Task 0226](../TASK_LOG/0226-agricultural-telemetry-profile-boundary.md)

Source coverage:
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`,
`SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`,
`SRC-METER-TEROS12-2026-05-25`,
`SRC-SENTEK-DRILLDROP-2026-05-25`,
`SRC-IRROMETER-SOIL-SENSORS-2026-05-25`,
and `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`.

## Verified Facts

- Candidate telemetry class planning profiles now exist for center-pivot
  monitoring, soil sensing, and GPS asset tracking.
- ADR-0011 accepts only a taxonomy and protocol-family boundary for future
  owner review.
- Existing external source rows remain candidate-only and do not prove local
  hardware, wiring, calibration, API access, or live telemetry.

## Assumptions

- Hardware and Communications owners will continue by identifying exact local
  equipment, vendor/source links, electrical boundaries, protocol/API details,
  and calibration/units before implementation.
- Firmware work remains downstream of exact hardware evidence and a separately
  accepted implementation gate.

## Unknowns

- Exact local center-pivot controller, monitor hardware, soil probes, adapters,
  loggers, GPS trackers, service subscriptions, API rights, owner
  authorization, power, voltage, boot-pin impact, isolation, connector pinouts,
  protocol frame behavior, calibration, units, retention/redaction policy, GPS
  pivot positioning, firmware dependencies, and live telemetry proof remain
  open.
- Release, publication, commit, push, PR, and deploy remain unauthorized.

## Continue With

- Capture exact equipment/service identity and source links before creating any
  selected hardware profile.
- For any ESP32-adjacent interface, record power, voltage, current, boot-pin,
  isolation, connector, surge/transient, and field-enclosure risks before bench
  action.
- For protocol work, record the exact API or wire protocol, authentication or
  no-secret handling, calibration/units, retention/redaction policy, and
  failure behavior before schemas or firmware dependencies.

## Do Not Do

- Do not wire pivot controllers, soil probes, adapters, loggers, GPS trackers,
  batteries, solar paths, relay paths, or field equipment based on Task 0226.
- Do not add firmware dependencies, select a framework, accept telemetry
  schemas, integrate controller/API access, flash, open serial/RF, run live
  bench, release, publish, commit, push, open a PR, or deploy without a
  separately named gate and explicit authority.
