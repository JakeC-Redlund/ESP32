# Handoff 0167: Modular Scaffold Audit CI Matrix To QA Tooling Release

Status: active

Task:
[Task 0230](../TASK_LOG/0230-modular-scaffold-audit-ci-matrix.md)

Source coverage:
`SRC-LOCAL-MODULAR-SCAFFOLD-AUDIT-CI-MATRIX-2026-06-07`,
`SRC-LOCAL-WORKSPACE-REVIEW-FOLLOW-UP-HARDENING-2026-06-05`,
`SRC-LOCAL-AGENT-INSTRUCTION-SKILL-HOOK-CI-HARDENING-2026-06-02`, and
`SRC-LOCAL-DEVELOPMENT-PLAN-CONSOLIDATION-2026-05-27`.

## Verified Facts

- Task 0230 adds `scripts/verify_scaffold.py --audit <name>` for one registered
  scaffold audit group.
- `scaffold-ci.yml` now has a non-deploy matrix job over the verifier audit
  group names.
- Focused tests compare the workflow matrix names against the verifier registry.

## Assumptions

- Future scaffold audit additions will update the verifier registry first, then
  update the workflow matrix and focused drift test together.
- QA will keep the aggregate verifier as the canonical local/full gate.

## Unknowns

- Remote GitHub Actions execution for the new matrix is not proven in this
  local task.
- Release, publication, workflow dispatch, commit, push, PR, deploy, framework
  matrix, and hardware behavior remain open or closed as previously recorded.

## Continue With

- Review future audit additions against both `verify_scaffold.py::AUDITS` and
  `.github/workflows/scaffold-ci.yml`.
- Re-run `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_scaffold_audit_reporting`
  after workflow matrix edits.
- Keep the later firmware-framework CI matrix separate until framework routing
  explicitly opens it.

## Do Not Do

- Do not treat Task 0230 as workflow dispatch, release, publication, commit,
  push, PR, deploy, framework-selection, firmware-runtime, live hardware,
  serial/RF/XBee write, flash/monitor, relay/load/mains, `/etc/codex`, or
  `admin-strict` authority.
