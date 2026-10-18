# Task 0202: Full-Service Mesh Discovery Gate M4Q Local Bridge Request Router Source Contract

Status: accepted-local-bridge-request-router-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Routing

- Verified facts: required governance/docs were reread. M4P is accepted as a
  pure-C local bridge-summary source contract and leaves live bridge dispatch,
  persistence, runtime proof, first-flash proof, and M5 live mesh as separate
  gates. The existing discovery request vocabulary is exactly
  `discovery_snapshot`, `discovery_events`, `service_catalog`, and
  `capability_report`.
- Assumptions: a caller-owned local request-router/accounting API over the M4P
  bridge-summary selector can advance bridge-adjacent source work without
  opening live bridge dispatch, serial ABI expansion, persistence, timers, or
  protocol/schema changes.
- Unknowns: live bridge dispatch proof, Win31/OPCON bridge request proof,
  firmware runtime acceptance, persistence model, timer behavior, route table,
  parent/root behavior, healing behavior, watchdog behavior, stack adequacy,
  first-flash behavior, rollback, cleanup, and live proof remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware with Communications, Architecture, Agent Operations, and
  QA.
- Evidence need: source-indexed M4P/M4O/M4C-M4E/M4K/ESP-IDF records,
  read-only reviewer quorum, host C tests, focused and broader audit
  validation, no-flash build, generated-output cleanup, closed-surface scan,
  task/handoff/source records, and final post-review acceptance.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/include/espnow_bbs_mesh_discovery_contract.h`,
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/src/espnow_bbs_mesh_discovery_contract.c`,
  `tests/espnow_bbs_mesh_discovery_contract/test_mesh_discovery_contract.c`,
  `scripts/scaffold_audit_firmware.py`,
  `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4q-local-bridge-request-router-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4q-local-bridge-request-router-source-contract.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0149-full-service-mesh-discovery-gate-m4q-local-bridge-request-router-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Two read-only reviewers were spawned, waited, and closed
  after output capture before mutation. Weighted pre-mutation disposition was
  8/8 conditional approval with no P1/P2 blockers for the named M4Q source
  boundary.
- Gate authority: pure-C caller-owned local request router/accounting over M4P,
  focused tests, path-aware audits, docs/status records, task log, and handoff.
  No live proof, flash, serial, release, or publication authority is opened.

## Implementation

- Added `espnow_bbs_mesh_contract_bridge_request_router_t` as caller-owned
  local router/accounting state.
- Added init/reset, route, and summary APIs for the local request router.
- `espnow_bbs_mesh_contract_bridge_request_route` accepts only the existing
  discovery request names by exact match, routes to the accepted M4P
  `espnow_bbs_mesh_contract_bridge_summary`, and records deterministic
  attempted/accepted/rejected/last-status accounting in caller-owned router
  state.
- Added host tests for accepted request routing, router summary bounds, null
  router behavior, invalid request/status paths, accounting determinism, and
  non-mutation of queue, owner, and runtime scheduler state.
- Added scaffold firmware audit markers for the new source boundary.

## Validation

Completed validation:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 202
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4q-local-bridge-request-router-source-contract build
find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
rg -n "bridge dispatch|serial_write|uart_|nvs_|esp_timer|xTimer|xQueue|esp_wifi|esp_now|esp_mesh|esp_ble|esp_bt|httpd_|esp_partition|esp_flash|esp_ota|fopen|fwrite|printf|ESP_LOG" firmware/projects/espnow-bbs/components/mesh_discovery_contract firmware/projects/espnow-bbs/main/main.c
git diff --check
```

Results:

- Focused scaffold unit tests passed: 15 tests.
- Host contract tests passed.
- Firmware, path, records, sources, docs, agent-process, pages, React Native,
  and skills audits passed.
- The aggregate `scripts/verify_scaffold.py` command was attempted twice and
  entered kernel I/O wait on `/mnt/h`; it was stopped both times. The
  individual audit groups covered by the aggregate command were run separately
  and passed, and those individual results are the scaffold acceptance
  evidence.
- No-flash ESP-IDF v6.0.1 build passed; generated
  `/tmp/esp32-espnow-bbs-m4q-local-bridge-request-router-source-contract/espnow_bbs_build_only.bin`
  size `0x22d20`, leaving `0xdd2e0` bytes free in the 1 MiB app partition.
- Generated `sdkconfig*` cleanup check produced no files, and
  `build/tests/espnow_bbs_mesh_discovery_contract` was absent.
- Closed-surface scan found only the expected blocked-action table string
  `serial_write`.
- `git diff --check` passed.
- Final post-review passed 8/8 weighted conditional approval after the
  record-alignment P2 was resolved by this record/status update. No P1/P2
  blockers remain for the named source-only M4Q gate.

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

[../handoffs/0149-full-service-mesh-discovery-gate-m4q-local-bridge-request-router-source-contract-to-qa-firmware-communications.md](../handoffs/0149-full-service-mesh-discovery-gate-m4q-local-bridge-request-router-source-contract-to-qa-firmware-communications.md)

## Decision

M4Q is accepted only as a pure-C caller-owned local bridge request
router/accounting source contract over the accepted M4P bridge-summary
selector. Live bridge dispatch, persistence, firmware runtime acceptance,
first-flash proof, and M5 live mesh remain separate gates.
