# GPS Asset Tracking Candidate Profile

## Status

Candidate telemetry class planning profile only. This profile does not select a
GPS tracker, subscription, API, pivot-position device, or local integration
path.

## Verified Facts

- `SRC-GEOTAB-ASSET-TRACKING-2026-05-25` lists Geotab asset tracking as a
  candidate source for GPS asset tracking classes covering vehicles, trailers,
  containers, heavy equipment, location update cadence examples, utilization
  reporting, and GPS marketplace tracking.
- The source row explicitly does not verify local service subscription, device
  selection, API access, or integration rights.

## Assumptions

- GPS asset tracking is a fleet/asset-reporting class, not proof of center-pivot
  GPS positioning.
- Any future GPS data path must separate precise location data from general
  operational summaries and must define retention/redaction policy before
  export.

## Required Verification Checklist

Use the
[agricultural telemetry evidence-intake packet](../evidence-intake/README.md)
before any implementation gate.

| Item | Required evidence before implementation |
| --- | --- |
| Exact tracker/service | Device model, service/subscription status, API rights, source docs, and owner authorization. |
| Location policy | Data fields, update cadence, retention, redaction, operator access, and precise-location handling. |
| Power and installation | Power source, voltage range, battery behavior, antenna placement, enclosure, and environmental rating. |
| Protocol/API | API endpoint or local interface, authentication, rate limits, failure mode, and no-secret logging rules. |
| Pivot positioning | Separate pivot-position source and local evidence; this asset profile does not prove pivot GPS integration. |

## Unknowns

- Exact local tracker/service, API access, subscription rights, data contract,
  power source, enclosure, antenna, retention/redaction policy, pivot-position
  relationship, and live-proof plan.

## Stop Gates

No GPS tracker selection, service/API integration, pivot GPS position claim,
telemetry schema, firmware dependency, live proof, release, publication,
commit, push, PR, or deploy is accepted by this candidate profile.
