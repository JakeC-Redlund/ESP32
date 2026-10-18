# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4N Startup Lifecycle Source Contract

## Scope

Tier 2 ESP-IDF-only firmware source proof for the ESP-NOW BBS full-service mesh
discovery lane.

This ledger records a separate accepted `mesh_startup_lifecycle` component,
path-aware firmware-audit coverage, docs/status records, and pre/post reviewer
output. It does not accept automatic startup from `app_main`, live task
execution proof, timer creation, callbacks, queues, bridge dispatch, firmware
persistence, generated build output, `sdkconfig`, flash, erase, monitor, serial
writes, serial ABI expansion, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE
pairing, Android app behavior, router/admin mutation, PCAP, RF/XBee writes,
relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`
- `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-START-PROJECT`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4M requires a future source gate to name lifecycle owner object, lifecycle
  states, transition rules, caller-owned/static state choice, task-buffer
  lifetime, failure cleanup, and validation.
- M4L keeps task creation caller-started through a static FreeRTOS task shell.
- M4N adds a caller-owned lifecycle object that embeds the M4L task object and
  copies caller-provided M4L task config pointers.
- `app_main` remains inert and does not include or start the M4N component.

## Assumptions

- Caller-owned lifecycle state is the safe source boundary before any later
  `app_main`, bridge, persistence, or live proof gate.
- Reconfigure-after-stopped is required to renew caller-owned buffer lifetime.

## Unknowns

- No automatic startup path, live task timing, watchdog behavior, stack
  adequacy, priority adequacy, shutdown timing, bridge-visible lifecycle
  summary, first-flash target, route table, BLE/GATT, Android, router/admin,
  PCAP, transcript, rollback, cleanup, or live proof is accepted.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | approve with conditions | No P1/P2 blockers for M4N as a caller-owned lifecycle source contract. Required explicit states/transitions, no `app_main`, no static runtime state, path-aware audit coverage, and no bridge/live/flash/serial authority. |
| QA/evidence reviewer | 3 | approve with conditions | No P1/P2 blockers if focused audit coverage, task/handoff/source records, source-index row, docs-index links, validation commands, generated-output checks, stale scans, and dirty-tree boundaries are preserved. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for starting
the named M4N source boundary.

Post-review initially found stale status wording that still described M4N as
pending. After record remediation and stale-authority scans, two read-only
reviewers approved the named acceptance boundary:

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | approve | No P1/P2 blockers after remediation for accepting the caller-owned lifecycle source contract. |
| QA/evidence/Agent Operations reviewer | 3 | approve | No P1/P2 blockers after remediation; validation, stale scans, generated-output cleanup, and durable records were aligned. |

Weighted final post-review result: 8/8 approval with no P1/P2 blockers for the
M4N caller-owned startup lifecycle source API only.

## Validation

Focused implementation validation passed:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
```

Full validation passed:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 199
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4n-startup-lifecycle-source-api build
find firmware/projects/espnow-bbs -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

The no-flash ESP-IDF build generated `espnow_bbs_build_only.bin` size
`0x21ab0` with `0xde550` bytes free in the smallest app partition. Generated
`firmware/projects/espnow-bbs/sdkconfig` was removed, and the follow-up
`sdkconfig*` scan was empty. Final post-review passed 8/8 weighted approval
after stale-record remediation.

## Decision

M4N is accepted for the caller-owned startup lifecycle source API only. Future
`app_main` integration, firmware runtime behavior, bridge dispatch,
persistence, first-flash proof, and M5 live mesh remain separate gates.
