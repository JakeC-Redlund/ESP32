# Handoff 0168: Firmware Framework CI Matrix To QA Tooling Firmware Release

Status: active

Task:
[Task 0231](../TASK_LOG/0231-firmware-framework-ci-matrix.md)

Source coverage:
`SRC-LOCAL-FIRMWARE-FRAMEWORK-CI-MATRIX-2026-06-07`,
`SRC-ESPRESSIF-ESP-IDF-CI-ACTION-2026-06-07`,
`SRC-ESP-IDF-STABLE-ESP32`, `SRC-ESP-IDF-START-PROJECT`, and
`SRC-LOCAL-MODULAR-SCAFFOLD-AUDIT-CI-MATRIX-2026-06-07`.

## Verified Facts

- `firmware-ci.yml` is a non-deploy PR/manual workflow with read-only contents
  permission.
- The workflow matrix includes only accepted project-local ESP-IDF targets:
  `four-relay-xbee-wifi` under `ADR-0002` and `espnow-bbs` under `ADR-0003`.
- The workflow uses `espressif/esp-idf-ci-action@v1`, pins
  `esp_idf_version: v6.0.1`, and builds target `esp32` with the action default
  `idf.py build` command.
- Focused tests check the workflow matrix, version, target, and closed
  flash/upload/deploy/release surfaces.

## Assumptions

- Future accepted ESP-IDF project targets will update the workflow matrix,
  focused drift test, source records, and project ADR/source coverage together.
- QA will keep this workflow separate from Pages deploy and release workflows.

## Unknowns

- Remote GitHub Actions execution for the new workflow is not proven in this
  local task.
- Docker image pull availability and future runner state remain unproved until
  a published workflow run exists.
- Workflow dispatch, release, publication, commit, push, PR, deploy, live
  hardware, firmware runtime, flash, monitor, serial, RF/XBee, relay/load, and
  mains behavior remain closed or open as separately recorded.

## Continue With

- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_scaffold_audit_reporting`
  after workflow matrix edits.
- Keep `.github/workflows/firmware-ci.yml` aligned with accepted project ADRs
  and the focused drift test.
- Treat any future remote workflow run result as a new evidence record if it is
  used for acceptance.

## Do Not Do

- Do not treat Task 0231 as workflow dispatch, artifact upload, deploy, release,
  publication, commit, push, PR, global framework-selection, firmware-runtime,
  live hardware, serial/RF/XBee write, flash/monitor, relay/load/mains,
  `/etc/codex`, or `admin-strict` authority.
