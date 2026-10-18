# Handoff 0145 - Full-Service Mesh Discovery Gate M4M Startup Lifecycle Requirements To QA/Firmware/Communications

Continuation record for Task 0198
`full-service-mesh-discovery-gate-m4m-startup-lifecycle-requirements`.

## Continue with

- Treat Task 0198 as accepted only for design-only startup/lifecycle
  requirements after completed scaffold audits, stale scans, and final reviewer
  output.
- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`
  for the accepted M4M design-only startup/lifecycle requirements boundary.
- Preserve M4L caller-started static task shell ownership, M4K scheduler policy
  strings, M4I runtime-cycle composition, M4J source-backed requirements,
  `mesh_discovery.v1`, Gate F service codes, bridge ABI, coordinator serial
  ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Next safe firmware continuation after acceptance remains a separately gated
  startup source contract, bridge contract, or live proof work. M5 live mesh
  remains Tier 3 only.

## Stop Gates

- No firmware source, CMake, test source, automatic startup from `app_main`,
  timer creation, queues, callbacks, bridge dispatch, firmware persistence,
  first-flash target, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
  router/admin mutation, PCAP, flash, erase, monitor, serial writes, serial ABI
  expansion, bridge ABI changes, Gate F service-code changes,
  `mesh_discovery.v1` schema changes, RF/XBee writes, relay/load/mains work,
  release, publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`,
  or `admin-strict` mutation is authorized by this handoff.
- Do not infer accepted startup behavior, runtime behavior, live task timing,
  stack adequacy, watchdog behavior, route-table, root/parent, healing,
  coexistence, cleanup, rollback, runtime proof, bridge proof, or live proof
  from the M4M design-only requirements packet.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 198
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
find firmware/projects/espnow-bbs -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

Recorded validation: pre-mutation read-only reviewer quorum passed 8/8 with no
P1/P2 blockers for the docs-only boundary. Post-mutation scaffold validation,
stale authority scans, generated-output checks, `git diff --check`, and
read-only post-review passed. Weighted post-review was 8/8 after record
remediation, with no P1/P2 blockers for accepting M4M as design-only
requirements only.
