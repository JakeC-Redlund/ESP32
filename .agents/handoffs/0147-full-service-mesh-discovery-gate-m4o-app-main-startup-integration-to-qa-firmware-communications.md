# Handoff 0147 - Full-Service Mesh Discovery Gate M4O App Main Startup Integration To QA/Firmware/Communications

Continuation record for Task 0200
`full-service-mesh-discovery-gate-m4o-app-main-startup-integration`.

## Continue with

- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`
  only for the accepted no-flash `app_main` startup integration source boundary.
- Preserve M4N lifecycle semantics, M4L static task buffer ownership, M4K
  scheduler policy strings, `mesh_discovery.v1`, Gate F service codes, bridge
  ABI, coordinator serial ABI, Win31 transport, and the accepted
  serial-nullmodem live path.
- Next safe continuation after acceptance remains separately gated bridge
  contract, persistence contract, runtime proof, first-flash proof, or M5 live
  mesh. M5 live mesh remains Tier 3 only.

## Stop Gates

- No bridge dispatch, firmware persistence, first-flash target, live
  ESP-WIFI-MESH, BLE pairing, Android app behavior, router/admin mutation,
  PCAP, flash, erase, monitor, serial writes, serial ABI expansion, bridge ABI
  changes, Gate F service-code changes, `mesh_discovery.v1` schema changes,
  RF/XBee writes, relay/load/mains work, release, publication, commit, push,
  PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation is
  authorized by this handoff.
- Do not infer accepted live task timing, stack adequacy, watchdog behavior,
  route-table, root/parent, healing, coexistence, cleanup, rollback, runtime
  proof, bridge proof, or live proof from the M4O source-only integration.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 200
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4o-app-main-startup-integration-source-contract build
find firmware/projects/espnow-bbs -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

Recorded validation: pre-mutation read-only reviewer quorum passed 8/8 with no
P1/P2 blockers for opening the source boundary. Focused scaffold unit tests,
host contract tests, firmware/path/records/sources/docs/agent-process/full
scaffold audits, no-flash ESP-IDF v6.0.1 build, generated-output cleanup,
closed-surface scan, and `git diff --check` passed. The no-flash build generated
`espnow_bbs_build_only.bin` size `0x22d50` with `0xdd2b0` bytes free in the
1 MiB app partition. Final read-only post-review passed 8/8 weighted approval
with no P1/P2 blockers after record-finalization conditions were captured and
resolved.
