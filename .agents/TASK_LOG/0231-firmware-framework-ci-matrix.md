# Task 0231: Firmware Framework CI Matrix

Status: accepted

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-FIRMWARE-FRAMEWORK-CI-MATRIX-2026-06-07`,
`SRC-ESPRESSIF-ESP-IDF-CI-ACTION-2026-06-07`,
`SRC-ESP-IDF-STABLE-ESP32`, `SRC-ESP-IDF-GET-STARTED`,
`SRC-ESP-IDF-START-PROJECT`,
`SRC-LOCAL-MODULAR-SCAFFOLD-AUDIT-CI-MATRIX-2026-06-07`, and
`SRC-LOCAL-DEVELOPMENT-PLAN-CONSOLIDATION-2026-05-27`.

## Routing

- Verified facts: `ADR-0001` keeps the workspace framework-neutral outside
  accepted project ADRs. `ADR-0002` accepts ESP-IDF stable v6.0.1 for
  `four-relay-xbee-wifi` only, and `ADR-0003` accepts ESP-IDF stable v6.0.1
  for `espnow-bbs` only. Existing local records prove no-flash ESP-IDF
  v6.0.1 builds through `/home/cyber/.espressif/tools/activate_idf_v6.0.1.sh`.
  The official Espressif CI action supports project `path`, `esp_idf_version`,
  `target`, and defaults to `idf.py build`.
- Assumptions: A separate non-deploy firmware CI workflow over the accepted
  ESP-IDF project directories closes only the firmware CI matrix planning gap
  without changing firmware source, framework scope, release authority, or any
  live hardware gate.
- Unknowns/open evidence: remote GitHub Actions execution for the new workflow,
  Docker image pull availability during a future run, release/publication
  state, workflow dispatch, and any live hardware, serial, RF, flash,
  relay/load/mains, firmware-runtime, or runtime-behavior proof remain unproved
  or unauthorized.
- Selected tier: Tier 2 host-only CI, tooling, docs, and records update.
- Owner role: Tooling and Firmware QA, with Release/Records and Agent
  Operations review.
- Evidence need: required reading refresh, accepted ADR/source inspection,
  read-only reviewer quorum, official CI action source coverage, non-deploy
  workflow matrix over accepted project directories only, focused drift tests,
  required-path guard, workflow/docs/status/gap records, local no-flash builds
  using temporary build directories where available, scaffold audits,
  durable-record/source/docs/agent-process audits, and diff hygiene.
- Mutation boundary:
  `.github/workflows/firmware-ci.yml`, `.github/workflows/README.md`,
  `scripts/scaffold_audit_data.py`,
  `tests/scaffold_audits/test_scaffold_audit_reporting.py`,
  `research/workspace-review-command-matrix.md`,
  `.agents/TASK_LOG/0231-firmware-framework-ci-matrix.md`,
  `.agents/handoffs/0168-firmware-framework-ci-matrix-to-qa-tooling-firmware-release.md`,
  `knowledge-base/source-ledger/2026-06-07-firmware-framework-ci-matrix.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: focused workflow drift tests, path/source/docs/records/
  agent-process/firmware audits, local no-flash ESP-IDF builds for the accepted
  project directories when the activation script is present, full scaffold
  verification, generated-output cleanup, and scoped `git diff --check`.

## Reviewer Quorum

Read-only reviewer quorum was completed and closed before mutation:

- Tooling/QA reviewer `019e9fd2-c006-77e1-ad6c-49176fe4c5f5`, weight 3,
  approved with no P1 findings and two conditional P2 requirements: add drift
  tests for the exact project matrix, ESP-IDF version, target, and default
  build command; and keep the workflow read-only/non-deploy with no artifact,
  deploy, release, flash, monitor, serial, RF/XBee, or publication steps.
- Release/Records governance reviewer
  `019e9fd2-e9dc-7b93-a774-fc25122f0ec2`, weight 3, approved with no P1/P2
  blockers if acceptance keeps project-local ADR wording, closes only the
  firmware CI matrix gap, cites the CI action/toolchain sources, leaves remote
  Actions success unproved, and records temporary-build/generated-output
  cleanup.

Weighted reviewer result before mutation: 6/6 conditional approval. Conditions
were addressed by limiting the workflow matrix to the two accepted project ADR
targets, pinning ESP-IDF v6.0.1 and target `esp32`, leaving the action default
`idf.py build` command, adding focused workflow drift tests, adding source
coverage for the official Espressif CI action, and keeping all live/release
surfaces closed. No mutating worker agents were spawned. Visible lifecycle
state included the two reviewer IDs; both completed reviewers were waited and
closed before mutation. Under `bypassPermissions`, repo hooks remain advisory
and are not runtime cleanup proof.

## Implementation

- Added `.github/workflows/firmware-ci.yml` as a separate non-deploy
  pull-request/manual workflow with `permissions: contents: read`.
- Added a matrix over `four-relay-xbee-wifi`/`ADR-0002` and
  `espnow-bbs`/`ADR-0003`, both using `espressif/esp-idf-ci-action@v1`,
  `esp_idf_version: v6.0.1`, target `esp32`, and the action default
  `idf.py build` command.
- Added focused workflow tests that reject matrix drift, command overrides,
  flash/monitor commands, artifact upload, Pages deploy, and release steps.
- Added the firmware CI workflow to required-path auditing and documented the
  local no-flash ESP-IDF build command pattern as temp-mutating.
- Updated workflow docs, source records, docs index, development plan/status,
  triage, and known gaps.

## Validation

Passed host-only validation on 2026-06-07:

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest
  tests.scaffold_audits.test_scaffold_audit_reporting`:
  `Ran 6 tests in 1.136s`, `OK`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  paths`: `PASS: ESP32 scaffold audit paths succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  sources`: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  docs`: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  firmware`: `PASS: ESP32 scaffold audit firmware succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  records`: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit
  agent_process`: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `rm -rf /tmp/esp32-four-relay-firmware-ci-matrix && source
  /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C
  firmware/projects/four-relay-xbee-wifi -B
  /tmp/esp32-four-relay-firmware-ci-matrix build`: local no-flash build passed;
  `four_relay_xbee_wifi.bin` size `0x30660`, smallest app partition
  `0x100000`, `0xcf9a0` bytes (81%) free.
- `rm -rf /tmp/esp32-espnow-bbs-firmware-ci-matrix && source
  /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C
  firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-firmware-ci-matrix
  build`: local no-flash build passed; `espnow_bbs_build_only.bin` size
  `0x29940`, smallest app partition `0x100000`, `0xd66c0` bytes (84%) free.
- `rm -rf /tmp/esp32-four-relay-firmware-ci-matrix
  /tmp/esp32-espnow-bbs-firmware-ci-matrix`: temporary build directories
  removed.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py
  --min-task-id 231`: `PASS: ESP32 durable-record audit succeeded`.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3
  scripts/verify_scaffold.py`: `PASS: ESP32 scaffold validation succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover -s
  tests/scaffold_audits -p 'test_*.py'`: `Ran 142 tests in 118.377s`, `OK`.
- Scoped `git diff --check` over the Task 0231 touched tracked files passed
  with no output.

## Authority Limits

This task does not accept or authorize workflow dispatch, artifact upload,
GitHub Pages deploy, release, publication, commit, push, PR, global firmware
framework selection, framework selection for projects outside accepted ADRs,
firmware runtime, live hardware, serial/RF/XBee writes, flashing, monitor,
relay/load/mains work, `/etc/codex/requirements.toml`, or `admin-strict`
installation. Remote GitHub Actions execution remains unproved until a future
published workflow run exists.

## Handoff

Handoff:
[../handoffs/0168-firmware-framework-ci-matrix-to-qa-tooling-firmware-release.md](../handoffs/0168-firmware-framework-ci-matrix-to-qa-tooling-firmware-release.md)

## Decision

Decision: accepted for the host-only accepted-project firmware CI matrix. The
scope is limited to no-flash ESP-IDF builds for project-local accepted ADR
targets and does not change workspace framework neutrality. Remote GitHub
Actions execution, workflow dispatch, release/publication, flash, monitor,
serial/RF/XBee, runtime behavior, and live hardware proof remain unproved and
unauthorized.
