# Task 0204: Full-Service Mesh Discovery Gate M4S Bridge-Frame Intent Source Contract

Status: accepted-bridge-request-text-intent-source-contract-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
`SRC-ESP-IDF-STABLE-ESP32`,
`SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`

## Routing

- Verified facts: required governance/docs were reread. M4R is accepted as a
  pure-C caller-owned local dispatch-envelope/accounting source contract over
  M4Q. The accepted discovery request names remain `discovery_snapshot`,
  `discovery_events`, `service_catalog`, and `capability_report`.
- Assumptions: a caller-owned request-text intent normalizer over M4R can
  advance bridge-adjacent source readiness without defining transport framing,
  serial ABI, bridge ABI, runtime behavior, persistence, or live dispatch.
- Unknowns: Win31/OPCON bridge transcript shape, serial line framing, bridge
  ABI, bridge frame parser, live bridge dispatch, persistence, timer behavior,
  firmware runtime acceptance, first-flash proof, and M5 live mesh remain
  unaccepted.
- Selected tier: Tier 2 firmware/source/docs/status work.
- Owner role: Communications/Firmware with Architecture, Agent Operations, and
  QA.
- Evidence need: accepted M4R/M4Q/M4P/M4O/M4K/ESP-IDF records, read-only
  reviewer quorum, host C tests, focused and broader audit validation,
  no-flash build, cleanup, closed-surface scan, task/handoff/source records,
  and final post-review acceptance.
- Mutation boundary:
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/include/espnow_bbs_mesh_discovery_contract.h`,
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/src/espnow_bbs_mesh_discovery_contract.c`,
  `tests/espnow_bbs_mesh_discovery_contract/test_mesh_discovery_contract.c`,
  `scripts/scaffold_audit_firmware.py`,
  `scripts/scaffold_audit_data.py`,
  `docs/architecture/espnow-bbs-mesh-m4s-bridge-frame-intent-source-contract.md`,
  `knowledge-base/source-ledger/2026-06-06-full-service-mesh-discovery-gate-m4s-bridge-frame-intent-source-contract.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `docs/projects/espnow-bbs/README.md`,
  `docs/projects/espnow-bbs/full-service-mesh-discovery.md`,
  `firmware/projects/espnow-bbs/README.md`,
  `research/development-plan.md`,
  `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/known-gaps.md`, this task record, and
  `.agents/handoffs/0151-full-service-mesh-discovery-gate-m4s-bridge-frame-intent-source-contract-to-qa-firmware-communications.md`.
- Reviewer quorum: lifecycle listing was not visible through available tools
  before spawning. Two read-only reviewers were spawned, waited, and closed
  after output capture before mutation. Weighted pre-mutation disposition was
  8/8 conditional approval with no P1/P2 blockers for a caller-provided ASCII
  request-text intent boundary only. Final read-only reviewers were spawned,
  waited, captured, and closed after validation. They found no source-code
  P1/P2 blocker; their only P2 was stale validation-in-progress record wording,
  resolved by this acceptance update.
  Post-remediation follow-up reviewers `019e9da0-7d7e-7060-97f3-c1aa7264f3ec`
  (Architecture/Communications/Firmware, weight 5) and
  `019e9da0-9938-7172-9d50-2bd7403655f9` (QA/Evidence/AgentOps, weight 3)
  were spawned read-only, waited, captured, and closed. They approved 8/8 with
  no P1/P2 blockers and confirmed remaining validation-in-progress hits are
  historical resolved-P2 wording only.
- Gate authority: pure-C caller-owned local request-text intent/accounting over
  accepted M4R/M4Q, focused tests, path-aware audits, docs/status records,
  task log, and handoff. No live proof, flash, serial, release, or publication
  authority is opened.

## Implementation

- Added `espnow_bbs_mesh_contract_bridge_request_text_intent_t` as
  caller-owned local request-text intent/accounting state.
- Added init/reset, route, and summary APIs for the request-text intent layer.
- The route normalizes only leading/trailing ASCII space, tab, CR, and LF
  around caller-provided request text, rejects blank text, embedded whitespace,
  JSON/key-value/colon or length-prefixed text, Gate F request names,
  live-action names, unknown names, non-ASCII text, overlong text, and nulls.
- Accepted normalized request names call M4R
  `espnow_bbs_mesh_contract_bridge_dispatch_envelope_route`.
- Malformed request text is rejected before envelope/router accounting, and
  caller-owned queue, owner, and runtime scheduler state are not mutated.
- Summary output includes `intent=request_text`, local counters, last
  normalized request/status, `live=blocked`, `serial=closed`, and `abi=closed`.

## Validation

Completed validation:

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_build_only_target tests.scaffold_audits.test_espnow_bbs_mesh_startup_lifecycle tests.scaffold_audits.test_espnow_bbs_mesh_runtime_task_shell tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 204
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4s-bridge-frame-intent-source-contract build
find firmware/projects/espnow-bbs -maxdepth 1 -name 'sdkconfig*' -print
test ! -e build/tests/espnow_bbs_mesh_discovery_contract
rg -n "bridge dispatch|serial_write|uart_|nvs_|esp_timer|xTimer|xQueue|esp_wifi|esp_now|esp_mesh|esp_ble|esp_bt|httpd_|esp_partition|esp_flash|esp_ota|fopen|fwrite|printf|ESP_LOG" firmware/projects/espnow-bbs/components/mesh_discovery_contract firmware/projects/espnow-bbs/main/main.c
git diff --check
```

Results:

- Host contract tests passed.
- Firmware audit passed.
- Focused mesh discovery scaffold unittest passed: 2 tests.
- Broader selected scaffold unittest suite passed: 15 tests.
- Record, source, docs, agent-process, pages, React Native, skills, and path
  audits passed.
- No-flash ESP-IDF v6.0.1 build passed and generated
  `espnow_bbs_build_only.bin` size `0x22d30`; the 1 MiB app partition had
  `0xdd2d0` bytes free.
- Generated `sdkconfig*` files were removed from
  `firmware/projects/espnow-bbs/`.
- `build/tests/espnow_bbs_mesh_discovery_contract` was absent after host-test
  cleanup.
- Closed-surface scan matched only the existing denied-token data string
  `"serial_write"` in
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/src/espnow_bbs_mesh_discovery_contract.c`.
- `git diff --check` passed.
- Final read-only reviewers found no source-code P1/P2 blocker. Their stale
  validation-record P2 is resolved by this acceptance update.
- Post-remediation follow-up reviewers approved 8/8 with no P1/P2 blockers
  after the M4S durable records were aligned.

## Open Evidence

- Live bridge dispatch, bridge frame parsing, bridge ABI, serial ABI, firmware
  persistence, timer behavior, route table, root/parent behavior, healing,
  watchdog behavior, stack adequacy, first-flash behavior, bridge transcript,
  Win31/OPCON proof, rollback, cleanup, and live proof remain unaccepted.

## Authority Limits

This task does not authorize live bridge dispatch, bridge frame parsing,
firmware persistence, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing,
Android app behavior, router/admin mutation, PCAP, flash, erase, monitor,
serial writes, serial ABI expansion, bridge ABI changes, Gate F service-code
changes, `mesh_discovery.v1` schema changes, Win31 transport changes,
RF/XBee writes, relay actions, load/mains work, live bench, release,
publication, commit, push, PR, deploy, `/etc/codex/requirements.toml`, or
`admin-strict` mutation.

## Handoff

[../handoffs/0151-full-service-mesh-discovery-gate-m4s-bridge-frame-intent-source-contract-to-qa-firmware-communications.md](../handoffs/0151-full-service-mesh-discovery-gate-m4s-bridge-frame-intent-source-contract-to-qa-firmware-communications.md)

## Decision

M4S is accepted source-only for the pure-C caller-owned request-text
intent/accounting contract over accepted M4R/M4Q. The final reviewer P2 was
stale validation-in-progress record wording and is resolved by this acceptance
update; post-remediation follow-up reviewers approved 8/8 with no P1/P2
blockers. Live bridge dispatch, arbitrary bridge frame parsing, serial ABI
expansion, bridge ABI changes, Gate F request/service changes,
`mesh_discovery.v1` schema changes, persistence, timer creation, firmware
runtime acceptance, live task execution proof, flash, serial writes, and live
proof remain separate gates.
