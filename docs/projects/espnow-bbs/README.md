# ESP-NOW BBS Coordinator and Client Firmware

Source index: [../../../knowledge-base/source-index.md](../../../knowledge-base/source-index.md)

## Purpose

This project lane supports the DOS-C off-grid BBS architecture:

`Windows 3.1 dashboard -> DOSBox-X nullmodem -> Raspberry Pi bridge -> USB serial -> ESP32 coordinator -> ESP-NOW clients`

The ESP32 firmware responsibility is limited to the coordinator and client
radio/device behavior. Durable BBS storage, sysop UI, and firmware management
remain outside the ESP32 firmware image.

## Current Status

- ADR-0003 accepts ESP-IDF v6.0.1 for this `espnow-bbs` lane.
- The accepted live baseline is the Pi-visible ESP32 coordinator answering
  USB serial `hello`, `state`, and `diag` through the Win3.1 OPCON dashboard
  path.
- DOS-C now contains the first encrypted one-peer ESP-NOW PING/ACK
  implementation, with tracked defaults keeping encryption disabled and live
  PMK/LMK material generated only under ignored `secrets/espnow-bbs/`.
- The Windows COM6 peer was identified as ESP32-D0WDQ6 MAC
  `78:e3:6d:0a:90:14`, privately backed up, flashed with the live encrypted
  peer build, and observed sending bounded PING attempts.
- The first live encrypted proof is accepted: after fresh Pi/coordinator
  identity, USB-only/no-load/no external wiring confirmation, and private
  coordinator backup, the coordinator was flashed and Win3.1 OPCON showed
  peer `peer01`, link `espnow-enc`, peer count `1`, and moving RX/TX/ACK
  counters over the accepted Pi bridge path.
- DOS-C source contains the multi-peer dashboard slice: original Win31 retro
  splash, Message Board search/pull/post/ack controls, bounded peer/message row
  parsing, coordinator serial `peer_list`, three encrypted coordinator peer
  slots, ignored live config generation for `peer01` through `peer03`,
  `bridge-transcript.jsonl` proof output, and local-admin Gate G redacted JSON
  export under accepted `ADR-0005`.
- ESP32 tooling provides the three-peer live gate and completion gate:
  read-only preflight, prepare/flash manifest tooling, `complete` transcript
  and vision-gate auditing, structured JSONL transcript support, peer remap
  support, and current LAN-DHCP preflight support. Prepare/flash remain gated
  by fresh identity, backups, manifest review, write confirmation, and cleanup.
- A fresh read-only preflight on 2026-05-23 found exactly three Windows CP210x
  ESP32 peers on `COM4`, `COM5`, and `COM6`, all ESP32-D0WDQ6 with 4 MB flash
  and distinct MACs. Direct Pi access at `172.16.0.2` failed from the current
  host route, but the accepted WAN-side SSH-forwarded address
  `192.168.137.93` reached the Pi and passed host-key, hostname, serial, root,
  `eth0=172.16.0.2/24`, stale listener, stale process, and `/dev/ttyUSB0`
  presence gates. Pi-side `esptool` was installed from Debian Trixie and the
  live gate now uses `--no-stub` for Pi coordinator operations. The latest
  forwarded read-only preflight passed with coordinator MAC
  `78:e3:6d:10:4d:6c`, distinct from the three peers. Source ID:
  `SRC-LOCAL-ESPNOW-LIVE-PREFLIGHT-2026-05-23`.
- A later 2026-05-23 three-peer live attempt first stopped before any valid
  backup or flash when Pi Debian `esptool --no-stub read_flash` failed around
  50% with a CRC/checksum error. The blocker was resolved by using the proven
  Pi esptool venv/stub runtime for coordinator backup/flash, translating
  Windows peer artifact paths, resolving activated ESP-IDF `idf.py`, and
  isolating per-role `SDKCONFIG`. Corrected proof records four full-flash
  backups, complete manifest, coordinator plus `peer01`/`peer02`/`peer03`
  flash/verify evidence, three `espnow-enc` peers, moving RX/TX/ACK counters
  from `126/126/126` to `129/129/129`, Win31 runtime captures, Program Manager
  helper proof, and final cleanup.
  Source ID: `SRC-LOCAL-ESPNOW-THREE-PEER-LIVE-ATTEMPT-2026-05-23`.
- A later 2026-05-25 Gate H structured live rerun captured
  `bridge-transcript.jsonl` on the accepted serial-nullmodem path. The JSONL
  transcript included startup telemetry, a pre-action telemetry refresh,
  `msg_post`, `msg_pull`, `msg_search`, `msg_ack`, `download_queue`,
  `otap_intent`, and a final telemetry refresh; DOS-C vision gate and ESP32
  completion gate both reported `status: pass`. Source ID:
  `SRC-LOCAL-ESPNOW-GATE-H-STRUCTURED-LIVE-ACCEPTANCE-2026-05-25`.
- `ADR-0005` opens Gate G only as a local-admin redacted JSON export from the
  DOS-C/Pi bridge spool. Win31 export controls, firmware export ABI, and live
  bridge export request types remain closed. Source ID:
  `SRC-LOCAL-ESPNOW-GATE-G-LIVE-EXPORT-IMPLEMENTATION-2026-05-25`.
- `ADR-0006` is accepted for Gate F firmware ABI design contract only. It
  mirrors the simulator packet budget, service codes, custody codes, frame
  budgets, host-only packet golden vectors, and non-executing `control_intent`
  semantics, but does not authorize firmware runtime behavior or live proof
  work. Source IDs:
  `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-FIRMWARE-ABI-2026-05-26`,
  `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-OWNER-REVIEW-2026-05-26`,
  `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-GATE-F-GOLDEN-VECTORS-2026-05-26`.
- `ADR-0008` accepts Phase 5/6 runtime design review as host-only simulator
  work: balanced queue defaults, atomic backpressure, deterministic scheduler
  order, retry/expiry handling, duplicate handling, visible counters, and
  non-executing `control_intent`. It does not authorize coordinator/peer
  firmware runtime migration, persistence, flash, serial writes, or live proof.
  Source ID:
  `SRC-LOCAL-ESPNOW-CUSTOM-WIRELESS-PROTOCOL-PHASE-5-6-RUNTIME-DESIGN-REVIEW-2026-05-26`.
- A 2026-05-25 America/Denver LAN DHCP/current-remap pass, with UTC evidence
  filenames on 2026-05-26, validated a read-only current mapping:
  `peer01=COM6`, `peer02=COM10`, `peer03=COM12`, and coordinator `/dev/ttyUSB0`
  MAC `78:e3:6d:10:4d:6c`. It did not run bridge, Win31/OPCON, BBS, prepare,
  flash, erase, monitor, or radio proof. Source ID:
  `SRC-LOCAL-ESPNOW-LAN-DHCP-CURRENT-REMAP-2026-05-25`.
- ADR-0004 proposes ESP-WIFI-MESH as a future self-healing branch and BLE GATT
  as the Android client-node interface model, but no firmware migration is
  accepted. The paired DOS-C bridge/operator source now has simulator-only
  network metadata and a read-only Win31 Network view.
- A host-only LCD/encoder field-console plan now defines `bbs_lcd_state.v1`
  as a local 20x4 renderer snapshot for custody, peers, queue, files, mesh,
  XBee closed-surface status, locks, and diagnostics. It does not change the
  bridge ABI, firmware ABI, coordinator serial ABI, Win31 transport, or live
  hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-BBS-LCD-ENCODER-FIELD-CONSOLE-2026-05-30`.
- The LCD menu graphics/browser continuation now defines `bbs_lcd_render.v1`,
  software cursor metadata, named glyph banks, host-rendered widgets, an inert
  browser mirror, and a recallable LCD menu skill/agent. It remains host-only
  and does not open live browser proof, firmware HTTP/SoftAP, XBee/RF, relay,
  serial-write, flash, or ESP-NOW runtime authority. Source ID:
  `SRC-LOCAL-ESPNOW-BBS-LCD-MENU-GRAPHICS-BROWSER-AGENT-2026-05-31`.
- Full-service mesh discovery Gate M4B now records an inert build-only
  ESP-IDF target under `firmware/projects/espnow-bbs/` and a no-flash local
  ESP-IDF v6.0.1 build proof. It does not accept firmware runtime behavior,
  persistence, live ESP-WIFI-MESH, BLE, Android, router/admin, flash, serial
  writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`.
- Full-service mesh discovery Gate M4C now records an inert pure-C firmware
  contract component under
  `firmware/projects/espnow-bbs/components/mesh_discovery_contract/`, host C
  tests, recursive firmware-audit coverage, and a no-flash ESP-IDF build proof.
  It mirrors `mesh_discovery.v1` constants and invariants but does not accept
  firmware runtime behavior, bridge dispatch, persistence, live ESP-WIFI-MESH,
  BLE, Android, router/admin, flash, serial writes, or hardware authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`.
- Full-service mesh discovery Gate M4D now records a caller-owned fixed-size
  discovery-event queue contract inside the same inert pure-C component, with
  host C tests, recursive firmware-audit coverage, and a no-flash ESP-IDF build
  proof. It validates bounded ASCII event ingress, FIFO push/peek/pop, explicit
  full/empty backpressure, dropped counters, and compact local queue summaries,
  but does not accept firmware runtime behavior, bridge dispatch, persistence,
  live ESP-WIFI-MESH, BLE, Android, router/admin, flash, serial writes, or
  hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`.
- Full-service mesh discovery Gate M4E now records a caller-owned runtime/task
  ownership contract over the M4D queue inside the same inert pure-C component,
  with host C tests, recursive firmware-audit coverage, and a no-flash ESP-IDF
  build proof. It validates known owner names, caller-held claim/release
  tokens, stale-token rejection, queue-bound ingress/drain helpers, owner
  counters, and compact local owner summaries, but does not accept actual
  firmware task creation, scheduler integration, bridge dispatch, persistence,
  live ESP-WIFI-MESH, BLE, Android, router/admin, flash, serial writes, or
  hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`.
- Full-service mesh discovery Gate M4F now records a caller-owned dispatch-plan
  contract over the M4E owner state and M4D queue inside the same inert pure-C
  component, with completed host/build/audit validation and final 8/8 weighted
  reviewer approval. It validates deterministic drain/ingest/snapshot action
  order, requested/planned/rejected accounting, per-step local status, summary
  bounds, and no owner/queue mutation while planning, but does not accept
  scheduler integration, actual firmware task creation, bridge dispatch,
  persistence, live ESP-WIFI-MESH, BLE, Android, router/admin, flash, serial
  writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`.
- Full-service mesh discovery Gate M4G now records a caller-owned scheduler
  integration contract over the M4F dispatch plan inside the same inert pure-C
  component, with completed host/build/audit validation and final 8/8 weighted
  reviewer approval. It validates known schedule profiles, bounded tick
  accounting, deterministic profile-to-plan staging, summary bounds, and no
  owner/queue mutation while staging, but does not accept actual firmware task
  creation, runtime scheduler loops, bridge dispatch, persistence, live
  ESP-WIFI-MESH, BLE, Android, router/admin, flash, serial writes, or hardware
  authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`.
- Full-service mesh discovery Gate M4H now records a caller-owned execution
  accounting contract over an already staged M4F dispatch plan inside the same
  inert pure-C component, with completed host/build/audit validation and
  reviewer P2 stale-record remediation. It validates bounded execution cycles,
  same-owner ready-step accounting, non-ready planned-step rejection,
  mixed-owner rejection, apply/skip totals, ready-action counters, summary
  bounds, and no owner/queue/scheduler/plan mutation while accounting, but does
  not accept actual execution runtime, actual firmware task creation, bridge
  dispatch, persistence, live ESP-WIFI-MESH, BLE, Android, router/admin, flash,
  serial writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`.
- Full-service mesh discovery Gate M4I now records a caller-owned
  runtime-cycle contract over M4G scheduler staging and M4H execution
  accounting inside the same inert pure-C component, with completed
  host/build/audit validation and final 8/8 weighted reviewer approval after
  stale-record remediation. It validates bounded runtime cycles,
  temporary-copy staging/accounting, no partial mutation on rejected
  cycle/profile/owner inputs, scheduled/applied/skipped/rejected counters,
  summary bounds, and no owner/queue mutation while composing, but does not
  accept actual scheduler runtime, actual firmware task creation, bridge
  dispatch, persistence, live ESP-WIFI-MESH, BLE, Android, router/admin, flash,
  serial writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`.
- Full-service mesh discovery Gate M4J records a design-only runtime scheduler
  review for the next source gate, with source-indexed ESP-IDF FreeRTOS and ESP
  Timer requirements for task ownership, core affinity, priority, stack budget,
  timer choice, callback dispatch model, bounded backpressure, bridge-visible
  counters, volatile-only state, cleanup/deinit behavior, and validation
  commands. It does not accept runtime scheduler source implementation,
  ESP-IDF task/timer code, bridge dispatch, persistence, live ESP-WIFI-MESH,
  BLE, Android, router/admin, flash, serial writes, or hardware authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`.
- Full-service mesh discovery Gate M4K now records an accepted caller-owned
  runtime scheduler source contract over M4I runtime-cycle composition. It
  accepts only `caller_invoked`, `task_none`, and `timer_none` policies,
  rejects selected core affinity, nonzero priority, nonzero stack bytes, static
  state, invalid period/yield values, and unknown policies, and commits
  scheduler state only after an existing runtime-cycle step succeeds. It does
  not accept actual task creation, timer creation, firmware runtime, bridge
  dispatch, persistence, live ESP-WIFI-MESH, BLE, Android, router/admin, flash,
  serial writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4L is accepted for a separate
  caller-started ESP-IDF-only static task shell source API under
  `firmware/projects/espnow-bbs/components/mesh_runtime_task_shell/`. It requires
  caller-provided `StaticTask_t` and `StackType_t` buffers, uses
  `xTaskCreateStaticPinnedToCore`, invokes the accepted M4K scheduler step as an
  external caller, and does not start from `app_main`. It does not accept
  automatic startup, live task execution proof, timer creation, queues,
  callbacks, bridge dispatch, persistence, live ESP-WIFI-MESH, BLE, Android,
  router/admin, flash, serial writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4M is accepted only for design-only
  startup/lifecycle requirements after M4L. It names required lifecycle states,
  transition handling, caller-owned versus static state decisions, task-buffer
  lifetime, failure cleanup, and validation expectations for a future source
  gate. It does not accept firmware source implementation, `app_main` startup,
  bridge dispatch, persistence, live ESP-WIFI-MESH, BLE, Android, router/admin,
  flash, serial writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`.
- Full-service mesh discovery Gate M4N is accepted for a caller-owned startup
  lifecycle source API around M4L after validation and final 8/8 weighted
  post-review passed.
  It defines
  `uninitialized`, `configured`, `starting`, `running`, `stop_requested`,
  `stopped`, and `failed` states, embeds the M4L task object in the lifecycle
  object, requires reconfigure after stopped, and clears copied task config plus
  the embedded M4L task object on failure/deinit paths. It does not accept
  `app_main` startup, automatic startup acceptance, bridge dispatch,
  persistence, live ESP-WIFI-MESH, BLE, Android, router/admin, flash, serial
  writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4O is accepted for a no-flash `app_main`
  startup integration source contract. It adds static app-main-owned event
  queue, owner, runtime scheduler, lifecycle, task buffer, and stack storage,
  then configures M4N and calls the lifecycle start path. Focused scaffold tests,
  host contract tests, scaffold audits, no-flash ESP-IDF v6.0.1 build,
  generated-output cleanup, closed-surface scan, `git diff --check`, and final
  8/8 weighted post-review passed. It does not accept firmware runtime behavior,
  live task proof, bridge dispatch, persistence, live ESP-WIFI-MESH, BLE,
  Android, router/admin, flash, serial writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`.
- Full-service mesh discovery Gate M4P is accepted for a pure-C local
  bridge-summary source contract over the existing caller-owned discovery
  state. It adds `espnow_bbs_mesh_contract_bridge_summary_state_t` and
  `espnow_bbs_mesh_contract_bridge_summary` for the accepted
  `discovery_snapshot`, `discovery_events`, `service_catalog`, and
  `capability_report` request names. Focused scaffold tests, host contract
  tests, scaffold audits, no-flash ESP-IDF v6.0.1 build, generated-output
  cleanup, closed-surface scan, `git diff --check`, and final 8/8 weighted
  post-review passed after the record-alignment P2 was resolved. It does not
  accept live bridge dispatch, serial ABI expansion, Gate F request/service
  changes, `mesh_discovery.v1` schema changes, persistence, timer creation,
  firmware runtime acceptance, live proof, flash, serial writes, or hardware
  authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4Q is accepted for a pure-C local bridge
  request router/accounting contract over the accepted M4P bridge-summary
  selector. It adds caller-owned router state, init/reset, route, and summary
  APIs; exact-matches only the existing discovery request names; routes
  through `espnow_bbs_mesh_contract_bridge_summary`; and records
  attempted/accepted/rejected/last-status accounting without mutating queue,
  owner, or runtime scheduler state. Focused scaffold tests, host contract
  tests, firmware/path/records/sources/docs/agent-process/pages/React
  Native/skills audits, no-flash ESP-IDF build, cleanup checks,
  closed-surface scan, `git diff --check`, and final 8/8 weighted post-review
  passed after the record-alignment P2 was resolved. The aggregate
  `scripts/verify_scaffold.py` command entered kernel I/O wait twice and was
  stopped; the covered audit groups were run individually and passed. It does
  not accept live bridge dispatch, serial ABI expansion, Gate F
  request/service changes, `mesh_discovery.v1` schema changes, persistence,
  timer creation, firmware runtime acceptance, live proof, flash, serial
  writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4R is accepted source-only for a pure-C
  local dispatch-envelope/accounting contract over the accepted M4Q request
  router. It adds caller-owned envelope state, init/reset, route, and summary
  APIs; calls `espnow_bbs_mesh_contract_bridge_request_route`; records
  attempted/would-dispatch/rejected/last-status accounting without mutating
  queue, owner, or runtime scheduler state; and keeps summaries marked
  `live=blocked` and `serial=closed`. Host contract tests, focused scaffold
  tests, firmware/path/record/source/docs/agent-process/pages/React
  Native/skills audits, no-flash ESP-IDF build, cleanup checks,
  closed-surface scan, `git diff --check`, and final post-review passed after
  the record-alignment P2 was resolved. It does not accept live bridge
  dispatch, bridge frame parsing, serial ABI expansion, bridge ABI changes,
  Gate F request/service changes, `mesh_discovery.v1` schema changes,
  persistence, timer creation, firmware runtime acceptance, live proof, flash,
  serial writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4S is accepted source-only for a pure-C
  caller-owned request-text intent/accounting contract over the accepted M4R
  dispatch envelope. It adds caller-owned intent state, init/reset, route, and
  summary APIs; trims only leading/trailing ASCII space, tab, CR, and LF;
  rejects blank, embedded-whitespace, structured, Gate F, live-action, unknown,
  non-ASCII, overlong, and null request text before envelope/router
  accounting; and delegates accepted normalized names to M4R. Host contract
  tests, scaffold/audit validation, no-flash build, cleanup, closed-surface
  scan, `git diff --check`, and final post-review passed after the
  record-alignment P2 was resolved. It does not accept arbitrary bridge frame parsing, live bridge
  dispatch, serial ABI expansion, bridge ABI changes, Gate F request/service
  changes, `mesh_discovery.v1` schema changes, persistence, timer creation,
  firmware runtime acceptance, live proof, flash, serial writes, or hardware
  authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4T is accepted source-only for a
  pure-C caller-owned volatile persistence-intent/accounting contract over
  accepted M4S request-text routing. It adds
  caller-owned persistence-intent state, init/reset, stage, and summary APIs;
  records only counters, last normalized request/status, and staged byte
  totals; does not retain generated bridge summary payloads; and marks
  summaries with `storage=volatile`, `nvs=closed`, `file=closed`,
  `live=blocked`, `serial=closed`, and `abi=closed`. Host contract tests,
  firmware/path/record/source/docs audits, focused and broader selected
  scaffold unittests, no-flash ESP-IDF v6.0.1 build, cleanup checks,
  closed-surface scan, and `git diff --check` passed. It does not accept
  retained persistence, NVS/file storage, reboot durability, runtime
  persistence, live bridge dispatch, serial ABI expansion, bridge ABI changes,
  Gate F request/service changes, `mesh_discovery.v1` schema changes, timer
  creation, firmware runtime acceptance, live proof, flash, serial writes, or
  hardware authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4T-PERSISTENCE-INTENT-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4U records design-only retained-persistence
  requirements after accepted M4T. It requires a future source gate to name
  storage owner, namespace/key policy, initialization, deinitialization,
  recovery, write policy, bounded/versioned M4T-derived checkpoint fields,
  corrupt/missing/unsupported-record handling, secret exclusion, no generated
  bridge summary retention, no runtime scheduler stepping, no queue/owner
  mutation during recovery, and closed-surface scans before any storage API is
  called. It does not accept retained persistence implementation, NVS/file
  storage, reboot durability, runtime persistence, live bridge dispatch, serial
  ABI expansion, bridge ABI changes, Gate F request/service changes,
  `mesh_discovery.v1` schema changes, timer creation, firmware runtime
  acceptance, live proof, flash, serial writes, or hardware authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4U-RETAINED-PERSISTENCE-REQUIREMENTS-2026-06-06`.
- Full-service mesh discovery Gate M4V implements a source-only retained
  persistence component after accepted M4U. It adds
  `mesh_retained_persistence`, a host-testable fixed 96-byte checkpoint codec,
  and NVS adapter source with explicit init/deinit/load/save/erase APIs. Host
  retained-persistence tests, focused scaffold tests, firmware audit, no-flash
  ESP-IDF v6.0.1 build, P2 remediation validation, generated-output cleanup,
  and final 8/8 weighted post-review passed. It does not accept live NVS execution proof,
  reboot durability, runtime persistence proof, live bridge dispatch, first
  flash, live proof, flash, serial writes, or hardware authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`.
- Full-service mesh discovery Gate M4W accepts source-only app-main retained
  recovery integration after accepted M4V. It adds `mesh_retained_persistence`
  as a private main dependency, initializes retained intent to empty volatile
  state, calls storage context init, storage init, and storage load from
  startup, records retained init/load statuses, and passed host/focused
  audit/no-flash-build validation plus final 8/8 weighted post-review. It does
  not wire save, erase, direct NVS writes, live NVS execution proof, reboot
  durability, runtime persistence proof, first flash, live proof, flash, serial
  writes, or hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4W-RETAINED-RECOVERY-APP-MAIN-SOURCE-2026-06-06`.
- Full-service mesh discovery Gate M4X accepts source-only app-main
  bridge-readiness status capture after accepted M4W. It adds static
  app-main-owned bridge summary state,
  request router, dispatch envelope, request-text intent, summary buffer,
  status field, and line-byte field; routes only fixed `discovery_snapshot`
  through the accepted M4S request-text intent path; records status and summary
  bytes; and preserves M4O/M4W lifecycle startup ordering without blocking
  startup on readiness failure. Full validation, generated-output cleanup,
  post-review, and P2 record-alignment remediation passed. It does not accept live bridge dispatch,
  arbitrary bridge frame parsing, serial ABI expansion, bridge ABI changes,
  live NVS execution proof, reboot durability, runtime persistence proof,
  first flash, firmware runtime proof, live proof, flash, serial writes, or
  hardware authority. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4X-BRIDGE-READINESS-APP-MAIN-SOURCE-2026-06-06`.

## Firmware Roles

- Coordinator: USB serial command endpoint, ESP-NOW peer table, read-only
  `hello`/`state`/`diag`/`peer_list`, channel/key
  policy, ACK/retry handling, duplicate suppression, TTL enforcement, and
  compact telemetry back to the Pi bridge.
- Client: provisioned identity, bounded receive/send queues, app-level ACKs,
  diagnostics, firmware version reporting, and store-and-forward message
  handling.

## Safety Boundaries

- This lane does not control relays, XBee modules, mains/load wiring, or SD
  imaging.
- The peer flash was limited to the Windows COM6 ESP32 after read-only identity
  and private backup evidence passed.
- The coordinator encrypted flash was limited to the accepted Pi USB serial
  device after fresh identity, USB-only/no-load/no external wiring evidence,
  stale listener absence, and private backup passed.
- Relay, XBee, TFT, MicroSD, load, mains, PCAP, Windows COM proxy, erase, and
  dashboard state-changing commands remain closed.
- The 2026-05-23 USB-only three-peer flashing gate is closed by corrected
  evidence for the current coordinator and peers only; future reruns still
  require fresh same-session identity, backups, manifest review, explicit
  write confirmation, and cleanup proof.

## Implementation Order

1. Preserve the accepted serial-nullmodem dashboard path and stop extending the
   failed SLIRP packet-driver callback lane for OPCON.
2. Keep ESP-IDF v6.0.1 toolchain evidence current; Gate M4B records the first
  build-only `espnow-bbs` target proof, while Gate M4C/M4D/M4E/M4F/M4G/M4H/M4I
  and M4K record inert firmware-side contract source slices only.
3. Preserve the accepted coordinator USB serial `hello`/`state`/`diag` proof.
4. Add chunked message delivery and custody telemetry only through simulator
   and owner-reviewed ABI gates first.
5. Add provisioning and firmware inventory flows only after source-backed
   security and operator policy review.
6. Preserve the accepted three-peer live gate evidence and require a fresh
   same-session gate for any rerun or new hardware mix.
7. Keep ESP-WIFI-MESH/BLE GATT work design-only until ADR-0004 is accepted and
   fresh mesh, BLE, coexistence, backup, recovery, and cleanup evidence exists.
8. Keep firmware ABI runtime work, persistence, Win31 export controls, bridge
   export request types, chunked live delivery, provisioning UX,
   mobile/BLE client-node work, PCAP, and physical wiring beyond USB-only as
   separate future evidence records.
