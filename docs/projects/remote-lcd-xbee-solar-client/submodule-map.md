# Submodule Map

## Verified facts

- This repository now tracks seven private Git submodules for this project lane
  under `submodules/hardware/`. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-PRIVATE-SUBMODULES-2026-05-26`.
- Each private submodule has a docs-only initial commit on `main` and contains
  only `README.md`, `AGENTS.md`, `docs/`, `knowledge-base/`, `research/`, a
  task record, and a Hardware QA handoff. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-PRIVATE-SUBMODULES-2026-05-26`.
- Existing XBee coverage is for Digi `XBP9B-DPUT-001` and the current
  read-only proof boundary. Source IDs: `SRC-DIGI-XBP9B-DPUT-001`,
  `SRC-LOCAL-XBEE-READONLY-PROBE-2026-05-18`.
- Current same-session dirty worktree evidence shows a `docs/identity-intake.md`
  packet file in all seven private `rlxsc-*` submodule worktrees. This does
  not prove committed submodule state, parent-pinned submodule state, exact
  hardware identity, selected parts, measurements, or bench readiness. Source
  ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-IDENTITY-WORKTREE-CONSOLIDATION-2026-06-06`.
- The first three priority power/safety packets (`rlxsc-18650-cell`,
  `rlxsc-bms-protection`, and `rlxsc-solar-charger-power-path`) have a
  parent records-only docs review/tracking pass. This does not prove exact
  hardware identity, submodule commit state, parent-pinned state,
  measurements, compatibility, safety closure, or bench readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-POWER-SAFETY-PACKET-REVIEW-2026-06-06`.
- The remaining four board/interface/XBee packets (`rlxsc-esp32-client-node`,
  `rlxsc-lcd-20x4-i2c`, `rlxsc-rotary-encoder`, and
  `rlxsc-xbee-pro-s3b`) have a parent records-only docs review/tracking pass.
  This does not prove exact hardware identity, submodule commit state,
  parent-pinned state, measurements, pin readiness, RF readiness,
  compatibility, firmware readiness, or bench readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-BOARD-INTERFACE-XBEE-PACKET-REVIEW-2026-06-06`.
- All seven identity-intake packets now have submodule-local docs/records-only
  review records in current dirty submodule worktrees. This does not prove
  exact hardware identity, committed submodule state, parent-pinned state,
  source closure, measurements, compatibility, safety closure, pin readiness,
  RF readiness, firmware readiness, or bench readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-LOCAL-REVIEW-2026-06-06`.

## Assumptions

- Each lane may later receive firmware, hardware, QA, or communications work
  only after source-backed intake closes the relevant unknowns.

## Private submodules

| Submodule path | Private repo | Owner lane | Acceptance boundary |
| --- | --- | --- | --- |
| `submodules/hardware/rlxsc-esp32-client-node` | `cyberpivots/rlxsc-esp32-client-node` | Hardware plus Architecture | Exact board, boot pins, recovery, power path, and framework ADR |
| `submodules/hardware/rlxsc-xbee-pro-s3b` | `cyberpivots/rlxsc-xbee-pro-s3b` | Communications plus Hardware | Read-only discovery, carrier and antenna review, no writes |
| `submodules/hardware/rlxsc-lcd-20x4-i2c` | `cyberpivots/rlxsc-lcd-20x4-i2c` | Hardware plus Firmware later | Exact module, voltage, address, pullups, and backlight evidence |
| `submodules/hardware/rlxsc-rotary-encoder` | `cyberpivots/rlxsc-rotary-encoder` | Hardware plus Firmware later | Exact part, debounce, pullups, voltage, and boot-pin review |
| `submodules/hardware/rlxsc-18650-cell` | `cyberpivots/rlxsc-18650-cell` | Hardware plus QA | Exact cell datasheet, inspection, protection status, condition, and safe storage record |
| `submodules/hardware/rlxsc-bms-protection` | `cyberpivots/rlxsc-bms-protection` | Hardware plus QA | Exact board schematic/source, thresholds, FET path, current rating, and connector labels |
| `submodules/hardware/rlxsc-solar-charger-power-path` | `cyberpivots/rlxsc-solar-charger-power-path` | Hardware plus QA | Exact panel, charger, thermistor policy, current limit, load sharing, fuse/protection, and enclosure review |

## Development order

1. Commit the current submodule-local identity-intake and local-review records
   only with explicit submodule commit authority.
2. Later parent-pin reviewed submodule commits only with explicit parent
   pointer authority.
3. Continue exact power and safety evidence collection in `rlxsc-18650-cell`,
   `rlxsc-bms-protection`, and
   `rlxsc-solar-charger-power-path`.
4. Continue exact board and interface evidence collection in
   `rlxsc-esp32-client-node`,
   `rlxsc-lcd-20x4-i2c`, `rlxsc-rotary-encoder`, and
   `rlxsc-xbee-pro-s3b`.
5. Write parent hardware ADRs only after submodule evidence exists.
6. Keep any early behavior prototype host-only and outside hardware evidence
   submodules.

## Unknowns

- Whether the LCD and fuel-gauge paths share an I2C bus, use separate buses, or
  remain unimplemented.
- Whether the final power architecture includes a fuel gauge.
- Whether any exact selected antenna, fuse/protection part, or enclosure later
  justifies a separate private repo.

## Stop gates

Do not add firmware packages, wiring diagrams, accepted interface contracts,
XBee writes/transmit frames, battery/solar wiring, or live bench instructions
from this map. Future module work must cite source IDs or remain marked
unresolved.
