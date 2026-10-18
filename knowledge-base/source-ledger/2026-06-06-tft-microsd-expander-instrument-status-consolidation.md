# TFT MicroSD Expander Instrument Status Consolidation Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-TFT-MICROSD-EXPANDER-INSTRUMENT-STATUS-CONSOLIDATION-2026-06-06`

## Verified Facts

- Existing records already describe the Open-Smart R61509V TFT profile as a
  planning target with exact module identity and bench evidence unverified.
- Existing records already describe the SPI MicroSD reader profile as a generic
  reader/card planning branch with no approved reader, wiring, shield header,
  firmware implementation, or card-formatting gate.
- Existing records already describe the TCA9555/MCP23017 expander profile as a
  planning-only relay-output-expansion branch that still requires exact
  expander board, address, pullup, inactive default, latch/readback, and driver
  stage evidence before relay use.
- Existing hardware-circuit records describe Fluke 87V, Keysight E36200-series,
  and Saleae Logic 8 sources as candidate instrument-class examples only.
- Task 0225 consolidates the above as records-only candidate lineage and adds
  direct documentation-index reachability for the status record and candidate
  profile paths.

## Evidence

- Task record:
  `.agents/TASK_LOG/0225-tft-microsd-expander-instrument-status-consolidation.md`
- Handoff:
  `.agents/handoffs/0162-tft-microsd-expander-instrument-status-consolidation-to-hardware-qa.md`
- Source-index row:
  `SRC-LOCAL-TFT-MICROSD-EXPANDER-INSTRUMENT-STATUS-CONSOLIDATION-2026-06-06`
- Existing task records: `.agents/TASK_LOG/0005-admin-hmi-microsd-assets-logs.md`
  and `.agents/TASK_LOG/0010-tft-relay-expansion.md`
- Existing source ledgers:
  `knowledge-base/source-ledger/2026-05-18-spi-microsd-assets-logs.md`,
  `knowledge-base/source-ledger/2026-05-18-tft-relay-expansion.md`, and
  `knowledge-base/source-ledger/2026-05-19-four-relay-hardware-circuit-improvement.md`
- Existing candidate profiles:
  `hardware-profiles/displays/open-smart-r61509v/README.md`,
  `hardware-profiles/storage/spi-microsd-reader/README.md`, and
  `hardware-profiles/interface-expansion/tca9555-mcp23017/README.md`
- Same-session read-only reviewer quorum: Architecture/Hardware/Firmware/
  Communications reviewer weight 5 and QA/Evidence/AgentOps reviewer weight 3
  approved the named records-only boundary with no P1/P2 blockers.

## Authority Limits

This source record does not accept exact module, card, board, or instrument
identity. It does not select hardware, approve wiring, add firmware
dependencies, select a framework, open serial/RF/XBee, authorize flash, open
live bench, prove voltage/pullup/boot-pin/address/card-prep evidence, approve
release or publication, or authorize commit, push, PR, or deploy.

## Open Evidence

- Exact Open-Smart R61509V module identity, pinout, power/backlight, touch
  interface, driver path, and shared pin-budget impact.
- Exact MicroSD reader/card identity, 3.3 V path, pullups, card-detect/
  write-protect behavior, shield continuity, card capacity, FAT preparation,
  low-space handling, rotation, and fallback behavior.
- Exact TCA9555/MCP23017 board identity, I2C address pins, pullups, inactive
  default, latch/readback behavior, output-current limits, and driver-stage
  boundary.
- Local bench instrument inventory, calibration/identity notes, current-limited
  supply availability, logic analyzer or LED proof fixture, USB serial tools,
  labeled harnesses, and low-voltage dummy-load ratings.
