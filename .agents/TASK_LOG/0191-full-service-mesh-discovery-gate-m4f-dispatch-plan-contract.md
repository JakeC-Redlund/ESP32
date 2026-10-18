# Task 0191: Full-Service Mesh Discovery Gate M4F Dispatch Plan Contract

Status: completed-dispatch-plan-contract-only; firmware runtime and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
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

- Verified facts: required governance/docs were reread. M4E accepted only a
  caller-owned owner contract over the M4D event queue inside the inert pure-C
  `mesh_discovery_contract` component. `ADR-0009` and prior M4 gates keep
  `mesh_discovery.v1`, Gate F services, bridge ABI, coordinator serial ABI,
  and Win31 transport unchanged.
- Assumptions: a caller-owned dispatch-plan contract is the correct next source
  slice because it validates deterministic action ordering and budget
  accounting before firmware scheduler integration, task creation, persistence,
  bridge dispatch, or live APIs are introduced.
- Unknowns: no firmware runtime implementation, FreeRTOS task graph, scheduler
  integration, memory budget acceptance, persistence, first-flash target,
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
  `docs/architecture/espnow-bbs-mesh-m4f-dispatch-plan-contract.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m4f-dispatch-plan-contract.md`,
  `docs/index.md`, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0138-full-service-mesh-discovery-gate-m4f-dispatch-plan-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Firmware/Communications/Architecture and QA review, waited, and closed after
  output capture. Weighted disposition was 8/8 approval with no P1/P2 blockers
  to starting the named mutation boundary. Reviewer conditions required
  caller-owned pure-C dispatch planning only, no actual task creation/startup,
  no handler calls, deterministic owner/action order, budget/bounds tests, no
  queue or owner mutation while planning, no ABI/schema drift, no tracked
  generated output, and no Tier 3 authority.
- Gate authority: inert dispatch-plan contract source, no-flash build proof,
  audits, docs, source ledger, source-index row, status updates, task log, and
  handoff. No Tier 3 authority is opened.
- Validation plan: run host C tests, focused scaffold unittest checks, no-flash
  ESP-IDF build, firmware/path/custom-protocol/scaffold record/source/docs/
  agent-process audits, full scaffold verification, closed runtime marker
  scans, generated-output checks, and `git diff --check`.
- Trust boundary: local repo source/tests/docs, source-indexed prior records,
  local ESP-IDF v6.0.1 toolchain output, and read-only reviewer output. No
  hardware, serial, RF, router, BLE, release, or publication authority is
  inferred.

## Implementation

- Extended `mesh_discovery_contract` with a caller-owned
  `espnow_bbs_mesh_contract_dispatch_plan_t` and fixed-size plan-step records.
- Added three inert dispatch action names: `owner_drain`, `owner_ingest`, and
  `owner_snapshot`, with compact local labels `od`, `oi`, and `os`.
- Added an 8-step plan capacity, a 16-byte dispatch action-name cap, and a
  per-action budget cap of 16 units.
- Added explicit dispatch rejection statuses for unknown action,
  over-limit action name, full plan, zero budget, and too-large budget.
- Added stage helpers that validate known owners/actions and record requested,
  planned, rejected, and per-step status values in caller-owned memory only.
- Added `dispatch_plan_from_owner`, which builds a deterministic local plan in
  drain, ingest, snapshot order from a claimed owner and its caller-owned
  queue without popping, pushing, transmitting, starting tasks, scheduling
  handlers, mutating bridge state, or changing owner counters.
- Added compact dispatch summaries:
  `schema=mesh_discovery.v1;plan=...;req=...;ready=...;reject=...;first=...;last=...;status=...`.
- Expanded host C tests for dispatch vocabulary, null/non-ASCII/over-limit/
  unknown rejection, zero/too-large/planned-over-requested budgets, fixed
  capacity, deterministic owner plan order, queue-count and owner-counter
  non-mutation, full/empty queue backpressure accounting, stale/unbound/
  unclaimed owner rejection, repeated summary determinism, exact/small/null
  summary capacity, and Gate F/bridge/serial ABI invariance.
- Expanded recursive firmware-audit markers for the M4F dispatch API and tests.
- Added this task record, M4F architecture note, source ledger, source-index
  row, handoff, and status/doc updates.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed: `PASS: espnow_bbs_mesh_discovery_contract host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4f-dispatch-plan build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550`
  bytes free in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`.
- Direct generated-output checks found no repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` generated path.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for allocation, FreeRTOS task/queue/timer, callback,
  logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP, partition, flash,
  OTA, or selected runtime API markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 191`
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
- Initial post-implementation read-only reviewer quorum was closed after output
  capture. Firmware/Communications/Architecture voted PASS with no P1/P2
  findings. QA/evidence found one P2 validation-record inconsistency because
  durable records still said final validation was pending after the validation
  commands had passed. This task update records the completed evidence packet;
  focused QA re-review was then spawned, waited, and closed after output
  capture. QA/evidence voted PASS with no P1/P2 findings and confirmed the P2
  stale-record finding was cleared. Weighted final disposition was 8/8 approval
  for the named M4F dispatch-plan contract boundary.

## Open Evidence

- Runtime firmware implementation, actual scheduler integration, task creation,
  memory budget acceptance, persistence, first-flash target, recovery path,
  live route table, root/parent selection, healing, coexistence, BLE/GATT,
  Android, router/admin policy, PCAP, transcript, rollback, cleanup, and live
  proof remain unaccepted.

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

[../handoffs/0138-full-service-mesh-discovery-gate-m4f-dispatch-plan-contract-to-qa-firmware-communications.md](../handoffs/0138-full-service-mesh-discovery-gate-m4f-dispatch-plan-contract-to-qa-firmware-communications.md)

## Decision

Decision: accept the bounded Tier 2 M4F dispatch-plan contract proof. Future
scheduler integration, actual task creation, firmware runtime implementation,
first-flash proof, and M5 live mesh remain separate gates.
