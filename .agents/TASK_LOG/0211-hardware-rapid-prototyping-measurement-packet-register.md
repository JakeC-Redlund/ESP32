# Task 0211: Hardware Rapid Prototyping Measurement Packet Register

Status: accepted-records-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source coverage:
`SRC-LOCAL-HARDWARE-RAPID-PROTOTYPING-2026-05-28`,
`SRC-LOCAL-FOUR-RELAY-LOW-VOLTAGE-FIXTURE-KIT-2026-05-28`,
`SRC-NIOSH-SAFE-3D-PRINTING-2024-103`,
`SRC-OPENSCAD-DOCS-2026-05-28`,
`SRC-CADQUERY-DOCS-2026-05-28`,
`SRC-FREECAD-FEATURES-2026-05-28`,
`SRC-KICAD9-PCBNEW-3D-EXPORT-2026-05-28`

## Routing

- Verified facts: `docs/projects/hardware-rapid-prototyping/3d-printed-prototype-development.md`
  requires per-lane measurement packets before hardware rapid-prototyping
  guides or parts become repeatable. The four-relay low-voltage fixture kit
  workbook still records printer/scanner identity, material state, ventilation,
  calibration, dimensions, fit, and public package evidence as not recorded.
- Assumptions: adding an internal measurement packet register is records-only
  structure and does not create physical measurements, generated artifacts,
  print readiness, scan accuracy, fit acceptance, electrical acceptance, or
  hardware gate closure.
- Unknowns/open evidence: local printer/scanner identity, firmware, nozzle,
  build plate, calibration, safe operating location, filament/SDS/dry state,
  ventilation, local exhaust, PPE, calibration coupons, scan-to-caliper proof,
  lane dimensions, fit evidence, live print proof, live scan proof, electrical
  proof, relay/load/mains readiness, battery/solar readiness, and hardware
  acceptance remain unverified.
- Selected tier: Tier 2 records/evidence structure.
- Owner role: Hardware/Safety/Fabrication/CAD with QA/Evidence/AgentOps.
- Evidence need: existing source-indexed hardware rapid prototyping and
  fixture-kit records, current workbook gaps, read-only reviewer quorum output,
  public leak validation, this task record, and scaffold audit output.
- Mutation boundary:
  `research/hardware-rapid-prototyping/per-lane-measurement-packet-register.md`,
  `research/hardware-rapid-prototyping/four-relay-low-voltage-fixture-kit-workbook.md`,
  and this task record only.
- Reviewer quorum: read-only Hardware/Safety/Fabrication/CAD reviewer weight 5
  and read-only QA/Evidence/AgentOps reviewer weight 3 were spawned, waited,
  captured, and closed before mutation. Both approved with no P1/P2 blockers
  inside the named three-file records-only boundary, with conditions to keep all
  measurement evidence open and run public leak checks. Weighted reviewer vote:
  8/8 approve with conditions; coordinator gate owner weight 5 accepted the
  same boundary.
- Agent lifecycle cleanup: no listable completed-agent inventory was available
  in the exposed toolset before spawn. Both spawned reviewers were waited and
  closed before mutation.
- Gate authority: internal records-only register. No public Pages allowlist,
  source-index/source-ledger, docs-index, CAD generation, print, scan, fit,
  electrical, firmware, serial, RF/XBee, hardware, live bench, hook/config,
  release, publication, commit, push, PR, or deploy authority is opened.

## Implementation

- Added an internal per-lane measurement packet register under
  `research/hardware-rapid-prototyping/`.
- Added a workbook-only internal link to the register and noted that every
  physical measurement packet remains open.
- Kept every lane measurement and stop field as `Not recorded`, `Open`, or
  `Pending`.

## Validation Plan

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 211`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py`
- Focused public Pages leak check for `per-lane-measurement-packet-register`,
  `measurement packet register`, and
  `research/hardware-rapid-prototyping/per-lane-measurement-packet-register`.
- Changed-file source-ID scan.
- Changed-file Markdown link check.
- `git diff --check`

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 211`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py` passed:
  `PASS: ESP32 scaffold validation succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py` passed and
  built `build/github-pages` with 64 public files.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py` passed:
  `PASS: public manifest audit succeeded for /mnt/h/esp32/build/github-pages/public-file-manifest.json`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py` passed:
  `PASS: GitHub Pages smoke succeeded for /mnt/h/esp32/build/github-pages`.
- Focused public Pages leak scan
  `rg -n "per-lane-measurement-packet-register|measurement packet register|research/hardware-rapid-prototyping/per-lane-measurement-packet-register" build/github-pages`
  returned no matches, as expected.
- Changed-file source-ID scan confirmed the source IDs remain in
  `research/hardware-rapid-prototyping/per-lane-measurement-packet-register.md`
  and this task record.
- Changed-file Markdown link check over this task record, the register, and
  the fixture-kit workbook passed: `PASS: changed-file Markdown links resolve`.
- Scoped `git diff --check -- research/hardware-rapid-prototyping/per-lane-measurement-packet-register.md research/hardware-rapid-prototyping/four-relay-low-voltage-fixture-kit-workbook.md .agents/TASK_LOG/0211-hardware-rapid-prototyping-measurement-packet-register.md`
  passed with exit code 0.

## Authority Limits

This task does not authorize live printing, live scanning, fit acceptance,
electrical acceptance, wiring, flashing, monitor/serial writes, RF/XBee writes,
relay input work, relay contact work, load equipment, mains equipment, battery
charging, solar connections, firmware runtime changes, framework changes,
generated CAD/print artifacts, slicer projects, G-code, scan meshes, release,
publication, commit, push, PR, deploy, source-index/source-ledger creation,
docs-index updates, hook changes, config changes, `/etc/codex/requirements.toml`,
or `admin-strict`.

## Handoff

No handoff required; this is an internal records-only register.

## Decision

Decision: accept the internal hardware rapid prototyping per-lane measurement
packet register. The register structures future measurement evidence while all
physical, print, scan, fit, electrical, live, generated-artifact, public, and
release surfaces remain closed.
