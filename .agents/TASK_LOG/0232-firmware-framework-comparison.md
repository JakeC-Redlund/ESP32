# Task 0232: Firmware Framework Comparison

Status: accepted

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-FIRMWARE-FRAMEWORK-COMPARISON-2026-06-07`,
`SRC-ARDUINO-ESP32-CORE-2026-06-07`,
`SRC-PLATFORMIO-ESPRESSIF32-2026-06-07`,
`SRC-ESP-IDF-STABLE-ESP32`, `SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-START-PROJECT`,
`SRC-LOCAL-FIRMWARE-FRAMEWORK-CI-MATRIX-2026-06-07`, and
`SRC-LOCAL-DEVELOPMENT-PLAN-CONSOLIDATION-2026-05-27`.

## Routing

- Verified facts: `ADR-0001` remains proposed and keeps the workspace
  framework-neutral outside accepted project ADRs. `ADR-0002` accepts ESP-IDF
  stable v6.0.1 only for `four-relay-xbee-wifi`, and `ADR-0003` accepts
  ESP-IDF stable v6.0.1 only for `espnow-bbs`. Task 0231 closes only
  accepted-project firmware CI for those ADR targets. The current source index
  had ESP-IDF rows but no Arduino-ESP32 or PlatformIO comparison rows before
  this task.
- Assumptions: A docs/source-record-only comparison packet can narrow the
  framework-planning gap without selecting any framework, adding framework
  files, expanding CI, or changing firmware source.
- Unknowns/open evidence: exact future project requirements, hardware
  identities, SoC variants, carrier boards, boot-pin risks, voltage/isolation
  facts, Arduino library needs, PlatformIO environment needs, remote workflow
  behavior, and all live/runtime behavior remain unresolved.
- Selected tier: Tier 2 docs, architecture, source-record, and status update.
- Owner role: Architecture and Firmware, with QA/Records and Agent Operations.
- Evidence need: required reading refresh, accepted ADR inspection, current
  official source refresh for ESP-IDF, Arduino-ESP32, and PlatformIO,
  read-only reviewer quorum, source-index coverage, comparison criteria,
  status/gap/docs records, durable task/handoff/source records, scaffold
  audits, and diff hygiene.
- Mutation boundary:
  `docs/architecture/firmware-framework-comparison.md`,
  `knowledge-base/toolchain/framework-neutral-status.md`,
  `.agents/TASK_LOG/0232-firmware-framework-comparison.md`,
  `.agents/handoffs/0169-firmware-framework-comparison-to-architecture-firmware-qa.md`,
  `knowledge-base/source-ledger/2026-06-07-firmware-framework-comparison.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: source/docs/records/firmware/agent-process/path scaffold
  audits, durable-record audit for Task 0232, full scaffold verification, and
  scoped `git diff --check`.

## Reviewer Quorum

Read-only reviewer quorum was completed and closed before mutation:

- Architecture/Firmware governance reviewer
  `019e9fe4-fff5-72b0-8f45-d4014e7aaec6`, weight 5, approved a Tier 2
  docs/source-record-only scope with no P1 blockers. Conditions: add official
  Arduino-ESP32 and PlatformIO source coverage before comparison claims, avoid
  framework selection, avoid firmware/workflow mutation, and preserve dirty
  tree boundaries.
- QA/Records reviewer `019e9fe5-13d7-7270-b553-6e389ab241d2`, weight 3,
  approved with no P1 blockers. Conditions: add task/source/handoff/docs-index
  and status records, add official Arduino/PlatformIO source rows, update stale
  framework-neutral status text, keep closed surfaces closed, and run records,
  source, docs, firmware, agent-process, full scaffold, and diff validation.

Weighted reviewer result before mutation: 8/8 conditional approval. Conditions
were addressed by limiting the task to comparison records, adding official
source rows, avoiding firmware/workflow files, and closing both reviewer agents
after their outputs were captured. No mutating worker agents were spawned.

## Implementation

- Added a framework comparison note with project-ADR criteria and stop gates.
- Added official source-index rows for Arduino-ESP32 and PlatformIO framework
  comparison evidence.
- Added a local source ledger, task record, and continuation handoff.
- Updated framework-neutral status to include both accepted project-local
  exceptions and the comparison packet.
- Updated docs index, development plan, development status ledger, triage
  status, and known gaps to record that comparison evidence exists while
  framework selection remains open outside accepted project ADRs.

## Validation

Passed host-only validation on 2026-06-07:

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  paths`: `PASS: ESP32 scaffold audit paths succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  sources`: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  docs`: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py
  --min-task-id 232`: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  firmware`: `PASS: ESP32 scaffold audit firmware succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  records`: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  agent_process`: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3
  scripts/verify_scaffold.py`: `PASS: ESP32 scaffold validation succeeded`.
- Scoped `git diff --check` over the Task 0232 touched tracked files passed
  with no output.

## Authority Limits

This task does not accept or authorize Arduino, ESP-IDF, PlatformIO, or any
other framework selection for projects outside accepted ADRs. It does not
authorize firmware source, framework project files, CMake/sdkconfig/
platformio/Arduino project files, CI expansion for non-ADR projects, workflow
dispatch, artifact upload, deploy, release, publication, commit, push, PR,
firmware runtime, live hardware, flash, monitor, serial/RF/XBee writes,
relay/load/mains work, `/etc/codex/requirements.toml`, or `admin-strict`
installation.

## Handoff

Handoff:
[../handoffs/0169-firmware-framework-comparison-to-architecture-firmware-qa.md](../handoffs/0169-firmware-framework-comparison-to-architecture-firmware-qa.md)

## Decision

Decision: accepted for the docs/source-record-only firmware framework
comparison packet. Framework selection remains open outside `ADR-0002` and
`ADR-0003` until a future project-specific ADR is accepted.
