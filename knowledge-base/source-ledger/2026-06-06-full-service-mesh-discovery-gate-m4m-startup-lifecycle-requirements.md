# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4M Startup Lifecycle Requirements

## Scope

Tier 2 design-only source-backed requirements for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger records startup/lifecycle requirements, durable records, status
updates, and reviewer output. It does not accept firmware source
implementation, CMake changes, test source changes, automatic startup from
`app_main`, generated build output, `sdkconfig`, flash, erase, monitor, serial
writes, serial ABI expansion, bridge dispatch, firmware persistence, ESP-NOW
runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app behavior,
router/admin mutation, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`
- `SRC-ESP-IDF-FREERTOS-IDF-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`

## Verified Facts

- Current records accept M4L only as a caller-started static task shell source
  API and keep automatic startup, firmware runtime acceptance, bridge dispatch,
  flash, serial writes, monitor, and live proof closed.
- `firmware/projects/espnow-bbs/main/main.c` remains inert and does not include
  or start the M4L runtime task shell.
- The consolidated plan names separately gated firmware startup/lifecycle/bridge
  follow-up as the next safe mesh-discovery continuation.
- This M4M gate does not edit firmware source, CMake, tests, generated build
  artifacts, or `sdkconfig`.

## Assumptions

- Design-only startup/lifecycle requirements should precede source
  implementation because lifecycle ownership, accepted transitions, task buffer
  lifetime, failure cleanup, and recovery behavior are still unknown.
- A future source gate can use this packet to choose whether startup remains
  caller-driven or whether a narrow `app_main` integration path is opened.

## Unknowns

- No accepted automatic startup path, lifecycle state object, static runtime
  state, task-buffer lifetime, stack adequacy, watchdog policy, startup order,
  shutdown timing, bridge-visible lifecycle summary, first-flash target, route
  table, healing, BLE/GATT, Android app, router/admin policy, PCAP, transcript,
  rollback, cleanup, or live proof exists.

## Pre-Mutation Reviewer Quorum

Lifecycle state listing was not visible before spawning reviewers. Two
project-local read-only reviewers were spawned, waited, and closed after output
capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for starting M4M as a design-only gate. Required no firmware source, no CMake/tests, no `app_main` start, no runtime/bridge/live/flash/serial/release authority, preservation of `mesh_discovery.v1`, Gate F, bridge ABI, coordinator serial ABI, Win31 transport, M4K policies, and M4L static-buffer boundary. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers for starting docs-only mutation. Required architecture note, source ledger, source-index row, Task 0198, Handoff 0145, docs/status updates, validation commands, stale scans, and preservation of dirty-tree boundaries. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for starting
the named M4M design-only boundary.

## Post-Mutation Reviewer Quorum

Two project-local read-only reviewers were waited and closed after output
capture before final acceptance wording.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | approve with wording cleanup | No P1/P2 blockers for accepting M4M as design-only startup/lifecycle requirements after status cleanup. Confirmed preservation of `mesh_discovery.v1`, Gate F service codes, bridge ABI, coordinator serial ABI, Win31 transport, M4K policy strings, M4L static-buffer boundary, inert `app_main`, and closed live/source surfaces. |
| QA/evidence reviewer | 3 | approve after record remediation | Found one P2 record-state blocker while records still said validation pending; after remediation, the evidence set is acceptable as design-only requirements only. Required validation commands and closed-authority wording to be recorded. |

Weighted post-mutation result after record cleanup: 8/8 approval for accepting
the named M4M design-only requirements boundary, with no P1/P2 blockers.

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

## Decision

M4M is accepted as design-only startup/lifecycle requirements after clean
scaffold validation and post-review. Future startup source implementation,
`app_main` integration, firmware runtime behavior, bridge dispatch, persistence,
first-flash proof, and M5 live mesh remain separate gates. This gate does not
authorize firmware source implementation, CMake/test changes, automatic startup
from `app_main`, runtime behavior, bridge dispatch, persistence, live task
proof, live mesh, BLE, Android, router/admin, flash, serial/RF/XBee, release,
publication, commit, push, PR, or deploy.
