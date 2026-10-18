# Handoff 0179: XBee Explicit Receive Host Parser To QA Communications

Status: active

Task:
[Task 0242](../TASK_LOG/0242-xbee-explicit-receive-host-parser.md)

Source coverage:
`SRC-LOCAL-XBEE-EXPLICIT-RECEIVE-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-EXPLICIT-RECEIVE-0X91-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AO`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0242 adds a pure host-only parser for already-decoded Digi XBee
  Explicit Receive Indicator `0x91` frame data.
- The parser requires frame type `0x91`, more than 18 decoded frame-data bytes,
  and no more than `FR_XBEE_MAX_FRAME_DATA` bytes.
- The parser preserves the source 64-bit address as an eight-byte array.
- The parser preserves the 900HP/XSC 16-bit reserved field as `reserved16`.
- The parser preserves source endpoint, destination endpoint, cluster ID,
  profile ID, raw receive options, and received payload bytes.
- Focused safe-core host tests passed after the parser/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat parsed explicit receive packets as diagnostics over an already
  received frame, not as proof that current project hardware has `AO=1`, can
  emit `0x91`, transmitted anything, diagnosed live links, or accepted relay
  payloads.
- The local parser rejects header-only `0x91` data as safe-core receive-packet
  policy; this is not a Digi-documented requirement.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, current `AO`,
  whether local radios can emit `0x91`, Tier B read safety, XCTU/XBee Studio
  readiness, live RF behavior, range, throughput, API transmit-to-hardware
  behavior, live bridge behavior, relay payload readiness, deployment
  readiness, release, publication, commit, push, PR, and deploy remain open.

## Continue With

- Preserve the parser as an in-memory decoded-frame parser only.
- Keep the 900HP/XSC 16-bit field wording as `reserved16`, not source/network
  address.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0242 as authority to change `AO`, build `0x10` or `0x11`
  frames, generate API transmit frames, identify current hardware, open serial,
  run Tier B reads, launch XCTU/XBee Studio discovery, write XBee settings,
  transmit to hardware, test RF/range/throughput, dispatch bridge payloads,
  operate relays, wire hardware, flash, monitor, run live bench, release,
  publish, commit, push, create a PR, or deploy.
