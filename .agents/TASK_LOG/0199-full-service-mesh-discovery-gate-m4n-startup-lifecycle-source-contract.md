# Task 0199: Full-Service Mesh Discovery Gate M4N Startup Lifecycle Source Contract

Status: accepted-startup-lifecycle-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`,
`SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`

## Routing

- Verified facts: required governance/docs were reread. M4M is accepted only as
  design-only startup/lifecycle requirements. M4L is accepted only as a
  caller-started static FreeRTOS task shell source API. `app_main` remains inert
  and does not start M4L or M4N. Current records keep automatic startup, bridge
  dispatch, firmware runtime, live proof, flash, serial, RF/BLE, release,
  publication, commit, push, PR, and deploy closed.
- Assumptions: a caller-owned startup lifecycle source contract is the correct
  next movement before any `app_main` integration, bridge, persistence, or live
  proof gate.
- Unknowns: no live task timing, watchdog behavior, stack adequacy, priority
  adequacy, shutdown timing, bridge-visible lifecycle summary, first-flash
  target, route table, healing, BLE/GATT, Android, router/admin, PCAP,
  transcript, rollback, cleanup, or live proof is accepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware with Communications, Architecture, Agent Operations, and
  QA.
- Evidence need: source-indexed M4M/M4L/M4K/M4J/ESP-IDF FreeRTOS records,
  read-only reviewer quorum, focused and broader audit validation, no-flash
  build, generated-output cleanup, closed-surface scans, task/handoff/source
  records, and final post-review.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_startup_lifecycle/`,
  `scripts/scaffold_audit_firmware.py`, `scripts/scaffold_audit_data.py`,
  `tests/scaffold_audits/test_espnow_bbs_mesh_startup_lifecycle.py`,
  `docs/architecture/espnow-bbs-mesh-m4n-startup-lifecycle-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4n-startup-lifecycle-source-contract.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0146-full-service-mesh-discovery-gate-m4n-startup-lifecycle-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Two read-only reviewers were spawned, waited, and closed after
  output capture before mutation. Weighted pre-mutation disposition was 8/8
  approval with no P1/P2 blockers for the named M4N source boundary.
- Gate authority: caller-owned startup lifecycle source API, path-aware audits,
  docs/status records, task log, and handoff. No automatic startup, live task
  proof, flash, serial, release, or publication authority is opened.

## Implementation

- Added `components/mesh_startup_lifecycle/` as a separate ESP-IDF component.
- Added caller-owned `espnow_bbs_mesh_startup_lifecycle_t`, embedding the M4L
  `espnow_bbs_mesh_runtime_task_t` object and copying caller-provided M4L task
  config pointers.
- Added lifecycle states for `uninitialized`, `configured`, `starting`,
  `running`, `stop_requested`, `stopped`, and `failed`.
- Added transition checks for configure, start, repeated start,
  stop-before-start, repeated stop, stop polling, restart-after-stopped via
  reconfigure, task-create failure cleanup, and deinit.
- Added path-aware audit coverage and focused scaffold tests for the M4N
  boundary.

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
`sdkconfig*` scan was empty. Initial post-review found stale status wording
only; after record remediation, final post-review passed 8/8 weighted approval
with no P1/P2 blockers.

## Open Evidence

- Automatic startup source implementation from `app_main`, live task timing,
  stack adequacy, watchdog behavior, priority adequacy, bridge-visible lifecycle
  summary, persistence, first-flash target, recovery path, route table, BLE/GATT,
  Android, router/admin, PCAP, transcript, rollback, cleanup, and live proof
  remain unaccepted.

## Authority Limits

This task does not authorize automatic startup from `app_main`, timer creation,
callbacks, queues, firmware persistence, bridge dispatch, ESP-NOW runtime,
ESP-WIFI-MESH runtime, BLE pairing, Android app behavior, router/admin
mutation, PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
bridge ABI changes, Gate F service-code changes, `mesh_discovery.v1` schema
changes, Win31 transport changes, RF/XBee writes, relay actions, load/mains
work, live bench, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0146-full-service-mesh-discovery-gate-m4n-startup-lifecycle-source-contract-to-qa-firmware-communications.md](../handoffs/0146-full-service-mesh-discovery-gate-m4n-startup-lifecycle-source-contract-to-qa-firmware-communications.md)

## Decision

M4N is accepted for the caller-owned startup lifecycle source API only. Future
`app_main` integration, first-flash proof, bridge dispatch, persistence, and M5
live mesh remain separate gates.
