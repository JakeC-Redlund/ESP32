# Task 0228: XBee API Parser Host Vectors

Status: accepted-host-parser-vectors-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`,
`SRC-LOCAL-XBEE-HUB-SPOKE-HOST-PLAN-2026-06-05`, and
`SRC-LOCAL-XBEE-TIER-A-NO-SERIAL-IDENTITY-EVIDENCE-2026-06-05`.

## Routing

- Verified facts: `tests/four_relay_safe_core/test_xbee_frame_codec.c` already
  covered escaped API2 encode/decode, checksum rejection, bad length,
  truncated escape, short frame, transmit-status decode, AT-response parsing,
  and receive-packet parsing. `tests/four_relay_safe_core/run_host_tests.py`
  builds safe-core C with `-Werror` and includes `xbee_frame.c`. The current
  known-gap list requested additional XBee API parser vectors beyond that
  coverage.
- Assumptions: Host-only codec/parser boundary vectors improve local safety
  coverage without changing live XBee settings, RF behavior, relay behavior,
  serial surfaces, or firmware framework selection.
- Unknowns/open evidence: current physical XBee adapter identity, exact radio
  settings, Tier B read safety, XCTU/XBee Studio readiness, live RF behavior,
  range, throughput, relay payload readiness, deployment readiness, release,
  publication, commit, push, PR, and deploy remain unproved or unauthorized.
- Selected tier: Tier 2 host-only protocol-adjacent source/test/record update.
- Owner role: Communications and Firmware, with QA/Evidence and Agent
  Operations.
- Evidence need: current test/source inspection, read-only reviewer quorum,
  host parser vector tests, source guard for adversarial direct parser input,
  source ledger, source-index row, docs-index reachability, plan/status/gap
  updates, focused host test output, firmware/source/docs/records/process
  audits, diff hygiene, and scaffold validation.
- Mutation boundary:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`,
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`,
  `.agents/TASK_LOG/0228-xbee-api-parser-host-vectors.md`,
  `.agents/handoffs/0165-xbee-api-parser-host-vectors-to-qa-communications.md`,
  `knowledge-base/source-ledger/2026-06-07-xbee-api-parser-host-vectors.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: safe-core host tests, firmware audit, durable-record audit
  from Task 0228, source/docs/agent-process audits, scoped `git diff --check`,
  closed-surface review, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was waited, captured, and closed before mutation:

- Communications/Firmware reviewer
  `019e9fa0-f121-7552-bce5-16099e1085fd`, weight 3, approved with no P1
  blockers and two P2 conditions: add a receive-packet parser guard before
  adversarial direct parser vectors can set `frame.data_len` beyond the fixed
  frame buffer, and keep records strictly host-only.
- QA/Agent-Operations/Evidence reviewer
  `019e9fa1-0960-7092-b1c7-07c94fd5f6af`, weight 3, approved the named Tier 2
  host-only parser-vector and durable-record mutation with no P1/P2 blockers,
  conditioned on preserving closed live XBee, serial, RF, relay, flash,
  release, and publication surfaces.

Weighted reviewer result before mutation: 6/6 conditional approval. The
receive-packet parser P2 was resolved by adding an early
`frame->data_len > FR_XBEE_MAX_FRAME_DATA` rejection before field copies and a
host test that exercises the over-max direct parser input. No mutating worker
agents were spawned. No listable completed-agent inventory was available in the
exposed toolset before spawn; both spawned reviewers were waited and closed
before mutation. Under `bypassPermissions`, repo hooks remain advisory and are
not runtime cleanup proof.

## Implementation

- Added a host-side receive-packet parser guard for `frame.data_len` values
  larger than `FR_XBEE_MAX_FRAME_DATA`.
- Added exact escaped-wire API2 output coverage, checksum-byte escaping,
  reserved length-byte escaping, insufficient escaped-capacity rejection,
  max-frame encode/decode, over-max encode rejection, missing-delimiter
  rejection, trailing decoded-byte rejection, and over-max length-field
  rejection.
- Added AT-response parser coverage for no-value responses and over-max AT
  values.
- Added receive-packet parser coverage for max host frames and over-max direct
  host frame length rejection.
- Added durable source, task, handoff, docs, and status records for the
  host-only parser-vector gate.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  passed: `PASS: four_relay_safe_core split host tests succeeded`.
- `git diff --check -- firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c tests/four_relay_safe_core/test_xbee_frame_codec.c .agents/TASK_LOG/0228-xbee-api-parser-host-vectors.md .agents/handoffs/0165-xbee-api-parser-host-vectors-to-qa-communications.md knowledge-base/source-ledger/2026-06-07-xbee-api-parser-host-vectors.md knowledge-base/source-index.md docs/index.md research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 228`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
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
[../handoffs/0165-xbee-api-parser-host-vectors-to-qa-communications.md](../handoffs/0165-xbee-api-parser-host-vectors-to-qa-communications.md)

## Decision

Decision: accept host-only XBee API2 codec/parser boundary vectors and the
receive-packet parser length guard. Live XBee identity, serial, RF, relay, and
deployment gates remain separate future work.
