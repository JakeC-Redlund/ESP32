# ESP-NOW BBS Mesh M4H Execution Accounting Contract Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4H is a Tier 2 inert firmware interface source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This proof extends the existing pure-C `mesh_discovery_contract` component with
a caller-owned execution-accounting contract over an already staged M4F
dispatch plan. It defines a bounded execution cycle value, caller-owned
execution-accounting state, validation that ready plan steps are local and
owned by one accepted owner, aggregate apply/skip and ready-action counters,
and a compact bounded accounting summary.

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
- Gate M4G already accepted caller-owned scheduler profile/tick accounting that
  stages an M4F dispatch plan without mutating owner or queue state. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`.
- M4H keeps execution accounting caller-owned in
  `espnow_bbs_mesh_contract_execution_accounting_t`.
- Execution cycles are capped at 64.
- `execution_accounting_apply_plan` accepts only a non-empty caller-owned plan
  with ready steps for one accepted owner and rejects non-ready planned steps,
  all-skipped plans, mixed owners, unknown owners, unknown actions, zero
  cycles, and over-limit cycles.
- Accepted M4H accounting records local applied, skipped, drain-ready,
  ingest-ready, snapshot-ready, cycle-count, last-cycle, owner, and last-status
  values only.
- The M4H contract does not pop queue records, push queue records, call bridge
  handlers, transmit, start tasks, schedule handlers, or mutate owner,
  scheduler, dispatch-plan, or queue state.
- Execution summaries are compact local snapshots of schema, accounting count,
  owner, last cycle, apply/skip counters, ready-action counters, and last local
  status.
- The component host tests compile with `-Wall -Wextra -Werror`.

## Assumptions

- Caller-owned execution accounting is the correct next firmware interface
  slice because it validates deterministic local accounting over a staged plan
  before actual task creation, runtime scheduler loops, persistence, bridge
  dispatch, or live APIs are introduced.
- A compact local execution-accounting summary is sufficient for this gate;
  bridge response shaping remains closed.

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
  passed with expanded forbidden marker coverage for static FreeRTOS runtime
  objects, callback/hook/handler markers, file/persistence markers, BLE/NimBLE
  markers, allocation, logging, Wi-Fi, ESP-NOW, mesh, NVS, UART, GPIO, HTTP,
  partition, flash, and OTA runtime markers.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4h-execution-accounting-contract build`
  passed. ESP-IDF reported IDF version `6.0`, target `esp32`, project
  `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550` bytes free
  in the smallest app partition.
- The generated project `sdkconfig` was removed after the build, and no
  `sdkconfig*` files remain under `firmware/projects/espnow-bbs`.
- Record/source/docs/agent-process audits, full scaffold verification, direct
  closed-runtime marker scan, generated-output checks, and `git diff --check`
  passed.
- Post-implementation read-only reviewers were closed after output capture.
  Firmware/Communications/Architecture voted PASS with no P1/P2 findings.
  QA/evidence identified one P2 stale pending-record wording issue; this final
  record alignment remediates that stale wording without expanding authority.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep queue, owner, dispatch-plan, scheduler, and execution-accounting state
  caller-owned; do not add static mutable runtime queues, dynamic allocation,
  callbacks, tasks, timers, dispatch loops, handler calls, persistence, or
  file output.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

Accept the M4H execution-accounting contract proof as a bounded Tier 2 step.
This packet records caller-owned local execution accounting over the M4F
dispatch plan. It does not accept actual scheduler runtime behavior, task
creation, firmware runtime behavior, persistence, bridge dispatch, live mesh,
BLE, Android, router/admin, flash, serial, RF/XBee, relay/load/mains, release,
publication, or deployment authority.
