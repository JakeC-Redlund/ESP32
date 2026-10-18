# Handoff 0178: XBee Aggregate Addressing Update Host Parser To QA Communications

Status: active

Task:
[Task 0241](../TASK_LOG/0241-xbee-aggregate-addressing-update-host-parser.md)

Source coverage:
`SRC-LOCAL-XBEE-AGGREGATE-ADDRESSING-UPDATE-HOST-PARSER-2026-06-07`,
`SRC-DIGI-XBEE-AGGREGATE-ADDRESSING-UPDATE-0X8E-2026-06-07`,
`SRC-DIGI-XBEE-900HP-USER-GUIDE-REFRESH-2026-06-05`,
`SRC-LOCAL-XBEE-API-PARSER-HOST-VECTORS-2026-06-07`,
`SRC-DIGI-XBEE-900HP-AP`, and
`SRC-LOCAL-FOUR-RELAY-SAFE-CORE-CONTRACT-2026-05-19`.

## Verified Facts

- Task 0241 adds a pure host-only parser for already-decoded Digi XBee
  Aggregate Addressing Update `0x8E` frame data.
- The parser requires frame type `0x8E` and exactly 18 decoded frame-data
  bytes.
- The parser preserves the raw reserved byte.
- The parser preserves the new and old 64-bit `DH`/`DL` destination-address
  fields as byte arrays.
- The parser rejects null inputs, wrong frame type, short frame data, and long
  frame data.
- Focused safe-core host tests passed after the parser/vector changes.

## Assumptions

- Future XBee parser work will remain host-only unless a separate live gate
  explicitly opens serial, RF, bridge dispatch, or hardware surfaces.
- QA will treat parsed aggregate-addressing updates as diagnostics over an
  already received frame, not as proof that current project hardware can emit
  `0x8E`, supports DigiMesh `AG`, updated `DH`/`DL`, transmitted anything,
  diagnosed live links, or accepted relay payloads.

## Unknowns

- Current physical XBee adapter identity, exact radio settings, whether local
  radios can emit `0x8E`, DigiMesh support for the local configuration, Tier B
  read safety, XCTU/XBee Studio readiness, live RF behavior, range, throughput,
  API transmit-to-hardware behavior, relay payload readiness, deployment
  readiness, release, publication, commit, push, PR, and deploy remain open.

## Continue With

- Preserve the parser as an in-memory decoded-frame parser only.
- Keep new/old address wording scoped to `DH`/`DL` destination addresses, not
  radio identity or `SH`/`SL`.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 tests/four_relay_safe_core/run_host_tests.py`
  before accepting future safe-core XBee parser changes.

## Do Not Do

- Do not treat Task 0241 as authority to issue `AG`, update `DH` or `DL`, build
  `0x10` frames, generate API transmit frames, identify current hardware, open
  serial, run Tier B reads, launch XCTU/XBee Studio discovery, write XBee
  settings, transmit to hardware, test RF/range/throughput, dispatch bridge
  payloads, operate relays, wire hardware, flash, monitor, run live bench,
  release, publish, commit, push, create a PR, or deploy.
