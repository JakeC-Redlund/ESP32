# Task 0187: Full-Service Mesh Discovery Gate M4B Build-Only Target

Status: completed-build-only-target; firmware runtime and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-START-PROJECT`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`,
`SRC-ESP-IDF-WIFI-MESH`,
`SRC-ESP-IDF-RF-COEXIST`

## Routing

- Verified facts: required governance/docs were reread. `ADR-0003` accepts
  ESP-IDF stable v6.0.1 only for the `espnow-bbs` firmware lane. M4 accepted
  only host adapter fixture proof and explicitly left ESP-IDF target/build
  proof open. Local ESP-IDF v6.0.1 activation and `idf.py` paths exist.
- Assumptions: a minimal inert target is the correct next proof because it
  validates project shape and local build tooling before runtime APIs,
  persistence, radio behavior, or live proof are introduced.
- Unknowns: no firmware runtime implementation, task graph, memory budget,
  queue ownership, persistence, first-flash target, recovery path, live route
  table, root/parent selection, healing, coexistence, BLE/GATT, Android app,
  router/admin policy, PCAP, transcript, rollback, cleanup, or live proof is
  accepted.
- Selected tier: Tier 2 firmware target/build-only proof, audit, docs, and
  records.
- Owner role: Firmware with Communications, Architecture, and QA.
- Evidence need: accepted ADR/source IDs, read-only reviewer output, local
  no-flash ESP-IDF build output, focused audit tests, scaffold audits, source
  ledger, status updates, and handoff.
- Mutation boundary:
  `firmware/projects/espnow-bbs/README.md`,
  `firmware/projects/espnow-bbs/CMakeLists.txt`,
  `firmware/projects/espnow-bbs/main/CMakeLists.txt`,
  `firmware/projects/espnow-bbs/main/main.c`,
  `scripts/scaffold_audit_data.py`, `scripts/scaffold_audit_firmware.py`,
  `tests/scaffold_audits/test_espnow_bbs_build_only_target.py`,
  `docs/architecture/espnow-bbs-mesh-m4b-build-only-target-proof.md`,
  `docs/architecture/espnow-bbs-mesh-m4-host-adapter-proof.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m4b-build-only-target.md`,
  `docs/index.md`, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0134-full-service-mesh-discovery-gate-m4b-build-only-target-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Firmware/Communications/Architecture and QA review, waited, and closed after
  output capture. Initial disposition: Firmware/Communications/Architecture
  conditional pass; QA failed current acceptance because the target, build
  output, audit coverage, and durable records did not yet exist. This task
  resolves those named blockers inside the approved build-only boundary.
- Gate authority: build-only ESP-IDF target, no-flash build proof, audits,
  docs, source ledger, source-index row, status updates, task log, and handoff.
  No Tier 3 authority is opened.
- Validation plan: run the no-flash ESP-IDF build, focused target audit tests,
  firmware/path/custom-protocol/scaffold record/source/docs/agent-process
  audits, full scaffold verification, closed-surface scans, and `git diff
  --check`.
- Trust boundary: local repo source/tests/docs, source-indexed prior records,
  local ESP-IDF v6.0.1 toolchain output, and read-only reviewer output. No
  hardware, serial, RF, router, BLE, release, or publication authority is
  inferred.

## Implementation

- Added `firmware/projects/espnow-bbs/`, a minimal ESP-IDF project with an
  inert no-op `app_main`.
- Added project README facts, assumptions, unknowns, hard gates, and validation
  boundary.
- Updated scaffold path allowlists for the new project CMake files.
- Added a firmware audit that requires build-only markers and rejects runtime
  API markers in the new target source.
- Added a focused scaffold-audit regression test for the build-only target.
- Added this task record, M4B architecture note, source ledger, source-index
  row, handoff, and status/doc updates.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-build-only build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21aa0`, and `0xde560`
  bytes free in the smallest app partition.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 42 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 187`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- Runtime API marker scan over `firmware/projects/espnow-bbs` returned no
  matches for Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO-output, HTTP, or
  partition-erase runtime markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.
- `git diff --check` passed with exit code 0.
- Stale target/build-future wording scan over `research`, `docs`,
  `.agents/TASK_LOG`, and `knowledge-base/source-ledger` returned no matches.

## Open Evidence

- Runtime firmware implementation, queue/task ownership, memory budget,
  persistence, first-flash target, recovery path, live route table, root/parent
  selection, healing, coexistence, BLE/GATT, Android, router/admin policy,
  PCAP, transcript, rollback, cleanup, and live proof remain unaccepted.

## Authority Limits

This task does not authorize firmware runtime implementation, firmware
persistence, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, flash, erase, monitor, serial writes,
serial ABI expansion, bridge ABI changes, Gate F service-code changes,
`mesh_discovery.v1` schema changes, RF/XBee writes, relay actions, load/mains
work, live bench, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0134-full-service-mesh-discovery-gate-m4b-build-only-target-to-qa-firmware-communications.md](../handoffs/0134-full-service-mesh-discovery-gate-m4b-build-only-target-to-qa-firmware-communications.md)

## Decision

Decision: accept the bounded Tier 2 M4B build-only target proof. Future
runtime implementation, first-flash proof, and M5 live mesh remain separate
gates.
