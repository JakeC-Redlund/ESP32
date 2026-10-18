#!/usr/bin/env python3
"""Regression tests for the ESP-NOW BBS app_main integration boundary."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT_DIR = ROOT / "scripts"
if str(SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPT_DIR))

from scaffold_audit_firmware import (  # noqa: E402
    audit_espnow_bbs_app_main_bridge_readiness_integration,
    audit_espnow_bbs_app_main_startup_integration,
    audit_espnow_bbs_app_main_retained_recovery_integration,
    audit_espnow_bbs_mesh_discovery_contract,
    audit_espnow_bbs_mesh_retained_persistence,
    audit_espnow_bbs_mesh_runtime_task_shell,
    audit_espnow_bbs_mesh_startup_lifecycle,
)


class EspnowBbsAppMainStartupIntegrationTests(unittest.TestCase):
    def test_app_main_startup_integration_audit_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_app_main_startup_integration())

    def test_app_main_retained_recovery_integration_audit_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_app_main_retained_recovery_integration())

    def test_app_main_bridge_readiness_integration_audit_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_app_main_bridge_readiness_integration())

    def test_retained_persistence_boundary_still_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_mesh_retained_persistence())

    def test_startup_lifecycle_boundary_still_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_mesh_startup_lifecycle())

    def test_runtime_task_shell_boundary_still_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_mesh_runtime_task_shell())

    def test_mesh_discovery_contract_boundary_still_passes(self) -> None:
        self.assertEqual([], audit_espnow_bbs_mesh_discovery_contract())


if __name__ == "__main__":
    unittest.main()
