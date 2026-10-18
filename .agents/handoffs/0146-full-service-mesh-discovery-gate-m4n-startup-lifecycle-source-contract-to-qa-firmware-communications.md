# Handoff 0146 - Full-Service Mesh Discovery Gate M4N Startup Lifecycle Source Contract To QA/Firmware/Communications

Continuation record for Task 0199
`full-service-mesh-discovery-gate-m4n-startup-lifecycle-source-contract`.

## Continue with

- Treat Task 0199 as accepted for the caller-owned startup lifecycle source API
  only. Full scaffold audits, no-flash build, generated-output cleanup, diff
  hygiene, stale-record remediation, and final 8/8 weighted post-review have
  passed.
- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`
  for the named accepted M4N caller-owned startup lifecycle source boundary.
- Preserve M4L caller-started static task shell ownership, M4K scheduler policy
  strings, M4I runtime-cycle composition, M4J source-backed requirements,
  `mesh_discovery.v1`, Gate F service codes, bridge ABI, coordinator serial
  ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Next safe firmware continuation after acceptance remains separately gated
  `app_main` integration, bridge contract, persistence contract, or live proof
  work. M5 live mesh remains Tier 3 only.

## Stop Gates

- No `app_main` startup, bridge dispatch, firmware persistence, first-flash
  target, live ESP-WIFI-MESH, BLE pairing, Android app behavior, router/admin
  mutation, PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
  bridge ABI changes, Gate F service-code changes, `mesh_discovery.v1` schema
  changes, RF/XBee writes, relay/load/mains work, release, publication, commit,
  push, PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation
  is authorized by this handoff.
- Do not infer accepted startup-from-`app_main` behavior, live task timing,
  stack adequacy, watchdog behavior, route-table, root/parent, healing,
  coexistence, cleanup, rollback, runtime proof, bridge proof, or live proof
  from the M4N source-only lifecycle API.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 199
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4n-startup-lifecycle-source-api build
find firmware/projects/espnow-bbs -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

Recorded validation: pre-mutation read-only reviewer quorum passed 8/8 with no
P1/P2 blockers for the source boundary. Focused scaffold tests, firmware/path
audits, host tests, record/source/docs/agent-process audits, `verify_scaffold.py`,
no-flash ESP-IDF v6.0.1 build, generated-output cleanup, and `git diff --check`
passed. The no-flash build generated `espnow_bbs_build_only.bin` size `0x21ab0`
with `0xde550` bytes free in the smallest app partition. Initial post-review
found stale status wording only; after record remediation, final post-review
passed 8/8 weighted approval with no P1/P2 blockers.
