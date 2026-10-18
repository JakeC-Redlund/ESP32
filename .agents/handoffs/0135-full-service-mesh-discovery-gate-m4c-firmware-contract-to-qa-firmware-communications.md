# Handoff 0135 - Full-Service Mesh Discovery Gate M4C Firmware Contract To QA/Firmware/Communications

Continuation record for Task 0188
`full-service-mesh-discovery-gate-m4c-firmware-contract`.

## Continue with

- Treat Task 0188 as inert firmware contract component acceptance for
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`.
- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`
  for the current firmware contract proof packet.
- Preserve `mesh_discovery.v1`, Gate F service codes, bridge ABI, coordinator
  serial ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Next safe firmware continuation remains a separately gated runtime/interface
  source slice. M5 live mesh remains Tier 3 only.

## Stop Gates

- No firmware runtime implementation, firmware persistence, first-flash target,
  live ESP-WIFI-MESH, BLE pairing, Android app behavior, router/admin mutation,
  PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
  bridge ABI changes, Gate F service-code changes, `mesh_discovery.v1` schema
  changes, RF/XBee writes, relay/load/mains work, release, publication,
  commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
  `admin-strict` mutation is authorized by this handoff.
- Do not infer route-table, root/parent, healing, coexistence, cleanup,
  rollback, runtime proof, or live proof from the firmware contract component.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4c-contract build
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 188
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
git diff --check
```
