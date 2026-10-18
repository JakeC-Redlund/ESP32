# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4F Dispatch Plan Contract

## Scope

Tier 2 inert firmware interface source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger accepts only a caller-owned dispatch-plan contract inside the
existing pure-C `mesh_discovery_contract` component, host C tests, recursive
firmware-audit coverage, and a no-flash local ESP-IDF build proof. It does not
accept firmware runtime implementation, actual FreeRTOS
task creation, static runtime queues, dispatch loops, callbacks, handler calls,
firmware persistence, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
router/admin mutation, flash, erase, monitor, serial writes, serial ABI
expansion, bridge ABI changes, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

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
- Gate M4E accepted caller-owned `espnow_bbs_mesh_contract_owner_t` state over
  the M4D event queue.
- This gate extends the same component with caller-owned
  `espnow_bbs_mesh_contract_dispatch_plan_t` state.
- The accepted local dispatch action names are `owner_drain`, `owner_ingest`,
  and `owner_snapshot`; action names are capped at 16 ASCII bytes.
- Dispatch plans are capped at eight steps, and each requested budget is capped
  at 16 units.
- `dispatch_plan_from_owner` builds a deterministic drain, ingest, snapshot
  plan from a claimed owner and its caller-owned queue without popping/pushing
  queue records, mutating owner counters, transmitting, calling handlers, or
  starting runtime work.
- Dispatch summaries are compact local snapshots and do not add a bridge
  request, Gate F service, coordinator serial ABI, Win31 transport, or schema
  change.
- `tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py` compiles the
  component source with `-Wall -Wextra -Werror`.

## Assumptions

- Dispatch-plan contract proof is the correct next step after M4E because it
  validates deterministic ordering and budget accounting before runtime APIs,
  task creation, scheduler integration, persistence, or bridge dispatch are
  introduced.
- Future scheduler integration will use separate gates tied to Gate F and
  `mesh_discovery.v1` invariants.

## Unknowns

- No runtime task graph, scheduler integration, queue budget acceptance, memory
  budget, persistence, first-flash target, recovery path, live route-table
  proof, root/parent selection, healing, coexistence, BLE/GATT, Android,
  router/admin, PCAP, transcript, cleanup, rollback, or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named M4F boundary; required caller-owned pure-C dispatch planning only, deterministic action order, bounded budget accounting, no task creation/startup, no handler calls, no ABI/schema drift, and no Tier 3 authority. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required host C tests, recursive firmware audit, no-flash build, scaffold audits, marker scans, generated-output cleanup, durable records, and post-review. |

Initial post-implementation read-only reviewers were waited and closed after
output capture. Firmware/Communications/Architecture voted PASS with no P1/P2
findings. QA/evidence found one P2 validation-record inconsistency because the
durable records still said final validation was pending after the validation
commands had passed. After this ledger update recorded the completed evidence
packet, focused QA re-review was spawned, waited, and closed after output
capture. QA/evidence voted PASS with no P1/P2 findings and confirmed the P2
stale-record finding was cleared. Weighted final disposition was 8/8 approval
for the named M4F dispatch-plan contract boundary.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4f-dispatch-plan build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550`
  bytes free in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`, and the repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` generated path is absent.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for allocation, FreeRTOS task/queue/timer, callback,
  logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP, partition, flash,
  OTA, or selected runtime API markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 191`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed.
- `git diff --check` passed with exit code 0.

## Decision

Decision: accept the M4F dispatch-plan contract proof only. Future scheduler
integration, actual task creation, firmware runtime implementation,
persistence, first-flash proof, and M5 live mesh remain separate gates.
