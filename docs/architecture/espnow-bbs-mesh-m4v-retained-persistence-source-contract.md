# ESP-NOW BBS Mesh M4V Retained Persistence Source Contract

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4V is a Tier 2 firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It implements a separate
`mesh_retained_persistence` component after the accepted M4U requirements.

This packet accepts source-only NVS-facing adapter code and a host-testable
retained checkpoint codec after final 8/8 weighted post-review. It does not run NVS on hardware, prove reboot
durability, prove runtime persistence, integrate retained recovery into
`app_main`, dispatch live bridge requests, parse arbitrary bridge frames,
change the serial ABI, change the bridge ABI, change Gate F requests or
service codes, change `mesh_discovery.v1`, create timers, accept firmware
runtime behavior, prove live task execution, flash, erase, monitor, write
serial, run ESP-NOW, run ESP-WIFI-MESH, pair BLE, prove Android behavior,
mutate router/admin state, capture PCAP, write RF/XBee, operate relay/load/
mains surfaces, release, publish, commit, push, open a PR, or deploy.

## Verified Facts

- M4U is accepted design-only and requires any future retained-persistence
  source gate to name storage owner, namespace/key policy, init/deinit,
  recovery, write policy, bounded/versioned M4T-derived fields, corrupt/
  missing/unsupported handling, payload exclusions, no scheduler stepping, no
  queue/owner mutation during recovery, and closed-surface scans. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`.
- `SRC-ESP-IDF-NVS` is the source-indexed ESP-IDF NVS API source for the NVS
  storage API surface used by the adapter.
- M4V adds `firmware/projects/espnow-bbs/components/mesh_retained_persistence/`
  as a separate component. The existing `mesh_discovery_contract` and
  `app_main` stay storage-free.
- The retained record is fixed at 96 bytes, versioned as record version 1, and
  stores only `mesh_discovery.v1`, last normalized discovery request, last M4T
  status, attempted/staged/rejected counters, and staged byte total.

## Assumptions

- A source-only adapter with host-testable codec coverage is the correct next
  slice before live NVS execution or reboot durability proof.
- Caller-triggered save is the least broad write policy for the first
  retained-persistence source gate because no lifecycle or runtime scheduler
  write timing has been accepted.

## Unknowns

- Power-loss behavior, flash wear behavior beyond one explicit commit per
  caller-triggered save, live NVS partition state, first-flash behavior, reboot
  recovery behavior, stack impact under live firmware, runtime timing, bridge
  transcript behavior, Win31/OPCON behavior, rollback, and live proof remain
  unaccepted.

## Source Boundary

- Storage owner: `mesh_ingress`.
- NVS namespace/key policy: namespace `mesh_retained`, key `m4t_checkpoint`.
- Initialization sequence: callers initialize
  `espnow_bbs_mesh_retained_persistence_storage_t`, then explicitly call
  `espnow_bbs_mesh_retained_persistence_storage_init` before load/save/erase.
- Deinitialization sequence: callers explicitly call
  `espnow_bbs_mesh_retained_persistence_storage_deinit`; the storage context is
  marked not initialized whether deinit succeeds or fails.
- Recovery path: load opens the namespace read-only, reads the fixed record,
  validates magic, version, checksum, schema, counters, request safety, and
  status range, then applies only M4T intent/accounting fields to caller-owned
  volatile persistence intent state.
- Missing, corrupt, wrong-length, unsupported-version, unsupported-schema,
  unsafe-field, or validation-failed records clear caller-owned persistence
  intent to empty volatile state and return a bounded local status.
- Write policy: caller-triggered save only. Each accepted save encodes a fixed
  checkpoint and performs one `nvs_set_blob` followed by one `nvs_commit`.
- Erase policy: caller-triggered erase only. Existing key erase performs one
  `nvs_erase_key` followed by one `nvs_commit`; missing key returns missing.
- The adapter never stores generated bridge summaries, raw bridge text, serial
  bytes, BBS message payloads, credentials, PMK/LMK material, peer keys, queue
  contents, owner claim tokens, runtime task handles, or live transport state.
- Recovery does not step the runtime scheduler and does not mutate queue or
  owner state.

## Validation Status

- Pre-mutation reviewers approved opening the narrowed M4V source boundary 8/8
  weighted with no P1/P2 blockers. Lifecycle listing was not exposed by the
  available tools; both read-only reviewers were waited and closed after output
  capture.
- Host retained-persistence tests, existing discovery-contract host tests,
  selected scaffold unittests, firmware/data/record/source/docs/path/
  agent-process/pages/React Native/skills audits, targeted closed-surface
  scans, `git diff --check`, and no-flash ESP-IDF v6.0.1 build passed before
  final post-review.
- The no-flash build in
  `/tmp/esp32-espnow-bbs-m4v-retained-persistence-source` generated
  `espnow_bbs_build_only.bin` size `0x22d20` with `0xdd2e0` bytes free in the
  1 MiB app partition.
- First post-mutation reviewer quorum found two P2 blockers: wrong-length NVS
  blob load could leave stale caller intent, and top-level durable-record
  wording said accepted before final post-review. The P2s were remediated by
  clearing intent on mapped NVS blob read errors, adding a host regression test,
  and changing the top-level wording to proposed before final post-review.
- After P2 remediation, the host tests, selected scaffold unittests, firmware
  audit, record/source/docs/path/data/agent-process/pages/React Native/skills
  audits, no-flash ESP-IDF build, generated-output cleanup, stale acceptance
  wording scan, and `git diff --check` were rerun and passed.
- Final post-mutation reviewer quorum passed 8/8 weighted with no P1/P2
  blockers after the wrong-length NVS and source-index wording P2s were
  resolved.

## Decision

M4V is accepted source-only for retained-persistence adapter code plus
host-testable retained checkpoint handling after final 8/8 weighted
post-review. It is not accepted reboot durability, runtime persistence proof,
live NVS execution proof, live bridge dispatch, flash, serial writes, or live
proof.
