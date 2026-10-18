# Task 0208: Skill Inventory Cache Refresh 2026-06-06

Status: accepted-path-only

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-06

Source IDs:
`SRC-LOCAL-CODEX-SKILL-INVENTORY-REFRESH-2026-06-06`,
`SRC-LOCAL-CODEX-SKILL-INVENTORY-REFRESH-2026-06-05`,
`SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-05`,
`SRC-LOCAL-AGENTIC-PLANNING-GUIDE-2026-06-05`

## Routing

- Verified facts: `scripts/scaffold_audit_skills.py` failed on seven stale
  GitHub/Canva plugin skill paths in `research/skills/available-skills.md`.
  The failing inventory used cache hash `2cb26e7b`; same-session path checks and
  plugin-cache `SKILL.md` inventory confirmed matching current paths under
  cache hash `3f0def1b`.
- Assumptions: replacing the exact seven `2cb26e7b` path claims with matching
  `3f0def1b` path claims is a path-only inventory refresh and does not change
  installed plugins, enabled capabilities, or repo-local skill routing.
- Unknowns: future sessions may rotate plugin cache hashes again.
- Selected tier: Tier 2 docs/process inventory cleanup.
- Owner role: Agent Operations/Tooling with QA/Evidence.
- Evidence need: failing skill audit output, same-session path inventory,
  read-only reviewer quorum output, updated skills inventory, source-index
  refresh row, task record, and validation output.
- Mutation boundary: `research/skills/available-skills.md`,
  `knowledge-base/source-index.md`, and this task record only.
- Reviewer quorum: read-only AgentOps/Tooling reviewer weight 3 and read-only
  QA/Evidence reviewer weight 3 were spawned, waited, captured, and closed
  before mutation; coordinator gate owner weight 5 completed local triage.
  Weighted vote: 11/11 approve within the named path-only boundary, with no
  P1/P2 blockers after conditions are satisfied.
- Agent lifecycle cleanup: completed reviewers were closed before the mutation.
- Gate authority: path-only skills inventory refresh. No plugin install,
  plugin uninstall, hook/config/admin mutation, live hardware, serial/RF/XBee,
  release, publication, commit, push, PR, or deploy authority is opened.

## Implementation

- Updated seven GitHub/Canva plugin skill paths in
  `research/skills/available-skills.md` from cache hash `2cb26e7b` to
  `3f0def1b`.
- Added source-index row
  `SRC-LOCAL-CODEX-SKILL-INVENTORY-REFRESH-2026-06-06` to supersede the stale
  cache path claim for plugin skill paths.
- Preserved the required `SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-05` marker in
  `research/skills/available-skills.md` while adding the 2026-06-06 refresh
  source.

## Validation

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py`
  passed: `PASS: ESP32 skill audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py` passed:
  `PASS: ESP32 scaffold validation succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 208`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py` passed
  with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- Targeted stale-path scan
  `rg -n "/home/cyber/.codex/plugins/cache/openai-curated/(github|canva)/2cb26e7b" research/skills/available-skills.md`
  returned no matches.
- Targeted scoped hash/source scan confirmed `3f0def1b` paths in
  `research/skills/available-skills.md`, retained source IDs, and only
  historical/stale `2cb26e7b` references in source/task explanatory text.
- `git diff --check` passed with exit code 0.

## Authority Limits

This task does not authorize plugin installation, plugin removal, marketplace
mutation, `.codex/config.toml` changes, hook changes, admin profile changes,
`/etc/codex/requirements.toml`, `admin-strict`, live hardware, serial/RF/XBee
writes, firmware changes, release, publication, commit, push, PR, or deploy.

## Handoff

No handoff required; this is a path-only inventory cleanup and validation is
recorded in this task.

## Decision

Decision: accept the path-only skills inventory refresh. Plugin cache paths
remain drift-prone and future sessions must re-inventory before relying on local
plugin cache path claims.
