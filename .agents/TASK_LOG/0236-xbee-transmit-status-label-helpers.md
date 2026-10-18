# Task 0236: XBee Transmit Status Label Helpers

Status: accepted-host-label-helpers-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-LABEL-HELPERS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-TRANSMIT-STATUS-HOST-PARSER-2026-06-07`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Routing

- Verified facts: Task 0235 accepts host-only raw parsing of decoded Digi XBee
  Extended Transmit Status `0x8B` frame data. Digi source row
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05` covers the official
  `0x8B` delivery-status and discovery-status byte table.
- Assumptions: Source-backed label helpers over raw status bytes improve host
  diagnostics without changing parser behavior or claiming live delivery, link
  health, RF success, relay readiness, or deployment readiness.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, Tier B read safety, XCTU/XBee Studio readiness, live RF behavior,
  range, throughput, API transmit-to-hardware behavior, live bridge dispatch,
  relay payload readiness, deployment readiness, release, publication, commit,
  push, PR, and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence, Hardware Safety,
  and Architecture Risk.
- Evidence need: official Digi source coverage, read-only reviewer quorum,
  pure host label helpers, host tests for source-listed and unknown values,
  source ledger, source-index row, docs-index reachability, plan/status/gap
  updates, focused host test output, firmware and scaffold audits, diff hygiene,
  and closed-surface review.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/include/four_relay_core.h`,
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0236-xbee-transmit-status-label-helpers.md`,
  `.agents/handoffs/0173-xbee-transmit-status-label-helpers-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-transmit-status-label-helpers.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0236, source/docs/records/agent-process audits, scoped
  `git diff --check`, closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was attempted, waited, captured, and closed before
mutation. The first wait timed out before final messages; later waits captured
all final reviewer outputs, and all three reviewer agents were closed after
their outputs were recorded.

- Governance/architecture-risk reviewer
  `019ea02f-9aa2-7960-b68b-9ceabaa5e0da`, weight 5, approved the host-only
  helper boundary with no P1/P2 blockers.
- Communications/Firmware reviewer
  `019ea02f-b9fa-7c61-ba9e-11b184d32092`, weight 3, approved with exact test
  conditions for all source-listed delivery labels, source-listed discovery
  labels, and unknown fallbacks for unlisted bytes such as `0x24`, `0x40`, and
  `0xff`.
- QA/Evidence and Hardware Safety reviewer
  `019ea02f-d753-76c1-abdf-13f271be7c80`, weight 3, approved with no P1/P2
  blockers and required host tests, durable records, and explicit closed live
  surfaces.

Weighted pre-mutation gate result: 11/11 approval for the named host-only label
helper boundary. No mutating worker agents were spawned.

## Implementation

- Added `fr_xbee_delivery_status_label` and
  `fr_xbee_discovery_status_label`.
- Preserved `fr_xbee_parse_transmit_status` raw parser behavior.
- Returned source-backed static labels only for Digi-listed `0x8B` delivery and
  discovery status values.
- Returned static unknown labels for unlisted status bytes.
- Added host tests for all listed delivery labels, listed discovery labels, and
  required unknown fallback values.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 236`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `git diff --check` over the Task 0236 touched files passed with no output.
- Closed-surface review over the Task 0236 touched files found only explicit
  unknown/open-evidence or authority-limit language for live XBee, serial, RF,
  relay, flash, release, publication, and git surfaces.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize `0x10` builder work, API transmit-frame
generation, serial opens, Tier B AT reads, XCTU or XBee Studio discovery, XBee
setting writes, `WR`, `AC`, `KY`, API transmit to hardware, RF/range/throughput
tests, live bridge dispatch, relay payloads, relay/load/mains work, wiring,
flashing, monitor, framework selection, release, publication, commit, push, PR,
or deploy.

## Handoff

Handoff:
[../handoffs/0173-xbee-transmit-status-label-helpers-to-qa-communications.md](../handoffs/0173-xbee-transmit-status-label-helpers-to-qa-communications.md)

## Decision

Decision: accept host-only label helpers for decoded Digi XBee Extended
Transmit Status `0x8B`. Live XBee identity, serial, RF, bridge, relay, and
deployment gates remain separate future work.
