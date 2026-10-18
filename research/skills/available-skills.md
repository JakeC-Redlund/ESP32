# Available Skills Inventory

Historical host observations were recorded on 2026-06-06 from
`SRC-LOCAL-CODEX-SKILL-INVENTORY-REFRESH-2026-06-06`; prior coverage retained from
`SRC-LOCAL-CODEX-SKILL-INVENTORY-REFRESH-2026-06-05`,
`SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-05`,
`SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-03` and
`SRC-LOCAL-CODEX-SKILL-INVENTORY-2026-06-02`.

## Relevant for XBee radio work

| Skill | Scope | Locator | Availability | Task use |
| --- | --- | --- | --- | --- |
| xbee-radio-integration | project_required | `.codex/skills/xbee-radio-integration` | source_checked | Use for Digi XBee, XCTU, XBee Studio, XBee-PRO 900HP/S3B, XBP9B-DPUT-001, radio profile, and repo-local XBee CLI study work. Added by `SRC-LOCAL-XBEE-RADIO-STUDY-2026-05-29`. |

## Relevant for BBS UI system operation work

| Skill | Scope | Locator | Availability | Task use |
| --- | --- | --- | --- | --- |
| expert-agent-panels | host_optional | `session-catalog:expert-agent-panels` | not_evaluated | Controls the evidence-first specialist review loop and task/source-record updates. |
| esp32-live-gate-coordinator | project_required | `.codex/skills/esp32-live-gate-coordinator` | source_checked | Use for ESP-NOW BBS live-gate boundary checks, same-session evidence requirements, and accepted-path preservation. |
| lcd-menu-operations | project_required | `.codex/skills/lcd-menu-operations` | source_checked | Use for LCD menu, HD44780/PCF8574, glyph bank, browser mirror, and host-only LCD simulator work. |
| win31-dashboard-vision-gate | project_required | `.codex/skills/win31-dashboard-vision-gate` | source_checked | Use only for copied Win31 screenshots, OCR/CV JSON, and bridge transcript corroboration. |

## Conditional future use

| Skill | Scope | Locator | Availability | Task use |
| --- | --- | --- | --- | --- |
| github:github | host_optional | `session-catalog:github:github` | not_evaluated | Future GitHub repository, PR, issue, or Pages deployment orientation. |
| github:gh-address-comments | host_optional | `session-catalog:github:gh-address-comments` | not_evaluated | Future actionable PR review comment handling. |
| github:gh-fix-ci | host_optional | `session-catalog:github:gh-fix-ci` | not_evaluated | Future GitHub Actions failure investigation. |
| github:yeet | host_optional | `session-catalog:github:yeet` | not_evaluated | Future commit, push, and draft PR publishing when explicitly requested. |
| imagegen | host_optional | `session-catalog:imagegen` | not_evaluated | Future generated raster assets only; exact factual labels stay in HTML/Markdown. |
| openai-docs | host_optional | `session-catalog:openai-docs` | not_evaluated | Future OpenAI API, model, Codex, or prompt-upgrade documentation checks. |
| plugin-creator | host_optional | `session-catalog:plugin-creator` | not_evaluated | Future local plugin scaffolding. |
| skill-creator | host_optional | `session-catalog:skill-creator` | not_evaluated | Use only if a later authorized edit updates or creates skills. |
| skill-installer | host_optional | `session-catalog:skill-installer` | not_evaluated | Future curated or repo-hosted skill installation. |

## Not relevant to this implementation

| Skill | Scope | Locator | Availability | Task use |
| --- | --- | --- | --- | --- |
| canva:canva-branded-presentation | host_optional | `session-catalog:canva:canva-branded-presentation` | not_evaluated | No Canva deck requested. |
| canva:canva-resize-for-all-social-media | host_optional | `session-catalog:canva:canva-resize-for-all-social-media` | not_evaluated | No Canva social resize requested. |
| canva:canva-translate-design | host_optional | `session-catalog:canva:canva-translate-design` | not_evaluated | No Canva localization requested. |
| davinci-resolve-audio-production | host_optional | `session-catalog:davinci-resolve-audio-production` | not_evaluated | DaVinci Resolve audio automation is out of scope. |
| davinci-resolve-automation | host_optional | `session-catalog:davinci-resolve-automation` | not_evaluated | DaVinci Resolve diagnostics and scripting are out of scope. |
| davinci-resolve-color-automation | host_optional | `session-catalog:davinci-resolve-color-automation` | not_evaluated | DaVinci Resolve color automation is out of scope. |
| davinci-resolve-editing | host_optional | `session-catalog:davinci-resolve-editing` | not_evaluated | DaVinci Resolve edit-page automation is out of scope. |
| davinci-resolve-fusion-composition | host_optional | `session-catalog:davinci-resolve-fusion-composition` | not_evaluated | DaVinci Resolve Fusion composition is out of scope. |
| davinci-resolve-production-review | host_optional | `session-catalog:davinci-resolve-production-review` | not_evaluated | DaVinci Resolve render review is out of scope. |
| davinci-resolve-project-factory | host_optional | `session-catalog:davinci-resolve-project-factory` | not_evaluated | DaVinci Resolve project creation is out of scope. |
| davinci-resolve-world-regeneration | host_optional | `session-catalog:davinci-resolve-world-regeneration` | not_evaluated | Source-backed Resolve world regeneration is out of scope. |


## Notes

- Source-contract review date: 2026-10-18. Optional host discovery is not evaluated.
- Required project skill locators are relative to this repository and remain strict source/configuration validation targets.
- Optional host rows retain historical skill identities only. Resolve availability and any renamed equivalent in the current session catalog before use; a cache file or metadata row does not prove loader discovery.
- Historical paths and cache hashes remain in their dated source ledgers and Git history, rather than serve as current locators.
- This inventory grants no installation, live action, external publication or other authority.
