# Task 0201: Full-Service Mesh Discovery Gate M4P Bridge Summary Source Contract

Status: accepted-bridge-summary-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Routing

- Verified facts: required governance/docs were reread. M4O is accepted as a
  no-flash `app_main` startup source gate and leaves bridge work separate.
  Existing `mesh_discovery_contract` state and tests preserve discovery request
  membership, Gate F request/service disjointness, 512-byte ASCII summary
  bounds, secret rejection, and live-action blocking.
- Assumptions: a pure-C local bridge-summary selector can advance the bridge
  contract without accepting a bridge dispatcher, serial path, runtime proof, or
  persistence.
- Unknowns: live bridge dispatch proof, Win31/OPCON bridge request proof,
  firmware runtime acceptance, persistence, timer behavior, route table,
  parent/root behavior, healing behavior, watchdog behavior, stack adequacy,
  first-flash behavior, rollback, cleanup, and live proof remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware with Communications, Architecture, Agent Operations, and
  QA.
- Evidence need: source-indexed M4O/M4C-M4I/M4K/ESP-IDF records, read-only
  reviewer quorum, host C tests, focused and broader audit validation,
  no-flash build, generated-output cleanup, closed-surface scans,
  task/handoff/source records, and final post-review acceptance.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/include/espnow_bbs_mesh_discovery_contract.h`,
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/src/espnow_bbs_mesh_discovery_contract.c`,
  `tests/espnow_bbs_mesh_discovery_contract/test_mesh_discovery_contract.c`,
  `scripts/scaffold_audit_firmware.py`,
  `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4p-bridge-summary-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4p-bridge-summary-source-contract.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0148-full-service-mesh-discovery-gate-m4p-bridge-summary-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Two read-only reviewers were spawned, waited, and closed
  after output capture before mutation. Weighted pre-mutation disposition was
  8/8 conditional approval with no P1/P2 blockers for the named M4P source
  boundary.
- Gate authority: pure-C local bridge-summary selector, focused tests,
  path-aware audits, docs/status records, task log, and handoff. No live proof,
  flash, serial, release, or publication authority is opened.

## Implementation

- Added `espnow_bbs_mesh_contract_bridge_summary_state_t` to carry
  caller-owned queue, owner, and runtime scheduler state pointers.
- Added `espnow_bbs_mesh_contract_bridge_summary` to select bounded local
  summaries for `discovery_snapshot`, `discovery_events`, `service_catalog`,
  and `capability_report`.
- Added host tests for known request selection, request rejection paths,
  undersized output rejection, 512-byte ASCII bounds, missing state rejection,
  and non-mutation of queue, owner, and runtime scheduler state.
- Added scaffold firmware audit markers for the new source boundary.

## Validation

Validation passed.

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 201
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4p-bridge-summary-source-contract build
find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

Results:

- Focused scaffold unit tests passed: 15 tests.
- Host contract tests passed.
- Firmware, path, records, sources, docs, agent-process, and full scaffold
  validation passed.
- No-flash ESP-IDF v6.0.1 build passed; generated
  `/tmp/esp32-espnow-bbs-m4p-bridge-summary-source-contract/espnow_bbs_build_only.bin`
  size `0x22d20`, leaving `0xdd2e0` bytes free in the 1 MiB app partition.
- Generated `firmware/projects/espnow-bbs/sdkconfig` was removed and the final
  `find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print`
  cleanup check had no output.
- Closed-surface scan found only the expected blocked-live-action
  `serial_write` string in the contract rejection table.
- `git diff --check` passed.
- Final post-review passed 8/8 weighted conditional approval. The sole P2
  record-alignment condition was resolved by adding M4P to project/status
  records; no P1/P2 blockers remain for the named source-only boundary.

## Open Evidence

- Live bridge dispatch, firmware runtime behavior, persistence, timer behavior,
  route table, root/parent behavior, healing, watchdog behavior, stack
  adequacy, first-flash behavior, bridge transcript, Win31/OPCON proof,
  rollback, cleanup, and live proof remain unaccepted.

## Authority Limits

This task does not authorize live bridge dispatch, firmware persistence,
ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app behavior,
router/admin mutation, PCAP, flash, erase, monitor, serial writes, serial ABI
expansion, bridge ABI changes, Gate F service-code changes,
`mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
relay actions, load/mains work, live bench, release, publication, commit, push,
PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0148-full-service-mesh-discovery-gate-m4p-bridge-summary-source-contract-to-qa-firmware-communications.md](../handoffs/0148-full-service-mesh-discovery-gate-m4p-bridge-summary-source-contract-to-qa-firmware-communications.md)

## Decision

M4P is accepted as a source-only bridge-summary contract gate after full
validation and final post-review. Live bridge dispatch, persistence, firmware
runtime acceptance, first-flash proof, and M5 live mesh remain separate gates.
