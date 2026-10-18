# Task 0244: Win31 vision-gate command root correction

Date: 2026-10-18
Status: source-corrected-offline-review-only

## Routing

- Verified facts: the vision-gate helper exists in the verified DOS-C checkout;
  ESP32 has no `scripts/win31_dashboard_vision_gate.py`. Its previous relative
  command depended on the caller accidentally using another repository's cwd.
- Assumptions: none about a device, port, screenshot, transcript or live runtime.
- Unknowns: current hardware/runtime identity and behavior remain unqualified.
- Selected tier: Tier 2 instruction-source correction, no live operation.
- Owner role: Agent Operations, with coordinator/evidence/QA and local
  communications/UI-protocol review lenses. Parent-coordinated independent panel
  findings supplied source review; the sole writer spawned no additional agents.
- Evidence need: current helper/path existence, source-ledger context, prior
  EN-03 panel evidence and the corrected absolute-helper/absolute-artifact contract.
- Mutation boundary: this record and
  `.codex/skills/win31-dashboard-vision-gate/SKILL.md` only in ESP32.
- Validation plan: creator validation, existing skills/durable-record audits and
  static path checks; none executes the vision workflow or operates a device.

## Authority limits

The user authorized execution of relevant reviewed improvements. This source
correction does not authorize DOSBox-X, Pi/serial access, flashing, radio, relay,
load, mains, release, publication or other closed surfaces. The existing helper
writes its requested derived JSON; copied-artifact review and live proof stay
separate. H identity was checked through the coordinator's verified native guard.

## Decision

Resolve the DOS-C owner independently of caller cwd, verify its marker/helper,
and pass absolute input/output paths. If it is unavailable, preserve copied
evidence and report the missing gate rather than guessing a path.

Validation results are retained in the cross-repository execution packet at
`/mnt/h/skills-review/2026-10-18/execution/engineering-validation.json`.
Handoff: none within ESP32; the parent coordinator owns final integration.
No runtime, physical-device or publication pass is claimed.
