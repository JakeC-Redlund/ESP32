# Handoff 0154 - Full-Service Mesh Discovery Gate M4V Retained Persistence Source Contract To QA/Firmware/Communications

Continuation record for Task 0207
`full-service-mesh-discovery-gate-m4v-retained-persistence-source-contract`.

## Continue with

- Use
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M4V-RETAINED-PERSISTENCE-SOURCE-CONTRACT-2026-06-06`
  only for the source-only `mesh_retained_persistence` component boundary.
- Preserve `mesh_discovery.v1`, accepted discovery request names, Gate F
  service codes, Gate F bridge request membership, coordinator serial ABI,
  bridge ABI, Win31 transport, and the accepted serial-nullmodem live path.
- Treat the NVS adapter as source implementation only. It has no live NVS
  execution, reboot durability, first-flash, runtime persistence, or live proof.

## Stop Gates

- No live NVS execution proof, reboot durability, runtime persistence proof,
  live bridge dispatch, bridge frame parsing, firmware persistence acceptance,
  first-flash target, live ESP-WIFI-MESH, BLE pairing, Android behavior,
  router/admin mutation, PCAP, flash, erase, monitor, serial writes, serial ABI
  expansion, bridge ABI changes, Gate F service-code changes,
  `mesh_discovery.v1` schema changes, Win31 transport changes, RF/XBee writes,
  relay/load/mains work, release, publication, commit, push, PR, deploy,
  `/etc/codex/requirements.toml`, or `admin-strict` mutation is authorized by
  this handoff.
- Do not infer accepted live retained persistence, reboot recovery, live bridge
  behavior, live task timing, stack adequacy, watchdog behavior, route-table,
  root/parent, healing, coexistence, rollback, runtime proof, first-flash proof,
  or live proof from the M4V source-only component.

## Validation to preserve

```bash
PYTHONDONTWRITEBYTECODE=1 python3 tests/espnow_bbs_mesh_retained_persistence/run_host_tests.py
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest tests.scaffold_audits.test_espnow_bbs_mesh_retained_persistence
PYTHONDONTWRITEBYTECODE=1 python3 scripts/scaffold_audit_firmware.py
source /home/cyber/.espressif/tools/activate_idf_v6.0.1.sh && idf.py -C firmware/projects/espnow-bbs -B /tmp/esp32-espnow-bbs-m4v-retained-persistence-source build
```

Recorded: pre-mutation read-only reviewer quorum passed 8/8 with no P1/P2
blockers for opening the source-only M4V retained-persistence boundary.
Retained-persistence host tests, selected scaffold unittests, firmware and
record/source/docs/path/data/agent-process/pages/React Native/skills audits,
no-flash ESP-IDF v6.0.1 build, generated-output cleanup, stale acceptance
wording scan, and `git diff --check` passed. The no-flash build generated
`espnow_bbs_build_only.bin` size `0x22d20` with `0xdd2e0` bytes free in the
1 MiB app partition. Final post-review passed 8/8 weighted after the
wrong-length NVS and source-index wording P2s were resolved.
