#!/usr/bin/env python3
"""Build and run host tests for the ESP-NOW BBS retained persistence component."""

from __future__ import annotations

import os
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DISCOVERY_COMPONENT = (
    ROOT
    / "firmware"
    / "projects"
    / "espnow-bbs"
    / "components"
    / "mesh_discovery_contract"
)
RETAINED_COMPONENT = (
    ROOT
    / "firmware"
    / "projects"
    / "espnow-bbs"
    / "components"
    / "mesh_retained_persistence"
)
BUILD = Path("/tmp/esp32-espnow-bbs-mesh-retained-persistence-host-tests")

COMPONENT_SOURCES = [
    DISCOVERY_COMPONENT / "src" / "espnow_bbs_mesh_discovery_contract.c",
    RETAINED_COMPONENT / "src" / "espnow_bbs_mesh_retained_persistence_codec.c",
    RETAINED_COMPONENT / "src" / "espnow_bbs_mesh_retained_persistence_nvs.c",
]

TEST_SOURCES = [
    ROOT
    / "tests"
    / "espnow_bbs_mesh_retained_persistence"
    / "test_mesh_retained_persistence.c",
]


def build_and_run(compiler: str, test_source: Path) -> None:
    output = BUILD / test_source.stem
    command = [
        compiler,
        "-std=c99",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-DESPNOW_BBS_MESH_RETAINED_PERSISTENCE_HOST_TEST",
        "-I",
        str(DISCOVERY_COMPONENT / "include"),
        "-I",
        str(RETAINED_COMPONENT / "include"),
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
    print("PASS: espnow_bbs_mesh_retained_persistence host tests succeeded")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
