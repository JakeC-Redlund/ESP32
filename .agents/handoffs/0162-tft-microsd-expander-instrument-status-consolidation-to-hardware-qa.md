# Handoff 0162: TFT MicroSD Expander Instrument Status Consolidation To Hardware QA

Status: active

Task:
[Task 0225](../TASK_LOG/0225-tft-microsd-expander-instrument-status-consolidation.md)

Source coverage:
`SRC-LOCAL-TFT-MICROSD-EXPANDER-INSTRUMENT-STATUS-CONSOLIDATION-2026-06-06`,
`SRC-LCDWIKI-R61509V-MRB2802`,
`SRC-NOPNOP2002-ESP-IDF-PARALLEL-TFT`,
`SRC-ESP-IDF-FATFS`,
`SRC-ESP-IDF-SDSPI`,
`SRC-ESP-IDF-SDMMC`,
`SRC-ESP-IDF-SD-PULLUP`,
`SRC-ESP-IDF-SDSPI-EXAMPLE`,
`SRC-SD-ASSOCIATION-FORMATTER`,
`SRC-SD-ASSOCIATION-CAPACITY`,
`SRC-TI-TCA9555`,
`SRC-ESPRESSIF-MCP23017-COMPONENT`,
`SRC-FLUKE-87V`,
`SRC-KEYSIGHT-E36200`,
and `SRC-SALEAE-LOGIC-8`.

Evidence anchors include existing task and source-ledger paths:
`.agents/TASK_LOG/0005-admin-hmi-microsd-assets-logs.md`,
`.agents/TASK_LOG/0010-tft-relay-expansion.md`,
`knowledge-base/source-ledger/2026-05-18-spi-microsd-assets-logs.md`,
`knowledge-base/source-ledger/2026-05-18-tft-relay-expansion.md`, and
`knowledge-base/source-ledger/2026-05-19-four-relay-hardware-circuit-improvement.md`.

## Verified Facts

- The TFT, MicroSD, expander, storage, and instrument lane now has records-only
  candidate-profile lineage in the current plan/status records.
- Existing candidate sources and profiles do not select exact hardware or prove
  local bench state.
- Direct docs-index reachability now covers the new status record and the
  Open-Smart R61509V plus TCA9555/MCP23017 profile paths.

## Assumptions

- Hardware QA will continue by capturing source-backed or same-session physical
  evidence, not by inferring exact identity or readiness from candidate
  profiles.
- Firmware work remains downstream of exact hardware evidence and a separately
  accepted framework/implementation gate.

## Unknowns

- Exact TFT module, MicroSD reader/card, expander board, and local instrument
  identities remain open.
- Voltage rails, pullups, boot-pin interactions, address pins, shield
  continuity, card-prep workflow, local calibration/identity notes, and dummy
  load ratings remain open.
- Wiring, firmware dependencies, serial/RF/flash/live bench work, release,
  publication, commit, push, PR, and deploy remain unauthorized.

## Continue With

- Capture exact part markings, vendor/source links, photos/inspection notes,
  jumper/address states, voltage/pullup evidence, shield continuity, and
  boot-pin risk notes for each candidate lane.
- Create bench records for local instruments and fixtures before relying on
  DMM, current-limited supply, logic-analyzer, LED-proof, USB-serial, harness,
  or dummy-load evidence.
- Keep MicroSD card-prep records explicit about host-side formatting policy and
  normal-firmware no-auto-format expectations unless a later ADR changes them.

## Do Not Do

- Do not wire the TFT, MicroSD reader, expander, relay inputs, relay contacts,
  XBee carrier, load, mains, battery, or solar paths based on Task 0225 alone.
- Do not add firmware dependencies, select a new framework, flash, open serial,
  run RF/XBee actions, run live bench, release, publish, commit, push, open a
  PR, or deploy without a separately named gate and explicit authority.
