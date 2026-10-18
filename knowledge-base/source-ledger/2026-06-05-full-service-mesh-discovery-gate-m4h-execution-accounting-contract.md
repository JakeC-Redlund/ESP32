# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4H Execution Accounting Contract

## Scope

Tier 2 inert firmware interface source proof for the ESP-NOW BBS full-service
mesh discovery lane.

This ledger records only a caller-owned execution-accounting contract inside
the existing pure-C `mesh_discovery_contract` component, host C tests,
recursive firmware-audit coverage, docs/status records, and no-flash local
ESP-IDF build proof. It does not accept firmware runtime implementation,
actual FreeRTOS task creation, static runtime queues, dispatch loops,
callbacks, handler calls, firmware persistence, live ESP-WIFI-MESH, BLE
pairing, Android app behavior, router/admin mutation, flash, erase, monitor,
serial writes, serial ABI expansion, bridge ABI changes, PCAP, RF/XBee writes,
relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-START-PROJECT`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for the `espnow-bbs`
  coordinator/client firmware lane.
- Gate M4F accepted caller-owned `espnow_bbs_mesh_contract_dispatch_plan_t`
  state over the M4E owner state and M4D event queue.
- Gate M4G accepted caller-owned scheduler profile/tick accounting over the
  M4F dispatch plan.
- This gate extends the same component with caller-owned
  `espnow_bbs_mesh_contract_execution_accounting_t` state.
- Execution cycles are capped at 64.
- `execution_accounting_apply_plan` validates non-null caller-owned state,
  non-empty staged plans, cycle bounds, known owner names, known action names,
  same-owner ready steps, non-ready planned-step rejection, and at least one
  applied ready step.
- Accepted accounting records apply/skip totals and drain/ingest/snapshot-ready
  totals locally and does not own or start runtime work.
- Execution summaries are compact local snapshots and do not add a bridge
  request, Gate F service, coordinator serial ABI, Win31 transport, or schema
  change.
- `tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py` compiles the
  component source with `-Wall -Wextra -Werror`.

## Assumptions

- Execution-accounting contract proof is the correct next step after M4G
  because it validates deterministic local accounting over an already staged
  plan before runtime APIs, task creation, persistence, or bridge dispatch are
  introduced.
- Future runtime scheduler implementation will use separate gates tied to Gate
  F and `mesh_discovery.v1` invariants.

## Unknowns

- No runtime task graph, actual scheduler loop, queue budget acceptance, memory
  budget, persistence, first-flash target, recovery path, live route-table
  proof, root/parent selection, healing, coexistence, BLE/GATT, Android,
  router/admin, PCAP, transcript, cleanup, rollback, or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Firmware/Communications/Architecture reviewer | 5 | pass with conditions | No P1/P2 blockers for the named M4H boundary; required caller-owned pure-C accounting over a staged plan only, no owner/queue/scheduler/plan mutation, no bridge dispatch, no callbacks or handlers, no ABI/schema drift, and no Tier 3 authority. |
| QA/evidence reviewer | 3 | pass with conditions | No P1/P2 blockers to starting; required host C tests, recursive firmware audit, no-flash build, scaffold audits, expanded runtime marker scans, durable records, and post-review. |

Post-implementation read-only reviewers were waited and closed after output
capture. Firmware/Communications/Architecture voted PASS with no P1/P2
findings. QA/evidence identified one P2 stale pending-record wording issue;
this final record alignment remediates that condition without expanding the
M4H mutation boundary or Tier 3 authority.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_build_only_target tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 44 tests OK.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4h-execution-accounting-contract build`
  passed with exit code 0. ESP-IDF reported IDF version `6.0`, target `esp32`,
  project `espnow_bbs_build_only`, app binary size `0x21ab0`, and `0xde550`
  bytes free in the smallest app partition.
- The generated project `sdkconfig` was removed after the build, and
  generated-output checks found no `sdkconfig*` files under
  `firmware/projects/espnow-bbs`.
- Direct runtime/allocation marker scan over `firmware/projects/espnow-bbs`
  returned no matches.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py` passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 193`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py` passed.
- `git diff --check` passed.

## Decision

Decision: accept the M4H execution-accounting contract proof only. Future
actual task creation, firmware runtime implementation, runtime scheduler
implementation, persistence, first-flash proof, and M5 live mesh remain
separate gates.
