# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4X Bridge Readiness App Main Source

## Scope

Tier 2 firmware source proof for source-only local bridge-readiness status
capture in `app_main` after accepted M4W retained recovery.

This ledger records accepted source-only bridge-readiness app-main wiring after
validation, post-review, and P2 record-alignment remediation. It does not accept live bridge dispatch,
arbitrary bridge frame parsing, serial ABI expansion, bridge ABI changes, Gate
F request/service changes, `mesh_discovery.v1` schema changes, live NVS
execution proof, reboot durability, runtime persistence proof, first-flash
proof, firmware runtime acceptance, timer creation, live task execution proof,
flash, erase, monitor, serial writes, ESP-NOW runtime, ESP-WIFI-MESH runtime,
BLE pairing, Android behavior, router/admin mutation, PCAP, RF/XBee writes,
relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4X-BRIDGE-READINESS-APP-MAIN-SOURCE-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4W-RETAINED-RECOVERY-APP-MAIN-SOURCE-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4P through M4S already accept pure-C local bridge summary, request-router,
  dispatch-envelope, and request-text intent source APIs over known discovery
  request names.
- M4W accepts retained recovery in `app_main` as source-only load/status
  capture and keeps live bridge dispatch closed.
- Current M4X source updates add static app-main-owned bridge state and route
  only fixed request text `discovery_snapshot` through
  `espnow_bbs_mesh_contract_bridge_request_text_intent_route()`.
- `main.c` records bridge readiness status and summary line bytes. It does not
  block lifecycle start on readiness failure.
- `main.c` does not call direct bridge dispatch routes, direct local request
  routes, arbitrary request ingestion, retained save, retained erase, direct
  NVS writes, serial writes, RF/XBee writes, Wi-Fi, ESP-NOW, ESP-WIFI-MESH,
  BLE, timers, HTTPD, file I/O, dynamic allocation, or logging.

## Assumptions

- No-flash ESP-IDF build plus focused scaffold checks can prove source wiring
  and closed-surface boundaries only.
- A later live bridge dispatch gate must provide same-session authority,
  bridge input evidence, transcript proof, rollback/cleanup, and reviewer
  acceptance before any live dispatch claim is made.

## Unknowns

- Live bridge input source, serial transcript behavior, bridge ABI, serial ABI,
  runtime scheduling behavior, live NVS behavior, reboot durability,
  first-flash behavior, M5 mesh behavior, hardware behavior, and release
  readiness remain unproved.

## Reviewer Quorum

Pre-mutation read-only reviewers were spawned, waited, captured, and closed
before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer `019e9eb2-f90b-7202-84fa-1af6d996ab43` | 5 | approve with conditions | No P1/P2 blockers if the helper runs after contract configuration, before lifecycle start, uses fixed `discovery_snapshot`, records status/line bytes only, and keeps live/ABI/hardware/release surfaces closed. |
| QA/Evidence reviewer `019e9eb2-ff9e-7a43-9104-231003453423` | 3 | approve with conditions | No P1/P2 blockers if durable records use source-only wording, the M4W audit contradiction is resolved, and live dispatch/runtime/NVS/first-flash/M5/hardware/release claims remain closed. |
| Agent Process reviewer `019e9eb3-17c0-7903-b3e4-16766c0e1318` | 2 | approve with conditions | No P1/P2 blockers if the Tier 2 routing packet, lifecycle cleanup, dirty-tree boundary, task/handoff/source records, and closed Tier 3 gates are preserved. |

Weighted pre-mutation result: 10/10 approval with no P1/P2 blockers for the
named M4X source-only boundary.

Post-mutation read-only reviewers were spawned, waited, captured, and closed
after validation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer `019e9ec1-58f2-7942-bcc3-5e3d81e6fe14` | 5 | approve with record-alignment condition | No source-boundary P1/P2 blockers; source-only acceptance is valid after stale validation/post-review wording is remediated. |
| QA/Evidence reviewer `019e9ec1-5fca-7870-9be5-fda80ecaebf9` | 3 | reject until P2 remediation | No source-boundary P1 blockers and no unearned live/runtime/NVS/ABI/release claims; final acceptance required stale validation/post-review wording remediation. |
| Agent Process reviewer `019e9ec1-6738-7a22-b14a-9c5238355457` | 2 | reject until P2 remediation | Process and source boundary were acceptable; final acceptance required stale validation/post-review wording remediation. |

The P2 record-alignment issue was remediated by updating the M4X task, handoff,
architecture note, source ledger, source-index row, and status records to match
the completed full validation matrix and post-review outputs. Weighted
post-mutation result after remediation: 10/10 conditional approval with no
remaining P1/P2 blockers for M4X source-only acceptance.

## Validation

Validation passed:

- Discovery-contract host tests:
  `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
- Retained-persistence host tests:
  `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py`
- Focused scaffold unittest:
  `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration`
- Focused scaffold unittests:
  `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence`
- Firmware scaffold audit:
  `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
- Firmware, data, records, sources, docs, paths, agent-process, pages,
  React Native, and skills scaffold audits.
- No-flash ESP-IDF v6.0.1 build:
  `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4x-bridge-readiness-app-main-source build`
- Full scaffold verification:
  `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`

The no-flash build generated `espnow_bbs_build_only.bin` size `0x29940` with
`0xd66c0` bytes free in the 1 MiB app partition. No flash, monitor, serial, RF,
hardware, release, publication, commit, push, PR, or deploy action was run.
Generated `sdkconfig` residue was removed. Scoped stale-wording and
closed-surface scans passed. `git diff --check` passed.

## Decision

Decision accepted after P2 record-alignment remediation: source-only local
bridge readiness status capture after accepted M4W. Live bridge dispatch,
runtime proof, live NVS proof, first-flash proof, M5 live mesh, hardware, and
release remain separate gates.
