# Client-1 Current-Tree UI Drift Audit

Status: accepted-records-only

Date: 2026-06-06

Source coverage:
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`,
`SRC-LOCAL-CBBS-REACT-NATIVE-CLIENT-PLATFORM-2026-06-02`, ADR-0010,
Task 0185, Task 0213, and Task 0214.

This record is metadata-only. It does not track screenshots, OCR output, CV
output, browser logs, bundles, native projects, raw evidence artifacts, app
source changes, package changes, lockfile changes, firmware changes, or live
bench evidence.

## Routing

- Verified facts: Task 0185 accepted host-only Client-1 static browser proof.
  Task 0213 accepted metadata-only screenshot/CV corroboration over the ignored
  Task 0185 evidence packet. Current tracked Client-1 UI source/config files
  have no diff. Current dirty Client-1 boundary files are dependency alignment
  files, not UI source files.
- Assumptions: a records-only current-tree audit can remove the present
  "later UI changes" ambiguity without rerunning browser/device proof because
  no tracked Client-1 UI source drift is visible in the current tree.
- Unknowns/open evidence: live browser/device behavior, selected-board
  identity, coordinator/router/end-node role, Client-2 Wi-Fi proof,
  dummy-output proof, native runtime, BLE, Web Serial, Web Bluetooth,
  SoftAP/LAN, runtime public API, ESP-IDF runtime proof, bridge ABI, serial
  ABI, Gate F service-code map, `mesh_discovery.v1` schema change, firmware
  runtime proof, hardware proof, release, future UI-change screenshot/OCR/CV
  proof, and live gates remain unverified.
- Selected tier: Tier 2 records/evidence consolidation.
- Owner role: React Native Client/UI with QA/Evidence/AgentOps.
- Evidence need: current git diff/status checks, ADR-0010, React Native project
  docs, Task 0185, Task 0213, Task 0214, reviewer quorum output, task record,
  scaffold audits, public leak checks, and boundary scans.
- Mutation boundary: this internal report, Task 0215, and narrow
  BBS UI status/gap wording only. No app source, package file, lockfile,
  source index, source ledger, docs index, public docs, Pages tooling, native
  project, runtime, firmware, hardware, release, serial, RF, flash, or live
  mutation.

## Current-Tree Checks

| Check | Result |
| --- | --- |
| `git diff --name-only -- apps/cbbs-client package docs/projects/cbbs-react-native pnpm-lock.yaml` | `apps/cbbs-client/package.json`, `docs/projects/cbbs-react-native/README.md`, and `pnpm-lock.yaml`. |
| `git diff --quiet -- apps/cbbs-client/app.json apps/cbbs-client/app/_layout.tsx apps/cbbs-client/app/index.tsx apps/cbbs-client/src/README.md apps/cbbs-client/tsconfig.json` | Exit code 0. No tracked diff in these Client-1 UI source/config files. |
| `git diff --numstat -- apps/cbbs-client/app.json apps/cbbs-client/app/_layout.tsx apps/cbbs-client/app/index.tsx apps/cbbs-client/src/README.md apps/cbbs-client/tsconfig.json apps/cbbs-client/package.json pnpm-lock.yaml docs/projects/cbbs-react-native/README.md` | `3 2 apps/cbbs-client/package.json`, `12 0 docs/projects/cbbs-react-native/README.md`, and `133 130 pnpm-lock.yaml`. |
| `git ls-files apps/cbbs-client` | Tracked files are `apps/cbbs-client/app.json`, `apps/cbbs-client/app/_layout.tsx`, `apps/cbbs-client/app/index.tsx`, `apps/cbbs-client/package.json`, `apps/cbbs-client/src/README.md`, and `apps/cbbs-client/tsconfig.json`. |
| `find apps/cbbs-client -maxdepth 3 -type d \( -name android -o -name ios -o -name windows \) -print` | No output. No visible native `android`, `ios`, or `windows` directory exists under `apps/cbbs-client`. |

## Drift Decision

Task 0215 records no current tracked Client-1 UI source drift beyond the
accepted Task 0185 static browser proof and Task 0213 metadata-only
screenshot/CV corroboration. The existing dirty `apps/cbbs-client/package.json`
and `pnpm-lock.yaml` entries are the Task 0185 SDK-56 dependency alignment
surface, not UI source drift. The dirty
`docs/projects/cbbs-react-native/README.md` entry is documentation alignment
wording, not app UI source drift.

## Not Accepted

This audit does not provide fresh screenshot/OCR/CV proof, fresh browser proof,
live browser/device proof, selected-board identity, Client-2 Wi-Fi proof,
dummy-output proof, native runtime proof, BLE, Web Serial, Web Bluetooth,
SoftAP/LAN proof, runtime public API proof, ESP-IDF runtime proof, firmware
runtime proof, bridge ABI or serial ABI expansion, Gate F service-code changes,
`mesh_discovery.v1` schema changes, Win31 transport changes, hardware proof,
release, publication, commit, push, PR, deploy, flash, serial write, RF write,
or any live gate.

Future tracked UI source changes after Task 0215 still require copied
screenshot/OCR/CV corroboration and whatever separate live or native gates are
needed for the claim being made.

## Validation

- Scoped `git diff --check -- research/bbs-ui-client1-current-tree-drift-audit .agents/TASK_LOG/0215-bbs-ui-client1-current-tree-ui-drift-audit.md research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 215`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py`
  passed: `PASS: CBBS React Native scaffold audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py`,
  `scripts/scaffold_audit_sources.py`, `scripts/scaffold_audit_docs.py`, and
  `scripts/scaffold_audit_data.py` passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py` passed:
  `PASS: ESP32 skill audit succeeded`.
- Boundary scans passed: `git status --short -- apps/cbbs-client pnpm-lock.yaml`
  returned only `apps/cbbs-client/package.json` and `pnpm-lock.yaml`;
  `git diff --name-only -- apps/cbbs-client | rg -v '^apps/cbbs-client/package\.json$' || true`
  returned no output; native-directory scan returned no output.
- Pages build, manifest audit, and smoke test passed for
  `build/esp32-pages-client1-drift.p8cjwC`; focused public leak scan for
  Task 0215/internal-report terms returned no matches, and the temporary build
  directory was removed.
