# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4T Persistence Intent Source Contract

## Scope

Tier 2 pure-C firmware source proof for a caller-owned volatile
persistence-intent and accounting contract over the accepted M4S request-text
intent route.

This ledger records the M4T source API, host C tests, scaffold audit coverage,
docs/status records, and read-only reviewer output. It does not accept retained
persistence, NVS writes, file writes, reboot durability, runtime persistence,
live bridge dispatch, arbitrary bridge frame parsing, transport framing, serial
ABI expansion, bridge ABI changes, Gate F request or service changes,
`mesh_discovery.v1` schema changes, timer creation, firmware runtime
acceptance, live task execution proof, flash, erase, monitor, serial writes,
ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app behavior,
router/admin mutation, PCAP, RF/XBee writes, relay/load/mains, release,
publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4S is accepted source-only and leaves retained persistence separate.
- Existing `mesh_discovery_contract` request membership preserves accepted
  discovery request names and Gate F bridge request/service disjointness.
- M4T adds `espnow_bbs_mesh_contract_persistence_intent_t`,
  `espnow_bbs_mesh_contract_persistence_intent_init`,
  `espnow_bbs_mesh_contract_persistence_intent_reset`,
  `espnow_bbs_mesh_contract_persistence_intent_stage`, and
  `espnow_bbs_mesh_contract_persistence_intent_summary`.
- M4T host tests cover accepted M4S-backed staging, malformed request
  rejection through M4S before envelope/router accounting, downstream rejection
  without staging, undersized output rejection, deterministic volatile
  accounting, summary bounds/closed flags, and non-mutation of queue, owner,
  and runtime scheduler state.

## Assumptions

- A volatile persistence-intent/accounting layer is safe to advance before
  retained persistence because it does not write NVS/files, retain payloads,
  mutate runtime state, or change protocol membership.
- A no-flash ESP-IDF build can prove compile-time integration only.

## Unknowns

- No NVS namespace/key policy, file format, reboot recovery behavior, wear
  policy, cleanup, runtime persistence, live bridge transcript, bridge frame
  parsing, serial ABI, bridge ABI, firmware runtime acceptance, timer behavior,
  route table, parent/root behavior, healing behavior, watchdog behavior, stack
  adequacy, first-flash behavior, rollback, cleanup, or live proof is accepted.

## Reviewer Quorum

Lifecycle listing was not visible through the available tools before spawning.
Two read-only reviewers were spawned, waited, and closed after output capture
before mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer | 5 | approve | No P1/P2 blockers for opening M4T if it remains pure-C volatile caller-owned persistence-intent/accounting over M4S, records only counters/last request/status/staged bytes, does not retain payloads, and keeps NVS/files/timers/tasks/queues, bridge/serial ABI, Gate F, schema, live dispatch, flash, serial/RF, and release closed. |
| QA/Evidence/Agent Operations reviewer | 3 | approve with conditions | No P1/P2 blockers if M4T is not described as accepted persistence, reboot durability, NVS/file storage, or live/runtime persistence, and if records align aggregate M4S/M4T status. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4T source boundary.

Post-mutation reviewers were spawned, waited, captured, and closed after
source/docs/status mutation. Lifecycle listing was not exposed by available
tools before spawning.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer | 5 | approve with conditions | Found no source-boundary defect and one P2 record-alignment issue in the development-plan primary rows. The P2 was resolved by updating the full-service mesh and BBS UI table rows to include accepted M4T status, source ID, closed retained/runtime persistence surfaces, and current truth. |
| QA/Evidence/Agent Operations reviewer | 3 | approve with conditions | Found no P1/P2 blockers; required replacing pending-review wording with accepted source-only wording after quorum passed while preserving all closed-surface disclaimers. |

Weighted post-mutation result: 8/8 approval after the development-plan
record-alignment P2 was resolved. No P1/P2 blockers remain for accepted M4T
source-only status.

## Validation

Validation passed:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 205
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4t-persistence-intent-source-contract build
git diff --check
```

- Host contract tests passed.
- Firmware audit passed.
- Focused mesh discovery scaffold unittest passed: 2 tests.
- Record, source, docs, path, data, agent-process, pages, React Native, and
  skills audits passed.
- Broader selected scaffold unittests passed: 15 tests.
- No-flash ESP-IDF v6.0.1 build passed in
  `/tmp/esp32-espnow-bbs-m4t-persistence-intent-source-contract`;
  `espnow_bbs_build_only.bin` size was `0x22d20` with `0xdd2e0` bytes free in
  the 1 MiB app partition.
- Generated-output cleanup checks passed after removing the generated project
  `sdkconfig`.
- Closed-surface scan found only the existing denied-token string
  `serial_write`, not a call site.
- `git diff --check` passed.
- Final post-review passed after the development-plan record-alignment P2 was resolved.

## Decision

M4T is accepted source-only for the pure-C caller-owned volatile
persistence-intent source contract over accepted M4S. Retained persistence, NVS/file
storage, reboot durability, runtime persistence, live bridge dispatch,
first-flash proof, and M5 live mesh remain separate gates.
