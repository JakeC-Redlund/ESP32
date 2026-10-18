# Client-1 Static Browser Screenshot/CV Corroboration

Status: accepted-metadata-only-corroboration

Date: 2026-06-06

Source coverage:
`SRC-LOCAL-BBS-UI-CLIENT1-STATIC-BROWSER-PROOF-2026-06-05`,
`SRC-LOCAL-CBBS-REACT-NATIVE-CLIENT-PLATFORM-2026-06-02`, and ADR-0010.

Evidence packet:
`research/bench-records/bbs-ui-client1/local-client1-static-browser-20260605T235752Z/`

This record is metadata-only. It does not track screenshots, logs, bundles,
raw accessibility snapshots, base64 payloads, generated app output, native
projects, or bulky evidence artifacts.

## Routing

- Verified facts: Task 0185 accepted host-only Client-1 static browser proof
  for the CBBS browser fixture. The evidence packet remains ignored by
  `.gitignore` and includes desktop/mobile screenshots, accessibility
  snapshots, loopback-only browser network data, a favicon-only console 404,
  bundle inventory, and server cleanup evidence.
- Assumptions: screenshot pixel statistics and accessibility snapshot checks
  can corroborate the already accepted host-only static browser proof without
  rerunning the browser, modifying app source, or expanding the proof boundary.
- Unknowns/open evidence: live browser/device behavior, selected-board
  identity, coordinator/router/end-node role, Client-2 Wi-Fi proof,
  dummy-output proof, native runtime, BLE, Web Serial, Web Bluetooth,
  SoftAP/LAN, runtime public API, ESP-IDF runtime proof, bridge ABI, serial
  ABI, Gate F service-code map, `mesh_discovery.v1` schema change, firmware
  runtime proof, hardware proof, release, and live gates remain unverified.
- Selected tier: Tier 2 records/evidence corroboration.
- Owner role: React Native Client/UI with QA/Evidence/AgentOps.
- Evidence need: ignored Task 0185 evidence metadata, source coverage, reviewer
  quorum output, task record, scaffold audits, public leak checks, and diff
  checks.
- Mutation boundary: this internal report, Task 0213, and narrow
  status/gap-routing wording only. No app source, package files, lockfiles,
  source index, source ledger, docs index, public docs, Pages tooling, native
  project, runtime, hardware, release, serial, RF, firmware, or live mutation.

## Screenshot Metadata

| Viewport | Image | Dimensions | SHA-256 | Pixel/canvas check |
| --- | --- | --- | --- | --- |
| Desktop | `desktop-1280x800.png` | 1280 x 800 | `9fdcaae1c5453f3882dd8ab7848e77964dbe8fe04dbf0d3d3ffe67d72d5ecef3` | Mean RGB `[230.08, 231.86, 233.79]`, standard deviation `[67.21, 64.12, 60.17]`, extrema `((15,255),(24,255),(39,255))`, 5114 colors. |
| Mobile | `mobile-390x844.png` | 390 x 844 | `d6a8ddd619c47ec61309509701e899f83a1538d63f93b5e9a92a60087ab75c11` | Mean RGB `[224.33, 227.56, 229.79]`, standard deviation `[71.3, 66.06, 62.36]`, extrema `((15,255),(24,255),(39,255))`, 4966 colors. |

The screenshot checks corroborate nonblank, varied-pixel static render evidence
for the already accepted host-only browser fixture. They do not prove live
device connectivity, native runtime behavior, firmware runtime behavior, or
future UI changes.

## Accessibility Snapshot Checks

| Snapshot | Expected terms | Disabled controls | Buttons |
| --- | --- | --- | --- |
| `page-desktop.yml` | All expected Client-1 navigation, closed-authority, transport, radio, browser, SoftAP/LAN, output, config, native, and fixture terms present. | 17 | 33 |
| `page-mobile.yml` | All expected Client-1 navigation, closed-authority, transport, radio, browser, SoftAP/LAN, output, config, native, and fixture terms present. | 17 | 33 |

Expected terms included `CBBS Client`, `HOME`, `DOWNLOADS`, `PEERS`,
`NETWORK`, `DIAGNOSTICS`, `SAFETY`, `CONFIG`, `EVIDENCE`, `Closed Authority`,
`Bridge ABI changes`, `Transport write gate`, `Radio change gate`,
`BLE pairing`, `Web Bluetooth`, `Browser transport gate`, `SoftAP probing`,
`Local-network discovery`, `Device update tools`, `Output power action`,
`Persistent config writes`, `Native prebuild`, `Windows native project
generation`, `External build automation`, and `Transcript-first fixture`.

## Browser Log Checks

| Evidence | Result |
| --- | --- |
| Console log | Only favicon 404 was observed. |
| Network log | Loopback/static-only activity: `GET /` returned 200 and the bundle request returned 304. |
| Evidence size list | `curl-index.html` 34832 bytes, `dist-sha256.txt` 4031 bytes, `dist-file-inventory.txt` 2616 bytes, total 41479 bytes. |

## Corroboration Decision

This packet corroborates Task 0185 Client-1 host-only static browser proof
only. It records that the existing screenshots are nonblank and varied, the
accessibility snapshots contain the expected closed-authority and navigation
surface, and the browser evidence stays within the accepted loopback/static
boundary.

## Not Accepted

This record does not accept live browser/device connectivity, Client-2 Wi-Fi
proof, selected-board identity, coordinator/router/end-node role, dummy-output
proof, native runtime, BLE, Web Serial, Web Bluetooth, SoftAP/LAN, runtime
public API, ESP-IDF runtime proof, bridge ABI, serial ABI, Gate F service-code
map, `mesh_discovery.v1` schema change, Win31 transport change, hardware proof,
release, publication, commit, push, PR, deploy, flash, serial write, RF write,
or any live gate.
