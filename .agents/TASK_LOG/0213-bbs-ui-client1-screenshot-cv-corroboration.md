# Task 0213: BBS UI Client-1 Screenshot/CV Corroboration

Status: accepted-metadata-only-corroboration

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source coverage:
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`,
`SRC-LOCAL-CBBS-REACT-NATIVE-CLIENT-PLATFORM-2026-06-02`, and ADR-0010.

## Routing

- Verified facts: Task 0185 accepted host-only Client-1 static browser proof
  for the CBBS browser fixture. The ignored evidence packet includes
  desktop/mobile screenshots, accessibility snapshots, loopback-only network
  data, favicon-only console 404, bundle inventory, and server cleanup
  evidence.
- Assumptions: screenshot pixel metadata and accessibility snapshot checks can
  corroborate Task 0185 without opening a live/browser/device/native/hardware
  gate and without modifying app source or package metadata.
- Unknowns/open evidence: live browser/device behavior, selected-board
  identity, coordinator/router/end-node role, Client-2 Wi-Fi proof,
  dummy-output proof, native runtime, BLE, Web Serial, Web Bluetooth,
  SoftAP/LAN, runtime public API, ESP-IDF runtime proof, bridge ABI, serial
  ABI, Gate F service-code map, `mesh_discovery.v1` schema change, firmware
  runtime proof, hardware proof, release, and live gates remain unverified.
- Selected tier: Tier 2 records/evidence corroboration.
- Owner role: React Native Client/UI with QA/Evidence/AgentOps.
- Evidence need: ignored Task 0185 evidence metadata, source coverage, reviewer
  quorum output, this task record, scaffold audits, public leak checks, and
  diff checks.
- Mutation boundary: internal metadata report under
  `research/bbs-ui-client1-screenshot-corroboration/`, this task record, and
  narrow status/gap-routing wording only.
- Reviewer quorum: read-only React Native Client/UI/Protocol reviewer weight 5
  and read-only QA/Evidence/AgentOps reviewer weight 3 approved with no P1/P2
  blockers, conditioned on keeping the record metadata-only, using the existing
  source ID, and preserving all live/native/hardware/release closures. Weighted
  reviewer vote: 8/8 approve with conditions; coordinator gate owner weight 5
  accepted the same boundary.
- Agent lifecycle cleanup: no listable completed-agent inventory was available
  in the exposed toolset before spawn. The React Native reviewer was waited and
  closed before compaction. The QA reviewer completed and its output was
  captured; after compaction, `close_agent` was attempted for its agent ID but
  the runtime no longer exposed that handle, so the close attempt returned
  `agent ... not found`.
- Gate authority: metadata-only records/evidence corroboration. No app source,
  package files, lockfiles, source index, source ledger, docs index, public
  docs, Pages tooling, native project, runtime, hardware, release, firmware,
  serial, RF, flash, live bench, commit, push, PR, deploy, hook, config,
  `/etc/codex/requirements.toml`, or `admin-strict` authority is opened.

## Implementation

- Added an internal metadata-only corroboration report for the existing ignored
  Task 0185 Client-1 static browser evidence packet.
- Recorded screenshot dimensions, hashes, pixel distribution checks,
  accessibility snapshot coverage, and browser log boundaries without tracking
  raw screenshots, raw logs, dist output, base64 payloads, or bulky artifacts.
- Added narrow status/gap-routing entries for Client-1 screenshot/CV
  corroboration while preserving live/native/hardware/release closures.

## Validation

- Scoped `git diff --check -- research/bbs-ui-client1-screenshot-corroboration/client1-static-browser-screenshot-cv-corroboration.md .agents/TASK_LOG/0213-bbs-ui-client1-screenshot-cv-corroboration.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 213`
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
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py --out build/esp32-pages-corroboration.upp1I1`
  passed and built 64 public files.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py build/esp32-pages-corroboration.upp1I1/public-file-manifest.json`
  passed: `PASS: public manifest audit succeeded for build/esp32-pages-corroboration.upp1I1/public-file-manifest.json`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py build/esp32-pages-corroboration.upp1I1`
  passed: `PASS: GitHub Pages smoke succeeded for /mnt/h/esp32/build/esp32-pages-corroboration.upp1I1`.
- Focused public leak scan for the new internal report path, report basename,
  and screenshot hashes in `build/esp32-pages-corroboration.upp1I1` returned
  no matches, as expected.
- Broader public scan including the ignored evidence-packet path found only the
  pre-existing source-index row for
  `SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`; it did not
  expose the new internal report path, report basename, or screenshot hashes.
- `git check-ignore -v` confirmed the desktop/mobile screenshots and
  accessibility snapshots remain ignored by
  `.gitignore:37:research/bench-records/**/local-*`.
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

No handoff required; this is a metadata-only corroboration of existing
Client-1 static browser evidence.

## Decision

Decision: accept the Client-1 screenshot/CV corroboration as metadata-only
evidence over the already accepted Task 0185 host-only static browser proof.
All live, native, hardware, firmware runtime, release, flash, serial, RF, and
public authority surfaces remain closed.
