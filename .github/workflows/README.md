# GitHub Workflows

- `scaffold-ci.yml` is non-deploy CI for pull requests and manual validation.
  It runs a modular scaffold-audit matrix, diff whitespace checks, syntax
  checks, React Native workspace checks, full scaffold-audit unittest discovery,
  public Pages build/manifest/smoke/browser QA, publication hygiene reporting,
  aggregate scaffold verification, Python compilation, and host-side contract
  tests. It does not upload Pages artifacts or deploy.
- `firmware-ci.yml` is non-deploy CI for accepted project-local ESP-IDF build
  targets only. It builds `four-relay-xbee-wifi` under `ADR-0002` and
  `espnow-bbs` under `ADR-0003` with ESP-IDF `v6.0.1` and target `esp32` by
  using `espressif/esp-idf-ci-action@v1`. It does not flash, monitor, upload
  artifacts, deploy, publish, or select a framework for other projects.
- `pages.yml` builds the curated public DIY site with
  `scripts/build_github_pages.py` and deploys only `build/github-pages` through
  GitHub Pages on `main` push or manual dispatch.
- Before upload, the workflow now runs JSON and JavaScript syntax checks,
  `git diff --check`, public manifest audit, Pages smoke checks, public
  browser QA, full scaffold-audit unittest discovery, scaffold verification,
  Python compilation, and host-side contract tests.
- Current repository Pages settings use the GitHub Actions source for
  `https://cyberpivots.github.io/ESP32/`; re-verify the Pages API and latest
  workflow result after any repository, branch, workflow, permission, or Pages
  setting change.
- The workflow intentionally does not publish the repository root or the
  working `docs/` tree.
