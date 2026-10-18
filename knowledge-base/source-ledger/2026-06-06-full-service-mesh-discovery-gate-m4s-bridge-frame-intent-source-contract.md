# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4S Bridge-Frame Intent Source Contract

## Scope

Tier 2 pure-C firmware source proof for a caller-owned request-text intent and
accounting contract over the accepted M4R local dispatch envelope.

This ledger records the M4S source API, host C tests, scaffold audit coverage,
docs/status records, and read-only reviewer output. It does not accept live
bridge dispatch, arbitrary bridge frame parsing, transport framing, serial ABI
expansion, bridge ABI changes, Gate F request or service changes,
`mesh_discovery.v1` schema changes, persistence, timer creation, firmware
runtime acceptance, live task execution proof, flash, erase, monitor, serial
writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains,
release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4R is accepted source-only and leaves live bridge dispatch plus bridge frame
  parsing separate.
- Existing `mesh_discovery_contract` request membership preserves accepted
  discovery request names and Gate F bridge request/service disjointness.
- M4S adds `espnow_bbs_mesh_contract_bridge_request_text_intent_t`,
  `espnow_bbs_mesh_contract_bridge_request_text_intent_init`,
  `espnow_bbs_mesh_contract_bridge_request_text_intent_reset`,
  `espnow_bbs_mesh_contract_bridge_request_text_intent_route`, and
  `espnow_bbs_mesh_contract_bridge_request_text_intent_summary`.
- M4S host tests cover accepted trimmed request text, malformed request text
  rejection before envelope/router accounting, null envelope/router downstream
  rejection after normalization, undersized output rejection, missing
  caller-owned summary state rejection, deterministic accounting, summary
  bounds, M4R/M4Q composition, and non-mutation of queue, owner, and runtime
  scheduler state.

## Assumptions

- A request-text intent/accounting layer is safe to advance before live bridge
  dispatch because it does not parse bridge frames, write serial, mutate
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
| Architecture/Communications/Firmware reviewer | 5 | conditional approve | No P1/P2 blockers for opening M4S if it remains pure-C caller-owned request-text intent/accounting over M4R/M4Q, trims only leading/trailing ASCII space/tab/CR/LF, rejects malformed/structured/live/Gate F/unknown/non-ASCII text, and keeps serial ABI, bridge ABI, arbitrary frame parsing, live dispatch, persistence, timers, runtime, flash, files, logging, Wi-Fi, ESP-NOW, ESP-WIFI-MESH, BLE, GPIO, RF/XBee, and release closed. |
| QA/Evidence/Agent Operations reviewer | 3 | conditional approve | No P1/P2 blockers for M4S if it receives task/handoff/source/docs records, audit updates, host tests, full scaffold validation, no-flash build, cleanup checks, closed-surface scan, and final post-review. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4S source boundary.

Final post-mutation read-only reviewers were spawned, waited, and closed after
output capture. They found no source-code P1/P2 blocker. Both rejected final
acceptance until stale validation-in-progress record wording was removed; that
record-alignment P2 is resolved by this acceptance update.
Post-remediation follow-up reviewers `019e9da0-7d7e-7060-97f3-c1aa7264f3ec`
(Architecture/Communications/Firmware, weight 5) and
`019e9da0-9938-7172-9d50-2bd7403655f9` (QA/Evidence/AgentOps, weight 3) were
spawned read-only, waited, captured, and closed. They approved 8/8 with no
P1/P2 blockers and confirmed remaining validation-in-progress hits are
historical resolved-P2 wording only.

## Validation

Completed commands/results:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 204
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4s-bridge-frame-intent-source-contract build
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
  `espnow_bbs_build_only.bin` size `0x22d30`; the 1 MiB app partition had
  `0xdd2d0` bytes free.
- Generated `sdkconfig*` files were removed from
  `firmware/projects/espnow-bbs/`, the host-test build artifact was absent,
  the closed-surface scan matched only the existing denied-token data string
  `"serial_write"`, and `git diff --check` passed.
- Final read-only reviewers found no source-code P1/P2 blocker. Their stale
  validation-record P2 is resolved by the final M4S acceptance update.
- Post-remediation follow-up reviewers approved 8/8 with no P1/P2 blockers
  after the M4S durable records were aligned.

## Decision

M4S is accepted source-only for the pure-C caller-owned request-text
intent/accounting contract over accepted M4R/M4Q. Live bridge dispatch,
arbitrary bridge frame parsing, serial ABI expansion, bridge ABI changes, Gate
F request/service changes, `mesh_discovery.v1` schema changes, persistence,
timer creation, firmware runtime acceptance, first-flash proof, and M5 live
mesh remain separate gates.
