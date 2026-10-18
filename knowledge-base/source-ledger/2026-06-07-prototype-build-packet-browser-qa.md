# Prototype Build Packet Browser QA Source Ledger

Source index: [../source-index.md](../source-index.md)

Source ID:
`SRC-LOCAL-PROTOTYPE-BUILD-PACKET-BROWSER-QA-2026-06-07`

## Verified Facts

- Task 0229 adds checked-in browser QA for the generated public
  `build/github-pages` artifact.
- `scripts/browser_qa_prototype_packet.js` starts a local HTTP server, launches
  Chrome/Chromium headless, and uses the Chrome DevTools Protocol through Node's
  built-in `WebSocket`.
- The browser QA covers desktop and mobile rendering for the public landing
  page, Prototype Build Packet, visual blueprints, quality page, and admin HMI
  demo.
- The browser QA checks visible text, decoded WebP dimensions, missing required
  prototype WebP images, horizontal overflow, runtime console errors,
  same-origin request failures, and keyboard focus visibility on
  `prototype.html`.
- `.github/workflows/pages.yml` runs the browser QA before uploading the Pages
  artifact, and `.github/workflows/scaffold-ci.yml` runs the browser QA in
  non-deploy CI.

## Evidence

- Browser QA script:
  `scripts/browser_qa_prototype_packet.js`
- Pages workflow:
  `.github/workflows/pages.yml`
- Scaffold CI workflow:
  `.github/workflows/scaffold-ci.yml`
- Scaffold audit guardrail:
  `scripts/scaffold_audit_pages.py`
- Public-site docs:
  `docs/github-pages-public-site.md`
- Task record:
  `.agents/TASK_LOG/0229-prototype-build-packet-browser-qa.md`
- Handoff:
  `.agents/handoffs/0166-prototype-build-packet-browser-qa-to-release-qa.md`
- Source-index row:
  `SRC-LOCAL-PROTOTYPE-BUILD-PACKET-BROWSER-QA-2026-06-07`

## Authority Limits

This source record does not accept or authorize live hardware, relay switching,
MicroSD mount proof, TFT wiring, XBee serial/RF behavior, XBee setting writes,
firmware flashing, monitor, load, mains, GitHub Pages settings mutation,
workflow dispatch, deployment, release, publication, commit, push, PR, or deploy.

## Open Evidence

- Remote GitHub Actions browser-run output after this branch is submitted.
- Live Pages deployment state and publication gate evidence.
- Any hardware-facing proof for relay, MicroSD, TFT, XBee, load, mains, flash, or
  monitor behavior.
