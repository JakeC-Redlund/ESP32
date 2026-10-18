#!/usr/bin/env python3
"""Build and run host tests for the ESP-NOW BBS mesh discovery contract."""

from __future__ import annotations

import os
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
COMPONENT = (
    ROOT
    / "firmware"
    / "projects"
    / "espnow-bbs"
    / "components"
    / "mesh_discovery_contract"
)
BUILD = Path("/tmp/esp32-espnow-bbs-mesh-discovery-contract-host-tests")

COMPONENT_SOURCES = [
    COMPONENT / "src" / "espnow_bbs_mesh_discovery_contract.c",
]

TEST_SOURCES = [
    ROOT
    / "tests"
    / "espnow_bbs_mesh_discovery_contract"
    / "test_mesh_discovery_contract.c",
]


def build_and_run(compiler: str, test_source: Path) -> None:
    output = BUILD / test_source.stem
    command = [
        compiler,
        "-std=c99",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-I",
        str(COMPONENT / "include"),
        "-I",
        str(ROOT / "tests" / "espnow_bbs_mesh_discovery_contract"),
        *[str(source) for source in COMPONENT_SOURCES],
        str(test_source),
        "-o",
        str(output),
    ]
    subprocess.run(command, check=True, cwd=ROOT)
    subprocess.run([str(output)], check=True, cwd=ROOT)


def main() -> int:
    compiler = os.environ.get("CC", "cc")
    BUILD.mkdir(parents=True, exist_ok=True)
    for test_source in TEST_SOURCES:
        build_and_run(compiler, test_source)
    print("PASS: espnow_bbs_mesh_discovery_contract host tests succeeded")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
