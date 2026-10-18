# Task 0237: XBee AT Command Status Label Helper

Status: accepted-host-at-status-label-helper-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-AT-COMMAND-STATUS-LABEL-HELPER-2026-06-07`,
`SRC-DIGI-XBEE-LOCAL-AT-RESPONSE-0X88-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-05-29`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: existing safe-core code parses Digi XBee Local AT Command
  Response `0x88` frames and preserves the raw `command_status` byte. The
  source index has current 900HP/XSC user-guide coverage and official Digi
  local AT response `0x88` coverage, and official Digi docs list current local
  `0x88` command status values `0x00` through `0x03`.
- Assumptions: Source-backed labels over the raw `command_status` byte improve
  host diagnostics without changing parser behavior, issuing an AT request,
  opening serial, generating an API transmit frame, or claiming live radio
  health.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, Tier B read safety, XCTU/XBee Studio readiness, live RF behavior,
  range, throughput, API transmit-to-hardware behavior, live bridge dispatch,
  relay payload readiness, deployment readiness, release, publication, commit,
  push, PR, and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Architecture Risk.
- Evidence need: official Digi source coverage, read-only reviewer quorum,
  pure host label helper, host tests for source-listed and unknown values,
  source ledger, source-index row, docs-index reachability, plan/status/gap
  updates, focused host test output, firmware and scaffold audits, diff hygiene,
  and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0237-xbee-at-command-status-label-helper.md`,
  `.agents/handoffs/0174-xbee-at-command-status-label-helper-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-at-command-status-label-helper.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0237, source/docs/records/agent-process audits, scoped
  `git diff --check`, closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. Lifecycle state inventory was not visible because no list-agent tool
was available; all spawned reviewer agents were closed after their outputs
were recorded.

- Governance/architecture-risk reviewer
  `019ea046-79b8-7792-8ae0-adc2408c120e`, weight 5, approved the host-only
  helper boundary with no P1/P2 blockers.
- Communications/Firmware reviewer
  `019ea046-940c-76e1-a7bf-2926973d8651`, weight 3, approved with exact test
  conditions for source-listed local `0x88` status labels and unknown fallbacks
  for unlisted bytes such as `0x04`, `0x0c`, and `0xff`.
- QA/Evidence and Hardware Safety reviewer
  `019ea046-abc0-7101-b3ca-269a8f80aedf`, weight 3, approved with no P1/P2
  blockers and required host tests, durable records, validation audits, and
  explicit closed live surfaces.

Weighted pre-mutation gate result: 11/11 approval for the named host-only
label helper boundary. No mutating worker agents were spawned.

## Implementation

- Added `fr_xbee_at_command_status_label`.
- Preserved `fr_xbee_parse_at_response` raw parser behavior.
- Returned source-backed static labels only for Digi-listed local `0x88`
  command status values: `0x00`, `0x01`, `0x02`, and `0x03`.
- Returned `unknown_at_command_status` for unlisted bytes including `0x04`,
  `0x0c`, and `0xff`.
- Added host tests for all listed labels and required unknown fallback values.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 237`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0237 touched files passed with no output.
- A broad added-line closed-surface scan over touched dirty files included
  pre-existing dirty-tree rows from earlier uncommitted work; focused code scan
  over the safe-core header/source/test files found only unknown-status test
  labels and no serial, transmit, RF, bridge, relay, flash, release, or git
  behavior in the new helper path.
- Closed-surface review over the new Task 0237 record, handoff, and source
  ledger found only explicit unknown/open-evidence or authority-limit language
  for live XBee, serial, RF, relay, flash, release, publication, and git
  surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize `0x10` builder work, Transmit Request
generation, API transmit-frame generation, serial opens, Tier B AT reads, XCTU
or XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`, API transmit
to hardware, RF/range/throughput tests, live bridge dispatch, relay payloads,
relay/load/mains work, wiring, flashing, monitor, framework selection, release,
publication, commit, push, PR, or deploy.

## Handoff

Handoff:
[../handoffs/0174-xbee-at-command-status-label-helper-to-qa-communications.md](../handoffs/0174-xbee-at-command-status-label-helper-to-qa-communications.md)

## Decision

Decision: accept a host-only label helper for parsed Digi XBee Local AT Command
Response `0x88` raw command-status bytes. Live XBee identity, serial, RF,
bridge, relay, and deployment gates remain separate future work.
