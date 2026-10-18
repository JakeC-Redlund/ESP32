# ESP-NOW BBS Mesh M4 Host Adapter Proof

Source index: [../../knowledge-base/source-index.md](../../knowledge-base/source-index.md)

## Scope

Gate M4 is a Tier 2 host-only adapter fixture for mapping future
ESP-WIFI-MESH-like source events into the existing `mesh_discovery.v1`
simulator contract.

This proof did not add ESP-IDF firmware source, create an ESP-NOW BBS firmware
project target, run an ESP-IDF build, accept firmware runtime implementation,
accept firmware persistence, flash, erase, monitor, write serial, run live
ESP-WIFI-MESH, pair BLE, build an Android app, mutate router/admin settings,
capture PCAP, write RF/XBee settings, operate relays, touch load/mains,
release, publish, commit, push, open a PR, or deploy.

Later Gate M4B records a separate inert target/build-only proof; M4 remains
host-adapter-only.

## Verified Facts

- `ADR-0009` accepts `mesh_discovery.v1` as a host-only simulator/design
  contract. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-2026-05-27`.
- Gate M2-A accepts DOS-C companion bridge/operator support for read-only
  discovery summaries without coordinator serial ABI expansion. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M2A-DOSC-2026-05-27`.
- Gate M3 records the source-backed mapping checklist carried into this M4
  fixture proof:
  event vectors, bridge bounds, secret rejection, live-action rejection,
  queue/custody isolation, and schema/ABI invariance. Source ID:
  `SRC-LOCAL-ESPNOW-FULL-SERVICE-MESH-DISCOVERY-GATE-M3-DESIGN-REVIEW-2026-06-05`.
- The M4 host adapter fixture adds
  `tools/simulators/custom_wireless_protocol/espnow_bbs_mesh_adapter.py` and
  focused tests in
  `tests/custom_wireless_protocol/test_espnow_bbs_custom_protocol.py`.

## Assumptions

- Future firmware may emit event-shaped data equivalent to the fixture inputs,
  but exact ESP-IDF callbacks and route-table APIs are not selected here.
- Host bridge summaries are the correct proof surface for M4 because M5 live
  mesh remains a separate Tier 3 gate.
- Compact bridge payloads may use short labels as long as the internal fixture
  log preserves full event names for validation.

## Unknowns

- Gate M4 did not create an ESP-NOW BBS ESP-IDF project target or build
  output. Later Gate M4B records only an inert target/build proof. No firmware
  memory measurement, task ownership, NVS/flash persistence, runtime recovery,
  route table, parent/root, healing, coexistence, BLE, Android, router/admin,
  PCAP, cleanup, rollback, or live proof exists.

## Accepted Host Proof

- Host fixture covers every accepted discovery event:
  `node_seen`, `node_lost`, `parent_selected`, `root_elected`,
  `root_switched`, `route_added`, `route_removed`, `heal_started`,
  `heal_observed`, `service_seen`, `capability_seen`, and
  `ble_client_seen`.
- Maximum requested host bridge summaries for `discovery_snapshot`,
  `discovery_events`, `service_catalog`, `capability_report`, and the M4
  review report fit the 512-byte bridge line budget.
- Discovery secret rejection is swept recursively across all configured
  discovery secret markers plus suffix/pattern cases.
- Blocked live-action names are rejected through both `process_bridge_request`
  and `RuntimeScheduler`.
- Discovery requests preserve outbound queues, custody state, control-intent
  counters, Gate F service codes, bridge request membership, and discovery
  request membership.

## Compatibility Requirements

- Keep `mesh_discovery.v1` unchanged.
- Keep Gate F service codes and custody codes unchanged.
- Keep discovery bridge request names disjoint from Gate F bridge request
  names.
- Keep discovery summaries host-only and ASCII 512-byte bounded.
- Do not couple discovery, service catalog, capability report, BLE metadata, or
  control intent to live actions.

## Gate Split

- M4 accepted here: host adapter fixture proof and bridge-bound tests only.
- M4B accepted separately: inert ESP-IDF target and no-flash local build proof
  only.
- Future firmware runtime proof: requires a separate gate before runtime APIs,
  persistence, or live behavior are introduced.
- M5: live mesh proof only after fresh identity, backups, hashes,
  router/channel/mesh ID policy, rollback, route-table proof, root/parent
  failure proof, BLE coexistence proof, transcript, and cleanup.

## Decision

Accept the M4 host adapter proof as a bounded Tier 2 fixture. This advances the
planned M4 evidence checklist but does not accept firmware runtime, firmware
build output, live mesh, BLE, Android, router/admin, serial, RF/XBee, relay,
release, publication, or deployment authority.
