# ESP-NOW BBS Mesh M4O App Main Startup Integration

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4O is a Tier 2 ESP-IDF-only firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It opens a narrow no-flash `app_main`
startup integration over the accepted M4N startup lifecycle source API.

This packet does not accept live task execution proof, firmware runtime
acceptance, bridge dispatch, persistence, generated build output, `sdkconfig`,
flash arguments, flash, erase, monitor, serial writes, serial ABI expansion,
ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app behavior,
router/admin mutation, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Verified Facts

- M4N is accepted as a caller-owned startup lifecycle source API around M4L.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`.
- M4L is accepted as a static FreeRTOS task shell using caller-provided
  `StaticTask_t` and `StackType_t` buffers. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`.
- M4K is accepted as a caller-owned runtime scheduler source contract preserving
  the `caller_invoked`, `task_none`, and `timer_none` policy strings. Source
  ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`.
- M4M required any future `app_main` integration gate to name the include,
  allocation/storage model, lifecycle object lifetime, stack buffer lifetime,
  priority, core affinity, startup ordering, stop/deinit path, and failure
  cleanup behavior. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`.
- M4O changes `firmware/projects/espnow-bbs/main/main.c` and
  `firmware/projects/espnow-bbs/main/CMakeLists.txt` only for the firmware
  startup source boundary.

## Assumptions

- Static file-scope state in `main.c` is the correct source boundary for this
  first `app_main` integration because it preserves explicit object and buffer
  lifetime without dynamic allocation.
- The no-flash build can prove source integration and compile-time wiring only.
- Runtime behavior remains unaccepted until a separate Tier 3 gate provides
  same-session identity, recovery, transcript, and cleanup evidence.

## Unknowns

- No live task timing, watchdog behavior, stack adequacy, priority adequacy,
  shutdown timing, bridge-visible lifecycle summary, first-flash target, route
  table, root/parent behavior, healing, coexistence, BLE/GATT, Android behavior,
  router/admin policy, PCAP, transcript, rollback, cleanup, or live proof is
  accepted.

## Integration Boundary

- Include: `espnow_bbs_mesh_startup_lifecycle.h`.
- Storage model: file-scope static state in `main.c` for the event queue,
  owner, runtime scheduler, startup lifecycle, `StaticTask_t`, and
  `StackType_t` stack.
- Lifecycle object lifetime: static for the process lifetime after `app_main`.
- Stack buffer lifetime: static for the process lifetime after `app_main`.
- Owner/profile: `mesh_ingress` with `balanced`.
- M4K policy strings: `caller_invoked`, `task_none`, and `timer_none`.
- Period/yield: one tick period and two-step yield interval.
- Priority/core: `tskIDLE_PRIORITY + 1u` and
  `ESPNOW_BBS_MESH_RUNTIME_TASK_CORE_NONE`.
- Startup ordering: initialize event queue, initialize owner, initialize runtime
  scheduler, claim owner, configure runtime scheduler, initialize lifecycle,
  configure lifecycle with the M4L task config, then start lifecycle.
- Failure cleanup: M4N handles lifecycle task-create failure cleanup. Contract
  setup failure returns `REJECT_TASK_CONFIG` without claiming runtime or live
  behavior.
- Stop/deinit path: no automatic stop/deinit is accepted by M4O; future stop,
  recovery, or shutdown handling remains a separate gate.

## Validation Status

- Pre-mutation reviewers approved opening the named M4O source boundary 8/8
  with no P1/P2 blockers, conditional on narrow audit allowlists, durable
  records, no live/runtime claims, no bridge/persistence/radio/serial/flash
  authority, no stale M4N wording, and no generated-output residue.
- Focused scaffold tests, firmware/path/record/source/docs/agent-process audits,
  host contract tests, `verify_scaffold.py`, no-flash ESP-IDF v6.0.1 build,
  generated-output cleanup, stale scans, closed-surface scan, and
  `git diff --check` passed.
- The no-flash build generated
  `/tmp/esp32-espnow-bbs-m4o-app-main-startup-integration-source-contract/espnow_bbs_build_only.bin`
  size `0x22d50`, with `0xdd2b0` bytes free in the 1 MiB app partition.
- Final read-only post-review passed 8/8 weighted approval with no P1/P2
  blockers after record-finalization conditions were identified.

## Decision

M4O is accepted for the no-flash `app_main` startup integration source contract
only. Firmware runtime behavior, bridge dispatch, persistence, first-flash proof,
and M5 live mesh remain separate gates.
