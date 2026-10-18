# Task 0216: Full-Service Mesh Discovery Gate M4W Retained Recovery App Main Source

Status: accepted-source-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4W-RETAINED-RECOVERY-APP-MAIN-SOURCE-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`,
`SRC-ESP-IDF-NVS`, `SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`, and `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`.

## Routing

- Verified facts: required governance/docs/source-index files were reread.
  M4V accepts source-only retained persistence adapter code but explicitly does
  not integrate retained recovery into `app_main`. Current `app_main` had M4O
  startup lifecycle wiring only before this task. `main/CMakeLists.txt`
  required only `mesh_discovery_contract mesh_startup_lifecycle` before this
  task.
- Assumptions: a source-only retained-recovery app-main integration can improve
  live-readiness without claiming live NVS execution, reboot durability,
  runtime persistence proof, first-flash proof, or firmware runtime acceptance.
- Unknowns/open evidence: live NVS partition behavior, missing/corrupt live
  partition behavior, reboot durability, runtime persistence, first-flash
  behavior, stack adequacy, watchdog behavior, live bridge dispatch, bridge
  transcript, Win31/OPCON proof, rollback, and live proof remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Firmware + Communications + Architecture with QA/Evidence/
  AgentOps.
- Evidence need: M4V/M4U/M4O records, source-indexed ESP-IDF NVS source,
  current source diff, focused scaffold tests, host tests, firmware/data/
  record/source/docs/path/agent-process/pages/React Native/skills audits,
  no-flash ESP-IDF build, generated-output cleanup, closed-surface scans,
  reviewer quorum, task/handoff/source records, and post-review acceptance.
- Mutation boundary: `firmware/projects/espnow-bbs/main/main.c`,
  `firmware/projects/espnow-bbs/main/CMakeLists.txt`,
  `tests/scaffold_audits/test_espnow_bbs_app_main_startup_integration.py`,
  `scripts/scaffold_audit_firmware.py`, `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4w-retained-recovery-app-main-source.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4w-retained-recovery-app-main-source.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0155-full-service-mesh-discovery-gate-m4w-retained-recovery-app-main-source-to-qa-firmware-communications.md`.
- Reviewer quorum: read-only Architecture/Communications/Firmware reviewer
  `019e9e86-5d9a-76a1-8e8f-ef878d47a65e` weight 5 and read-only
  QA/Evidence/AgentOps reviewer `019e9e86-74a3-7911-8061-92ad93883526`
  weight 3 approved with no P1/P2 blockers, conditioned on focused audit
  coverage, fail-closed retained intent initialization, load-only app-main
  recovery, explicit deinit policy, preserved M4O startup behavior, and no
  live/hardware/release authority. Weighted pre-mutation disposition: 8/8
  approval with no P1/P2 blockers.
- Agent lifecycle cleanup: both reviewers were waited, outputs captured, and
  closed before mutation. No mutating worker agents were spawned.
- Post-mutation reviewer quorum: read-only Architecture/Communications/Firmware
  reviewer `019e9e95-dea4-7a22-89d8-3f99e86b9dad` weight 5 and read-only
  QA/Evidence/AgentOps reviewer `019e9e95-fdf2-7143-9649-c35483efad68` weight
  3 approved with no P1/P2 blockers. Weighted post-mutation disposition: 8/8
  approval for M4W source-only acceptance. Both reviewers were waited, outputs
  captured, and closed before finalization.
- Gate authority: source-only app-main retained recovery integration, focused
  audit/test updates, docs/status/source/task/handoff records, host tests,
  audits, no-flash build, and post-review only.

## Implementation

- Added `mesh_retained_persistence` as a private main component dependency.
- Added static retained storage context, static M4T persistence intent, and
  retained init/load status fields in `main.c`.
- Added app startup retained recovery that initializes retained intent to empty
  volatile state, initializes retained storage context, calls storage init,
  records init status, calls storage load only if init succeeds, and records
  load status.
- Preserved M4O lifecycle configure/start ordering and did not wire save,
  erase, direct NVS writes, bridge dispatch, serial output, RF/XBee, Wi-Fi,
  ESP-NOW, ESP-WIFI-MESH, BLE, timers, file I/O, dynamic allocation, or logs in
  `main.c`.
- Recorded app-lifetime/no-shutdown storage policy as source-only; no automatic
  storage deinit or reboot durability proof is accepted.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence`
  passed with 9 tests.
- Firmware, data, records, sources, docs, paths, agent-process, pages,
  React Native, and skills scaffold audits passed.
- No-flash ESP-IDF v6.0.1 build passed:
  `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4w-retained-recovery-app-main-source build`.
  The build generated `espnow_bbs_build_only.bin` size `0x283e0` with
  `0xd7c20` bytes free in the 1 MiB app partition.
- Generated `sdkconfig` residue was removed. Scoped `git diff --check`,
  closed-surface scans, and stale acceptance wording scans passed.
- Final post-mutation reviewer quorum passed 8/8 weighted with no P1/P2
  blockers.

## Open Evidence

- Live NVS execution, reboot durability, runtime persistence, live recovery,
  first-flash target, live bridge dispatch, bridge frame parsing, bridge ABI,
  serial ABI, timer behavior, route table, root/parent behavior, healing,
  watchdog behavior, stack adequacy, bridge transcript, Win31/OPCON proof,
  rollback, and live proof remain unaccepted.

## Authority Limits

This task does not authorize live NVS execution proof, reboot durability,
runtime persistence proof, firmware runtime acceptance, live bridge dispatch,
bridge frame parsing, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing,
Android behavior, router/admin mutation, PCAP, flash, erase, monitor, serial
writes, serial ABI expansion, bridge ABI changes, Gate F service-code changes,
`mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
relay actions, load/mains work, live bench, release, publication, public Pages
allowlist/tooling changes, app/client changes, package/lockfile changes,
commit, push, PR, deploy, `/etc/codex/requirements.toml`, or `admin-strict`
mutation.

## Handoff

[../handoffs/0155-full-service-mesh-discovery-gate-m4w-retained-recovery-app-main-source-to-qa-firmware-communications.md](../handoffs/0155-full-service-mesh-discovery-gate-m4w-retained-recovery-app-main-source-to-qa-firmware-communications.md)

## Decision

Decision accepted: source-only app-main retained recovery integration after
M4V. It is not accepted reboot durability, runtime persistence proof, live NVS
execution proof, live bridge dispatch, flash, serial writes, or live proof.
