# Handoff 0153 - Full-Service Mesh Discovery Gate M4U Retained Persistence Requirements To QA/Firmware/Communications

Continuation record for Task 0206
`full-service-mesh-discovery-gate-m4u-retained-persistence-requirements`.

## Continue with

- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`
  only for design-only retained-persistence requirements after accepted M4T.
- Preserve `mesh_discovery.v1`, accepted discovery request names, Gate F
  service codes, Gate F bridge request membership, coordinator serial ABI,
  bridge ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Treat the M4U requirements as preconditions for a later source gate. They are
  not NVS/file storage implementation, reboot durability, or runtime
  persistence proof.

## Stop Gates

- No NVS writes, file writes, retained storage implementation, reboot
  durability, live bridge dispatch, bridge frame parsing, firmware persistence
  acceptance, first-flash target, live ESP-WIFI-MESH, BLE pairing, Android app
  behavior, router/admin mutation, PCAP, flash, erase, monitor, serial writes,
  serial ABI expansion, bridge ABI changes, Gate F service-code changes,
  `mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
  relay/load/mains work, release, publication, commit, push, PR, deploy,
  `/etc/codex/requirements.toml`, or `admin-strict` mutation is authorized by
  this handoff.
- Do not infer accepted durable persistence, live bridge behavior, live task
  timing, stack adequacy, watchdog behavior, route-table, root/parent, healing,
  coexistence, cleanup, rollback, runtime proof, first-flash proof, or live
  proof from the M4U design-only requirements.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 206
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
git diff --check
```

Recorded so far: pre-mutation read-only reviewer quorum passed 8/8 with no
P1/P2 blockers for opening the M4U design-only requirements boundary. Record,
source, docs, path, data, agent-process, pages, and skills audits passed;
`git diff --check` passed. Closed-surface scan found only the explicit
denied-marker list in the M4U requirements note, not storage API calls or
source/runtime openings. Final post-review passed 8/8 weighted after the status-ledger P2 was resolved. No P1/P2 blockers remain.
