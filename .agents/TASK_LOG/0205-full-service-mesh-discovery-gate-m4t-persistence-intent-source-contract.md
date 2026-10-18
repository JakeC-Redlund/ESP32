# Task 0205: Full-Service Mesh Discovery Gate M4T Persistence Intent Source Contract

Status: accepted-persistence-intent-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Routing

- Verified facts: required governance/docs were reread. M4S is accepted
  source-only for caller-owned request-text intent/accounting over accepted
  M4R/M4Q. The consolidated development plan now records M4T as accepted
  source-only and leaves retained/runtime persistence as separately gated
  continuation work.
- Assumptions: a volatile caller-owned persistence-intent/accounting layer over
  M4S can advance persistence planning without claiming retained storage,
  reboot durability, NVS/file writes, runtime persistence, or live proof.
- Unknowns: NVS namespace/key plan, file format, reboot recovery behavior,
  flash wear policy, cleanup, runtime timing, live bridge dispatch, first-flash
  proof, and live mesh proof remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Communications/Firmware with Architecture, Agent Operations, and
  QA.
- Evidence need: accepted M4S/M4R/M4Q/M4P records, read-only reviewer quorum,
  host C tests, focused and broader audit validation, no-flash build, generated
  output cleanup, closed-surface scan, task/handoff/source records, and final
  post-review acceptance.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/include/espnow_bbs_mesh_discovery_contract.h`,
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/src/espnow_bbs_mesh_discovery_contract.c`,
  `tests/espnow_bbs_mesh_discovery_contract/test_mesh_discovery_contract.c`,
  `scripts/scaffold_audit_firmware.py`,
  `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4t-persistence-intent-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4t-persistence-intent-source-contract.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0152-full-service-mesh-discovery-gate-m4t-persistence-intent-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Pre-mutation read-only reviewers
  `019e9da6-e983-7793-8d38-efae54cb40ea`
  (Architecture/Communications/Firmware, weight 5) and
  `019e9da7-1a1b-76b2-a3d1-444a2b86e3e8`
  (QA/Evidence/AgentOps, weight 3) were spawned, waited, captured, and closed.
  Weighted pre-mutation disposition was 8/8 approval with no P1/P2 blockers for
  a volatile persistence-intent/accounting source boundary only.
- Post-mutation read-only reviewers
  `019e9dbe-62b6-70c2-ae3d-b7ea7df2fe2d`
  (Architecture/Communications/Firmware, weight 5) and
  `019e9dbe-9771-7920-a37a-269983de404e`
  (QA/Evidence/AgentOps, weight 3) were spawned, waited, captured, and closed.
  Weighted post-mutation disposition was 8/8 approval after the
  development-plan record-alignment P2 was resolved. No P1/P2 blockers remain
  for final M4T source-only acceptance.
- Gate authority: pure-C caller-owned local persistence-intent/accounting over
  accepted M4S request-text routing, focused tests, path-aware audits,
  docs/status records, task log, and handoff. No retained persistence, live
  proof, flash, serial, release, or publication authority is opened.

## Implementation

- Added `espnow_bbs_mesh_contract_persistence_intent_t` as caller-owned local
  volatile persistence-intent/accounting state.
- Added init/reset, stage, and summary APIs for the persistence-intent layer.
- The stage API calls accepted M4S request-text intent routing to generate a
  bounded local bridge summary, then records only counters, last
  normalized request/status, and staged byte totals in caller-owned state.
- M4T does not retain the generated bridge summary payload inside persistence
  state.
- Summary output includes `persistence=intent`, local counters, last
  request/status, staged bytes, `storage=volatile`, `nvs=closed`,
  `file=closed`, `live=blocked`, `serial=closed`, and `abi=closed`.

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
find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
rg -n "bridge dispatch|serial_write|uart_|nvs_|esp_timer|xTimer|xQueue|esp_wifi|esp_now|esp_mesh|esp_ble|esp_bt|httpd_|esp_partition|esp_flash|esp_ota|fopen|fwrite|printf|ESP_LOG" firmware/projects/espnow-bbs/components/mesh_discovery_contract firmware/projects/espnow-bbs/main/main.c
git diff --check
```

Results:

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
- Generated `firmware/projects/espnow-bbs/sdkconfig` build residue was removed;
  follow-up `sdkconfig*` and host-test binary cleanup checks passed.
- Closed-surface scan found only the existing denied-token string
  `serial_write`, not a call site.
- `git diff --check` passed.
- Final read-only post-review passed after the development-plan record-alignment P2 was resolved.

## Open Evidence

- Retained persistence, NVS namespace/key policy, file format, reboot recovery,
  wear policy, cleanup, runtime timing, live bridge dispatch, bridge frame
  parsing, bridge ABI, serial ABI, timer behavior, route table, root/parent
  behavior, healing, watchdog behavior, stack adequacy, first-flash behavior,
  bridge transcript, Win31/OPCON proof, rollback, and live proof remain
  unaccepted.

## Authority Limits

This task does not authorize NVS writes, file writes, retained storage, reboot
durability, firmware persistence acceptance, live bridge dispatch, bridge frame
parsing, firmware runtime acceptance, ESP-NOW runtime, ESP-WIFI-MESH runtime,
BLE pairing, Android app behavior, router/admin mutation, PCAP, flash, erase,
monitor, serial writes, serial ABI expansion, bridge ABI changes, Gate F
service-code changes, `mesh_discovery.v1` schema changes, Win31 transport
changes, RF/XBee writes, relay actions, load/mains work, live bench, release,
publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
`admin-strict` mutation.

## Handoff

[../handoffs/0152-full-service-mesh-discovery-gate-m4t-persistence-intent-source-contract-to-qa-firmware-communications.md](../handoffs/0152-full-service-mesh-discovery-gate-m4t-persistence-intent-source-contract-to-qa-firmware-communications.md)

## Decision

M4T is accepted source-only for a pure-C caller-owned volatile
persistence-intent source contract over accepted M4S. It is not accepted retained
persistence, NVS/file storage, reboot durability, runtime persistence, live
bridge dispatch, flash, serial writes, or live proof.
