# Task 0218: Remote LCD XBee Solar Client Parent Status Consolidation

Status: completed-records-only; submodule identity packets present in dirty
worktrees; exact hardware identity remains open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-IDENTITY-WORKTREE-CONSOLIDATION-2026-06-06`,
`SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-HARDWARE-INTAKE-AUDIT-2026-06-05`,
`SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-PRIVATE-SUBMODULES-2026-05-26`

## Routing

- Verified facts: all seven private `rlxsc-*` submodule worktrees contain a
  `docs/identity-intake.md` packet in the current same-session worktree. The
  packet files are not parent-pinned commits, and each submodule remains dirty
  with local docs/records changes. Current parent records still routed the next
  action as starting submodule identity intake.
- Assumptions: this packet consolidates parent routing only. It does not
  accept exact hardware identity, selected parts, measurements, source closure,
  submodule commits, submodule pointer updates, or bench readiness.
- Unknowns: exact ESP32 board, LCD/backpack, encoder, cell, BMS/protection,
  panel, charger/power path, XBee carrier, antenna, fuse/protection,
  enclosure, power budget, pin map, framework ADR, read-only bench proof, and
  live proof remain unresolved.
- Selected tier: Tier 2 records/status/source consolidation.
- Owner role: Hardware with QA; Communications for XBee boundary; Agent
  Operations for durable records.
- Evidence need: seven submodule identity-intake paths, current submodule
  status, parent remote-client docs/status, source-index coverage, and
  read-only reviewer quorum output.
- Mutation boundary: parent docs/status/source/task/handoff records only. No
  submodule edits, submodule pointer updates, firmware/framework files,
  hardware profiles, communication protocol changes, wiring, charging, serial
  opens, XBee/RF actions, live bench, release, publication, commit, push, PR,
  or deploy.
- Reviewer quorum: read-only Hardware/Architecture coordinator reviewer
  `019e9f02-7a1e-7800-a4f4-72eea2b0db31` weight 5, read-only QA/Evidence
  reviewer `019e9f02-7b2d-7510-ab5f-0ce175bbda05` weight 3, and read-only
  Agent Process/Source Records reviewer
  `019e9f02-7c5c-7093-8928-cc82d74c463d` weight 2 approved this parent
  records-only boundary. Weighted disposition: 10/10 approval with no P1/P2
  blockers, conditioned on saying the identity-intake files are present only in
  current dirty submodule worktrees and do not prove committed/pinned
  submodule state or accepted hardware identity.
- Agent lifecycle cleanup: all three reviewers were waited, outputs captured,
  and closed before mutation. No mutating worker agents were spawned.
- Validation plan: check identity-intake file presence and source IDs across
  all seven submodules, run per-submodule `diff --check`, scan for bulky or
  private artifacts, run parent source/docs/record/path audits, build/audit/
  smoke public docs because `docs/index.md` changed, run parent
  `git diff --check --submodule`, and record any unrelated dirty-tree failures
  separately.

## Implementation

- Added this parent consolidation task, source ledger, source-index row, docs
  index links, and Hardware/QA handoff.
- Updated parent remote-client docs and status ledgers from "start identity
  intake" to "current same-session dirty submodule worktree packets are
  present; complete, review, and track them under submodule-local gates."
- Added direct docs-index reachability for the seven submodule identity-intake
  packet paths.

## Open Evidence

- The seven identity-intake packet files are not accepted as committed,
  parent-pinned, published, or hardware-identity closure.
- Exact hardware identity, part sources, physical markings, photos/inspection
  provenance, voltage/current measurements, charger/BMS/cell compatibility,
  panel Voc/Isc, current limits, enclosure review, fuse/protection review,
  rail budget, current-limited supply plan, boot/recovery packet, pin-risk
  closure, XBee carrier proof, antenna proof, and read-only bench proof remain
  open.

## Authority Limits

This task does not authorize submodule commits, parent submodule pointer
updates, hardware selection, wiring, charging, battery pack assembly, solar
connection, power-path connection, ESP32 GPIO attachment, ESP32 DIN/DOUT to
XBee, live measurement, bench bring-up, firmware implementation, framework
selection, hardware profile acceptance, pin assignment, charger/BMS/cell
selection, enclosure selection, XBee serial open, Tier B AT reads, XCTU or
XBee Studio live discovery, XBee setting writes, `WR`, `AC`, `KY`, API
transmit, RF/range/throughput tests, firmware update/recovery, flash, erase,
monitor, relay/load/mains work, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Validation

Validation completed on 2026-06-06:

- PASS: new task, handoff, and source ledger file checks.
- PASS: all seven `submodules/hardware/rlxsc-*/docs/identity-intake.md`
  packet files exist in current submodule worktrees.
- PASS: all seven identity-intake packet source IDs resolve in their
  submodule-local `knowledge-base/source-index.md` files.
- PASS: per-submodule `git diff --check` for all seven private `rlxsc-*`
  submodules.
- PASS: bulky/private artifact scan across the seven private submodules found
  no PDF, binary, image, installer, archive, or file over 512 KiB.
- PASS: `git submodule status --recursive submodules/hardware` showed all
  seven parent-pinned submodule commits unchanged.
- PASS: `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 218`.
- PASS: `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`.
- PASS: `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`.
- PASS: `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`.
- PASS: `PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py`
  built 64 public files.
- PASS: `PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py build/github-pages/public-file-manifest.json`.
- PASS: `PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py build/github-pages`.
- PASS: focused status wording and overclaim scans showed only negative or
  stop-gate contexts for committed/pinned state, selected hardware, and bench
  readiness.
- PASS: focused trailing-whitespace scan.
- PASS: `git diff --check --submodule`.

## Handoff

[../handoffs/0157-remote-lcd-xbee-solar-client-parent-status-consolidation-to-hardware-qa.md](../handoffs/0157-remote-lcd-xbee-solar-client-parent-status-consolidation-to-hardware-qa.md)

## Decision

Decision: accept this parent records/status consolidation only. Current dirty
submodule worktree identity-intake packet files are present for all seven
private `rlxsc-*` lanes, but exact hardware identity, committed submodule
state, parent-pinned submodule state, power/safety closure, bench readiness,
and all live gates remain open.
