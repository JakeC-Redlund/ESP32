# Agricultural Telemetry Profile Boundary Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`

## Verified Facts

- Task 0226 adds candidate telemetry class planning profiles for center-pivot
  monitoring, soil sensing, and GPS asset tracking.
- ADR-0011 accepts only a design-only taxonomy and protocol-family boundary for
  owner review.
- The five external agricultural source rows remain candidate-only and do not
  prove local hardware identity, wiring, controller/API access, calibration, or
  live telemetry.
- Same-session Tier 2 read-only reviewers approved the named records/profile
  boundary with no P1/P2 blockers.

## Evidence

- ADR:
  `.agents/DECISIONS/ADR-0011-agricultural-telemetry-profile-boundary.md`
- Task record:
  `.agents/TASK_LOG/0226-agricultural-telemetry-profile-boundary.md`
- Handoff:
  `.agents/handoffs/0163-agricultural-telemetry-profile-boundary-to-hardware-communications-qa.md`
- Candidate profiles:
  `hardware-profiles/agricultural/fieldnet-pivot-monitoring/README.md`,
  `hardware-profiles/agricultural/soil-sensing-candidates/README.md`, and
  `hardware-profiles/agricultural/gps-asset-tracking-candidates/README.md`
- Source-index row:
  `SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`
- External source rows:
  `SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`,
  `SRC-METER-TEROS12-2026-05-25`,
  `SRC-SENTEK-DRILLDROP-2026-05-25`,
  `SRC-IRROMETER-SOIL-SENSORS-2026-05-25`, and
  `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`

## Authority Limits

This source record does not accept selected hardware, local possession, wiring,
power/voltage closure, boot-pin closure, isolation closure, connector pinouts,
controller/API integration, protocol frame behavior, telemetry schemas,
firmware dependencies, framework choice, calibration, GPS pivot positioning,
live telemetry proof, release, publication, commit, push, PR, or deploy.

## Open Evidence

- Exact local center-pivot controller, monitoring device, soil probes, adapters,
  loggers, GPS trackers, service subscriptions, API rights, owner authorization,
  power/voltage/current budgets, connectors, isolation, surge protection,
  calibration/units, retention/redaction policy, and live-proof plan.
