# Task 0235: XBee Transmit Status Host Parser

Status: accepted-host-parser-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`,
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`, and
`SRC-LOCAL-XBEE-HUB-SPOKE-HOST-PLAN-2026-06-05`.

## Routing

- Verified facts: Task 0228 accepted host-only XBee API2 codec/parser vectors and
  a receive-packet direct-frame length guard. The local safe-core API already
  exposed raw API2 encode/decode, AT-response parsing, and receive-packet
  parsing. Digi source row
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05` covers the `0x10`
  Transmit Request to `0x8B` Extended Transmit Status relationship and the
  decoded `0x8B` frame-data shape.
- Assumptions: A host-only typed parser that preserves raw `0x8B` fields improves
  local parser coverage without claiming delivery semantics, hardware identity,
  RF behavior, relay readiness, or deployment readiness.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, Tier B read safety, XCTU/XBee Studio readiness, live RF behavior,
  range, throughput, live bridge dispatch, relay payload readiness, deployment
  readiness, release, publication, commit, push, PR, and deploy remain unproved
  or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Agent Operations.
- Evidence need: current code/source inspection, official Digi source row,
  read-only reviewer quorum, typed host parser source, decoded API2 host vector,
  invalid-input host vectors, source ledger, source-index row, docs-index
  reachability, plan/status/gap updates, focused host test output, firmware and
  scaffold audits, diff hygiene, and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0235-xbee-transmit-status-host-parser.md`,
  `.agents/handoffs/0172-xbee-transmit-status-host-parser-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-transmit-status-host-parser.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0235, source/docs/records/agent-process audits, scoped
  `git diff --check`, closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was waited, captured, and closed before mutation:

- Governance/architecture-risk reviewer
  `019ea018-8dce-7123-8a6f-438d698cc936`, weight 5, conditionally approved the
  host-only boundary and warned against accepting global completion. Required raw
  status-byte preservation and decoded API2 wire coverage.
- Communications/Firmware reviewer
  `019ea018-ab80-73c0-8a5f-648de5d32898`, weight 3, initially held approval
  until implementation existed, with conditions for exact seven-byte decoded
  `0x8B` parsing, wrong-type rejection, short/long rejection, nonzero status
  coverage, and host-only records.
- QA/Evidence reviewer `019ea018-c3b3-77c3-8570-c6af875bdc32`, weight 3,
  conditionally approved the typed parser/test/record boundary with no P1
  blocker.
- Hardware Safety reviewer `019ea018-e139-7ef1-9d31-87964fe221ac`, weight 3,
  initially held approval until implementation existed and required closed live
  hardware, serial, RF, transmit, bridge, relay, wiring, flash, release, and git
  surfaces.

Clarification reviewers were then waited, captured, and closed:

- Communications/Firmware clarification reviewer
  `019ea01b-d6ac-7f00-99da-9346d43318d7`, weight 3, approved the pre-mutation
  boundary with no P1/P2 blocker excluding the expected missing implementation.
- Hardware Safety clarification reviewer
  `019ea01b-f86b-7011-8b38-6d810feaab20`, weight 3, approved the pre-mutation
  boundary with no P1/P2 blocker excluding the expected missing implementation.

Weighted pre-mutation gate result: 14/14 approval for the named host-only parser
boundary after clarification. No mutating worker agents were spawned. Initial and
clarification reviewer agents were closed after output capture. The visible hook
surface later emitted advisory reminders under `bypassPermissions`; these were
recorded as advisory and did not expand the mutation boundary.

## Implementation

- Added `fr_xbee_transmit_status_t` for raw decoded `0x8B` host fields:
  `frame_id`, `reserved16`, `retry_count`, `delivery_status`, and
  `discovery_status`.
- Added `fr_xbee_parse_transmit_status`, which rejects null inputs, non-`0x8B`
  frame types, and decoded frame data whose length is not exactly seven bytes.
- Added host vectors that parse a decoded API2 `0x8B` frame with nonzero raw
  status bytes and reject receive-packet type, short frame data, long frame data,
  null frame, and null output cases.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 235`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0235 touched files passed with no output.
- Closed-surface review over the Task 0235 touched files found only explicit
  authority-limit or closed-surface language for live XBee, serial, RF, relay,
  flash, release, publication, and git surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize serial opens, Tier B AT reads, XCTU or
XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`, API transmit to
hardware, RF/range/throughput tests, live bridge dispatch, relay payloads,
relay/load/mains work, wiring, flashing, monitor, framework selection, release,
publication, commit, push, PR, or deploy.

## Handoff

Handoff:
[../handoffs/0172-xbee-transmit-status-host-parser-to-qa-communications.md](../handoffs/0172-xbee-transmit-status-host-parser-to-qa-communications.md)

## Decision

Decision: accept a host-only typed parser for decoded Digi XBee Extended
Transmit Status `0x8B`. Live XBee identity, serial, RF, bridge, relay, and
deployment gates remain separate future work.
