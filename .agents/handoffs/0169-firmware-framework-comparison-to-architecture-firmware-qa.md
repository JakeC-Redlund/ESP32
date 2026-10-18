# Handoff 0169: Firmware Framework Comparison To Architecture Firmware QA

Status: active

Task:
[Task 0232](../TASK_LOG/0232-firmware-framework-comparison.md)

Source coverage:
`SRC-LOCAL-FIRMWARE-FRAMEWORK-COMPARISON-2026-06-07`,
`SRC-ARDUINO-ESP32-CORE-2026-06-07`,
`SRC-PLATFORMIO-ESPRESSIF32-2026-06-07`,
`SRC-ESP-IDF-STABLE-ESP32`, and `SRC-ESP-IDF-GET-STARTED`.

## Verified Facts

- The workspace remains framework-neutral outside accepted project ADRs.
- `ADR-0002` accepts ESP-IDF v6.0.1 only for `four-relay-xbee-wifi`.
- `ADR-0003` accepts ESP-IDF v6.0.1 only for `espnow-bbs`.
- The comparison packet records source-backed criteria for future
  project-specific framework ADRs.
- Arduino-ESP32 and PlatformIO source coverage was added for comparison
  context only.

## Assumptions

- Future framework decisions will be project-specific unless an accepted ADR
  changes workspace-level framework status.
- The next useful framework action is a project-specific ADR packet naming the
  hardware identity, required APIs, version pin, toolchain, build shape, safety
  gates, dependency ownership, and validation plan.

## Unknowns

- Exact future project requirements and hardware identities remain unresolved.
- No Arduino, PlatformIO, or new ESP-IDF project outside accepted ADRs is
  selected.
- No remote workflow run, firmware runtime, live hardware, flash, monitor,
  serial/RF/XBee, relay/load/mains, release, publication, commit, push, PR, or
  deploy authority is present.

## Continue With

- Use [framework comparison](../../docs/architecture/firmware-framework-comparison.md)
  before drafting any new project-local framework ADR.
- Add project-specific sources and unresolved-gap notes before selecting a
  framework.
- Keep any future CI matrix entry tied to an accepted project ADR and focused
  drift tests.

## Do Not Do

- Do not treat Task 0232 as framework selection.
- Do not add framework source, CMake/sdkconfig/platformio/Arduino project
  files, firmware CI entries for non-ADR projects, flash, monitor, serial/RF
  writes, live hardware actions, release, publication, commit, push, PR, or
  deploy from this handoff.
