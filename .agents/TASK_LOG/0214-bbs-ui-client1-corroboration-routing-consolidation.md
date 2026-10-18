# Task 0214: BBS UI Client-1 Corroboration Routing Consolidation

Status: accepted-records-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source coverage:
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`,
`SRC-LOCAL-CBBS-REACT-NATIVE-CLIENT-PLATFORM-2026-06-02`, Task 0185, and
Task 0213.

## Routing

- Verified facts: Task 0213 is accepted as metadata-only Client-1
  screenshot/CV corroboration over the ignored Task 0185 host-only static
  browser evidence packet. Development plan/status/gap rows still had aggregate
  BBS UI wording that named only Client-1 static browser proof.
- Assumptions: aggregate routing records can be updated to name Task 0213
  without creating a new source ID, public source entry, app change, package
  change, lockfile change, live proof, native proof, hardware proof, firmware
  runtime proof, or release authority.
- Unknowns/open evidence: live browser/device behavior, selected-board
  identity, coordinator/router/end-node role, Client-2 Wi-Fi proof,
  dummy-output proof, native runtime, BLE, Web Serial, Web Bluetooth,
  SoftAP/LAN, runtime public API, ESP-IDF runtime proof, bridge ABI, serial
  ABI, Gate F service-code map, `mesh_discovery.v1` schema change, firmware
  runtime proof, hardware proof, release, and live gates remain unverified.
- Selected tier: Tier 2 records/status consolidation.
- Owner role: React Native Client/UI with QA/Evidence/AgentOps.
- Evidence need: Task 0185, Task 0213, the accepted metadata report, current
  plan/status/gap rows, read-only reviewer quorum output, scaffold audits,
  public leak checks, boundary scans, and diff checks.
- Mutation boundary: `research/development-plan.md`,
  `research/development-status-ledger.md`, `research/triage-status.md`,
  `research/known-gaps.md`, and this task record only.
- Reviewer quorum: read-only React Native Client/UI/Protocol reviewer weight 5
  and read-only QA/Evidence/AgentOps reviewer weight 3 approved with no P1/P2
  blockers, conditioned on naming Task 0213, preserving later-UI screenshot
  work, avoiding duplicate source/public/app changes, and keeping all
  live/native/hardware/release closures. Weighted reviewer vote: 8/8 approve
  with conditions; coordinator gate owner weight 5 accepted the same boundary.
- Agent lifecycle cleanup: no listable completed-agent inventory was available
  in the exposed toolset before spawn. Both spawned reviewers were waited,
  outputs captured, and closed before mutation.
- Gate authority: records/status consolidation only. No source index, source
  ledger, docs index, public docs, Pages tooling, app source, package file,
  lockfile, native project, runtime, firmware, hardware, release, serial, RF,
  flash, live bench, commit, push, PR, deploy, hook, config,
  `/etc/codex/requirements.toml`, or `admin-strict` authority is opened.

## Implementation

- Updated the BBS UI development-plan aggregate row and Client-1 milestone row
  to name accepted Task 0213 metadata-only Client-1 screenshot/CV
  corroboration.
- Updated the aggregate BBS UI status, triage, and known-gaps rows so they no
  longer imply Client-1 screenshot/CV corroboration is still pending.
- Preserved that remaining screenshot/OCR/CV work applies to later UI changes
  beyond Task 0213, and preserved all live/native/hardware/release closures.

## Validation

- Scoped `git diff --check -- research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md .agents/TASK_LOG/0214-bbs-ui-client1-corroboration-routing-consolidation.md`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 214`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py` passed:
  `PASS: ESP32 skill audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py`
  passed: `PASS: CBBS React Native scaffold audit succeeded`.
- Targeted routing scan confirmed Task 0213 and later-UI-beyond-Task-0213
  wording in the development plan, development status ledger, triage status,
  known gaps, and this task record.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py --out build/esp32-pages-routing.xAM8lj`
  passed and built 64 public files.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py build/esp32-pages-routing.xAM8lj/public-file-manifest.json`
  passed: `PASS: public manifest audit succeeded for build/esp32-pages-routing.xAM8lj/public-file-manifest.json`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py build/esp32-pages-routing.xAM8lj`
  passed: `PASS: GitHub Pages smoke succeeded for /mnt/h/esp32/build/esp32-pages-routing.xAM8lj`.
- Focused public leak scan for `0214-bbs-ui-client1-corroboration-routing-consolidation`,
  `Task 0214`, and Task-0214-specific routing tokens in
  `build/esp32-pages-routing.xAM8lj` returned no matches, as expected.
- Out-of-scope dirty-file check reported existing modifications outside this
  task boundary in `apps/cbbs-client/package.json`, `docs/index.md`,
  `knowledge-base/source-index.md`,
  `knowledge-base/source-ledger/2026-06-05-full-service-mesh-discovery-gate-m3-design-review.md`,
  and `pnpm-lock.yaml`; this task did not modify those paths.

## Authority Limits

This task does not authorize live browser/device connectivity, Client-2 Wi-Fi
proof, selected-board identity, coordinator/router/end-node role, dummy-output
proof, native runtime, BLE, Web Serial, Web Bluetooth, SoftAP/LAN, runtime
public API, ESP-IDF runtime proof, bridge ABI, serial ABI, Gate F service-code
map, `mesh_discovery.v1` schema change, Win31 transport change, hardware proof,
release, publication, flash, serial writes, RF writes, firmware runtime,
public Pages allowlist/tooling changes, package changes, app changes,
lockfile changes, source-index/source-ledger/docs-index changes, commit, push,
PR, deploy, hook changes, config changes, `/etc/codex/requirements.toml`, or
`admin-strict`.

## Handoff

No handoff required; this is a records-only routing consolidation.

## Decision

Decision: accept the records-only BBS UI Client-1 corroboration routing
consolidation. The aggregate plan/status/gap rows now name accepted Task 0213
metadata-only Client-1 screenshot/CV corroboration, while remaining
screenshot/OCR/CV work is limited to later UI changes beyond Task 0213 and all
live, native, hardware, firmware runtime, release, flash, serial, RF, source,
public, app, package, and lockfile authority remains closed.
