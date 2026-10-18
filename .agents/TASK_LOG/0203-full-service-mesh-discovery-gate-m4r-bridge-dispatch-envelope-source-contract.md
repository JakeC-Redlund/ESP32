# Task 0203: Full-Service Mesh Discovery Gate M4R Bridge Dispatch Envelope Source Contract

Status: accepted-bridge-dispatch-envelope-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`,
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

- Verified facts: required governance/docs were reread. M4Q is accepted as a
  pure-C caller-owned local bridge request router/accounting source contract
  over M4P. M4Q accepts only `discovery_snapshot`, `discovery_events`,
  `service_catalog`, and `capability_report`, and leaves live bridge dispatch,
  persistence, runtime proof, first-flash proof, and M5 live mesh as separate
  gates.
- Assumptions: a caller-owned local dispatch-envelope/accounting layer over
  M4Q can advance bridge-dispatch source readiness without parsing bridge
  frames, writing serial, expanding ABI, scheduling runtime work, or mutating
  persistent state.
- Unknowns: live bridge dispatch proof, Win31/OPCON bridge transcript proof,
  bridge frame parsing, serial ABI, bridge ABI, firmware runtime acceptance,
  persistence, timer behavior, route table, parent/root behavior, healing
  behavior, watchdog behavior, stack adequacy, first-flash behavior, rollback,
  cleanup, and live proof remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware with Communications, Architecture, Agent Operations, and
  QA.
- Evidence need: accepted M4Q/M4P/M4O/M4C-M4E/M4K/ESP-IDF records,
  read-only reviewer quorum, host C tests, focused and broader audit
  validation, no-flash build, generated-output cleanup, closed-surface scan,
  task/handoff/source records, and final post-review acceptance.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/include/espnow_bbs_mesh_discovery_contract.h`,
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/src/espnow_bbs_mesh_discovery_contract.c`,
  `tests/espnow_bbs_mesh_discovery_contract/test_mesh_discovery_contract.c`,
  `scripts/scaffold_audit_firmware.py`,
  `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4r-bridge-dispatch-envelope-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4r-bridge-dispatch-envelope-source-contract.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0150-full-service-mesh-discovery-gate-m4r-bridge-dispatch-envelope-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Two read-only reviewers were spawned, waited, and closed
  after output capture before mutation. Weighted pre-mutation disposition was
  8/8 conditional approval with no P1/P2 blockers for the named M4R source
  boundary.
- Gate authority: pure-C caller-owned local dispatch-envelope/accounting over
  M4Q, focused tests, path-aware audits, docs/status records, task log, and
  handoff. No live proof, flash, serial, release, or publication authority is
  opened.

## Implementation

- Added `espnow_bbs_mesh_contract_bridge_dispatch_envelope_t` as caller-owned
  local dispatch-envelope/accounting state.
- Added init/reset, route, and summary APIs for the local dispatch envelope.
- `espnow_bbs_mesh_contract_bridge_dispatch_envelope_route` calls the accepted
  M4Q `espnow_bbs_mesh_contract_bridge_request_route`, records
  attempted/would-dispatch/rejected/last-status accounting in caller-owned
  envelope state, and does not parse frames or write serial.
- Added host tests for accepted request routing through M4Q, null envelope,
  null router, null/non-ASCII/unknown request rejection, undersized output,
  missing caller-owned summary state, deterministic accounting, 512-byte bounds,
  and non-mutation of queue, owner, and runtime scheduler state.
- Added scaffold firmware audit markers for the new source boundary.

## Validation

Completed validation:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 203
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4r-bridge-dispatch-envelope-source-contract build
find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
rg -n "bridge dispatch|serial_write|uart_|nvs_|esp_timer|xTimer|xQueue|esp_wifi|esp_now|esp_mesh|esp_ble|esp_bt|httpd_|esp_partition|esp_flash|esp_ota|fopen|fwrite|printf|ESP_LOG" firmware/projects/espnow-bbs/components/mesh_discovery_contract firmware/projects/espnow-bbs/main/main.c
git diff --check
```

Results:

- Host contract tests passed.
- Firmware audit passed.
- Focused mesh discovery scaffold unittest passed: 2 tests.
- Broader selected scaffold unittest suite passed: 15 tests.
- Record, source, docs, agent-process, pages, React Native, skills, and path
  audits passed.
- No-flash ESP-IDF v6.0.1 build passed and generated
  `espnow_bbs_build_only.bin` size `0x22d20`; the 1 MiB app partition had
  `0xdd2e0` bytes free.
- Generated `sdkconfig*` files were removed from
  `firmware/projects/espnow-bbs/`.
- `build/tests/espnow_bbs_mesh_discovery_contract` was absent after host test
  cleanup.
- Closed-surface scan matched only the existing denied-token data string
  `"serial_write"` in
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/src/espnow_bbs_mesh_discovery_contract.c`.
- `git diff --check` passed.
- Final read-only reviewers found no source-code P1/P2 blocker but rejected
  final acceptance until stale pending record wording was removed. This
  acceptance update resolves that record-alignment P2.

## Open Evidence

- Live bridge dispatch, bridge frame parsing, firmware runtime behavior,
  persistence, timer behavior, route table, root/parent behavior, healing,
  watchdog behavior, stack adequacy, first-flash behavior, bridge transcript,
  Win31/OPCON proof, rollback, cleanup, and live proof remain unaccepted.

## Authority Limits

This task does not authorize live bridge dispatch, bridge frame parsing,
firmware persistence, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing,
Android app behavior, router/admin mutation, PCAP, flash, erase, monitor,
serial writes, serial ABI expansion, bridge ABI changes, Gate F service-code
changes, `mesh_discovery.v1` schema changes, Win31 transport changes,
RF/XBee writes, relay actions, load/mains work, live bench, release,
publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
`admin-strict` mutation.

## Handoff

[../handoffs/0150-full-service-mesh-discovery-gate-m4r-bridge-dispatch-envelope-source-contract-to-qa-firmware-communications.md](../handoffs/0150-full-service-mesh-discovery-gate-m4r-bridge-dispatch-envelope-source-contract-to-qa-firmware-communications.md)

## Decision

M4R is accepted source-only for the pure-C caller-owned local
dispatch-envelope/accounting contract over accepted M4Q request routing.
Record/source/docs/process/path/React Native/skills audits, generated-output
cleanup, closed-surface scan, `git diff --check`, no-flash build, and final
post-review completed; the final P2 was stale record/status wording and is
resolved by this acceptance update. Live bridge dispatch, bridge frame parsing,
serial ABI expansion, bridge ABI changes, Gate F request/service changes,
`mesh_discovery.v1` schema changes, persistence, timer creation, firmware
runtime acceptance, live task execution proof, flash, serial writes, and live
proof remain separate gates.
