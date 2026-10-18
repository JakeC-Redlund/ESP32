# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4L Static Task Shell Source Contract

## Scope

Tier 2 ESP-IDF-only firmware source proof for the ESP-NOW BBS full-service mesh
discovery lane.

This ledger records a separate `mesh_runtime_task_shell` component, path-aware
firmware-audit coverage, docs/status records, and pre/post reviewer output.
It does not accept automatic startup from `app_main`, live task execution proof,
timer creation, callbacks, queues, bridge dispatch, firmware persistence,
generated build output, `sdkconfig`, flash, erase, monitor, serial writes,
serial ABI expansion, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing,
Android app behavior, router/admin mutation, PCAP, RF/XBee writes,
relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`
- `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-START-PROJECT`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4J required a later source gate to name task ownership, core affinity,
  priority, stack budget, timer choice, caller-owned/static state rules,
  watchdog/yield behavior, cleanup/deinit behavior, and validation.
- M4K keeps the runtime scheduler contract caller-owned and accepts only
  `caller_invoked`, `task_none`, and `timer_none` policy strings.
- M4L adds a separate ESP-IDF component rather than changing the M4K component.
- M4L requires caller-provided `StaticTask_t` and `StackType_t` buffers and uses
  `xTaskCreateStaticPinnedToCore`.
- M4L does not start from `app_main`.
- M4L uses task notification for wait/stop behavior and does not add timers,
  queues, callbacks, bridge dispatch, storage, radio startup, serial, GPIO,
  flash, file I/O, or logging.

## Assumptions

- A separate static task shell is the correct follow-up because it can prove
  source/build shape while keeping live execution and automatic startup closed.
- Caller-owned buffers and caller-owned M4K scheduler state remain the correct
  boundary for this gate.

## Unknowns

- No live task timing, watchdog behavior, stack adequacy, priority adequacy,
  route-table behavior, root/parent selection, healing, coexistence, BLE/GATT,
  Android behavior, router/admin policy, PCAP, transcript, rollback, cleanup,
  first-flash target, or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers if M4L is a separate ESP-IDF-only task-shell component and does not weaken M4K. Required caller-owned buffers, no `app_main` start, no timers/callbacks/queues/bridge/radio/storage/serial/GPIO/logging, and path-aware audit exceptions only for the new shell. |
| QA/evidence reviewer | 3 | pass with conditions | No P1 blockers. Required stale-record remediation, path-aware audit boundary, host/audit validation, no-flash ESP-IDF build, generated-output cleanup, durable records, closed-surface scan, and post-review before final acceptance. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for starting
the named M4L source boundary.

Post-review completed after implementation and validation:

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | conditional pass | No source-boundary P1/P2 blockers. Confirmed the component is ESP-IDF-only, separate from M4K, caller-started, `app_main` remains inert, and no timers/callbacks/queues/bridge/radio/storage/serial/GPIO/logging surfaces were opened. |
| QA/evidence reviewer | 3 | pass | No P1/P2 blockers. Confirmed focused and broader validation, no-flash build, cleanup, closed-surface scans, and accepted boundary wording after stale-record remediation. |

Weighted post-review result after stale-record remediation: 8/8 acceptance for
the M4L source-only boundary with no P1/P2 blockers.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`
  passed during implementation: 5 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed during implementation.
- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed: `PASS: espnow_bbs_mesh_discovery_contract host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 47 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 197`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed.
- No-flash ESP-IDF v6.0.1 build passed with
  `idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4l-static-task-shell-source-contract build`;
  `espnow_bbs_build_only.bin` size was `0x21ab0` with `0xde550` bytes free in
  the smallest app partition.
- Generated `firmware/projects/espnow-bbs/sdkconfig` and `sdkconfig.old` were
  removed after the build; `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print`
  returned no output.
- `test ! -e build/tests/espnow_bbs_mesh_discovery_contract` passed.
- A closed-source/CMake scan found no allocation, dynamic task, queue, timer,
  callback/hook, Wi-Fi/ESP-NOW/BLE, NVS, UART, GPIO, HTTP, flash/partition/OTA,
  file I/O, or logging markers in ESP-NOW BBS firmware source outside the exact
  M4L allowlist.
- `git diff --check` passed.
- Post-review accepted the M4L source-only boundary 8/8 after stale-record
  remediation, with no P1/P2 blockers.

## Decision

M4L is accepted as a caller-started static FreeRTOS task shell source API only.
Automatic startup, live task execution proof, timer creation, callbacks, queues,
bridge dispatch, firmware runtime behavior, persistence, first-flash proof, and
M5 live mesh remain separate gates.
