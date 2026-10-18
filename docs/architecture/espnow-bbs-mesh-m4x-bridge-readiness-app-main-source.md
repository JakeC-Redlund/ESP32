# ESP-NOW BBS Mesh M4X Bridge Readiness App Main Source

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4X is a Tier 2 firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It integrates the accepted M4P through M4S
local bridge request stack into `app_main` as source-only bridge-readiness
status capture.

This packet is accepted source-only after validation, post-review, and P2
record-alignment remediation. It does not
accept live bridge dispatch, arbitrary bridge frame parsing, serial ABI
expansion, bridge ABI changes, Gate F request or service changes,
`mesh_discovery.v1` schema changes, live NVS execution proof, reboot
durability, runtime persistence proof, first-flash proof, firmware runtime
acceptance, timer creation, live task execution proof, flash, erase, monitor,
serial writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains,
release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4W accepts source-only retained recovery in `app_main` and explicitly does
  not accept live bridge dispatch.
- M4P accepts local bridge-summary selection for known discovery request names.
- M4Q accepts a local request router over M4P.
- M4R accepts a local dispatch envelope that records would-dispatch accounting
  while keeping `live=blocked` and `serial=closed`.
- M4S accepts request-text intent routing over M4R/M4Q and keeps malformed,
  structured, Gate F, live-action, unknown, non-ASCII, overlong, and null text
  rejected before external side effects.
- `app_main` now calls the M4S route only with fixed request text
  `discovery_snapshot` and a static app-owned summary buffer.

## Assumptions

- Fixed local readiness capture is useful before opening a future live bridge
  dispatch gate because it proves app-owned state can call the accepted local
  bridge request path.
- Readiness failure must not block lifecycle startup in this source-only slice;
  it is recorded as status and line bytes only.
- Static file-scope bridge state is the narrowest app-main boundary because it
  avoids dynamic allocation and preserves caller-owned contract state.

## Unknowns

- Live bridge input source, serial transcript behavior, bridge ABI, serial ABI,
  runtime scheduling behavior, live NVS behavior, reboot durability,
  first-flash behavior, M5 mesh behavior, hardware behavior, and release
  readiness remain unproved.

## Source Boundary

- Request text: fixed literal `discovery_snapshot` only.
- Bridge state: static app-main-owned bridge summary state, request router,
  dispatch envelope, request-text intent, summary buffer, readiness status, and
  summary line-byte field.
- Summary buffer: static
  `ESPNOW_BBS_MESH_DISCOVERY_BRIDGE_MAX_LINE_BYTES + 1u` bytes.
- Startup ordering: configure existing M4O contract state, perform M4W
  retained load-only recovery, capture local bridge readiness, then continue
  existing lifecycle configure/start.
- Failure behavior: readiness status and line bytes are recorded. Lifecycle
  startup is not blocked by readiness failure.
- Closed surfaces: no app-main live dispatch, external request ingestion,
  arbitrary frame parser, serial output, bridge ABI change, Gate F service
  change, direct NVS write, retained save/erase, Wi-Fi, ESP-NOW,
  ESP-WIFI-MESH, BLE, timers, HTTPD, file I/O, dynamic allocation, or logging.

## Validation Status

Validation passed:

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration`
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
- Firmware, data, records, sources, docs, paths, agent-process, pages, React
  Native, and skills scaffold audits.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/verify_scaffold.py`
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4x-bridge-readiness-app-main-source build`

The no-flash ESP-IDF build generated
`/tmp/esp32-espnow-bbs-m4x-bridge-readiness-app-main-source/espnow_bbs_build_only.bin`
at size `0x29940` with `0xd66c0` bytes free in the 1 MiB app partition.
Generated `sdkconfig` residue was removed. Closed-surface scans and
`git diff --check` passed. Post-review found no source-boundary P1/P2 blockers
and identified only stale validation/post-review wording, which was remediated
in the M4X records.

## Decision

Decision accepted after P2 record-alignment remediation: source-only app-main
bridge readiness status capture has source/build proof. Live bridge dispatch,
runtime behavior, NVS proof, first flash, M5 mesh, hardware, and release remain
closed.
