# FieldNET Pivot Monitoring Candidate Profile

## Status

Candidate telemetry class planning profile only. This profile does not select a
local center-pivot controller, prove controller access, verify wiring, or accept
GPS pivot positioning.

## Verified Facts

- Lindsay FieldNET and Pivot Watch sources are listed in
  `SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25` as candidate sources for
  center-pivot monitoring telemetry classes, including remote operation
  visibility, system alerts, tower/alignment fault alerts, and Pivot Watch
  remote monitoring compatibility claims.
- The same source row explicitly does not verify local pivot hardware, wiring,
  controller protocol, or GPS position integration.

## Assumptions

- Any future center-pivot telemetry path should be treated as a vendor or
  controller-integration problem first, not as an ESP32 wiring target.
- Any ESP32-adjacent bridge must remain electrically isolated from pivot control
  wiring until a source-backed controller interface and qualified review exist.

## Required Verification Checklist

Use the
[agricultural telemetry evidence-intake packet](../evidence-intake/README.md)
before any implementation gate.

| Item | Required evidence before implementation |
| --- | --- |
| Exact local equipment | Pivot make/model, controller model, installed monitor device, vendor docs, photos/markings, and owner authorization. |
| Power and isolation | Power source, voltage range, isolation boundary, surge/lightning exposure, grounding, and qualified review before any bench or field connection. |
| Connector and protocol | Source-backed connector pinout, controller protocol/API, read-only mode, rate limits, authentication, and failure behavior. |
| GPS pivot position | Separate source and local evidence for any GPS pivot position device; this profile does not prove it. |
| Calibration and units | Units, timestamps, alert semantics, tower/alignment fault meaning, and data-retention policy. |

## Unknowns

- Exact local pivot/controller identity, installed monitor hardware, connector
  access, protocol/API rights, credentials, electrical isolation, surge
  protection, GPS position source, calibration/units, and live-proof plan.
- Whether any future integration is permitted to be direct, API-only, or
  operator-entered summary data.

## Stop Gates

No wiring, controller connection, firmware dependency, telemetry schema, API
integration, GPS position claim, live proof, release, publication, commit, push,
PR, or deploy is accepted by this candidate profile.
