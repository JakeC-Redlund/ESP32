# Task 0238: XBee Modem Status Host Parser

Status: accepted-host-modem-status-parser-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-MODEM-STATUS-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-MODEM-STATUS-0X8A-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: existing safe-core code provides host-only decoded Digi XBee
  API2 frame handling and typed host parsers for `0x88`, `0x90`, and `0x8B`.
  Official Digi 900HP/XSC documentation lists Modem Status `0x8A` as a
  received API frame, describes decoded frame data as frame type plus one
  modem-status byte, and lists modem-status values including a stack-error
  range.
- Assumptions: A host-only parser and label helper over already-decoded
  `0x8A` frame data improves diagnostics without issuing requests, opening
  serial, touching RF, changing bridge dispatch, or asserting live radio
  health.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, Tier B read safety, XCTU/XBee Studio readiness, live RF behavior,
  range, throughput, API transmit-to-hardware behavior, live bridge dispatch,
  relay payload readiness, deployment readiness, release, publication, commit,
  push, PR, and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Architecture Risk.
- Evidence need: official Digi `0x8A` source coverage, read-only reviewer
  quorum, pure host parser/helper, host tests for exact frame length, nulls,
  wrong frame type, source-listed labels, unknown fallback, source ledger,
  source-index row, docs-index reachability, plan/status/gap updates, focused
  host test output, firmware and scaffold audits, diff hygiene, and
  closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0238-xbee-modem-status-host-parser.md`,
  `.agents/handoffs/0175-xbee-modem-status-host-parser-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-modem-status-host-parser.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0238, source/docs/records/agent-process audits, scoped
  `git diff --check`, closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. Lifecycle state inventory was not visible because no list-agent tool
was available; all spawned reviewer agents were closed after their outputs
were recorded.

- Governance/architecture-risk reviewer
  `019ea058-d8fd-78b1-9eff-39bba6ec333f`, weight 5, approved the named
  host-only parser boundary with no P1/P2 blockers, conditioned on
  source-index coverage for `0x8A` structure and mapped status values.
- Communications/Firmware reviewer
  `019ea058-da75-72f2-b28b-f4b1849f308b`, weight 3, approved with no P1/P2
  blockers and recommended the exact API shape: `fr_xbee_modem_status_t`,
  `fr_xbee_parse_modem_status`, and `fr_xbee_modem_status_label`.
- QA/Evidence and Hardware Safety reviewer
  `019ea058-dc4e-7bd3-a798-2c310efdee71`, weight 3, approved with no P1/P2
  blockers and required durable records, focused tests, scaffold audits, and
  explicit closed live surfaces.

Weighted pre-mutation gate result: 11/11 approval for the named host-only
`0x8A` parser/label-helper boundary. No mutating worker agents were spawned.

## Implementation

- Added `fr_xbee_modem_status_t` for the raw modem-status byte.
- Added `fr_xbee_parse_modem_status`.
- The parser accepts only already-decoded frame data with exactly two bytes:
  frame type `0x8A` and modem-status byte.
- The parser rejects null pointers, wrong frame type, short frame data, and
  long frame data.
- Added `fr_xbee_modem_status_label`.
- The helper labels only source-listed Digi modem-status values and the
  source-listed stack-error range `0x80` through `0xff`.
- Unlisted values such as `0x04`, `0x41`, and `0x7f` return
  `unknown_modem_status`.
- Added host tests for valid decode/parse, strict rejection paths,
  source-listed labels, stack-error range labels, and unknown fallbacks.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 238`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0238 touched files passed with no output.
- Focused code closed-surface scan over the safe-core header/source/test files
  found no serial, RF, bridge, relay, flash, monitor, release, or git behavior
  in the new `0x8A` parser/helper path.
- Closed-surface review over the new Task 0238 record, handoff, and source
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
[../handoffs/0175-xbee-modem-status-host-parser-to-qa-communications.md](../handoffs/0175-xbee-modem-status-host-parser-to-qa-communications.md)

## Decision

Decision: accept a host-only parser and label helper for decoded received Digi
XBee Modem Status `0x8A` frame data. Live XBee identity, serial, RF, bridge,
relay, and deployment gates remain separate future work.
