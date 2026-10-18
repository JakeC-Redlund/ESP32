# Task 0246: Reviewed source Git convergence

Date: 2026-10-18

Status: reviewed source published; hosted cleanup correction and final convergence checks pending

Contract: [../../AGENTS.md](../../AGENTS.md)

## Routing and ownership

- Requested action: review, commit, push, synchronize and consolidate branches
  and forks. The user explicitly selected JakeC-Redlund ownership and accepted
  repository transfers, including ESP32 and the seven private hardware children.
- Selected tier: Tier 2 source integration, with explicit user authorization for
  GitHub publication. Live hardware and physical acceptance remain separate.
- Root is the sole source/Git writer. Independent engineering, business/governance
  and installed-source QA readers supplied bounded review. Source-review weights:
  coordinator 5, each specialist 3; all four accepted the corrected bounded scope
  (14/14). Checks below and the recorded limitations constrain that acceptance.
- Model and reasoning choices inherit the session; no override or paid call.
  Completed reader state was inspected and reused. The runtime exposes no
  close-agent tool, so completed readers remain idle with captured handoffs.

## Reviewed integration

Preserve reviewed source, task/handoff and source-ledger updates, accepted ADR
boundaries, host-only XBee parser tests, firmware build targets and the curated
public-site workflow. Keep the seven hardware children private, docs-only and
unqualified for wiring, charging, RF, firmware or live bench use. Publish children
before the parent gitlinks and update current clone URLs to JakeC-Redlund.

Correct stale firmware startup/framework wording, redact two active public-facing
port summaries, distinguish required project skill sources from optional session
catalog discovery, reject stale plugin-cache locators, and test inherited role
permissions without removing read-only task intent or Tier 3 requirements.
Historical source ledgers retain their original attribution and dates.

## Validation observed

- Scaffold validation passed; scaffold unit suite: 149 passed after the stale
  permission assertion was corrected. Inventory-contract regression tests: 5 passed.
- Custom wireless protocol: 40 passed. Four-relay safety/API/storage/XBee host
  checks, mesh discovery and retained-persistence host checks passed.
- Existing native ESP-IDF 6.0.1 built both four-relay and ESP-NOW BBS targets.
  The first attempt lacked the explicit installed IDF_TOOLS_PATH; the corrected
  command-local native configuration passed. Outputs remain native. No tool
  installation, activation-state change, download, flash or device operation.
- Existing ESLint and TypeScript checks passed. Jest: 10 suites and 74 tests
  passed; its full result is retained in the publication receipt.
- Curated Pages artifact: 64 files; manifest audit, smoke and browser QA passed.
  These are source/browser checks, not hardware or physical acceptance.
- Expo Doctor was not run: its installed implementation performs external API
  calls. No full Doctor pass is claimed. Existing H: JS dependency storage is
  recorded as pre-existing; this task performs no dependency-storage migration.

Native recovery preserves original source, dirty files and branch refs. The
external git-convergence receipts record exact commands, results, source hashes,
commits and hosted/default checks. Failed or deferred checks remain explicit.

## Hosted CI follow-up

The first hosted Pages run for the published source failed with ENOTEMPTY while
removing its owned temporary Chromium profile. Local content/browser checks had
passed. The cleanup now waits for actual owned-child exit after TERM and, when
required, KILL, clears bounded timers/listeners, retains the profile if exit
cannot be confirmed, and retries temporary-directory removal within a bound.
Independent QA accepted the correction; four real temporary child-process
fixtures passed. Fresh actual browser QA also passed. Page, console, network,
text, image and keyboard checks remain intact. Hosted CI must pass separately;
the original failed run remains retained as evidence.

The hosted repeat still found an ENOTEMPTY race after the launcher exited. The
final correction requests graceful Browser.close through the exact endpoint
captured from the unique profile, then independently verifies that endpoint is
gone. An open, malformed or unresponsive endpoint prevents profile removal even
when the launcher has exited. Independent loopback fixtures cover the live
endpoint/exited-launcher failure, refused endpoint and malformed identity.
Actual full browser QA through a real shell launcher passed with zero new
temporary profiles left behind. Both prior hosted failures remain recorded;
fresh hosted acceptance is still required.
