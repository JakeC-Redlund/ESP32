# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4W Retained Recovery App Main Source

## Scope

Tier 2 firmware source proof for source-only retained-recovery startup wiring
in `app_main` after accepted M4V retained persistence source.

This ledger records accepted source-only retained-recovery app-main wiring after
validation and final 8/8 weighted post-review. It does not accept live NVS
execution proof, reboot durability, runtime persistence
proof, first-flash proof, firmware runtime acceptance, live bridge dispatch,
arbitrary bridge frame parsing, serial ABI expansion, bridge ABI changes, Gate
F request/service changes, `mesh_discovery.v1` schema changes, timer creation,
live task execution proof, flash, erase, monitor, serial writes, ESP-NOW
runtime, ESP-WIFI-MESH runtime, BLE pairing, Android behavior, router/admin
mutation, PCAP, RF/XBee writes, relay/load/mains, release, publication, commit,
push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4W-RETAINED-RECOVERY-APP-MAIN-SOURCE-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`
- `SRC-ESP-IDF-NVS`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4V accepts the retained persistence component but not app-main integration.
- Current M4W source updates add `mesh_retained_persistence` as a private main
  dependency and call only retained storage context init, storage init, and
  storage load from app startup.
- `main.c` records retained init/load statuses in static volatile fields and
  initializes retained intent to empty volatile state before any storage load.
- `main.c` does not call retained storage save, retained storage erase, direct
  NVS functions, bridge dispatch, serial writes, RF/XBee writes, Wi-Fi, ESP-NOW,
  ESP-WIFI-MESH, BLE, timers, file I/O, dynamic allocation, or logging.

## Assumptions

- No-flash ESP-IDF build plus focused scaffold checks can prove source wiring
  and closed-surface boundaries only.
- Storage deinit remains a future shutdown/lifecycle gate because no app-main
  stop/deinit path is accepted.

## Unknowns

- Live NVS execution, live partition state, reboot durability, runtime
  persistence, first-flash target behavior, stack impact, watchdog behavior,
  bridge transcript, Win31/OPCON proof, rollback, and live proof remain
  unaccepted.

## Reviewer Quorum

Pre-mutation read-only reviewers were spawned, waited, captured, and closed
before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer `019e9e86-5d9a-76a1-8e8f-ef878d47a65e` | 5 | approve with conditions | No P1/P2 blockers for source-only M4W if focused audit coverage proves load-only app-main recovery, rejects save/erase/direct NVS writes and live surfaces, preserves M4O startup behavior, and keeps records source-only. |
| QA/Evidence/AgentOps reviewer `019e9e86-74a3-7911-8061-92ad93883526` | 3 | approve with conditions | No P1/P2 blockers if M4W records source-only scope, initializes retained intent before load-error paths, records storage deinit policy, updates app-main audit coverage, adds durable records/index links, and preserves live/hardware/release closures. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4W source-only boundary.

Post-mutation read-only reviewers were spawned, waited, captured, and closed
after validation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer `019e9e95-dea4-7a22-89d8-3f99e86b9dad` | 5 | approve | No P1/P2 blockers; source-only boundary keeps retained recovery load-only, preserves M4O startup ordering, rejects save/erase/direct NVS/live surfaces, and does not authorize runtime/live proof. |
| QA/Evidence/AgentOps reviewer `019e9e95-fdf2-7143-9649-c35483efad68` | 3 | approve | No P1/P2 blockers; records, evidence wording, audit coverage, generated-output cleanup, and closed gates support source-only acceptance without runtime/live proof. |

Weighted post-mutation result: 8/8 approval with no P1/P2 blockers for M4W
source-only acceptance.

## Validation

Validation passed:

- Retained-persistence host tests:
  `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py`
- Discovery-contract host tests:
  `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
- Focused scaffold unittests:
  `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence`
- Firmware, data, records, sources, docs, paths, agent-process, pages,
  React Native, and skills scaffold audits.
- No-flash ESP-IDF v6.0.1 build:
  `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4w-retained-recovery-app-main-source build`
- Scoped `git diff --check`, generated-output cleanup, stale acceptance wording
  scan, and closed-surface scans.

The no-flash build generated `espnow_bbs_build_only.bin` size `0x283e0` with
`0xd7c20` bytes free in the 1 MiB app partition. No flash, monitor, serial, RF,
hardware, release, publication, commit, push, PR, or deploy action was run.

## Decision

Decision accepted: source-only retained-recovery app-main integration after
M4V. Live NVS execution proof, reboot durability, runtime persistence proof,
live bridge dispatch, first-flash proof, and M5 live mesh remain separate gates.
