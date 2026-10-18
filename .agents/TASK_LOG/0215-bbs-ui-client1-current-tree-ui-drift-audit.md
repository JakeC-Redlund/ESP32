# Task 0215: BBS UI Client-1 Current-Tree UI Drift Audit

Status: accepted-records-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source coverage:
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`,
`SRC-LOCAL-CBBS-REACT-NATIVE-CLIENT-PLATFORM-2026-06-02`, ADR-0010,
Task 0185, Task 0213, and Task 0214.

## Routing

- Verified facts: Task 0185 accepted host-only Client-1 static browser proof.
  Task 0213 accepted metadata-only screenshot/CV corroboration over the ignored
  Task 0185 evidence packet. Current `git diff --name-only -- apps/cbbs-client
  package docs/projects/cbbs-react-native pnpm-lock.yaml` lists
  `apps/cbbs-client/package.json`,
  `docs/projects/cbbs-react-native/README.md`, and `pnpm-lock.yaml`. Current
  `git diff --quiet` over tracked Client-1 UI source/config files exits 0.
- Assumptions: a records-only current-tree audit can clarify that the present
  tree has no tracked Client-1 UI source drift beyond Task 0185/Task 0213 while
  preserving screenshot/OCR/CV as a future gate for later UI source changes.
- Unknowns/open evidence: live browser/device behavior, selected-board
  identity, coordinator/router/end-node role, Client-2 Wi-Fi proof,
  dummy-output proof, native runtime, BLE, Web Serial, Web Bluetooth,
  SoftAP/LAN, runtime public API, ESP-IDF runtime proof, bridge ABI, serial
  ABI, Gate F service-code map, `mesh_discovery.v1` schema change, firmware
  runtime proof, hardware proof, release, and live gates remain unverified.
- Selected tier: Tier 2 records/evidence consolidation.
- Owner role: React Native Client/UI with QA/Evidence/AgentOps.
- Evidence need: current git diff/status checks, Task 0185, Task 0213,
  Task 0214, ADR-0010, React Native project docs, read-only reviewer quorum
  output, scaffold audits, public leak checks, boundary scans, and diff checks.
- Mutation boundary: `research/bbs-ui-client1-current-tree-drift-audit/`,
  this task record, `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`, and
  `research/known-gaps.md` only.
- Reviewer quorum: read-only React Native Client/UI/Protocol reviewer
  `019e9e76-60cd-7a72-bc24-1fb6023d1fb3` weight 5 and read-only
  QA/Evidence/AgentOps reviewer `019e9e76-8625-7f63-b90f-e65feeea6051`
  weight 3 approved with no P1/P2 blockers, conditioned on precise drift
  wording, separating package/lockfile dependency alignment from UI source
  drift, preserving future screenshot/OCR/CV gates, and avoiding app/package/
  lockfile/source/public/native/runtime/firmware/hardware/live/release changes.
  Weighted reviewer vote: 8/8 approve with conditions; coordinator gate owner
  weight 5 accepted the same records-only boundary.
- Agent lifecycle cleanup: completed reviewers were waited, outputs captured,
  and both reviewer handles were closed before mutation.
- Gate authority: records/status consolidation only. No source index, source
  ledger, docs index, public docs, Pages tooling, app source, package file,
  lockfile, native project, runtime, firmware, hardware, release, serial, RF,
  flash, live bench, commit, push, PR, deploy, hook, config,
  `/etc/codex/requirements.toml`, or `admin-strict` authority is opened.

## Implementation

- Added an internal metadata-only current-tree audit report for Client-1 UI
  drift.
- Updated aggregate BBS UI plan/status/gap wording to record that Task 0215
  finds no current tracked Client-1 UI source drift beyond Task 0185/Task 0213.
- Preserved that future later UI source changes still need copied
  screenshot/OCR/CV corroboration, and preserved all live/native/hardware/
  firmware-runtime/release closures.

## Validation

- Scoped `git diff --check -- research/bbs-ui-client1-current-tree-drift-audit .agents/TASK_LOG/0215-bbs-ui-client1-current-tree-ui-drift-audit.md research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 215`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py`
  passed: `PASS: CBBS React Native scaffold audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py` passed:
  `PASS: ESP32 skill audit succeeded`.
- `git status --short -- apps/cbbs-client pnpm-lock.yaml` returned only the
  pre-existing Task 0185 dependency-alignment drift:
  `apps/cbbs-client/package.json` and `pnpm-lock.yaml`.
- `git diff --name-only -- apps/cbbs-client | rg -v '^apps/cbbs-client/package\.json$' || true`
  returned no output.
- `find apps/cbbs-client -maxdepth 3 -type d \( -name android -o -name ios -o -name windows \) -print`
  returned no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py --out build/esp32-pages-client1-drift.p8cjwC`
  passed and built 64 public files.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py build/esp32-pages-client1-drift.p8cjwC/public-file-manifest.json`
  passed: `PASS: public manifest audit succeeded for build/esp32-pages-client1-drift.p8cjwC/public-file-manifest.json`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py build/esp32-pages-client1-drift.p8cjwC`
  passed: `PASS: GitHub Pages smoke succeeded for /mnt/h/esp32/build/esp32-pages-client1-drift.p8cjwC`.
- Focused public leak scan for `bbs-ui-client1-current-tree-drift-audit`,
  `0215-bbs-ui-client1-current-tree-ui-drift-audit`, `Task 0215`,
  `current-tree-ui-drift`, and `current tracked Client-1 UI source drift`
  returned no matches. The temporary Pages build directory was removed after
  validation.

## Authority Limits

This task does not authorize fresh screenshot/OCR/CV proof, fresh browser
proof, live browser/device connectivity, Client-2 Wi-Fi proof, selected-board
identity, coordinator/router/end-node role, dummy-output proof, native runtime,
BLE, Web Serial, Web Bluetooth, SoftAP/LAN, runtime public API, ESP-IDF runtime
proof, bridge ABI, serial ABI, Gate F service-code map, `mesh_discovery.v1`
schema change, Win31 transport change, hardware proof, release, publication,
flash, serial writes, RF writes, firmware runtime, public Pages allowlist/
tooling changes, package changes, app changes, lockfile changes, source-index/
source-ledger/docs-index changes, commit, push, PR, deploy, hook changes,
config changes, `/etc/codex/requirements.toml`, or `admin-strict`.

## Handoff

No handoff required; this is a records-only current-tree audit.

## Decision

Decision: accept only the records-only finding that there is no current tracked
Client-1 UI source drift beyond Task 0185/Task 0213, while existing
package/lockfile dependency alignment drift remains separate and all future UI
source changes still need their own screenshot/OCR/CV corroboration.
