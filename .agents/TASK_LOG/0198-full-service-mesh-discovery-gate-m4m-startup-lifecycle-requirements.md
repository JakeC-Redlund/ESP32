# Task 0198: Full-Service Mesh Discovery Gate M4M Startup Lifecycle Requirements

Status: accepted-design-only-startup-lifecycle-requirements

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`,
`SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`

## Routing

- Verified facts: required governance/docs were reread. M4L is accepted only as
  a caller-started static FreeRTOS task shell source API. `app_main` remains
  inert and does not start the M4L task shell. Current records keep automatic
  startup, bridge dispatch, firmware runtime, live proof, flash, serial,
  RF/BLE, release, publication, commit, push, PR, and deploy closed.
- Assumptions: a design-only startup/lifecycle requirements gate is the correct
  next movement before any source gate opens `app_main` integration or static
  runtime state.
- Unknowns: no automatic startup path, lifecycle state object, accepted
  transition table, task-buffer lifetime, stack adequacy, watchdog behavior,
  startup order, shutdown timing, bridge-visible lifecycle summary, first-flash
  target, route table, healing, BLE/GATT, Android, router/admin, PCAP,
  transcript, rollback, cleanup, or live proof is accepted.
- Selected tier: Tier 2 design/docs/source-record/status work.
- Owner role: Firmware with Communications, Architecture, Agent Operations, and
  QA.
- Evidence need: source-indexed M4J/M4K/M4L/ESP-IDF FreeRTOS records, read-only
  reviewer quorum, durable architecture/source/task/handoff/status records,
  scaffold audits, stale-wording and closed-surface scans, generated-output
  checks, and diff hygiene.
- Mutation boundary:
  `docs/architecture/espnow-bbs-mesh-m4m-startup-lifecycle-requirements.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4m-startup-lifecycle-requirements.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0145-full-service-mesh-discovery-gate-m4m-startup-lifecycle-requirements-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Two read-only reviewers were spawned, waited, and closed after
  output capture before mutation. Weighted pre-mutation disposition was 8/8
  approval with no P1/P2 blockers for the named docs-only M4M boundary.
- Post-review quorum: two read-only reviewers were spawned, waited, and closed
  after output capture before acceptance wording. Firmware/Communications/
  Architecture approved with weight 5 after wording cleanup. QA/evidence
  approved with weight 3 after record remediation. Weighted post-mutation
  disposition after cleanup is 8/8 with no P1/P2 blockers for the named
  design-only requirements boundary.
- Gate authority: design note, source ledger, source-index row, status updates,
  task log, and handoff. No firmware source, CMake, tests, `app_main` start,
  no-flash build, live bench, flash, serial, release, publication, commit, push,
  PR, or deploy authority is opened.
- Validation plan: run records/source/docs/firmware/agent-process audits,
  `verify_scaffold.py`, targeted stale-wording and closed-surface scans,
  generated-output checks, and `git diff --check`.

## Implementation

- Added a source-backed M4M startup/lifecycle requirements note.
- Recorded lifecycle requirements for any future source gate to name states,
  accepted/rejected transitions, caller-owned versus static state, task buffer
  lifetime, failure cleanup, and validation.
- Kept M4M records design-only; no firmware source, CMake, test source,
  `app_main`, build output, or generated configuration files were changed by
  this gate.

## Validation

Post-mutation validation passed:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 198
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
find firmware/projects/espnow-bbs -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

The `sdkconfig*` scan printed no paths, the generated build-test absence check
passed, and stale authority scans found only negative wording or expected
design-only status references.

## Open Evidence

- Automatic startup source implementation, `app_main` integration, accepted
  lifecycle state object, static runtime state, task-buffer lifetime, live task
  timing, stack adequacy, watchdog behavior, bridge-visible lifecycle summary,
  persistence, first-flash target, recovery path, route table, BLE/GATT,
  Android, router/admin, PCAP, transcript, rollback, cleanup, and live proof
  remain unaccepted.

## Authority Limits

This task does not authorize firmware source implementation, CMake changes,
test source changes, `app_main` startup, FreeRTOS task creation beyond the
already accepted M4L caller-started source API, timer creation, callbacks,
queues, bridge dispatch, firmware persistence, ESP-NOW runtime,
ESP-WIFI-MESH runtime, BLE pairing, Android app behavior, router/admin
mutation, PCAP, flash, erase, monitor, serial writes, serial ABI expansion,
bridge ABI changes, Gate F service-code changes, `mesh_discovery.v1` schema
changes, Win31 transport changes, RF/XBee writes, relay actions, load/mains
work, live bench, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0145-full-service-mesh-discovery-gate-m4m-startup-lifecycle-requirements-to-qa-firmware-communications.md](../handoffs/0145-full-service-mesh-discovery-gate-m4m-startup-lifecycle-requirements-to-qa-firmware-communications.md)

## Decision

M4M is accepted as design-only startup/lifecycle requirements after clean
scaffold validation and post-review. Future startup source implementation,
`app_main` integration, firmware runtime behavior, bridge dispatch, persistence,
first-flash proof, and M5 live mesh remain separate gates. This task does not
authorize firmware source implementation, CMake/test changes, automatic startup
from `app_main`, runtime behavior, bridge dispatch, persistence, live task
proof, live mesh, BLE, Android, router/admin, flash, serial/RF/XBee, release,
publication, commit, push, PR, or deploy.
