# ESP-NOW BBS Firmware Build-Only Target

Source index: [../../../knowledge-base/source-index.md](../../../knowledge-base/source-index.md)

## Purpose

This directory contains the first build-only ESP-IDF target for the
`espnow-bbs` coordinator/client firmware lane accepted by `ADR-0003`.

The target supports no-flash source and build verification under the accepted
ESP-IDF v6.0.1 toolchain. The current M4O–M4X source includes `app_main`
startup through the lifecycle/task shell, retained-intent recovery through
the NVS adapter, and local bridge-readiness status capture. Those source
contracts do not establish live coordinator/client operation, mesh discovery,
reboot durability, provisioning, radio service codes, or live bridge dispatch.
Firmware execution and physical hardware acceptance remain separate Tier 3
gates. The gate descriptions below record the narrower scope at each stage.

Gate M4C adds a pure-C `mesh_discovery_contract` component for compile-time
firmware-side constants and validators. The component is linked only for
build/link proof and remains inert.

Gate M4D extends the same pure-C component with a caller-owned fixed-size
discovery-event queue contract. The queue state lives in caller-provided
structs, uses bounded ASCII records, and remains inert.

Gate M4E extends the same pure-C component with a caller-owned runtime/task
ownership contract over the M4D queue. Owner state lives in caller-provided
structs, uses known owner names and claim/release tokens, and remains inert.

Gate M4F extends the same pure-C component with a caller-owned dispatch-plan
contract over the M4E owner state and M4D queue. Dispatch plan state lives in
caller-provided structs, uses deterministic drain/ingest/snapshot action
ordering and local budget accounting, and remains inert.

Gate M4G extends the same pure-C component with a caller-owned scheduler
integration contract over the M4F dispatch plan. Scheduler state lives in
caller-provided structs, uses known local schedule profiles and bounded tick
accounting, and remains inert.

Gate M4H extends the same pure-C component with a caller-owned execution
accounting contract over an already staged M4F dispatch plan. Execution
accounting state lives in caller-provided structs, uses bounded cycle
accounting and ready-action counters, and remains inert.

Gate M4I extends the same pure-C component with a caller-owned runtime-cycle
contract over M4G scheduler staging and M4H execution accounting. Runtime-cycle
state lives in caller-provided structs, uses temporary copies before commit,
and remains inert.

Gate M4J is design-only. It records requirements for a later runtime scheduler
source gate and does not add firmware source, task/timer code, callbacks, static
runtime state, or build output.

Gate M4K adds a caller-owned runtime scheduler source contract over the M4I
runtime-cycle contract. The contract accepts only caller-invoked, no-task, and
no-timer policies, rejects selected core affinity, nonzero priority, nonzero
stack bytes, static state, invalid period/yield values, and unknown policies,
and remains inert.

Gate M4L adds a separate ESP-IDF-only `mesh_runtime_task_shell` component. It
offers a caller-started static FreeRTOS task shell using caller-provided
`StaticTask_t` and `StackType_t` buffers and invokes the M4K scheduler step as
an external caller. M4L did not add `app_main` startup; the later M4O source
integration starts it through the accepted M4N lifecycle API.

## Verified facts

- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for this `espnow-bbs`
  firmware lane. Source IDs: `SRC-ESP-IDF-STABLE-ESP32`,
  `SRC-ESP-IDF-GET-STARTED`.
- Gate M4 accepts only host adapter fixture proof for
  `mesh_discovery.v1`; it does not accept ESP-IDF target creation, firmware
  runtime, or live proof. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`.
- Gate M4B accepts only the inert target/build proof. Gate M4C accepts only
  the pure-C firmware contract component and host/build proof. Gate M4D accepts
  only the caller-owned event queue contract and host/build proof. Gate M4E
  accepts only the caller-owned ownership contract and host/build proof. Gate
  M4F accepts only the caller-owned dispatch-plan contract and host/build
  proof. Gate M4G accepts only the caller-owned scheduler integration contract
  and host/build proof. Gate M4H accepts only the caller-owned execution
  accounting contract and host/build proof. Gate M4I accepts only the
  caller-owned runtime-cycle composition contract and host/build proof. Gate
  M4J accepts only design requirements for a later runtime scheduler source
  gate. Gate M4K accepts only the caller-owned runtime scheduler source
  contract and host/build proof. Gate M4L accepts only a separate caller-started
  static task shell source API and host/build proof. Gate M4M accepts only
  design-only startup/lifecycle requirements. Gate M4N accepts only a
  caller-owned startup lifecycle source API around M4L after validation and
  final 8/8 weighted post-review passed; it has no
  `app_main` startup, bridge dispatch, firmware runtime acceptance, live task
  proof, flash, or serial authority. Gate M4O is accepted for a no-flash
  `app_main` startup integration source contract over M4N after focused scaffold
  tests, host contract tests, scaffold audits, no-flash ESP-IDF v6.0.1 build,
  generated-output cleanup, closed-surface scan, `git diff --check`, and final
  8/8 weighted post-review passed; it has no firmware runtime acceptance, live
  task proof, bridge dispatch, persistence, flash, or serial authority. Gate
  M4P is accepted for a pure-C local bridge-summary source contract over the
  existing caller-owned discovery state after focused scaffold tests, host
  contract tests, scaffold audits, no-flash ESP-IDF v6.0.1 build,
  generated-output cleanup, closed-surface scan, `git diff --check`, and final
  8/8 weighted post-review passed after the record-alignment P2 was resolved;
  it has no live bridge dispatch, serial ABI expansion, Gate F request/service
  change, schema change, persistence, timer, firmware runtime, flash, or serial
  authority.
  Source IDs:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4C-FIRMWARE-CONTRACT-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4D-EVENT-QUEUE-CONTRACT-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4E-RUNTIME-TASK-OWNERSHIP-CONTRACT-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4F-DISPATCH-PLAN-CONTRACT-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4G-SCHEDULER-INTEGRATION-CONTRACT-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4H-EXECUTION-ACCOUNTING-CONTRACT-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4I-RUNTIME-CYCLE-CONTRACT-2026-06-05`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4J-RUNTIME-SCHEDULER-DESIGN-2026-06-06`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4K-RUNTIME-SCHEDULER-SOURCE-CONTRACT-2026-06-06`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4L-STATIC-TASK-SHELL-SOURCE-CONTRACT-2026-06-06`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4M-STARTUP-LIFECYCLE-REQUIREMENTS-2026-06-06`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4N-STARTUP-LIFECYCLE-SOURCE-CONTRACT-2026-06-06`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4O-APP-MAIN-STARTUP-INTEGRATION-2026-06-06`,
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4P-BRIDGE-SUMMARY-SOURCE-CONTRACT-2026-06-06`.
- Gate M4Q is accepted for a pure-C local bridge request router/accounting
  contract over the accepted M4P bridge-summary selector. It adds caller-owned
  router state, init/reset, route, and summary APIs; exact-matches only the
  existing discovery request names; routes through
  `espnow_bbs_mesh_contract_bridge_summary`; and records
  attempted/accepted/rejected/last-status accounting without mutating queue,
  owner, or runtime scheduler state. Focused scaffold tests, host contract
  tests, firmware/path/records/sources/docs/agent-process/pages/React
  Native/skills audits, no-flash ESP-IDF build, cleanup checks,
  closed-surface scan, `git diff --check`, and final 8/8 weighted post-review
  passed after the record-alignment P2 was resolved. The aggregate
  `scripts/verify_scaffold.py` command entered kernel I/O wait twice and was
  stopped; the covered audit groups were run individually and passed. It has
  no live bridge dispatch, serial ABI expansion, Gate F request/service
  change, schema change, persistence, timer, firmware runtime, flash, or
  serial authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4Q-LOCAL-BRIDGE-REQUEST-ROUTER-SOURCE-CONTRACT-2026-06-06`.
- Gate M4R is accepted source-only for a pure-C local dispatch
  envelope/accounting contract over the accepted M4Q request router. It adds
  caller-owned envelope state, init/reset, route, and summary APIs; routes
  through `espnow_bbs_mesh_contract_bridge_request_route`; records
  attempted/would-dispatch/rejected/last-status accounting; and keeps summary
  output marked `live=blocked` and `serial=closed`. Host contract tests,
  focused scaffold tests, firmware/path/record/source/docs/agent-process/pages/
  React Native/skills audits, no-flash ESP-IDF build, cleanup checks,
  closed-surface scan, `git diff --check`, and final post-review passed after
  the record-alignment P2 was resolved. It has no live bridge dispatch, bridge
  frame parser, serial ABI expansion, bridge ABI change, Gate F request/service
  change, schema change, persistence, timer, firmware runtime, flash, or serial
  authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4R-BRIDGE-DISPATCH-ENVELOPE-SOURCE-CONTRACT-2026-06-06`.
- Gate M4S is accepted source-only for a pure-C caller-owned request-text
  intent/accounting contract over the accepted M4R dispatch envelope. It adds
  caller-owned intent state, init/reset, route, and summary APIs; trims only
  leading/trailing ASCII space, tab, CR, and LF; rejects blank,
  embedded-whitespace, structured, Gate F, live-action, unknown, non-ASCII,
  overlong, and null request text before envelope/router accounting; delegates
  accepted normalized names to M4R; and marks summaries with `live=blocked`,
  `serial=closed`, and `abi=closed`. Host contract tests, scaffold/audit
  validation, no-flash ESP-IDF build, cleanup checks, closed-surface scan,
  `git diff --check`, and final post-review passed after the record-alignment
  P2 was resolved.
  It has no arbitrary bridge frame parsing, live bridge dispatch, serial ABI
  expansion, bridge ABI change, Gate F request/service change, schema change,
  persistence, timer, firmware runtime, flash, or serial authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4S-BRIDGE-FRAME-INTENT-SOURCE-CONTRACT-2026-06-06`.
- Gate M4X is accepted source-only for app-main bridge-readiness status capture
  after accepted M4W. It
  adds static app-main-owned local bridge state, routes only fixed
  `discovery_snapshot` through the accepted M4S request-text intent route,
  records status and line bytes in `main.c`, and preserves lifecycle startup.
  Full validation, generated-output cleanup, post-review, and P2
  record-alignment remediation passed.
  It has no live bridge dispatch, arbitrary bridge frame parsing, serial ABI
  expansion, bridge ABI change, Gate F request/service change, schema change,
  live NVS proof, reboot durability, runtime persistence proof, timer,
  firmware runtime, flash, or serial authority.
  Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4X-BRIDGE-READINESS-APP-MAIN-SOURCE-2026-06-06`.
- ESP-IDF build artifacts and flash argument files are generated outputs of a
  build. Source ID: `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS`.

## Assumptions

- A minimal no-op app is the correct first target because it proves toolchain
  and project structure before runtime APIs are introduced.
- Any future coordinator/client behavior will be added through separate
  owner-reviewed gates tied to the accepted protocol and discovery records.
- Firmware-side contract constants can be validated before runtime task,
  queue, persistence, or live API ownership is selected.
- Caller-owned event queue behavior can be validated before firmware runtime
  task ownership, dispatch integration, persistence, or live API ownership is
  selected.
- Caller-owned ownership behavior can be validated before actual task
  creation, scheduler integration, dispatch integration, persistence, or live
  API ownership is selected.
- Caller-owned dispatch planning can be validated before actual scheduler
  integration, dispatch loops, bridge dispatch, persistence, or live API
  ownership is selected.
- Caller-owned scheduler profile/tick accounting can be validated before an
  actual runtime scheduler loop, dispatch loops, bridge dispatch, persistence,
  or live API ownership is selected.
- Caller-owned execution accounting can be validated before actual runtime
  execution, dispatch loops, bridge dispatch, persistence, or live API
  ownership is selected.
- Runtime scheduler task/timer ownership must be designed before any actual
  scheduler source, callbacks, task creation, bridge dispatch, persistence, or
  live API ownership is selected.
- Caller-invoked runtime scheduler source composition can be validated before
  actual task/timer ownership, bridge dispatch, persistence, or live API
  ownership is selected.
- A caller-started static task shell source API can be compiled before automatic
  startup, live task execution proof, timer ownership, bridge dispatch,
  persistence, or live API ownership is selected.
- A fixed app-main bridge-readiness status capture can be compiled before live
  bridge dispatch, arbitrary bridge frame parsing, serial ABI expansion, bridge
  ABI changes, runtime proof, or live API ownership is selected.

## Unknowns

- No ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE runtime, NVS persistence,
  serial command runtime, route table, parent/root, healing, coexistence,
  Android, router/admin, PCAP, cleanup, rollback, or live proof exists in this
  target.
- No first-flash target is accepted by this build-only packet.

## Hard gates

No flash, erase, monitor, serial write, live mesh, BLE, Android, router/admin,
PCAP, RF/XBee, relay, load/mains, release, publication, commit, push, PR, or
deploy authority is opened by this target. Automatic startup from `app_main`,
live task execution proof, timer creation, firmware runtime, and live scheduler
implementation are also closed. Gate M4P adds only a local bridge-summary
selector; Gate M4Q adds only an accepted local bridge request
router/accounting source contract; Gate M4R adds only an accepted source-only
local dispatch envelope/accounting contract; Gate M4S adds only an
accepted source-only caller-owned request-text intent/accounting contract;
Gate M4T adds only an accepted source-only caller-owned
volatile persistence-intent/accounting contract with `storage=volatile`,
`nvs=closed`, and `file=closed`. Gate M4U adds only design-only
retained-persistence requirements. Gate M4V adds only a separate source-only
`mesh_retained_persistence` component with a fixed checkpoint codec and NVS
adapter source after those requirements; it does not prove live NVS execution,
reboot durability, or runtime persistence.
Gate M4W accepts only source-only app-main retained recovery status capture
after M4V. It initializes retained intent to volatile empty, calls retained
storage context init, storage init, and storage load, records retained
init/load status, keeps storage save/erase unwired, and passed host/focused
audit/no-flash-build validation plus final 8/8 weighted post-review. It does
not prove live NVS execution, reboot durability, runtime persistence, first
flash, or firmware runtime.
Live bridge dispatch, arbitrary bridge frame parsing, bridge ABI changes,
serial ABI expansion, Gate F request/service changes, schema changes,
live NVS execution proof, runtime persistence, and timers remain closed.

Generated `build/`, `sdkconfig`, `sdkconfig.*`, binary, map, and flash-argument
outputs remain untracked unless a later gate explicitly accepts them.

## Validation

The intended validation is a no-flash ESP-IDF build from this directory plus
the workspace scaffold audits and the host C contract tests under
`tests/espnow_bbs_mesh_discovery_contract/`. Passing build proof does not
accept firmware runtime behavior or live hardware behavior.
