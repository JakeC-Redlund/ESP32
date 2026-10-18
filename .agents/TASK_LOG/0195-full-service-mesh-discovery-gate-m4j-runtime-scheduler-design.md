# Task 0195: Full-Service Mesh Discovery Gate M4J Runtime Scheduler Design

Status: accepted-design-only; runtime scheduler source and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`,
`SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`,
`SRC-ESP-IDF-ESP-TIMER-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-PHASE-5-6-RUNTIME-DESIGN-REVIEW-2026-05-26`,
`SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-RUNTIME-REQUIREMENTS-2026-05-26`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`

## Routing

- Verified facts: required governance/docs were reread. M4I accepts only a
  caller-owned runtime-cycle contract and keeps actual scheduler runtime,
  task/timer code, firmware runtime, persistence, bridge dispatch, flash,
  serial, live mesh, BLE, router/admin, release, and publication closed.
- Assumptions: a design-only runtime scheduler review is the correct next
  movement before source implementation because task topology, timer mechanism,
  stack/priority/affinity, callback handoff, cleanup, memory budget, and
  recovery are still unknown.
- Unknowns: no runtime scheduler source, ESP-IDF task/timer code, static
  runtime state, callback integration, bridge dispatch, persistence,
  first-flash target, live mesh route table, BLE/GATT, Android app,
  router/admin policy, PCAP, transcript, cleanup, rollback, or live proof is
  accepted.
- Selected tier: Tier 2 design/docs/source-record/status work.
- Owner role: Firmware with Communications, Architecture, and QA.
- Evidence need: source-indexed ESP-IDF FreeRTOS/ESP Timer docs, source-indexed
  prior ADR/gate records, read-only reviewer output, durable records, status
  updates, scaffold audits, stale-wording scan, and diff hygiene.
- Mutation boundary:
  `docs/architecture/espnow-bbs-mesh-m4j-runtime-scheduler-design.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4j-runtime-scheduler-design.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0142-full-service-mesh-discovery-gate-m4j-runtime-scheduler-design-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Firmware/Communications/Architecture and QA review, waited, and closed after
  output capture. Weighted disposition was 8/8 approval with no P1/P2 blockers
  to starting the named design-only mutation boundary.
- Post-implementation reviewer quorum: after validation, read-only
  Firmware/Communications/Architecture and QA reviewers were spawned, waited,
  and closed after output capture. Weighted disposition was 8/8 approval with
  no P1/P2 blockers for final M4J design-only acceptance.
- Gate authority: design note, source ledger, source-index rows, status
  updates, task log, and handoff. No firmware source, task/timer code, no-flash
  build, live bench, flash, serial, release, or publication authority is opened.
- Validation plan: run record/source/docs/agent-process audits, full scaffold
  verification, targeted source-ID/stale-wording/closed-surface scans,
  generated-output checks, `git diff --check`, and post-mutation reviewer
  quorum.
- Trust boundary: local repo source/tests/docs, source-indexed prior records,
  official Espressif ESP-IDF v6.0.1 documentation, and read-only reviewer
  output. No hardware, serial, RF, router, BLE, release, or publication
  authority is inferred.

## Implementation

- Added a source-backed M4J runtime scheduler design note.
- Added source-index rows for ESP-IDF FreeRTOS and ESP Timer scheduler/timer
  design facts.
- Recorded requirements for any later runtime scheduler source gate to name
  task ownership, core affinity, priority, stack budget, timer choice, callback
  dispatch model, caller-owned versus static state, bounded backpressure,
  bridge-visible counters, watchdog/yield behavior, volatile-only state,
  cleanup/deinit behavior, and validation commands.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 195`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`:
  exit 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`:
  exit 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`: PASS.
- `git diff --check`: exit 0.
- `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print`: empty.
- `test ! -e build/tests/espnow_bbs_mesh_discovery_contract`: exit 0.
- Targeted source-ID, stale-wording, and closed-surface scans confirmed the
  final M4J source IDs, accepted design-only status, and closed firmware/live
  gates.
- Final post-review approved M4J 8/8 with no P1/P2 blockers.

## Open Evidence

- Runtime scheduler source implementation, actual task creation, timer
  creation, static runtime state, memory budget acceptance, bridge dispatch,
  persistence, first-flash target, recovery path, live route table,
  root/parent selection, healing, coexistence, BLE/GATT, Android, router/admin
  policy, PCAP, transcript, rollback, cleanup, and live proof remain
  unaccepted.

## Authority Limits

This task does not authorize firmware source implementation, ESP-IDF FreeRTOS
task creation, ESP Timer creation, callbacks, static runtime queues, firmware
persistence, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, flash, erase, monitor, serial writes,
serial ABI expansion, bridge ABI changes, Gate F service-code changes,
`mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
relay actions, load/mains work, live bench, release, publication, commit,
push, PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0142-full-service-mesh-discovery-gate-m4j-runtime-scheduler-design-to-qa-firmware-communications.md](../handoffs/0142-full-service-mesh-discovery-gate-m4j-runtime-scheduler-design-to-qa-firmware-communications.md)

## Decision

Decision: accept the bounded Tier 2 M4J runtime scheduler design review as a
design-only requirements packet. Future runtime scheduler source
implementation, ESP-IDF task/timer code, first-flash proof, and M5 live mesh
remain separate gates.
