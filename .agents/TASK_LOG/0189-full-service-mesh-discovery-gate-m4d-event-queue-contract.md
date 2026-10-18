# Task 0189: Full-Service Mesh Discovery Gate M4D Event Queue Contract

Status: completed-event-queue-contract-only; firmware runtime and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
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

- Verified facts: required governance/docs were reread. M4C accepted an inert
  pure-C firmware contract component linked into the build-only ESP-IDF target.
  `ADR-0009` keeps `mesh_discovery.v1` bounded to ASCII 512-byte bridge
  summaries, accepted event/request vocabulary, recursive secret-field
  rejection, and unchanged Gate F service codes.
- Assumptions: a caller-owned fixed-size event queue contract is the correct
  next source slice because it validates ingress/read behavior before runtime
  task ownership, persistence, bridge dispatch, or live APIs are introduced.
- Unknowns: no firmware runtime implementation, task graph, memory budget
  acceptance, scheduler ownership, persistence, first-flash target, recovery
  path, live route table, root/parent selection, healing, coexistence, BLE/GATT,
  Android app, router/admin policy, PCAP, transcript, rollback, cleanup, or
  live proof is accepted.
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
  `docs/architecture/espnow-bbs-mesh-m4d-event-queue-contract.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m4d-event-queue-contract.md`,
  `docs/index.md`, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0136-full-service-mesh-discovery-gate-m4d-event-queue-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Firmware/Communications/Architecture and QA review, waited, and closed after
  output capture. Weighted disposition was 8/8 approval with no P1/P2 blockers
  to starting the named mutation boundary. Reviewer conditions required
  caller-owned fixed-size C structs only, explicit capacity/overflow policy,
  host C tests for FIFO/full/empty/wraparound/input rejection/summary bounds,
  expanded forbidden runtime/allocation marker coverage, no ABI/schema drift,
  no tracked generated output, and no Tier 3 authority.
- Gate authority: inert firmware event queue contract source, no-flash build
  proof, audits, docs, source ledger, source-index row, status updates, task
  log, and handoff. No Tier 3 authority is opened.
- Validation plan: run host C tests, focused scaffold unittest checks,
  no-flash ESP-IDF build, firmware/path/custom-protocol/scaffold
  record/source/docs/agent-process audits, full scaffold verification, closed
  runtime marker scans, generated-output checks, and `git diff --check`.
- Trust boundary: local repo source/tests/docs, source-indexed prior records,
  local ESP-IDF v6.0.1 toolchain output, and read-only reviewer output. No
  hardware, serial, RF, router, BLE, release, or publication authority is
  inferred.

## Implementation

- Extended `mesh_discovery_contract` with a caller-owned
  `espnow_bbs_mesh_contract_event_queue_t` and fixed-size event record/input
  structs.
- Added constants for 16 records, 16-byte event-name storage, 8-byte node IDs,
  and 32-byte event detail strings.
- Added queue init/count/dropped/push/peek/pop/summary helpers.
- Added explicit rejection statuses for node/detail limits, queue full, queue
  empty, and insufficient summary output capacity.
- Full queues increment an explicit dropped counter and reject without
  overwriting or silently dropping records.
- Queue summaries are compact local snapshots of schema, count, dropped count,
  oldest event label/node, and newest event label/node.
- Expanded host C tests for caller-owned state, all 12 event types, FIFO order,
  full rejection/backpressure, empty pop/peek, wraparound/reuse, reset, unknown
  event rejection, null inputs, non-ASCII event/node/detail rejection,
  over-limit node/detail rejection, exact caller summary capacity, small
  capacity rejection, recursive secret-field rejection, and Gate F invariance.
- Expanded recursive firmware-audit coverage for allocation, task, queue,
  timer, callback, logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP,
  partition, flash, and OTA runtime markers.
- Added this task record, M4D architecture note, source ledger, source-index
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
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4d-event-queue build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550`
  bytes free in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 189`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for allocation, FreeRTOS task/queue/timer, callback,
  logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP, partition, flash,
  OTA, or selected runtime API markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.
- `git diff --check` passed with exit code 0.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`.
- The repo-local `build/tests/espnow_bbs_mesh_discovery_contract` generated
  path is absent; host C test binaries are generated under
  `/tmp/esp32-espnow-bbs-mesh-discovery-contract-host-tests`.
- Post-implementation read-only reviewer quorum was closed after output
  capture. Firmware/Communications/Architecture voted PASS, QA/evidence voted
  PASS with no P1/P2 blockers, and weighted disposition was 8/8 approval for
  the named M4D event queue contract boundary.

## Open Evidence

- Runtime firmware implementation, task/scheduler ownership, memory budget
  acceptance, persistence, first-flash target, recovery path, live route table,
  root/parent selection, healing, coexistence, BLE/GATT, Android, router/admin
  policy, PCAP, transcript, rollback, cleanup, and live proof remain
  unaccepted.

## Authority Limits

This task does not authorize firmware runtime implementation, firmware
persistence, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, flash, erase, monitor, serial writes,
serial ABI expansion, bridge ABI changes, Gate F service-code changes,
`mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
relay actions, load/mains work, live bench, release, publication, commit,
push, PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0136-full-service-mesh-discovery-gate-m4d-event-queue-contract-to-qa-firmware-communications.md](../handoffs/0136-full-service-mesh-discovery-gate-m4d-event-queue-contract-to-qa-firmware-communications.md)

## Decision

Decision: accept the bounded Tier 2 M4D event queue contract proof. Future
runtime implementation, task ownership, first-flash proof, and M5 live mesh
remain separate gates.
