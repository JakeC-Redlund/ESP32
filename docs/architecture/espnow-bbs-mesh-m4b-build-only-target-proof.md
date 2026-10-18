# ESP-NOW BBS Mesh M4B Build-Only Target Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4B is a Tier 2 build-only ESP-IDF target proof for the `espnow-bbs`
firmware lane accepted by `ADR-0003`.

This proof adds a minimal inert ESP-IDF project under
`firmware/projects/espnow-bbs/` and proves it can build with the local
ESP-IDF v6.0.1 toolchain. It does not implement firmware runtime behavior,
firmware persistence, ESP-NOW packet handling, ESP-WIFI-MESH runtime, BLE,
Android behavior, router/admin behavior, bridge requests, serial commands, or
live hardware behavior.

## Verified Facts

- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for the `espnow-bbs`
  coordinator/client firmware lane. Source IDs: `SRC-ESP-IDF-STABLE-ESP32`,
  `SRC-ESP-IDF-GET-STARTED`.
- `SRC-ESP-IDF-START-PROJECT` covers no-flash `idf.py build` validation.
- `SRC-ESP-IDF-BUILD-SYSTEM-FLASH-ARGS` records that ESP-IDF build output can
  include generated flash argument files; these are not source artifacts for
  this gate.
- Gate M4 remains accepted only as host adapter fixture proof. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`.
- The M4B target contains only:
  `firmware/projects/espnow-bbs/CMakeLists.txt`,
  `firmware/projects/espnow-bbs/main/CMakeLists.txt`, and an inert
  `firmware/projects/espnow-bbs/main/main.c`.

## Assumptions

- A no-op `app_main` is the correct first firmware target because it proves
  project shape and build environment before runtime APIs are introduced.
- Future runtime slices should remain separately gated and source-backed
  against the accepted Gate F and `mesh_discovery.v1` contracts.

## Unknowns

- No firmware runtime task, memory budget, queue ownership, persistence,
  recovery behavior, first-flash target, live route table, parent/root
  selection, healing, coexistence, BLE/GATT, Android app, router/admin policy,
  PCAP, transcript, rollback, cleanup, or live proof is accepted by this
  gate.

## Accepted Build Proof

- The local no-flash build command passed:
  `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-build-only build`.
- ESP-IDF reported IDF version `6.0`, default target `esp32`, project
  `espnow_bbs_build_only`, and generated
  `/tmp/esp32-espnow-bbs-build-only/espnow_bbs_build_only.bin`.
- The generated app binary size was `0x21aa0`; ESP-IDF reported `0xde560`
  bytes free in the smallest app partition.
- Build outputs remained generated artifacts outside source control; the
  generated project `sdkconfig` was removed after the build.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, and bridge request names
  unchanged.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep the firmware source inert until a separate runtime implementation gate
  names exact APIs, state ownership, validation, and stop gates.

## Decision

Accept the M4B build-only target proof as a bounded Tier 2 step. This proves
the ESP-NOW BBS firmware lane has an ESP-IDF project target that compiles
locally. It does not accept firmware runtime behavior, persistence, live mesh,
BLE, Android, router/admin, flash, serial, RF/XBee, relay/load/mains,
release, publication, or deployment authority.
