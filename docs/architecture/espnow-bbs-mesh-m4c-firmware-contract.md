# ESP-NOW BBS Mesh M4C Firmware Contract Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4C is a Tier 2 inert firmware contract source slice for the
`espnow-bbs` firmware lane accepted by `ADR-0003`.

This proof adds a pure-C `mesh_discovery_contract` component under
`firmware/projects/espnow-bbs/components/`, links it into the existing
build-only ESP-IDF target, and validates the component with host C tests and a
no-flash ESP-IDF build.

It does not implement firmware runtime behavior, bridge request dispatch,
serial command handling, persistence, Wi-Fi startup, mesh startup, BLE,
Android behavior, router/admin behavior, PCAP, flash, monitor, serial writes,
RF/XBee writes, relay/load/mains work, release, publication, commit, push,
PR, or deploy.

## Verified Facts

- `ADR-0003` accepts ESP-IDF stable v6.0.1 only for the `espnow-bbs`
  coordinator/client firmware lane. Source IDs: `SRC-ESP-IDF-STABLE-ESP32`,
  `SRC-ESP-IDF-GET-STARTED`.
- Gate M4 remains accepted only as host adapter fixture proof. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4-HOST-ADAPTER-2026-06-05`.
- Gate M4B records the inert target/build proof that M4C links into. Source
  ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4B-BUILD-ONLY-TARGET-2026-06-05`.
- M4C adds firmware-side constants and pure helpers for:
  `mesh_discovery.v1`, `esp-wifi-mesh`, `sim`, `disabled`, the 512-byte bridge
  summary budget, four discovery request names, twelve discovery event names,
  compact event labels, Gate F bridge request/service invariants, recursive
  field-path secret checks, and blocked live-action classification.
- The component host tests compile the component source with
  `-Wall -Wextra -Werror`.
- The no-flash ESP-IDF build passed with IDF version `6.0`, default target
  `esp32`, project `espnow_bbs_build_only`, and generated
  `/tmp/esp32-espnow-bbs-m4c-contract/espnow_bbs_build_only.bin`.

## Assumptions

- A pure-C contract component is the correct next firmware source slice because
  it gives the build target a validated interface surface without selecting
  runtime task ownership, queues, persistence, or live APIs.
- Calling `espnow_bbs_mesh_contract_event_count()` from the inert `app_main`
  is sufficient to prove component linkage without starting runtime work.

## Unknowns

- No firmware runtime task graph, memory budget, queue ownership, persistence,
  recovery behavior, first-flash target, live route table, root/parent
  selection, healing, coexistence, BLE/GATT, Android app, router/admin policy,
  PCAP, transcript, rollback, cleanup, or live proof is accepted by this gate.

## Accepted Proof

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
  passed. The host test binary is generated under
  `/tmp/esp32-espnow-bbs-mesh-discovery-contract-host-tests`, outside the
  repository.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_discovery_contract`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_build_only_target`
  passed: 2 tests OK.
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
  passed.
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4c-contract build`
  passed. ESP-IDF reported app binary size `0x21ab0` and `0xde550` bytes free
  in the smallest app partition.
- The generated project `sdkconfig` was removed after the build.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes, custody codes, bridge request names, coordinator
  serial ABI, and Win31 transport unchanged.
- Keep build output, `sdkconfig`, binary, map, and flash-argument artifacts
  untracked unless a later gate explicitly accepts them.
- Keep firmware runtime closed until a separate implementation gate names exact
  APIs, state ownership, validation, stop gates, and recovery evidence.

## Decision

Accept the M4C firmware contract proof as a bounded Tier 2 step. This proves an
inert firmware-side contract component can compile under host C tests and the
ESP-IDF build-only target. It does not accept firmware runtime behavior,
persistence, live mesh, BLE, Android, router/admin, flash, serial, RF/XBee,
relay/load/mains, release, publication, or deployment authority.
