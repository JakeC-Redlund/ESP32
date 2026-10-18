# Soil Sensing Candidate Profiles

## Status

Candidate telemetry class planning profiles only. These records do not select a
local soil probe, adapter, logger, calibration method, wiring path, or ESP32
electrical interface.

## Verified Facts

- `SRC-METER-TEROS12-2026-05-25` lists METER TEROS 12 as a candidate source for
  soil moisture, temperature, electrical conductivity, SDI-12 communication,
  voltage, and data-acquisition compatibility fields.
- `SRC-SENTEK-DRILLDROP-2026-05-25` lists Sentek Drill and Drop as a candidate
  source for multi-depth soil moisture, temperature, optional salinity, RS232,
  RS485, SDI-12, and Modbus telemetry profile planning.
- `SRC-IRROMETER-SOIL-SENSORS-2026-05-25` lists IRROMETER soil sensors as a
  candidate source for soil water tension, WATERMARK 200SS, tensiometer ranges,
  transducer outputs, and SDI-12 or analog adapter integration planning.
- The source rows do not verify local sensor possession, exact probe series,
  wiring, logger choice, adapter choice, calibration, or ESP32 electrical
  compatibility.

## Assumptions

- Soil sensing should be split by measurement family before any implementation:
  volumetric water content, temperature, electrical conductivity/salinity,
  soil-water tension, and adapter/logger interface.
- SDI-12, RS232, RS485, Modbus, and analog-adapter labels are protocol-family
  planning labels only until exact hardware and source-backed frame behavior are
  accepted.

## Required Verification Checklist

Use the
[agricultural telemetry evidence-intake packet](../evidence-intake/README.md)
before any implementation gate.

| Item | Required evidence before implementation |
| --- | --- |
| Exact probe | Manufacturer, model, probe length/depth, measurement fields, datasheet/source link, and possession/inspection record. |
| Interface | SDI-12, RS232, RS485, Modbus, analog, or logger/API path with voltage levels, connector pinout, isolation, and adapter choice. |
| Power | Supply voltage/current, wake timing, cable length limits, transient exposure, and field enclosure constraints. |
| Calibration and units | Units, calibration procedure, soil/media constraints, temperature compensation, salinity/EC meaning, and maintenance interval. |
| ESP32 boundary | Boot-pin risks, level shifting, isolation, shared bus conflicts, surge protection, and read-only test plan. |

## Unknowns

- Exact local sensor models, adapter/logger path, cable and connector details,
  protocol frame behavior, calibration/units, environmental exposure, power
  budget, isolation, boot-pin impact, and live-proof plan.

## Stop Gates

No selected probe, wiring, adapter, protocol frame, telemetry schema, firmware
dependency, calibration acceptance, live measurement, release, publication,
commit, push, PR, or deploy is accepted by these candidate profiles.
