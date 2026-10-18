# Task 0243: XBee Remote AT Response Host Parser

Status: accepted-host-remote-at-response-parser-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-REMOTE-AT-RESPONSE-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-REMOTE-AT-RESPONSE-0X97-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: existing safe-core code provides host-only decoded Digi XBee
  API2 frame handling and typed host parsers for received/status frames.
  Official Digi 900HP/XSC documentation lists Remote AT Command Response
  `0x97`, emitted for Remote AT Command Request `0x17`, and defines the
  response fields as frame type, frame ID, 64-bit source address, 16-bit
  reserved field, AT command, command status, and optional command data.
- Assumptions: A host-only parser for already-decoded Digi XBee Remote AT
  Command Response `0x97` frame data improves diagnostics without generating
  `0x17` requests, opening serial, touching RF, writing radio settings,
  changing bridge dispatch, or asserting live radio health.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, whether local radios can emit `0x97`, Tier B read safety,
  XCTU/XBee Studio readiness, live RF behavior, range, throughput, API
  transmit-to-hardware behavior, live bridge dispatch, relay payload readiness,
  deployment readiness, release, publication, commit, push, PR, and deploy
  remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with Architecture Risk and
  QA/Evidence.
- Evidence need: official Digi `0x97` source coverage, read-only reviewer
  quorum, pure host parser, host tests for no-data and command-data responses,
  nulls, wrong frame type, short frames, over-max frames, max command data,
  over-value rejection, big-endian `reserved16`, source64 byte-order
  preservation, remote-specific status labels, source ledger, source-index row,
  docs-index reachability, plan/status/gap updates, focused host test output,
  firmware and scaffold audits, diff hygiene, and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0243-xbee-remote-at-response-host-parser.md`,
  `.agents/handoffs/0180-xbee-remote-at-response-host-parser-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-remote-at-response-host-parser.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0243, source/docs/records/agent-process audits, scoped
  `git diff --check`, pending-marker scan, closed-surface review, and full
  scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. Lifecycle state inventory was not visible because no list-agent tool
was available; all visible spawned reviewer agents were closed after their
outputs were recorded.

- Architecture/governance/QA reviewer
  `019ea0ca-4437-7100-8b9e-18da38d9f6b5`, weight 5, approved the named
  host-only parser boundary with no P1/P2 blockers if dedicated `0x97` Digi
  source-index and source-ledger coverage is added before acceptance.
- Communications/Firmware reviewer
  `019ea0ca-6519-72c0-9791-668915ab1f4e`, weight 3, approved with no P1
  blockers and required resolving P2 conditions: explicit `0x97` source
  coverage, a remote-specific status label helper for `0x04` and `0x0c`, and
  preservation of the 900HP/XSC 16-bit field as raw `reserved16`.

Weighted pre-mutation gate result: 8/8 approval for the named host-only `0x97`
remote AT response parser boundary once the P2 source/label/reserved-field
conditions are satisfied. No mutating worker agents were spawned.

## Implementation

- Added `fr_xbee_remote_at_response_t`.
- Added `fr_xbee_parse_remote_at_response`.
- Added `fr_xbee_remote_at_command_status_label` so remote `0x97` statuses do
  not reuse the local `0x88` helper.
- The parser accepts only already-decoded frame data with frame type `0x97`,
  at least 15 bytes, no more than `FR_XBEE_MAX_FRAME_DATA` bytes, and optional
  command data no longer than `FR_XBEE_AT_VALUE_MAX`.
- The parser preserves frame ID, source64 byte order, the 900HP/XSC
  big-endian `reserved16` field, two-byte AT command, raw remote command
  status, and optional command data bytes.
- Host tests cover no-data and query-data examples, `TP` command data bytes
  `00 2f`, null inputs, wrong frame type, short frames, max command data,
  over-value rejection, over-max host frame length, non-`0xfffe` raw
  `reserved16`, source64 byte order, and remote status labels including
  `0x04` transmission failure and `0x0c` encryption error.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 243`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- Scoped `git diff --check` over the Task 0243 code and record boundary
  passed.
- Pre-acceptance pending-marker scan found only the expected Task 0243
  acceptance-status fields.
- Closed-surface review found only authority-limit/open-evidence wording and
  no authorized serial, RF, flash, relay, release, commit, push, PR, or deploy
  action.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize remote setting reads, Remote AT Command
Request `0x17` generation, API transmit-frame generation, serial opens, Tier B
AT reads, XCTU or XBee Studio discovery, XBee setting writes, `WR`, `AC`,
`KY`, API transmit to hardware, RF/range/throughput tests, live `0x97`
behavior claims, live bridge dispatch, relay payloads, relay/load/mains work,
wiring, flashing, monitor, framework selection, release, publication, commit,
push, PR, or deploy.

## Handoff

Handoff:
[../handoffs/0180-xbee-remote-at-response-host-parser-to-qa-communications.md](../handoffs/0180-xbee-remote-at-response-host-parser-to-qa-communications.md)

## Decision

Decision: accept a host-only parser for already-decoded Digi XBee Remote AT
Command Response `0x97` frame data. Live XBee identity, remote setting reads,
`0x17` request generation, serial, RF, bridge, relay, and deployment gates
remain separate future work.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter
  delta.
- Current local radio settings and whether local radios can emit `0x97`.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Live RF behavior, range, throughput, bridge-dispatch, relay-payload, and
  deployment proof.
