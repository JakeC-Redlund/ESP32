from __future__ import annotations

import re
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT_DIR = ROOT / "scripts"
if str(SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPT_DIR))

import scaffold_audit_react_native as rn_audit  # noqa: E402
import scaffold_audit_pages as pages_audit  # noqa: E402
import verify_scaffold  # noqa: E402


def scaffold_ci_matrix_audits() -> list[str]:
    workflow = (ROOT / ".github/workflows/scaffold-ci.yml").read_text(encoding="utf-8")
    match = re.search(
        r"scaffold-audit-matrix:.*?matrix:\n\s+audit:\n(?P<body>(?:\s+- [A-Za-z0-9_]+\n)+)",
        workflow,
        re.DOTALL,
    )
    if not match:
        return []
    return [line.strip()[2:] for line in match.group("body").splitlines()]


def firmware_ci_matrix_projects() -> list[tuple[str, str, str]]:
    workflow = (ROOT / ".github/workflows/firmware-ci.yml").read_text(encoding="utf-8")
    match = re.search(
        r"matrix:\n\s+include:\n(?P<body>(?:\s+- project: [A-Za-z0-9_-]+\n\s+path: [^\n]+\n\s+adr: ADR-\d+\n)+)",
        workflow,
        re.DOTALL,
    )
    if not match:
        return []
    return [
        (item.group("project"), item.group("path"), item.group("adr"))
        for item in re.finditer(
            r"- project: (?P<project>[A-Za-z0-9_-]+)\n\s+path: (?P<path>[^\n]+)\n\s+adr: (?P<adr>ADR-\d+)",
            match.group("body"),
        )
    ]


class ScaffoldAuditReportingTests(unittest.TestCase):
    def test_rnw_generated_output_groups_are_summarized(self) -> None:
        groups = rn_audit.summarize_forbidden_output_groups(
            [
                Path("CbbsClientWindows/obj"),
                Path("CbbsClientWindows/obj/project.assets.json"),
                Path("CbbsClientWindows/x64/Debug"),
                Path("CbbsClientWindows/x64/Debug/app.exe"),
                Path("CbbsClientWindows.Package/bin/x64/Debug/app.exe"),
            ]
        )

        self.assertEqual(
            [
                (Path("CbbsClientWindows.Package/bin"), 1),
                (Path("CbbsClientWindows/obj"), 2),
                (Path("CbbsClientWindows/x64/Debug"), 2),
            ],
            groups,
        )

    def test_verify_scaffold_reports_groups_before_details(self) -> None:
        lines = verify_scaffold.format_audit_groups(
            [
                ("react_native", ["rnw failure"]),
                ("skills", ["skill failure"]),
            ]
        )

        self.assertEqual("FAIL: ESP32 scaffold validation failed", lines[0])
        self.assertEqual("FAIL-GROUP: react_native (1 failures)", lines[1])
        self.assertEqual("FAIL-GROUP: skills (1 failures)", lines[2])
        self.assertLess(
            lines.index("FAIL-GROUP: skills (1 failures)"),
            lines.index("FAIL-GROUP-DETAILS: react_native"),
        )
        self.assertIn("FAIL: [react_native] rnw failure", lines)
        self.assertIn("FAIL: [skills] skill failure", lines)

    def test_verify_scaffold_can_run_one_registered_audit(self) -> None:
        self.assertIn("paths", verify_scaffold.audit_names())
        self.assertEqual([], verify_scaffold.run_audit_groups("paths"))

    def test_scaffold_ci_matrix_matches_verify_scaffold_registry(self) -> None:
        workflow = (ROOT / ".github/workflows/scaffold-ci.yml").read_text(encoding="utf-8")
        self.assertIn("scaffold-audit-matrix:", workflow)
        self.assertIn("fail-fast: false", workflow)
        self.assertIn('scripts/verify_scaffold.py --audit "${{ matrix.audit }}"', workflow)
        self.assertEqual(verify_scaffold.audit_names(), scaffold_ci_matrix_audits())

    def test_firmware_ci_matrix_is_accepted_project_build_only(self) -> None:
        workflow = (ROOT / ".github/workflows/firmware-ci.yml").read_text(encoding="utf-8")
        self.assertIn("name: Firmware CI", workflow)
        self.assertIn("workflow_dispatch:", workflow)
        self.assertIn("pull_request:", workflow)
        self.assertIn("permissions:\n  contents: read", workflow)
        self.assertIn("fail-fast: false", workflow)
        self.assertIn("uses: actions/checkout@v6", workflow)
        self.assertIn("uses: espressif/esp-idf-ci-action@v1", workflow)
        self.assertIn("esp_idf_version: v6.0.1", workflow)
        self.assertIn("target: esp32", workflow)
        self.assertNotIn("command:", workflow)
        for forbidden in [
            "idf.py flash",
            "idf.py monitor",
            "esptool.py",
            "actions/upload-artifact",
            "actions/upload-pages-artifact",
            "actions/deploy-pages",
            "gh release",
        ]:
            self.assertNotIn(forbidden, workflow)
        self.assertEqual(
            [
                ("four-relay-xbee-wifi", "firmware/projects/four-relay-xbee-wifi", "ADR-0002"),
                ("espnow-bbs", "firmware/projects/espnow-bbs", "ADR-0003"),
            ],
            firmware_ci_matrix_projects(),
        )

    def test_pages_workflow_preserves_hidden_nojekyll(self) -> None:
        workflow = (ROOT / ".github/workflows/pages.yml").read_text(encoding="utf-8")
        self.assertIn("actions/upload-pages-artifact@v4", workflow)
        self.assertIn("include-hidden-files: true", workflow)
        self.assertEqual([], pages_audit.audit_pages_build(ROOT))


if __name__ == "__main__":
    unittest.main()
