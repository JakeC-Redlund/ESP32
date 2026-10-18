# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4C Firmware Contract

## Scope

Tier 2 inert firmware contract source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger accepts only a pure-C `mesh_discovery_contract` component, host C
tests, recursive firmware-audit coverage, and a no-flash local ESP-IDF build
proof. It does not accept firmware runtime implementation, firmware
persistence, live ESP-WIFI-MESH, BLE pairing, Android app behavior,
router/admin mutation, flash, erase, monitor, serial writes, serial ABI
expansion, PCAP, RF/XBee writes, relay/load/mains, release, publication,
commit, push, PR, or deploy.

## Source IDs

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
- Gate M4 accepted host-only adapter fixture proof, and Gate M4B accepted an
  inert ESP-IDF target/build proof.
- This gate adds
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/` with a
  pure-C header/source pair and component CMake file.
- The component mirrors the accepted `mesh_discovery.v1` schema/defaults,
  discovery request names, discovery event names, compact event labels,
  512-byte ASCII summary budget, Gate F bridge/service invariants, recursive
  field-path secret rejection, and blocked live-action names.
- `firmware/projects/espnow-bbs/main/main.c` links the component by calling
  `espnow_bbs_mesh_contract_event_count()` without logging, callbacks, queues,
  persistence, or runtime startup.
- `tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py` compiles the
  component source with `-Wall -Wextra -Werror`.
- The local no-flash ESP-IDF build passed from `/mnt/h/ESP32` using
  `/home/cyber/.espressif/tools/activate_idf_v6.0.1.sh`, `IDF_PATH`
  `/home/cyber/Espressif/v6.0.1/esp-idf`, default target `esp32`, and build
  directory `/tmp/esp32-espnow-bbs-m4c-contract`.
- ESP-IDF generated
  `/tmp/esp32-espnow-bbs-m4c-contract/espnow_bbs_build_only.bin` with binary
  size `0x21ab0` and `0xde550` bytes free in the smallest app partition. These
  generated files are not tracked source artifacts.

## Assumptions

- Firmware contract proof is the correct next step after M4B because it gives
  the build-only target a validated interface surface before runtime APIs are
  introduced.
- Future runtime implementation will use separate gates tied to Gate F and
  `mesh_discovery.v1` invariants.

## Unknowns

- No runtime task graph, queue budget, memory budget, persistence, first-flash
  target, recovery path, live route-table proof, root/parent selection,
  healing, coexistence, BLE/GATT, Android, router/admin, PCAP, transcript,
  cleanup, rollback, or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named pure-C contract boundary; required exact vocabulary/defaults, host tests, recursive runtime-marker coverage, and no runtime/linkage side effects. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required host C tests, drift checks, no-flash build, scaffold audits, marker scans, generated-output cleanup, and durable records. |

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4c-contract build`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 188`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- Runtime API marker scan over `firmware/projects/espnow-bbs` returned no
  matches for Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO-output, HTTP, or
  partition-erase runtime markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed.
- `git diff --check` passed with exit code 0.
- Generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`.
- The host C test output directory
  `/tmp/esp32-espnow-bbs-mesh-discovery-contract-host-tests` is outside the
  repository, and the earlier repo-local
  `build/tests/espnow_bbs_mesh_discovery_contract` generated directory was
  removed after QA review.

## Decision

Decision: accept the M4C firmware contract proof only. Future firmware runtime
implementation, persistence, first-flash proof, and M5 live mesh remain
separate gates.
