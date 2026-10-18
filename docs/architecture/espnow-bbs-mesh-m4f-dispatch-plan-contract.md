# ESP-NOW BBS Mesh M4F Dispatch Plan Contract Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4F is a Tier 2 inert firmware interface source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This proof extends the existing pure-C `mesh_discovery_contract` component with
a caller-owned dispatch-plan contract over the M4E owner state and M4D event
queue. It defines known local dispatch action names, fixed plan capacity,
budget bounds, deterministic drain/ingest/snapshot planning, per-step local
status, aggregate requested/planned/rejected counters, and a compact bounded
dispatch summary.

It does not implement firmware runtime behavior, actual FreeRTOS tasks, static
runtime queues, callbacks, dispatch loops, handler calls, bridge request
dispatch, serial command handling, persistence, Wi-Fi startup, ESP-NOW startup,
ESP-WIFI-MESH startup, BLE, Android behavior, router/admin behavior, PCAP,
flash, monitor, serial writes, RF/XBee writes, relay/load/mains work, release,
publication, commit, push, PR, or deploy.

## Verified Facts

- Gate M4E already accepted the caller-owned owner contract over the M4D event
  queue. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`.
- M4F keeps dispatch-plan state caller-owned in
  `espnow_bbs_mesh_contract_dispatch_plan_t`.
- The accepted local dispatch action names are `owner_drain`, `owner_ingest`,
  and `owner_snapshot`, with compact labels `od`, `oi`, and `os`.
- Dispatch action names are capped at 16 ASCII bytes, plans are capped at
  eight steps, and requested budgets are capped at 16 units.
- Planning rejects unknown actions, over-limit action names, full plans, zero
  budgets, too-large budgets, unbound owners, unclaimed owners, and stale
  owner claim tokens.
- `dispatch_plan_from_owner` stages drain, ingest, then snapshot actions using
  only caller-owned owner and queue snapshots. It does not pop queue records,
  push queue records, call bridge handlers, transmit, start tasks, schedule
  handlers, or mutate owner counters.
- Dispatch summaries are compact local snapshots of schema, plan count,
  requested count, planned-ready count, rejected count, first action endpoint,
  last action endpoint, and last local status.
- The component host tests compile with `-Wall -Wextra -Werror`.

## Assumptions

- Caller-owned dispatch planning is the correct next firmware interface slice
  because it validates deterministic ordering and budget accounting before
  scheduler integration, actual task creation, persistence, bridge dispatch, or
  live APIs are introduced.
- A compact local dispatch summary is sufficient for this gate; bridge response
  shaping remains closed.

## Unknowns

- No firmware runtime task graph, scheduler integration, memory budget
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
  passed with the expanded forbidden marker scan for allocation, task, queue,
  timer, callback, logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP,
  partition, flash, and OTA runtime markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py` passed.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4f-dispatch-plan build`
  passed. ESP-IDF reported IDF version `6.0`, target `esp32`, project
  `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550` bytes free
  in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs` and no repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` generated path.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for allocation, FreeRTOS task/queue/timer, callback,
  logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP, partition, flash,
  OTA, or selected runtime API markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 191`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py` passed.
- `git diff --check` passed.
- Initial post-review: Firmware/Communications/Architecture voted PASS with no
  P1/P2 findings. QA/evidence found one P2 stale-record finding because this
  proof still said final validation was pending after validation passed.
  Focused QA re-review then passed with no P1/P2 findings and confirmed the
  stale-record P2 was cleared. Weighted final disposition was 8/8 approval for
  the named M4F dispatch-plan contract boundary.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep owner, queue, and dispatch-plan state caller-owned; do not add static
  mutable runtime queues, dynamic allocation, callbacks, tasks, timers,
  dispatch loops, handler calls, or persistence.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

Accept the M4F dispatch-plan contract proof as a bounded Tier 2 step. This
proves a caller-owned dispatch plan over the M4E owner state and M4D event
queue can compile under host C tests and the ESP-IDF build-only target. It does
not accept scheduler integration, actual task creation, firmware runtime
behavior, persistence, bridge dispatch, live mesh, BLE, Android, router/admin,
flash, serial, RF/XBee, relay/load/mains, release, publication, or deployment
authority.
