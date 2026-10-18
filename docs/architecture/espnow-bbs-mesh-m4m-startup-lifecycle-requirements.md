# ESP-NOW BBS Mesh M4M Startup Lifecycle Requirements

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4M is a Tier 2 design-only continuation for the `espnow-bbs`
full-service mesh discovery lane accepted by `ADR-0003` and `ADR-0009`.

This packet records requirements for a future startup/lifecycle source gate
after accepted Gate M4L. It does not add firmware source, CMake changes, test
source, task startup from `app_main`, generated build output, `sdkconfig`,
flash arguments, bridge dispatch, persistence, live ESP-WIFI-MESH, BLE,
router/admin behavior, release, publication, commit, push, PR, or deploy.

## Verified Facts

- Gate M4J accepted only source-backed runtime scheduler requirements for later
  source gates. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`.
- Gate M4K accepted only the caller-owned runtime scheduler source contract,
  preserving `caller_invoked`, `task_none`, and `timer_none` policy strings.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`.
- Gate M4L accepted only a caller-started static FreeRTOS task shell source API
  with caller-provided `StaticTask_t` and `StackType_t` buffers. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`.
- `firmware/projects/espnow-bbs/main/main.c` remains inert and does not include
  or start the M4L runtime task shell.
- ESP-IDF FreeRTOS source coverage for task ownership, core affinity, byte-sized
  stack depths, scheduler/tick behavior, and task cleanup/deletion concerns is
  already source-indexed. Source ID: `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`.

## Assumptions

- A design-only lifecycle gate is the correct next step because M4L deliberately
  kept automatic startup from `app_main`, bridge dispatch, firmware runtime
  acceptance, flash, serial writes, monitor, and live proof closed.
- A later source gate can decide whether startup remains caller-driven or opens
  a narrow `app_main` integration path, but M4M itself opens no such path.
- Caller-owned task buffers, scheduler state, owner state, stop/start control,
  and recovery responsibility remain the default boundary until a later gate
  explicitly accepts static runtime state.

## Unknowns

- No automatic startup path, accepted lifecycle state object, stack adequacy,
  watchdog behavior, startup ordering, shutdown timing, bridge-visible lifecycle
  summary, first-flash target, route table, root/parent selection, healing,
  coexistence, BLE/GATT, Android behavior, router/admin policy, PCAP,
  transcript, rollback, cleanup, or live proof is accepted by this gate.

## Startup Lifecycle Requirements For A Future Source Gate

- Name the exact lifecycle owner object and whether it is caller-owned or static
  runtime state.
- Name exact lifecycle states. At minimum the future gate must account for
  `uninitialized`, `configured`, `starting`, `running`, `stop_requested`,
  `stopped`, and `failed`.
- Define accepted and rejected transitions for configure, start, repeated start,
  stop-before-start, repeated stop, restart-after-stopped, create failure,
  failed-state deinit, and deinit-after-stopped.
- Preserve no partial mutation on rejected transitions or task-create failure.
- Keep task buffers, scheduler state, owner state, stop/start control, and
  recovery ownership caller-owned unless the future gate explicitly accepts
  static runtime state.
- If a future gate opens `app_main` integration, name the exact include,
  allocation/storage model, lifecycle object lifetime, stack buffer lifetime,
  priority, core affinity, startup ordering, stop/deinit path, and failure
  cleanup behavior.
- Keep timers, queues, callbacks, bridge request dispatch, persistence, logging,
  file output, Wi-Fi/ESP-NOW/BLE startup, flash, monitor, and serial writes
  closed unless a later named gate opens one of those surfaces.
- Preserve `mesh_discovery.v1`, Gate F service codes, custody codes, bridge
  request names, coordinator serial ABI, Win31 transport, M4K policy strings,
  and the M4L caller-provided static-buffer boundary.
- Require host tests or no-flash build evidence for any future source gate, plus
  closed-surface scans proving the named gate did not open unaccepted runtime,
  bridge, persistence, radio, serial, flash, file, logging, or allocation
  surfaces.
- Require same-session Tier 3 authority, identity, backups, hashes, recovery,
  transcript, and cleanup before any flash, monitor, serial-write, live task
  proof, or live mesh proof.

## Validation Status

- Pre-mutation reviewers approved the named M4M design-only boundary 8/8 with no
  P1/P2 blockers, conditional on keeping M4M docs/status-only and preserving all
  closed firmware/runtime/live surfaces.
- Post-mutation validation passed: `scaffold_audit_records.py --min-task-id
  198`, `scaffold_audit_sources.py`, `scaffold_audit_docs.py`,
  `scaffold_audit_agent_process.py`, `scaffold_audit_firmware.py`,
  `verify_scaffold.py`, no `sdkconfig*` paths, no generated
  `build/tests/espnow_bbs_mesh_discovery_contract`, stale authority scans, and
  `git diff --check`.
- Post-mutation reviewers approved accepting M4M as design-only requirements
  8/8 after record-finalization cleanup, with no P1/P2 safety/runtime blockers.

## Decision

M4M is accepted as design-only startup/lifecycle requirements after clean
scaffold validation and post-review. Future startup source implementation,
`app_main` integration, firmware runtime behavior, bridge dispatch, persistence,
first-flash proof, and M5 live mesh remain separate gates. This does not accept
automatic startup, firmware source implementation, `app_main` start,
task/timer/callback/queue implementation, firmware runtime behavior,
persistence, bridge dispatch, live mesh, BLE, Android, router/admin, flash,
serial, RF/XBee, relay/load/mains, release, publication, or deployment
authority.
