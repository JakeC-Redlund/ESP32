# Handoff 0149 - Full-Service Mesh Discovery Gate M4Q Local Bridge Request Router Source Contract To QA/Firmware/Communications

Continuation record for Task 0202
`full-service-mesh-discovery-gate-m4q-local-bridge-request-router-source-contract`.

## Continue with

- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`
  only for the pure-C caller-owned local request-router/accounting boundary
  over the accepted M4P bridge-summary selector.
- Preserve `mesh_discovery.v1`, accepted discovery request names, Gate F service
  codes, Gate F bridge request membership, coordinator serial ABI, bridge ABI,
  Win31 transport, and the accepted serial-nullmodem live path.
- Next safe continuation after M4Q acceptance remains separately gated live
  bridge dispatch design/source work, persistence contract, runtime proof,
  first-flash proof, or M5 live mesh. M5 live mesh remains Tier 3 only.

## Stop Gates

- No live bridge dispatch, firmware persistence, first-flash target, live
  ESP-WIFI-MESH, BLE pairing, Android app behavior, router/admin mutation,
  PCAP, flash, erase, monitor, serial writes, serial ABI expansion, bridge ABI
  changes, Gate F service-code changes, `mesh_discovery.v1` schema changes,
  Win31 transport changes, RF/XBee writes, relay/load/mains work, release,
  publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
  `admin-strict` mutation is authorized by this handoff.
- Do not infer accepted live bridge behavior, live task timing, stack adequacy,
  watchdog behavior, route-table, root/parent, healing, coexistence, cleanup,
  rollback, runtime proof, persistence proof, first-flash proof, or live proof
  from the M4Q source-only local request router.

## Validation to preserve

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

Recorded acceptance evidence: pre-mutation read-only reviewer quorum passed
8/8 with no P1/P2 blockers for opening the named source boundary. Focused
scaffold tests, host contract tests, firmware/path/records/sources/docs/
agent-process/pages/React Native/skills audits, cleanup checks,
closed-surface scan, `git diff --check`, and no-flash ESP-IDF v6.0.1 build
passed. The no-flash build generated `espnow_bbs_build_only.bin` size
`0x22d20` with `0xdd2e0` bytes free in the 1 MiB app partition. The aggregate
`scripts/verify_scaffold.py` command was attempted twice and entered kernel
I/O wait on `/mnt/h`; it was stopped both times, and the individual audit
groups covered by that aggregate command were run separately and passed. Final
post-review passed 8/8 weighted conditional approval after this
record-alignment P2 was resolved. No P1/P2 blockers remain for the accepted
M4Q source-only boundary.
