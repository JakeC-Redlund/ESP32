# ESP-NOW BBS Mesh M4T Persistence Intent Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4T is a Tier 2 pure-C firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It adds a caller-owned volatile
persistence-intent and accounting contract over the accepted M4S request-text
intent route.

This packet does not accept retained persistence, NVS writes, file writes,
reboot durability, runtime persistence, live bridge dispatch, arbitrary bridge
frame parsing, serial ABI expansion, bridge ABI changes, Gate F request or
service changes, `mesh_discovery.v1` schema changes, timer creation, firmware
runtime acceptance, live task execution proof, flash, erase, monitor, serial
writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android app
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains work,
release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4S is accepted as a source-only request-text intent/accounting contract over
  M4R/M4Q. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`.
- The consolidated development plan names persistence work as a separately
  gated continuation after M4S.
- M4T adds caller-owned
  `espnow_bbs_mesh_contract_persistence_intent_t` plus init/reset, stage, and
  summary APIs in the existing `mesh_discovery_contract` component.
- Accepted staging calls M4S request-text routing and records only counters,
  last normalized request/status, and staged byte totals.

## Assumptions

- A volatile persistence-intent layer is the correct next source slice because
  it creates an auditable persistence boundary without selecting storage media,
  NVS namespace/key policy, file format, or runtime timing.
- Caller-owned persistence-intent state preserves the existing ownership model
  and keeps the component free of dynamic allocation, retained payloads, and
  static runtime mutation.

## Unknowns

- No NVS namespace/key policy, file format, reboot recovery behavior, wear
  policy, cleanup, runtime persistence, live bridge transcript, bridge frame
  parser, serial ABI, bridge ABI, firmware runtime acceptance, timer behavior,
  route table, parent/root behavior, healing behavior, watchdog behavior, stack
  adequacy, first-flash behavior, rollback, cleanup, or live proof is accepted
  by this gate.

## Source Boundary

- The persistence stage accepts only caller-owned M4T state, caller-owned M4S
  request-text intent state, caller-owned M4R/M4Q state, caller-provided
  request text, caller-provided summary state, and caller-provided output
  buffers.
- Valid request text is normalized and routed by accepted M4S; M4T does not
  duplicate or loosen request validation.
- A successful route records a would-persist intent by incrementing volatile
  counters and staged byte totals only.
- Rejected routes do not increment staged counters.
- M4T does not retain the generated bridge summary payload.
- M4T summaries are bounded by the existing 512-byte ASCII summary limit and
  include `persistence=intent`, attempted/staged/rejected counts, last request,
  last status reason, staged bytes, `storage=volatile`, `nvs=closed`,
  `file=closed`, `live=blocked`, `serial=closed`, and `abi=closed`.

## Validation Status

- Pre-mutation reviewers approved opening the narrowed M4T source boundary 8/8
  weighted with no P1/P2 blockers. Lifecycle listing was not exposed by the
  available tools; the spawned read-only reviewers were waited and closed after
  output capture.
- Host contract tests, firmware audit, focused mesh discovery scaffold
  unittest, record/source/docs/path/data/agent-process/pages/React
  Native/skills audits, broader selected scaffold unittests, no-flash ESP-IDF
  v6.0.1 build, generated-output cleanup, closed-surface scan, and
  `git diff --check` passed.
- The no-flash build in
  `/tmp/esp32-espnow-bbs-m4t-persistence-intent-source-contract` generated
  `espnow_bbs_build_only.bin` size `0x22d20` with `0xdd2e0` bytes free in the
  1 MiB app partition.
- Final read-only post-review passed after the development-plan record-alignment P2 was resolved.

## Decision

M4T is accepted source-only for the pure-C caller-owned volatile
persistence-intent source contract over accepted M4S. It is not accepted retained
persistence, NVS/file storage, reboot durability, runtime persistence, live
bridge dispatch, flash, serial writes, or live proof.
