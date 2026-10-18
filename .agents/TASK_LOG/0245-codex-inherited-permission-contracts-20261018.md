# Task 0245: Codex inherited-permission contracts

Status: scoped source correction validated, no publication.
Date: 2026-10-18

## Routing

- Selected tier: Tier 2 governance/configuration source correction.
- Owner role: Agent Operations and Tooling; root serialized writer, independent Codex, governance and QA review.
- Verified facts: current official custom-agent documentation says sandbox/approval fields do not override parent settings.35 owner profiles contained sandbox fields.
- Assumptions: behavioral read-only/scoped-write role contracts remain useful after removing ineffective keys.
- Unknowns: effective native/device/connector confinement; no role file supplies independent OS confinement.
- Evidence need: source ledger in the separate machine knowledge base, exact native preimages, structural TOML comparison, negative permission-key cases and owner audit.
- Mutation boundary: .codex/agents/*.toml, scripts/scaffold_audit_agent_process.py, tests/scaffold_audits/test_agent_profile_permissions.py and this task record.
- Validation: two focused regression tests and full agent-process audit passed; existing sovereignty/hook behavior retained; git diff --check passed.

## Decision

Remove35 ineffective sandbox declarations, explain that permissions inherit from the parent session, preserve all models/effort and existing reviewer/worker boundaries, and reject future misleading permission keys in the audit. Source: https://learn.chatgpt.com/docs/agent-configuration/subagents (accessed2026-10-18). Native rollback fingerprints and independent review receipts are retained by the machine knowledge base; no raw transcripts or credentials retained here.

## Authority Limits

Operator sovereignty and yolo-compatible launch behavior remain unchanged. No admin policy installation, system requirements, live hardware/serial/RF actions, dependency selection, service restart, commit, push, publication or deployment. All49 previously dirty owner paths across this coordinated task were fingerprint-checked unchanged. Lifecycle tools expose completed reviewers; no close-agent API is available in this runtime, and no mutating leaf was spawned for this owner.

Handoff: none. Deferred machine/runtime gates remain in the separate machine records.
