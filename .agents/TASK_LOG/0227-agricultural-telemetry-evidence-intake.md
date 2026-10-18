# Task 0227: Agricultural Telemetry Evidence Intake

Status: accepted-evidence-intake-template-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-EVIDENCE-INTAKE-2026-06-07`,
`SRC-LOCAL-AGRICULTURAL-TELEMETRY-PROFILE-BOUNDARY-2026-06-07`,
`SRC-LINDSAY-FIELDNET-PIVOT-MONITOR-2026-05-25`,
`SRC-METER-TEROS12-2026-05-25`,
`SRC-SENTEK-DRILLDROP-2026-05-25`,
`SRC-IRROMETER-SOIL-SENSORS-2026-05-25`,
and `SRC-GEOTAB-ASSET-TRACKING-2026-05-25`.

## Routing

- Verified facts: ADR-0011 and Task 0226 accept only candidate agricultural
  telemetry class planning profiles and taxonomy/protocol-family boundaries.
  They require future source-backed or same-session evidence packets before any
  implementation.
- Assumptions: A reusable evidence-intake packet improves owner review routing
  without claiming exact local equipment, service/API access, wiring,
  calibration, protocol frame behavior, schemas, firmware, or live proof.
- Unknowns/open evidence: exact center-pivot controller, soil probes, adapters,
  loggers, GPS trackers, service subscriptions, API rights, owner
  authorization, power, voltage, current budget, boot-pin impact, isolation,
  connectors, field enclosure constraints, surge exposure, protocol/API
  details, calibration, units, retention/redaction policy, GPS pivot
  positioning, live telemetry proof, release, publication, commit, push, PR,
  and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 records/docs/status evidence-intake template.
- Owner role: Hardware with Communications, Architecture, Agent Operations, and
  QA/Evidence.
- Evidence need: existing agricultural source rows, ADR-0011, Task 0226,
  current agricultural candidate profiles, read-only reviewer quorum, new
  evidence-intake packet, this task record, matching handoff, source-ledger
  row, source-index row, docs-index reachability, status/gap updates, diff
  hygiene, source/docs/agent-process/records audits, closed-surface scan, and
  scaffold validation.
- Mutation boundary: `hardware-profiles/agricultural/evidence-intake/README.md`,
  `hardware-profiles/agricultural/fieldnet-pivot-monitoring/README.md`,
  `hardware-profiles/agricultural/soil-sensing-candidates/README.md`,
  `hardware-profiles/agricultural/gps-asset-tracking-candidates/README.md`,
  `.agents/TASK_LOG/0227-agricultural-telemetry-evidence-intake.md`,
  `.agents/handoffs/0164-agricultural-telemetry-evidence-intake-to-hardware-communications-qa.md`,
  `knowledge-base/source-ledger/2026-06-07-agricultural-telemetry-evidence-intake.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: scoped `git diff --check`, durable-record audit from Task
  0227, source audit, docs audit, agent-process audit, changed-file source-ID
  scan, closed-surface scan, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was waited, captured, and closed before mutation:

- Hardware/Communications/Evidence reviewer
  `019e9f91-2394-7520-bcc4-56b1b21ea3a6`, weight 3, approved the named Tier 2
  evidence-intake packet definition scope with no P1/P2 blockers and P3
  wording constraints to keep packet language evidence-required/template-only,
  use existing agricultural source IDs, keep GPS pivot positioning unresolved,
  and avoid calibration or live-proof acceptance.
- QA/Records/Process reviewer
  `019e9f91-2495-7fb3-b079-4745b88b4a42`, weight 7, approved the named Tier 2
  records-only evidence-intake packet gate with no P1/P2 blockers and P3
  requirements for durable records, separated Verified Facts/Assumptions/
  Unknowns, preserved ADR-0011 stop gates, docs-index reachability, and
  lifecycle wording.

Weighted reviewer result: 10/10 approval with no P1/P2 blockers. No mutating
worker agents were spawned. No listable completed-agent inventory was available
in the exposed toolset before spawn; both spawned reviewers were waited and
closed before mutation. Under `bypassPermissions`, repo hooks remain advisory
and are not runtime cleanup proof.

## Implementation

- Added an agricultural telemetry evidence-intake packet template covering lane
  identity, equipment/service identity, evidence provenance, power/voltage,
  isolation, connector/API boundary, protocol details, calibration/units, data
  policy, and live-proof criteria.
- Added lane-specific addenda for center-pivot monitoring, soil sensing, and
  GPS asset tracking.
- Added a local source-index row, source ledger, task record, handoff, and
  docs-index links for the evidence-intake template.
- Updated the three agricultural candidate profiles and current plan/status/gap
  rows to route future work through the filled evidence packet before
  implementation.

## Validation

- `git diff --check -- hardware-profiles/agricultural/evidence-intake/README.md hardware-profiles/agricultural/fieldnet-pivot-monitoring/README.md hardware-profiles/agricultural/soil-sensing-candidates/README.md hardware-profiles/agricultural/gps-asset-tracking-candidates/README.md .agents/TASK_LOG/0227-agricultural-telemetry-evidence-intake.md .agents/handoffs/0164-agricultural-telemetry-evidence-intake-to-hardware-communications-qa.md knowledge-base/source-ledger/2026-06-07-agricultural-telemetry-evidence-intake.md knowledge-base/source-index.md docs/index.md research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 227`
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
- Closed-surface scan was reviewed; matches are expected authority-limit and
  do-not-do language in the new files, not new hardware, firmware, API, live,
  release, publication, or git authority.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept exact local equipment or service identity, selected
hardware, local possession, wiring, connector pinout acceptance,
controller/API integration, protocol frame behavior, telemetry schemas,
firmware dependencies, framework choice, calibration acceptance, GPS pivot
positioning, live telemetry proof, release, publication, commit, push, PR, or
deploy.

## Handoff

Handoff:
[../handoffs/0164-agricultural-telemetry-evidence-intake-to-hardware-communications-qa.md](../handoffs/0164-agricultural-telemetry-evidence-intake-to-hardware-communications-qa.md)

## Decision

Decision: accept an agricultural telemetry evidence-intake packet template and
status routing for future exact equipment/service evidence collection. Exact
local equipment, protocol/API details, calibration, implementation, live proof,
release, and publication remain separate future gates.
