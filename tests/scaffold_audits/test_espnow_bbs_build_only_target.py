#!/usr/bin/env python3
"""Regression tests for the ESP-NOW BBS build-only target boundary."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT_DIR = ROOT / "scripts"
if str(SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPT_DIR))

from scaffold_audit_data import ALLOWED_PROJECT_FRAMEWORK_FILES  # noqa: E402
from scaffold_audit_firmware import audit_espnow_bbs_build_only_target  # noqa: E402


class EspnowBbsBuildOnlyTargetTests(unittest.TestCase):
    def test_espnow_bbs_cmake_files_are_project_allowlisted(self) -> None:
        self.assertIn(
            "firmware/projects/espnow-bbs/CMakeLists.txt",
            ALLOWED_PROJECT_FRAMEWORK_FILES,
        )
        self.assertIn(
            "firmware/projects/espnow-bbs/main/CMakeLists.txt",
            ALLOWED_PROJECT_FRAMEWORK_FILES,
        )

    def test_espnow_bbs_target_boundary_audit_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_build_only_target())


if __name__ == "__main__":
    unittest.main()
