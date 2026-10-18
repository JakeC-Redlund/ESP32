# Task 0206: Full-Service Mesh Discovery Gate M4U Retained Persistence Requirements

Status: accepted-design-only-retained-persistence-requirements

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-ESP-IDF-NVS`

## Routing

- Verified facts: required governance/docs were reread. M4T is accepted
  source-only for caller-owned volatile persistence-intent/accounting over
  accepted M4S, and it keeps retained persistence, NVS/file storage, reboot
  durability, runtime persistence, live bridge dispatch, flash, serial writes,
  and live proof closed.
- Assumptions: a design-only retained-persistence requirements gate can advance
  the persistence lane without adding storage APIs, runtime writes, or live
  proof claims.
- Unknowns: NVS namespace/key policy, record format, versioning, migration,
  corruption handling, power-loss behavior, wear policy, write trigger, commit
  cadence, cleanup/deinit behavior, boot recovery, stack impact, first-flash
  behavior, rollback, live bridge transcript, and live proof remain
  unaccepted.
- Selected tier: Tier 2 docs/status/source-record work.
- Owner role: Communications/Firmware with Architecture, Agent Operations, and
  QA.
- Evidence need: accepted M4T/M4S records, `SRC-ESP-IDF-NVS`, read-only
  reviewer quorum, task/handoff/source records, docs/status alignment, scaffold
  audits, closed-surface scan, and `git diff --check`.
- Mutation boundary:
  `docs/architecture/espnow-bbs-mesh-m4u-retained-persistence-requirements.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4u-retained-persistence-requirements.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`,
  `scripts/scaffold_audit_data.py`, this task record, and
  `.agents/handoffs/0153-full-service-mesh-discovery-gate-m4u-retained-persistence-requirements-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Pre-mutation read-only reviewers
  `019e9dcb-76a4-7490-849e-2c24b2813fa9`
  (Architecture/Communications/Firmware, weight 5) and
  `019e9dcb-9c70-7540-b4a4-a7e5843681ee`
  (QA/Evidence/AgentOps, weight 3) were spawned, waited, captured, and closed.
  Weighted pre-mutation disposition was 8/8 approval with no P1/P2 blockers for
  a design-only retained-persistence requirements boundary.
- Gate authority: requirements/source-boundary planning only. No retained
  persistence implementation, NVS/file writes, live proof, flash, serial,
  release, or publication authority is opened.

## Requirements Recorded

- Future retained-persistence source work must name storage owner,
  namespace/key policy, initialization, deinitialization, recovery, and write
  policy before any storage API is called.
- Persisted data must be bounded, versioned, and limited to compact M4T-derived
  intent/accounting fields.
- Future storage must not retain generated bridge summaries, raw bridge frame
  text, serial bytes, BBS message payloads, credentials, PMK/LMK material, peer
  keys, queue contents, owner claim tokens, runtime task handles, or live
  transport state.
- Missing, corrupt, unsupported-version, or validation-failed records must fail
  closed to empty/volatile behavior and must not trigger live bridge dispatch
  or serial output.
- Future validation must cover corrupt/missing records, version rejection,
  bounded-field limits, non-ASCII rejection, secret exclusion, cleanup/deinit,
  no generated summary retention, no runtime scheduler stepping, no queue/owner
  mutation during recovery, and closed-surface scans.

## Validation

Validation passed:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 206
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
git diff --check
```

Results:

- Record, source, docs, path, data, agent-process, pages, and skills audits
  passed.
- `git diff --check` passed.
- Closed-surface scan found only the explicit denied-marker list in the M4U
  requirements note, not storage API calls or source/runtime openings.
- Final post-review passed 8/8 weighted after the status-ledger P2 was resolved. No P1/P2 blockers remain.

## Open Evidence

Retained persistence implementation, NVS namespace/key policy, file format,
reboot recovery, wear policy, cleanup, runtime timing, live bridge dispatch,
bridge frame parsing, bridge ABI, serial ABI, timer behavior, route table,
root/parent behavior, healing, watchdog behavior, stack adequacy, first-flash
behavior, bridge transcript, Win31/OPCON proof, rollback, and live proof remain
unaccepted.

## Authority Limits

This task does not authorize NVS writes, file writes, retained storage
implementation, reboot durability, firmware persistence acceptance, live bridge
dispatch, bridge frame parsing, firmware runtime acceptance, ESP-NOW runtime,
ESP-WIFI-MESH runtime, BLE pairing, Android app behavior, router/admin
mutation, PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
bridge ABI changes, Gate F service-code changes, `mesh_discovery.v1` schema
changes, Win31 transport changes, RF/XBee writes, relay actions, load/mains
work, live bench, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0153-full-service-mesh-discovery-gate-m4u-retained-persistence-requirements-to-qa-firmware-communications.md](../handoffs/0153-full-service-mesh-discovery-gate-m4u-retained-persistence-requirements-to-qa-firmware-communications.md)

## Decision

M4U is accepted design-only for retained-persistence requirements after local validation and final 8/8 weighted post-review.
