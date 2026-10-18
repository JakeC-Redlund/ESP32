# Handoff 0160: Remote LCD XBee Solar Client Submodule Local Review To Hardware QA Communications

## Current State

Task 0221 records submodule-local docs/records-only review packets for all
seven current dirty `rlxsc-*` identity-intake packet worktrees.

Source ID:
`SRC-LOCAL-REMOTE-LCD-XBEE-SOLAR-CLIENT-SUBMODULE-LOCAL-REVIEW-2026-06-06`.

This is not committed submodule state, not parent-pinned state, not exact
hardware identity, not selected hardware, not source closure, not measurements,
not safety closure, not pin readiness, not RF readiness, not firmware readiness,
and not bench readiness.

## Continue With

- Commit submodule-local docs/records packets only under separate explicit
  submodule commit authority.
- Later parent-pin reviewed submodule commits only under separate explicit
  parent pointer authority.
- Continue exact hardware evidence collection: physical markings,
  vendor/source links, inspection records, voltage/pullup/current evidence,
  boot/recovery evidence, pin-risk review, XBee carrier/antenna/adapter
  evidence, power budget, current-limit plan, framework ADR, and read-only
  bench proof.
- Keep Communications in the loop for any XBee carrier, antenna, serial-read,
  profile, privacy, redaction, or later RF boundary.

## Stop Gates

Do not claim exact component identity, source closure, selected parts,
committed submodule state, parent-pinned state, hardware acceptance, pin
readiness, wiring readiness, charging readiness, RF readiness, firmware
readiness, or bench readiness from this handoff.

No firmware source, framework selection, pin assignment, XBee serial open, Tier
B read query, XCTU/XBee Studio live discovery, XBee setting write, AT/API
operation, RF test, flash, monitor, relay/load/mains, release, publication,
commit, push, PR, parent pointer update, or deploy is authorized.

## Validation To Preserve

```bash
for d in submodules/hardware/rlxsc-18650-cell submodules/hardware/rlxsc-bms-protection submodules/hardware/rlxsc-solar-charger-power-path submodules/hardware/rlxsc-esp32-client-node submodules/hardware/rlxsc-lcd-20x4-i2c submodules/hardware/rlxsc-rotary-encoder submodules/hardware/rlxsc-xbee-pro-s3b; do git -C "$d" diff --check; done
git submodule status --recursive submodules/hardware/rlxsc-18650-cell submodules/hardware/rlxsc-bms-protection submodules/hardware/rlxsc-solar-charger-power-path submodules/hardware/rlxsc-esp32-client-node submodules/hardware/rlxsc-lcd-20x4-i2c submodules/hardware/rlxsc-rotary-encoder submodules/hardware/rlxsc-xbee-pro-s3b
git diff --submodule=short -- submodules/hardware/rlxsc-18650-cell submodules/hardware/rlxsc-bms-protection submodules/hardware/rlxsc-solar-charger-power-path submodules/hardware/rlxsc-esp32-client-node submodules/hardware/rlxsc-lcd-20x4-i2c submodules/hardware/rlxsc-rotary-encoder submodules/hardware/rlxsc-xbee-pro-s3b
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 221
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
