# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4G Scheduler Integration Contract

## Scope

Tier 2 inert firmware interface source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger accepts only a caller-owned scheduler integration contract inside
the existing pure-C `mesh_discovery_contract` component, host C tests,
recursive firmware-audit coverage, and a no-flash local ESP-IDF build proof. It
does not accept firmware runtime implementation, actual FreeRTOS task creation,
static runtime queues, dispatch loops, callbacks, handler calls, firmware
persistence, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
router/admin mutation, flash, erase, monitor, serial writes, serial ABI
expansion, bridge ABI changes, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

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
- Gate M4F accepted caller-owned `espnow_bbs_mesh_contract_dispatch_plan_t`
  state over the M4E owner state and M4D event queue.
- This gate extends the same component with caller-owned
  `espnow_bbs_mesh_contract_scheduler_state_t` state.
- The accepted schedule profile names are `balanced`, `drain_first`, and
  `snapshot_only`; profile names are capped at 16 ASCII bytes.
- Scheduler ticks are capped at 64.
- `scheduler_stage_tick` validates profile and tick input, stages an M4F
  dispatch plan from a claimed owner, records requested/planned/rejected
  counters locally, and does not own or start runtime work.
- Scheduler summaries are compact local snapshots and do not add a bridge
  request, Gate F service, coordinator serial ABI, Win31 transport, or schema
  change.
- `tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py` compiles the
  component source with `-Wall -Wextra -Werror`.

## Assumptions

- Scheduler integration contract proof is the correct next step after M4F
  because it validates deterministic profile-to-plan accounting before runtime
  APIs, task creation, persistence, or bridge dispatch are introduced.
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
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named M4G boundary; required caller-owned pure-C schedule profile/state/tick APIs only, no static runtime scheduler state, no task/queue/timer/handle use, no callback or handler dispatch, no ABI/schema drift, and no Tier 3 authority. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required host C tests, recursive firmware audit, no-flash build, scaffold audits, marker scans, generated-output cleanup, durable records, and post-review. |

Post-implementation read-only reviewers were waited and closed after output
capture. Firmware/Communications/Architecture voted PASS with no P1/P2
findings. QA/evidence voted PASS with no P1/P2 findings. Weighted final
disposition was 8/8 approval for the named M4G scheduler integration contract
boundary.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4g-scheduler-contract build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550`
  bytes free in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.

## Decision

Decision: accept the M4G scheduler integration contract proof only. Future
actual task creation, firmware runtime implementation, runtime scheduler
implementation, persistence, first-flash proof, and M5 live mesh remain
separate gates.
