# Task 0209: BBS UI M4U/M4V Aggregate Ledger Consolidation

Status: accepted-records-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`

## Routing

- Verified facts: `research/development-plan.md` already records the BBS UI
  system operation program through M4U/M4V and Client-1 host-only proof. The
  status ledger and triage status each still carried a separate M4U/M4V or M4V
  addendum row next to the main BBS UI aggregate row.
- Assumptions: folding those addendum facts into the aggregate rows is a
  records-only consolidation and does not create new source evidence or change
  firmware, bridge, serial, browser, hardware, or release behavior.
- Unknowns: live NVS execution proof, reboot durability, runtime persistence
  proof, live bridge dispatch, first-flash proof, Client-2 selected-board proof,
  and M5 live mesh proof remain unverified.
- Selected tier: Tier 2 records/evidence consolidation.
- Owner role: Agent Operations/QA with Architecture/Communications/Firmware.
- Evidence need: existing M4S/M4T/M4U/M4V source IDs, current aggregate and
  addendum rows, read-only reviewer quorum output, updated status records, this
  task record, targeted scans, and scaffold audit output.
- Mutation boundary: `research/development-status-ledger.md`,
  `research/triage-status.md`, and this task record only.
- Reviewer quorum: read-only Architecture/Communications/Firmware reviewer
  weight 5 and read-only QA/Evidence/AgentOps reviewer weight 3 were spawned,
  waited, captured, and closed before mutation; coordinator gate owner weight 5
  completed local triage. Weighted vote: 13/13 approve within the named
  records-only boundary, with no P1/P2 blockers after conditions are satisfied.
- Agent lifecycle cleanup: completed reviewers were closed before mutation.
- Gate authority: records-only consolidation. No source-index/source-ledger,
  docs-index, firmware, bridge, serial, browser runtime, hardware, hook/config,
  live bench, flash, RF/XBee, release, publication, commit, push, PR, or deploy
  authority is opened.

## Implementation

- Folded the M4U retained-persistence requirements and M4V retained-persistence
  source-only adapter facts into the main BBS UI system operation row in
  `research/development-status-ledger.md`.
- Removed the adjacent `BBS UI system operation M4S/M4T/M4U/M4V aggregate
  addendum` row after the aggregate row carried M4U/M4V source IDs and closed
  surfaces directly.
- Folded the M4V source-only retained-persistence adapter fact into the main
  BBS UI system operation row in `research/triage-status.md`.
- Removed the adjacent `BBS UI system operation M4V addendum` row after the
  aggregate triage row carried the M4V source ID and closed surfaces directly.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 209`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- Targeted addendum-row removal scan
  `rg -n "BBS UI system operation M4S/M4T/M4U/M4V aggregate addendum|BBS UI system operation M4V addendum" research/development-status-ledger.md research/triage-status.md`
  returned no matches.
- Targeted M4U/M4V source-ID retention scan confirmed the source IDs remain in
  `research/development-status-ledger.md`, `research/triage-status.md`, and
  this task record.
- Scoped `git diff --check -- research/development-status-ledger.md research/triage-status.md .agents/TASK_LOG/0209-bbs-ui-m4uv-aggregate-ledger-consolidation.md`
  passed with exit code 0.

## Authority Limits

This task does not authorize live NVS execution proof, reboot durability,
runtime persistence proof, live bridge dispatch, first-flash proof, live mesh,
Client-2 selected-board proof, browser/device live proof, firmware runtime,
bridge ABI changes, serial ABI changes, Gate F service-code changes,
`mesh_discovery.v1` schema changes, flash, monitor, serial writes, RF/XBee
writes, relay/load/mains work, release, publication, commit, push, PR, deploy,
source-index/source-ledger creation, docs-index updates, hook changes, config
changes, `/etc/codex/requirements.toml`, or `admin-strict`.

## Handoff

No handoff required; this is a records-only consolidation and validation is
recorded in this task.

## Decision

Decision: accept the BBS UI M4U/M4V aggregate ledger consolidation. The BBS UI
aggregate rows now carry M4U/M4V directly, while live and release surfaces
remain separate future gates.
