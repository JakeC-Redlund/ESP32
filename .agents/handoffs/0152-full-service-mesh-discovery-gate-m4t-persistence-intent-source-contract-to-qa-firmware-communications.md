# Handoff 0152 - Full-Service Mesh Discovery Gate M4T Persistence Intent Source Contract To QA/Firmware/Communications

Continuation record for Task 0205
`full-service-mesh-discovery-gate-m4t-persistence-intent-source-contract`.

## Continue with

- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`
  only for the pure-C caller-owned volatile persistence-intent/accounting
  boundary over accepted M4S request-text routing.
- Preserve `mesh_discovery.v1`, accepted discovery request names, Gate F
  service codes, Gate F bridge request membership, coordinator serial ABI,
  bridge ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Treat staged bytes as local bounded-summary byte accounting only. They are
  not retained storage, reboot durability, NVS/file persistence, or runtime
  persistence proof.

## Stop Gates

- No NVS writes, file writes, retained storage, reboot durability, live bridge
  dispatch, bridge frame parsing, firmware persistence acceptance,
  first-flash target, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
  router/admin mutation, PCAP, flash, erase, monitor, serial writes, serial ABI
  expansion, bridge ABI changes, Gate F service-code changes,
  `mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
  relay/load/mains work, release, publication, commit, push, PR, deploy,
  `/etc/codex/requirements.toml`, or `admin-strict` mutation is authorized by
  this handoff.
- Do not infer accepted durable persistence, live bridge behavior, live task
  timing, stack adequacy, watchdog behavior, route-table, root/parent, healing,
  coexistence, cleanup, rollback, runtime proof, first-flash proof, or live
  proof from the M4T source-only persistence-intent contract.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 205
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4t-persistence-intent-source-contract build
git diff --check
```

Recorded so far: pre-mutation read-only reviewer quorum passed 8/8 with no
P1/P2 blockers for opening the volatile persistence-intent source boundary.
Host contract tests, firmware audit, focused mesh discovery scaffold unittest,
record/source/docs/path/data/agent-process/pages/React Native/skills audits,
broader selected scaffold unittests, no-flash ESP-IDF v6.0.1 build,
generated-output cleanup checks, closed-surface scan, and `git diff --check`
passed. The no-flash build generated
`espnow_bbs_build_only.bin` size `0x22d20` with `0xdd2e0` bytes free in the
1 MiB app partition. Final post-review passed after the development-plan record-alignment P2 was resolved.
