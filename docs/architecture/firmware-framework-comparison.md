# Firmware Framework Comparison

Status: comparison-recorded-selection-open

Source coverage:
`SRC-LOCAL-FIRMWARE-FRAMEWORK-COMPARISON-2026-06-07`,
`SRC-ESP-IDF-STABLE-ESP32`, `SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-START-PROJECT`, `SRC-ARDUINO-ESP32-CORE-2026-06-07`,
and `SRC-PLATFORMIO-ESPRESSIF32-2026-06-07`.

## Verified Facts

- `ADR-0001` keeps the workspace framework-neutral outside accepted project
  ADRs.
- `ADR-0002` accepts ESP-IDF stable v6.0.1 only for
  `four-relay-xbee-wifi`.
- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for `espnow-bbs`.
- ESP-IDF is Espressif's official development framework for ESP32-series SoCs.
- Arduino-ESP32 latest documentation reviewed on 2026-06-07 was for Arduino
  Core for ESP32 version 3.3.10 based on ESP-IDF 5.5.
- Arduino-ESP32 can also be used as an ESP-IDF component for advanced users,
  with the ESP-IDF toolchain installed.
- PlatformIO supports Espressif32 projects using `framework = espidf` or
  `framework = arduino`; its ESP-IDF framework integration uses
  PlatformIO-managed project structure and platform/package versions.

## Assumptions

- Future framework decisions will be project-local unless an accepted ADR
  explicitly changes workspace-wide framework status.
- The first comparison use case is deciding whether a project outside
  `ADR-0002` and `ADR-0003` should use ESP-IDF, Arduino-ESP32, PlatformIO with
  ESP-IDF, PlatformIO with Arduino, or remain framework-neutral.

## Unknowns

- Exact hardware identities, SoC variants, carrier boards, boot-pin risks, and
  voltage/isolation facts for projects without accepted framework ADRs.
- Project-specific needs for ESP-NOW, ESP-WIFI-MESH, BLE, UART, I2C, SPI,
  MicroSD, NVS, OTA, HTTP, local UI, host tests, CI, and release packaging.
- Whether any future project requires Arduino library compatibility or
  PlatformIO ecosystem features strongly enough to justify the added dependency
  and version-pin surface.

## Comparison Criteria

Any project-specific framework ADR must answer these questions before selecting
a framework:

| Criterion | Evidence required |
| --- | --- |
| Hardware identity | Exact board/module/carrier identity, SoC target, voltage rail, boot-pin, USB-UART, recovery, and isolation evidence. |
| Required APIs | Source-backed list of required Wi-Fi, ESP-NOW, BLE, UART, GPIO, I2C, SPI, storage, timer, task, and filesystem APIs. |
| Version pin | Exact framework/toolchain version and reason for pinning it. |
| Build shape | Local no-flash build command, generated-output location, and CI compatibility. |
| Dependency ownership | Official source, third-party source, package manager, component manager, library registry, or manual vendoring boundary. |
| Safety gates | Flash, monitor, serial/RF writes, relay/load/mains, live bench, release, and rollback gates kept separate from build proof. |
| Testability | Host tests, static scans, audit coverage, and no-live proof boundaries before first flash. |
| Operator ergonomics | Required IDE/CLI, setup complexity, reproducibility, and support route. |

## Framework Notes

ESP-IDF is the most direct Espressif-supported framework and is already
accepted for the two project-local lanes named in `ADR-0002` and `ADR-0003`.
New projects still need their own accepted ADR before adding ESP-IDF project
files, framework source, or CI matrix entries.

Arduino-ESP32 may be appropriate when a project is dominated by Arduino
library compatibility, sketch-style development, or operator familiarity. It is
not version-aligned with the current accepted ESP-IDF v6.0.1 lanes as of the
2026-06-07 source refresh, so any ADR that chooses Arduino-ESP32 must document
version compatibility and dependency ownership.

PlatformIO may be appropriate when a project needs its unified environment,
library tooling, or multi-framework workflows. PlatformIO is a toolchain layer
over ESP-IDF or Arduino choices, so an ADR must still name the underlying
framework, platform version, board target, project structure, and support
route.

## Stop Gates

This comparison does not select Arduino, ESP-IDF, PlatformIO, or any other
framework for projects outside accepted ADRs. It does not authorize firmware
source, framework project files, CI expansion for non-ADR projects, flash,
monitor, serial/RF/XBee writes, relay/load/mains work, live hardware, release,
publication, commit, push, PR, or deploy.
