# Task 0207: Full-Service Mesh Discovery Gate M4V Retained Persistence Source Contract

Status: accepted-retained-persistence-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-ESP-IDF-NVS`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Routing

- Verified facts: required governance/docs were reread. M4U is accepted
  design-only and names retained-persistence source implementation as a
  separate follow-up. `SRC-ESP-IDF-NVS` is present in the source index. The
  current app and mesh discovery contract are storage-free.
- Assumptions: a separate source-only retained persistence component can
  advance bounded checkpoint handling without claiming live NVS execution,
  reboot durability, runtime persistence proof, or live bridge behavior.
- Unknowns: live NVS partition behavior, power-loss behavior, flash wear beyond
  caller-triggered commit policy, reboot recovery, stack impact, first-flash
  behavior, runtime timing, bridge transcript behavior, Win31/OPCON behavior,
  rollback, and live proof remain unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Communications/Firmware with Architecture, Agent Operations, and
  QA.
- Evidence need: accepted M4U/M4T records, source-indexed NVS API, read-only
  reviewer quorum, host retained-persistence tests, focused and broader audit
  validation, no-flash build, generated-output cleanup, closed-surface scan,
  task/handoff/source records, and post-review acceptance.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_retained_persistence/`,
  `tests/espnow_bbs_mesh_retained_persistence/`,
  `tests/scaffold_audits/test_espnow_bbs_mesh_retained_persistence.py`,
  `scripts/scaffold_audit_firmware.py`,
  `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4v-retained-persistence-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4v-retained-persistence-source-contract.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0154-full-service-mesh-discovery-gate-m4v-retained-persistence-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Pre-mutation read-only reviewers
  `019e9de4-5c25-7ca2-b498-e34c14fc5729`
  (Architecture/Communications/Firmware, weight 5) and
  `019e9de4-7ce7-7783-8540-c1dbce40d9ce`
  (QA/Evidence/AgentOps, weight 3) were spawned, waited, captured, and closed.
  Weighted pre-mutation disposition was 8/8 approval with no P1/P2 blockers for
  the named M4V source boundary only.
- First post-mutation reviewer quorum was spawned, waited, captured, and closed:
  Architecture/Communications/Firmware reviewer
  `019e9e00-5df8-7360-97b5-81852d8fcf28` (weight 5) and QA/Evidence/AgentOps
  reviewer `019e9e00-5ed2-7930-851f-45056628de80` (weight 3). Both
  disapproved with P2 findings: wrong-length NVS blob load could return without
  clearing caller-owned intent, and two durable records used premature
  acceptance wording before final post-review.
- P2 remediation: `storage_load` now clears caller-owned persistence intent on
  mapped NVS blob errors, host tests seed an over-sized host-only NVS blob to
  assert `REJECT_LENGTH` plus volatile-state clearing, and top-level
  architecture/source-ledger wording now says the source-only adapter is
  proposed for final post-review rather than accepted before reviewer closure.
- Fresh Architecture/Communications/Firmware reviewer
  `019e9e0a-734a-7650-8008-96db545fce48` approved the remediated source-only
  boundary 5/5 with no P1/P2 findings and was closed. Fresh QA/Evidence/AgentOps
  reviewer `019e9e0a-7e2e-7520-aa8d-c0437f43575b` verified the wrong-length
  NVS P2 remediation but found one remaining P2: the M4V source-index row still
  said "accepted source boundary" while final post-review was pending. That
  reviewer was closed.
- Source-index P2 remediation: the M4V source-index row now says "The source
  boundary names..." instead of "The accepted source boundary names...".
- Narrow QA/Evidence/AgentOps confirmation reviewer
  `019e9e0f-3646-7430-84c2-eead41dab09f` approved the source-index remediation
  3/3 with no P1/P2 findings and was closed. Final post-mutation reviewer
  disposition is 8/8 weighted approval with no P1/P2 blockers for the named
  source-only M4V boundary.
- Gate authority: source-only retained persistence component, host-testable
  record codec, NVS adapter source, focused tests, path-aware audits,
  docs/status records, task log, and handoff. No live NVS execution proof,
  reboot durability, runtime persistence proof, live bridge dispatch, flash,
  serial, release, or publication authority is opened.

## Implementation

- Added `mesh_retained_persistence` as a separate ESP-IDF component.
- Added a pure-C retained checkpoint codec that encodes/decodes a fixed 96-byte
  version-1 record with `mesh_discovery.v1`, last normalized request, last
  status, attempted/staged/rejected counters, and staged byte total.
- Added validation for magic, version, checksum, schema, request safety,
  counter bounds, non-ASCII data, secret-like request names, and status range.
- Added recovery that clears caller-owned M4T persistence intent to empty
  volatile state for missing or invalid records.
- Added NVS adapter source with explicit init/deinit/load/save/erase functions,
  namespace `mesh_retained`, key `m4t_checkpoint`, storage owner
  `mesh_ingress`, and caller-triggered save/erase policy.
- Added a wrong-length NVS blob recovery fix and host regression test so mapped
  NVS blob read errors clear caller-owned M4T persistence intent before
  returning.
- Kept `mesh_discovery_contract` and `app_main` storage-free.

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
- Generated `firmware/projects/espnow-bbs/sdkconfig` build residue was removed.
- Targeted closed-surface scans found only source-only NVS adapter references
  or explicit closed authority statements.
- `git diff --check` passed.
- After P2 remediation, the host tests, selected scaffold unittests, firmware
  audit, record/source/docs/path/data/agent-process/pages/React Native/skills
  audits, no-flash ESP-IDF build, generated-output cleanup, stale acceptance
  wording scan, and `git diff --check` were rerun and passed.
- Final post-review passed 8/8 weighted after the wrong-length NVS and
  source-index wording P2s were resolved.

## Open Evidence

- Live NVS execution, reboot durability, runtime persistence, live recovery,
  first-flash target, live bridge dispatch, bridge frame parsing, bridge ABI,
  serial ABI, timer behavior, route table, root/parent behavior, healing,
  watchdog behavior, stack adequacy, bridge transcript, Win31/OPCON proof,
  rollback, and live proof remain unaccepted.

## Authority Limits

This task does not authorize live NVS execution proof, reboot durability,
firmware persistence acceptance, live bridge dispatch, bridge frame parsing,
firmware runtime acceptance, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE
pairing, Android behavior, router/admin mutation, PCAP, flash, erase, monitor,
serial writes, serial ABI expansion, bridge ABI changes, Gate F service-code
changes, `mesh_discovery.v1` schema changes, Win31 transport changes,
RF/XBee writes, relay actions, load/mains work, live bench, release,
publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
`admin-strict` mutation.

## Handoff

[../handoffs/0154-full-service-mesh-discovery-gate-m4v-retained-persistence-source-contract-to-qa-firmware-communications.md](../handoffs/0154-full-service-mesh-discovery-gate-m4v-retained-persistence-source-contract-to-qa-firmware-communications.md)

## Decision

M4V is accepted source-only for retained persistence adapter code plus
host-testable retained checkpoint handling after final 8/8 weighted
post-review. It is not accepted reboot durability, runtime persistence proof,
live NVS execution proof, live bridge dispatch, flash, serial writes, or live
proof.
