# Firmware Framework CI Matrix Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-FIRMWARE-FRAMEWORK-CI-MATRIX-2026-06-07`

Related source IDs:
`SRC-ESPRESSIF-ESP-IDF-CI-ACTION-2026-06-07`,
`SRC-ESP-IDF-STABLE-ESP32`, `SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-START-PROJECT`, and
`SRC-LOCAL-MODULAR-SCAFFOLD-AUDIT-CI-MATRIX-2026-06-07`.

## Verified Facts

- `ADR-0002` accepts ESP-IDF stable v6.0.1 for `four-relay-xbee-wifi` only.
- `ADR-0003` accepts ESP-IDF stable v6.0.1 for `espnow-bbs` only.
- The workspace remains framework-neutral outside accepted project ADRs.
- `firmware-ci.yml` defines a non-deploy build matrix for those two accepted
  project directories only.
- The workflow uses `espressif/esp-idf-ci-action@v1`, pins ESP-IDF `v6.0.1`,
  sets target `esp32`, and leaves the action default `idf.py build` command.
- Focused tests check the accepted-project matrix and closed flash/upload/
  deploy/release surfaces.

## Evidence

- Non-deploy firmware CI workflow:
  `.github/workflows/firmware-ci.yml`
- Focused workflow tests:
  `tests/scaffold_audits/test_scaffold_audit_reporting.py`
- Required-path audit guard:
  `scripts/scaffold_audit_data.py`
- Workflow documentation:
  `.github/workflows/README.md`
- Command matrix:
  `research/workspace-review-command-matrix.md`
- Task record:
  `.agents/TASK_LOG/0231-firmware-framework-ci-matrix.md`
- Handoff:
  `.agents/handoffs/0168-firmware-framework-ci-matrix-to-qa-tooling-firmware-release.md`
- Source-index row:
  `SRC-LOCAL-FIRMWARE-FRAMEWORK-CI-MATRIX-2026-06-07`

## Validation

- Focused workflow drift tests passed:
  `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest
  tests.scaffold_audits.test_scaffold_audit_reporting`.
- Scaffold audits passed for `paths`, `sources`, `docs`, `firmware`,
  `records`, and `agent_process`.
- Local no-flash ESP-IDF v6.0.1 build passed for
  `firmware/projects/four-relay-xbee-wifi` in
  `/tmp/esp32-four-relay-firmware-ci-matrix`; `four_relay_xbee_wifi.bin` size
  was `0x30660`, with `0xcf9a0` bytes (81%) free in the smallest app
  partition.
- Local no-flash ESP-IDF v6.0.1 build passed for
  `firmware/projects/espnow-bbs` in
  `/tmp/esp32-espnow-bbs-firmware-ci-matrix`; `espnow_bbs_build_only.bin` size
  was `0x29940`, with `0xd66c0` bytes (84%) free in the smallest app
  partition.
- Temporary build directories under `/tmp` were removed after build
  validation.
- Durable-record audit passed for `--min-task-id 231`.
- Full `scripts/verify_scaffold.py` validation passed.
- Full scaffold-audit unittest discovery passed with 142 tests.
- Scoped `git diff --check` over the Task 0231 touched tracked files passed.

## Authority Limits

This source record does not accept or authorize workflow dispatch, artifact
upload, GitHub Pages deploy, release, publication, commit, push, PR, global
firmware framework selection, framework selection for projects outside accepted
ADRs, firmware runtime, live hardware, serial/RF/XBee writes, flashing,
monitor, relay/load/mains work, `/etc/codex/requirements.toml`, or
`admin-strict` installation.

## Open Evidence

- Remote GitHub Actions run output for the new firmware workflow after future
  publication.
- Firmware CI coverage for any future project that receives its own accepted
  framework ADR.
