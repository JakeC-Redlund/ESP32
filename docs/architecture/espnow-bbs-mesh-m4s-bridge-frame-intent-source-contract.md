# ESP-NOW BBS Mesh M4S Bridge-Frame Intent Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4S is a Tier 2 pure-C firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It adds a caller-owned request-text intent
normalizer and accounting contract over the accepted M4R local dispatch
envelope.

The phrase bridge-frame intent is scoped to caller-provided ASCII request text.
This packet does not accept arbitrary bridge frame parsing, transport framing,
live bridge dispatch, serial ABI expansion, bridge ABI changes, Gate F request
or service changes, `mesh_discovery.v1` schema changes, persistence, timer
creation, firmware runtime acceptance, live task execution proof, flash, erase,
monitor, serial writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing,
Android app behavior, router/admin mutation, PCAP, RF/XBee writes,
relay/load/mains work, release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4R is accepted as a source-only local dispatch-envelope contract over M4Q.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`.
- M4S adds caller-owned
  `espnow_bbs_mesh_contract_bridge_request_text_intent_t` plus init/reset,
  route, and summary APIs in the existing `mesh_discovery_contract` component.
- The accepted discovery request names remain `discovery_snapshot`,
  `discovery_events`, `service_catalog`, and `capability_report`.
- Accepted normalized request text routes through M4R
  `espnow_bbs_mesh_contract_bridge_dispatch_envelope_route`.

## Assumptions

- A request-text intent layer is the correct next bridge-adjacent source slice
  because it verifies normalization and rejection boundaries without parsing
  transport frames, writing serial, scheduling runtime work, or mutating
  persistent state.
- Caller-owned intent state preserves the existing ownership model and keeps
  the component free of dynamic allocation and static runtime mutation.

## Unknowns

- No live bridge transcript, Win31/OPCON bridge request proof, bridge frame
  parser, serial ABI, bridge ABI, firmware runtime acceptance, persistence
  model, timer behavior, route table, parent/root behavior, healing behavior,
  watchdog behavior, stack adequacy, first-flash behavior, rollback, cleanup,
  or live proof is accepted by this gate.

## Source Boundary

- The intent route accepts only caller-provided C string request text.
- It trims only leading/trailing ASCII space, tab, CR, and LF.
- It rejects blank text, embedded whitespace/newline content, JSON-like text,
  key-value text, colon or length-prefixed text, Gate F request names,
  live-action names, unknown request names, non-ASCII text, overlong text, and
  null inputs.
- Malformed request text is rejected before M4R envelope or M4Q router
  accounting.
- Valid normalized request text updates only caller-owned intent state and then
  delegates to accepted M4R/M4Q caller-owned accounting.
- Intent summaries are bounded by the existing 512-byte ASCII summary limit and
  include `intent=request_text`, attempted/accepted/rejected counts, last
  request, last status reason, `live=blocked`, `serial=closed`, and
  `abi=closed`.

## Validation Status

- Pre-mutation reviewers approved opening the narrowed M4S source boundary 8/8
  weighted with no P1/P2 blockers. Lifecycle listing was not exposed by the
  available tools; the spawned read-only reviewers were waited and closed after
  output capture.
- Host contract tests, firmware audit, focused mesh discovery scaffold
  unittest, broader selected scaffold unittests, record/source/docs/
  agent-process/pages/React Native/skills/path audits, no-flash ESP-IDF v6.0.1
  build, generated-output cleanup, closed-surface scan, and `git diff --check`
  passed.
- The no-flash build generated `espnow_bbs_build_only.bin` size `0x22d30`; the
  1 MiB app partition had `0xdd2d0` bytes free.
- Final read-only reviewers found no source-code P1/P2 blocker. Their stale
  validation-in-progress record P2 is resolved by the M4S acceptance update.
- Post-remediation follow-up reviewers approved 8/8 with no P1/P2 blockers
  after the M4S durable records were aligned; remaining validation-in-progress
  wording is historical resolved-P2 text only.

## Decision

M4S is accepted source-only for the pure-C caller-owned request-text
intent/accounting contract over accepted M4R/M4Q. Live bridge dispatch,
arbitrary bridge frame parsing, serial ABI expansion, bridge ABI changes, Gate
F request/service changes, `mesh_discovery.v1` schema changes, persistence,
timer creation, firmware runtime acceptance, first-flash proof, and M5 live
mesh remain separate gates.
