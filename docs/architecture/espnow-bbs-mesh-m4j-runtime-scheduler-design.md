# ESP-NOW BBS Mesh M4J Runtime Scheduler Design Review

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4J is a Tier 2 design-only continuation for the `espnow-bbs`
full-service mesh discovery lane accepted by `ADR-0003` and `ADR-0009`.

This packet records requirements for the next runtime scheduler source gate
after M4I. It does not add firmware source, tests, CMake changes, task or timer
code, generated build output, `sdkconfig`, flash arguments, serial commands,
bridge dispatch, persistence, live ESP-WIFI-MESH, BLE, router/admin behavior,
release, publication, commit, push, PR, or deploy.

## Verified Facts

- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for the `espnow-bbs`
  coordinator/client firmware lane. Source IDs: `SRC-ESP-IDF-STABLE-ESP32`,
  `SRC-ESP-IDF-GET-STARTED`.
- `ADR-0007` requires deterministic runtime scheduling, bounded queues, visible
  backpressure and counters, volatile default firmware state, and no flash
  persistence under that ADR. Source ID:
  `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-RUNTIME-REQUIREMENTS-2026-05-26`.
- `ADR-0008` pins host-only scheduler defaults and dispatch order while keeping
  coordinator/peer firmware runtime, persistence, flash, serial writes, BLE,
  mesh, router/admin, and live proof closed. Source ID:
  `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-PHASE-5-6-RUNTIME-DESIGN-REVIEW-2026-05-26`.
- Gate M4G accepted only caller-owned scheduler staging over an M4F dispatch
  plan. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`.
- Gate M4H accepted only caller-owned execution accounting over an already
  staged M4F dispatch plan. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`.
- Gate M4I accepted only caller-owned runtime-cycle composition over M4G and
  M4H. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`.
- ESP-IDF FreeRTOS for ESP32 uses IDF FreeRTOS with SMP support, task/core
  affinity concerns, byte-based task stack sizing in IDF task creation APIs,
  tick and scheduling behavior, and task cleanup/deletion considerations.
  Source ID: `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`.
- ESP Timer supports task-dispatched and interrupt-dispatched callbacks, and
  its task-dispatch callbacks are serialized; callbacks should stay short and
  defer larger work to lower-priority tasks using FreeRTOS primitives. Source
  ID: `SRC-ESP-IDF-ESP-TIMER-2026-06-06`.

## Assumptions

- A design-only gate is the correct next movement because M4I deliberately
  kept actual scheduler runtime, task creation, callback dispatch, persistence,
  bridge dispatch, and live proof closed.
- The next source gate should separate policy that can remain host-testable
  from ESP-IDF task/timer ownership that requires explicit stack, priority,
  affinity, callback, cleanup, and recovery decisions.
- Volatile firmware scheduler state remains the default until a separate
  persistence and flash-wear gate is accepted.

## Unknowns

- No runtime task topology, task priority, core affinity, stack budget,
  watchdog/yield policy, queue ownership, timer mechanism, callback-to-task
  handoff, memory budget, ESP-NOW callback integration, ESP-WIFI-MESH callback
  integration, shutdown/deinit path, recovery path, first-flash target, route
  table, root/parent selection, healing, coexistence, BLE/GATT, Android app,
  router/admin policy, PCAP, transcript, cleanup, rollback, or live proof is
  accepted by this gate.

## Runtime Scheduler Requirements For The Next Source Gate

- Name the exact scheduler owner object, whether it remains caller-owned, and
  which state is allowed to become static runtime state.
- Name the exact task topology, if any, including priority, core affinity,
  stack budget, watchdog/yield behavior, cleanup/deinit behavior, and deletion
  constraints.
- Name the exact timer mechanism, if any, including dispatch method, period,
  callback work limit, and callback-to-task handoff contract.
- Keep ESP Timer callbacks short and non-blocking; defer any heavier scheduler
  work to a task or caller-driven loop in the later implementation gate.
- Preserve deterministic ordering from ADR-0008 unless a later owner gate
  changes that order with tests and source records.
- Preserve bounded queue/backpressure behavior, no silent loss, and
  bridge-visible counters from ADR-0007.
- Preserve volatile-only default state; do not persist scheduler, queue,
  custody, retry, or runtime-cycle state to flash.
- Preserve `mesh_discovery.v1`, Gate F service codes, custody codes, bridge
  request names, coordinator serial ABI, and Win31 transport.
- Require host tests or no-flash build evidence that covers task/timer source
  boundaries before any source gate accepts implementation.
- Require same-session Tier 3 authority, identity, backups, hashes, recovery,
  transcript, and cleanup before any flash, serial-write, monitor, or live mesh
  proof.

## Validation

- Pre-mutation reviewers approved this design-only M4J boundary 8/8 with no
  P1/P2 blockers.
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
- Post-implementation reviewers approved final design-only acceptance 8/8
  with no P1/P2 blockers after validation: Firmware/Communications/
  Architecture 5/5 and QA/Evidence 3/3.

## Decision

M4J is accepted as a source-backed design review only. It records requirements
for a later runtime scheduler source gate and does not accept runtime scheduler
source implementation, ESP-IDF task or timer creation, firmware runtime
behavior, persistence, bridge dispatch, live mesh, BLE, Android, router/admin,
flash, serial, RF/XBee, relay/load/mains, release, publication, or deployment
authority.
