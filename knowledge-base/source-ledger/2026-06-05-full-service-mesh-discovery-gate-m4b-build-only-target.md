# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4B Build-Only Target

## Scope

Tier 2 build-only ESP-IDF target proof for the ESP-NOW BBS full-service mesh
discovery lane.

This ledger accepts only a minimal inert `espnow-bbs` ESP-IDF project target
and a no-flash local build proof. It does not accept firmware runtime
implementation, firmware persistence, live ESP-WIFI-MESH, BLE pairing,
Android app behavior, router/admin mutation, flash, erase, monitor, serial
writes, serial ABI expansion, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-START-PROJECT`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`
- `SRC-ESP-IDF-WIFI-MESH`
- `SRC-ESP-IDF-RF-COEXIST`

## Verified Facts

- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for the `espnow-bbs`
  coordinator/client firmware lane.
- Gate M4 accepted host-only adapter fixture proof but did not create a
  firmware target or build output.
- This gate adds `firmware/projects/espnow-bbs/README.md`,
  `firmware/projects/espnow-bbs/CMakeLists.txt`,
  `firmware/projects/espnow-bbs/main/CMakeLists.txt`, and
  `firmware/projects/espnow-bbs/main/main.c`.
- `main.c` contains only an inert `app_main` and comments preserving the M4
  host adapter boundary.
- `scripts/scaffold_audit_data.py`,
  `scripts/scaffold_audit_firmware.py`, and
  `tests/scaffold_audits/test_espnow_bbs_build_only_target.py` now own the
  new framework-file allowlist and build-only firmware boundary.
- The local no-flash ESP-IDF build passed from `/mnt/h/ESP32` using
  `/home/cyber/.espressif/tools/activate_idf_v6.0.1.sh`, `IDF_PATH`
  `/home/cyber/Espressif/v6.0.1/esp-idf`, default target `esp32`, and build
  directory `/tmp/esp32-espnow-bbs-build-only`.
- ESP-IDF generated `/tmp/esp32-espnow-bbs-build-only/espnow_bbs_build_only.bin`
  with binary size `0x21aa0` and `0xde560` bytes free in the smallest app
  partition. These generated files are not tracked source artifacts.

## Assumptions

- Build-only proof is the correct next step after M4 because it resolves target
  shape and local toolchain evidence before runtime APIs are introduced.
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
| Firmware/Communications/Architecture reviewer | 3 | conditional pass | No P1 blockers for build-only scaffolding; required audit allowlist, toolchain evidence, no tracked generated output, and closed runtime/live surfaces. |
| QA reviewer | 3 | fail for current acceptance | Named missing target, missing build output, missing audit coverage, and missing durable records as P1 acceptance blockers. |

The QA P1 items are addressed by this gate packet. A post-implementation
reviewer pass should preserve the build-only boundary before any broader
runtime work proceeds.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed with exit code 0.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-build-only build`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 42 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 187`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- Runtime API marker scan over `firmware/projects/espnow-bbs` returned no
  matches for Wi-Fi, ESP-NOW, mesh, BLE, NVS, UART, GPIO-output, HTTP, or
  partition-erase runtime markers.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.
- `git diff --check` passed with exit code 0.
- Stale target/build-future wording scan over `research`, `docs`,
  `.agents/TASK_LOG`, and `knowledge-base/source-ledger` returned no matches.

## Decision

Decision: accept the M4B build-only target proof only. Future firmware runtime
implementation, persistence, first-flash proof, and M5 live mesh remain
separate gates.
