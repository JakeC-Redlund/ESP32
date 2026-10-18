# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4J Runtime Scheduler Design

## Scope

Tier 2 design-only source-backed review for the ESP-NOW BBS full-service mesh
discovery lane.

This ledger records only runtime scheduler design requirements, durable records,
source-index coverage, status updates, and reviewer output. It does not accept
firmware source implementation, ESP-IDF task or timer code, generated build
output, `sdkconfig`, flash, erase, monitor, serial writes, serial ABI
expansion, bridge dispatch, firmware persistence, ESP-NOW runtime,
ESP-WIFI-MESH runtime, BLE pairing, Android app behavior, router/admin
mutation, PCAP, RF/XBee writes, relay/load/mains, release, publication, commit,
push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`
- `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`
- `SRC-ESP-IDF-ESP-TIMER-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-PHASE-5-6-RUNTIME-DESIGN-REVIEW-2026-05-26`
- `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-RUNTIME-REQUIREMENTS-2026-05-26`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`

## Verified Facts

- Current status records accept M4I only as a caller-owned runtime-cycle
  contract and keep actual scheduler runtime closed.
- The consolidated plan names a separately gated actual runtime scheduler
  source slice or source-backed design review as the next safe mesh-discovery
  continuation.
- ESP-IDF FreeRTOS and ESP Timer primary docs are now source-indexed for
  task/core-affinity, scheduling, tick, task cleanup, timer dispatch, and
  timer-callback design concerns.
- This M4J gate does not edit firmware source, tests, CMake, or generated build
  artifacts.

## Assumptions

- Design-only requirements should precede source implementation because
  scheduler task/timer ownership, callback handoff, memory budget, cleanup, and
  live recovery are still unknown.
- A later source gate can use this packet to choose whether to stay
  caller-driven, introduce static runtime state, or introduce ESP-IDF task/timer
  surfaces.

## Unknowns

- No task topology, timer mechanism, stack budget, priority, core affinity,
  watchdog policy, ESP-NOW callback integration, ESP-WIFI-MESH callback
  integration, bridge dispatch, persistence, recovery, first-flash target, live
  route table, healing, BLE/GATT, Android app, router/admin, PCAP, transcript,
  cleanup, rollback, or live proof is accepted.

## Pre-Mutation Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Prior M4I reviewers had already been waited and closed. Two read-only
reviewers were spawned, waited, and closed after output capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass | No P1/P2 blockers for M4J as a design-only gate. Required no firmware source, no tests, no task/timer code, no build output, no `sdkconfig`, no live/flash/serial/BLE/router/admin/release, preservation of `mesh_discovery.v1`, Gate F, bridge ABI, coordinator serial ABI, and Win31 transport, and source-index coverage for ESP-IDF FreeRTOS and ESP Timer if cited. |
| QA/evidence reviewer | 3 | pass | No P1/P2 blockers to starting. Required architecture note, source ledger, Task 0195, Handoff 0142, source-index rows, docs index links, status updates, stale-wording cleanup, scaffold audits, and post-mutation review before final acceptance. |

## Post-Implementation Reviewer Quorum

After validation, two read-only reviewers were spawned, waited, and closed
after output capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass | Approved final M4J design-only acceptance with no P1/P2 blockers. Confirmed official ESP-IDF FreeRTOS/ESP Timer source rows, no implementation leakage, no `sdkconfig*` or generated build/test output, and preservation of `mesh_discovery.v1`, Gate F, bridge ABI, coordinator serial ABI, and Win31 transport. |
| QA/evidence reviewer | 3 | pass | Approved final M4J acceptance wording with no P1/P2 blockers. Confirmed task, handoff, docs index, source-index, status ledgers, known gaps, and validation evidence alignment. |

Weighted result: 8/8 approval with no P1/P2 blockers.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 195`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`:
  exit 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`:
  exit 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`: PASS.
- `git diff --check`: exit 0.
- `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print`: empty.
- `test ! -e build/tests/espnow_bbs_mesh_discovery_contract`: exit 0.

## Decision

Decision: M4J is accepted as a design-only runtime scheduler requirements
review. Future runtime scheduler source implementation, ESP-IDF task/timer
code, firmware runtime behavior, persistence, first-flash proof, and M5 live
mesh remain separate gates.
