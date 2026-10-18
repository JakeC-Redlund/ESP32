# Separate Hardware Development Stream

## Status

`remote-lcd-xbee-solar-client` is a separate hardware-device development
stream. The parent ESP32 repository remains the coordination, source-index,
status, and validation surface. Hardware evidence is collected inside the
private `rlxsc-*` submodules first.

This record does not authorize firmware source, framework files, XBee writes,
API transmit frames, ESP32 DIN/DOUT wiring, battery charging, solar wiring,
power-path wiring, pin assignments, or live bench action.

## Verified facts

- The project scaffold is documentation-only and framework-neutral. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SCAFFOLD-2026-05-26`.
- Seven private docs-only Git submodules exist under
  `submodules/hardware/`. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-PRIVATE-SUBMODULES-2026-05-26`.
- Task 0183 records a parent repo hardware intake audit with fresh submodule
  status context and keeps all component identity gaps open. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-HARDWARE-INTAKE-AUDIT-2026-06-05`.
- Task 0218 records that all seven private `rlxsc-*` submodule worktrees now
  contain current `docs/identity-intake.md` packet files. The packet files are
  dirty worktree evidence only, not committed submodule state, parent-pinned
  state, selected hardware, exact identity closure, measurements, or bench
  readiness. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-IDENTITY-WORKTREE-CONSOLIDATION-2026-06-06`.
- Task 0219 records that the first three priority power/safety packets
  (`rlxsc-18650-cell`, `rlxsc-bms-protection`, and
  `rlxsc-solar-charger-power-path`) have been reviewed and tracked as
  docs/records-only packet structures in current dirty submodule worktrees.
  No exact hardware identity, submodule commit, parent pin, measurement,
  compatibility, safety closure, or bench readiness is accepted. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-POWER-SAFETY-PACKET-REVIEW-2026-06-06`.
- Task 0220 records that the remaining four board/interface/XBee packets
  (`rlxsc-esp32-client-node`, `rlxsc-lcd-20x4-i2c`,
  `rlxsc-rotary-encoder`, and `rlxsc-xbee-pro-s3b`) have been reviewed and
  tracked as docs/records-only packet structures in current dirty submodule
  worktrees. No exact hardware identity, submodule commit, parent pin,
  measurement, pin readiness, RF readiness, firmware readiness, compatibility,
  or bench readiness is accepted. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-BOARD-INTERFACE-XBEE-PACKET-REVIEW-2026-06-06`.
- Task 0221 records that all seven identity-intake packets now have
  submodule-local docs/records-only review records in current dirty submodule
  worktrees. No exact hardware identity, submodule commit, parent pin,
  source closure, measurement, compatibility, safety closure, pin readiness,
  RF readiness, firmware readiness, or bench readiness is accepted. Source ID:
  `SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-LOCAL-REVIEW-2026-06-06`.
- ESP-NOW BBS, Gate F, Gate G, Gate H, mesh, BLE, PCAP, Win31/DOS-C bridge,
  flash, erase, monitor, serial-write, relay, load, mains, TFT, MicroSD, and
  network expansion gates are tracked separately in the development status
  ledger. Source ID:
  `SRC-LOCAL-ESPNOW-DEVELOPMENT-STATUS-REVIEW-2026-05-26`.
- The current XBee boundary remains read-only discovery only, with no setting
  writes, no API transmit frames, and no ESP32 DIN/DOUT wiring. Source IDs:
  `SRC-LOCAL-XBEE-READONLY-PROBE-2026-05-18`,
  `SRC-DIGI-XBEE-900HP-USER-GUIDE`.

## Assumptions

- Hardware submodules are evidence and design ownership lanes, not application
  runtime repositories.
- No DOS-C validation is needed for this hardware-device stream unless a later
  accepted interface ADR explicitly opens a narrow paired boundary.
- Future integration with ESP-NOW BBS, Win31/DOS-C, mesh, BLE, or network work
  requires a later accepted ADR and source-backed interface boundary.

## Unknowns

- Exact ESP32 board, LCD/backpack, encoder, 18650 cell, BMS/protection board,
  solar panel, charger/power path, XBee carrier, antenna, fuse/protection, and
  enclosure remain unresolved.
- No accepted power budget, pin map, framework ADR, XBee carrier proof,
  antenna proof, charger/power-path design, read-only bench proof, or live
  implementation gate exists for this lane.

## Separation rules

- Develop hardware-device evidence in the relevant `rlxsc-*` submodule first.
- Limit parent ESP32 changes to coordination records, source-index entries,
  task logs, handoffs, docs index updates, status ledgers, validation records,
  and submodule pointer updates.
- Do not change DOS-C for hardware-device work unless a future accepted ADR
  requires a paired DOS-C validation surface.
- Do not import BBS runtime assumptions, bridge request types, Win31 controls,
  mesh behavior, BLE behavior, or live ESP-NOW proof claims into hardware
  submodule records.
- Keep radio identifiers, raw settings, address plans, keys, photos, and raw
  bench captures private unless a separate publication review redacts and
  approves them.

## Development order

1. Hardware identity intake in each private submodule.
2. Power and safety review for `rlxsc-18650-cell`,
   `rlxsc-bms-protection`, and
   `rlxsc-solar-charger-power-path`.
3. Board and interface review for `rlxsc-esp32-client-node`,
   `rlxsc-lcd-20x4-i2c`, `rlxsc-rotary-encoder`, and
   `rlxsc-xbee-pro-s3b`.
4. Hardware-only ADR package in the parent repo after submodule evidence
   exists.
5. Host-only prototype work, if needed, outside hardware evidence submodules.
6. Read-only bench gate after identity, power, recovery, and current-limit
   evidence are complete.
7. Implementation gate only after hardware identity, power safety, pin plan,
   framework ADR, and read-only bench proof are accepted.

## Current next action

Commit the current submodule-local identity-intake and local-review records
only with explicit submodule commit authority, then later parent-pin reviewed
submodule commits only with explicit parent pointer authority. Continue exact
hardware evidence collection for cell, BMS/protection, charger, panel,
fuse/protection, enclosure, ESP32 board, LCD/backpack, encoder, XBee carrier,
antenna, voltage domains, pullups, pin-risk, compatibility, power budget, and
current-limit evidence before any bench action.

Tasks 0218, 0219, 0220, and 0221 change only routing/status wording and
docs/records packets. Exact hardware identities, measurements, safety
evidence, pin readiness, RF readiness, firmware readiness, submodule commits,
parent pointer updates, and bench readiness remain open.

## Stop gates

No wiring, charging, radio writes, live RF transmit, framework selection,
firmware implementation, submodule runtime code, or live bench procedure is
authorized by this record.
