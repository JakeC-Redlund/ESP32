# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4I Runtime Cycle Contract

## Scope

Tier 2 inert firmware interface source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger records only a caller-owned runtime-cycle contract inside the
existing pure-C `mesh_discovery_contract` component, host C tests, recursive
firmware-audit coverage, docs/status records, and completed no-flash local
ESP-IDF build proof. It does not accept firmware runtime
implementation, actual FreeRTOS task creation, static runtime queues, dispatch
loops, callbacks, handler calls, firmware persistence, live ESP-WIFI-MESH, BLE
pairing, Android app behavior, router/admin mutation, flash, erase, monitor,
serial writes, serial ABI expansion, bridge ABI changes, PCAP, RF/XBee writes,
relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-START-PROJECT`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for the `espnow-bbs`
  coordinator/client firmware lane.
- Gate M4G accepted caller-owned scheduler staging over the M4F dispatch plan.
- Gate M4H accepted caller-owned execution accounting over an already staged
  M4F dispatch plan.
- This gate extends the same component with caller-owned
  `espnow_bbs_mesh_contract_runtime_cycle_t` state.
- Runtime cycles are capped at 64.
- `runtime_cycle_step` stages scheduler output and execution accounting through
  local temporary copies and commits caller-owned scheduler, dispatch-plan,
  accounting, and runtime-cycle state only after the composed cycle succeeds.
- Rejected runtime cycles do not mutate caller-owned runtime-cycle, scheduler,
  dispatch-plan, or execution-accounting state.
- Accepted runtime cycles record scheduled, applied, skipped, rejected,
  cycle-count, last-cycle, profile, owner, and last-status values locally and
  do not own or start runtime work.
- Runtime-cycle summaries are compact local snapshots and do not add a bridge
  request, Gate F service, coordinator serial ABI, Win31 transport, or schema
  change.
- `tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py` compiles the
  component source with `-Wall -Wextra -Werror`.

## Assumptions

- Runtime-cycle contract proof is the correct next step after M4H because it
  validates deterministic local composition of scheduler staging and execution
  accounting before runtime APIs, task creation, persistence, or bridge
  dispatch are introduced.
- Future runtime scheduler implementation will use separate gates tied to Gate
  F and `mesh_discovery.v1` invariants.

## Unknowns

- No runtime task graph, actual scheduler loop, queue budget acceptance, memory
  budget, persistence, first-flash target, recovery path, live route-table
  proof, root/parent selection, healing, coexistence, BLE/GATT, Android,
  router/admin, PCAP, transcript, cleanup, rollback, or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named M4I boundary; required pure-C caller-owned composition only, no static runtime state, no allocation, no callbacks/hooks/handlers, no FreeRTOS tasks/queues/timers, no bridge dispatch, no schema/ABI drift, and no Tier 3 authority. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required atomic no-partial-mutation tests, host C tests, recursive firmware audit, no-flash build, scaffold audits, durable records, and post-review. |

After implementation and validation, two read-only post-implementation
reviewers were spawned, waited, and closed after output capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | conditional pass | No source-boundary P1/P2 findings. Confirmed pure-C caller-owned runtime-cycle composition, temporary-copy commit behavior, no owner/queue mutation, no schema/ABI/Gate F drift, and no static runtime state, allocation, callbacks/hooks/handlers, FreeRTOS tasks/queues/timers, bridge dispatch, persistence, live control, or Tier 3 authority. Required stale record alignment, now remediated. |
| QA/evidence reviewer | 3 | conditional pass | No evidence-structure P1 findings. Required stale interim record wording to be remediated, final validation evidence recorded, and final record/source/docs/diff audits rerun. |

Post-review weighted disposition is 8/8 approval after the stale-record P2 was
remediated by final record alignment. The firmware reviewer noted optional P3
future tightening for additional null-input no-mutation assertions; it is not a
gate blocker because current source handles null rejection paths and existing
tests cover atomic rejected-cycle/profile/owner behavior.

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
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 194`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4i-runtime-cycle-contract build`
  passed without flash/monitor/serial action. The build generated
  `espnow_bbs_build_only.bin` size `0x21ab0`; the smallest app partition had
  `0xde550` bytes free.
- Generated-output cleanup removed `firmware/projects/espnow-bbs/sdkconfig`;
  `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print` produced no
  output, and `test ! -e build/tests/espnow_bbs_mesh_discovery_contract`
  passed.
- A narrowed marker scan over tracked firmware source/CMake files found no
  framework runtime API, allocation, task/queue/timer, callback/hook/handler,
  persistence, logging, or live-control markers. A broader scan only found the
  expected closed-surface service-label strings `ble_pair` and `flash`.
- `git diff --check` passed before and after final record alignment.

## Decision

Decision: M4I is accepted as a bounded runtime-cycle contract proof only.
Future actual task creation, firmware runtime implementation, runtime scheduler
implementation, persistence, first-flash proof, and M5 live mesh remain
separate gates.
