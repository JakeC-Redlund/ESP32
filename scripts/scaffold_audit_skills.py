#!/usr/bin/env python3
"""Audit repo-local Codex skill routing and local skill inventory."""

from __future__ import annotations

import re
import sys
import tomllib
from collections import defaultdict
from pathlib import Path

from scaffold_audit_data import ROOT


FRONTMATTER_RE = re.compile(r"\A---\n(.*?)\n---\n", re.DOTALL)
FIELD_RE = re.compile(r"^([A-Za-z0-9_-]+)\s*:\s*(.*)$")
MAX_DESCRIPTION_CHARS = 420
REQUIRED_LOCAL_SKILLS = {
    "esp32-live-gate-coordinator",
    "lcd-menu-operations",
    "win31-dashboard-vision-gate",
    "xbee-radio-integration",
}


def _read(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def _frontmatter(path: Path) -> dict[str, str]:
    text = _read(path)
    match = FRONTMATTER_RE.search(text)
    if not match:
        return {}
    data: dict[str, str] = {}
    for line in match.group(1).splitlines():
        field = FIELD_RE.match(line.strip())
        if not field:
            continue
        key, value = field.groups()
        data[key] = value.strip().strip('"').strip("'")
    return data


def _configured_skill_paths(root: Path) -> list[Path]:
    config_path = root / ".codex" / "config.toml"
    config = tomllib.loads(_read(config_path))
    paths: list[Path] = []
    for entry in config.get("skills", {}).get("config", []):
        if not isinstance(entry, dict):
            continue
        value = entry.get("path")
        if not isinstance(value, str):
            continue
        candidate = Path(value)
        if not candidate.is_absolute():
            candidate = root / ".codex" / candidate
        paths.append(candidate)
    return paths


def _skill_md_for_path(path: Path) -> Path:
    if path.name == "SKILL.md":
        return path
    return path / "SKILL.md"


def audit_project_skill_frontmatter(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    names: defaultdict[str, list[str]] = defaultdict(list)
    for skill_md in sorted((root / ".codex" / "skills").glob("*/SKILL.md")):
        rel = skill_md.relative_to(root).as_posix()
        data = _frontmatter(skill_md)
        name = data.get("name", "")
        description = data.get("description", "")
        if not name:
            failures.append(f"{rel} missing frontmatter name")
        else:
            names[name].append(rel)
        if not description:
            failures.append(f"{rel} missing frontmatter description")
        elif len(description) > MAX_DESCRIPTION_CHARS:
            failures.append(f"{rel} description is not concise ({len(description)} chars)")
    for name, paths in sorted(names.items()):
        if len(paths) > 1:
            failures.append(f"duplicate project skill name {name}: {', '.join(paths)}")
    missing_required = sorted(REQUIRED_LOCAL_SKILLS - set(names))
    for name in missing_required:
        failures.append(f"missing required project skill: {name}")
    return failures


def audit_configured_skill_paths(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    configured = _configured_skill_paths(root)
    configured_names: set[str] = set()
    for path in configured:
        skill_md = _skill_md_for_path(path)
        rel = skill_md.relative_to(root).as_posix() if skill_md.is_relative_to(root) else str(skill_md)
        if not skill_md.exists():
            failures.append(f".codex/config.toml configured skill path missing: {rel}")
            continue
        data = _frontmatter(skill_md)
        name = data.get("name")
        if name:
            configured_names.add(name)
    for name in sorted(REQUIRED_LOCAL_SKILLS - configured_names):
        failures.append(f".codex/config.toml does not enable required skill: {name}")
    return failures


def audit_skill_inventory(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    inventory_path = root / "research" / "skills" / "available-skills.md"
    if not inventory_path.is_file():
        return ["research/skills/available-skills.md is missing"]
    text = _read(inventory_path)
    if "optional host discovery is not evaluated" not in text.lower():
        failures.append("skill inventory must distinguish source validation from optional host discovery")
    if "SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-05" not in text:
        failures.append("skill inventory must retain its dated source provenance")
    if "/plugins/cache/" in text:
        failures.append("active skill inventory must use host identities, not version-pinned cache locators")
    seen: set[str] = set()
    project_names: set[str] = set()
    for number, line in enumerate(text.splitlines(), 1):
        if not line.startswith("|"):
            continue
        cells = [value.strip().strip("`") for value in line.strip().strip("|").split("|")]
        if cells[0] == "Skill" or all(re.fullmatch(r"[-:\s]+", value) for value in cells):
            continue
        if len(cells) != 5:
            failures.append(f"skill inventory row {number} must declare skill, scope, locator, availability and use")
            continue
        name, scope, locator, availability, use = cells
        if not re.fullmatch(r"[a-z0-9][a-z0-9:_-]*", name) or name in seen or not use:
            failures.append(f"skill inventory row {number} has an invalid/duplicate identity or empty use")
        seen.add(name)
        if scope == "project_required":
            project_names.add(name)
            expected = f".codex/skills/{name}"
            if locator != expected or availability != "source_checked":
                failures.append(f"project skill {name} needs its repository-relative source contract")
                continue
            skill_md = root / locator / "SKILL.md"
            if not skill_md.resolve().is_relative_to(root.resolve()) or not skill_md.is_file():
                failures.append(f"required inventory skill source missing or outside repository: {name}")
            elif _frontmatter(skill_md).get("name") != name:
                failures.append(f"required inventory skill identity differs from frontmatter: {name}")
        elif scope == "host_optional":
            if locator != f"session-catalog:{name}" or availability != "not_evaluated":
                failures.append(f"optional host skill {name} cannot claim installation/discovery or a fixed local path")
        else:
            failures.append(f"skill inventory row {number} has a missing or unknown scope")
    for name in sorted(REQUIRED_LOCAL_SKILLS - project_names):
        failures.append(f"skill inventory lacks required project identity: {name}")
    return failures


def audit_skills(root: Path = ROOT) -> list[str]:
    failures: list[str] = []
    failures.extend(audit_project_skill_frontmatter(root))
    failures.extend(audit_configured_skill_paths(root))
    failures.extend(audit_skill_inventory(root))
    return failures


def main() -> int:
    failures = audit_skills()
    if failures:
        for failure in failures:
            print(f"FAIL: {failure}")
        return 1
    print("PASS: ESP32 skill source contracts succeeded; optional host discovery is not evaluated")
    return 0


if __name__ == "__main__":
    sys.exit(main())
