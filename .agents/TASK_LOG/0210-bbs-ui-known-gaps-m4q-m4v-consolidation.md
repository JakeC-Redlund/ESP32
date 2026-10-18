# Task 0210: BBS UI Known-Gaps M4Q-M4V Consolidation

Status: accepted-records-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`

## Routing

- Verified facts: `research/development-status-ledger.md` and
  `research/triage-status.md` already aggregate the BBS UI system operation
  program through M4Q, M4R, M4S, M4T, M4U, M4V, and Client-1 host-only proof.
  `research/known-gaps.md` still had the main BBS UI row stopping at M4P plus
  Client-1, followed by a separate M4V retained-persistence source addendum.
- Assumptions: folding the accepted M4Q-M4V facts into the main known-gaps row
  is records-only consolidation and does not create new source evidence or
  alter firmware, bridge, serial, browser, hardware, live bench, release, or
  publication behavior.
- Unknowns: copied screenshot/OCR/CV corroboration for later UI changes, live
  bridge dispatch, live NVS execution proof, reboot durability, runtime
  persistence proof, firmware runtime proof, first-flash proof, live mesh proof,
  future Client-2 selected-board proof, flash, serial writes, and
  release/publication authority remain unverified.
- Selected tier: Tier 2 records/evidence consolidation.
- Owner role: Agent Operations/QA with Architecture/Communications/Firmware.
- Evidence need: current known-gaps rows, existing source IDs, existing
  aggregate status/triage rows, read-only reviewer quorum output, this task
  record, targeted scans, and scaffold audit output.
- Mutation boundary: `research/known-gaps.md` and this task record only.
- Reviewer quorum: read-only Architecture/Communications/Firmware reviewer
  weight 5 and read-only QA/Evidence/AgentOps reviewer weight 3 were spawned
  and waited before mutation. Both approved with no P1/P2 blockers inside the
  named records-only boundary. Weighted reviewer vote: 8/8 approve with
  conditions; coordinator gate owner weight 5 accepted the same boundary.
- Agent lifecycle cleanup: no listable completed-agent inventory was available
  in the exposed toolset before spawn. Both spawned reviewers were waited and
  closed before mutation.
- Gate authority: records-only consolidation. No source-index/source-ledger,
  docs-index, firmware, bridge, serial, browser runtime, hardware, hook/config,
  live bench, flash, RF/XBee, release, publication, commit, push, PR, or deploy
  authority is opened.

## Implementation

- Folded accepted M4Q local request-router, M4R bridge-dispatch envelope, M4S
  request-text intent, M4T volatile persistence-intent, M4U design-only
  retained-persistence requirements, and M4V source-only retained-persistence
  adapter facts into the main BBS UI system operation row in
  `research/known-gaps.md`.
- Removed the adjacent `M4V retained-persistence source addendum` row after the
  main BBS UI row carried the M4V source ID and closed live/runtime/release
  surfaces directly.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 210`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py` passed:
  `PASS: ESP32 skill audit succeeded`.
- Targeted addendum-row removal scan
  `rg -n "M4V retained-persistence source addendum" research/known-gaps.md`
  returned no matches, as expected.
- Targeted M4Q/M4R/M4S/M4T/M4U/M4V retention scan confirmed those labels and
  source IDs remain in `research/known-gaps.md` and this task record.
- Scoped `git diff --check -- research/known-gaps.md .agents/TASK_LOG/0210-bbs-ui-known-gaps-m4q-m4v-consolidation.md`
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

No handoff required; this is a records-only consolidation.

## Decision

Decision: accept the BBS UI known-gaps M4Q-M4V consolidation. The BBS UI
known-gaps row now carries M4Q-M4V directly, while live/runtime/hardware and
release surfaces remain separate future gates.
