# Task 0241: XBee Aggregate Addressing Update Host Parser

Status: accepted-host-aggregate-addressing-update-parser-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-AGGREGATE-ADDRESSING-UPDATE-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-AGGREGATE-ADDRESSING-UPDATE-0X8E-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: existing safe-core code provides host-only decoded Digi XBee
  API2 frame handling and typed host parsers for received/status frames.
  Official Digi 900HP API documentation lists received Aggregate Addressing
  Update `0x8E`. Digi's `0x8E` reference defines the decoded frame data as
  frame type, one reserved byte, new 64-bit destination address, and old
  64-bit destination address. Digi's 900HP `AG` command page says
  API-enabled devices with updated `DH`/`DL` emit `0x8E`.
- Assumptions: A host-only parser over already-decoded `0x8E` frame data
  improves diagnostics without issuing `AG`, updating `DH`/`DL`, generating
  transmit frames, opening serial, touching RF, changing bridge dispatch, or
  asserting live radio health.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, whether local radios can emit `0x8E`, DigiMesh support for the
  local configuration, Tier B read safety, XCTU/XBee Studio readiness, live RF
  behavior, range, throughput, API transmit-to-hardware behavior, live bridge
  dispatch, relay payload readiness, deployment readiness, release,
  publication, commit, push, PR, and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Architecture Risk.
- Evidence need: official Digi `0x8E` source coverage, 900HP received-frame ID
  support and `AG` emission context, read-only reviewer quorum, pure host
  parser, host tests for the official example, nulls, wrong frame type, short
  and long frames, raw reserved-byte preservation, new/old `DH`/`DL` address
  byte ordering, source ledger, source-index row, docs-index reachability,
  plan/status/gap updates, focused host test output, firmware and scaffold
  audits, diff hygiene, and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0241-xbee-aggregate-addressing-update-host-parser.md`,
  `.agents/handoffs/0178-xbee-aggregate-addressing-update-host-parser-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-aggregate-addressing-update-host-parser.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0241, source/docs/records/agent-process audits, scoped
  `git diff --check`, closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. Lifecycle state inventory was not visible because no list-agent tool
was available; all spawned reviewer agents were closed after their outputs
were recorded.

- Governance/architecture-risk reviewer
  `019ea08e-2057-7832-918f-4935b38f4bc0`, weight 5, approved the named
  host-only parser boundary with no P1/P2 blockers, conditioned on exact
  decoded length 18 and offsets `0`, `1`, `2..9`, and `10..17`.
- Communications/Firmware reviewer
  `019ea08e-4c27-7730-8c03-36c3a50fdb7d`, weight 3, approved with no P1/P2
  blockers and required preserving the reserved byte without assigning
  semantics.
- QA/Evidence and Hardware Safety reviewer
  `019ea08e-732a-7040-90be-695d67b65722`, weight 3, approved with no P1/P2
  blockers and required exact source coverage, durable records, focused tests,
  and closed live surfaces. This reviewer also required that new/old 64-bit
  fields be described as `DH`/`DL` destination addresses, not radio identity
  or `SH`/`SL`.

Weighted pre-mutation gate result: 11/11 approval for the named host-only
`0x8E` aggregate-addressing update parser boundary. No mutating worker agents
were spawned.

## Implementation

- Added `fr_xbee_aggregate_addressing_update_t`.
- Added `fr_xbee_parse_aggregate_addressing_update`.
- The parser accepts only already-decoded frame data with exactly 18 bytes and
  frame type `0x8E`.
- The parser preserves the raw reserved byte.
- The parser copies the new 64-bit `DH`/`DL` destination address from decoded
  frame data bytes `2..9`.
- The parser copies the old 64-bit `DH`/`DL` destination address from decoded
  frame data bytes `10..17`.
- The parser rejects null pointers, wrong frame type, short frame data, and
  long frame data.
- Added host tests for the official decoded example body, raw reserved-byte
  preservation, exact address byte ordering, null inputs, wrong frame type,
  short frame data, and long frame data.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 241`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- Source, docs, records, and agent-process scaffold audits passed.
- Scoped `git diff --check` passed for the Task 0241 mutation boundary.
- Closed-surface review passed: implementation changes add only host parser/test
  coverage, and live/API-transmit/serial/RF/relay/flash/release matches are
  authority-limit text in records.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize issuing `AG`, updating `DH` or `DL`,
`0x10` builder work, Transmit Request generation, API transmit-frame
generation, serial opens, Tier B AT reads, XCTU or XBee Studio discovery, XBee
setting writes, `WR`, `AC`, `KY`, API transmit to hardware,
RF/range/throughput tests, live DigiMesh behavior claims, live bridge dispatch,
relay payloads, relay/load/mains work, wiring, flashing, monitor, framework
selection, release, publication, commit, push, PR, or deploy.

## Handoff

Handoff:
[../handoffs/0178-xbee-aggregate-addressing-update-host-parser-to-qa-communications.md](../handoffs/0178-xbee-aggregate-addressing-update-host-parser-to-qa-communications.md)

## Decision

Decision: accept a host-only parser for already-decoded received Digi XBee
Aggregate Addressing Update `0x8E` frame data. Live XBee identity, serial, RF,
bridge, relay, and deployment gates remain separate future work.
