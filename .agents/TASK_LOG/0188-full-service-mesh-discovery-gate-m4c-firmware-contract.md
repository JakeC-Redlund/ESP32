# Task 0188: Full-Service Mesh Discovery Gate M4C Firmware Contract

Status: completed-firmware-contract-only; firmware runtime and live mesh remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
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

- Verified facts: required governance/docs were reread. `ADR-0003` accepts
  ESP-IDF stable v6.0.1 only for the `espnow-bbs` firmware lane. M4 accepted
  host-only adapter fixture proof. M4B accepted an inert ESP-IDF target and
  no-flash local build proof. `ADR-0009` keeps `mesh_discovery.v1` host-only
  and bounded to ASCII 512-byte bridge summaries.
- Assumptions: a pure-C firmware contract component is the correct next source
  slice because it validates constants and helper contracts before runtime
  task ownership, queues, persistence, or live APIs are introduced.
- Unknowns: no firmware runtime implementation, task graph, memory budget,
  queue ownership, persistence, first-flash target, recovery path, live route
  table, root/parent selection, healing, coexistence, BLE/GATT, Android app,
  router/admin policy, PCAP, transcript, rollback, cleanup, or live proof is
  accepted.
- Selected tier: Tier 2 firmware contract source, audit, docs, and records.
- Owner role: Firmware with Communications, Architecture, and QA.
- Evidence need: accepted ADR/source IDs, read-only reviewer output, host C
  tests, local no-flash ESP-IDF build output, focused audit tests, scaffold
  audits, source ledger, status updates, and handoff.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`,
  `firmware/projects/espnow-bbs/main/CMakeLists.txt`,
  `firmware/projects/espnow-bbs/main/main.c`,
  `firmware/projects/espnow-bbs/README.md`,
  `tests/espnow_bbs_mesh_discovery_contract/`,
  `tests/scaffold_audits/test_espnow_bbs_mesh_discovery_contract.py`,
  `scripts/scaffold_audit_data.py`, `scripts/scaffold_audit_firmware.py`,
  `docs/architecture/espnow-bbs-mesh-m4c-firmware-contract.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m4c-firmware-contract.md`,
  `docs/index.md`, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0135-full-service-mesh-discovery-gate-m4c-firmware-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Read-only project-local subagents were spawned for
  Firmware/Communications/Architecture and QA review, waited, and closed after
  output capture. Weighted disposition was 8/8 approval with no P1/P2 blockers
  to starting the named mutation boundary. Reviewer conditions required pure-C
  contract-only implementation, host C tests with `-Werror`, recursive
  forbidden-runtime marker coverage, drift checks against existing
  `mesh_discovery.v1`/Gate F invariants, no tracked generated output, and
  no Tier 3 authority.
- Gate authority: inert firmware contract component, no-flash build proof,
  audits, docs, source ledger, source-index row, status updates, task log, and
  handoff. No Tier 3 authority is opened.
- Validation plan: run host C tests, focused scaffold unittest checks,
  no-flash ESP-IDF build, firmware/path/custom-protocol/scaffold
  record/source/docs/agent-process audits, full scaffold verification, closed
  runtime marker scans, generated-output checks, and `git diff --check`.
- Trust boundary: local repo source/tests/docs, source-indexed prior records,
  local ESP-IDF v6.0.1 toolchain output, and read-only reviewer output. No
  hardware, serial, RF, router, BLE, release, or publication authority is
  inferred.

## Implementation

- Added `mesh_discovery_contract`, a pure-C component that mirrors the
  accepted `mesh_discovery.v1` vocabulary and bounded summary constraints.
- Linked the component into the existing inert `espnow-bbs` ESP-IDF target by
  calling `espnow_bbs_mesh_contract_event_count()` from `app_main`.
- Added host C tests for schema/defaults, event labels, request vocabulary,
  512-byte ASCII summary bounds, recursive field-path secret rejection, Gate F
  service/bridge invariance, and blocked live-action names.
- Added recursive firmware-audit coverage for `firmware/projects/espnow-bbs`.
- Added this task record, M4C architecture note, source ledger, source-index
  row, handoff, and status/doc updates.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed: `PASS: espnow_bbs_mesh_discovery_contract host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with exit code 0.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4c-contract build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550`
  bytes free in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 188`
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
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`.
- The host C test output directory
  `/tmp/esp32-espnow-bbs-mesh-discovery-contract-host-tests` is outside the
  repository, and the earlier repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` generated directory was
  removed after QA review.

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
`mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
relay actions, load/mains work, live bench, release, publication, commit,
push, PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0135-full-service-mesh-discovery-gate-m4c-firmware-contract-to-qa-firmware-communications.md](../handoffs/0135-full-service-mesh-discovery-gate-m4c-firmware-contract-to-qa-firmware-communications.md)

## Decision

Decision: accept the bounded Tier 2 M4C firmware contract proof. Future
runtime implementation, first-flash proof, and M5 live mesh remain separate
gates.
