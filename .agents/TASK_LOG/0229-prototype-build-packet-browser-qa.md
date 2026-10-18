# Task 0229: Prototype Build Packet Browser QA

Status: browser-qa-ci-accepted

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-07

Source coverage:
`SRC-LOCAL-PROTOTYPE-BUILD-PACKET-BROWSER-QA-2026-06-07`,
`SRC-GITHUB-PAGES-WHAT-IS`,
`SRC-GITHUB-PAGES-PUBLISHING-SOURCE`,
`SRC-GITHUB-PAGES-CUSTOM-WORKFLOWS`,
`SRC-GITHUB-PAGES-LIMITS`, and
`SRC-LOCAL-PROTOTYPE-PACKET-2026-05-21`.

## Routing

- Verified facts: `scripts/build_github_pages.py` builds a curated
  `build/github-pages` artifact; `scripts/audit_public_manifest.py` verifies
  public allowlists, links, source IDs, hashes, and WebP header dimensions; and
  `scripts/smoke_github_pages.py` performs static page smoke checks. Before this
  task, `.github/workflows/pages.yml` did not run real-browser rendering checks
  before artifact upload, and the known-gap ledger requested checked-in browser
  QA for the public Prototype Build Packet.
- Assumptions: A dependency-free Node script using a real Chrome/Chromium
  executable and the Chrome DevTools Protocol can cover the browser-regression
  gap without adding Playwright, Puppeteer, Selenium, or publication authority.
- Unknowns/open evidence: live GitHub Pages deployment state, live hardware,
  relay behavior, MicroSD mount, TFT wiring, XBee hardware behavior, load,
  mains, release, publication, commit, push, PR, and deploy remain unproved or
  unauthorized.
- Selected tier: Tier 2 host-only tooling, CI, docs, and records update.
- Owner role: Tooling/QA with Release and Agent Operations review.
- Evidence need: current Pages tooling inspection, read-only reviewer quorum,
  checked-in browser QA script, workflow-before-upload integration, PR CI
  integration, scaffold-audit guardrails, durable task/source/handoff records,
  docs/status/gap updates, browser run output, focused audits, diff hygiene, and
  full scaffold verification.
- Mutation boundary:
  `scripts/browser_qa_prototype_packet.js`, `.github/workflows/pages.yml`,
  `.github/workflows/scaffold-ci.yml`, `scripts/scaffold_audit_pages.py`,
  `scripts/scaffold_audit_data.py`, `docs/github-pages-public-site.md`,
  `.agents/TASK_LOG/0229-prototype-build-packet-browser-qa.md`,
  `.agents/handoffs/0166-prototype-build-packet-browser-qa-to-release-qa.md`,
  `knowledge-base/source-ledger/2026-06-07-prototype-build-packet-browser-qa.md`,
  `knowledge-base/source-index.md`, `docs/index.md`,
  `research/development-plan.md`, `research/development-status-ledger.md`,
  `research/triage-status.md`, and `research/known-gaps.md`.
- Validation plan: Node syntax check, Pages build, manifest audit, smoke check,
  real-browser QA against `build/github-pages`, scaffold-audit unittest
  discovery, durable-record/source/docs/agent-process/page audits, scoped
  `git diff --check`, and full scaffold verification.

## Reviewer Quorum

Read-only reviewer quorum was completed and closed before mutation:

- Tooling/QA reviewer `019e9fad-f349-7c82-989f-11430ff110c4`, weight 3,
  approved the named Tier 2 host-only browser QA mutation with no P1 blockers and
  no P2 blockers if the browser QA fails closed in CI and does not depend on
  unpinned ambient browser automation packages.
- Release/Records/Agent-Operations reviewer
  `019e9fae-25e9-7e73-b95d-724575bff032`, weight 3, approved with no P1
  blockers and P2 conditions that the Pages workflow run the browser QA before
  artifact upload, that the browser runner be reproducible or fail closed, and
  that durable records/status updates preserve closed live/release boundaries.

Weighted reviewer result before mutation: 6/6 conditional approval. Conditions
were resolved by using a checked-in dependency-free Node/CDP script, adding it to
the Pages workflow before artifact upload, adding it to PR `scaffold-ci.yml`, and
adding scaffold-audit markers that require the script and workflow invocation. No
mutating worker agents were spawned. Visible lifecycle state included the two
reviewer IDs; both completed reviewers were closed before mutation. Under
`bypassPermissions`, repo hooks remain advisory and are not runtime cleanup
proof.

## Implementation

- Added `scripts/browser_qa_prototype_packet.js`, which starts a local HTTP
  server for `build/github-pages`, discovers Chrome/Chromium from `CHROME_PATH`,
  common runner paths, or the local Playwright cache, launches headless Chrome,
  and uses the Chrome DevTools Protocol through Node's built-in `WebSocket`.
- The browser QA checks desktop and mobile rendering for `index.html`,
  `prototype.html`, `blueprints.html`, `quality.html`, and
  `demos/admin-hmi/index.html`.
- The script fails on missing visible text, zero-size rendered bodies,
  horizontal overflow, decoded WebP dimension mismatches, missing required
  prototype WebP images, runtime console errors, same-origin 4xx/load failures,
  and insufficient keyboard focus visibility on `prototype.html`.
- Added `node scripts/browser_qa_prototype_packet.js build/github-pages` to
  `.github/workflows/pages.yml` after build/manifest/smoke and before artifact
  upload.
- Added the same build/manifest/smoke/browser QA sequence to non-deploy
  `.github/workflows/scaffold-ci.yml` for pull-request and manual CI coverage.
- Updated scaffold Pages and known-file audits so the browser QA script and
  workflow marker are required.
- Updated public-site docs, source records, docs index, plan/status/triage, and
  known gaps.

## Validation

- `node --check scripts/browser_qa_prototype_packet.js` passed with no output.
- `python3 scripts/build_github_pages.py --out build/github-pages && python3 scripts/audit_public_manifest.py && python3 scripts/smoke_github_pages.py`
  passed:
  `Built /mnt/h/esp32/build/github-pages with 64 public files`,
  `PASS: public manifest audit succeeded for /mnt/h/esp32/build/github-pages/public-file-manifest.json`,
  and `PASS: GitHub Pages smoke succeeded for /mnt/h/esp32/build/github-pages`.
- `node scripts/browser_qa_prototype_packet.js build/github-pages` passed:
  `PASS: Prototype Build Packet browser QA succeeded for /mnt/h/esp32/build/github-pages`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover -s tests/scaffold_audits -p 'test_*.py'`
  passed: `Ran 139 tests in 138.837s` and `OK`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py`
  passed with no output.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 229`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`,
  `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`, and
  `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed; agent-process audit printed `PASS: ESP32 agent-process audit
  succeeded`.
- `python3 scripts/build_github_pages.py --out build/github-pages && python3 scripts/audit_public_manifest.py && python3 scripts/smoke_github_pages.py && node scripts/browser_qa_prototype_packet.js build/github-pages`
  passed after all record/status edits.
- `git diff --check -- scripts/browser_qa_prototype_packet.js .github/workflows/pages.yml .github/workflows/scaffold-ci.yml scripts/scaffold_audit_pages.py scripts/scaffold_audit_data.py docs/github-pages-public-site.md .agents/TASK_LOG/0229-prototype-build-packet-browser-qa.md .agents/handoffs/0166-prototype-build-packet-browser-qa-to-release-qa.md knowledge-base/source-ledger/2026-06-07-prototype-build-packet-browser-qa.md knowledge-base/source-index.md docs/index.md research/development-plan.md research/development-status-ledger.md research/triage-status.md research/known-gaps.md`
  passed with no output.
- `timeout 240s env PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.

## Authority Limits

This task does not accept or authorize live hardware, relay switching, MicroSD
mount proof, TFT wiring, XBee serial/RF behavior, XBee setting writes, firmware
flashing, monitor, load, mains, GitHub Pages settings mutation, workflow
dispatch, deployment, release, publication, commit, push, PR, or deploy. The
workflow edits define CI behavior only; they do not run a remote workflow in this
session.

## Handoff

Handoff:
[../handoffs/0166-prototype-build-packet-browser-qa-to-release-qa.md](../handoffs/0166-prototype-build-packet-browser-qa-to-release-qa.md)

## Decision

Decision: accept checked-in CI-backed browser QA for the generated public
Prototype Build Packet artifact. Keep deployment, publication, and all hardware
proof gates separate.
