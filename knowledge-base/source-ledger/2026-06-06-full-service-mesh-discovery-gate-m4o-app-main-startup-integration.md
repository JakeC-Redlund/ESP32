# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4O App Main Startup Integration

## Scope

Tier 2 ESP-IDF-only firmware source proof for a no-flash `app_main` startup
integration over the accepted M4N lifecycle API.

This ledger records `main.c` and main CMake startup wiring, path-aware firmware
audit coverage, focused scaffold tests, docs/status records, and pre/post
reviewer output. It does not accept live task execution proof, firmware runtime
acceptance, bridge dispatch, persistence, generated build output, `sdkconfig`,
flash arguments, flash, erase, monitor, serial writes, serial ABI expansion,
ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app behavior,
router/admin mutation, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4N accepts a caller-owned lifecycle API but leaves `app_main` integration as
  a separate gate.
- M4M requires a later `app_main` gate to name storage model, lifetime, stack
  buffer, priority, core affinity, startup ordering, stop/deinit path, failure
  cleanup, and validation.
- M4O adds static `main.c` state for event queue, owner, runtime scheduler,
  startup lifecycle, `StaticTask_t`, and `StackType_t` stack buffers.
- M4O wires `app_main` to initialize/claim/configure the contract stack,
  configure M4N, and call `espnow_bbs_mesh_startup_lifecycle_start`.

## Assumptions

- Static file-scope state is appropriate for this source-only startup gate
  because it gives explicit lifetime and avoids dynamic allocation.
- A no-flash ESP-IDF build is enough to validate compile-time integration only.

## Unknowns

- No live task timing, watchdog behavior, stack adequacy, priority adequacy,
  shutdown timing, first-flash behavior, bridge-visible lifecycle summary,
  route table, root/parent behavior, healing, coexistence, BLE/GATT, Android,
  router/admin, PCAP, transcript, rollback, cleanup, or live proof is accepted.

## Reviewer Quorum

Lifecycle listing was not visible through available tools before spawning. Two
read-only reviewers were spawned, waited, and closed after output capture before
mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | approve with conditions | No P1/P2 blockers for opening a no-flash M4O source gate if audits become narrow positive allowlists and no runtime/live claims are made. |
| QA/Evidence/Agent Operations reviewer | 3 | approve with conditions | No P1/P2 blockers for mutation if M4O receives focused tests, durable records, stale scans, generated-output cleanup, no-flash build proof, and final post-review before acceptance. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for starting
the named M4O source boundary.

Final read-only post-review reviewers were spawned, waited, and closed after
output capture before acceptance record finalization.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | conditional approve | No P1/P2 blockers; source-only acceptance allowed after acceptance records are finalized. |
| QA/Evidence/Agent Operations reviewer | 3 | conditional approve | No P1/P2 blockers; validation evidence is sufficient after accepted/source-only wording and actual validation results are recorded. |

Weighted final post-review result: 8/8 approval with no P1/P2 blockers for the
named M4O no-flash source boundary.

## Validation

Validation passed. Commands/results:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 200
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4o-app-main-startup-integration-source-contract build
find firmware/projects/espnow-bbs -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

- Focused scaffold unit tests passed: 15 tests.
- Host contract tests passed.
- Firmware, path, records, sources, docs, agent-process, and full scaffold
  validation passed.
- No-flash ESP-IDF v6.0.1 build passed; generated
  `espnow_bbs_build_only.bin` size `0x22d50`, leaving `0xdd2b0` bytes free in
  the 1 MiB app partition.
- Generated `firmware/projects/espnow-bbs/sdkconfig` was removed and the final
  `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print` cleanup check
  had no output.
- `main.c` closed-surface scan had no forbidden marker matches.
- `git diff --check` passed.

## Decision

M4O is accepted for the no-flash `app_main` startup integration source contract.
Firmware runtime behavior, bridge dispatch, persistence, first-flash proof, and
M5 live mesh remain separate gates.
