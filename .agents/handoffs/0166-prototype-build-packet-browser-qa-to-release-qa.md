# Handoff 0166: Prototype Build Packet Browser QA To Release QA

Status: active

Task:
[Task 0229](../TASK_LOG/0229-prototype-build-packet-browser-qa.md)

Source coverage:
`SRC-LOCAL-PROTOTYPE-BUILD-PACKET-BROWSER-QA-2026-06-07`,
`SRC-GITHUB-PAGES-WHAT-IS`,
`SRC-GITHUB-PAGES-PUBLISHING-SOURCE`,
`SRC-GITHUB-PAGES-CUSTOM-WORKFLOWS`,
`SRC-GITHUB-PAGES-LIMITS`, and
`SRC-LOCAL-PROTOTYPE-PACKET-2026-05-21`.

## Verified Facts

- Task 0229 adds checked-in browser QA for the generated public
  `build/github-pages` artifact.
- The Pages deployment workflow runs the browser QA before
  `actions/upload-pages-artifact@v4`.
- The non-deploy scaffold CI workflow builds the public artifact and runs
  manifest, smoke, and browser QA without uploading or deploying Pages.
- The browser QA passed locally against `build/github-pages`.

## Assumptions

- Future public-site changes will keep the browser QA as a fail-closed CI step,
  not as optional local review.
- Release QA will treat this as generated-artifact browser validation only, not
  publication success.

## Unknowns

- Live GitHub Pages deployment state, Pages settings, remote workflow outcome,
  release state, publication state, commit/push/PR/deploy state, and hardware
  behavior remain open.

## Continue With

- Re-run `python3 scripts/build_github_pages.py --out build/github-pages`,
  `python3 scripts/audit_public_manifest.py`,
  `python3 scripts/smoke_github_pages.py`, and
  `node scripts/browser_qa_prototype_packet.js build/github-pages` before future
  public-site acceptance.
- Keep `scripts/scaffold_audit_pages.py` markers aligned with the browser QA
  script and workflow invocation.
- If CI runner browser availability changes, update the script or workflow so
  browser QA remains reproducible and fail-closed.

## Do Not Do

- Do not treat Task 0229 as live hardware, relay, MicroSD, TFT, XBee, load,
  mains, release, publication, workflow-dispatch, Pages settings, commit, push,
  PR, or deploy authority.
