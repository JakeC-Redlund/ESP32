# ESP-NOW BBS Mesh M4E Runtime Task Ownership Contract Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4E is a Tier 2 inert firmware interface source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This proof extends the existing pure-C `mesh_discovery_contract` component with
a caller-owned runtime/task ownership contract over the M4D event queue. It
defines known owner names, caller-owned owner state, claim/release tokens,
queue-bound ingress/drain helpers, owner counters, and a compact bounded owner
summary.

It does not implement firmware runtime behavior, actual FreeRTOS tasks, static
runtime queues, callbacks, dispatch loops, bridge request dispatch, serial
command handling, persistence, Wi-Fi startup, ESP-NOW startup,
ESP-WIFI-MESH startup, BLE, Android behavior, router/admin behavior, PCAP,
flash, monitor, serial writes, RF/XBee writes, relay/load/mains work, release,
publication, commit, push, PR, or deploy.

## Verified Facts

- Gate M4D already accepted the caller-owned event queue contract for accepted
  `mesh_discovery.v1` events. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`.
- M4E keeps ownership state caller-owned in
  `espnow_bbs_mesh_contract_owner_t`; the struct binds to a caller-provided
  `espnow_bbs_mesh_contract_event_queue_t`.
- The accepted owner names are `mesh_ingress`, `mesh_drain`, and
  `mesh_snapshot`.
- Owner names are capped at 16 ASCII bytes.
- Owner claims return caller-held claim tokens. Ingest, drain, and release
  reject unbound, unclaimed, already-claimed, unknown-owner, over-limit,
  non-ASCII, and stale-token states as appropriate.
- Owner counters track accepted ingress, rejected ingress, and drained records
  in caller-owned memory.
- The owner summary is a compact local snapshot of schema, owner, state,
  accepted input count, drained output count, rejected count, queue count,
  dropped queue count, and last status.
- The component host tests compile with `-Wall -Wextra -Werror`.
- The no-flash ESP-IDF build passed with IDF version `6.0`, default target
  `esp32`, project `espnow_bbs_build_only`, and generated
  `/tmp/esp32-espnow-bbs-m4e-runtime-task-ownership/espnow_bbs_build_only.bin`.

## Assumptions

- Caller-owned owner state is the correct next firmware interface slice because
  it validates ownership and backpressure semantics before selecting actual
  scheduler integration, task creation, persistence, dispatch, or live APIs.
- A compact local owner summary is sufficient for this gate; bridge response
  shaping remains closed.

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
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4e-runtime-task-ownership build`
  passed. ESP-IDF reported app binary size `0x21ab0` and `0xde550` bytes free
  in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for the expanded forbidden marker set.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 190`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py` passed.
- `git diff --check` passed.
- Post-implementation read-only reviewer quorum passed with 8/8 weighted
  approval and no P1/P2 findings.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep owner and queue state caller-owned; do not add static mutable runtime
  queues, dynamic allocation, callbacks, tasks, timers, dispatch loops, or
  persistence.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

Accept the M4E runtime/task ownership contract proof as a bounded Tier 2 step.
This proves a caller-owned owner contract over the M4D event queue can compile
under host C tests and the ESP-IDF build-only target. It does not accept actual
task creation, scheduler integration, firmware runtime behavior, persistence,
bridge dispatch, live mesh, BLE, Android, router/admin, flash, serial, RF/XBee,
relay/load/mains, release, publication, or deployment authority.
