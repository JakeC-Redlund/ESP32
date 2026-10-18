# XBee API Parser Host Vectors Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`

## Verified Facts

- Task 0228 adds host-only XBee API2 codec/parser boundary vectors to
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`.
- Task 0228 adds an early receive-packet parser guard that rejects direct host
  frames whose `data_len` is larger than `FR_XBEE_MAX_FRAME_DATA`.
- The new host vectors cover exact escaped-wire bytes, escaped checksum bytes,
  escaped length bytes, max and over-max frame boundaries, malformed wire
  rejection, AT-response boundaries, and receive-packet boundaries.
- Same-session Tier 2 read-only reviewers approved the named host-only parser
  vector gate after the receive-packet guard P2 was resolved.

## Evidence

- Parser source:
  `firmware/projects/four-relay-xbee-wifi/components/safe_core/src/xbee_frame.c`
- Host vectors:
  `tests/four_relay_safe_core/test_xbee_frame_codec.c`
- Host test runner:
  `tests/four_relay_safe_core/run_host_tests.py`
- Task record:
  `.agents/TASK_LOG/0228-xbee-api-parser-host-vectors.md`
- Handoff:
  `.agents/handoffs/0165-xbee-api-parser-host-vectors-to-qa-communications.md`
- Source-index row:
  `SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`
- Existing source context:
  `SRC-DIGI-XBEE-900HP-AP`,
  `SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
  `SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`,
  `SRC-LOCAL-XBEE-HUB-SPOKE-HOST-PLAN-2026-06-05`, and
  `SRC-LOCAL-XBEE-TIER-A-NO-SERIAL-IDENTITY-EVIDENCE-2026-06-05`

## Authority Limits

This source record does not accept or authorize serial opens, Tier B AT reads,
XCTU or XBee Studio discovery, XBee setting writes, `WR`, `AC`, `KY`, API
transmit to hardware, RF/range/throughput tests, live bridge dispatch, relay
payloads, relay/load/mains work, wiring, flashing, monitor, framework
selection, release, publication, commit, push, PR, or deploy.

## Open Evidence

- Current physical XBee adapter identity and second one-at-a-time adapter delta.
- Tier B read-safety and XCTU/XBee Studio readiness evidence.
- Any future live RF, range, throughput, relay-payload, or deployment proof.
