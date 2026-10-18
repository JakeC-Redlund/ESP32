# Modular Scaffold Audit CI Matrix Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-MODULAR-SCAFFOLD-AUDIT-CI-MATRIX-2026-06-07`

## Verified Facts

- Task 0230 adds one-audit execution to `scripts/verify_scaffold.py` through
  `--audit <name>`.
- The accepted audit group names remain owned by `verify_scaffold.py::AUDITS`.
- `.github/workflows/scaffold-ci.yml` now includes a non-deploy
  `scaffold-audit-matrix` job over those audit group names.
- Focused reporting tests check that the workflow matrix exactly matches the
  verifier registry.

## Evidence

- Verifier:
  `scripts/verify_scaffold.py`
- Non-deploy workflow:
  `.github/workflows/scaffold-ci.yml`
- Focused tests:
  `tests/scaffold_audits/test_scaffold_audit_reporting.py`
- Workflow documentation:
  `.github/workflows/README.md`
- Command matrix:
  `research/workspace-review-command-matrix.md`
- Task record:
  `.agents/TASK_LOG/0230-modular-scaffold-audit-ci-matrix.md`
- Handoff:
  `.agents/handoffs/0167-modular-scaffold-audit-ci-matrix-to-qa-tooling-release.md`
- Source-index row:
  `SRC-LOCAL-MODULAR-SCAFFOLD-AUDIT-CI-MATRIX-2026-06-07`

## Authority Limits

This source record does not accept or authorize workflow dispatch, artifact
upload, GitHub Pages deploy, release, publication, commit, push, PR, framework
selection, firmware runtime, live hardware, serial/RF/XBee writes, flashing,
monitor, relay/load/mains work, `/etc/codex/requirements.toml`, or
`admin-strict` installation.

## Open Evidence

- Remote GitHub Actions run output for the new matrix after future publication.
- Future firmware-framework CI matrix definition after framework selection.
