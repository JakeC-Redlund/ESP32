# Task 0212: Hardware Measurement Register Status Consolidation

Status: accepted-records-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source coverage:
`SRC-LOCAL-HARDWARE-RAPID-PROTOTYPING-2026-05-28`,
`SRC-LOCAL-FOUR-RELAY-LOW-VOLTAGE-FIXTURE-KIT-2026-05-28`,
`SRC-NIOSH-SAFE-3D-PRINTING-2024-103`,
Task 0211 internal measurement register validation.

## Routing

- Verified facts: Task 0211 is accepted records-only and adds an internal
  per-lane measurement packet register under `research/`. The register states
  it is internal, not for public Pages allowlisting, and every physical
  measurement packet remains `Not recorded`, `Open`, or `Pending`.
- Assumptions: status/planning/gap records can mention the internal register
  without creating a new source ID, public link, physical measurement, print
  readiness, scan proof, fit acceptance, electrical acceptance, hardware
  acceptance, or live gate.
- Unknowns/open evidence: printer/scanner identity, material/SDS/dry state,
  ventilation, calibration coupons, lane dimensions, scan-to-caliper proof,
  CAD/export packet, print proof, scan proof, fit proof, electrical proof,
  power, voltage, boot-pin, isolation, battery/solar readiness,
  relay/load/mains readiness, and hardware acceptance remain unverified.
- Selected tier: Tier 2 records/status consolidation.
- Owner role: Hardware/Safety/Fabrication/CAD with QA/Evidence/AgentOps.
- Evidence need: Task 0211 validation, current plan/status/gap rows, read-only
  reviewer quorum output, targeted scans, public leak check, this task record,
  and scaffold audit output.
- Mutation boundary: `research/development-status-ledger.md`,
  `research/triage-status.md`, `research/development-plan.md`,
  `research/known-gaps.md`, and this task record only.
- Reviewer quorum: read-only Hardware/Safety/Fabrication/CAD reviewer weight 5
  and read-only QA/Evidence/AgentOps reviewer weight 3 were spawned, waited,
  captured, and closed before mutation. Both approved with no P1/P2 blockers
  inside the named records-only boundary, with conditions to keep all physical
  evidence open and preserve existing live/public/source-index closures.
  Weighted reviewer vote: 8/8 approve with conditions; coordinator gate owner
  weight 5 accepted the same boundary.
- Agent lifecycle cleanup: no listable completed-agent inventory was available
  in the exposed toolset before spawn. Both spawned reviewers were waited and
  closed before mutation.
- Gate authority: records/status consolidation only. No source-index,
  source-ledger, docs-index, public docs, public Pages allowlist/tooling, CAD,
  generated artifacts, print, scan, fit, electrical, firmware, serial, RF/XBee,
  hardware, live bench, hook/config, release, publication, commit, push, PR, or
  deploy authority is opened.

## Implementation

- Updated development status, triage, development plan, and known-gaps rows to
  mention Task 0211's internal open measurement-register structure.
- Preserved that all physical measurement packet evidence remains open/not
  recorded and that live print, live scan, fit, electrical, hardware, public,
  and release surfaces remain closed.

## Validation

- Scoped `git diff --check -- research/development-status-ledger.md research/triage-status.md research/development-plan.md research/known-gaps.md .agents/TASK_LOG/0212-hardware-measurement-register-status-consolidation.md`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 212`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py` passed:
  `PASS: ESP32 skill audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py` passed and
  built `build/github-pages` with 64 public files.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py` passed:
  `PASS: public manifest audit succeeded for /mnt/h/esp32/build/github-pages/public-file-manifest.json`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py` passed:
  `PASS: GitHub Pages smoke succeeded for /mnt/h/esp32/build/github-pages`.
- Focused public Pages leak scan
  `rg -n "per-lane-measurement-packet-register|measurement packet register|research/hardware-rapid-prototyping/per-lane-measurement-packet-register" build/github-pages`
  returned no matches, as expected.
- Targeted status-row scan confirmed Task 0211/internal measurement-register
  wording appears in the status/planning/gap rows and this task record.
- Out-of-scope dirty-file check
  `git diff --name-only -- docs/index.md knowledge-base/source-index.md knowledge-base/source-ledger`
  reported pre-existing dirty files outside this task boundary; this task did
  not modify those paths.

## Authority Limits

This task does not authorize source-index/source-ledger creation, docs-index
updates, public docs, public Pages allowlist/tooling changes, live printing,
live scanning, fit acceptance, electrical acceptance, wiring, flashing,
monitor/serial writes, RF/XBee writes, relay input work, relay contact work,
load equipment, mains equipment, battery charging, solar connections, firmware
runtime changes, framework changes, generated CAD/print artifacts, slicer
projects, G-code, scan meshes, release, publication, commit, push, PR, deploy,
hook changes, config changes, `/etc/codex/requirements.toml`, or
`admin-strict`.

## Handoff

No handoff required; this is a records-only status consolidation.

## Decision

Decision: accept the hardware measurement-register status consolidation. The
canonical status/planning/gap rows now acknowledge Task 0211's internal open
register while all physical measurement, print, scan, fit, electrical,
hardware, public, live, and release surfaces remain closed.
