# Handoff 0151 - Full-Service Mesh Discovery Gate M4S Bridge-Frame Intent Source Contract To QA/Firmware/Communications

Continuation record for Task 0204
`full-service-mesh-discovery-gate-m4s-bridge-frame-intent-source-contract`.

## Continue with

- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`
  only for the pure-C caller-owned request-text intent/accounting boundary
  over accepted M4R local dispatch-envelope routing.
- Preserve `mesh_discovery.v1`, accepted discovery request names, Gate F
  service codes, Gate F bridge request membership, coordinator serial ABI,
  bridge ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Treat "bridge-frame intent" here as a planning label for caller-provided
  request text only. It is not accepted arbitrary frame parsing or transport
  framing.

## Stop Gates

- No live bridge dispatch, bridge frame parsing, firmware persistence,
  first-flash target, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
  router/admin mutation, PCAP, flash, erase, monitor, serial writes, serial ABI
  expansion, bridge ABI changes, Gate F service-code changes,
  `mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
  relay/load/mains work, release, publication, commit, push, PR, deploy,
  `/etc/codex/requirements.toml`, or `admin-strict` mutation is authorized by
  this handoff.
- Do not infer accepted live bridge behavior, live task timing, stack adequacy,
  watchdog behavior, route-table, root/parent, healing, coexistence, cleanup,
  rollback, runtime proof, persistence proof, first-flash proof, or live proof
  from the M4S source-only request-text intent.

## Validation to preserve

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

Recorded: pre-mutation read-only reviewer quorum passed 8/8 with no P1/P2
blockers for opening the narrowed caller-provided ASCII request-text intent
source boundary. Host contract tests, firmware audit, focused mesh discovery
scaffold unittest, broader selected scaffold unittests, record/source/docs/
agent-process/pages/React Native/skills/path audits, no-flash ESP-IDF v6.0.1
build, generated-output cleanup, closed-surface scan, and `git diff --check`
passed. The no-flash build generated `espnow_bbs_build_only.bin` size
`0x22d30` with `0xdd2d0` bytes free in the 1 MiB app partition. Final read-only
reviewers found no source-code P1/P2 blocker; their stale validation-record P2
is resolved by the M4S acceptance record update.
