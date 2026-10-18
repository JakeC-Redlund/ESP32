# Source Ledger - 2026-06-05 Full-Service Mesh Discovery Gate M4 Host Adapter

## Scope

Tier 2 host-only adapter fixture and durable records for Gate M4
full-service mesh discovery. The adapter maps future firmware-like mesh event
records into the existing `mesh_discovery.v1` host simulator.

This ledger records host adapter proof only. It does not accept ESP-IDF
firmware source, firmware target creation, firmware build output, firmware
runtime implementation, firmware persistence, live ESP-WIFI-MESH, BLE pairing,
Android app behavior, router/admin mutation, serial/RF/XBee writes,
relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M2A-DOSC-2026-05-27`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`
- `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-RUNTIME-REQUIREMENTS-2026-05-26`
- `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-PHASE-5-6-RUNTIME-DESIGN-REVIEW-2026-05-26`
- `SRC-LOCAL-HOST-PROTOCOL-CUSTODY-REVIEW-2026-06-05`
- `SRC-ESP-IDF-WIFI-MESH`
- `SRC-ESP-IDF-RF-COEXIST`
- `SRC-LOCAL-AGENTIC-PLANNING-GUIDE-2026-06-05`

## Verified Facts

- Gate M3 left a concrete M4 fixture checklist: event vectors, 512-byte bridge
  bounds, secret rejection, live-action rejection, queue/custody isolation, and
  schema/ABI invariance.
- The M4 adapter fixture is pure Python under
  `tools/simulators/custom_wireless_protocol/` and does not add framework
  files or firmware source.
- Focused tests now cover all M3 M4 fixture requirements and preserve the Gate
  F/discovery ABI boundary.
- Compact bridge summaries are used only when maximum summary requests would
  otherwise exceed the 512-byte line budget; internal event logs keep full
  event names.

## Assumptions

- Host-only event dictionaries are sufficient for this M4 proof because no
  exact ESP-IDF callback/API selection is accepted.
- A later firmware target/build gate may reuse these fixture expectations, but
  this ledger does not claim that target exists.

## Unknowns

- No firmware memory budget, FreeRTOS task budget, persistence mechanism,
  recovery behavior, RF coexistence measurement, route table, parent/root,
  healing, BLE/GATT, Android, router/admin, flash, serial, cleanup, rollback,
  or live proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Communications/protocol bridge ABI reviewer | 3 | approve with conditions | Required host-only adapter scope, full event vectors, max bridge bounds, secret/live rejection, queue/custody isolation, and ABI invariance. |
| QA/evidence-record reviewer | 3 | approve with conditions | Required task/source ledger/source-index/status records and closed-surface wording. |

Weighted disposition: 6/6 approved for the named Tier 2 host-only mutation
boundary. Threshold: 70 percent. No P1 blockers remained. P2 conditions are
addressed by the adapter fixture, tests, this source ledger, task record,
handoff, source-index row, architecture note, and status updates.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.custom_wireless_protocol.test_espnow_bbs_custom_protocol`
  passed: 40 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 184`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `git diff --check` passed with exit code 0.

Broader scaffold skill/verify checks are not claimed by this ledger:
`scripts/scaffold_audit_skills.py` and `scripts/verify_scaffold.py` were not run
for this M4 gate.

## Decision

Decision: accept the M4 host adapter fixture proof only. Client-1
static/simulated browser proof is now recorded separately; future mesh work may
choose a separately gated firmware target/build proof. Live mesh remains
M5/Tier 3 only.
