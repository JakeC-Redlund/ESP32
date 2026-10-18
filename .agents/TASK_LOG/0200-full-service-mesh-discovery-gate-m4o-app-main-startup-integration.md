# Task 0200: Full-Service Mesh Discovery Gate M4O App Main Startup Integration

Status: accepted-app-main-startup-integration-source-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Routing

- Verified facts: required governance/docs were reread. M4N is accepted as a
  caller-owned startup lifecycle source API only. Its handoff names separately
  gated `app_main` integration as a next safe firmware continuation. No
  same-session live/flash/serial authority exists.
- Assumptions: a no-flash `app_main` startup integration source gate can wire
  accepted M4N/M4L/M4K contracts without accepting firmware runtime behavior or
  live task proof.
- Unknowns: live task timing, watchdog behavior, stack adequacy, priority
  adequacy, shutdown timing, first-flash behavior, bridge-visible lifecycle
  summary, route table, BLE/GATT, Android, router/admin, PCAP, transcript,
  rollback, cleanup, and live proof remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware with Communications, Architecture, Agent Operations, and
  QA.
- Evidence need: source-indexed M4N/M4M/M4L/M4K/ESP-IDF records, read-only
  reviewer quorum, focused and broader audit validation, no-flash build,
  generated-output cleanup, closed-surface scans, task/handoff/source records,
  and post-review.
- Mutation boundary:
  `firmware/projects/espnow-bbs/main/main.c`,
  `firmware/projects/espnow-bbs/main/CMakeLists.txt`,
  `scripts/scaffold_audit_firmware.py`, `scripts/scaffold_audit_data.py`,
  `tests/scaffold_audits/test_espnow_bbs_app_main_startup_integration.py`,
  `docs/architecture/espnow-bbs-mesh-m4o-app-main-startup-integration.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4o-app-main-startup-integration.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0147-full-service-mesh-discovery-gate-m4o-app-main-startup-integration-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Two read-only reviewers were spawned, waited, and closed
  after output capture before mutation. Weighted pre-mutation disposition was
  8/8 approval with no P1/P2 blockers for the named M4O source boundary.
  Final read-only post-review was also spawned, waited, and closed after output
  capture; weighted final disposition was 8/8 conditional approval with no P1/P2
  blockers, conditional only on record finalization.
- Gate authority: no-flash `app_main` startup integration source contract,
  path-aware audits, docs/status records, task log, and handoff. No live proof,
  flash, serial, release, or publication authority is opened.

## Implementation

- Changed `main/CMakeLists.txt` to depend on `mesh_startup_lifecycle`.
- Changed `main.c` to include `espnow_bbs_mesh_startup_lifecycle.h`.
- Added static app-main-owned event queue, owner, runtime scheduler, lifecycle,
  `StaticTask_t`, and `StackType_t` stack storage.
- Added startup ordering that initializes and claims the contract owner,
  configures the runtime scheduler, configures M4N, and calls
  `espnow_bbs_mesh_startup_lifecycle_start`.
- Added path-aware audit coverage and a focused scaffold test for M4O.

## Validation

Validation passed.

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

Results:

- Focused scaffold unit tests passed: 15 tests.
- Host contract tests passed.
- Firmware, path, records, sources, docs, agent-process, and full scaffold
  validation passed.
- No-flash ESP-IDF v6.0.1 build passed; generated
  `/tmp/esp32-espnow-bbs-m4o-app-main-startup-integration-source-contract/espnow_bbs_build_only.bin`
  size `0x22d50`, leaving `0xdd2b0` bytes free in the 1 MiB app partition.
- Generated `firmware/projects/espnow-bbs/sdkconfig` was removed and the final
  `find firmware/projects/espnow-bbs -name 'sdkconfig*' -print` cleanup check
  had no output.
- `main.c` closed-surface scan had no forbidden marker matches.
- `git diff --check` passed.

## Open Evidence

- Runtime task behavior, watchdog behavior, stack adequacy, priority adequacy,
  shutdown timing, first-flash behavior, bridge-visible lifecycle summary,
  persistence, recovery path, route table, BLE/GATT, Android, router/admin,
  PCAP, transcript, rollback, cleanup, and live proof remain unaccepted.

## Authority Limits

This task does not authorize bridge dispatch, firmware persistence, ESP-NOW
runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app behavior, router/admin
mutation, PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
bridge ABI changes, Gate F service-code changes, `mesh_discovery.v1` schema
changes, Win31 transport changes, RF/XBee writes, relay actions, load/mains
work, live bench, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0147-full-service-mesh-discovery-gate-m4o-app-main-startup-integration-to-qa-firmware-communications.md](../handoffs/0147-full-service-mesh-discovery-gate-m4o-app-main-startup-integration-to-qa-firmware-communications.md)

## Decision

M4O is accepted for the no-flash `app_main` startup integration source contract
only. Firmware runtime behavior, first-flash proof, bridge dispatch,
persistence, and M5 live mesh remain separate gates.
