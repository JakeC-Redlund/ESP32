# Hardware Intake

## Verified facts

- The exact LCD module, encoder, ESP32 board, 18650 cell, BMS board, solar
  panel, charger module, XBee carrier, antenna, fuse/protection parts, and
  enclosure are unresolved in this workspace. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SCAFFOLD-2026-05-26`.
- Private submodules exist for each hardware lane, but they are evidence
  folders only and do not select exact parts. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-PRIVATE-SUBMODULES-2026-05-26`.
- Candidate/reference sources exist for relevant part classes, but they do not
  identify selected project hardware. Source IDs: `SRC-NXP-PCF8574-74A`,
  `SRC-BOURNS-PEC11R`, `SRC-TI-BQ25185`, `SRC-TI-BQ2970`,
  `SRC-TI-BQ27441-G1`, `SRC-UL-LIION-SAFETY`.
- Task 0183 records a parent repo hardware intake audit only. It confirms the
  checklist and submodule lanes are current routing surfaces, but it does not
  close exact component identity, power safety, pin-risk, XBee carrier, or
  bench readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-HARDWARE-INTAKE-AUDIT-2026-06-05`.
- Current same-session dirty worktree evidence shows a `docs/identity-intake.md`
  packet file in all seven private `rlxsc-*` submodule worktrees. These files
  are not accepted here as committed submodule state, parent-pinned state,
  selected hardware, exact component identity, measurements, or bench
  readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-IDENTITY-WORKTREE-CONSOLIDATION-2026-06-06`.
- The first three priority power/safety packets have been reviewed and tracked
  only as docs/records-only packet structures in current dirty submodule
  worktrees: `rlxsc-18650-cell`, `rlxsc-bms-protection`, and
  `rlxsc-solar-charger-power-path`. This does not close exact hardware
  identity, source closure, compatibility, measurements, current limits,
  submodule commits, parent pointer updates, safety closure, or bench
  readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-POWER-SAFETY-PACKET-REVIEW-2026-06-06`.
- The remaining four board/interface/XBee packets have been reviewed and
  tracked only as docs/records-only packet structures in current dirty
  submodule worktrees: `rlxsc-esp32-client-node`, `rlxsc-lcd-20x4-i2c`,
  `rlxsc-rotary-encoder`, and `rlxsc-xbee-pro-s3b`. This does not close exact
  hardware identity, source closure, pin readiness, RF readiness,
  compatibility, measurements, submodule commits, parent pointer updates,
  firmware readiness, or bench readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-BOARD-INTERFACE-XBEE-PACKET-REVIEW-2026-06-06`.
- All seven identity-intake packets now have submodule-local docs/records-only
  review records in current dirty submodule worktrees. This does not close
  exact hardware identity, source closure, measurements, compatibility, safety
  closure, pin readiness, RF readiness, firmware readiness, submodule commits,
  parent pointer updates, or bench readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-LOCAL-REVIEW-2026-06-06`.

## Assumptions

- Initial intake will be source and physical-inspection driven.
- Vendor module pages, datasheets, markings, and photos should be recorded
  before any electrical action.
- Bench records should use the existing bench-record template style and keep
  raw local captures out of public artifacts unless explicitly redacted.

## Required intake evidence

| Hardware | Required evidence before closure |
| --- | --- |
| ESP32 board | Board vendor/revision, module marking, regulator marking, USB-UART marking, boot/recovery method, power input options, and source links |
| XBee carrier | Carrier vendor/revision, socket pinout, DIN/DOUT naming, logic voltage, power source, reset/sleep/flow-control pins, and antenna clearance |
| 20x4 LCD | Exact module/backpack markings, controller/backpack IC, I2C address policy, pullups, logic voltage, backlight current, and source links |
| Rotary encoder | Exact part number, detents, pulse count, switch option, pullup/debounce needs, voltage, mounting, and source links |
| 18650 cell | Manufacturer, model, chemistry, capacity, max charge/discharge ratings, protection status, age/condition, and safe handling source |
| BMS/protection | Board model, IC markings, thresholds, FET path, current rating, connector labels, balancing/protection scope, and source links |
| Solar panel | Open-circuit voltage, short-circuit current, rated power, connector, environmental rating, blocking protection, and source links |
| Charger/power path | Module model, charger IC, current limits, charge voltage, thermistor policy, load-sharing behavior, input range, and source links |
| Fuse/protection | Fuse or resettable protection rating, placement rationale, connector strain relief, reverse protection, and source links |
| Enclosure | Outdoor rating need, cable glands, venting, thermal constraints, battery retention, and inspection access |

## Unknowns

- Whether any requested hardware is already physically present.
- Whether candidate IC references match any purchased module.
- Whether the deployment environment needs weatherproofing, UV exposure,
  condensation control, temperature derating, or regulatory review.
- Whether any private submodule has collected source-backed exact selected-part
  evidence beyond the current dirty worktree identity-intake and local-review
  packet structures.
- Whether any identity-intake packet file has been committed inside its
  submodule and later parent-pinned.

## Stop gates

Do not infer pinouts, voltages, current limits, I2C addresses, charge limits, or
connector labels from generic module photos. Missing evidence remains an
unresolved gap.
