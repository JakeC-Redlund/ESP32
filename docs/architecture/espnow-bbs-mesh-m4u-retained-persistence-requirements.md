# ESP-NOW BBS Mesh M4U Retained Persistence Requirements

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4U is a Tier 2 design-only requirements packet for retained persistence
after the accepted M4T volatile persistence-intent source contract.

This packet does not implement retained persistence. It does not add NVS calls,
file writes, flash writes, reboot durability, runtime persistence, live bridge
dispatch, arbitrary bridge frame parsing, serial ABI expansion, bridge ABI
changes, Gate F request or service changes, `mesh_discovery.v1` schema changes,
timer creation, firmware runtime acceptance, live task execution proof, flash,
erase, monitor, serial writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE
pairing, Android app behavior, router/admin mutation, PCAP, RF/XBee writes,
relay/load/mains work, release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4T is accepted source-only for caller-owned volatile persistence intent and
  accounting over accepted M4S request-text routing. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`.
- The development plan names retained persistence as separately gated
  continuation work after M4T.
- `SRC-ESP-IDF-NVS` is the source-indexed Espressif ESP-IDF NVS API source for
  future NVS storage design discussion.
- Current firmware audit coverage still treats `nvs_`, `nvs_flash_init`,
  `esp_flash`, `esp_partition`, `fopen`, `fwrite`, `open(`, and `write(` as
  closed markers for the M4T/M4U boundary.

## Assumptions

- The first retained persistence source gate should preserve the M4T
  caller-owned boundary and persist only a compact intent/accounting checkpoint,
  not generated bridge summary payloads or raw bridge text.
- Retained persistence must fail closed to volatile behavior until a later
  source gate proves storage initialization, record validation, recovery, and
  cleanup behavior.

## Unknowns

- NVS namespace/key policy, record format, versioning, migration, corruption
  handling, power-loss behavior, wear policy, write trigger, commit cadence,
  cleanup/deinit behavior, boot recovery behavior, stack impact, first-flash
  behavior, rollback, live bridge transcript, and live proof remain unaccepted.

## Requirements For A Future Source Gate

- The future source gate must name its storage owner, namespace/key policy,
  initialization sequence, deinitialization sequence, and recovery path before
  any storage API is called.
- The persisted record must be bounded and versioned, and must include only
  local intent/accounting fields derived from M4T: schema identity, record
  version, last normalized discovery request, last status, attempted count,
  staged count, rejected count, and staged byte total.
- The future source gate must not persist generated bridge summary payloads,
  raw bridge frame text, serial bytes, BBS message payloads, credentials,
  PMK/LMK material, peer keys, queue contents, owner claim tokens, runtime task
  handles, or live transport state.
- Missing, corrupt, unsupported-version, or validation-failed storage records
  must leave runtime state empty/volatile and expose a bounded local status
  summary; they must not trigger live bridge dispatch or serial output.
- A write policy must be explicit. The future source gate must name whether
  writes are caller-triggered, lifecycle-triggered, or runtime-triggered, and
  must include a wear/commit budget before accepting any retained write path.
- Recovery must be explicit and separately testable. A recovered checkpoint may
  restore only retained intent/accounting fields after validation; it may not
  replay bridge requests or dispatch live actions.
- The future source gate must preserve `mesh_discovery.v1`, accepted discovery
  request names, Gate F bridge request and service-code membership,
  coordinator serial ABI, bridge ABI, Win31 transport, and accepted
  serial-nullmodem live-path boundaries.
- Validation must include host/unit tests for missing record, corrupt record,
  unsupported version, bounded-field limits, non-ASCII rejection, no secret
  persistence, no generated summary retention, no runtime scheduler stepping,
  no queue/owner mutation during recovery, cleanup/deinit, and closed-surface
  scans.

## Validation Status

- Pre-mutation reviewers approved opening the narrowed M4U design-only boundary
  8/8 weighted with no P1/P2 blockers. Lifecycle listing was not exposed by the
  available tools; both read-only reviewers were waited and closed after output
  capture.
- Record, source, docs, path, data, agent-process, pages, and skills audits
  passed. `git diff --check` passed. The closed-surface scan found only the
  explicit denied-marker list in this requirements note, not storage API calls
  or source/runtime openings.
- Final post-review passed 8/8 weighted after the status-ledger P2 was resolved. No P1/P2 blockers remain.

## Decision

M4U is accepted design-only for retained-persistence requirements after local validation and final 8/8 weighted post-review. It is not accepted retained persistence, NVS/file storage
implementation, reboot durability, runtime persistence, live bridge dispatch,
flash, serial writes, or live proof.
