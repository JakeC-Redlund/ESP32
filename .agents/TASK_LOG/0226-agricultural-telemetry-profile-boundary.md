# Task 0226: Agricultural Telemetry Profile Boundary

Status: accepted-design-only-profile-boundary

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`,
`SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`,
`SRC-METER-TEROS12-2026-05-25`,
`SRC-SENTEK-DRILLDROP-2026-05-25`,
`SRC-IRROMETER-SOIL-SENSORS-2026-05-25`,
and `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`.

## Routing

- Verified facts: Current records classify agricultural telemetry, pivot, soil,
  and GPS planning as design-only. The five source-index rows are candidate
  source coverage only and explicitly do not verify local hardware possession,
  wiring, controller/API access, calibration, or integration rights.
- Assumptions: Candidate telemetry class planning profiles and a design-only ADR
  improve owner review routing without selecting hardware or authorizing
  implementation.
- Unknowns/open evidence: exact center-pivot controller, soil probes, adapters,
  loggers, GPS trackers, service subscriptions, API rights, power, voltage,
  boot-pin impact, isolation, connectors, protocol frame behavior, calibration,
  units, retention/redaction policy, GPS pivot positioning, firmware
  dependencies, live telemetry proof, release, publication, commit, push, PR,
  and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 records/profile-boundary and design-only ADR.
- Owner role: Hardware with Communications, Architecture, Agent Operations, and
  QA/Evidence.
- Evidence need: existing source-index rows, current protocol-brief blockers,
  current plan/status/gap rows, same-session read-only reviewer quorum,
  candidate profile docs, ADR-0011, docs-index reachability, this task record,
  matching handoff, source-ledger row, diff hygiene, source/docs/agent-process/
  records audits, closed-surface scan, and scaffold validation.
- Mutation boundary: `hardware-profiles/agricultural/fieldnet-pivot-monitoring/README.md`,
  `hardware-profiles/agricultural/soil-sensing-candidates/README.md`,
  `hardware-profiles/agricultural/gps-asset-tracking-candidates/README.md`,
  `.agents/DECISIONS/ADR-0011-agricultural-telemetry-profile-boundary.md`,
  `.agents/TASK_LOG/0226-agricultural-telemetry-profile-boundary.md`,
  `.agents/handoffs/0163-agricultural-telemetry-profile-boundary-to-hardware-communications-qa.md`,
  `knowledge-base/source-ledger/2026-06-07-agricultural-telemetry-profile-boundary.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`,
  `docs/projects/espnow-bbs/custom-wireless-protocol-brief.md`, and
  `comm-protocols/custom/README.md`.
- Validation plan: scoped `git diff --check`, durable-record audit from Task
  0226, source audit, docs audit, agent-process audit, changed-file source-ID
  scan, closed-surface scan, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was waited, captured, and closed before mutation:

- Hardware/Communications/Evidence reviewer
  `019e9f81-912f-7ab3-8e1d-4f36425f1464`, weight 3, approved the named Tier 2
  records/profile-boundary gate with no P1/P2 blockers and P3 conditions to
  keep wording candidate-only, mark GPS pivot positioning unresolved, keep
  schemas/API/firmware/live work closed, and add direct docs-index links.
- QA/Evidence reviewer `019e9f81-cb93-7be1-9883-6ea8154ecd33`, weight 3,
  approved the named Tier 2 records/design-only mutation with no P1/P2 blockers
  and P3 conditions for source-separated profile docs, boundary-only ADR
  wording, docs-index reachability, and source-index coverage.

Weighted reviewer result: 6/6 approval with no P1/P2 blockers. No mutating
worker agents were spawned. No listable completed-agent inventory was available
in the exposed toolset before spawn; both spawned reviewers were waited and
closed before mutation.

## Implementation

- Added three candidate telemetry class planning profiles for center-pivot
  monitoring, soil sensing, and GPS asset tracking.
- Added ADR-0011 accepting only the agricultural telemetry taxonomy and
  protocol-family boundary for future owner review.
- Added a local source-index row and source ledger for this records/profile
  boundary.
- Updated current plan, status ledger, triage status, known gaps, docs index,
  and protocol wording to show that candidate planning profiles exist while
  selected hardware, schemas, API integration, wiring, firmware, and live proof
  remain closed.

## Validation

- `git diff --check -- hardware-profiles/agricultural/fieldnet-pivot-monitoring/README.md hardware-profiles/agricultural/soil-sensing-candidates/README.md hardware-profiles/agricultural/gps-asset-tracking-candidates/README.md .agents/DECISIONS/ADR-0011-agricultural-telemetry-profile-boundary.md .agents/TASK_LOG/0226-agricultural-telemetry-profile-boundary.md .agents/handoffs/0163-agricultural-telemetry-profile-boundary-to-hardware-communications-qa.md knowledge-base/source-ledger/2026-06-07-agricultural-telemetry-profile-boundary.md knowledge-base/source-index.md docs/index.md research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md docs/projects/espnow-bbs/custom-wireless-protocol-brief.md comm-protocols/custom/README.md`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 226`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- Changed-file source-ID scan was reviewed after the source audit; the source
  audit confirmed retained source IDs resolve through
  `knowledge-base/source-index.md`.
- Closed-surface scan was reviewed; matches are expected closed/unauthorized
  authority-limit language and existing status rows, not new hardware,
  firmware, API, live, release, publication, or git authority.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept selected hardware, local possession, wiring,
controller/API integration, protocol frame behavior, telemetry schemas,
firmware dependencies, framework choice, power/voltage closure, boot-pin
closure, isolation closure, connector pinouts, calibration, GPS pivot
positioning, live telemetry proof, release, publication, commit, push, PR, or
deploy.

## Handoff

Handoff:
[../handoffs/0163-agricultural-telemetry-profile-boundary-to-hardware-communications-qa.md](../handoffs/0163-agricultural-telemetry-profile-boundary-to-hardware-communications-qa.md)

## Decision

Decision: accept agricultural telemetry candidate class planning profiles and
ADR-0011's design-only taxonomy/protocol-family boundary. Exact local hardware,
service/API rights, electrical evidence, schemas, implementation, live proof,
release, and publication remain separate future gates.
