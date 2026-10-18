# ESP-NOW BBS Mesh M4P Bridge Summary Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4P is a Tier 2 pure-C firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It adds a bounded local bridge-summary
selector over the accepted `mesh_discovery_contract` caller-owned state.

This packet does not accept live bridge dispatch, serial ABI expansion, Gate F
request or service changes, `mesh_discovery.v1` schema changes, persistence,
timer creation, firmware runtime acceptance, live task execution proof, flash,
erase, monitor, serial writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE
pairing, Android app behavior, router/admin mutation, PCAP, RF/XBee writes,
relay/load/mains work, release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4O is accepted as a no-flash `app_main` startup integration source gate and
  leaves bridge work, persistence work, runtime proof, first-flash proof, and
  M5 live mesh as separate gates. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`.
- M4C through M4K define pure-C caller-owned discovery, queue, owner, dispatch,
  scheduler, execution-accounting, runtime-cycle, and runtime-scheduler
  contracts with 512-byte summary bounds and no live bridge dispatch. Source
  IDs are listed in the M4P source ledger.
- Gate M4P adds
  `espnow_bbs_mesh_contract_bridge_summary_state_t` and
  `espnow_bbs_mesh_contract_bridge_summary` in the existing
  `mesh_discovery_contract` component.
- The accepted discovery request names remain `discovery_snapshot`,
  `discovery_events`, `service_catalog`, and `capability_report`.

## Assumptions

- A source-only bridge-summary selector is the correct next bridge-adjacent
  slice because it exposes local, bounded summary shaping without accepting a
  bridge request dispatcher or serial path.
- Caller-owned state pointers are the right contract boundary for M4P because
  they preserve the existing ownership model and avoid dynamic allocation.

## Unknowns

- No live bridge transcript, Win31/OPCON bridge request proof, firmware runtime
  acceptance, persistence model, timer behavior, route table, parent/root
  behavior, healing behavior, watchdog behavior, stack adequacy, first-flash
  behavior, rollback, cleanup, or live proof is accepted by this gate.

## Source Boundary

- `discovery_snapshot` selects a compact summary of schema, transport, mode,
  admin gate, queue count/drop count, owner state, runtime step count, and last
  local status.
- `discovery_events` selects a compact summary of queue count/drop count and
  oldest/newest event endpoints.
- `service_catalog` selects a compact invariant summary of Gate F service count,
  accepted discovery request count, Gate F bridge request count, first/last Gate
  F service names/codes, and the disjoint bridge boundary.
- `capability_report` selects a compact summary of transport/admin gate,
  configured flag, accepted caller-invoked/task-none/timer-none policies, and
  closed live/persistence/serial surfaces.
- The API validates request names through the existing discovery request
  vocabulary, rejects null/non-ASCII/unknown request names, rejects undersized
  caller buffers, validates 512-byte ASCII summary bounds, and does not mutate
  caller-owned queue, owner, or runtime scheduler state.

## Validation Status

- Pre-mutation reviewers approved opening the named M4P source boundary 8/8
  weighted with no P1/P2 blockers. Lifecycle listing was not exposed by the
  available tools; the spawned read-only reviewers were waited and closed after
  output capture.
- Focused scaffold tests, host contract tests, record/source/docs/agent-process/
  firmware/path audits, full scaffold validation, no-flash ESP-IDF v6.0.1
  build, generated-output cleanup, closed-surface scan, and `git diff --check`
  passed.
- The no-flash build generated
  `/tmp/esp32-espnow-bbs-m4p-bridge-summary-source-contract/espnow_bbs_build_only.bin`
  size `0x22d20`, with `0xdd2e0` bytes free in the 1 MiB app partition.
- The closed-surface scan found only the expected blocked-live-action
  `serial_write` string in the contract rejection table.
- Final post-review passed 8/8 weighted conditional approval. The sole P2
  record-alignment condition was resolved by adding M4P to the project/status
  records; no P1/P2 blockers remain for the named source-only boundary.

## Decision

M4P is accepted as a source-only bridge-summary contract gate with full
validation and final post-review passed. Live bridge dispatch, persistence,
firmware runtime acceptance, first-flash proof, and M5 live mesh remain
separate gates.
