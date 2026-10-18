# Handoff 0141 - Full-Service Mesh Discovery Gate M4I Runtime Cycle Contract To QA/Firmware/Communications

Continuation record for Task 0194
`full-service-mesh-discovery-gate-m4i-runtime-cycle-contract`.

## Continue with

- Treat Task 0194 as inert runtime-cycle contract work for
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`.
- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`
  for the current bounded runtime-cycle proof packet.
- Preserve caller-owned runtime-cycle state, caller-owned execution-accounting
  state, caller-owned scheduler state, caller-owned dispatch plan state,
  caller-owned owner and queue state, two-phase no-partial-mutation behavior,
  `mesh_discovery.v1`, Gate F service codes, bridge ABI, coordinator serial
  ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Next safe firmware continuation remains separately gated actual runtime
  scheduler design/source work or source-backed design review. M5 live mesh
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
  rollback, runtime proof, actual scheduler proof, actual execution proof, or
  live proof from the runtime-cycle contract.
- Do not convert the runtime-cycle contract into static mutable runtime state,
  FreeRTOS tasks/queues/timers, callbacks, dispatch loops, bridge handlers,
  serial handlers, persistence, file output, or dynamic allocation without a
  separate gate.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4i-runtime-cycle-contract build
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 194
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
git diff --check
```

The accepted no-flash build generated `espnow_bbs_build_only.bin` size
`0x21ab0` with `0xde550` bytes free in the smallest app partition. Project
`sdkconfig*` files and host-test build outputs remained cleaned from tracked
workspace paths. Post-implementation reviewers accepted the source/evidence
boundary 8/8 after stale interim record wording was remediated.
