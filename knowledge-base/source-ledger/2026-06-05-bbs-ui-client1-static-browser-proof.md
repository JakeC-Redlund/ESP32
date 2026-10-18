# Source Ledger - 2026-06-05 BBS UI Client-1 Static Browser Proof

## Scope

Tier 2 host-only static/simulated browser proof for the BBS UI System Operation
Improvement Program Client-1 gate. The proof uses the existing CBBS
Expo/React Native fixture app and loopback static web serving only.

This ledger records Client-1 browser fixture proof only. It does not accept
native prebuild, native build/run, Expo Go, Android/iOS/Windows device or
simulator launch, RNW runtime proof, live network, BLE pairing, Web Bluetooth,
Web Serial, SoftAP/LAN discovery, bridge ABI changes, serial ABI changes,
firmware ABI changes, Gate F service-code changes, ESP-IDF target/build/runtime
proof, live mesh proof, router/admin mutation, flash, erase, monitor,
serial/RF/XBee writes, PCAP, persistent config writes, dummy output,
relay/load/mains, release, publication, commit, push, PR, or deploy.

## Source IDs

- `SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`
- `SRC-LOCAL-CBBS-REACT-NATIVE-CLIENT-PLATFORM-2026-06-02`
- `SRC-LOCAL-BBS-UI-UI0-M2B-HOST-SLICE-2026-05-28`
- `SRC-LOCAL-BBS-UI-SYSTEM-OPERATION-PROGRAM-2026-05-28`
- `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`
- `SRC-REACT-NATIVE-WEB-2026-06-02`
- `SRC-EXPO-SDK-56-REFERENCE-2026-06-02`
- `SRC-LOCAL-AGENTIC-PLANNING-GUIDE-2026-06-05`

## Verified Facts

- `ADR-0010` accepts Expo SDK 56 and React Native Web browser fixture work for
  the CBBS client/operator app lane.
- The BBS UI program defined Client-1 as simulated/static or read-only browser
  proof with screenshots, console/network logs, smoke checks, and optional
  CV/OCR corroboration.
- Before this task, Client-1 was still listed as unaccepted in the status
  ledger. The 2026-06-02 React Native platform ledger proved the platform
  scaffold, not this BBS UI Client-1 gate.
- Same-session proof regenerated `apps/cbbs-client/dist`, served it only on
  `127.0.0.1:4173`, and captured desktop/mobile screenshots plus
  console/network evidence.
- `expo-doctor` required SDK-56 patch alignment. The accepted dependency patch
  updates only `apps/cbbs-client/package.json` and `pnpm-lock.yaml`.

## Assumptions

- Static loopback browser evidence is sufficient for Client-1 because this gate
  is explicitly host-only and simulated/static.
- The ignored evidence path can hold screenshots, generated dist hashes, logs,
  and curl output without becoming source.
- The single `favicon.ico` 404 in the browser console is a static-server artifact
  and not a React Native redbox, loading-only state, or app runtime failure.

## Unknowns

- No live browser/device connectivity, selected-board identity, coordinator,
  router, end-node role, Client-2 Wi-Fi proof, dummy output, BLE, Web Bluetooth,
  Web Serial, SoftAP/LAN discovery, native app runtime, firmware runtime, or
  release proof exists.

## Reviewer Quorum

Visible lifecycle state was not listable by the available subagent tools before
spawning. Two read-only reviewers were spawned, waited, and closed after output
capture.

| Role | Weight | Vote | Disposition |
| --- | ---: | --- | --- |
| React Native/browser-fixture reviewer | 3 | approve with conditions | Required fresh export/static smoke/browser screenshot, console/network logs, loopback-only serving, and closed-surface wording. |
| QA/evidence-record reviewer | 3 | approve proceeding; veto acceptance until evidence/records existed | Required source ledger/source-index/task/handoff/status updates, ignored evidence path, generated-output policy, and no native/live claims. |

Weighted disposition for proceeding: 6/6 approved for the named Tier 2
host-only mutation boundary. Threshold: 70 percent. No P1 blockers remained.
The P2 acceptance conditions are addressed by the evidence packet, source
ledger, source-index row, task record, handoff, docs/status updates, and
validation results.

## Evidence Packet

Ignored local evidence path:
`research/bench-records/bbs-ui-client1/local-client1-static-browser-20260605T235752Z/`

Evidence contents:

- `export-web.log`: Expo web export command log.
- `curl-headers.txt`, `curl-index.html`, `static-smoke.log`: loopback static
  smoke proof.
- `dist-sha256.txt`, `dist-file-inventory.txt`, `evidence-sizes.txt`: generated
  static bundle inventory.
- `desktop-1280x800.png`, `mobile-390x844.png`: loaded fixture UI screenshots.
- `page-desktop.yml`, `page-mobile.yml`: Playwright accessibility snapshots.
- `browser-network.log`: loopback request log.
- `browser-console.log`: one `favicon.ico` 404; no app runtime/redbox error.
- `server-cleanup.log`: empty after cleanup, indicating no port 4173 listener.

## Validation

Pre-record validation passed:

- `pnpm install --frozen-lockfile` passed after dependency patch; pnpm reported
  ignored build scripts for `msgpackr-extract` and `unrs-resolver`.
- `pnpm lint` passed.
- `pnpm typecheck` passed.
- `pnpm test` passed: 10 suites and 74 tests.
- `pnpm doctor:expo` passed: 21/21 checks.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py`
  passed: `PASS: CBBS React Native scaffold audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_react_native_scaffold tests.scaffold_audits.test_agent_process_classifiers`
  passed: 21 tests OK.
- `pnpm --filter @cbbs/client export:web` passed and exported `dist`.
- Static smoke `curl -fsS -D ... -o ... http://127.0.0.1:4173/` passed with
  HTTP 200 and `Content-Length: 34832`.
- Browser proof captured desktop `1280x800` and mobile `390x844` full-page
  screenshots showing loaded CBBS fixture UI and Closed Authority controls.
- Browser network log contained only loopback static requests:
  `GET /` HTTP 200 and `GET /_expo/static/js/web/...js` HTTP 304.
- Browser console log contained one static `favicon.ico` 404 and no app
  redbox/loading-only/runtime error.
- The loopback server was stopped and `lsof` showed no port 4173 listener.

Post-record validation passed:

- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 184`
  passed: `PASS: ESP32 durable-record audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
  passed with exit code 0.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
  passed: `PASS: ESP32 agent-process audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py`
  passed: `PASS: ESP32 skill audit succeeded`.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
  passed: `PASS: ESP32 scaffold validation succeeded`.
- `git diff --check` passed with exit code 0.

## Decision

Decision: accept Client-1 static/simulated browser proof only. Future work may
continue with copied screenshot/OCR/CV corroboration, separately gated ESP-IDF
target/build proof, or Client-2 selected-board read-only Wi-Fi proof after a
fresh Tier 3 gate.
