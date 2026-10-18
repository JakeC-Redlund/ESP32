# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4E Runtime Task Ownership Contract

## Scope

Tier 2 inert firmware interface source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger accepts only a caller-owned runtime/task ownership contract inside
the existing pure-C `mesh_discovery_contract` component, host C tests,
recursive firmware-audit coverage, and a no-flash local ESP-IDF build proof.
It does not accept firmware runtime implementation, actual FreeRTOS task
creation, static runtime queues, dispatch loops, callbacks, firmware
persistence, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
router/admin mutation, flash, erase, monitor, serial writes, serial ABI
expansion, bridge ABI changes, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

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
- Gate M4D accepted caller-owned
  `espnow_bbs_mesh_contract_event_queue_t` state and fixed-size event
  input/record structs inside the inert pure-C component.
- This gate extends that component with caller-owned
  `espnow_bbs_mesh_contract_owner_t` state bound to a caller-provided queue.
- The accepted owner names are `mesh_ingress`, `mesh_drain`, and
  `mesh_snapshot`; owner names are capped at 16 ASCII bytes.
- Owner claims return caller-held tokens. Release, ingest, and drain reject
  stale tokens.
- Owner ingress/drain helpers maintain accepted, rejected, and drained counters
  without adding actual tasks, dispatch loops, callbacks, persistence, static
  mutable runtime queues, or dynamic allocation.
- Owner summaries are compact local snapshots and do not add a bridge request,
  Gate F service, coordinator serial ABI, Win31 transport, or schema change.
- `tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py` compiles the
  component source with `-Wall -Wextra -Werror`.
- The local no-flash ESP-IDF build passed from `/mnt/h/ESP32` using
  `/home/cyber/.espressif/tools/activate_idf_v6.0.1.sh`, `IDF_PATH`
  `/home/cyber/Espressif/v6.0.1/esp-idf`, default target `esp32`, and build
  directory `/tmp/esp32-espnow-bbs-m4e-runtime-task-ownership`.
- ESP-IDF generated
  `/tmp/esp32-espnow-bbs-m4e-runtime-task-ownership/espnow_bbs_build_only.bin`
  with binary size `0x21ab0` and `0xde550` bytes free in the smallest app
  partition. These generated files are not tracked source artifacts.

## Assumptions

- Runtime/task ownership contract proof is the correct next step after M4D
  because it validates caller ownership and backpressure accounting before
  runtime APIs, task creation, scheduler integration, persistence, or bridge
  dispatch are introduced.
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
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named M4E boundary; required caller-owned pure-C ownership only, no actual task creation/startup, known owner names, claim/release tokens, counter tests, no ABI/schema drift, and no Tier 3 authority. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required host C tests, recursive firmware audit, no-flash build, scaffold audits, marker scans, generated-output cleanup, and durable records. |

Post-implementation read-only reviewers were also waited and closed after
output capture. Firmware/Communications/Architecture voted PASS with no P1/P2
findings, QA/evidence voted PASS with no P1/P2 findings, and weighted
disposition was 8/8 approval for the named M4E runtime/task ownership contract
boundary.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches for allocation, FreeRTOS task/queue/timer, callback,
  logging, Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO, HTTP, partition, flash,
  OTA, or selected runtime API markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4e-runtime-task-ownership build`
  passed with exit code 0.
- The generated project `sdkconfig` was removed after the build.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 190`
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
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`, and the repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` generated path is absent.

## Decision

Decision: accept the M4E runtime/task ownership contract proof only. Future
firmware runtime implementation, scheduler integration, actual task creation,
persistence, first-flash proof, and M5 live mesh remain separate gates.
