# Task 0242: XBee Explicit Receive Host Parser

Status: accepted-host-explicit-receive-parser-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-EXPLICIT-RECEIVE-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-EXPLICIT-RECEIVE-0X91-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AO`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: existing safe-core code provides host-only decoded Digi XBee
  API2 frame handling and typed host parsers for received/status frames.
  Official Digi 900HP/XSC documentation lists received Explicit Receive
  Indicator `0x91`, maps `AO=1` to explicit receive output, and defines the
  `0x91` frame fields as frame type, 64-bit source address, 16-bit reserved
  field, source endpoint, destination endpoint, cluster ID, profile ID,
  receive options, and received data.
- Assumptions: A host-only parser over already-decoded `0x91` frame data
  improves diagnostics without changing `AO`, generating `0x10` or `0x11`
  transmit frames, opening serial, touching RF, changing bridge dispatch, or
  asserting live radio health.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, current `AO` value, whether local radios can emit `0x91`, Tier B
  read safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput,
  API transmit-to-hardware behavior, live bridge dispatch, relay payload
  readiness, deployment readiness, release, publication, commit, push, PR, and
  deploy remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Architecture Risk.
- Evidence need: official Digi `0x91` source coverage, 900HP received-frame ID
  support and `AO=1` context, read-only reviewer quorum, pure host parser,
  host tests for the official example, nulls, wrong frame type, short frames,
  local header-only rejection policy, over-max frames, max payload, big-endian
  `reserved16`, `cluster_id`, and `profile_id`, raw receive-options
  preservation, payload byte ordering, source ledger, source-index row,
  docs-index reachability, plan/status/gap updates, focused host test output,
  firmware and scaffold audits, diff hygiene, and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0242-xbee-explicit-receive-host-parser.md`,
  `.agents/handoffs/0179-xbee-explicit-receive-host-parser-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-explicit-receive-host-parser.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0242, source/docs/records/agent-process audits, scoped
  `git diff --check`, pending-marker scan, closed-surface review, and full
  scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. Lifecycle state inventory was not visible because no list-agent tool
was available; all spawned reviewer agents were closed after their outputs
were recorded.

- Governance/architecture-risk reviewer
  `019ea0a1-3e11-79b0-8a4d-e10214685b52`, weight 5, approved the named
  host-only parser boundary with no P1/P2 blockers, conditioned on using
  `reserved16` wording and keeping `receive_options` raw/source-scoped.
- Communications/Firmware reviewer
  `019ea0a1-62e7-72c2-85b6-faf34e984034`, weight 3, approved with no P1/P2
  blockers and required exact decoded offsets, big-endian 16-bit fields,
  source64 byte-order preservation, and focused boundary tests.
- QA/Evidence and Hardware Safety reviewer
  `019ea0a1-8773-7973-bed0-f7ed537505ca`, weight 3, approved with no P1/P2
  blockers and required a dedicated `0x91` source-index row, durable records,
  audits, and closed live surfaces. This reviewer also required that any
  empty-payload rejection be recorded as local safe-core policy, not a Digi
  requirement.

Weighted pre-mutation gate result: 11/11 approval for the named host-only
`0x91` explicit receive parser boundary. No mutating worker agents were
spawned.

Post-pause read-only QA/records reviewer
`019ea0b3-1b0b-7140-8d57-04c50fa96194` returned PASS with no P1/P2 blockers
for accepting the host-only record after the validation rerun. The reviewer was
waited and closed before the final acceptance decision.

## Implementation

- Added `fr_xbee_explicit_receive_packet_t`.
- Added `fr_xbee_parse_explicit_receive_packet`.
- The parser accepts only already-decoded frame data with frame type `0x91`,
  more than 18 bytes, and no more than `FR_XBEE_MAX_FRAME_DATA` bytes.
- The parser rejects `data_len == 18` as a local safe-core non-empty-payload
  policy matching the existing `0x90` receive-packet parser posture; this is
  not recorded as a Digi-documented requirement.
- The parser preserves the source 64-bit address as an eight-byte array.
- The parser preserves the 900HP/XSC 16-bit reserved field as big-endian
  `reserved16`.
- The parser preserves source endpoint, destination endpoint, cluster ID,
  profile ID, raw receive options, and received payload bytes.
- Added host tests for the official decoded example body, exact source64 and
  payload bytes, big-endian `reserved16`, `cluster_id`, and `profile_id`,
  max payload, null inputs, wrong frame type, short frame data, local
  header-only rejection, and over-max host frame length.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 242`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- Scoped `git diff --check` over the Task 0242 code and record boundary
  passed.
- Pending-marker scan before acceptance found only the expected Task 0242
  acceptance-status fields.
- Closed-surface review found only authority-limit/open-evidence wording and
  no authorized serial, RF, flash, relay, release, commit, push, PR, or deploy
  action.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize changing `AO`, building `0x10` or
`0x11` frames, Transmit Request generation, Explicit Addressing Command
Request generation, API transmit-frame generation, serial opens, Tier B AT
reads, XCTU or XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`,
API transmit to hardware, RF/range/throughput tests, live `0x91` behavior
claims, live bridge dispatch, relay payloads, relay/load/mains work, wiring,
flashing, monitor, framework selection, release, publication, commit, push,
PR, or deploy.

## Handoff

Handoff:
[../handoffs/0179-xbee-explicit-receive-host-parser-to-qa-communications.md](../handoffs/0179-xbee-explicit-receive-host-parser-to-qa-communications.md)

## Decision

Decision: accept a host-only parser for already-decoded received Digi XBee
Explicit Receive Indicator `0x91` frame data. Live XBee identity, current
`AO`, serial, RF, bridge, relay, and deployment gates remain separate future
work.
