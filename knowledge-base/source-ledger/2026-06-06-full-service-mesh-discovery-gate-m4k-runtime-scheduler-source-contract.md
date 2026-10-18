# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4K Runtime Scheduler Source Contract

## Scope

Tier 2 inert firmware interface source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger records a caller-owned runtime scheduler source contract inside
the existing pure-C `mesh_discovery_contract` component, host C tests,
recursive firmware-audit coverage, docs/status records, and pre-mutation
reviewer output. It does not accept actual FreeRTOS task creation, timer
creation, callback dispatch, static runtime queues, bridge dispatch, firmware
persistence, generated build output, `sdkconfig`, flash, erase, monitor,
serial writes, serial ABI expansion, ESP-NOW runtime, ESP-WIFI-MESH runtime,
BLE pairing, Android app behavior, router/admin mutation, PCAP, RF/XBee
writes, relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`
- `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`
- `SRC-ESP-IDF-ESP-TIMER-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-START-PROJECT`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- Gate M4J required a later source gate to name task ownership, core affinity,
  priority, stack budget, timer choice, caller-owned/static state rules,
  bounded backpressure, bridge-visible counters, watchdog/yield behavior,
  volatile state, cleanup/deinit behavior, and validation.
- M4K names the scheduler owner object as
  `espnow_bbs_mesh_contract_runtime_scheduler_t` and keeps it caller-owned.
- M4K accepts only `caller_invoked`, `task_none`, and `timer_none` policy
  strings.
- M4K rejects selected core affinity, nonzero priority, nonzero stack bytes,
  static-state requests, unknown policies, zero period/yield values, and
  over-limit period/yield values.
- M4K delegates source execution only to the existing M4I
  `espnow_bbs_mesh_contract_runtime_cycle_step` after validating the configured
  owner claim and owner name.
- Rejected M4K configure and step paths do not mutate caller-owned runtime
  scheduler state.
- M4K summaries remain compact local snapshots and do not add a bridge
  request, Gate F service, coordinator serial ABI, Win31 transport, or schema
  change.

## Assumptions

- Caller-invoked source composition is the correct source slice after M4J
  because actual task/timer ownership and firmware runtime behavior still need
  a later implementation gate.
- Future runtime scheduler implementation will use separate gates tied to Gate
  F and `mesh_discovery.v1` invariants.

## Unknowns

- No actual task graph, actual scheduler loop, timer lifecycle, callback
  dispatch, queue budget acceptance, memory budget, persistence, first-flash
  target, recovery path, live route-table proof, root/parent selection,
  healing, coexistence, BLE/GATT, Android, router/admin, PCAP, transcript,
  cleanup, rollback, or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named M4K boundary; required pure-C caller-owned source contract, only caller-invoked stepping, no task/timer creation, no allocation, no static mutable runtime ownership, no bridge dispatch, no schema/ABI/Gate F drift, audit marker updates, and no Tier 3 authority. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required audit-marker updates, no stale accepted wording before validation/post-review, explicit no-partial-mutation tests, host C tests, broader audits, no-flash build, generated-output checks, durable records, and post-review. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for starting
the named M4K source boundary.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed: `PASS: espnow_bbs_mesh_discovery_contract host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 196`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed.
- No-flash ESP-IDF v6.0.1 build passed with
  `idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4k-runtime-scheduler-source-contract build`;
  `espnow_bbs_build_only.bin` size was `0x21ab0` with `0xde550` bytes free in
  the smallest app partition.
- Generated `firmware/projects/espnow-bbs/sdkconfig` and `sdkconfig.old` were
  removed after the build; `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print`
  returned no output.
- `test ! -e build/tests/espnow_bbs_mesh_discovery_contract` passed.
- A targeted closed-surface scan over the M4K component, firmware main, and
  firmware CMake files returned no forbidden runtime markers for allocation,
  FreeRTOS tasks/queues/timers, callbacks/hooks, Wi-Fi/ESP-NOW/BLE, NVS, UART,
  GPIO, HTTP, flash/partition/OTA, file I/O, or logging.
- `git diff --check` passed.
- Host tests were expanded before final acceptance to assert invalid
  `task_policy` and `timer_policy` rejection.

## Post-Implementation Reviewer Quorum

Both post-implementation reviewers were waited and closed after output capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | source boundary pass | No P1 blockers and no implementation P2 blockers. Required durable records to move stale pending wording to accepted and suggested symmetric invalid task/timer policy assertions. |
| QA/evidence reviewer | 3 | source boundary pass | No P1 blockers and no implementation P2 blockers. Required durable records to record completed validation/post-review evidence and flagged the missing M4K source ID in the firmware README. |

Weighted post-review result: 8/8 source-boundary approval with no P1/P2
implementation blockers. Reviewer stale-record P2 findings are resolved by
this ledger, task, handoff, source-index, docs, and status update; the P3
task/timer policy test coverage suggestion was added before final validation.

## Decision

M4K is accepted as a caller-owned runtime scheduler source contract only.
Future actual task creation, timer creation, callback implementation, firmware
runtime implementation, persistence, first-flash proof, and M5 live mesh remain
separate gates.
