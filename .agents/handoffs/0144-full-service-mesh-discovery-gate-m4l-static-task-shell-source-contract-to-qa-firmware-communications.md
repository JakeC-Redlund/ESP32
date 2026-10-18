# Handoff 0144 - Full-Service Mesh Discovery Gate M4L Static Task Shell Source Contract To QA/Firmware/Communications

Continuation record for Task 0197
`full-service-mesh-discovery-gate-m4l-static-task-shell-source-contract`.

## Continue with

- Treat Task 0197 as accepted for the source-only M4L static task shell
  boundary.
- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`
  for the named accepted M4L source boundary.
- Preserve M4K caller-owned scheduler policy strings, M4I runtime-cycle
  composition, M4J source-backed requirements, `mesh_discovery.v1`, Gate F
  service codes, bridge ABI, coordinator serial ABI, Win31 transport, and the
  accepted serial-nullmodem live path.
- Next safe firmware continuation after acceptance remains separately gated
  startup/lifecycle, bridge, or live proof work. M5 live mesh remains Tier 3
  only.

## Stop Gates

- No automatic startup from `app_main`, timer creation, queues, callbacks,
  bridge dispatch, firmware persistence, first-flash target, live
  ESP-WIFI-MESH, BLE pairing, Android app behavior, router/admin mutation, PCAP,
  flash, erase, monitor, serial writes, serial ABI expansion, bridge ABI
  changes, Gate F service-code changes, `mesh_discovery.v1` schema changes,
  RF/XBee writes, relay/load/mains work, release, publication, commit, push, PR,
  deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation is
  authorized by this handoff.
- Do not infer live task timing, stack adequacy, watchdog behavior, route-table,
  root/parent, healing, coexistence, cleanup, rollback, runtime proof, bridge
  proof, or live proof from the M4L source contract.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 197
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
git diff --check
```

Recorded validation so far: host C tests PASS, 47-test focused suite PASS,
firmware/path/source/docs/records/agent-process audits PASS, `verify_scaffold.py`
PASS, no-flash ESP-IDF v6.0.1 build PASS with `espnow_bbs_build_only.bin` size
`0x21ab0` and `0xde550` bytes free, generated-output cleanup PASS,
closed-source/CMake scan PASS, and `git diff --check` PASS. Post-review
accepted the M4L source-only boundary 8/8 with no P1/P2 blockers after
stale-record remediation.
