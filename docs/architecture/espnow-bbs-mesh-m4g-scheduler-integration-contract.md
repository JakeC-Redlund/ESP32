# ESP-NOW BBS Mesh M4G Scheduler Integration Contract Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4G is a Tier 2 inert firmware interface source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This proof extends the existing pure-C `mesh_discovery_contract` component with
a caller-owned scheduler integration contract over the M4F dispatch-plan
contract. It defines fixed schedule profile names, bounded profile validation,
a bounded tick value, caller-owned scheduler state, deterministic staging of an
M4F dispatch plan, aggregate requested/planned/rejected counters, and a compact
bounded scheduler summary.

It does not implement firmware runtime behavior, actual FreeRTOS tasks, static
runtime queues, callbacks, dispatch loops, handler calls, bridge request
dispatch, serial command handling, persistence, Wi-Fi startup, ESP-NOW startup,
ESP-WIFI-MESH startup, BLE, Android behavior, router/admin behavior, PCAP,
flash, monitor, serial writes, RF/XBee writes, relay/load/mains work, release,
publication, commit, push, PR, or deploy.

## Verified Facts

- Gate M4F already accepted the caller-owned dispatch-plan contract over the
  M4E owner state and M4D event queue. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`.
- M4G keeps scheduler state caller-owned in
  `espnow_bbs_mesh_contract_scheduler_state_t`.
- The accepted local schedule profile names are `balanced`, `drain_first`, and
  `snapshot_only`.
- Schedule profile names are capped at 16 ASCII bytes, and ticks are capped at
  64.
- `scheduler_stage_tick` validates the profile and tick, delegates to the M4F
  `dispatch_plan_from_owner` helper, records local counters in caller-owned
  scheduler state, and returns no runtime handle or live action.
- The M4G contract does not pop queue records, push queue records, call bridge
  handlers, transmit, start tasks, schedule handlers, or mutate owner counters.
- Scheduler summaries are compact local snapshots of schema, profile, owner,
  tick count, last tick, requested count, planned-ready count, rejected count,
  and last local status.
- The component host tests compile with `-Wall -Wextra -Werror`.

## Assumptions

- Caller-owned schedule staging is the correct next firmware interface slice
  because it validates deterministic profile-to-plan accounting before actual
  task creation, firmware runtime scheduling, persistence, bridge dispatch, or
  live APIs are introduced.
- A compact local scheduler summary is sufficient for this gate; bridge
  response shaping remains closed.

## Unknowns

- No firmware runtime task graph, actual scheduler loop, memory budget
  acceptance, persistence, recovery behavior, first-flash target, live route
  table, root/parent selection, healing, coexistence, BLE/GATT, Android app,
  router/admin policy, PCAP, transcript, rollback, cleanup, or live proof is
  accepted by this gate.

## Completed Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed. The host test binary is generated under
  `/tmp/esp32-espnow-bbs-mesh-discovery-contract-host-tests`, outside the
  repository.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with expanded forbidden marker coverage for allocation, FreeRTOS
  task/queue/timer/handle markers, callback/handler markers, logging, Wi-Fi,
  ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP, partition, flash, and OTA runtime
  markers.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4g-scheduler-contract build`
  passed. ESP-IDF reported IDF version `6.0`, target `esp32`, project
  `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550` bytes free
  in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep queue, owner, dispatch-plan, and scheduler state caller-owned; do not
  add static mutable runtime queues, dynamic allocation, callbacks, tasks,
  timers, dispatch loops, handler calls, or persistence.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

Accept the M4G scheduler integration contract proof as a bounded Tier 2 step.
This proves a caller-owned schedule profile and tick contract can stage the M4F
dispatch plan under host C tests and the ESP-IDF build-only target. Final
post-review completed with 8/8 weighted approval and no P1/P2 findings. It
does not accept actual scheduler runtime behavior, task creation, firmware
runtime behavior, persistence, bridge dispatch, live mesh, BLE, Android,
router/admin, flash, serial, RF/XBee, relay/load/mains, release, publication,
or deployment authority.
