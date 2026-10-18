# ESP-NOW BBS Mesh M4I Runtime Cycle Contract Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4I is a Tier 2 inert firmware interface source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This proof extends the existing pure-C `mesh_discovery_contract` component with
a caller-owned runtime-cycle contract that composes M4G scheduler staging and
M4H execution accounting over caller-provided state. It uses one bounded
runtime-cycle value as both the scheduler tick and execution cycle, stages a
dispatch plan through temporary local copies, applies execution accounting
through temporary local copies, and commits the caller-owned scheduler, plan,
accounting, and runtime-cycle state only after the full cycle succeeds.

It does not implement firmware runtime behavior, actual scheduler runtime,
actual FreeRTOS tasks, static runtime queues, callbacks, dispatch loops,
handler calls, bridge request dispatch, serial command handling, persistence,
Wi-Fi startup, ESP-NOW startup, ESP-WIFI-MESH startup, BLE, Android behavior,
router/admin behavior, PCAP, flash, monitor, serial writes, RF/XBee writes,
relay/load/mains work, release, publication, commit, push, PR, or deploy.

## Verified Facts

- Gate M4G already accepted caller-owned scheduler profile/tick accounting that
  stages an M4F dispatch plan without mutating owner or queue state. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`.
- Gate M4H already accepted caller-owned execution accounting over an already
  staged M4F dispatch plan. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`.
- M4I keeps runtime-cycle state caller-owned in
  `espnow_bbs_mesh_contract_runtime_cycle_t`.
- Runtime cycles are capped at 64.
- `runtime_cycle_step` rejects null inputs, zero cycle, over-limit cycle,
  unknown scheduler profile, unclaimed owner, stale owner token, and any
  scheduler/accounting rejection before committing caller-owned state.
- Rejected M4I cycles do not mutate caller-owned runtime-cycle state,
  scheduler state, dispatch plan, or execution-accounting state.
- Accepted M4I cycles record local scheduled, applied, skipped, rejected,
  cycle-count, last-cycle, profile, owner, and last-status values.
- M4I does not pop queue records, push queue records, call bridge handlers,
  transmit, start tasks, schedule handlers, or mutate owner or queue state.
- Runtime-cycle summaries are compact local snapshots of schema, cycle count,
  profile, owner, last cycle, scheduled/applied/skipped/rejected counters, and
  last local status.
- The component host tests compile with `-Wall -Wextra -Werror`.

## Assumptions

- A source-level runtime-cycle composition contract is the correct next
  firmware interface slice because it proves deterministic local composition
  before actual task creation, runtime scheduler loops, persistence, bridge
  dispatch, or live APIs are introduced.
- A compact local runtime-cycle summary is sufficient for this gate; bridge
  response shaping remains closed.

## Unknowns

- No firmware runtime task graph, actual scheduler loop, memory budget
  acceptance, persistence, recovery behavior, first-flash target, live route
  table, root/parent selection, healing, coexistence, BLE/GATT, Android app,
  router/admin policy, PCAP, transcript, rollback, cleanup, or live proof is
  accepted by this gate.

## Completed Validation

- Host C tests passed with `-Wall -Wextra -Werror`.
- Focused and broader scaffold unittests passed, including the mesh-discovery
  contract audit, build-only target audit, and custom wireless protocol tests.
- Firmware, path, record, source, docs, and agent-process scaffold audits
  passed.
- `scripts/verify_scaffold.py` passed.
- The no-flash ESP-IDF v6.0.1 build passed in
  `/tmp/esp32-espnow-bbs-m4i-runtime-cycle-contract` with
  `espnow_bbs_build_only.bin` size `0x21ab0` and `0xde550` bytes free in the
  smallest app partition.
- Generated-output cleanup removed project `sdkconfig`; no `sdkconfig*` or
  host-test build output remained under the tracked project paths.
- Narrowed closed-runtime marker scans found no framework runtime APIs,
  allocation, tasks, queues, timers, callbacks/hooks/handlers, persistence, or
  live-control markers. The broader scan only surfaced existing closed-surface
  service labels `ble_pair` and `flash`.
- `git diff --check` passed.
- Post-implementation reviewers approved the source/evidence boundary 8/8
  after stale interim record wording was remediated.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep queue, owner, dispatch-plan, scheduler, execution-accounting, and
  runtime-cycle state caller-owned; do not add static mutable runtime queues,
  dynamic allocation, callbacks, tasks, timers, dispatch loops, handler calls,
  persistence, or file output.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

M4I is accepted as a bounded Tier 2 source slice for the caller-owned
runtime-cycle contract only. This packet does not accept actual scheduler
runtime behavior, task creation, firmware runtime behavior, persistence, bridge
dispatch, live mesh, BLE, Android, router/admin, flash, serial, RF/XBee,
relay/load/mains, release, publication, or deployment authority.
