# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4Q Local Bridge Request Router Source Contract

## Scope

Tier 2 pure-C firmware source proof for a caller-owned local request router and
accounting contract over the accepted M4P bridge-summary selector.

This ledger records the M4Q source API, host C tests, scaffold audit coverage,
docs/status records, and read-only reviewer output. It does not accept live
bridge dispatch, serial ABI expansion, Gate F request or service changes,
`mesh_discovery.v1` schema changes, persistence, timer creation, firmware
runtime acceptance, live task execution proof, flash, erase, monitor, serial
writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains,
release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4P accepts only a source-only local bridge-summary selector and leaves live
  bridge dispatch separate.
- Existing `mesh_discovery_contract` request membership preserves accepted
  discovery request names and Gate F bridge request/service disjointness.
- M4Q adds `espnow_bbs_mesh_contract_bridge_request_router_t`,
  `espnow_bbs_mesh_contract_bridge_request_router_init`,
  `espnow_bbs_mesh_contract_bridge_request_router_reset`,
  `espnow_bbs_mesh_contract_bridge_request_route`, and
  `espnow_bbs_mesh_contract_bridge_request_router_summary`.
- M4Q host tests cover all accepted discovery request names, null router
  behavior, null/non-ASCII/unknown request rejection, undersized output
  rejection, missing caller-owned summary state rejection, deterministic
  attempted/accepted/rejected/last-status accounting, router summary bounds,
  and non-mutation of queue, owner, and runtime scheduler state.

## Assumptions

- A local request-router/accounting layer is safe to advance before live bridge
  dispatch because it does not parse bridge frames, write serial, mutate runtime
  state, or change protocol membership.
- A no-flash ESP-IDF build can prove compile-time integration only.

## Unknowns

- No live bridge dispatch proof, Win31/OPCON bridge request proof, firmware
  runtime acceptance, persistence model, timer behavior, route table,
  parent/root behavior, healing behavior, watchdog behavior, stack adequacy,
  first-flash behavior, rollback, cleanup, or live proof is accepted.

## Reviewer Quorum

Lifecycle listing was not visible through the available tools before spawning.
Two read-only reviewers were spawned, waited, and closed after output capture
before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer | 5 | conditional approve | No P1/P2 blockers for opening M4Q if it exact-matches only the four accepted discovery request names, wraps M4P summary selection, mutates only caller-owned router/accounting state, and keeps queues, owners, schedulers, tasks, timers, serial, persistence, Wi-Fi, ESP-NOW, BLE, GPIO, logging, flash, files, and live dispatch closed. |
| QA/Evidence/Agent Operations reviewer | 3 | conditional approve | No P1/P2 blockers for M4Q if it receives task/handoff/source/docs records, audit updates, host tests, full scaffold validation, no-flash build, cleanup checks, and final post-review. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4Q source boundary.

## Validation

Completed commands/results:

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
  `espnow_bbs_build_only.bin` size `0x22d20`, leaving `0xdd2e0` bytes free in
  the 1 MiB app partition.
- Generated `sdkconfig*` cleanup check produced no files, and
  `build/tests/espnow_bbs_mesh_discovery_contract` was absent.
- Closed-surface scan found only the expected blocked-action table string
  `serial_write`.
- `git diff --check` passed.
- Final post-review passed 8/8 weighted conditional approval after the
  record-alignment P2 was resolved. No P1/P2 blockers remain for the named
  source-only M4Q gate.

## Decision

M4Q is accepted only as a pure-C caller-owned local bridge request
router/accounting source contract over the accepted M4P bridge-summary
selector. Live bridge dispatch, persistence, firmware runtime acceptance,
first-flash proof, and M5 live mesh remain separate gates.
