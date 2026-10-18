# Agricultural Telemetry Evidence Intake Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-EVIDENCE-INTAKE-2026-06-07`

## Verified Facts

- Task 0227 adds an agricultural telemetry evidence-intake packet template.
- The template defines required future evidence fields for lane identity, exact
  equipment/service identity, provenance, power/voltage/current, isolation,
  connector/API boundary, protocol details, calibration/units, data policy,
  live-proof criteria, and cleanup criteria.
- The template includes lane-specific addenda for center-pivot monitoring, soil
  sensing, and GPS asset tracking.
- Same-session Tier 2 read-only reviewers approved the named evidence-intake
  packet definition with no P1/P2 blockers.

## Evidence

- Evidence-intake template:
  `hardware-profiles/agricultural/evidence-intake/README.md`
- Task record:
  `.agents/TASK_LOG/0227-agricultural-telemetry-evidence-intake.md`
- Handoff:
  `.agents/handoffs/0164-agricultural-telemetry-evidence-intake-to-hardware-communications-qa.md`
- Source-index row:
  `SRC-LOCAL-AGRICULTURAL-TELEMETRY-EVIDENCE-INTAKE-2026-06-07`
- Prior boundary:
  `.agents/DECISIONS/ADR-0011-agricultural-telemetry-profile-boundary.md` and
  `.agents/TASK_LOG/0226-agricultural-telemetry-profile-boundary.md`
- Candidate profiles:
  `hardware-profiles/agricultural/fieldnet-pivot-monitoring/README.md`,
  `hardware-profiles/agricultural/soil-sensing-candidates/README.md`, and
  `hardware-profiles/agricultural/gps-asset-tracking-candidates/README.md`
- External source rows:
  `SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`,
  `SRC-METER-TEROS12-2026-05-25`,
  `SRC-SENTEK-DRILLDROP-2026-05-25`,
  `SRC-IRROMETER-SOIL-SENSORS-2026-05-25`, and
  `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`

## Authority Limits

This source record does not accept exact local equipment or service identity,
selected hardware, local possession, wiring, connector pinout acceptance,
controller/API integration, protocol frame behavior, telemetry schemas,
firmware dependencies, framework choice, calibration acceptance, GPS pivot
positioning, live telemetry proof, release, publication, commit, push, PR, or
deploy.

## Open Evidence

- Filled packet evidence for one exact center-pivot controller, soil probe,
  adapter/logger, GPS tracker/service, or other separately sourced agricultural
  telemetry lane.
- Source-backed power, voltage, current, connector, isolation, surge,
  calibration, data-policy, and live-proof criteria for that exact lane.
