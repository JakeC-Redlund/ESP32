# Hardware Rapid Prototyping Per-Lane Measurement Packet Register

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

Source coverage:
`SRC-LOCAL-HARDWARE-RAPID-PROTOTYPING-2026-05-28`,
`SRC-LOCAL-FOUR-RELAY-LOW-VOLTAGE-FIXTURE-KIT-2026-05-28`,
`SRC-NIOSH-SAFE-3D-PRINTING-2024-103`,
`SRC-OPENSCAD-DOCS-2026-05-28`,
`SRC-CADQUERY-DOCS-2026-05-28`,
`SRC-FREECAD-FEATURES-2026-05-28`,
`SRC-KICAD9-PCBNEW-3D-EXPORT-2026-05-28`

Status: Internal measurement register. Do not include this register in the
public Pages allowlist. Every packet below is open until same-session physical
or source-backed measurement evidence is recorded and reviewed.

## Verified Facts

- The hardware rapid prototyping program requires per-lane measurement packets
  before a guide, fixture, CAD model, print, scan, or fit decision becomes
  repeatable.
- The four-relay low-voltage fixture kit workbook still records printer,
  scanner, material, ventilation, calibration, dimensions, fit, and public
  package evidence as not recorded.
- This register adds no generated CAD, generated print artifact, slicer
  project, G-code, scan mesh, live print, live scan, wiring, flash, serial,
  RF/XBee, relay, load, mains, battery, solar, fit, or electrical acceptance.
- Task 0222 records structural validation and public-bundle exclusion status
  only; all physical measurement packets remain open.

## Assumptions

- Measurements will be captured from physical hardware, source-backed drawings,
  or reviewer-accepted evidence packets, not inferred from photos alone.
- Each lane can keep its measurement packet open while unrelated lanes continue
  to accumulate source-backed records.
- Public documentation remains limited to public-safe guide material and
  unresolved gaps.

## Unknowns

- Local printer/scanner identity, firmware, nozzle state, build plate,
  calibration state, operating location, and safe-use controls.
- Filament identity, SDS, dry state, humidity, storage, and selected material
  profile.
- Ventilation, local exhaust, enclosure, PPE, administrative control, and stop
  rules for the selected material and room.
- Board, radio, LCD, encoder, battery, BMS, charger, solar, relay, TFT,
  MicroSD, expander, connector, cable, antenna, heat, boot-pin, and isolation
  dimensions.

## Global Packet Rules

| Rule | Status | Notes |
| --- | --- | --- |
| No inferred dimensions | Open | Record method and source for every value. |
| No live print or scan acceptance | Open | A later gate must capture printer/scanner identity and controls first. |
| No electrical acceptance | Open | Power, voltage, boot-pin, and isolation evidence must be captured before bench actions. |
| No generated artifact publication | Open | Generated CAD/print/scan artifacts remain out of this register and out of the public Pages bundle. |
| No wiring or command authority | Open | Serial writes, RF/XBee writes, flashing, relay input work, load, mains, battery, and solar work remain closed. |

## Lane: ESP-NOW BBS Path

### Stop Fields

| Stop field | Required evidence | Status |
| --- | --- | --- |
| Power state | Exact powered/unpowered condition before any fit check. | Not recorded |
| Voltage domain | Board and fixture contact voltage limits. | Not recorded |
| Boot/reset/pin contact risk | Boot, reset, EN, IO0, USB, and button keep-out review. | Not recorded |
| Isolation/clearance | Nonconductive tray, airflow, cable strain, and connector clearance. | Not recorded |
| Serial writes | Proof that the fixture review is read-only and does not send serial bytes. | Not recorded |
| Radio/XBee writes | Proof that no radio configuration or RF write is involved. | Not recorded |
| Battery/solar | Proof that no battery or solar hardware is involved. | Not recorded |
| Relay/load/mains | Proof that no relay, load, or mains equipment is involved. | Not recorded |
| Generated artifacts | Export, slicer, G-code, and scan-mesh artifact policy. | Not recorded |

### Measurement Items

| Item | Dimension or clearance | Method | Source or record | Status |
| --- | --- | --- | --- | --- |
| Coordinator board | Outline, USB protrusion, boot/reset keep-out, cable bend radius | Not recorded | Not recorded | Open |
| Peer board | Outline, USB protrusion, antenna area, airflow clearance | Not recorded | Not recorded | Open |
| USB cable path | Bend radius, strain relief, tray exit, label zone | Not recorded | Not recorded | Open |
| Transport label | Text size, placement, no-obstruction check | Not recorded | Not recorded | Open |

## Lane: Four-Relay XBee And Low-Voltage Fixture

### Stop Fields

| Stop field | Required evidence | Status |
| --- | --- | --- |
| Power state | Exact powered/unpowered condition for board, relay module, XBee carrier, and instruments. | Not recorded |
| Voltage domain | Low-voltage-only domains and no-contact zones for relay contacts. | Not recorded |
| Boot/reset/pin contact risk | ESP32 boot/reset/button and service connector keep-outs. | Not recorded |
| Isolation/clearance | Relay terminal block, antenna, heat, connector, cable, and low-voltage separation review. | Not recorded |
| Serial writes | Proof that no serial bytes are sent during measurement capture. | Not recorded |
| Radio/XBee writes | Proof that no XBee setting writes, RF writes, or carrier wiring changes are involved. | Not recorded |
| Battery/solar | Proof that no battery or solar hardware is involved. | Not recorded |
| Relay/load/mains | Proof that relay inputs, relay contacts, load equipment, and mains remain untouched. | Not recorded |
| Generated artifacts | Export, slicer, G-code, and scan-mesh artifact policy. | Not recorded |

### Measurement Items

| Item | Dimension or clearance | Method | Source or record | Status |
| --- | --- | --- | --- | --- |
| Fixture plate | Overall length, width, thickness, corner radius, grid spacing | Not recorded | Not recorded | Open |
| Cable-tie slots | Slot width, slot length, tie width, bend radius, no-contact check | Not recorded | Not recorded | Open |
| ESP32 board | Outline, tallest component, USB protrusion, buttons, antenna, heat zones | Not recorded | Not recorded | Open |
| Relay module | Outline, terminal block keep-out, indicator clearance, low-voltage header clearance | Not recorded | Not recorded | Open |
| XBee carrier | Outline, DIN/DOUT access direction, antenna keep-out, connector clearance | Not recorded | Not recorded | Open |
| MicroSD reader | Outline, insertion direction, card overhang, connector clearance | Not recorded | Not recorded | Open |
| TFT module | Display outline, viewing area, ribbon/header clearance, bezel keep-out | Not recorded | Not recorded | Open |
| I2C expander | Outline, header clearance, address jumper access, pullup/voltage review gap | Not recorded | Not recorded | Open |

## Lane: Remote LCD XBee Solar Client

### Stop Fields

| Stop field | Required evidence | Status |
| --- | --- | --- |
| Power state | Exact powered/unpowered condition for LCD, ESP32, XBee, battery, BMS, charger, and solar parts. | Not recorded |
| Voltage domain | Logic, LCD backpack, XBee, cell, charger, BMS, and solar voltage/current limits. | Not recorded |
| Boot/reset/pin contact risk | ESP32 boot/reset pins, encoder contacts, LCD backpack pins, and service access. | Not recorded |
| Isolation/clearance | Antenna, cell, charger, fuse, enclosure, heat, and field-clearance review. | Not recorded |
| Serial writes | Proof that no serial bytes are sent during measurement capture. | Not recorded |
| Radio/XBee writes | Proof that no XBee settings or RF writes are involved. | Not recorded |
| Battery/solar | Cell condition, BMS/charger identity, fuse/protection, solar disconnect, and stop rules. | Not recorded |
| Relay/load/mains | Proof that no relay, load, or mains equipment is involved. | Not recorded |
| Generated artifacts | Export, slicer, G-code, and scan-mesh artifact policy. | Not recorded |

### Measurement Items

| Item | Dimension or clearance | Method | Source or record | Status |
| --- | --- | --- | --- | --- |
| 20x4 LCD and backpack | Viewing area, bezel, backpack height, I2C header, contrast access | Not recorded | Not recorded | Open |
| Rotary encoder | Shaft, knob, panel clearance, wire bend, switch access | Not recorded | Not recorded | Open |
| ESP32 board | Outline, antenna area, USB/service access, boot/reset keep-out | Not recorded | Not recorded | Open |
| XBee carrier | Outline, antenna area, connector access, enclosure clearance | Not recorded | Not recorded | Open |
| 18650 cell bay | Cell length, diameter, retention, insulation, service access | Not recorded | Not recorded | Open |
| BMS/protection board | Outline, heat, connector, fuse, and separation keep-outs | Not recorded | Not recorded | Open |
| Charger and solar lead | Module outline, cable strain, connector clearance, disconnect access | Not recorded | Not recorded | Open |

## Lane: TFT, MicroSD, Expander, And Storage

### Stop Fields

| Stop field | Required evidence | Status |
| --- | --- | --- |
| Power state | Exact powered/unpowered condition before any module fit check. | Not recorded |
| Voltage domain | TFT, MicroSD, expander, pullup, bus, and module voltage limits. | Not recorded |
| Boot/reset/pin contact risk | ESP32 boot-pin, reset, card insertion, and service connector conflict review. | Not recorded |
| Isolation/clearance | Ribbon, socket, jumper, connector, heat, and enclosure clearance. | Not recorded |
| Serial writes | Proof that no serial bytes are sent during measurement capture. | Not recorded |
| Radio/XBee writes | Proof that no radio or XBee settings are involved. | Not recorded |
| Battery/solar | Proof that no battery or solar hardware is involved. | Not recorded |
| Relay/load/mains | Proof that no relay, load, or mains equipment is involved. | Not recorded |
| Generated artifacts | Export, slicer, G-code, and scan-mesh artifact policy. | Not recorded |

### Measurement Items

| Item | Dimension or clearance | Method | Source or record | Status |
| --- | --- | --- | --- | --- |
| TFT module | Active area, PCB outline, ribbon/header, bezel, viewing angle | Not recorded | Not recorded | Open |
| MicroSD module | PCB outline, card insertion/removal envelope, latch/access clearance | Not recorded | Not recorded | Open |
| I2C/SPI expander | Board outline, jumpers, header height, pullup/voltage review gap | Not recorded | Not recorded | Open |
| Storage access door | Card reach, label visibility, no-obstruction check | Not recorded | Not recorded | Open |

## Lane: Future Browser And Client-Node Interface

### Stop Fields

| Stop field | Required evidence | Status |
| --- | --- | --- |
| Power state | Exact powered/unpowered condition for selected board, client fixture, and display device. | Not recorded |
| Voltage domain | Selected-board voltage, USB/power source, dummy-output, and isolation review. | Not recorded |
| Boot/reset/pin contact risk | Boot/reset/button, USB, antenna, and enclosure service-access keep-outs. | Not recorded |
| Isolation/clearance | Laptop/phone stand, QR/label holder, board tray, cable, airflow, and antenna clearance. | Not recorded |
| Serial writes | Proof that no serial bytes are sent during measurement capture. | Not recorded |
| Radio/XBee writes | Proof that no radio, SoftAP, Wi-Fi mutation, BLE, Web Serial, or XBee write is involved. | Not recorded |
| Battery/solar | Proof that no battery or solar hardware is involved unless a later gate opens it. | Not recorded |
| Relay/load/mains | Proof that no dummy output, relay, load, or mains equipment is involved. | Not recorded |
| Generated artifacts | Export, slicer, G-code, and scan-mesh artifact policy. | Not recorded |

### Measurement Items

| Item | Dimension or clearance | Method | Source or record | Status |
| --- | --- | --- | --- | --- |
| Selected client board tray | Board outline, USB access, antenna keep-out, boot/reset keep-out | Not recorded | Not recorded | Open |
| Phone or laptop stand | Device size range, viewing angle, cable clearance, QR/label visibility | Not recorded | Not recorded | Open |
| QR/label holder | Text size, placement, glare, no-obstruction check | Not recorded | Not recorded | Open |
| Dummy-output guard | Boundary marker and no-contact proof before any output concept is used | Not recorded | Not recorded | Open |

## Review Status

| Role | Required decision | Status |
| --- | --- | --- |
| Hardware | Lane measurements are recorded or unresolved gaps are explicit. | Pending |
| Safety/Power | Power, voltage, boot-pin, isolation, relay/load/mains, battery, solar, and stop gates are explicit. | Pending |
| Fabrication/CAD | CAD parameters use measured evidence and generated artifacts stay out of the repo. | Pending |
| QA/Evidence | Source IDs, link checks, public leak checks, and scaffold audits pass. | Accepted records-only in Task 0222 for current structural/public-exclusion validation; no physical acceptance. |

## Closed Surfaces

This register does not authorize live printing, live scanning, fit acceptance,
electrical acceptance, wiring, flashing, monitor/serial writes, RF/XBee writes,
relay input work, relay contact work, load equipment, mains equipment, battery
charging, solar connections, firmware runtime changes, framework changes,
generated CAD/print artifacts, slicer projects, G-code, scan meshes, release,
publication, commit, push, PR, or deploy.
