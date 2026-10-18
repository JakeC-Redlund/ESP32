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
  `https://jakec-redlund.github.io/ESP32/`; re-verify the Pages API and latest
  workflow result after any repository, branch, workflow, permission, or Pages
  setting change.
- The workflow intentionally does not publish the repository root or the
  working `docs/` tree.

Public checkouts explicitly select standalone DOS-C parity scope. An absent
private sibling is reported as not evaluated; local default audits and any
explicit or available DOS-C source remain required and strict. Pages cannot
establish the six private-source Win31 integration checks.

The host-test steps use an isolated runner-temporary environment with the exact
hashed Linux x86_64 / CPython 3.12 image-test artifacts in
`../requirements-host-tests.txt`. Pillow, NumPy and headless OpenCV are needed by
synthetic legibility fixtures. Installation occurs after scaffold audits, uses
binary artifacts without dependency resolution, and does not update an existing
environment. See the exact release metadata for
[Pillow](https://pypi.org/pypi/Pillow/12.3.0/json),
[NumPy](https://pypi.org/pypi/numpy/2.3.4/json) and
[OpenCV headless](https://pypi.org/pypi/opencv-python-headless/4.13.0.92/json).
