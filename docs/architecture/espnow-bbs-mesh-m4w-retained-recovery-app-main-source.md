# ESP-NOW BBS Mesh M4W Retained Recovery App Main Source

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4W is a Tier 2 firmware source slice for the `espnow-bbs`
full-service mesh discovery lane. It integrates the accepted M4V retained
persistence source component into `app_main` as source-only retained-recovery
startup wiring.

This packet is accepted source-only after validation and final 8/8 weighted
post-review. It does not accept live NVS execution proof, reboot durability,
runtime persistence proof, first-flash proof, firmware runtime acceptance, live
bridge dispatch, arbitrary bridge frame parsing, serial ABI expansion, bridge
ABI changes, Gate F request or service changes, `mesh_discovery.v1` schema
changes, timer creation, live task execution proof, flash, erase, monitor,
serial writes, ESP-NOW runtime, ESP-WIFI-MESH runtime, BLE pairing, Android
behavior, router/admin mutation, PCAP, RF/XBee writes, relay/load/mains,
release, publication, commit, push, PR, or deploy.

## Verified Facts

- M4V accepts a separate `mesh_retained_persistence` source component with
  source-only NVS adapter code, fixed retained checkpoint codec, and host tests.
  It explicitly does not integrate retained recovery into `app_main`.
- M4O accepts no-flash `app_main` startup wiring over M4N and keeps runtime/live
  behavior unaccepted.
- The retained-persistence API exposes context init, storage init, storage
  load, storage save, and storage erase. M4W wires only context init, storage
  init, and storage load.
- `SRC-ESP-IDF-NVS` remains the source-indexed ESP-IDF NVS API source for the
  underlying adapter surface. M4W does not run NVS on hardware.

## Assumptions

- Static file-scope retained storage and persistence intent are the narrowest
  integration boundary because they preserve explicit app lifetime without
  dynamic allocation.
- App startup may record retained init/load status without using that status to
  replay requests, dispatch bridge actions, or claim runtime persistence proof.
- App-lifetime storage without automatic deinit stays inside this source slice
  because no shutdown path is in scope for M4O/M4W.

## Unknowns

- Live NVS partition behavior, missing/corrupt live partition behavior, reboot
  durability, runtime persistence, first-flash behavior, stack adequacy,
  watchdog behavior, bridge transcript behavior, Win31/OPCON behavior,
  rollback, and live proof remain unaccepted.

## Source Boundary

- Include: `espnow_bbs_mesh_retained_persistence.h`.
- CMake dependency: `mesh_retained_persistence` is a private main component
  dependency; `main` does not depend directly on `nvs_flash`.
- Storage model: static retained storage context and static M4T persistence
  intent in `main.c`.
- Startup ordering: configure existing M4O contract state, initialize retained
  intent to empty volatile state, initialize retained storage context, call
  storage init, call storage load only if init succeeds, record retained
  init/load statuses, then continue existing M4O lifecycle configure/start.
- Write policy: no app-main save or erase trigger is wired. M4V's
  caller-triggered save/erase APIs remain available only in the component and
  are not called by M4W.
- Failure behavior: storage init failure records
  `REJECT_NOT_INITIALIZED`, leaves retained intent empty/volatile, and
  continues the existing M4O lifecycle startup path. Missing, corrupt, rejected,
  wrong-length, or unsupported records are bounded by M4V recovery behavior and
  retained status capture.
- Shutdown/deinit: no automatic retained-storage deinit path is accepted by
  M4W. Future stop, shutdown, deinit, reboot durability, or lifecycle cleanup
  proof remains separately gated.
- Recovery does not step the runtime scheduler, mutate queue/owner state, replay
  bridge requests, write serial, or dispatch live actions.

## Validation Status

Validation passed for the source-only boundary:

- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_discovery_contract/run_host_tests.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_app_main_startup_integration tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_data.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_records.py --min-task-id 216`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_sources.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_docs.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_paths.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_agent_process.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_pages.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_react_native.py`
- `PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_skills.py`
- `source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4w-retained-recovery-app-main-source build`
- Scoped `git diff --check` and closed-surface scans.

The no-flash ESP-IDF build generated
`/tmp/esp32-espnow-bbs-m4w-retained-recovery-app-main-source/espnow_bbs_build_only.bin`
at size `0x283e0` with `0xd7c20` bytes free in the 1 MiB app partition.
Generated `sdkconfig` residue was removed. Post-mutation reviewers
`019e9e95-dea4-7a22-89d8-3f99e86b9dad` and
`019e9e95-fdf2-7143-9649-c35483efad68` approved 8/8 weighted with no P1/P2
blockers and were closed.

## Decision

Decision accepted: source-only app-main retained-recovery integration after
M4V. This is not live NVS execution proof, reboot durability, runtime
persistence proof, flash, serial writes, or live proof.
