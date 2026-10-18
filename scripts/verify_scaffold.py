#!/usr/bin/env python3
"""Validate the ESP32 workspace scaffold."""

from __future__ import annotations

import argparse
import sys
from collections.abc import Callable

from scaffold_audit_agent_process import audit_agent_process
from scaffold_audit_docs import audit_docs
from scaffold_audit_firmware import audit_firmware
from scaffold_audit_pages import audit_pages
from scaffold_audit_paths import audit_paths
from scaffold_audit_react_native import audit_react_native
from scaffold_audit_records import audit_records
from scaffold_audit_sources import audit_sources
from scaffold_audit_skills import audit_skills


AUDITS = [
    ("paths", audit_paths),
    ("sources", audit_sources),
    ("docs", audit_docs),
    ("firmware", audit_firmware),
    ("pages", audit_pages),
    ("react_native", audit_react_native),
    ("agent_process", audit_agent_process),
    ("skills", audit_skills),
    ("records", audit_records),
]
AUDIT_BY_NAME: dict[str, Callable[[], list[str]]] = dict(AUDITS)


def audit_names() -> list[str]:
    return [name for name, _audit in AUDITS]


def run_audit_groups(selected_audit: str | None = None) -> list[tuple[str, list[str]]]:
    groups: list[tuple[str, list[str]]] = []
    selected = AUDITS if selected_audit is None else [(selected_audit, AUDIT_BY_NAME[selected_audit])]
    for name, audit in selected:
        failures = audit()
        if failures:
            groups.append((name, failures))
    return groups


def format_audit_groups(groups: list[tuple[str, list[str]]]) -> list[str]:
    lines: list[str] = []
    if not groups:
        return lines
    lines.append("FAIL: ESP32 scaffold validation failed")
    for name, failures in groups:
        lines.append(f"FAIL-GROUP: {name} ({len(failures)} failures)")
    for name, failures in groups:
        lines.append(f"FAIL-GROUP-DETAILS: {name}")
        for failure in failures:
            lines.append(f"FAIL: [{name}] {failure}")
    return lines


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--audit",
        choices=audit_names(),
        help="Run one named scaffold audit group instead of the full aggregate.",
    )
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    groups = run_audit_groups(args.audit)
    if groups:
        for line in format_audit_groups(groups):
            print(line)
        return 1

    if args.audit:
        print(f"PASS: ESP32 scaffold audit {args.audit} succeeded")
    else:
        print("PASS: ESP32 scaffold validation succeeded")
    return 0


if __name__ == "__main__":
    sys.exit(main())
