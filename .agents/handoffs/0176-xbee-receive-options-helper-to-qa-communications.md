# Handoff 0176: XBee Receive Options Helper To QA Communications

Status: active

Task:
[Task 0239](../TASK_LOG/0239-xbee-receive-options-helper.md)

Source coverage:
`SRC-LOCAL-XBEE-RECEIVE-OPTIONS-HELPER-2026-06-07`,
`SRC-DIGI-XBEE-RECEIVE-PACKET-0X90-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AO`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0239 adds pure host-only diagnostic helpers over parsed Digi XBee
  Receive Packet `0x90` raw `receive_options` bytes.
- `fr_xbee_parse_receive_packet` still preserves the raw `receive_options`
  byte and parser acceptance boundaries are unchanged.
- The acknowledged helper checks only bit `0x01`.
- The broadcast helper checks only bit `0x02`.
- The delivery-method helper masks only `options & 0xC0u`.
- Lower-bit combinations such as `0x41`, `0x82`, `0xC1`, and `0xFF` keep the
  delivery-method label determined by the masked high bits.
- Focused safe-core host tests passed after the helper/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat acknowledged/broadcast/delivery-method helpers as diagnostics
  over raw received option bits, not as live RF proof, app-level ACK proof,
  relay acceptance, command success proof, or deployment readiness.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, Tier B read
  safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput, API
  transmit-to-hardware behavior, relay payload readiness, deployment readiness,
  release, publication, commit, push, PR, and deploy remain open.

## Continue With

- Preserve raw `0x90` parser output and keep parser acceptance unchanged.
- Keep delivery-method decoding masked to `options & 0xC0u`.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0239 as `0x10` builder, Transmit Request generation, API
  transmit-frame generation, current hardware identity, serial-open, Tier B
  read, XCTU/XBee Studio discovery, XBee setting-write, API
  transmit-to-hardware, RF/range/throughput, live bridge dispatch,
  relay-payload, wiring, flash, monitor, live bench, release, publication,
  commit, push, PR, or deploy authority.
