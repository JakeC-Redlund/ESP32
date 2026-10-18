# ESP-NOW BBS Mesh M4N Startup Lifecycle Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4N is a Tier 2 ESP-IDF-only firmware source slice for the `espnow-bbs`
full-service mesh discovery lane accepted by `ADR-0003` and `ADR-0009`.

This packet adds a separate `mesh_startup_lifecycle` component that wraps the
accepted M4L caller-started static task shell in a caller-owned lifecycle API.
The lifecycle object embeds the M4L task object and copies caller-provided task
configuration pointers; callers still own the scheduler state, owner state,
`StaticTask_t`, `StackType_t`, stack buffer, stop/start decisions, and recovery
responsibility.

M4N does not edit `app_main`, start automatically, add static runtime state,
add timers, queues, callbacks, bridge request dispatch, serial command handling,
persistence, logging, file output, Wi-Fi startup, ESP-NOW startup,
ESP-WIFI-MESH startup, BLE, Android behavior, router/admin behavior, PCAP,
flash, monitor, serial writes, RF/XBee writes, relay/load/mains work, release,
publication, commit, push, PR, or deploy.

## Verified Facts

- Gate M4M accepted design-only startup/lifecycle requirements for a later
  source gate, including lifecycle states, transition handling, caller-owned
  state, task-buffer lifetime, failure cleanup, and validation. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`.
- Gate M4L accepted only a caller-started static FreeRTOS task shell source API
  with caller-provided `StaticTask_t` and `StackType_t` buffers. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`.
- Gate M4K accepted only a caller-owned runtime scheduler source contract,
  preserving `caller_invoked`, `task_none`, and `timer_none` policy strings.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`.
- `firmware/projects/espnow-bbs/main/main.c` remains inert and does not include
  or start the M4L task shell or the M4N lifecycle component.
- ESP-IDF FreeRTOS source coverage for task ownership, core affinity, byte-sized
  stack depths, scheduler/tick behavior, and task cleanup/deletion concerns is
  already source-indexed. Source ID: `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`.

## Assumptions

- A caller-owned lifecycle wrapper is the correct next source step because it
  proves transition semantics without opening automatic startup, bridge
  dispatch, persistence, live task proof, or live mesh proof.
- Caller-owned task buffers and caller-owned M4K/M4L state remain the correct
  boundary until a later gate explicitly accepts a static runtime object or
  `app_main` integration.
- A `running` lifecycle state means the M4L start API accepted and returned a
  task handle; it is not live task timing or scheduler-execution proof.

## Unknowns

- No automatic startup from `app_main`, live task timing, watchdog behavior,
  stack adequacy, priority adequacy, route-table behavior, root/parent
  selection, healing, coexistence, BLE/GATT, Android behavior, router/admin
  policy, PCAP, transcript, rollback, cleanup, first-flash target, or live proof
  is accepted by this gate.

## Accepted Source Semantics

- Lifecycle owner object:
  `espnow_bbs_mesh_startup_lifecycle_t` is caller-owned and embeds the M4L
  `espnow_bbs_mesh_runtime_task_t` object.
- Lifecycle states:
  `uninitialized`, `configured`, `starting`, `running`, `stop_requested`,
  `stopped`, and `failed`.
- Configure is accepted only from `uninitialized` or `stopped`; rejected
  configure attempts do not mutate the lifecycle state.
- Start is accepted only from `configured`; repeated start from `starting`,
  `running`, `stop_requested`, `stopped`, or `failed` is rejected without
  mutation.
- Stop before start is rejected. Repeated stop while `stop_requested` is
  idempotent and does not issue another M4L stop request.
- `stop_requested` transitions to `stopped` only after polling observes the M4L
  task handle cleared and M4L no longer reports running.
- Restart after `stopped` requires configure again, renewing the caller's task
  buffer and pointer lifetime assertion.
- Task-create failure moves the lifecycle to `failed`, clears copied task
  config, resets the embedded M4L task object, and increments failure
  accounting.
- Deinit is accepted from `configured`, `stopped`, or `failed`; it clears copied
  task config, resets the embedded M4L task object, and returns to
  `uninitialized`. Deinit is rejected from `running`, `starting`, and
  `stop_requested`.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep M4K `caller_invoked`, `task_none`, and `timer_none` policy strings
  unchanged.
- Keep task buffers, scheduler state, owner state, and lifecycle control
  caller-owned.
- Keep timers, callbacks, queues, bridge dispatch, persistence, logging, file
  output, Wi-Fi/ESP-NOW/BLE startup, flash, monitor, and serial writes closed.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.

## Validation Status

- Pre-mutation reviewers approved the named M4N source boundary 8/8 with no
  P1/P2 blockers, conditional on focused audit coverage, durable records, no
  `app_main` integration, and preservation of all closed runtime/live surfaces.
- Focused scaffold tests and firmware/path audits passed during implementation.
- Full scaffold validation, no-flash ESP-IDF v6.0.1 build, generated-output
  cleanup, stale authority scans, and `git diff --check` passed. The no-flash
  build generated `espnow_bbs_build_only.bin` size `0x21ab0` with `0xde550`
  bytes free in the smallest app partition under
  `/tmp/esp32-espnow-bbs-m4n-startup-lifecycle-source-api`.
- Initial post-review found stale status wording only; after record remediation,
  final post-review passed 8/8 weighted approval with no P1/P2 blockers.

## Decision

M4N is accepted for the caller-owned startup lifecycle source API only. Future
`app_main` integration, firmware runtime acceptance, bridge dispatch,
persistence, first-flash proof, and M5 live mesh remain separate gates.
