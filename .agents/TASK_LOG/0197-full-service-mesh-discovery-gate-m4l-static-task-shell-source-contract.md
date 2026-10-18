# Task 0197: Full-Service Mesh Discovery Gate M4L Static Task Shell Source Contract

Status: accepted-static-task-shell-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`,
`SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`

## Routing

- Verified facts: required governance/docs were reread. M4K is accepted as a
  caller-owned runtime scheduler source contract only. ESP-IDF FreeRTOS exposes
  `xTaskCreateStaticPinnedToCore` and the existing `app_main` is inert.
- Assumptions: a separate caller-started static task shell is the safe next
  source movement because it proves compile-time FreeRTOS task-shell shape while
  keeping automatic startup and live behavior closed.
- Unknowns: no live task timing, watchdog behavior, stack adequacy, priority
  adequacy, route table, root/parent selection, healing, coexistence, BLE/GATT,
  Android, router/admin, PCAP, transcript, rollback, cleanup, first-flash target,
  or live proof is accepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware with Communications, Architecture, Agent Operations, and
  QA.
- Evidence need: source-indexed M4J/M4K/ESP-IDF FreeRTOS records, read-only
  reviewer quorum, focused and broader host/audit validation, no-flash build,
  generated-output cleanup, closed-surface scan, task/handoff/source records,
  and post-review.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_runtime_task_shell/`,
  `scripts/scaffold_audit_firmware.py`, `scripts/scaffold_audit_data.py`,
  `tests/scaffold_audits/test_espnow_bbs_mesh_runtime_task_shell.py`,
  `docs/architecture/espnow-bbs-mesh-m4l-static-task-shell-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4l-static-task-shell-source-contract.md`,
  `knowledge-base/source-index.md`, docs/status files, this task record, and
  `.agents/handoffs/0144-full-service-mesh-discovery-gate-m4l-static-task-shell-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Two read-only reviewers were spawned, waited, and closed after
  output capture before mutation. Weighted pre-mutation disposition was 8/8
  approval with no P1/P2 blockers for the named boundary.
- Gate authority: static task shell source API, path-aware audits, docs/status
  records, task log, and handoff. No automatic startup, live task proof, flash,
  serial, release, or publication authority is opened.

## Implementation

- Added `components/mesh_runtime_task_shell/` as a separate ESP-IDF component.
- Added caller-provided config for M4K scheduler pointer, owner pointer, task
  name, `StaticTask_t`, `StackType_t`, stack depth, priority, core ID, and
  period ticks.
- Added validation for null required pointers, zero stack depth, zero/over-limit
  period, invalid core ID, zero priority, duplicate start, and stop-before-start.
- Added a caller-started `xTaskCreateStaticPinnedToCore` start function and an
  explicit task-notify stop request.
- Kept the task loop limited to notification wait, M4K scheduler step, local
  counters/status, and exit on stop.
- Added path-aware audit coverage and focused scaffold tests for the M4L
  boundary.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`:
  PASS, 5 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`:
  PASS, 47 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 197`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`:
  PASS.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`:
  PASS.
- No-flash ESP-IDF v6.0.1 build passed with
  `idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4l-static-task-shell-source-contract build`;
  `espnow_bbs_build_only.bin` size was `0x21ab0` with `0xde550` bytes free in
  the smallest app partition.
- Generated `sdkconfig` output was removed; `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print`
  returned no output.
- `test ! -e build/tests/espnow_bbs_mesh_discovery_contract`: PASS.
- Closed-source/CMake scan: PASS, no forbidden runtime markers found outside the
  exact M4L allowlist.
- `git diff --check`: PASS.
- Post-review completed: the Firmware/Communications/Architecture reviewer
  recorded a conditional pass with no source-boundary P1/P2 blockers, the
  QA/evidence reviewer recorded PASS with no P1/P2 blockers, and stale-record
  remediation closes the named acceptance condition. Weighted post-review result
  is 8/8 for the M4L source-only boundary.

## Open Evidence

- Live task execution, stack adequacy, watchdog behavior, priority adequacy,
  runtime latency, shutdown timing, route table, root/parent selection, healing,
  coexistence, BLE/GATT, Android, router/admin policy, PCAP, transcript,
  rollback, cleanup, first-flash target, and live proof remain unaccepted.

## Authority Limits

This task does not authorize automatic startup from `app_main`, timer creation,
callbacks, queues, firmware persistence, ESP-NOW runtime, ESP-WIFI-MESH
runtime, BLE pairing, Android app behavior, router/admin mutation, PCAP, flash,
erase, monitor, serial writes, serial ABI expansion, bridge ABI changes, Gate F
service-code changes, `mesh_discovery.v1` schema changes, Win31 transport
changes, RF/XBee writes, relay actions, load/mains work, live bench, release,
publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
`admin-strict` mutation.

## Handoff

[../handoffs/0144-full-service-mesh-discovery-gate-m4l-static-task-shell-source-contract-to-qa-firmware-communications.md](../handoffs/0144-full-service-mesh-discovery-gate-m4l-static-task-shell-source-contract-to-qa-firmware-communications.md)

## Decision

Accepted as a caller-started static FreeRTOS task shell source API only.
Automatic startup, live task proof, timer creation, callbacks, queues, bridge
dispatch, firmware runtime, persistence, flash/serial/live proof, release,
publication, commit, push, PR, and deployment remain closed behind separate
gates.
