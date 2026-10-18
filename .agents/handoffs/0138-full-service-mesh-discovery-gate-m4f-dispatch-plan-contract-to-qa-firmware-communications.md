# Handoff 0138 - Full-Service Mesh Discovery Gate M4F Dispatch Plan Contract To QA/Firmware/Communications

Continuation record for Task 0191
`full-service-mesh-discovery-gate-m4f-dispatch-plan-contract`.

## Continue with

- Treat Task 0191 as inert dispatch-plan contract work for
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`.
- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`
  for the current bounded dispatch-plan proof packet.
- Preserve caller-owned plan state, caller-owned owner and queue state,
  deterministic drain/ingest/snapshot action order, requested/planned/rejected
  accounting, bounded ASCII validation, `mesh_discovery.v1`, Gate F service
  codes, bridge ABI, coordinator serial ABI, Win31 transport, and the accepted
  serial-nullmodem live path.
- Next safe firmware continuation remains a separately gated scheduler
  integration source slice or source-backed design review. M5 live mesh
  remains Tier 3 only.

## Stop Gates

- No firmware runtime implementation, actual FreeRTOS task creation, static
  runtime queues, dispatch loops, callbacks, handler calls, firmware
  persistence, first-flash target, live ESP-WIFI-MESH, BLE pairing, Android
  app behavior, router/admin mutation, PCAP, flash, erase, monitor, serial
  writes, serial ABI expansion, bridge ABI changes, Gate F service-code
  changes, `mesh_discovery.v1` schema changes, RF/XBee writes,
  relay/load/mains work, release, publication, commit, push, PR, deploy,
  `/etc/codex/requirements.toml`, or `admin-strict` mutation is authorized by
  this handoff.
- Do not infer route-table, root/parent, healing, coexistence, cleanup,
  rollback, runtime proof, scheduler proof, or live proof from the dispatch
  plan contract.
- Do not convert the dispatch plan into static mutable runtime state, FreeRTOS
  tasks/queues/timers, callbacks, dispatch loops, bridge handlers, serial
  handlers, persistence, or dynamic allocation without a separate gate.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4f-dispatch-plan build
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 191
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
git diff --check
```
