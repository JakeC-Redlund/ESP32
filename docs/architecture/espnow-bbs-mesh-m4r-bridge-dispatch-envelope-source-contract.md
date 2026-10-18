# ESP-NOW BBS Mesh M4R Bridge Dispatch Envelope Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4R is a Tier 2 pure-C firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It adds a caller-owned local dispatch
envelope and accounting contract over the accepted M4Q local request router.

This packet does not accept live bridge dispatch, bridge frame parsing, serial
ABI expansion, bridge ABI changes, Gate F request or service changes,
`mesh_discovery.v1` schema changes, persistence, timer creation, firmware
runtime acceptance, live task execution proof, flash, erase, monitor, serial
writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains work,
release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4Q is accepted as a source-only local request router over M4P. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`.
- M4R adds caller-owned
  `espnow_bbs_mesh_contract_bridge_dispatch_envelope_t` plus init/reset,
  route, and summary APIs in the existing `mesh_discovery_contract` component.
- The accepted discovery request names remain `discovery_snapshot`,
  `discovery_events`, `service_catalog`, and `capability_report`.
- The local envelope routes through the accepted M4Q
  `espnow_bbs_mesh_contract_bridge_request_route` API and records attempted,
  would-dispatch, rejected, and last-status accounting in caller-owned
  envelope state.

## Assumptions

- A local dispatch envelope is the correct next bridge-adjacent source slice
  because it verifies response accounting over M4Q without parsing bridge
  frames, writing serial, scheduling runtime work, or mutating persistent state.
- Caller-owned envelope state preserves the existing ownership model and keeps
  the component free of dynamic allocation and static runtime mutation.

## Unknowns

- No live bridge transcript, Win31/OPCON bridge request proof, bridge frame
  parser, serial ABI, bridge ABI, firmware runtime acceptance, persistence
  model, timer behavior, route table, parent/root behavior, healing behavior,
  watchdog behavior, stack adequacy, first-flash behavior, rollback, cleanup,
  or live proof is accepted by this gate.

## Source Boundary

- The envelope accepts only the existing discovery request vocabulary by exact
  match through M4Q.
- The envelope rejects null, non-ASCII, and unknown request names; null router;
  missing caller-owned summary state; null summary output; and undersized
  summary buffers.
- A null envelope returns `ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL` without
  mutating the router because there is no caller-owned envelope state to
  update.
- Valid envelope calls update only the caller-owned envelope accounting fields,
  M4Q router accounting fields, and last request/status fields.
- The route API does not mutate caller-owned event queue, owner, or runtime
  scheduler state while composing M4Q/M4P summaries.
- Envelope summaries are bounded by the existing 512-byte ASCII summary limit
  and include `envelope=local_dispatch`, attempted/would/rejected counts, last
  request, last status reason, `live=blocked`, and `serial=closed`.

## Validation Status

- Pre-mutation reviewers approved opening the named M4R source boundary 8/8
  weighted with no P1/P2 blockers. Lifecycle listing was not exposed by the
  available tools; the spawned read-only reviewers were waited and closed after
  output capture.
- Host contract tests, firmware audit, and focused mesh discovery scaffold
  unittest passed.
- Broader selected scaffold unittest suite, record/source/docs/agent-process/
  pages/React Native/skills/path audits, no-flash ESP-IDF v6.0.1 build,
  generated-output cleanup, closed-surface scan, and `git diff --check` passed.
- The no-flash build generated `espnow_bbs_build_only.bin` size `0x22d20`; the
  1 MiB app partition had `0xdd2e0` bytes free.
- Final read-only reviewers found no source-code P1/P2 blocker. Their sole P2
  was stale pending record/status wording, resolved by the M4R acceptance update.

## Decision

M4R is accepted source-only for the pure-C caller-owned local
dispatch-envelope/accounting contract over accepted M4Q request routing. Live
bridge dispatch, bridge frame parsing, serial ABI expansion, bridge ABI changes,
Gate F request/service changes, `mesh_discovery.v1` schema changes,
persistence, timer creation, firmware runtime acceptance, first-flash proof,
and M5 live mesh remain separate gates.
