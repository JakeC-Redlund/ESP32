# Handoff 0158: Remote LCD XBee Solar Client Power Safety Packet Review To Hardware QA

## Current State

The parent ESP32 records now distinguish two remote-client facts:

- Task 0218 records current dirty worktree `docs/identity-intake.md` packet
  path presence in all seven private `rlxsc-*` submodules.
- Task 0219 records docs-only review/tracking of the first three priority
  power/safety packets only: `rlxsc-18650-cell`,
  `rlxsc-bms-protection`, and `rlxsc-solar-charger-power-path`.

Source ID:
`SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-POWER-SAFETY-PACKET-REVIEW-2026-06-06`.

This is not committed submodule state, not parent-pinned state, not exact
hardware identity, not selected hardware, not measurements, not compatibility
closure, and not bench readiness.

## Continue With

- Review and track the remaining four identity-intake packets under
  parent-only docs/status gates: `rlxsc-esp32-client-node`,
  `rlxsc-lcd-20x4-i2c`, `rlxsc-rotary-encoder`, and
  `rlxsc-xbee-pro-s3b`.
- After all seven packets have docs-only review records, complete
  submodule-local review/commit gates and later parent-pin gates only with
  explicit authority.
- Continue exact hardware evidence collection for power/safety first:
  physical markings, vendor/source links, inspection records, cell/BMS/charger
  compatibility, panel Voc/Isc, fuse/protection, enclosure, power budget, and
  current-limit plan.

## Stop Gates

Do not claim exact component identity, source closure, selected parts,
committed submodule state, parent-pinned state, hardware acceptance, wiring
readiness, charging readiness, RF readiness, firmware readiness, or bench
readiness from this handoff.

Do not connect a battery, BMS/protection board, charger, solar panel, ESP32
rail, LCD, encoder, XBee carrier, antenna hardware, fuse/protection hardware,
or enclosure hardware from this handoff.

No firmware source, framework selection, pin assignment, XBee serial open, Tier
B read query, XCTU/XBee Studio live discovery, XBee setting write, API transmit
frame, RF test, relay/load/mains, release, publication, commit, push, PR, or
deploy is authorized.

## Validation To Preserve

```bash
test -f .agents/TASK_LOG/0219-remote-lcd-xbee-solar-client-power-safety-packet-review.md
test -f .agents/handoffs/0158-remote-lcd-xbee-solar-client-power-safety-packet-review-to-hardware-qa.md
test -f knowledge-base/source-ledger/2026-06-06-remote-lcd-xbee-solar-client-power-safety-packet-review.md
git submodule status --recursive submodules/hardware/rlxsc-18650-cell submodules/hardware/rlxsc-bms-protection submodules/hardware/rlxsc-solar-charger-power-path
for d in submodules/hardware/rlxsc-18650-cell submodules/hardware/rlxsc-bms-protection submodules/hardware/rlxsc-solar-charger-power-path; do test -f "$d/docs/identity-intake.md"; git -C "$d" diff --check; done
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 219
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/build_github_pages.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/audit_public_manifest.py build/github-pages/public-file-manifest.json
PYTHONDONTWRITEBYTECODE=1 python3 scripts/smoke_github_pages.py build/github-pages
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py
git diff --check --submodule
```
