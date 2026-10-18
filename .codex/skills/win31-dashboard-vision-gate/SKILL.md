---
name: win31-dashboard-vision-gate
description: Use when reviewing copied Win31 OPCON screenshots, OCR/CV JSON, and bridge transcript evidence for ESP-NOW BBS completion gates.
---

# Win31 Dashboard Vision Gate

1. Operate only on copied screenshots, transcripts, and cleanup files. Do not drive DOSBox-X, the Pi desktop, serial ports, or live hardware.
2. Resolve the verified DOS-C checkout independently of the current working
   directory. Its `scripts/win31_dashboard_vision_gate.py` owns this operation;
   ESP32 does not contain that helper. A sibling checkout is a candidate, not
   authority: confirm its DOS-C `AGENTS.md` and the helper before use. Run with
   the helper's resolved absolute path and absolute input/output paths:
   `python3 -B <verified-dos-c-root>/scripts/win31_dashboard_vision_gate.py --screenshot-dir <absolute-dir> --bridge-transcript <absolute-json> --cleanup-proof <absolute-proof> --out <absolute-output-json>`.
   The explicit output JSON is a derived artifact write. If DOS-C is unavailable,
   review copied evidence and report the missing gate instead of guessing a path.
3. Require the output status to be `pass` before using screenshots as corroboration.
4. Keep OCR/CV evidence secondary to bridge transcript behavior. If OCR is weak or views are missing, mark the visual result `needs_manual_review`.
5. Preserve the accepted serial-nullmodem path and closed unsafe lanes.

For visible-state expectations, read `references/opcon-visible-states.md`.
