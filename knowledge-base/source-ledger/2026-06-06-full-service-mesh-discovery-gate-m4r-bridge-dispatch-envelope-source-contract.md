# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4R Bridge Dispatch Envelope Source Contract

## Scope

Tier 2 pure-C firmware source proof for a caller-owned local dispatch envelope
and accounting contract over the accepted M4Q local bridge request router.

This ledger records the M4R source API, host C tests, scaffold audit coverage,
docs/status records, and read-only reviewer output. It does not accept live
bridge dispatch, bridge frame parsing, serial ABI expansion, bridge ABI changes,
Gate F request or service changes, `mesh_discovery.v1` schema changes,
persistence, timer creation, firmware runtime acceptance, live task execution
proof, flash, erase, monitor, serial writes, ESP-NOW runtime, ESP-WIFI-MESH
runtime, BLE pairing, Android app behavior, router/admin mutation, PCAP,
RF/XBee writes, relay/load/mains, release, publication, commit, push, PR, or
deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`
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

- M4Q accepts only a source-only local bridge request router and leaves live
  bridge dispatch separate.
- Existing `mesh_discovery_contract` request membership preserves accepted
  discovery request names and Gate F bridge request/service disjointness.
- M4R adds `espnow_bbs_mesh_contract_bridge_dispatch_envelope_t`,
  `espnow_bbs_mesh_contract_bridge_dispatch_envelope_init`,
  `espnow_bbs_mesh_contract_bridge_dispatch_envelope_reset`,
  `espnow_bbs_mesh_contract_bridge_dispatch_envelope_route`, and
  `espnow_bbs_mesh_contract_bridge_dispatch_envelope_summary`.
- M4R host tests cover all accepted discovery request names, null envelope,
  null router, null/non-ASCII/unknown request rejection, undersized output
  rejection, missing caller-owned summary state rejection, deterministic
  attempted/would-dispatch/rejected/last-status accounting, envelope summary
  bounds, M4Q router composition, and non-mutation of queue, owner, and runtime
  scheduler state.

## Assumptions

- A local dispatch-envelope/accounting layer is safe to advance before live
  bridge dispatch because it does not parse bridge frames, write serial, mutate
  runtime state, or change protocol membership.
- A no-flash ESP-IDF build can prove compile-time integration only.

## Unknowns

- No live bridge dispatch proof, Win31/OPCON bridge request proof, bridge frame
  parsing, serial ABI, bridge ABI, firmware runtime acceptance, persistence
  model, timer behavior, route table, parent/root behavior, healing behavior,
  watchdog behavior, stack adequacy, first-flash behavior, rollback, cleanup,
  or live proof is accepted.

## Reviewer Quorum

Lifecycle listing was not visible through the available tools before spawning.
Two read-only reviewers were spawned, waited, and closed after output capture
before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer | 5 | conditional approve | No P1/P2 blockers for opening M4R if it remains pure-C caller-owned envelope/accounting over M4Q, exact-matches only the four accepted discovery request names, mutates only caller-owned envelope/router accounting, includes `live=blocked` and `serial=closed`, and keeps live dispatch, bridge frame parsing, serial/Gate F/schema behavior, persistence, timers, runtime, flash, files, logging, Wi-Fi, ESP-NOW, ESP-WIFI-MESH, BLE, GPIO, RF/XBee, and release closed. |
| QA/Evidence/Agent Operations reviewer | 3 | conditional approve | No P1/P2 blockers for M4R if it receives task/handoff/source/docs records, audit updates, host tests, full scaffold validation, no-flash build, cleanup checks, closed-surface scan, and final post-review. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4R source boundary.

## Validation

Completed commands/results:

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

- Host contract tests passed.
- Firmware audit passed.
- Focused mesh discovery scaffold unittest passed: 2 tests.
- Broader selected scaffold unittest suite passed: 15 tests.
- Record/source/docs/agent-process/pages/React Native/skills/path audits passed.
- No-flash ESP-IDF v6.0.1 build passed and generated
  `espnow_bbs_build_only.bin` size `0x22d20`; the 1 MiB app partition had
  `0xdd2e0` bytes free.
- Generated `sdkconfig*` files were removed from
  `firmware/projects/espnow-bbs/`, the host-test build artifact was absent, the
  closed-surface scan matched only the existing denied-token data string
  `"serial_write"`, and `git diff --check` passed.
- Final read-only reviewers found no source-code P1/P2 blocker. Their stale
  record/status wording P2 is resolved by the final M4R acceptance update.

## Decision

M4R is accepted source-only for the pure-C caller-owned local
dispatch-envelope/accounting contract over accepted M4Q request routing. Live
bridge dispatch, bridge frame parsing, serial ABI expansion, bridge ABI changes,
Gate F request/service changes, `mesh_discovery.v1` schema changes,
persistence, timer creation, firmware runtime acceptance, first-flash proof,
and M5 live mesh remain separate gates.
