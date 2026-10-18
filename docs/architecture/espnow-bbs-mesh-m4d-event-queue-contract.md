# ESP-NOW BBS Mesh M4D Event Queue Contract Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4D is a Tier 2 inert firmware interface source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This proof extends the existing pure-C `mesh_discovery_contract` component with
a caller-owned, fixed-size discovery-event queue contract. The queue validates
the accepted `mesh_discovery.v1` event vocabulary, keeps event records in
caller-provided memory, exposes FIFO push/peek/pop helpers, and provides a
bounded local summary helper.

It does not implement firmware runtime behavior, FreeRTOS tasks, callbacks,
dispatch loops, bridge request dispatch, serial command handling, persistence,
Wi-Fi startup, ESP-NOW startup, ESP-WIFI-MESH startup, BLE, Android behavior,
router/admin behavior, PCAP, flash, monitor, serial writes, RF/XBee writes,
relay/load/mains work, release, publication, commit, push, PR, or deploy.

## Verified Facts

- Gate M4C already accepted the inert firmware-side constants/helpers for
  `mesh_discovery.v1`. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`.
- M4D keeps the queue state caller-owned in
  `espnow_bbs_mesh_contract_event_queue_t` and uses fixed-size arrays only.
- The accepted capacity is 16 event records. Node IDs are capped at 8 ASCII
  bytes and event detail strings are capped at 32 ASCII bytes.
- Queue ingress rejects null inputs, unknown events, non-ASCII event/node/detail
  values, over-limit node/detail values, and full queues.
- Full queues return `ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL`, increment an
  explicit dropped counter, and do not silently overwrite or enqueue records.
- The summary helper returns a compact local snapshot of schema, count, dropped
  count, oldest event label/node, and newest event label/node. It does not add a
  bridge request, serial ABI, Gate F service, Win31 transport, or schema change.
- The component host tests compile with `-Wall -Wextra -Werror`.
- The no-flash ESP-IDF build passed with IDF version `6.0`, default target
  `esp32`, project `espnow_bbs_build_only`, and generated
  `/tmp/esp32-espnow-bbs-m4d-event-queue/espnow_bbs_build_only.bin`.

## Assumptions

- Caller-owned queue state is the correct next firmware interface slice because
  it validates bounded ingress/read behavior before selecting runtime task
  ownership, scheduler integration, persistence, or live APIs.
- A compact local summary is sufficient for this gate; detailed inspection uses
  peek/pop records, and bridge response shaping remains closed.

## Unknowns

- No firmware runtime task graph, memory budget acceptance, scheduler
  integration, persistence, recovery behavior, first-flash target, live route
  table, root/parent selection, healing, coexistence, BLE/GATT, Android app,
  router/admin policy, PCAP, transcript, rollback, cleanup, or live proof is
  accepted by this gate.

## Accepted Proof

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
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4d-event-queue build`
  passed. ESP-IDF reported app binary size `0x21ab0` and `0xde550` bytes free
  in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 189`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py` passed.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for the expanded forbidden marker set.
- `git diff --check` passed.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`, and the old repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` path is absent.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep queue state caller-owned; do not add static mutable runtime queues,
  dynamic allocation, callbacks, tasks, timers, dispatch loops, or persistence.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

Accept the M4D event queue contract proof as a bounded Tier 2 step. This proves
a fixed-size caller-owned discovery-event queue interface and local snapshot
helper can compile under host C tests and the ESP-IDF build-only target. It does
not accept firmware runtime behavior, persistence, bridge dispatch, live mesh,
BLE, Android, router/admin, flash, serial, RF/XBee, relay/load/mains, release,
publication, or deployment authority.
