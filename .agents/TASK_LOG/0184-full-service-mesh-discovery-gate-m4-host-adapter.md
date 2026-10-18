# Task 0184: Full-Service Mesh Discovery Gate M4 Host Adapter

Status: completed-host-adapter-only; firmware build and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M2A-DOSC-2026-05-27`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`,
`SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-RUNTIME-REQUIREMENTS-2026-05-26`,
`SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-PHASE-5-6-RUNTIME-DESIGN-REVIEW-2026-05-26`,
`SRC-LOCAL-HOST-PROTOCOL-CUSTODY-REVIEW-2026-06-05`,
`SRC-ESP-IDF-WIFI-MESH`,
`SRC-ESP-IDF-RF-COEXIST`,
`SRC-LOCAL-AGENTIC-PLANNING-GUIDE-2026-06-05`

## Routing

- Verified facts: required governance/docs were reread. The development plan
  and triage status named M4 adapter proof as a current safe continuation.
  Gate M3 and handoff 0128 define the M4 checklist. Existing simulator/runtime
  code already maintains `mesh_discovery.v1`, Gate F service-code separation,
  bridge line bounds, and host-only discovery summaries. ADR-0003 accepts
  ESP-IDF for the ESP-NOW BBS lane, but no `firmware/projects/espnow-bbs`
  target exists in this repo and this task does not create one.
- Assumptions: M4 can advance as host-only adapter fixtures because the next
  useful proof is the mapping and invariant checklist, not live hardware or
  firmware runtime behavior. Exact firmware callbacks and ESP-IDF build target
  remain future gates.
- Unknowns: no firmware target/build output, runtime implementation,
  persistence mechanism, live route table, root/parent selection, healing,
  coexistence measurement, BLE/GATT, Android app, router/admin policy, flash,
  serial, cleanup, rollback, or M5 live proof exists.
- Selected tier: Tier 2 host-only adapter/test/docs/records.
- Owner role: Communications with Architecture and QA; Agent Operations owns
  durable task/source/handoff records.
- Evidence need: ADR-0009, M3 design note/task/handoff, Gate F runtime records,
  host protocol custody review, ESP-WIFI-MESH/coexistence source rows, focused
  tests, and read-only reviewer quorum output.
- Mutation boundary:
  `tools/simulators/custom_wireless_protocol/espnow_bbs_mesh_adapter.py`,
  `tools/simulators/custom_wireless_protocol/espnow_bbs_custom_protocol.py`,
  `tests/custom_wireless_protocol/test_espnow_bbs_custom_protocol.py`,
  `docs/architecture/espnow-bbs-mesh-m4-host-adapter-proof.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `comm-protocols/custom/README.md`, `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m4-host-adapter.md`,
  `docs/index.md`, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0132-full-service-mesh-discovery-gate-m4-host-adapter-to-qa-communications-architecture.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Communications/protocol ABI and QA/evidence-record review, waited, and
  closed after output capture. Weighted disposition: 6/6 approved with
  conditions, threshold 70 percent. No P1 blockers remained. P2 conditions
  required the M4 fixture checklist and durable records.
- Gate authority: host-only adapter fixture, tests, docs, source ledger,
  source-index row, status updates, task log, and handoff. No Tier 3 authority
  is opened.
- Validation plan: run focused custom protocol tests, source/record/docs/agent
  audits, `git diff --check`, and record known scaffold-skill/verify limits if
  stale plugin cache paths still block broader verification.
- Trust boundary: local repo source/tests/docs and source-indexed prior records.
  No hardware, serial, RF, router, BLE, firmware, release, or publication
  authority is inferred.

## Implementation

- Added `espnow_bbs_mesh_adapter.py`, a pure host-only fixture that applies
  future firmware-like mesh events into `ProtocolSimulator` discovery records.
- Added compact bridge-summary fallback for maximum discovery snapshot/event
  requests that would otherwise exceed 512 bytes.
- Added M4 tests for all discovery event vectors, maximum bridge line bounds,
  recursive secret marker rejection, live-action rejection through both bridge
  surfaces, queue/custody isolation, and Gate F/discovery ABI invariance.
- Added this task record, M4 architecture note, source ledger, source-index
  row, handoff, and status/doc updates.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 40 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 184`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `git diff --check` passed with exit code 0.

Not claimed for this gate: `scripts/scaffold_audit_skills.py` and
`scripts/verify_scaffold.py` were not run. The QA reviewer noted stale
plugin-cache path risk for broader scaffold verification; this task claims only
the bounded M4 validation listed above.

## Open Evidence

- Exact firmware callback/API mapping is not selected.
- No ESP-IDF `espnow-bbs` target, build output, firmware runtime task, memory
  measurement, NVS/flash persistence, recovery behavior, live mesh route table,
  BLE coexistence proof, Android proof, router/admin policy, PCAP, transcript,
  rollback, or cleanup proof exists.

## Authority Limits

This task does not authorize firmware runtime implementation, firmware
persistence, ESP-IDF project creation, firmware build acceptance, flash, erase,
monitor, serial writes, serial ABI expansion, live ESP-WIFI-MESH, BLE pairing,
Android app behavior, router/admin mutation, PCAP, RF/XBee writes, relay
actions, load/mains work, live bench, release, publication, commit, push, PR,
deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0132-full-service-mesh-discovery-gate-m4-host-adapter-to-qa-communications-architecture.md](../handoffs/0132-full-service-mesh-discovery-gate-m4-host-adapter-to-qa-communications-architecture.md)

## Decision

Decision: accept the bounded Tier 2 M4 host adapter fixture proof. Client-1
static/simulated browser proof is now recorded separately; future mesh work can
continue with a separately gated firmware target/build proof. M5 live mesh
remains Tier 3 only.
