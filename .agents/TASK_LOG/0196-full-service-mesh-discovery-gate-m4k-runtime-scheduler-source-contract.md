# Task 0196: Full-Service Mesh Discovery Gate M4K Runtime Scheduler Source Contract

Status: accepted-runtime-scheduler-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`,
`SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`,
`SRC-ESP-IDF-ESP-TIMER-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`

## Routing

- Verified facts: required governance/docs were reread. M4J accepts only
  source-backed runtime scheduler requirements. M4I accepts only caller-owned
  runtime-cycle composition. Current source remains pure C and host-testable.
- Assumptions: a caller-invoked runtime scheduler source contract is the safe
  next movement because it names the scheduler owner/configuration while
  keeping actual task/timer ownership closed.
- Unknowns: no actual task graph, timer lifecycle, callback dispatch, static
  runtime queues, bridge dispatch, persistence, first-flash target, live mesh,
  BLE/GATT, Android, router/admin, PCAP, transcript, rollback, cleanup, or live
  proof is accepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware with Communications, Architecture, and QA.
- Evidence need: source-indexed M4J/M4I/ESP-IDF records, read-only reviewer
  quorum, host C tests, scaffold audits, no-flash build if available,
  generated-output cleanup, closed-surface scans, task/handoff/source records,
  and post-review.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`,
  `tests/espnow_bbs_mesh_discovery_contract/test_mesh_discovery_contract.c`,
  `scripts/scaffold_audit_firmware.py`,
  `docs/architecture/espnow-bbs-mesh-m4k-runtime-scheduler-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4k-runtime-scheduler-source-contract.md`,
  `knowledge-base/source-index.md`, docs/status files, this task record, and
  `.agents/handoffs/0143-full-service-mesh-discovery-gate-m4k-runtime-scheduler-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Two read-only reviewers were spawned, waited, and closed after
  output capture before mutation. Weighted pre-mutation disposition was 8/8
  approval with no P1/P2 blockers for the named boundary.
- Gate authority: source contract, host tests, audit markers, docs/status
  records, task log, and handoff. No task/timer creation, live bench, flash,
  serial, release, or publication authority is opened.

## Implementation

- Added caller-owned `espnow_bbs_mesh_contract_runtime_scheduler_t` state and
  `espnow_bbs_mesh_contract_runtime_scheduler_config_t` configuration.
- Added config validation for `caller_invoked`, `task_none`, `timer_none`,
  period/yield bounds, no selected core, no priority, no stack budget, and no
  static state.
- Added caller-invoked step composition over M4I runtime-cycle state with
  rejected-step atomicity.
- Added compact runtime scheduler summaries.
- Added host C tests for config bounds, summary bounds, deterministic caller
  steps, owner/queue non-mutation, and rejected configure/step atomicity.
- Expanded scaffold audit markers for the new M4K API/source/test markers.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`:
  PASS, 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 196`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`:
  PASS.
- No-flash ESP-IDF v6.0.1 build passed with
  `idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4k-runtime-scheduler-source-contract build`;
  `espnow_bbs_build_only.bin` size was `0x21ab0` with `0xde550` bytes free in
  the smallest app partition.
- Generated `sdkconfig` output was removed; `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print`
  returned no output.
- `test ! -e build/tests/espnow_bbs_mesh_discovery_contract`: PASS.
- Targeted closed-surface scan: PASS, no forbidden runtime markers found in
  the M4K component, firmware main, or firmware CMake files.
- `git diff --check`: PASS.
- Post-review quorum: 8/8 source-boundary approval with no P1/P2 implementation
  blockers; stale-record P2 findings are resolved by the M4K acceptance record
  updates.

## Open Evidence

- Actual task creation, timer creation, callback dispatch, static runtime
  queues, bridge dispatch, persistence, first-flash target, recovery path, live
  route table, root/parent selection, healing, coexistence, BLE/GATT, Android,
  router/admin policy, PCAP, transcript, rollback, cleanup, and live proof
  remain unaccepted.

## Authority Limits

This task does not authorize actual FreeRTOS task creation, timer creation,
callbacks, static runtime queues, firmware persistence, ESP-NOW runtime,
ESP-WIFI-MESH runtime, BLE pairing, Android app behavior, router/admin
mutation, PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
bridge ABI changes, Gate F service-code changes, `mesh_discovery.v1` schema
changes, Win31 transport changes, RF/XBee writes, relay actions,
load/mains work, live bench, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0143-full-service-mesh-discovery-gate-m4k-runtime-scheduler-source-contract-to-qa-firmware-communications.md](../handoffs/0143-full-service-mesh-discovery-gate-m4k-runtime-scheduler-source-contract-to-qa-firmware-communications.md)

## Decision

Accepted as a caller-owned runtime scheduler source contract only. Actual task
creation, timer creation, callback implementation, firmware runtime,
persistence, bridge dispatch, flash/serial/live proof, release, publication,
commit, push, PR, and deployment remain closed.
