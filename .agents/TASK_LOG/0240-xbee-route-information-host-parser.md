# Task 0240: XBee Route Information Host Parser

Status: accepted-host-route-information-parser-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-ROUTE-INFORMATION-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-ROUTE-INFORMATION-0X8D-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: existing safe-core code provides host-only decoded Digi XBee
  API2 frame handling and typed host parsers for received/status frames.
  Official Digi documentation defines Route Information `0x8D` as a received
  DigiMesh routing-information frame emitted after a separately authorized
  DigiMesh unicast transmission with Trace Routing or NACK enabled. The source
  lists source-event values `0x11` NACK and `0x12` Trace route, a data-length
  byte for following bytes, timestamp in microseconds, ACK timeout count, TX
  blocked count, one 8-bit reserved field, and four 64-bit addresses.
- Assumptions: A host-only parser over already-decoded `0x8D` frame data
  improves diagnostics without generating requests, opening serial, touching
  RF, changing bridge dispatch, or asserting live radio health.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, Tier B read safety, XCTU/XBee Studio readiness, live `0x8D`
  availability, live RF behavior, range, throughput, API transmit-to-hardware
  behavior, live bridge dispatch, relay payload readiness, deployment
  readiness, release, publication, commit, push, PR, and deploy remain
  unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Architecture Risk.
- Evidence need: official Digi `0x8D` source coverage, read-only reviewer
  quorum, one-byte reserved-field reconciliation against the official example,
  pure host parser/helper, host tests for the official example, nulls, wrong
  frame type, short frames, data-length mismatches, future appended fields,
  source-event labels, source ledger, source-index row, docs-index
  reachability, plan/status/gap updates, focused host test output, firmware
  and scaffold audits, diff hygiene, and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0240-xbee-route-information-host-parser.md`,
  `.agents/handoffs/0177-xbee-route-information-host-parser-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-route-information-host-parser.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0240, source/docs/records/agent-process audits, scoped
  `git diff --check`, closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. Lifecycle state inventory was not visible because no list-agent tool
was available; all spawned reviewer agents were closed after their outputs
were recorded.

- Governance/architecture-risk reviewer
  `019ea07b-1623-7aa3-bd07-0a1b7cdd0266`, weight 5, initially approved the
  named boundary while raising a reserved-field width concern. A focused
  read-only reconciliation against the official Digi page and example then
  approved the one-byte raw reserved field with no P1/P2 blockers.
- Communications/Firmware reviewer
  `019ea07b-36e9-7c12-bc8f-2eefed5230fa`, weight 3, approved with no P1/P2
  blockers and noted that the official example and `data_length = 0x27`
  support decoded destination/source/responder/receiver indices `10..17`,
  `18..25`, `26..33`, and `34..41`.
- QA/Evidence and Hardware Safety reviewer
  `019ea07b-5745-7591-bcfe-c535851cc297`, weight 3, approved with no P1/P2
  blockers and required exact source-index coverage, durable records, focused
  tests, scaffold audits, and explicit closed live surfaces.

Weighted pre-mutation gate result: 11/11 approval for the named host-only
`0x8D` route-information parser/label-helper boundary. No mutating worker
agents were spawned.

## Source Reconciliation

- Digi lists the `0x8D` reserved field as 8-bit and the example uses
  `data_length = 0x27`.
- The source-listed base body after the data-length byte is timestamp 4 bytes,
  ACK timeout 1 byte, TX blocked 1 byte, reserved 1 byte, and four 64-bit
  addresses for 32 bytes, totaling 39 bytes.
- A two-byte reserved field would total 40 bytes and would not match the
  official example body length.
- Task 0240 therefore preserves one raw `reserved` byte and parses the four
  addresses at decoded frame-data indices `10..17`, `18..25`, `26..33`, and
  `34..41`.

## Implementation

- Added `fr_xbee_route_information_t`.
- Added `fr_xbee_parse_route_information`.
- Added `fr_xbee_route_information_source_event_label`.
- The parser accepts only already-decoded frame data with frame type `0x8D`,
  at least 42 decoded bytes, `data_length == frame->data_len - 3`, and
  `data_length >= 39`.
- The parser preserves raw `source_event`, `data_length`, `timestamp_us`,
  `ack_timeout_count`, `tx_blocked_count`, `reserved`, four 64-bit addresses,
  and `trailing_len`.
- The parser accepts future appended source fields only by increasing
  `trailing_len`; it does not assign meanings to trailing bytes.
- The label helper maps only `0x11` to `nack` and `0x12` to `trace_route`;
  unlisted source events return `unknown_route_information_source_event`.
- Added host tests for the official decoded example shape, timestamp, counts,
  one reserved byte, all four address arrays, null inputs, wrong frame type,
  below-minimum body, data-length mismatch, long mismatched frame,
  source-consistent future appended fields, and source-event labels.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 240`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0240 touched files passed with no output.
- Focused code closed-surface scan over the safe-core header/source/test files
  found no new serial, RF, bridge, relay, flash, monitor, release, or git
  behavior in the new `0x8D` parser/helper path.
- Closed-surface review over the new Task 0240 record, handoff, and source
  ledger found only explicit unknown/open-evidence or authority-limit language
  for live XBee, serial, RF, relay, flash, release, publication, and git
  surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize `0x10` builder work, Trace Routing
request generation, NACK request generation, Transmit Request generation, API
transmit-frame generation, serial opens, Tier B AT reads, XCTU or XBee Studio
discovery, XBee setting writes, `WR`, `AC`, `KY`, API transmit to hardware,
RF/range/throughput tests, live bridge dispatch, relay payloads,
relay/load/mains work, wiring, flashing, monitor, framework selection,
release, publication, commit, push, PR, or deploy.

## Handoff

Handoff:
[../handoffs/0177-xbee-route-information-host-parser-to-qa-communications.md](../handoffs/0177-xbee-route-information-host-parser-to-qa-communications.md)

## Decision

Decision: accept a host-only parser and label helper for already-decoded
received Digi XBee Route Information `0x8D` frame data. Live XBee identity,
serial, RF, bridge, relay, and deployment gates remain separate future work.
