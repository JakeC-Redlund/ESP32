# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4P Bridge Summary Source Contract

## Scope

Tier 2 pure-C firmware source proof for a bounded bridge-summary selector over
the accepted `mesh_discovery_contract` caller-owned state.

This ledger records the M4P source API, host C tests, scaffold audit coverage,
docs/status records, and read-only reviewer output. It does not accept live
bridge dispatch, serial ABI expansion, Gate F request or service changes,
`mesh_discovery.v1` schema changes, persistence, timer creation, firmware
runtime acceptance, live task execution proof, flash, erase, monitor, serial
writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains,
release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4O accepts only no-flash `app_main` startup source integration and leaves
  bridge work separate.
- Existing `mesh_discovery_contract` request membership preserves accepted
  discovery request names and Gate F bridge request/service disjointness.
- M4P adds `espnow_bbs_mesh_contract_bridge_summary_state_t` and
  `espnow_bbs_mesh_contract_bridge_summary`.
- M4P host tests cover all accepted discovery request names, null/non-ASCII/
  unknown request rejection, undersized output rejection, 512-byte ASCII bounds,
  missing caller-owned state rejection, and non-mutation of queue, owner, and
  runtime scheduler state.

## Assumptions

- Local summary selection is safe to advance before bridge dispatch because it
  does not parse bridge frames, write serial, mutate runtime state, or change
  protocol membership.
- A no-flash ESP-IDF build can prove compile-time integration only.

## Unknowns

- No live bridge dispatch proof, Win31/OPCON bridge request proof, firmware
  runtime acceptance, persistence model, timer behavior, route table,
  parent/root behavior, healing behavior, watchdog behavior, stack adequacy,
  first-flash behavior, rollback, cleanup, or live proof is accepted.

## Reviewer Quorum

Lifecycle listing was not visible through the available tools before spawning.
Two read-only reviewers were spawned, waited, and closed after output capture
before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer | 5 | conditional approve | No P1/P2 blockers for opening M4P if it stays a bounded pure-C summary selector and keeps live bridge dispatch, serial ABI, Gate F, schema, persistence, timer, runtime, flash, and live surfaces closed. |
| QA/Evidence/Agent Operations reviewer | 3 | conditional approve | No P1/P2 blockers for M4P if it receives task/handoff/source/docs records, audit updates, host tests, full scaffold validation, no-flash build, cleanup checks, and final post-review. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4P source boundary.

## Validation

Validation passed. Commands/results:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 201
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4p-bridge-summary-source-contract build
find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
git diff --check
```

- Focused scaffold unit tests passed: 15 tests.
- Host contract tests passed.
- Firmware, path, records, sources, docs, agent-process, and full scaffold
  validation passed.
- No-flash ESP-IDF v6.0.1 build passed; generated
  `espnow_bbs_build_only.bin` size `0x22d20`, leaving `0xdd2e0` bytes free in
  the 1 MiB app partition.
- Generated `firmware/projects/espnow-bbs/sdkconfig` was removed and the final
  `find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print`
  cleanup check had no output.
- Closed-surface scan found only the expected blocked-live-action
  `serial_write` string in the contract rejection table.
- `git diff --check` passed.
- Final post-review passed 8/8 weighted conditional approval. The sole P2
  record-alignment condition was resolved by adding M4P to project/status
  records; no P1/P2 blockers remain for the named source-only boundary.

## Decision

M4P is accepted after full validation and final reviewer disposition. The
accepted source boundary is limited to local bridge-summary selection; live
bridge dispatch, persistence, firmware runtime acceptance, first-flash proof,
and M5 live mesh remain separate gates.
