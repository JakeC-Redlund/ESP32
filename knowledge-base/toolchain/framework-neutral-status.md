# Framework-Neutral Status

## Current decision

ADR-0001 keeps the workspace framework-neutral by default.

ADR-0002 accepts ESP-IDF stable v6.0.1 only for the
`four-relay-xbee-wifi` project design and future firmware implementation.

ADR-0003 accepts ESP-IDF stable v6.0.1 only for the `espnow-bbs` project lane.

Task 0232 records a source-backed comparison packet for future project-specific
framework ADRs. It does not select a framework outside `ADR-0002` and
`ADR-0003`.

## Prohibited until ADR acceptance

- `CMakeLists.txt`
- `sdkconfig`
- `sdkconfig.*`
- `platformio.ini`
- `idf_component.yml`
- `arduino-cli.yaml`
- framework-specific firmware source

## Allowed

- Architecture docs
- Interface contracts
- Source-backed hardware profiles
- Verification scripts
- Research ledgers
- Project-specific ADRs and design notes that do not add framework-dependent
  firmware source
