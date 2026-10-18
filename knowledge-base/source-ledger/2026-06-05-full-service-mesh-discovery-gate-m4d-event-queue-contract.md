# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4D Event Queue Contract

## Scope

Tier 2 inert firmware interface source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger accepts only a caller-owned fixed-size discovery-event queue
contract inside the existing pure-C `mesh_discovery_contract` component, host C
tests, recursive firmware-audit coverage, and a no-flash local ESP-IDF build
proof. It does not accept firmware runtime implementation, firmware
persistence, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
router/admin mutation, flash, erase, monitor, serial writes, serial ABI
expansion, bridge ABI changes, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

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
- Gate M4C accepted the inert pure-C firmware contract component under
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`.
- This gate extends that component with caller-owned
  `espnow_bbs_mesh_contract_event_queue_t` state and fixed-size event
  input/record structs.
- The accepted queue capacity is 16 records; node IDs are capped at 8 ASCII
  bytes; event detail strings are capped at 32 ASCII bytes.
- Queue ingress rejects null inputs, unknown events, non-ASCII event/node/detail
  values, over-limit node/detail values, and full queues.
- Full queues increment an explicit dropped counter and return
  `ESPNOW_BBS_MESH_CONTRACT_REJECT_QUEUE_FULL` without overwriting records.
- Queue peek/pop preserve FIFO order and monotonic sequence numbers within the
  caller-owned queue.
- Queue summaries are compact local snapshots and do not add a bridge request,
  Gate F service, coordinator serial ABI, Win31 transport, or schema change.
- `tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py` compiles the
  component source with `-Wall -Wextra -Werror`.
- The local no-flash ESP-IDF build passed from `/mnt/h/ESP32` using
  `/home/cyber/.espressif/tools/activate_idf_v6.0.1.sh`, `IDF_PATH`
  `/home/cyber/Espressif/v6.0.1/esp-idf`, default target `esp32`, and build
  directory `/tmp/esp32-espnow-bbs-m4d-event-queue`.
- ESP-IDF generated
  `/tmp/esp32-espnow-bbs-m4d-event-queue/espnow_bbs_build_only.bin` with binary
  size `0x21ab0` and `0xde550` bytes free in the smallest app partition. These
  generated files are not tracked source artifacts.

## Assumptions

- Event queue contract proof is the correct next step after M4C because it
  validates bounded ingress/read behavior before runtime APIs, task ownership,
  persistence, or bridge dispatch are introduced.
- Future runtime implementation will use separate gates tied to Gate F and
  `mesh_discovery.v1` invariants.

## Unknowns

- No runtime task graph, scheduler ownership, queue budget acceptance, memory
  budget, persistence, first-flash target, recovery path, live route-table
  proof, root/parent selection, healing, coexistence, BLE/GATT, Android,
  router/admin, PCAP, transcript, cleanup, rollback, or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named M4D boundary; required caller-owned fixed-size C structs, explicit capacity/overflow behavior, host C tests, expanded runtime/allocation marker coverage, and no ABI/schema drift. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required host C tests, recursive firmware audit, no-flash build, scaffold audits, marker scans, generated-output cleanup, and durable records. |

Post-implementation read-only reviewers were also waited and closed after
output capture. Firmware/Communications/Architecture voted PASS, QA/evidence
voted PASS with no P1/P2 blockers, and weighted disposition was 8/8 approval
for the named M4D event queue contract boundary.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4d-event-queue build`
  passed with exit code 0.
- The generated project `sdkconfig` was removed after the build.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 189`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for allocation, FreeRTOS task/queue/timer, callback,
  logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP, partition, flash,
  OTA, or selected runtime API markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed.
- `git diff --check` passed with exit code 0.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`, and the repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` generated path is absent.

## Decision

Decision: accept the M4D event queue contract proof only. Future firmware
runtime implementation, scheduler/task ownership, persistence, first-flash
proof, and M5 live mesh remain separate gates.
