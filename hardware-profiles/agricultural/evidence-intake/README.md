# Agricultural Telemetry Evidence Intake Packet

## Status

Evidence-intake packet template only. This record defines the evidence required
before any agricultural telemetry implementation gate. It does not record exact
local equipment, accept a service/API, select hardware, approve wiring, accept
calibration, or prove live telemetry.

## Verified Facts

- ADR-0011 accepts only a design-only taxonomy and protocol-family boundary for
  center-pivot monitoring, soil sensing, and GPS asset tracking.
- Task 0226 records candidate planning profiles only; it keeps local equipment,
  service/API rights, wiring, protocol behavior, calibration, and live proof
  unresolved.
- The five agricultural source rows remain candidate-only:
  `SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`,
  `SRC-METER-TEROS12-2026-05-25`,
  `SRC-SENTEK-DRILLDROP-2026-05-25`,
  `SRC-IRROMETER-SOIL-SENSORS-2026-05-25`, and
  `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`.

## Assumptions

- Future agricultural telemetry work should start by filling an evidence packet
  for one exact equipment/service lane, then running owner review before any
  schema, firmware, API, field, or bench work.
- Vendor/service APIs, SDI-12, RS232, RS485, Modbus, and analog/logger paths are
  protocol-family labels until exact device documentation and local evidence
  prove otherwise.
- GPS asset tracking and GPS pivot positioning are separate evidence topics
  unless a later source/local packet proves their relationship.

## Required Packet Fields

| Field | Required evidence before implementation |
| --- | --- |
| Lane identity | One named lane: center-pivot monitoring, soil sensing, GPS asset tracking, or a separately sourced agricultural telemetry lane. |
| Exact equipment or service | Manufacturer, model, serial/marking evidence where available, installed service/device identity, source links, local inspection notes, and owner authorization. |
| Evidence provenance | Date, collector, source IDs, photo or inspection references if available, credential/API access boundary, and unresolved gaps. |
| Power and voltage | Power source, voltage range, current budget, battery/solar relationship if any, boot-pin impact for ESP32-adjacent paths, and safe-off state. |
| Isolation and field risk | Electrical isolation boundary, surge/lightning exposure, grounding, enclosure/environmental exposure, connector stress, and qualified review need. |
| Connector or API boundary | Connector pinout or API endpoint class, read-only versus write capability, authentication or no-secret handling, rate limits, error behavior, and cleanup requirements. |
| Protocol details | Source-backed protocol/API documentation, command/frame behavior when relevant, adapter/logger choice, timing limits, and failure modes. |
| Calibration and units | Units, calibration procedure, compensation rules, timestamps, alert meanings, maintenance interval, and field validity limits. |
| Data policy | Retention, redaction, precise-location handling, operator access, no-secret logging, and export boundaries. |
| Live-proof criteria | Same-session identity, recovery path, read-only or write authority, expected transcript/data evidence, cleanup proof, and explicit stop conditions. |

## Lane-Specific Addenda

| Lane | Additional required evidence |
| --- | --- |
| Center-pivot monitoring | Pivot make/model, controller model, installed monitor device, vendor/controller access method, alert semantics, tower/alignment meaning, and separate proof for any GPS pivot-position source. |
| Soil sensing | Probe family, probe length/depth, measurement fields, sensor/logger/adapter path, bus voltage, cable length, soil/media limits, EC/salinity meaning, and maintenance/calibration constraints. |
| GPS asset tracking | Tracker/service identity, subscription/API rights, location update cadence, precise-location retention/redaction, antenna/power/enclosure facts, and proof that it is not being used as pivot-position evidence unless separately accepted. |

## Review Gate

An agricultural telemetry implementation gate can start only after one filled
packet is reviewed by Hardware, Communications, Architecture, and QA owners and
is backed by source-index rows or same-session local evidence. The review must
name the exact mutation boundary and whether it remains documentation-only,
host-only, bench read-only, or a later Tier 3 live/field action.

## Unknowns

- No exact local center-pivot controller, soil probe, adapter, logger, GPS
  tracker, service subscription, API right, connector, voltage/current budget,
  isolation boundary, calibration record, data-retention policy, or live-proof
  plan is captured by this packet template.
- GPS pivot positioning remains unresolved and separate from generic GPS asset
  tracking.

## Sources

- `SRC-LOCAL-AGRICULTURAL-TELEMETRY-EVIDENCE-INTAKE-2026-06-07`
- `SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`
- `SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`
- `SRC-METER-TEROS12-2026-05-25`
- `SRC-SENTEK-DRILLDROP-2026-05-25`
- `SRC-IRROMETER-SOIL-SENSORS-2026-05-25`
- `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`

## Stop Gates

No selected hardware, local possession claim, wiring, connector pinout
acceptance, controller/API integration, protocol frame behavior, telemetry
schema, firmware dependency, framework choice, calibration acceptance, GPS
pivot positioning claim, live telemetry proof, release, publication, commit,
push, PR, or deploy is accepted by this evidence-intake packet template.
