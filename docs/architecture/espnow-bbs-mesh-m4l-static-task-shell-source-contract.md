# ESP-NOW BBS Mesh M4L Static Task Shell Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4L is a Tier 2 ESP-IDF-only firmware source slice for the `espnow-bbs`
firmware lane accepted by `ADR-0003`.

This packet adds a separate `mesh_runtime_task_shell` component. The component
does not change the M4K caller-owned runtime scheduler contract. Instead, it
provides an explicit caller-started FreeRTOS static task shell that invokes the
existing M4K scheduler step from a caller-owned task context.

The only accepted task creation API in this source slice is
`xTaskCreateStaticPinnedToCore` with caller-provided `StaticTask_t` and
`StackType_t` buffers. The component rejects null owner/scheduler/buffer inputs,
zero stack depth, zero or over-limit period ticks, invalid core IDs, zero
priority, duplicate starts, and stop requests before start.

The shell is not called from `app_main`. It does not add timers, queues,
callbacks, bridge request dispatch, serial command handling, persistence,
logging, Wi-Fi startup, ESP-NOW startup, ESP-WIFI-MESH startup, BLE, Android
behavior, router/admin behavior, PCAP, flash, monitor, serial writes, RF/XBee
writes, relay/load/mains work, release, publication, commit, push, PR, or
deploy.

## Verified Facts

- Gate M4J required any later source gate to name task ownership, core affinity,
  priority, stack budget, timer choice, caller-owned/static state rules,
  bounded backpressure, bridge-visible counters, watchdog/yield behavior,
  cleanup/deinit, and validation. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`.
- Gate M4K accepted only a caller-owned runtime scheduler source contract with
  `caller_invoked`, `task_none`, and `timer_none` policy strings. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`.
- ESP-IDF FreeRTOS exposes `xTaskCreateStaticPinnedToCore`, including the
  ESP-IDF byte-sized stack-depth convention. Source ID:
  `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`.
- M4L adds `espnow_bbs_mesh_runtime_task_config_t` and
  `espnow_bbs_mesh_runtime_task_t` in a separate component; M4K contract policy
  strings remain unchanged.
- `firmware/projects/espnow-bbs/main/main.c` remains inert and does not include
  or start the runtime task shell.
- The M4L loop only waits on task notification, calls
  `espnow_bbs_mesh_contract_runtime_scheduler_step`, updates local counters, and
  exits after an explicit stop request.

## Assumptions

- A caller-started static task shell is the correct next source step after M4K
  because it proves compile-time FreeRTOS task-shell shape without automatic
  startup, timer ownership, bridge dispatch, persistence, or live behavior.
- Caller-owned task buffers and caller-owned M4K scheduler state remain the
  correct ownership boundary for this gate.
- No bridge-visible summary is added by M4L; bridge shaping remains a separate
  future gate.

## Unknowns

- No live task timing, watchdog behavior, stack adequacy, priority adequacy,
  route-table behavior, root/parent selection, healing, coexistence, BLE/GATT,
  Android behavior, router/admin policy, PCAP, transcript, rollback, cleanup,
  first-flash target, or live proof is accepted by this gate.

## Validation Status

- Pre-mutation reviewers approved the named M4L source boundary 8/8 with no
  P1/P2 blockers.
- Focused scaffold tests for the runtime task shell and mesh-discovery contract
  passed during implementation.
- Host C tests passed with `-Wall -Wextra -Werror`.
- The focused 47-test suite passed.
- Firmware/path/source/docs/records/agent-process audits passed.
- `verify_scaffold.py` passed.
- The no-flash ESP-IDF v6.0.1 build passed and generated
  `espnow_bbs_build_only.bin` size `0x21ab0` with `0xde550` bytes free in the
  smallest app partition.
- Generated `sdkconfig` output was removed, generated-output cleanup checks
  passed, closed-source/CMake scans found no forbidden runtime markers outside
  the exact M4L allowlist, and `git diff --check` passed.
- Post-review completed: the Firmware/Communications/Architecture reviewer
  recorded a conditional pass with no source-boundary P1/P2 blockers, the
  QA/evidence reviewer recorded PASS with no P1/P2 blockers, and stale-record
  remediation closes the named acceptance condition. Weighted post-review result
  is 8/8 for the M4L source-only boundary.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep M4K `caller_invoked`, `task_none`, and `timer_none` policy strings
  unchanged; the M4L FreeRTOS task shell is an external caller of M4K, not a
  rewrite of the M4K policy contract.
- Keep task buffers, scheduler state, owner state, and stop/start control
  caller-owned.
- Keep timers, callbacks, queues, bridge dispatch, persistence, logging, file
  output, Wi-Fi/ESP-NOW/BLE startup, flash, monitor, and serial writes closed.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.

## Decision

M4L is accepted as a caller-started static FreeRTOS task shell source API only.
Automatic startup, live task execution proof, timer creation, callback
implementation, firmware runtime behavior, persistence, bridge dispatch, live
mesh, BLE, Android, router/admin, flash, serial, RF/XBee, relay/load/mains,
release, publication, and deployment authority remain closed behind separate
gates.
