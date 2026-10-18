# Task 0217: Full-Service Mesh Discovery Gate M4X Bridge Readiness App Main Source

Status: accepted-source-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4X-BRIDGE-READINESS-APP-MAIN-SOURCE-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4W-RETAINED-RECOVERY-APP-MAIN-SOURCE-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`, `SRC-ESP-IDF-GET-STARTED`, and
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`.

## Routing

- Verified facts: required governance/docs/source-index files were reread.
  M4W accepts source-only retained recovery in `app_main` but explicitly leaves
  live bridge dispatch closed. M4P through M4S already accept caller-owned local
  bridge summary, request-router, dispatch-envelope, and request-text intent
  APIs over fixed discovery request names. Current `app_main` configured the
  contract, recovered retained intent, and started the lifecycle before this
  task.
- Assumptions: a source-only app-main bridge-readiness probe over the fixed
  accepted `discovery_snapshot` request can improve live-readiness without
  accepting live dispatch, arbitrary frame parsing, ABI changes, runtime proof,
  first flash, or hardware proof.
- Unknowns/open evidence: live bridge input source, serial transcript, bridge
  ABI, serial ABI, firmware runtime behavior, live task behavior, live NVS
  behavior, reboot durability, first-flash behavior, rollback, and M5 live mesh
  behavior remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Communications + Firmware + Architecture with QA/Evidence and
  AgentOps.
- Evidence need: M4P/M4Q/M4R/M4S/M4W source records, current source diff,
  focused scaffold tests, host tests, firmware/data/record/source/docs/path/
  agent-process/pages/React Native/skills audits, no-flash ESP-IDF build,
  generated-output cleanup, closed-surface scans, reviewer quorum, task/
  handoff/source records, and post-review acceptance.
- Mutation boundary: `firmware/projects/espnow-bbs/main/main.c`,
  `tests/scaffold_audits/test_espnow_bbs_app_main_startup_integration.py`,
  `scripts/scaffold_audit_firmware.py`, `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4x-bridge-readiness-app-main-source.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4x-bridge-readiness-app-main-source.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0156-full-service-mesh-discovery-gate-m4x-bridge-readiness-app-main-source-to-qa-firmware-communications.md`.
- Reviewer quorum: read-only Architecture/Communications/Firmware reviewer
  `019e9eb2-f90b-7202-84fa-1af6d996ab43` weight 5, read-only QA/Evidence
  reviewer `019e9eb2-ff9e-7a43-9104-231003453423` weight 3, and read-only
  Agent Process reviewer `019e9eb3-17c0-7903-b3e4-16766c0e1318` weight 2
  approved the source-only M4X boundary with no P1/P2 blockers, conditioned on
  fixed request text, local status capture only, preserved lifecycle startup,
  focused audit coverage, and closed live/ABI/hardware/release surfaces.
  Weighted pre-mutation disposition: 10/10 approval with no P1/P2 blockers.
- Agent lifecycle cleanup: all three pre-mutation reviewers were waited,
  outputs captured, and closed before mutation. No mutating worker agents were
  spawned.
- Post-mutation reviewer quorum: read-only Architecture/Communications/
  Firmware reviewer `019e9ec1-58f2-7942-bcc3-5e3d81e6fe14` weight 5 found no
  source-boundary P1/P2 blockers and approved with a record-alignment
  condition. Read-only QA/Evidence reviewer
  `019e9ec1-5fca-7870-9be5-fda80ecaebf9` weight 3 and read-only Agent Process
  reviewer `019e9ec1-6738-7a22-b14a-9c5238355457` weight 2 rejected final
  acceptance until the same P2 record-alignment remediation: stale wording
  said full validation, cleanup, closed-surface scans, `git diff --check`, and
  final post-review still remained after those checks had passed. This task
  record, source ledger, architecture note, source index, handoff, and status
  records were updated to record the completed validation matrix and
  post-review outputs. Weighted post-mutation disposition after P2 remediation:
  10/10 conditional approval with no remaining P1/P2 blockers for M4X
  source-only acceptance.
- Gate authority: source-only app-main bridge-readiness status capture, focused
  audit/test updates, docs/status/source/task/handoff records, host tests,
  audits, no-flash build, and post-review only.

## Implementation

- Added static app-main-owned bridge summary state, request router, dispatch
  envelope, request-text intent, summary buffer, status field, and line-byte
  field.
- Added `espnow_bbs_mesh_app_capture_bridge_readiness()` after successful
  contract configuration and retained load-only recovery, before lifecycle
  configure/start.
- The helper uses only the fixed literal request
  `ESPNOW_BBS_MESH_APP_BRIDGE_READINESS_REQUEST`, set to
  `discovery_snapshot`.
- The helper initializes caller-owned local bridge state and calls
  `espnow_bbs_mesh_contract_bridge_request_text_intent_route()` into a static
  `ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u` buffer.
- The helper records bridge readiness status and line bytes only. It does not
  block lifecycle start on readiness failure.
- No live bridge dispatch, arbitrary bridge frame parsing, serial output,
  bridge ABI change, Gate F change, direct NVS write, retained save/erase,
  Wi-Fi, ESP-NOW, ESP-WIFI-MESH, BLE, timers, HTTPD, file I/O, dynamic
  allocation, or logs were added.

## Validation

Validation passed:

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration`
  passed with 7 tests.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence`
  passed with 12 tests.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- Firmware, data, records, sources, docs, paths, agent-process, pages,
  React Native, and skills scaffold audits passed.
- No-flash ESP-IDF v6.0.1 build passed:
  `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4x-bridge-readiness-app-main-source build`.
  The build generated `espnow_bbs_build_only.bin` size `0x29940` with
  `0xd66c0` bytes free in the 1 MiB app partition.
- Full `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py` passed.
- Generated `sdkconfig` residue was removed. Scoped stale-wording and
  closed-surface scans passed. `git diff --check` passed.
- Final post-mutation reviewer quorum found no source-boundary P1/P2 blockers;
  the P2 stale-record alignment issue was remediated in this record update.

## Open Evidence

- Live bridge dispatch, arbitrary bridge frame parsing, bridge ABI, serial ABI,
  live NVS execution, reboot durability, runtime persistence, first-flash
  target behavior, firmware runtime acceptance, live task behavior, route
  table behavior, root/parent behavior, healing, watchdog behavior, stack
  adequacy, bridge transcript, Win31/OPCON proof, rollback, and live proof
  remain unaccepted.

## Authority Limits

This task does not authorize live bridge dispatch, arbitrary bridge frame
parsing, serial ABI expansion, bridge ABI changes, Gate F request/service
changes, `mesh_discovery.v1` schema changes, live NVS execution proof, reboot
durability, runtime persistence proof, firmware runtime acceptance, first-flash
proof, live task execution proof, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE
pairing, Android behavior, router/admin mutation, PCAP, flash, erase, monitor,
serial writes, RF/XBee writes, relay actions, load/mains work, live bench,
release, publication, public Pages allowlist/tooling changes, app/client
changes, package/lockfile changes, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0156-full-service-mesh-discovery-gate-m4x-bridge-readiness-app-main-source-to-qa-firmware-communications.md](../handoffs/0156-full-service-mesh-discovery-gate-m4x-bridge-readiness-app-main-source-to-qa-firmware-communications.md)

## Decision

Decision accepted after P2 record-alignment remediation: source-only app-main
bridge readiness status capture after M4W. It is not accepted live bridge
dispatch, arbitrary bridge frame parsing, runtime proof, live NVS proof,
first-flash proof, M5 live mesh, hardware, flash, serial writes, release, or
live proof.
