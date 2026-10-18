# ESP-NOW BBS Mesh M4Q Local Bridge Request Router Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4Q is a Tier 2 pure-C firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It adds a caller-owned local request router
and accounting contract over the accepted M4P bridge-summary selector.

This packet does not accept live bridge dispatch, serial ABI expansion, Gate F
request or service changes, `mesh_discovery.v1` schema changes, persistence,
timer creation, firmware runtime acceptance, live task execution proof, flash,
erase, monitor, serial writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE
pairing, Android app behavior, router/admin mutation, PCAP, RF/XBee writes,
relay/load/mains work, release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4P is accepted as a source-only bridge-summary selector over existing
  caller-owned discovery state. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`.
- M4Q adds caller-owned
  `espnow_bbs_mesh_contract_bridge_request_router_t` plus init/reset, route,
  and summary APIs in the existing `mesh_discovery_contract` component.
- The accepted discovery request names remain `discovery_snapshot`,
  `discovery_events`, `service_catalog`, and `capability_report`.
- The local router routes only to the accepted M4P
  `espnow_bbs_mesh_contract_bridge_summary` API and records attempted,
  accepted, rejected, and last-status accounting in caller-owned router state.

## Assumptions

- A local request router is the correct next bridge-adjacent source slice
  because it validates request selection and accounting without parsing bridge
  frames, writing serial, scheduling runtime work, or mutating persistent state.
- Caller-owned router state preserves the existing ownership model and keeps
  the component free of dynamic allocation and static runtime mutation.

## Unknowns

- No live bridge transcript, Win31/OPCON bridge request proof, firmware runtime
  acceptance, persistence model, timer behavior, route table, parent/root
  behavior, healing behavior, watchdog behavior, stack adequacy, first-flash
  behavior, rollback, cleanup, or live proof is accepted by this gate.

## Source Boundary

- The router accepts only the existing discovery request vocabulary by exact
  match.
- The router rejects null, non-ASCII, and unknown request names; missing
  caller-owned summary state; null summary output; and undersized summary
  buffers.
- A null router returns `ESPNOW_BBS_MESH_CONTRACT_REJECT_NULL` without
  accounting mutation because there is no caller-owned router state to update.
- Valid router calls update only the caller-owned router accounting fields and
  last request/status fields.
- The route API does not mutate caller-owned event queue, owner, or runtime
  scheduler state while composing M4P summaries.
- Router summaries are bounded by the existing 512-byte ASCII summary limit and
  include `router=local_request`, attempted/accepted/rejected counts, last
  request, last status reason, and `live=blocked`.

## Validation Status

- Pre-mutation reviewers approved opening the named M4Q source boundary 8/8
  weighted with no P1/P2 blockers. Lifecycle listing was not exposed by the
  available tools; the spawned read-only reviewers were waited and closed after
  output capture.
- Focused scaffold tests, host contract tests, firmware, path, records,
  sources, docs, agent-process, pages, React Native, and skills audits,
  cleanup checks, closed-surface scan, and `git diff --check` passed.
- The aggregate `scripts/verify_scaffold.py` command was attempted twice and
  entered kernel I/O wait on `/mnt/h`; it was stopped both times. The
  individual audit groups covered by the aggregate command were run separately
  and passed, and those individual results are the scaffold acceptance
  evidence.
- No-flash ESP-IDF v6.0.1 build passed.
- The no-flash build generated
  `/tmp/esp32-espnow-bbs-m4q-local-bridge-request-router-source-contract/espnow_bbs_build_only.bin`
  size `0x22d20`, with `0xdd2e0` bytes free in the 1 MiB app partition.
- Closed-surface scan found only the expected blocked-action table string
  `serial_write`.
- Final post-review passed 8/8 weighted conditional approval after the
  record-alignment P2 was resolved. No P1/P2 blockers remain for the named
  source-only M4Q gate.

## Decision

M4Q is accepted only as a pure-C caller-owned local bridge request
router/accounting source contract over the accepted M4P bridge-summary
selector. Live bridge dispatch, persistence, firmware runtime acceptance,
first-flash proof, and M5 live mesh remain separate gates.
