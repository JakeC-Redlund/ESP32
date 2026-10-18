# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4U Retained Persistence Requirements

## Scope

Tier 2 design-only requirements and source-boundary planning for future
retained persistence after accepted M4T volatile persistence intent.

This ledger records the M4U requirements packet, status updates, source-index
entry, task record, handoff, and read-only reviewer output. It does not accept
retained persistence implementation, NVS writes, file writes, reboot durability,
runtime persistence, live bridge dispatch, arbitrary bridge frame parsing,
transport framing, serial ABI expansion, bridge ABI changes, Gate F request or
service changes, `mesh_discovery.v1` schema changes, timer creation, firmware
runtime acceptance, live task execution proof, flash, erase, monitor, serial
writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains,
release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`
- `SRC-ESP-IDF-NVS`

## Verified Facts

- M4T is accepted source-only and keeps retained persistence, NVS/file storage,
  reboot durability, runtime persistence, live bridge dispatch, flash, serial
  writes, and live proof closed.
- The development plan names retained persistence work as a separately gated
  continuation after M4T.
- `SRC-ESP-IDF-NVS` exists in the source index for future NVS storage design
  discussion.
- The current firmware audit still forbids storage/runtime markers for this
  boundary; M4U does not add storage API exceptions.

## Assumptions

- Requirements-only planning is the correct next slice because it can define
  retained-persistence evidence expectations without writing to flash or
  changing firmware runtime behavior.
- The first retained persistence implementation should store only compact
  M4T-derived intent/accounting fields until a later gate proves a broader
  storage model.

## Unknowns

- NVS namespace/key policy, record format, versioning, migration, corruption
  handling, power-loss behavior, wear policy, write trigger, commit cadence,
  cleanup/deinit behavior, boot recovery, stack impact, first-flash behavior,
  rollback, live bridge transcript, and live proof remain unaccepted.

## Reviewer Quorum

Lifecycle listing was not visible through the available tools before spawning.
Two read-only reviewers were spawned, waited, captured, and closed after output
capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer | 5 | approve | No P1/P2 blockers for opening M4U if it remains design-only, cites source-index rows or marks gaps, does not add storage/runtime audit exceptions, and preserves bridge/serial ABI, Gate F, and `mesh_discovery.v1`. |
| QA/Evidence/Agent Operations reviewer | 3 | approve | No P1/P2 blockers if M4U is not described as accepted retained persistence implementation, reboot durability, runtime persistence, or live proof, and if records/status/audits are aligned. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4U design-only requirements boundary.

## Validation

Validation passed:

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

- Record, source, docs, path, data, agent-process, pages, and skills audits
  passed.
- `git diff --check` passed.
- Closed-surface scan found only the explicit denied-marker list in the M4U
  requirements note, not storage API calls or source/runtime openings.
- Final post-review passed 8/8 weighted after the status-ledger P2 was resolved. No P1/P2 blockers remain.

## Decision

M4U is accepted design-only for retained-persistence requirements after local validation and final 8/8 weighted post-review. It does not accept retained persistence
implementation, NVS/file storage, reboot durability, runtime persistence, live
bridge dispatch, first-flash proof, or M5 live mesh.
