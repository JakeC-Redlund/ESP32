# ESP-NOW BBS Full-Service Mesh Discovery

Source index: [../../../knowledge-base/source-index.md](../../../knowledge-base/source-index.md)

## Scope

This packet defines `mesh_discovery.v1` as a host-only discovery contract for
topology, node, service, capability, BLE/Android presence, and healing-event
evidence.

The accepted baseline remains:

`OPCON.EXE -> COM1 -> DOSBox-X nullmodem -> Pi bridge -> /dev/ttyUSB0 -> ESP32 coordinator`

This packet does not run live mesh, BLE pairing, flashing, serial writes,
router/admin mutation, relay, XBee, TFT, MicroSD, load, mains, or firmware
runtime migration.

Gate M4 host adapter proof is now recorded in
[../../architecture/espnow-bbs-mesh-m4-host-adapter-proof.md](../../architecture/espnow-bbs-mesh-m4-host-adapter-proof.md).
It adds host-only adapter fixtures and bridge-bound tests.

Gate M4B build-only target proof is now recorded in
[../../architecture/espnow-bbs-mesh-m4b-build-only-target-proof.md](../../architecture/espnow-bbs-mesh-m4b-build-only-target-proof.md).
Gate M4C firmware contract proof is now recorded in
[../../architecture/espnow-bbs-mesh-m4c-firmware-contract.md](../../architecture/espnow-bbs-mesh-m4c-firmware-contract.md).
Gate M4D event queue contract proof is now recorded in
[../../architecture/espnow-bbs-mesh-m4d-event-queue-contract.md](../../architecture/espnow-bbs-mesh-m4d-event-queue-contract.md).
Gate M4E runtime/task ownership contract proof is now recorded in
[../../architecture/espnow-bbs-mesh-m4e-runtime-task-ownership-contract.md](../../architecture/espnow-bbs-mesh-m4e-runtime-task-ownership-contract.md).
Gate M4F dispatch plan contract proof is recorded in
[../../architecture/espnow-bbs-mesh-m4f-dispatch-plan-contract.md](../../architecture/espnow-bbs-mesh-m4f-dispatch-plan-contract.md).
Gate M4G scheduler integration contract proof is recorded in
[../../architecture/espnow-bbs-mesh-m4g-scheduler-integration-contract.md](../../architecture/espnow-bbs-mesh-m4g-scheduler-integration-contract.md).
Gate M4H execution accounting contract proof is recorded in
[../../architecture/espnow-bbs-mesh-m4h-execution-accounting-contract.md](../../architecture/espnow-bbs-mesh-m4h-execution-accounting-contract.md).
Gate M4I runtime cycle contract proof is recorded in
[../../architecture/espnow-bbs-mesh-m4i-runtime-cycle-contract.md](../../architecture/espnow-bbs-mesh-m4i-runtime-cycle-contract.md).
Gate M4J runtime scheduler design review is recorded in
[../../architecture/espnow-bbs-mesh-m4j-runtime-scheduler-design.md](../../architecture/espnow-bbs-mesh-m4j-runtime-scheduler-design.md).
Gate M4K runtime scheduler source contract is recorded in
[../../architecture/espnow-bbs-mesh-m4k-runtime-scheduler-source-contract.md](../../architecture/espnow-bbs-mesh-m4k-runtime-scheduler-source-contract.md).
Gate M4L static task shell source contract is recorded in
[../../architecture/espnow-bbs-mesh-m4l-static-task-shell-source-contract.md](../../architecture/espnow-bbs-mesh-m4l-static-task-shell-source-contract.md).
Gate M4M startup lifecycle requirements are recorded in
[../../architecture/espnow-bbs-mesh-m4m-startup-lifecycle-requirements.md](../../architecture/espnow-bbs-mesh-m4m-startup-lifecycle-requirements.md).
Gate M4N startup lifecycle source contract is recorded in
[../../architecture/espnow-bbs-mesh-m4n-startup-lifecycle-source-contract.md](../../architecture/espnow-bbs-mesh-m4n-startup-lifecycle-source-contract.md).
Gate M4O app_main startup integration is recorded in
[../../architecture/espnow-bbs-mesh-m4o-app-main-startup-integration.md](../../architecture/espnow-bbs-mesh-m4o-app-main-startup-integration.md).
Gate M4P bridge summary source contract is recorded in
[../../architecture/espnow-bbs-mesh-m4p-bridge-summary-source-contract.md](../../architecture/espnow-bbs-mesh-m4p-bridge-summary-source-contract.md).
Gate M4Q local bridge request router source contract is recorded in
[../../architecture/espnow-bbs-mesh-m4q-local-bridge-request-router-source-contract.md](../../architecture/espnow-bbs-mesh-m4q-local-bridge-request-router-source-contract.md).
Gate M4R bridge dispatch envelope source contract is recorded in
[../../architecture/espnow-bbs-mesh-m4r-bridge-dispatch-envelope-source-contract.md](../../architecture/espnow-bbs-mesh-m4r-bridge-dispatch-envelope-source-contract.md).
Gate M4S bridge-frame intent source contract is recorded in
[../../architecture/espnow-bbs-mesh-m4s-bridge-frame-intent-source-contract.md](../../architecture/espnow-bbs-mesh-m4s-bridge-frame-intent-source-contract.md).
Gate M4T persistence intent source contract is accepted source-only in
[../../architecture/espnow-bbs-mesh-m4t-persistence-intent-source-contract.md](../../architecture/espnow-bbs-mesh-m4t-persistence-intent-source-contract.md).
Gate M4U retained persistence requirements are recorded design-only in
[../../architecture/espnow-bbs-mesh-m4u-retained-persistence-requirements.md](../../architecture/espnow-bbs-mesh-m4u-retained-persistence-requirements.md).
Gate M4V retained persistence source contract is accepted source-only in
[../../architecture/espnow-bbs-mesh-m4v-retained-persistence-source-contract.md](../../architecture/espnow-bbs-mesh-m4v-retained-persistence-source-contract.md).
Gate M4W retained recovery app-main source contract is accepted source-only in
[../../architecture/espnow-bbs-mesh-m4w-retained-recovery-app-main-source.md](../../architecture/espnow-bbs-mesh-m4w-retained-recovery-app-main-source.md).
Gate M4X bridge readiness app-main source contract is accepted source-only in
[../../architecture/espnow-bbs-mesh-m4x-bridge-readiness-app-main-source.md](../../architecture/espnow-bbs-mesh-m4x-bridge-readiness-app-main-source.md).
These gates add an inert ESP-IDF target, no-flash local build proof, an inert
firmware contract component, a caller-owned fixed-size event queue interface,
caller-owned ownership contract, caller-owned dispatch-plan contract, and
caller-owned scheduler integration contract, and caller-owned execution
accounting contract, caller-owned runtime-cycle composition contract, a
design-only runtime scheduler requirements packet, an accepted caller-owned
runtime scheduler source contract, an accepted caller-started static task shell
source API, accepted design-only startup/lifecycle requirements, and an accepted
caller-owned startup lifecycle source API with validation and final post-review
completed, an accepted no-flash `app_main` startup integration source, an
accepted pure-C local bridge-summary selector, an accepted pure-C local bridge
request router/accounting source contract, and an accepted pure-C local
dispatch-envelope/accounting source contract, and an accepted pure-C
caller-owned request-text intent/accounting layer over M4R. Gate M4T adds a
source-only pure-C caller-owned volatile persistence-intent/accounting layer
over M4S. Gate M4U adds design-only retained-persistence requirements for a
future source gate. Gate M4V adds only a separate source-only retained
persistence component with a fixed checkpoint codec and NVS adapter source.
Gate M4W adds only source-only app-main retained recovery status capture and
keeps retained save/erase unwired after host/focused audit/no-flash-build
validation plus final 8/8 weighted post-review. Gate M4X adds only
source-only app-main local bridge-readiness status capture for fixed
`discovery_snapshot`; full validation and post-review P2 remediation passed.
These gates do not accept
firmware runtime acceptance, arbitrary bridge frame parsing, bridge ABI changes,
serial ABI changes, live bridge dispatch, live NVS execution proof, reboot
durability, runtime persistence, or live proof.

## Verified Facts

- ESP-WIFI-MESH is the v1 transport target for future self-healing design
  because the project has already proposed it in `ADR-0004`. Source IDs:
  `SRC-LOCAL-ESPNOW-NETWORK-LIVE-GATE-2026-05-23`,
  `SRC-ESP-IDF-WIFI-MESH`.
- ESP-WIFI-MESH source material covers tree topology, root/parent selection,
  routing tables, root/parent failure handling, and self-healing concepts.
  Source ID: `SRC-ESP-IDF-WIFI-MESH`.
- ESP32 Wi-Fi and BLE coexistence requires explicit review before live proof.
  Source ID: `SRC-ESP-IDF-RF-COEXIST`.
- Android BLE remains modeled as central/GATT client, with the ESP32 client
  node as BLE peripheral/GATT server. Source IDs:
  `SRC-ANDROID-BLE-OVERVIEW`, `SRC-ANDROID-BLE-GATT-CONNECT`.
- Android Bluetooth permissions and location handling remain metadata until a
  separate BLE live gate opens. Source ID:
  `SRC-ANDROID-BLUETOOTH-PERMISSIONS`.
- ESP-Mesh-Lite is comparison/future research only; ESP-MDF is not selected.
  Source IDs: `SRC-ESP-MESH-LITE-2026-05-27`,
  `SRC-ESP-MDF-ARCHIVED-2026-05-27`.

## Assumptions

- `mesh_discovery.v1` stays host-only until a later accepted ADR maps firmware
  ESP-WIFI-MESH APIs/events into the schema.
- BLE/Android discovery is presence and capability metadata only for v1.
- The Pi bridge remains the durable BBS custody and operator boundary.

## Unknowns

- No live mesh route-table, parent, root, healing, coexistence, flash, or
  cleanup proof exists.
- No BLE UUIDs, Android package, permission proof, bonding/SMP proof,
  coexistence proof, or live GATT proof exists.
- No router SSID/BSSID/channel policy, mesh ID, root-election policy,
  max-layer policy, or rollback packet is accepted for a live mesh gate.

## Schema Defaults

| Field | Value |
| --- | --- |
| `schema` | `mesh_discovery.v1` |
| `transport` | `esp-wifi-mesh` |
| `mode` | `sim` |
| `admin_gate` | `disabled` |

## Record Model

Each host discovery node record carries:

- node identity and MAC
- link type, role, layer, parent, and root
- RSSI, seen age, and health
- service and capability lists
- security summary and source evidence

Discovery payloads must not include PMKs, LMKs, BLE bonding keys, pairing
tokens, Android identifiers, raw message bodies, credential fields, or precise
location data.

## Event Model

Accepted host event names are:

- `node_seen`
- `node_lost`
- `parent_selected`
- `root_elected`
- `root_switched`
- `route_added`
- `route_removed`
- `heal_started`
- `heal_observed`
- `service_seen`
- `capability_seen`
- `ble_client_seen`

Healing acceptance remains future work. The v1 simulator only records the event
shape and host summary behavior.

## Service Catalog

The v1 host catalog includes:

- BBS messaging
- file transfer
- telemetry
- node status
- custody ACK
- non-executing control intent
- BLE client presence
- Android client metadata

## Bridge Summaries

The host simulator supports compact, schema-versioned, ASCII summaries:

| Request | Purpose |
| --- | --- |
| `discovery_snapshot` | Compact node/topology snapshot. |
| `discovery_events` | Recent discovery and healing events. |
| `service_catalog` | v1 service names. |
| `capability_report` | Capability counts and BLE/Android metadata summary. |

Responses must fit the existing 512-byte bridge line limit. These request names
do not add Gate F radio service codes and do not authorize live bridge
authority.

## Derived Dashboard Fields

Existing live-safe dashboard fields such as `mesh`, `mesh_root`,
`mesh_parent`, `mesh_layer`, `mesh_heal`, `ble`, and `android` should be
derived from `mesh_discovery.v1` summaries when this data is presented. They
are not the source of truth.

## Future Gates

- Gate M1: this host ADR/design/research plus simulator schema.
- Gate M2: DOS-C companion bridge/operator fixture support and Win31 read-only
  Network/Services summary.
- Gate M3: firmware-design review for mapping ESP-WIFI-MESH concepts/events to
  the schema; recorded as design-only in
  [../../architecture/espnow-bbs-mesh-m3-firmware-mapping.md](../../architecture/espnow-bbs-mesh-m3-firmware-mapping.md).
- Gate M4: host adapter fixture proof with event-vector, bridge-bound,
  secret-rejection, live-action-rejection, queue/custody-isolation, and
  schema/ABI-invariance tests; recorded in
  [../../architecture/espnow-bbs-mesh-m4-host-adapter-proof.md](../../architecture/espnow-bbs-mesh-m4-host-adapter-proof.md).
- Gate M4B: inert ESP-IDF target and no-flash local build proof; recorded in
  [../../architecture/espnow-bbs-mesh-m4b-build-only-target-proof.md](../../architecture/espnow-bbs-mesh-m4b-build-only-target-proof.md).
- Gate M4C: inert pure-C firmware contract component for the accepted
  discovery constants, bounds, secret checks, blocked live-action names, and
  Gate F invariants; recorded in
  [../../architecture/espnow-bbs-mesh-m4c-firmware-contract.md](../../architecture/espnow-bbs-mesh-m4c-firmware-contract.md).
  Runtime implementation remains a separate future gate.
- Gate M4D: inert caller-owned fixed-size firmware event queue contract for
  accepted discovery events, FIFO push/peek/pop, explicit full/empty
  backpressure, bounded ASCII node/detail validation, and compact local queue
  summaries; recorded in
  [../../architecture/espnow-bbs-mesh-m4d-event-queue-contract.md](../../architecture/espnow-bbs-mesh-m4d-event-queue-contract.md).
  Runtime task ownership, bridge dispatch, persistence, and live proof remain
  separate future gates.
- Gate M4E: inert caller-owned runtime/task ownership contract for known owner
  names, claim/release tokens, queue-bound ingress/drain, owner counters,
  stale-token rejection, and compact owner summaries; recorded in
  [../../architecture/espnow-bbs-mesh-m4e-runtime-task-ownership-contract.md](../../architecture/espnow-bbs-mesh-m4e-runtime-task-ownership-contract.md).
  Actual task creation, scheduler integration, bridge dispatch, persistence,
  and live proof remain separate future gates.
- Gate M4F: inert caller-owned dispatch-plan contract for deterministic
  drain/ingest/snapshot action ordering, local budget accounting, per-step
  status, compact plan summaries, and no owner/queue mutation while planning;
  recorded in
  [../../architecture/espnow-bbs-mesh-m4f-dispatch-plan-contract.md](../../architecture/espnow-bbs-mesh-m4f-dispatch-plan-contract.md).
  Host/build validation and final 8/8 weighted reviewer approval are complete.
  Scheduler integration, actual task creation, bridge dispatch, persistence,
  and live proof remain separate future gates.
- Gate M4G: inert caller-owned scheduler integration contract for known local
  schedule profiles, bounded tick accounting, deterministic M4F dispatch-plan
  staging, compact scheduler summaries, and no owner/queue mutation while
  staging; recorded in
  [../../architecture/espnow-bbs-mesh-m4g-scheduler-integration-contract.md](../../architecture/espnow-bbs-mesh-m4g-scheduler-integration-contract.md).
  Host/build validation and final 8/8 weighted reviewer approval are complete.
  Actual scheduler runtime, task creation, bridge dispatch, persistence, and
  live proof remain separate future gates.
- Gate M4H: inert caller-owned execution-accounting contract for bounded
  execution cycles, same-owner ready-step accounting, non-ready planned-step
  rejection, mixed-owner rejection, compact execution-accounting summaries, and
  no owner/queue/scheduler/plan mutation while accounting; recorded in
  [../../architecture/espnow-bbs-mesh-m4h-execution-accounting-contract.md](../../architecture/espnow-bbs-mesh-m4h-execution-accounting-contract.md).
  Host/build validation, reviewer P2 stale-record remediation, and final record
  alignment are complete. Actual execution runtime, scheduler runtime, task
  creation, bridge dispatch, persistence, and live proof remain separate future
  gates.
- Gate M4I: inert caller-owned runtime-cycle contract for composing M4G
  scheduler staging and M4H execution accounting through local temporary
  copies, committing caller-owned state only after a full cycle succeeds, and
  producing compact runtime-cycle summaries; recorded in
  [../../architecture/espnow-bbs-mesh-m4i-runtime-cycle-contract.md](../../architecture/espnow-bbs-mesh-m4i-runtime-cycle-contract.md).
  Host/build/audit validation, generated-output cleanup, and final
  post-review record alignment are complete. Actual scheduler runtime,
  execution runtime, task creation, bridge dispatch, persistence, and live
  proof remain separate future gates.
- Gate M4J: design-only runtime scheduler review for the next source gate,
  including task ownership, core affinity, priority, stack budget, timer
  choice, callback dispatch model, bounded backpressure, bridge-visible
  counters, volatile-only state, cleanup/deinit behavior, and validation
  requirements; recorded in
  [../../architecture/espnow-bbs-mesh-m4j-runtime-scheduler-design.md](../../architecture/espnow-bbs-mesh-m4j-runtime-scheduler-design.md).
  Runtime scheduler source implementation, ESP-IDF task/timer code, bridge
  dispatch, persistence, flash, serial, and live proof remain separate future
  gates.
- Gate M4K: caller-owned runtime scheduler source contract over M4I
  runtime-cycle composition, accepting only `caller_invoked`, `task_none`, and
  `timer_none` policies, rejecting selected core affinity, nonzero priority,
  nonzero stack bytes, static-state requests, invalid period/yield values, and
  unknown policies, and committing caller-owned scheduler state only after an
  existing runtime-cycle step succeeds; recorded in
  [../../architecture/espnow-bbs-mesh-m4k-runtime-scheduler-source-contract.md](../../architecture/espnow-bbs-mesh-m4k-runtime-scheduler-source-contract.md).
  Host/build/audit validation, generated-output cleanup, targeted
  closed-surface scan, and post-review record alignment are complete.
  Actual task creation, timer creation, firmware runtime, bridge dispatch,
  persistence, flash, serial, and live proof remain separate future gates.
- Gate M4L: separate ESP-IDF-only static task shell source API using
  caller-provided `StaticTask_t` and `StackType_t` buffers and
  `xTaskCreateStaticPinnedToCore` to invoke the accepted M4K scheduler step as
  an external caller; recorded in
  [../../architecture/espnow-bbs-mesh-m4l-static-task-shell-source-contract.md](../../architecture/espnow-bbs-mesh-m4l-static-task-shell-source-contract.md).
  Full validation, no-flash build, generated-output cleanup, closed-surface
  scan, and final post-review are complete. Automatic startup from `app_main`,
  live task execution proof, timer creation, queues, callbacks, bridge dispatch,
  persistence, flash, serial, and live proof remain separate future gates.
- Gate M4M: design-only startup/lifecycle requirements after M4L; recorded in
  [../../architecture/espnow-bbs-mesh-m4m-startup-lifecycle-requirements.md](../../architecture/espnow-bbs-mesh-m4m-startup-lifecycle-requirements.md).
  It names required lifecycle states, accepted/rejected transition handling,
  caller-owned versus static state decisions, task-buffer lifetime, failure
  cleanup, and validation expectations for a later source gate. Clean validation
  and post-review are complete for design-only requirements. Automatic startup
  from `app_main`, firmware source implementation, bridge dispatch, persistence,
  flash, serial, and live proof remain separate future gates.
- Gate M4N: caller-owned startup lifecycle source API around M4L; recorded in
  [../../architecture/espnow-bbs-mesh-m4n-startup-lifecycle-source-contract.md](../../architecture/espnow-bbs-mesh-m4n-startup-lifecycle-source-contract.md).
  It names `uninitialized`, `configured`, `starting`, `running`,
  `stop_requested`, `stopped`, and `failed` states, embeds the M4L task object,
  requires reconfigure after stopped, and clears copied task config plus the
  embedded M4L task object on failure/deinit paths. Full validation, no-flash
  ESP-IDF build, generated-output cleanup, diff hygiene, stale-record
  remediation, and final 8/8 weighted post-review passed. Automatic startup from
  `app_main`, bridge dispatch, persistence, flash, serial, firmware runtime
  acceptance, and live proof remain separate future gates.
- Gate M4O: no-flash `app_main` startup integration source contract over M4N;
  recorded in
  [../../architecture/espnow-bbs-mesh-m4o-app-main-startup-integration.md](../../architecture/espnow-bbs-mesh-m4o-app-main-startup-integration.md).
  It defines static app-main-owned event queue, owner, runtime scheduler,
  lifecycle, task buffer, and stack storage; configures `mesh_ingress` with the
  `balanced` profile and M4K policy strings; configures M4N; and calls the M4N
  start path. Focused scaffold tests, host contract tests, scaffold audits,
  no-flash ESP-IDF v6.0.1 build, generated-output cleanup, closed-surface scan,
  `git diff --check`, and final 8/8 weighted post-review passed. Firmware
  runtime acceptance, bridge dispatch, persistence, flash, serial, and live proof
  remain separate future gates.
- Gate M4P: pure-C local bridge-summary source contract over the existing
  caller-owned discovery state; recorded in
  [../../architecture/espnow-bbs-mesh-m4p-bridge-summary-source-contract.md](../../architecture/espnow-bbs-mesh-m4p-bridge-summary-source-contract.md).
  It selects bounded local summaries for `discovery_snapshot`,
  `discovery_events`, `service_catalog`, and `capability_report`, validates
  request names with the existing discovery request vocabulary, rejects null,
  non-ASCII, unknown, and undersized-buffer cases, and does not mutate the
  caller-owned queue, owner, or runtime scheduler state. Full validation and
  final 8/8 weighted post-review passed after the record-alignment P2 was
  resolved. Live bridge dispatch, serial ABI expansion, Gate F request/service
  changes, schema changes, persistence, timers, firmware runtime acceptance,
  flash, serial, and live proof remain separate future gates.
- Gate M4R: pure-C local dispatch-envelope source contract over the accepted
  M4Q request router; recorded in
  [../../architecture/espnow-bbs-mesh-m4r-bridge-dispatch-envelope-source-contract.md](../../architecture/espnow-bbs-mesh-m4r-bridge-dispatch-envelope-source-contract.md).
  It calls `espnow_bbs_mesh_contract_bridge_request_route`, records
  attempted/would-dispatch/rejected/last-status accounting in caller-owned
  envelope state, keeps summary output marked `live=blocked` and
  `serial=closed`, and does not mutate the caller-owned queue, owner, or
  runtime scheduler state. Full source-only validation and final post-review
  passed after the record-alignment P2 was resolved. Live bridge dispatch,
  bridge frame parsing, serial ABI expansion, bridge ABI changes, Gate F
  request/service changes, schema changes, persistence, timers, firmware runtime
  acceptance, flash, serial, and live proof remain separate future gates.
- Gate M4S: pure-C caller-owned request-text intent source contract over the
  accepted M4R dispatch envelope; recorded in
  [../../architecture/espnow-bbs-mesh-m4s-bridge-frame-intent-source-contract.md](../../architecture/espnow-bbs-mesh-m4s-bridge-frame-intent-source-contract.md).
  It trims only leading/trailing ASCII space, tab, CR, and LF; rejects blank,
  embedded-whitespace, structured, Gate F, live-action, unknown, non-ASCII,
  overlong, and null request text before envelope/router accounting; delegates
  accepted normalized names to M4R; and keeps `live=blocked`,
  `serial=closed`, and `abi=closed`. Full validation and final post-review
  passed after the record-alignment P2 was resolved. Live bridge dispatch,
  arbitrary bridge frame parsing, serial ABI expansion, bridge ABI changes,
  Gate F request/service changes, schema
  changes, persistence, timers, firmware runtime acceptance, flash, serial, and
  live proof remain separate future gates.
- Gate M4T: pure-C caller-owned volatile persistence-intent source contract
  over accepted M4S request-text routing; recorded in
  [../../architecture/espnow-bbs-mesh-m4t-persistence-intent-source-contract.md](../../architecture/espnow-bbs-mesh-m4t-persistence-intent-source-contract.md).
  It records only counters, last normalized request/status, and staged byte
  totals in caller-owned state; does not retain generated bridge summary
  payloads; and keeps `storage=volatile`, `nvs=closed`, `file=closed`,
  `live=blocked`, `serial=closed`, and `abi=closed`. Host contract tests,
  firmware/path/record/source/docs audits, focused and broader selected
  scaffold unittests, no-flash ESP-IDF v6.0.1 build, cleanup checks,
  closed-surface scan, and `git diff --check` passed; final post-review passed after the development-plan record-alignment
  P2 was resolved. Retained persistence, NVS/file storage, reboot durability, runtime
  persistence, live bridge dispatch, arbitrary bridge frame parsing, serial ABI
  expansion, bridge ABI changes, Gate F request/service changes, schema
  changes, timers, firmware runtime acceptance, flash, serial, and live proof
  remain separate future gates.
- Gate M4U: design-only retained-persistence requirements after accepted M4T;
  recorded in
  [../../architecture/espnow-bbs-mesh-m4u-retained-persistence-requirements.md](../../architecture/espnow-bbs-mesh-m4u-retained-persistence-requirements.md).
  It requires any future source gate to name storage owner, namespace/key
  policy, initialization, deinitialization, recovery, write policy,
  bounded/versioned M4T-derived checkpoint fields, corrupt/missing/
  unsupported-record handling, secret exclusion, no generated bridge summary
  retention, no runtime scheduler stepping, no queue/owner mutation during
  recovery, and closed-surface scans before any storage API is called. It does
  not accept NVS/file implementation, reboot durability, runtime persistence,
  flash, serial, or live proof.
- Gate M4V: source-only retained-persistence component after accepted M4U;
  recorded in
  [../../architecture/espnow-bbs-mesh-m4v-retained-persistence-source-contract.md](../../architecture/espnow-bbs-mesh-m4v-retained-persistence-source-contract.md).
  It adds `mesh_retained_persistence`, a fixed 96-byte version-1 checkpoint
  codec for bounded M4T-derived fields, and NVS adapter source with explicit
  init/deinit/load/save/erase APIs. Host retained-persistence tests, focused
  scaffold tests, firmware audit, no-flash ESP-IDF v6.0.1 build, and
  generated-output cleanup passed; final 8/8 weighted post-review passed after
  the wrong-length NVS and source-index wording P2s were resolved. It does not
  accept live NVS execution proof, reboot durability, runtime persistence proof,
  live bridge dispatch, first-flash proof, flash, serial, or live proof.
- Gate M4W: accepted source-only retained recovery app-main integration after
  accepted M4V; recorded in
  [../../architecture/espnow-bbs-mesh-m4w-retained-recovery-app-main-source.md](../../architecture/espnow-bbs-mesh-m4w-retained-recovery-app-main-source.md).
  It initializes retained intent to empty volatile state, calls retained
  storage context init, storage init, and storage load, records init/load
  statuses, keeps storage save/erase unwired, and passed host/focused
  audit/no-flash-build validation plus final 8/8 weighted post-review. It does
  not accept live NVS execution proof, reboot durability, runtime persistence
  proof, flash, serial, or live proof.
- Gate M4X: accepted source-only bridge-readiness app-main
  integration after accepted M4W; recorded in
  [../../architecture/espnow-bbs-mesh-m4x-bridge-readiness-app-main-source.md](../../architecture/espnow-bbs-mesh-m4x-bridge-readiness-app-main-source.md).
  It initializes static local bridge summary/router/envelope/request-text
  intent state, routes only fixed `discovery_snapshot` into a bounded static
  summary buffer, records status and line bytes, and preserves lifecycle
  startup. Full validation and post-review P2 remediation passed. It does not accept live bridge
  dispatch, arbitrary bridge frame parsing, bridge ABI changes, serial ABI
  expansion, firmware runtime proof, live NVS proof, first-flash proof, flash,
  serial, or live proof.
- Gate M5: live mesh proof only after fresh identity, backups, hashes,
  router/channel/mesh ID policy, rollback, route-table proof, root/parent
  failure proof, BLE coexistence proof, transcript, and cleanup.

## Validation

The host simulator tests cover schema defaults, topology snapshots, events,
service and capability reports, BLE/Android presence metadata, duplicate node
coalescing, stale/lost transitions, healing-event summaries, 512-byte bridge
bounds, recursive secret-field rejection, closed live controls, runtime report
inclusion, unchanged Gate F radio service-code membership, M4 host adapter
event-vector/invariance fixtures, M4C firmware contract host/build tests, M4D
caller-owned event queue contract tests, M4E caller-owned ownership contract
tests, M4F caller-owned dispatch-plan contract tests, and M4G caller-owned
scheduler integration contract tests, and M4H caller-owned execution accounting
contract tests, M4I caller-owned runtime-cycle contract tests, M4J
  design-record/source-index validation, and focused M4K caller-owned runtime
  scheduler source contract tests. Full M4K and M4L validation/post-review are
  satisfied. M4M design-record/source-index validation and post-review are
  satisfied for design-only requirements. Focused M4N startup lifecycle source
  scaffold tests, host tests, scaffold audits, no-flash ESP-IDF build,
  generated-output cleanup, `git diff --check`, stale-record remediation, and
  final 8/8 weighted post-review passed. Focused M4O app_main startup
  integration tests, host contract tests, scaffold audits, no-flash build,
  generated-output cleanup, closed-surface scan, and final 8/8 weighted
  post-review passed. Focused M4P bridge-summary source tests, host contract
  tests, scaffold audits, no-flash build, generated-output cleanup,
  closed-surface scan, and final 8/8 weighted post-review passed after the
  record-alignment P2 was resolved. Focused M4Q local bridge request router
  source tests, host contract tests, firmware/path/records/sources/docs/
  agent-process/pages/React Native/skills audits, no-flash build,
  generated-output cleanup, closed-surface scan, `git diff --check`, and final
  8/8 weighted post-review passed after the record-alignment P2 was resolved.
  The aggregate `scripts/verify_scaffold.py` command entered kernel I/O wait
  twice and was stopped; the covered audit groups were run individually and
  passed. Focused M4R local dispatch-envelope source tests, host contract tests,
  firmware/path/record/source/docs/agent-process/pages/React Native/skills
  audits, no-flash build, cleanup checks, closed-surface scan, `git diff
  --check`, and final 8/8 weighted post-review passed after the
  record-alignment P2 was resolved. Focused M4S request-text intent source
  tests, host contract tests, scaffold audits, no-flash build, cleanup,
  closed-surface scan, `git diff --check`, and final post-review passed after
  the record-alignment P2 was resolved. Focused M4T volatile persistence-intent
  source host tests, firmware/path/record/source/docs audits, focused and
  broader selected scaffold unittests, no-flash build, cleanup, closed-surface
  scan, and `git diff --check` passed; final post-review passed after the development-plan record-alignment P2 was resolved.
  Focused M4V retained-persistence host tests, focused scaffold tests, firmware
  audit, no-flash build, generated-output cleanup, P2 remediation validation,
  and final 8/8 weighted post-review passed. Focused M4X app-main
  bridge-readiness source tests, discovery/retained host tests, firmware audit,
  and no-flash build passed before final post-review. No firmware/runtime/live proof, live bridge dispatch,
  arbitrary bridge frame parsing, live NVS execution proof, reboot durability,
  runtime persistence, flash, or serial authority is accepted.
