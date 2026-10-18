# Firmware Framework Comparison Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-FIRMWARE-FRAMEWORK-COMPARISON-2026-06-07`

Related source IDs:
`SRC-ARDUINO-ESP32-CORE-2026-06-07`,
`SRC-PLATFORMIO-ESPRESSIF32-2026-06-07`,
`SRC-ESP-IDF-STABLE-ESP32`, `SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-START-PROJECT`,
`SRC-LOCAL-FIRMWARE-FRAMEWORK-CI-MATRIX-2026-06-07`, and
`SRC-LOCAL-DEVELOPMENT-PLAN-CONSOLIDATION-2026-05-27`.

## Verified Facts

- The workspace remains framework-neutral outside accepted project ADRs.
- `ADR-0002` accepts ESP-IDF stable v6.0.1 for `four-relay-xbee-wifi` only.
- `ADR-0003` accepts ESP-IDF stable v6.0.1 for `espnow-bbs` only.
- ESP-IDF is source-covered as Espressif's official ESP32-series development
  framework.
- Arduino-ESP32 latest documentation reviewed on 2026-06-07 covers Arduino
  Core for ESP32 version 3.3.10 based on ESP-IDF 5.5, plus an advanced
  Arduino-as-ESP-IDF-component path.
- PlatformIO source coverage reviewed on 2026-06-07 covers Espressif32 projects
  using `framework = espidf` or `framework = arduino`, with PlatformIO-managed
  platform/package versions and project structure.

## Evidence

- Comparison note:
  `docs/architecture/firmware-framework-comparison.md`
- Framework-neutral status:
  `knowledge-base/toolchain/framework-neutral-status.md`
- Task record:
  `.agents/TASK_LOG/0232-firmware-framework-comparison.md`
- Handoff:
  `.agents/handoffs/0169-firmware-framework-comparison-to-architecture-firmware-qa.md`
- Source-index row:
  `SRC-LOCAL-FIRMWARE-FRAMEWORK-COMPARISON-2026-06-07`
- Current status/gap records:
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`

## Validation

- Scaffold audits passed for `paths`, `sources`, `docs`, `firmware`,
  `records`, and `agent_process`.
- Durable-record audit passed for `--min-task-id 232`.
- Full `scripts/verify_scaffold.py` validation passed.
- Scoped `git diff --check` over the Task 0232 touched tracked files passed.

## Authority Limits

This source record does not accept or authorize Arduino, ESP-IDF, PlatformIO,
or any other framework selection for projects outside accepted ADRs. It does
not authorize firmware source, framework project files, CMake/sdkconfig/
platformio/Arduino project files, CI expansion for non-ADR projects, workflow
dispatch, artifact upload, deploy, release, publication, commit, push, PR,
firmware runtime, live hardware, serial/RF/XBee writes, flashing, monitor,
relay/load/mains work, `/etc/codex/requirements.toml`, or `admin-strict`
installation.

## Open Evidence

- Project-specific framework ADRs for any project outside `ADR-0002` and
  `ADR-0003`.
- Exact project hardware identity, required APIs, version pin, toolchain,
  build shape, dependency ownership, validation plan, and live-gate limits.
