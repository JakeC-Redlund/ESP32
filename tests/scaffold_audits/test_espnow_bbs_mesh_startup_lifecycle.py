#!/usr/bin/env python3
"""Regression tests for the ESP-NOW BBS startup lifecycle boundary."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT_DIR = ROOT / "scripts"
if str(SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPT_DIR))

from scaffold_audit_data import ALLOWED_PROJECT_FRAMEWORK_FILES  # noqa: E402
from scaffold_audit_firmware import (  # noqa: E402
    audit_espnow_bbs_mesh_discovery_contract,
    audit_espnow_bbs_mesh_runtime_task_shell,
    audit_espnow_bbs_mesh_startup_lifecycle,
)


class EspnowBbsMeshStartupLifecycleTests(unittest.TestCase):
    def test_component_cmake_is_project_allowlisted(self) -> None:
        self.assertIn(
            (
                "firmware/projects/espnow-bbs/components/"
                "mesh_startup_lifecycle/CMakeLists.txt"
            ),
            ALLOWED_PROJECT_FRAMEWORK_FILES,
        )

    def test_startup_lifecycle_boundary_audit_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_mesh_startup_lifecycle())

    def test_runtime_task_shell_boundary_still_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_mesh_runtime_task_shell())

    def test_mesh_discovery_contract_audit_keeps_marker_allowlists(self) -> None:
        self.assertEqual([], audit_espnow_bbs_mesh_discovery_contract())


if __name__ == "__main__":
    unittest.main()
