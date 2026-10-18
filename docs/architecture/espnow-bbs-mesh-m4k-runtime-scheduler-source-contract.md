# ESP-NOW BBS Mesh M4K Runtime Scheduler Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4K is a Tier 2 inert firmware interface source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This packet extends the existing pure-C `mesh_discovery_contract` component
with a caller-owned runtime scheduler source contract. The contract names the
scheduler owner object as
`espnow_bbs_mesh_contract_runtime_scheduler_t`, stores only caller-provided
configuration, and advances the existing M4I runtime-cycle contract only when a
caller explicitly invokes `runtime_scheduler_step`.

The only accepted policies in this source slice are `caller_invoked`,
`task_none`, and `timer_none`. The contract rejects selected core affinity,
priority, stack budget, and static-state requests because this gate does not
open actual task or timer ownership.

It does not implement firmware runtime behavior, actual scheduler runtime,
actual FreeRTOS task creation, timer creation, static runtime queues,
callback dispatch, dispatch loops, bridge request dispatch, serial command
handling, persistence, Wi-Fi startup, ESP-NOW startup, ESP-WIFI-MESH startup,
BLE, Android behavior, router/admin behavior, PCAP, flash, monitor, serial
writes, RF/XBee writes, relay/load/mains work, release, publication, commit,
push, PR, or deploy.

## Verified Facts

- Gate M4I accepted caller-owned runtime-cycle composition over M4G scheduler
  staging and M4H execution accounting. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`.
- Gate M4J accepted only a design requirements packet for later runtime
  scheduler source work. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`.
- ESP-IDF FreeRTOS and ESP Timer source rows are available for later actual
  task/timer design decisions, but M4K does not call or create those runtime
  surfaces. Source IDs: `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`,
  `SRC-ESP-IDF-ESP-TIMER-2026-06-06`.
- M4K adds caller-owned `espnow_bbs_mesh_contract_runtime_scheduler_t` state
  and `espnow_bbs_mesh_contract_runtime_scheduler_config_t` configuration.
- M4K rejects any configuration that asks for nonzero priority, nonzero stack
  bytes, selected core affinity, static state, unknown policy strings, zero
  period/yield values, or over-limit period/yield values.
- M4K step execution first validates the configured owner claim and owner name,
  then delegates to the already accepted M4I runtime-cycle step through a local
  copy and commits the caller-owned scheduler object only after success.
- Rejected M4K configure and step paths do not mutate caller-owned scheduler
  state.
- M4K summaries are compact local snapshots of schema, source policy, owner,
  profile, step count, last cycle, scheduled/applied/skipped/rejected counters,
  yield due count, and last local status.
- The component host tests compile with `-Wall -Wextra -Werror`.

## Assumptions

- A caller-invoked source contract is the correct first source movement after
  M4J because it records task/timer policy decisions without opening actual
  task creation, timer creation, callbacks, bridge dispatch, persistence, or
  live proof.
- A compact local scheduler summary is sufficient for this gate; bridge
  response shaping remains closed.

## Unknowns

- No firmware runtime task graph, actual scheduler loop, memory budget
  acceptance, callback-to-task handoff, persistence, recovery behavior,
  first-flash target, live route table, root/parent selection, healing,
  coexistence, BLE/GATT, Android app, router/admin policy, PCAP, transcript,
  rollback, cleanup, or live proof is accepted by this gate.

## Validation Status

- Pre-mutation reviewers approved the named M4K source boundary 8/8 with no
  P1/P2 blockers.
- Host C tests passed with `-Wall -Wextra -Werror`.
- Focused and broader scaffold audit suites passed.
- The no-flash ESP-IDF v6.0.1 build passed and generated
  `espnow_bbs_build_only.bin` size `0x21ab0` with `0xde550` bytes free in the
  smallest app partition.
- Generated `sdkconfig` output was removed, generated-output cleanup checks
  passed, the targeted closed-surface scan found no forbidden runtime markers,
  and `git diff --check` passed.
- Post-implementation reviewers approved the source boundary 8/8 with no
  P1/P2 implementation blockers; their stale-record P2 findings are addressed
  by this acceptance update.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep queue, owner, dispatch-plan, scheduler, execution-accounting,
  runtime-cycle, and runtime-scheduler state caller-owned; do not add static
  mutable runtime queues, dynamic allocation, callbacks, tasks, timers,
  dispatch loops, persistence, or file output.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

M4K is accepted as a bounded Tier 2 caller-owned runtime scheduler source
contract over M4I runtime-cycle composition. This packet does not accept actual
scheduler runtime behavior, task creation, timer creation, firmware runtime
behavior, persistence, bridge dispatch, live mesh, BLE, Android, router/admin,
flash, serial, RF/XBee, relay/load/mains, release, publication, or deployment
authority.
