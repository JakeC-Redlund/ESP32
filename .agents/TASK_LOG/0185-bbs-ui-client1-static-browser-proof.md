# Task 0185: BBS UI Client-1 Static Browser Proof

Status: completed-host-only-static-browser; live and device gates remain open

Contract: [../../AGENTS.md](../../AGENTS.md)

Date: 2026-06-05

Source IDs:
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`,
`SRC-LOCAL-CBBS-REACT-NATIVE-CLIENT-PLATFORM-2026-06-02`,
`SRC-LOCAL-BBS-UI-UI0-M2B-HOST-SLICE-2026-05-28`,
`SRC-LOCAL-BBS-UI-SYSTEM-OPERATION-PROGRAM-2026-05-28`,
`SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`,
`SRC-REACT-NATIVE-WEB-2026-06-02`,
`SRC-EXPO-SDK-56-REFERENCE-2026-06-02`,
`SRC-LOCAL-AGENTIC-PLANNING-GUIDE-2026-06-05`

## Routing

- Verified facts: `ADR-0010` accepts the CBBS Expo/React Native browser fixture
  lane. The 2026-06-02 React Native platform ledger records host-only scaffold
  and prior web export proof, but the BBS UI program still listed Client-1
  browser proof as open. Gate M4 is accepted only as a host adapter fixture and
  does not create firmware/live authority.
- Assumptions: Client-1 can be accepted by fresh same-session host-only export,
  loopback static smoke, screenshots, console/network logs, and durable records
  without changing UI behavior or connecting to devices.
- Unknowns: no live browser/device connectivity, selected-board identity,
  coordinator/router/end-node role, BLE, Web Bluetooth, Web Serial, SoftAP/LAN
  discovery, native app runtime, firmware runtime, dummy output, or release
  proof exists.
- Selected tier: Tier 2 host-only browser/static evidence and records.
- Owner role: React Native Client with UI/Protocol and QA; Agent Operations owns
  durable source/task/handoff records.
- Evidence need: ADR-0010, React Native platform ledger, BBS UI program records,
  read-only reviewer quorum, fresh Expo web export, static loopback smoke,
  desktop/mobile screenshots, console/network logs, dependency patch validation,
  and scaffold audits.
- Mutation boundary: `apps/cbbs-client/package.json`, `pnpm-lock.yaml`, Client-1
  source ledger/task/handoff/status/docs updates, and ignored local evidence
  under `research/bench-records/bbs-ui-client1/local-client1-static-browser-20260605T235752Z/`.
- Reviewer quorum: lifecycle listing was not visible in available tools before
  spawning. Two read-only project-local reviewers were spawned, waited, and
  closed after output capture. Weighted disposition before proof: 6/6 approved
  proceeding with conditions, threshold 70 percent. No P1 blockers remained.
  P2 conditions were fresh browser evidence, source/task/handoff/status records,
  generated-output policy, and closed-surface wording.
- Gate authority: host-only Expo dependency patch, web export, static browser
  proof, screenshots/logs in ignored local evidence, source records, docs/status
  updates, task log, and handoff. No Tier 3 or release authority is opened.
- Validation plan: run pnpm install/lint/typecheck/test/Expo Doctor/export,
  React Native scaffold audits, static smoke/browser capture, record/source/docs
  audits, `verify_scaffold.py`, and `git diff --check`.
- Trust boundary: local repo source/tests/docs, same-session loopback browser
  evidence, and source-indexed prior records. No live hardware, serial, RF,
  router/admin, BLE, firmware, release, publication, commit, push, PR, or deploy
  authority is inferred.

## Implementation

- Patched `apps/cbbs-client` SDK-56 dependencies from `expo` `56.0.8`,
  `expo-router` `56.2.8`, and transitive `@expo/metro-runtime` `56.0.13` to
  Doctor-compatible `expo` `56.0.9`, `expo-router` `56.2.9`, and direct
  `@expo/metro-runtime` `56.0.14`.
- Regenerated `pnpm-lock.yaml` through `pnpm` only.
- Regenerated ignored `apps/cbbs-client/dist` with
  `pnpm --filter @cbbs/client export:web`.
- Captured ignored local evidence at
  `research/bench-records/bbs-ui-client1/local-client1-static-browser-20260605T235752Z/`:
  export log, curl smoke headers/body, bundle hash and size inventory,
  desktop/mobile screenshots, accessibility snapshots, browser network log,
  console log, and server cleanup proof.
- Added this task record, Client-1 source ledger, source-index row, handoff, and
  status/doc updates.

## Validation

Pre-record validation passed:

- `pnpm install --frozen-lockfile` passed after the dependency patch; pnpm
  reported ignored build scripts for `msgpackr-extract` and `unrs-resolver`.
- `pnpm lint` passed.
- `pnpm typecheck` passed.
- `pnpm test` passed: 10 test suites and 74 tests.
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

## Open Evidence

- No live browser/device connectivity, selected-board identity, coordinator,
  router, or end-node role is accepted.
- No Client-2 selected-board read-only Wi-Fi proof, dummy-output proof, live
  browser proof, BLE, Web Bluetooth, Web Serial, SoftAP/LAN discovery, native
  Android/iOS/Windows runtime, Expo Go, EAS, App Center, signing, release,
  firmware runtime, mesh live proof, or hardware proof exists.

## Authority Limits

This task does not authorize native prebuild, native build/run, Expo Go,
Android/iOS/Windows device or simulator launch, RNW runtime proof, live network,
BLE pairing, Web Bluetooth, Web Serial, SoftAP/LAN discovery, bridge ABI changes,
serial ABI changes, firmware ABI changes, Gate F service-code changes, ESP-IDF
target/build/runtime proof, live mesh proof, router/admin mutation, flash,
erase, monitor, serial/RF/XBee writes, PCAP, persistent config writes, dummy
output, relay/load/mains work, release, publication, commit, push, PR, deploy,
`/etc/codex/requirements.toml`, or `admin-strict` mutation.

## Handoff

[../handoffs/0133-bbs-ui-client1-static-browser-proof-to-qa-ui-react-native.md](../handoffs/0133-bbs-ui-client1-static-browser-proof-to-qa-ui-react-native.md)

## Decision

Decision: accept Client-1 static/simulated browser proof only. Future work may
continue with copied screenshot/OCR/CV corroboration, separately gated ESP-IDF
target/build proof, or Client-2 selected-board read-only Wi-Fi proof after a
fresh Tier 3 gate.
