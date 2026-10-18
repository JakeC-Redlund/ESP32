# ADR-0011: Agricultural Telemetry Profile Boundary

Status: Accepted

Date: 2026-06-07

## Context

The custom wireless protocol records have long kept agricultural telemetry,
soil sensing, GPS, and reporting schemas blocked until source-backed hardware
profiles and owner review exist. Current source-index rows cover candidate
telemetry classes, but they do not prove local hardware possession, wiring,
controller protocol, GPS pivot positioning, calibration, or live telemetry.

## Verified Facts

- `SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25` is candidate-only evidence for
  center-pivot monitoring classes and explicitly does not verify local pivot
  hardware, wiring, controller protocol, or GPS position integration.
- `SRC-METER-TEROS12-2026-05-25` is candidate-only evidence for soil moisture,
  temperature, electrical conductivity, SDI-12, voltage, and data-acquisition
  planning fields.
- `SRC-SENTEK-DRILLDROP-2026-05-25` is candidate-only evidence for multi-depth
  soil moisture, temperature, optional salinity, RS232, RS485, SDI-12, and
  Modbus planning fields.
- `SRC-IRROMETER-SOIL-SENSORS-2026-05-25` is candidate-only evidence for soil
  water tension, WATERMARK 200SS, tensiometer ranges, transducer outputs, and
  SDI-12 or analog-adapter planning.
- `SRC-GEOTAB-ASSET-TRACKING-2026-05-25` is candidate-only evidence for GPS
  asset tracking classes and explicitly does not verify local subscription,
  device selection, API access, or integration rights.

## Accepted Decision

Accept `agricultural-telemetry-profile-boundary.v1` as a design-only taxonomy
and protocol-family boundary for future owner review.

The accepted planning classes are:

- center-pivot monitoring candidate profile
- soil-sensing candidate profiles
- GPS asset-tracking candidate profile

The accepted protocol-family labels are planning labels only:

- vendor/controller or service API
- SDI-12
- RS232
- RS485
- Modbus
- analog adapter or logger interface
- GPS asset-tracking service/API

This ADR does not accept protocol frame behavior, telemetry payload schemas,
controller API integration, adapter selection, selected hardware, wiring,
firmware dependencies, calibration, or live telemetry proof.

## Required Future Gate

Before implementation, each agricultural telemetry lane must produce a
source-backed or same-session evidence packet that covers:

- exact local hardware or service identity
- source links and ownership/authorization notes
- power source, voltage range, current budget, and boot-pin impact where an
  ESP32 is involved
- isolation, surge/transient exposure, connector pinout, and field enclosure
  constraints
- protocol/API details, authentication or no-secret handling, and failure modes
- calibration, units, timestamps, retention, redaction, and data-access policy
- live-proof and cleanup criteria for the exact named gate

## Assumptions

- The useful next step is to keep agricultural telemetry as a planning lane
  until exact local equipment and owner constraints are available.
- Candidate source records are enough to structure future review, but not to
  select devices or write firmware.

## Unknowns

- Exact local center-pivot controller, soil probes, adapters, loggers, GPS
  trackers, service subscriptions, API rights, power/voltage interfaces,
  connectors, isolation boundaries, calibration/units, retention policy, and
  live-proof plan.
- Whether GPS pivot positioning exists as a separate local device or service;
  current sources do not prove it.

## Review Quorum

- Hardware/Communications/Evidence reviewer
  `019e9f81-912f-7ab3-8e1d-4f36425f1464`, weight 3, approved the Tier 2
  records/profile-boundary gate with no P1/P2 blockers and P3 conditions to
  keep wording candidate-only, mark GPS pivot positioning unresolved, keep
  schemas/API/firmware/live work closed, and add docs-index reachability.
- QA/Evidence reviewer `019e9f81-cb93-7be1-9883-6ea8154ecd33`, weight 3,
  approved the Tier 2 records/design-only mutation with no P1/P2 blockers and
  P3 conditions for source-separated profile docs, boundary-only ADR wording,
  docs-index reachability, and source-index coverage.

Weighted reviewer result: 6/6 approval with no P1/P2 blockers.

## Sources

- `SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`
- `SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`
- `SRC-METER-TEROS12-2026-05-25`
- `SRC-SENTEK-DRILLDROP-2026-05-25`
- `SRC-IRROMETER-SOIL-SENSORS-2026-05-25`
- `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`

## Stop Gates

This ADR does not authorize selected hardware, wiring, controller/API
integration, protocol frame behavior, telemetry schemas, firmware dependencies,
framework choice, local possession claims, GPS pivot positioning, calibration
acceptance, live telemetry proof, serial/RF, flash, release, publication,
commit, push, PR, or deploy.
