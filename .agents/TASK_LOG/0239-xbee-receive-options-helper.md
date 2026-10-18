# Task 0239: XBee Receive Options Helper

Status: accepted-host-receive-options-helper-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-RECEIVE-OPTIONS-HELPER-2026-06-07`,
`SRC-DIGI-XBEE-RECEIVE-PACKET-0X90-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AO`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: existing safe-core code parses decoded Digi XBee Receive
  Packet `0x90` frames, preserves the raw `receive_options` byte, and already
  has focused host tests for receive-packet parser boundaries. Official Digi
  900HP/XSC documentation defines `0x90` receive options as a bit field where
  bit `0x01` means packet acknowledged, bit `0x02` means packet broadcast,
  delivery method is encoded by `0xC0`, and option values may combine.
- Assumptions: Host-only helpers over the raw `receive_options` byte improve
  diagnostics without changing receive-packet parser acceptance, generating
  requests, opening serial, touching RF, changing bridge dispatch, or asserting
  live radio health.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, Tier B read safety, XCTU/XBee Studio readiness, live RF behavior,
  range, throughput, API transmit-to-hardware behavior, live bridge dispatch,
  relay payload readiness, deployment readiness, release, publication, commit,
  push, PR, and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Architecture Risk.
- Evidence need: official Digi `0x90` receive-options source coverage,
  read-only reviewer quorum, pure host helper implementation, host tests for
  acknowledged bit, broadcast bit, combined options, delivery-method mask
  labels, source ledger, source-index row, docs-index reachability,
  plan/status/gap updates, focused host test output, firmware and scaffold
  audits, diff hygiene, and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0239-xbee-receive-options-helper.md`,
  `.agents/handoffs/0176-xbee-receive-options-helper-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-receive-options-helper.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0239, source/docs/records/agent-process audits, scoped
  `git diff --check`, closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. Lifecycle state inventory was not visible because no list-agent tool
was available; all spawned reviewer agents were closed after their outputs
were recorded.

- Governance/architecture-risk reviewer
  `019ea069-d658-7d33-9e33-4e108b7fc2f5`, weight 5, approved the named
  host-only helper boundary with no P1/P2 blockers, conditioned on masking
  delivery method with `options & 0xC0u` and adding exact source coverage.
- Communications/Firmware reviewer
  `019ea069-d7db-7f63-89c2-ab521f5b74b5`, weight 3, approved with no P1/P2
  blockers and recommended `fr_xbee_receive_options_acknowledged`,
  `fr_xbee_receive_options_broadcast`, and
  `fr_xbee_receive_options_delivery_method_label`.
- QA/Evidence and Hardware Safety reviewer
  `019ea069-d9d7-71d1-ac5b-16d79879cab6`, weight 3, approved with no P1/P2
  blockers and required exact source coverage, durable records, focused tests,
  scaffold audits, and explicit closed live surfaces.

Weighted pre-mutation gate result: 11/11 approval for the named host-only
`0x90` receive-options helper boundary. No mutating worker agents were spawned.

## Implementation

- Added `fr_xbee_receive_options_acknowledged`.
- Added `fr_xbee_receive_options_broadcast`.
- Added `fr_xbee_receive_options_delivery_method_label`.
- Preserved `fr_xbee_parse_receive_packet` raw parser behavior and acceptance
  boundaries.
- The acknowledged helper checks only bit `0x01`.
- The broadcast helper checks only bit `0x02`.
- The delivery-method helper masks only `options & 0xC0u` and labels:
  `0x00` as `invalid_delivery_method`, `0x40` as `point_multipoint`,
  `0x80` as `directed_broadcast`, and `0xC0` as `digimesh`.
- Added host tests for individual and combined acknowledge/broadcast bits,
  each delivery-method value, and lower-bit combinations such as `0x41`,
  `0x82`, `0xC1`, and `0xFF`.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 239`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0239 touched files passed with no output.
- Focused code closed-surface scan over the safe-core header/source/test files
  found no serial, RF, bridge, relay, flash, monitor, release, or git behavior
  in the new receive-options helper path.
- Closed-surface review over the new Task 0239 record, handoff, and source
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
[../handoffs/0176-xbee-receive-options-helper-to-qa-communications.md](../handoffs/0176-xbee-receive-options-helper-to-qa-communications.md)

## Decision

Decision: accept host-only diagnostic helpers over decoded raw Digi XBee
Receive Packet `0x90` `receive_options` bytes. Live XBee identity, serial, RF,
bridge, relay, and deployment gates remain separate future work.
