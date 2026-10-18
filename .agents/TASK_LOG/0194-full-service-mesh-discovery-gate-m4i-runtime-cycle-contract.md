# Task 0194: Full-Service Mesh Discovery Gate M4I Runtime Cycle Contract

Status: accepted-runtime-cycle-contract-only; firmware runtime and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-START-PROJECT`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Routing

- Verified facts: required governance/docs were reread. M4H accepted only a
  caller-owned execution-accounting contract over a staged M4F dispatch plan.
  Current status records named a separately gated runtime scheduler source
  slice as the next safe mesh-discovery continuation.
- Assumptions: a caller-owned runtime-cycle composition contract is the correct
  next source slice because it validates scheduler/accounting composition
  before firmware runtime scheduling, task creation, persistence, bridge
  dispatch, or live APIs are introduced.
- Unknowns: no firmware runtime implementation, FreeRTOS task graph, actual
  scheduler loop, memory budget acceptance, persistence, first-flash target,
  recovery path, live route table, root/parent selection, healing,
  coexistence, BLE/GATT, Android app, router/admin policy, PCAP, transcript,
  rollback, cleanup, or live proof is accepted.
- Selected tier: Tier 2 firmware interface contract source, audit, docs, and
  records.
- Owner role: Firmware with Communications, Architecture, and QA.
- Evidence need: accepted ADR/source IDs, read-only reviewer output, host C
  tests, local no-flash ESP-IDF build output, focused audit tests, scaffold
  audits, source ledger, status updates, and handoff.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`,
  `tests/espnow_bbs_mesh_discovery_contract/`,
  `scripts/scaffold_audit_firmware.py`,
  `docs/architecture/espnow-bbs-mesh-m4i-runtime-cycle-contract.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m4i-runtime-cycle-contract.md`,
  `docs/index.md`, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0141-full-service-mesh-discovery-gate-m4i-runtime-cycle-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Firmware/Communications/Architecture and QA review, waited, and closed after
  output capture. Weighted disposition was 8/8 approval with no P1/P2 blockers
  to starting the named mutation boundary.
- Gate authority: inert runtime-cycle contract source, no-flash build proof,
  audits, docs, source ledger, source-index row, status updates, task log, and
  handoff. No Tier 3 authority is opened.
- Validation plan: run host C tests, focused scaffold unittest checks,
  no-flash ESP-IDF build, firmware/path/custom-protocol/scaffold record/source/
  docs/agent-process audits, full scaffold verification, closed runtime marker
  scans, generated-output checks, and `git diff --check`.
- Trust boundary: local repo source/tests/docs, source-indexed prior records,
  local ESP-IDF v6.0.1 toolchain output, and read-only reviewer output. No
  hardware, serial, RF, router, BLE, release, or publication authority is
  inferred.

## Implementation

- Extended `mesh_discovery_contract` with caller-owned
  `espnow_bbs_mesh_contract_runtime_cycle_t`.
- Added a 64-cycle local runtime-cycle bound.
- Added explicit runtime-cycle rejection statuses for zero and over-limit
  cycles.
- Added `runtime_cycle_step`, which composes scheduler staging and execution
  accounting through temporary local copies, commits caller-owned scheduler,
  dispatch-plan, execution-accounting, and runtime-cycle state only after a
  full cycle succeeds, and leaves caller-owned state unchanged on rejected
  cycles/profile/owner inputs.
- Added compact runtime-cycle summaries:
  `schema=mesh_discovery.v1;runtime=...;profile=...;owner=...;last_cycle=...;sched=...;apply=...;skip=...;reject=...;status=...`.
- Expanded host C tests for summary bounds, deterministic balanced/drain-first
  cycles, no owner/queue mutation, atomic rejection behavior, null rejection,
  full-queue backpressure accounting, reset, and Gate F/bridge/serial ABI
  invariance.
- Expanded recursive firmware-audit markers for the M4I runtime-cycle API and
  host tests.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed: `PASS: espnow_bbs_mesh_discovery_contract host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 194`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4i-runtime-cycle-contract build`
  passed without flash/monitor/serial action. The build generated
  `espnow_bbs_build_only.bin` size `0x21ab0`; the smallest app partition had
  `0xde550` bytes free.
- Generated-output cleanup removed `firmware/projects/espnow-bbs/sdkconfig`;
  no `sdkconfig*` files remained under `firmware/projects/espnow-bbs`, and no
  host-test build output remained under `build/tests/espnow_bbs_mesh_discovery_contract`.
- Narrowed closed-runtime marker scans over tracked firmware source/CMake files
  found no framework runtime API, allocation, task/queue/timer,
  callback/hook/handler, persistence, logging, or live-control markers. The
  broader scan only found existing closed-surface service-label strings
  `ble_pair` and `flash`.
- `git diff --check` passed before and after final record alignment.

## Post-Implementation Review

- Visible lifecycle state was not listable in the available tools before
  spawning post-reviewers. The prior reviewers had already been waited and
  closed.
- Firmware/Communications/Architecture reviewer, weight 5: conditional pass
  with no source-boundary P1/P2 findings; required stale record alignment.
- QA/evidence reviewer, weight 3: conditional pass with no evidence-structure
  P1 findings; required stale interim wording remediation and final
  record/source/docs/diff audits.
- Weighted disposition: 8/8 approval after stale-record P2 remediation. No
  P1/P2 blockers remain for the named M4I runtime-cycle contract boundary.

## Open Evidence

- Runtime firmware implementation, actual scheduler loop, task creation, memory
  budget acceptance, persistence, first-flash target, recovery path, live route
  table, root/parent selection, healing, coexistence, BLE/GATT, Android,
  router/admin policy, PCAP, transcript, rollback, cleanup, and live proof
  remain unaccepted.

## Authority Limits

This task does not authorize firmware runtime implementation, actual FreeRTOS
task creation, static runtime queues, dispatch loops, callbacks, handler calls,
firmware persistence, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing,
Android app behavior, router/admin mutation, PCAP, flash, erase, monitor,
serial writes, serial ABI expansion, bridge ABI changes, Gate F service-code
changes, `mesh_discovery.v1` schema changes, Win31 transport changes,
RF/XBee writes, relay actions, load/mains work, live bench, release,
publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
`admin-strict` mutation.

## Handoff

[../handoffs/0141-full-service-mesh-discovery-gate-m4i-runtime-cycle-contract-to-qa-firmware-communications.md](../handoffs/0141-full-service-mesh-discovery-gate-m4i-runtime-cycle-contract-to-qa-firmware-communications.md)

## Decision

Decision: accept the bounded Tier 2 M4I runtime-cycle contract proof only.
Future actual task creation, runtime scheduler implementation, firmware runtime
implementation, first-flash proof, and M5 live mesh remain separate gates.
