# Handoff 0134 - Full-Service Mesh Discovery Gate M4B Build-Only Target To QA/Firmware/Communications

Continuation record for Task 0187
`full-service-mesh-discovery-gate-m4b-build-only-target`.

## Continue with

- Treat Task 0187 as build-only ESP-IDF target acceptance for
  `firmware/projects/espnow-bbs/`.
- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`
  for the current target/build proof packet.
- Preserve `mesh_discovery.v1`, Gate F service codes, bridge ABI, coordinator
  serial ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Next safe firmware continuation is a separately gated runtime/interface
  design or source slice. M5 live mesh remains Tier 3 only.

## Stop Gates

- No firmware runtime implementation, firmware persistence, first-flash target,
  live ESP-WIFI-MESH, BLE pairing, Android app behavior, router/admin mutation,
  PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
  RF/XBee writes, relay/load/mains work, release, publication, commit, push,
  PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict` mutation is
  authorized by this handoff.
- Do not infer route-table, root/parent, healing, coexistence, cleanup,
  rollback, or runtime proof from the build-only target.

## Validation to preserve

```bash
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-build-only build
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 187
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
git diff --check
```
