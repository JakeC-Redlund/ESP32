# Handoff 0155 - Full-Service Mesh Discovery Gate M4W Retained Recovery App Main Source To QA/Firmware/Communications

Continuation record for Task 0216
`full-service-mesh-discovery-gate-m4w-retained-recovery-app-main-source`.

## Continue With

- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4W-RETAINED-RECOVERY-APP-MAIN-SOURCE-2026-06-06`
  only for source-only app-main retained recovery integration after accepted
  M4V.
- Preserve `mesh_discovery.v1`, accepted discovery request names, Gate F
  service codes, Gate F bridge request membership, coordinator serial ABI,
  bridge ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Treat retained recovery in `app_main` as load-only status capture. The source
  initializes retained intent to empty volatile state, initializes storage
  context, calls storage init/load, records init/load status, and then continues
  M4O lifecycle startup.

## Stop Gates

- No live NVS execution proof, reboot durability, runtime persistence proof,
  live bridge dispatch, bridge frame parsing, firmware runtime acceptance,
  first-flash target, live ESP-WIFI-MESH, BLE pairing, Android behavior,
  router/admin mutation, PCAP, flash, erase, monitor, serial writes, serial ABI
  expansion, bridge ABI changes, Gate F service-code changes,
  `mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
  relay/load/mains work, release, publication, commit, push, PR, deploy,
  `/etc/codex/requirements.toml`, or `admin-strict` mutation is authorized by
  this handoff.
- Do not infer accepted live retained persistence, reboot recovery, live bridge
  behavior, live task timing, stack adequacy, watchdog behavior, route-table,
  root/parent, healing, coexistence, rollback, runtime proof, first-flash proof,
  or live proof from the M4W source-only integration.

## Validation To Preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 216
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4w-retained-recovery-app-main-source build
```

Final validation and post-review passed. The no-flash build generated
`espnow_bbs_build_only.bin` size `0x283e0` with `0xd7c20` bytes free in the 1
MiB app partition. Post-mutation reviewers
`019e9e95-dea4-7a22-89d8-3f99e86b9dad` and
`019e9e95-fdf2-7143-9649-c35483efad68` approved 8/8 weighted with no P1/P2
blockers and were closed.
