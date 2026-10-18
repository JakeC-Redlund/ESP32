# Task 0230: Modular Scaffold Audit CI Matrix

Status: accepted-host-only-ci-matrix

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-MODULAR-SCAFFOLD-AUDIT-CI-MATRIX-2026-06-07`,
`SRC-LOCAL-WORKSPACE-REVIEW-FOLLOW-UP-HARDENING-2026-06-05`,
`SRC-LOCAL-AGENT-INSTRUCTION-SKILL-HOOK-CI-HARDENING-2026-06-02`, and
`SRC-LOCAL-DEVELOPMENT-PLAN-CONSOLIDATION-2026-05-27`.

## Routing

- Verified facts: `scripts/verify_scaffold.py` owns the current scaffold audit
  registry with the groups `paths`, `sources`, `docs`, `firmware`, `pages`,
  `react_native`, `agent_process`, `skills`, and `records`.
  `.github/workflows/scaffold-ci.yml` already runs non-deploy PR/manual CI and
  keeps workflow permissions at `contents: read`. The known-gap ledger requested
  a decision on whether modular scaffold audits should become a CI matrix after
  the local-only verifier proved stable.
- Assumptions: A verifier-owned single-audit selector plus a non-deploy matrix
  job improves CI failure isolation without changing the aggregate local
  verifier, release authority, or any hardware gate.
- Unknowns/open evidence: remote GitHub Actions execution for the new matrix,
  release/publication state, commit/push/PR/deploy state, and any live hardware,
  serial, RF, flash, relay/load/mains, or firmware-runtime proof remain
  unproved or unauthorized.
- Selected tier: Tier 2 host-only tooling, CI, docs, and records update.
- Owner role: Tooling and QA, with Release/Records and Agent Operations review.
- Evidence need: current workflow/verifier inspection, read-only reviewer quorum,
  verifier single-audit mode, workflow matrix wiring, focused drift tests,
  workflow/docs/status/gap updates, durable task/source/handoff records, focused
  audit-selector validation, record/source/docs/agent-process audits, diff
  hygiene, and full scaffold verification.
- Mutation boundary:
  `.github/workflows/scaffold-ci.yml`, `.github/workflows/README.md`,
  `scripts/verify_scaffold.py`,
  `tests/scaffold_audits/test_scaffold_audit_reporting.py`,
  `research/workspace-review-command-matrix.md`,
  `.agents/TASK_LOG/0230-modular-scaffold-audit-ci-matrix.md`,
  `.agents/handoffs/0167-modular-scaffold-audit-ci-matrix-to-qa-tooling-release.md`,
  `knowledge-base/source-ledger/2026-06-07-modular-scaffold-audit-ci-matrix.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: focused reporting/matrix tests, each verifier audit group via
  `--audit`, full verifier, durable-record/source/docs/agent-process audits,
  scoped `git diff --check`, and workflow/status review.

## Reviewer Quorum

Read-only reviewer quorum was completed and closed before mutation:

- Tooling/QA reviewer `019e9fc2-383a-7660-a01a-33784bcad810`, weight 3,
  approved the named Tier 2 host-only CI/tooling mutation with no P1/P2 blockers
  if drift protection proves the workflow matrix audit names match
  `verify_scaffold.py::AUDITS`.
- Release/Records/Agent-Operations reviewer
  `019e9fc2-3949-7292-857e-da764e15df44`, weight 3, initially rejected because
  the pre-mutation checkout did not yet contain the implementation or durable
  records, then clarified approval for the pre-mutation boundary once the plan
  explicitly included actual implementation, focused tests, durable records,
  source/docs/status/gap closure, read-only workflow permissions, and closed
  release/publication/hardware surfaces.

Weighted reviewer result before mutation: 6/6 conditional approval. Conditions
were resolved by adding verifier-owned single-audit execution, adding a matrix
job over the registry names, keeping the aggregate verifier in CI, and adding a
test that compares workflow matrix names against `verify_scaffold.audit_names()`.
No mutating worker agents were spawned. Visible lifecycle state included the two
reviewer IDs; both completed reviewers were waited and closed before mutation.
Under `bypassPermissions`, repo hooks remain advisory and are not runtime cleanup
proof.

## Implementation

- Added `--audit <name>` to `scripts/verify_scaffold.py` so one registered audit
  group can run independently while the no-argument command remains the full
  aggregate verifier.
- Added a non-deploy `scaffold-audit-matrix` job to
  `.github/workflows/scaffold-ci.yml` with `fail-fast: false`, checkout only, and
  read-only audit group execution through `verify_scaffold.py --audit`.
- Kept the aggregate `verify_scaffold.py` invocation in the primary scaffold job
  as the canonical local/full verifier.
- Added focused tests that verify one registered audit can run and that the
  workflow matrix names exactly match `verify_scaffold.audit_names()`.
- Updated workflow docs, command matrix, source records, docs index,
  development plan/status/triage, and known gaps.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_scaffold_audit_reporting`
  passed before record wiring: `Ran 5 tests in 0.886s` and `OK`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit paths`
  passed: `PASS: ESP32 scaffold audit paths succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit docs`
  passed: `PASS: ESP32 scaffold audit docs succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit sources`
  passed: `PASS: ESP32 scaffold audit sources succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit firmware`
  passed: `PASS: ESP32 scaffold audit firmware succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit pages`
  passed: `PASS: ESP32 scaffold audit pages succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit react_native`
  passed: `PASS: ESP32 scaffold audit react_native succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit agent_process`
  passed: `PASS: ESP32 scaffold audit agent_process succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit skills`
  passed: `PASS: ESP32 scaffold audit skills succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py --audit records`
  passed: `PASS: ESP32 scaffold audit records succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_scaffold_audit_reporting`
  passed after record/status wiring: `Ran 5 tests in 1.472s` and `OK`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover -s tests/scaffold_audits -p 'test_*.py'`
  passed: `Ran 141 tests in 96.492s` and `OK`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 230`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.
- `git diff --check -- .github/workflows/scaffold-ci.yml .github/workflows/README.md scripts/verify_scaffold.py tests/scaffold_audits/test_scaffold_audit_reporting.py research/workspace-review-command-matrix.md .agents/TASK_LOG/0230-modular-scaffold-audit-ci-matrix.md .agents/handoffs/0167-modular-scaffold-audit-ci-matrix-to-qa-tooling-release.md knowledge-base/source-ledger/2026-06-07-modular-scaffold-audit-ci-matrix.md knowledge-base/source-index.md docs/index.md research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md`
  passed with no output.

## Authority Limits

This task does not accept or authorize workflow dispatch, artifact upload,
GitHub Pages deploy, release, publication, commit, push, PR, framework
selection, firmware runtime, live hardware, serial/RF/XBee writes, flashing,
monitor, relay/load/mains work, `/etc/codex/requirements.toml`, or
`admin-strict` installation. The later firmware-framework CI matrix remains a
separate open gap.

## Handoff

Handoff:
[../handoffs/0167-modular-scaffold-audit-ci-matrix-to-qa-tooling-release.md](../handoffs/0167-modular-scaffold-audit-ci-matrix-to-qa-tooling-release.md)

## Decision

Decision: accept the host-only modular scaffold-audit CI matrix for non-deploy
PR/manual CI failure isolation. Keep the aggregate scaffold verifier as the
canonical local/full validation gate and keep the firmware-framework CI matrix
blocked until the framework lane opens it.
