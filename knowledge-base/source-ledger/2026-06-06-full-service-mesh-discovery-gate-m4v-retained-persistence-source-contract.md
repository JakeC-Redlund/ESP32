# Source Ledger - 2026-06-06 Full-Service Mesh Discovery Gate M4V Retained Persistence Source Contract

## Scope

Tier 2 firmware source proof for a separate ESP-NOW BBS retained persistence
component after accepted M4U retained-persistence requirements.

This ledger records the accepted M4V source API, host tests, scaffold audit
coverage, docs/status records, no-flash build proof, and final 8/8 weighted
post-review output. It accepts source-only NVS adapter code in
`mesh_retained_persistence`; it does not accept
live NVS execution proof, reboot durability, runtime persistence proof, live
bridge dispatch, arbitrary bridge frame parsing, serial ABI expansion, bridge
ABI changes, Gate F request/service changes, `mesh_discovery.v1` schema
changes, timer creation, firmware runtime acceptance, live task execution
proof, flash, erase, monitor, serial writes, ESP-NOW runtime, ESP-WIFI-MESH
runtime, BLE pairing, Android behavior, router/admin mutation, PCAP, RF/XBee
writes, relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`
- `SRC-ESP-IDF-NVS`
- `SRC-ESP-IDF-STABLE-ESP32`
- `SRC-ESP-IDF-GET-STARTED`
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Verified Facts

- M4U is accepted design-only and leaves retained-persistence source
  implementation separately gated.
- M4V adds a new `mesh_retained_persistence` component with public header,
  pure-C retained checkpoint codec, and NVS-facing adapter source.
- The codec stores only bounded M4T-derived fields: schema identity, record
  version, last normalized request, last status, attempted count, staged count,
  rejected count, and staged byte total.
- The adapter uses namespace `mesh_retained`, key `m4t_checkpoint`, storage
  owner `mesh_ingress`, and caller-triggered save/erase only.
- Host tests cover encode/decode, missing record, corrupt record, unsupported
  version, bounded-field and counter rejection, non-ASCII rejection, secret-like
  field rejection, no generated summary retention, no serial payload retention,
  no runtime scheduler stepping, no queue/owner mutation during recovery,
  storage init/load/save/erase/deinit behavior through host NVS stubs, and
  closed summary markers.

## Assumptions

- Source-only NVS adapter code is safe to accept before live proof because no
  flash, serial, monitor, or hardware command was run.
- A no-flash ESP-IDF build proves compile-time integration only.

## Unknowns

- No live NVS partition behavior, power-loss behavior, reboot durability,
  runtime persistence proof, stack impact, first-flash behavior, bridge
  transcript, Win31/OPCON corroboration, rollback, or live proof is accepted.

## Reviewer Quorum

Lifecycle listing was not visible through the available tools before spawning.
Two read-only reviewers were spawned, waited, captured, and closed before
mutation.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| Architecture/Communications/Firmware reviewer | 5 | approve with conditions | No P1/P2 blockers for opening M4V if it uses a separate `mesh_retained_persistence` component, keeps `mesh_discovery_contract` pure-C/storage-free, makes the record codec host-testable without ESP-IDF/NVS dependencies, allows NVS markers only in the NVS adapter, tests corrupt/missing/unsupported and no-mutation cases, and does not claim reboot durability, runtime persistence proof, live recovery, first-flash proof, bridge dispatch, ABI/schema changes, or live mesh. |
| QA/Evidence/Agent Operations reviewer | 3 | approve | No P1/P2 blockers for opening M4V. Conditions: path-aware audit exceptions only for the new component, required-path/allowlist updates, codec host tests independent from NVS, cleanup/deinit testing, closed-surface scans, no-flash build if available, and no claims of live/runtime persistence or ABI/schema changes. |

Weighted pre-mutation result: 8/8 approval with no P1/P2 blockers for the
named M4V source boundary.

First post-mutation reviewer quorum found no P1 blockers and two P2 blockers:
wrong-length NVS blob load could return without clearing caller-owned intent,
and top-level architecture/source-ledger wording said the source-only adapter
was accepted before final post-review. The reviewers
`019e9e00-5df8-7360-97b5-81852d8fcf28` and
`019e9e00-5ed2-7930-851f-45056628de80` were waited, captured, and closed.

P2 remediation changed `storage_load` to clear caller-owned persistence intent
on mapped NVS blob read errors, added a host-only over-sized NVS blob regression
test for `REJECT_LENGTH` plus volatile-state clearing, and changed the top-level
architecture/source-ledger wording from accepted to proposed before final
post-review.

Fresh Architecture/Communications/Firmware reviewer
`019e9e0a-734a-7650-8008-96db545fce48` approved the remediated source-only
boundary 5/5 with no P1/P2 findings and was closed. Fresh QA/Evidence/AgentOps
reviewer `019e9e0a-7e2e-7520-aa8d-c0437f43575b` verified the wrong-length NVS
P2 remediation but found one remaining P2: the M4V source-index row still said
"accepted source boundary" while final post-review was pending. That reviewer
was closed.

Source-index P2 remediation changed the M4V row to "The source boundary
names..." instead of "The accepted source boundary names...".

Narrow QA/Evidence/AgentOps confirmation reviewer
`019e9e0f-3646-7430-84c2-eead41dab09f` approved the source-index remediation
3/3 with no P1/P2 findings and was closed. Final post-mutation reviewer
disposition is 8/8 weighted approval with no P1/P2 blockers for the named
source-only M4V boundary.

## Validation

Validation run so far:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 207
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4v-retained-persistence-source build
git diff --check
```

Results:

- Retained-persistence host tests passed.
- Existing discovery-contract host tests passed.
- Selected scaffold unittest batch passed: 18 tests.
- Firmware audit passed.
- Record/source/docs/path/data/agent-process/pages/React Native/skills audits
  passed.
- No-flash ESP-IDF v6.0.1 build passed in
  `/tmp/esp32-espnow-bbs-m4v-retained-persistence-source`;
  `espnow_bbs_build_only.bin` size was `0x22d20` with `0xdd2e0` bytes free in
  the 1 MiB app partition.
- Generated project `sdkconfig` was removed from
  `firmware/projects/espnow-bbs/`.
- Targeted closed-surface scans found only source-only NVS adapter references
  or explicit closed authority statements.
- `git diff --check` passed.
- After P2 remediation, the host tests, selected scaffold unittests, firmware
  audit, record/source/docs/path/data/agent-process/pages/React Native/skills
  audits, no-flash ESP-IDF build, generated-output cleanup, stale acceptance
  wording scan, and `git diff --check` were rerun and passed.
- Final post-review passed 8/8 weighted after the wrong-length NVS and
  source-index wording P2s were resolved.

## Decision

M4V is accepted source-only for retained persistence adapter code plus
host-testable retained checkpoint handling after final 8/8 weighted
post-review. Reboot durability, runtime persistence proof, live NVS execution
proof, live bridge dispatch, first-flash proof, and M5 live mesh remain
separate gates.
