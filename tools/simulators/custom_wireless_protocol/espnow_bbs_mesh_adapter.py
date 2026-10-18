#!/usr/bin/env python3
"""Host-only Gate M4 mesh adapter fixture for ESP-NOW BBS planning."""

from __future__ import annotations

from collections.abc import Iterable, Mapping
from typing import Any

from espnow_bbs_custom_protocol import (
    BRIDGE_MAX_LINE_BYTES,
    BRIDGE_REQUEST_TYPES,
    CUSTODY_CODES,
    DISCOVERY_ADMIN_GATE,
    DISCOVERY_BRIDGE_REQUEST_TYPES,
    DISCOVERY_EVENT_TYPES,
    DISCOVERY_MODE,
    DISCOVERY_SCHEMA_VERSION,
    DISCOVERY_TRANSPORT,
    SERVICE_CODES,
    DiscoveryNode,
    ProtocolError,
    ProtocolSimulator,
    assert_no_discovery_secrets,
    encode_bridge_frame,
)
from espnow_bbs_runtime import RuntimeConfig, RuntimeScheduler


M4_ADAPTER_SCHEMA = "mesh_m4.host_adapter.v1"
M4_ADAPTER_MODE = "host_only_build_fixture"


def apply_mesh_adapter_event(
    simulator: ProtocolSimulator,
    event: Mapping[str, Any],
) -> dict[str, Any]:
    """Apply one future firmware-like mesh event to the host discovery model."""

    assert_no_discovery_secrets(event)
    event_type = _require_str(event, "event")
    node_id = _require_str(event, "node")
    detail = _optional_str(event, "detail", "")
    if event_type not in DISCOVERY_EVENT_TYPES:
        raise ProtocolError("discovery_event_unknown", event_type)

    if event_type == "node_seen":
        simulator.observe_discovery_node(_node_from_event(event))
    elif event_type == "node_lost":
        _mark_node_lost(simulator, node_id, event)
    else:
        simulator.record_discovery_event(event_type, node_id, detail=detail)

    return _adapter_ack(event_type, node_id, len(simulator.discovery_event_log))


def apply_mesh_adapter_events(
    simulator: ProtocolSimulator,
    events: Iterable[Mapping[str, Any]],
) -> dict[str, Any]:
    """Apply a bounded fixture sequence and return a bridge-safe proof summary."""

    applied: list[dict[str, Any]] = []
    for event in events:
        applied.append(apply_mesh_adapter_event(simulator, event))
    response = {
        "type": "mesh_adapter_fixture",
        "schema": M4_ADAPTER_SCHEMA,
        "discovery_schema": DISCOVERY_SCHEMA_VERSION,
        "mode": M4_ADAPTER_MODE,
        "accepted": True,
        "events_applied": len(applied),
        "nodes": len(simulator.discovery_nodes),
        "last_event": applied[-1]["event"] if applied else "none",
        "simulator_only": True,
    }
    assert_no_discovery_secrets(response)
    encode_bridge_frame(response)
    return response


def m4_fixture_events() -> tuple[dict[str, Any], ...]:
    """Return one deterministic M4 vector for every accepted discovery event."""

    return (
        _node_event(
            "coord01",
            role="root",
            layer=1,
            parent="none",
            root="coord01",
            capabilities=("mesh_root", "route_table", "heal_observer"),
        ),
        _node_event(
            "peer01",
            role="child",
            layer=2,
            parent="coord01",
            root="coord01",
            rssi=-49,
            capabilities=("mesh_child", "route_leaf"),
        ),
        {"event": "parent_selected", "node": "peer01", "detail": "coord01"},
        {"event": "root_elected", "node": "coord01", "detail": "initial"},
        {"event": "route_added", "node": "peer01", "detail": "layer2"},
        {"event": "service_seen", "node": "peer01", "detail": "bbs_msg"},
        {"event": "capability_seen", "node": "peer01", "detail": "mesh_child"},
        {"event": "ble_client_seen", "node": "peer01", "detail": "metadata_only"},
        {"event": "heal_started", "node": "peer01", "detail": "parent_loss"},
        {"event": "root_switched", "node": "coord01", "detail": "simulated_failover"},
        {"event": "route_removed", "node": "peer01", "detail": "stale_route"},
        {"event": "node_lost", "node": "peer01", "detail": "stale"},
        {"event": "heal_observed", "node": "peer01", "detail": "summary_only"},
    )


def m4_adapter_review_report(runtime: RuntimeScheduler | None = None) -> dict[str, Any]:
    """Summarize M4 memory, queue, persistence, and coexistence boundaries."""

    runtime = runtime if runtime is not None else RuntimeScheduler()
    config = runtime.config
    response = {
        "type": "m4_review",
        "schema": M4_ADAPTER_SCHEMA,
        "disc": DISCOVERY_SCHEMA_VERSION,
        "mode": "host_only",
        "line": BRIDGE_MAX_LINE_BYTES,
        "mem": f"ev={len(m4_fixture_events())};ctr={len(runtime.snapshot()['counters'])};meas=no",
        "q": f"o={config.outbound_radio_jobs};i={config.inbound_packets};a={config.custody_ack_events};deq=no",
        "persist": "volatile;flash=no",
        "coex": "src;rf=no",
        "abi": {
            "disc_req": len(DISCOVERY_BRIDGE_REQUEST_TYPES),
            "svc": len(SERVICE_CODES),
            "cust": len(CUSTODY_CODES),
            "admin": DISCOVERY_ADMIN_GATE,
        },
        "closed": "rt,pst,fl,ser,mesh,ble,rtr,pcap,rf",
        "sim": True,
    }
    assert_no_discovery_secrets(response)
    encode_bridge_frame(response)
    return response


def discovery_invariance_snapshot(runtime: RuntimeScheduler) -> dict[str, Any]:
    """Capture the state that discovery requests must not mutate."""

    snapshot = runtime.snapshot()
    return {
        "queues": dict(snapshot["queues"]),
        "custody": dict(snapshot["custody"]),
        "control_intents": snapshot["counters"]["control_intents"],
        "gate_f_services": dict(SERVICE_CODES),
        "bridge_requests": sorted(BRIDGE_REQUEST_TYPES),
        "discovery_requests": sorted(DISCOVERY_BRIDGE_REQUEST_TYPES),
    }


def _node_from_event(event: Mapping[str, Any]) -> DiscoveryNode:
    return DiscoveryNode(
        node_id=_require_str(event, "node"),
        mac=_optional_str(event, "mac", "00:00:00:00:00:00"),
        link=_optional_str(event, "link", DISCOVERY_TRANSPORT),
        role=_optional_str(event, "role", "child"),
        layer=_optional_int(event, "layer", 1),
        parent=_optional_str(event, "parent", "none"),
        root=_optional_str(event, "root", "coord01"),
        rssi=_optional_int(event, "rssi", -60),
        seen_ms=_optional_int(event, "seen_ms", 0),
        health=_optional_str(event, "health", "ok"),
        services=_optional_str_tuple(event, "services"),
        capabilities=_optional_str_tuple(event, "capabilities"),
        evidence=("m4-host-adapter-fixture",),
    )


def _mark_node_lost(
    simulator: ProtocolSimulator,
    node_id: str,
    event: Mapping[str, Any],
) -> None:
    prior = simulator.discovery_nodes.get(node_id)
    if prior is None:
        lost = _node_from_event(
            {
                **dict(event),
                "event": "node_seen",
                "health": "lost",
                "role": event.get("role", "unknown"),
            }
        )
    else:
        lost = DiscoveryNode(
            node_id=prior.node_id,
            mac=prior.mac,
            link=prior.link,
            role=prior.role,
            layer=prior.layer,
            parent=prior.parent,
            root=prior.root,
            rssi=prior.rssi,
            seen_ms=prior.seen_ms,
            health="lost",
            services=prior.services,
            capabilities=prior.capabilities,
            security=prior.security,
            evidence=prior.evidence,
        )
    simulator.discovery_nodes[node_id] = lost
    simulator.record_discovery_event("node_lost", node_id, detail=_optional_str(event, "detail", "lost"))


def _adapter_ack(event_type: str, node_id: str, seq: int) -> dict[str, Any]:
    return {
        "type": "mesh_adapter_event",
        "schema": M4_ADAPTER_SCHEMA,
        "discovery_schema": DISCOVERY_SCHEMA_VERSION,
        "mode": M4_ADAPTER_MODE,
        "event": event_type,
        "node": node_id,
        "seq": seq,
        "accepted": True,
        "simulator_only": True,
    }


def _node_event(
    node_id: str,
    *,
    role: str,
    layer: int,
    parent: str,
    root: str,
    rssi: int = -38,
    services: tuple[str, ...] = ("bbs_msg", "node_status"),
    capabilities: tuple[str, ...] = (),
) -> dict[str, Any]:
    return {
        "event": "node_seen",
        "node": node_id,
        "mac": "00:00:00:00:00:00",
        "role": role,
        "layer": layer,
        "parent": parent,
        "root": root,
        "rssi": rssi,
        "seen_ms": 100,
        "health": "ok",
        "services": list(services),
        "capabilities": list(capabilities),
    }


def _require_str(payload: Mapping[str, Any], key: str) -> str:
    value = payload.get(key)
    if not isinstance(value, str):
        raise ProtocolError("field_type_invalid", key)
    try:
        value.encode("ascii")
    except UnicodeEncodeError as exc:
        raise ProtocolError("non_ascii", key) from exc
    return value


def _optional_str(payload: Mapping[str, Any], key: str, default: str) -> str:
    if key not in payload:
        return default
    return _require_str(payload, key)


def _optional_int(payload: Mapping[str, Any], key: str, default: int) -> int:
    if key not in payload:
        return default
    value = payload.get(key)
    if not isinstance(value, int) or isinstance(value, bool):
        raise ProtocolError("field_type_invalid", key)
    return value


def _optional_str_tuple(payload: Mapping[str, Any], key: str) -> tuple[str, ...]:
    if key not in payload:
        return ()
    values = payload[key]
    if not isinstance(values, (list, tuple)):
        raise ProtocolError("field_type_invalid", key)
    result: list[str] = []
    for value in values:
        if not isinstance(value, str):
            raise ProtocolError("field_type_invalid", key)
        try:
            value.encode("ascii")
        except UnicodeEncodeError as exc:
            raise ProtocolError("non_ascii", key) from exc
        result.append(value)
    return tuple(result)
