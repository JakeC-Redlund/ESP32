"""Portable inventory contracts must not hide missing project configuration."""
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "scripts"))
import scaffold_audit_skills as audit


class SkillInventoryContractTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="esp32-skill-contract-")
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        for name in audit.REQUIRED_LOCAL_SKILLS:
            path = self.root / ".codex/skills" / name / "SKILL.md"
            path.parent.mkdir(parents=True)
            path.write_text(f"---\nname: {name}\ndescription: Use for the project contract.\n---\n")
        entries = "\n".join(
            f'[[skills.config]]\npath = "skills/{name}"\nenabled = true\n'
            for name in sorted(audit.REQUIRED_LOCAL_SKILLS)
        )
        (self.root / ".codex/config.toml").write_text(entries)
        self.inventory = self.root / "research/skills/available-skills.md"
        self.inventory.parent.mkdir(parents=True)
        self.text = (
            "SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-05\n"
            "Optional host discovery is not evaluated.\n"
            "| Skill | Scope | Locator | Availability | Use |\n"
            "| --- | --- | --- | --- | --- |\n"
            + "\n".join(
                f"| {name} | project_required | .codex/skills/{name} | source_checked | Local task |"
                for name in sorted(audit.REQUIRED_LOCAL_SKILLS)
            )
            + "\n| github:yeet | host_optional | session-catalog:github:yeet | not_evaluated | Conditional future task |\n"
        )
        self.inventory.write_text(self.text)

    def test_absent_optional_host_does_not_prevent_source_validation(self):
        self.assertEqual([], audit.audit_skills(self.root))

    def test_missing_configured_project_skill_still_fails(self):
        name = sorted(audit.REQUIRED_LOCAL_SKILLS)[0]
        (self.root / ".codex/skills" / name / "SKILL.md").unlink()
        failures = audit.audit_skills(self.root)
        self.assertTrue(any("configured skill path missing" in x for x in failures))
        self.assertTrue(any("required inventory skill source missing" in x for x in failures))

    def test_unknown_scope_fixed_cache_and_claimed_discovery_are_rejected(self):
        for old, new in [
            ("host_optional", ""),
            ("host_optional", "unknown_scope"),
            ("session-catalog:github:yeet", "/home/example/.codex/plugins/cache/github/old/skills/yeet"),
            ("not_evaluated", "installed"),
        ]:
            with self.subTest(new=new):
                self.inventory.write_text(self.text.replace(old, new))
                self.assertTrue(audit.audit_skill_inventory(self.root))

    def test_empty_inventory_cannot_bypass_validation_without_cache_matches(self):
        for text in ["", "SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-05\nOptional host discovery is not evaluated.\n"]:
            with self.subTest(text=text):
                self.inventory.write_text(text)
                self.assertTrue(audit.audit_skill_inventory(self.root))

    def test_outside_repository_locator_and_duplicate_identity_are_rejected(self):
        name = sorted(audit.REQUIRED_LOCAL_SKILLS)[0]
        for text in [self.text.replace(f".codex/skills/{name}", "../foreign"), self.text + self.text.splitlines()[-1] + "\n"]:
            with self.subTest(text=text[-100:]):
                self.inventory.write_text(text)
                self.assertTrue(audit.audit_skill_inventory(self.root))


if __name__ == "__main__":
    unittest.main()
