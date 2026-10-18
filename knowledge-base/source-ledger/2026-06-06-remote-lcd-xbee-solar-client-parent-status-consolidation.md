# Remote LCD XBee Solar Client Parent Status Consolidation Source Ledger

Date: 2026-06-06

Source ID:
`SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-IDENTITY-WORKTREE-CONSOLIDATION-2026-06-06`

## Scope

This ledger records a parent ESP32 records/status consolidation after all seven
private `rlxsc-*` submodule worktrees gained `docs/identity-intake.md` packet
files in the current same-session worktree.

This ledger does not claim committed submodule state, parent-pinned submodule
state, hardware identity closure, selected parts, measurements, bench
readiness, live proof, release, publication, commit, push, PR, or deploy.

## Evidence Reviewed

- `submodules/hardware/rlxsc-18650-cell/docs/identity-intake.md`
- `submodules/hardware/rlxsc-bms-protection/docs/identity-intake.md`
- `submodules/hardware/rlxsc-solar-charger-power-path/docs/identity-intake.md`
- `submodules/hardware/rlxsc-esp32-client-node/docs/identity-intake.md`
- `submodules/hardware/rlxsc-lcd-20x4-i2c/docs/identity-intake.md`
- `submodules/hardware/rlxsc-rotary-encoder/docs/identity-intake.md`
- `submodules/hardware/rlxsc-xbee-pro-s3b/docs/identity-intake.md`
- Current `git submodule status --recursive submodules/hardware`.
- Parent remote-client records, status ledgers, and Task 0183/Handoff 0131.

## Verified Facts

- Seven private `rlxsc-*` hardware submodules exist under
  `submodules/hardware/`.
- Each of the seven submodule worktrees contains a current
  `docs/identity-intake.md` packet.
- The identity-intake packet files and related submodule docs/records are dirty
  worktree evidence. They are not accepted here as committed submodule state or
  parent-pinned submodule state.
- Parent status records before this ledger still routed the next action as
  starting submodule identity intake.

## Unknowns

- Exact ESP32 board, LCD/backpack, encoder, 18650 cell, BMS/protection board,
  solar panel, charger/power path, XBee carrier, antenna, fuse/protection,
  enclosure, power budget, pin map, framework ADR, read-only bench proof, and
  live proof remain unresolved.
- Submodule-local review, tracking, commits, and parent pointer updates remain
  separate future gates.

## Stop Gates

This ledger does not authorize submodule commits, parent submodule pointer
updates, hardware selection, wiring, charging, battery pack assembly, solar
connection, ESP32 GPIO attachment, XBee serial/RF actions, firmware/framework
work, live measurement, bench bring-up, flash, monitor, release, publication,
commit, push, PR, or deploy.

## Validation

Validation completed on 2026-06-06:

- All seven identity-intake packet paths exist in current private submodule
  worktrees.
- Each packet's source ID resolves in its submodule-local source index.
- Per-submodule `git diff --check`, parent records/source/docs/path audits,
  public docs build/audit/smoke, bulky artifact scan, focused wording scans,
  and parent `git diff --check --submodule` passed.
- `git submodule status --recursive submodules/hardware` showed the
  parent-pinned submodule commits unchanged.
