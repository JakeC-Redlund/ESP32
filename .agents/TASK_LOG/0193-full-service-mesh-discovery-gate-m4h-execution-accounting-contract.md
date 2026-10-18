# Task 0193: Full-Service Mesh Discovery Gate M4H Execution Accounting Contract

Status: completed-execution-accounting-contract-only; firmware runtime and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
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

- Verified facts: required governance/docs were reread. M4G accepted only a
  caller-owned scheduler integration contract over the M4F dispatch plan
  inside the inert pure-C `mesh_discovery_contract` component. `ADR-0009` and
  prior M4 gates keep `mesh_discovery.v1`, Gate F services, bridge ABI,
  coordinator serial ABI, and Win31 transport unchanged.
- Assumptions: a caller-owned execution-accounting contract is the correct
  next source slice because it validates local apply/skip accounting over a
  staged plan before firmware runtime scheduling, task creation, persistence,
  bridge dispatch, or live APIs are introduced.
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
  `docs/architecture/espnow-bbs-mesh-m4h-execution-accounting-contract.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m4h-execution-accounting-contract.md`,
  `docs/index.md`, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0140-full-service-mesh-discovery-gate-m4h-execution-accounting-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Firmware/Communications/Architecture and QA review, waited, and closed after
  output capture. Weighted disposition was 8/8 approval with no P1/P2 blockers
  to starting the named mutation boundary.
- Gate authority: inert execution-accounting contract source, no-flash build
  proof, audits, docs, source ledger, source-index row, status updates, task
  log, and handoff. No Tier 3 authority is opened.
- Validation plan: run host C tests, focused scaffold unittest checks,
  no-flash ESP-IDF build, firmware/path/custom-protocol/scaffold record/source/
  docs/agent-process audits, full scaffold verification, closed runtime marker
  scans, generated-output checks, and `git diff --check`.
- Trust boundary: local repo source/tests/docs, source-indexed prior records,
  local ESP-IDF v6.0.1 toolchain output, and read-only reviewer output. No
  hardware, serial, RF, router, BLE, release, or publication authority is
  inferred.

## Implementation

- Extended `mesh_discovery_contract` with a caller-owned
  `espnow_bbs_mesh_contract_execution_accounting_t`.
- Added a 64-cycle local execution-accounting bound.
- Added explicit execution-accounting rejection statuses for zero cycle,
  over-limit cycle, empty plan, non-ready planned step, and mixed owner.
- Added `execution_accounting_apply_plan`, which validates a staged M4F
  dispatch plan, requires ready steps to have one accepted owner, rejects
  non-ready planned steps, records apply/skip and ready-action counters in
  caller-owned execution-accounting state, and returns no runtime handle or
  live action.
- Added compact execution-accounting summaries:
  `schema=mesh_discovery.v1;acct=...;owner=...;last_cycle=...;apply=...;skip=...;drain=...;ingest=...;snap=...;status=...`.
- Expanded host C tests for summary bounds, deterministic balanced-plan
  accounting, repeated cycles, no owner/queue/scheduler/plan mutation, null
  rejection, zero/over-limit cycle rejection, empty plan rejection, all-skipped
  plan rejection, non-ready planned-step rejection, mixed-owner rejection,
  partial backpressure accounting, reset, and Gate F/bridge/serial ABI
  invariance.
- Expanded recursive firmware-audit markers for the M4H execution-accounting
  API and closed runtime markers including static FreeRTOS objects,
  callback/hook/handler markers, file/persistence markers, and BLE/NimBLE
  markers.
- Added this task record, M4H architecture note, source ledger, source-index
  row, handoff, and status/doc updates.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed: `PASS: espnow_bbs_mesh_discovery_contract host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4h-execution-accounting-contract build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550`
  bytes free in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`.
- `test ! -e build/tests/espnow_bbs_mesh_discovery_contract` passed.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for allocation, FreeRTOS task/queue/timer/handle,
  callback/hook/handler, file/persistence, BLE/NimBLE, logging, Wi-Fi,
  ESP-NOW, mesh, NVS, UART, GPIO, HTTP, partition, flash, OTA, or selected
  runtime API markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 193`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.
- `git diff --check` passed with exit code 0.
- Post-implementation read-only reviewers were closed after output capture.
  Firmware/Communications/Architecture voted PASS with no P1/P2 findings.
  QA/evidence identified one P2 stale pending-record wording issue; this final
  record alignment remediates that stale wording without expanding authority.

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
changes, `mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee
writes, relay actions, load/mains work, live bench, release, publication,
commit, push, PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict`
mutation.

## Handoff

[../handoffs/0140-full-service-mesh-discovery-gate-m4h-execution-accounting-contract-to-qa-firmware-communications.md](../handoffs/0140-full-service-mesh-discovery-gate-m4h-execution-accounting-contract-to-qa-firmware-communications.md)

## Decision

Decision: accept the bounded Tier 2 M4H execution-accounting contract proof.
Future actual task creation, runtime scheduler implementation, firmware
runtime implementation, first-flash proof, and M5 live mesh remain separate
gates.
