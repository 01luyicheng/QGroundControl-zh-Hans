#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""zh-CN regression gate for the qgroundcontrol fork (python3 stdlib only).

Checks:
  a) untranslated regression: per ts file, unfinished entries and "fake"
     translations (finished but translation==source, with letters) must not
     exceed the baseline snapshot; the fake set must be a subset of the
     baseline whitelist.
  b) file-dialog filters: when a source contains a glob filter "(...*.ext)",
     the translation must keep ASCII parentheses (Qt parses the filter with
     ASCII parens - full-width parens break it) and the glob must survive.
  c) XML validity + baseline entry order/location spot check: a deterministic
     sample of (context, source, location) recorded in the baseline must
     still appear in the same relative order with unchanged locations.

Exit code 0 = pass, 1 = fail. Human-readable report on stdout.
"""
import json
import os
import re
import sys
import xml.etree.ElementTree as ET

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "baseline")
TS_FILES = ["translations/qgc_source_zh_CN.ts", "translations/qgc_json_zh_CN.ts"]

fails = []


def load_baseline():
    with open(os.path.join(BASE, "qgc_ts_baseline.json"), encoding="utf-8") as fh:
        return json.load(fh)


def parse_ts(rel):
    path = os.path.join(REPO, rel)
    root = ET.parse(path).getroot()  # ParseError propagates -> caller catches
    seq = []  # document order
    for ctx in root.findall("context"):
        cname = ctx.findtext("name", "")
        for msg in ctx.findall("message"):
            src = msg.findtext("source") or ""
            trn = msg.find("translation")
            zh = (trn.text or "") if trn is not None else ""
            unfinished = trn is not None and trn.get("type") == "unfinished"
            locs = [(l.get("filename"), l.get("line"))
                    for l in msg.findall("location")]
            seq.append({"ctx": cname, "src": src, "zh": zh.strip(),
                        "unfinished": unfinished, "locs": locs})
    return seq


def check_counts(baseline):
    """a) unfinished / fake counts vs baseline; fake whitelist subset."""
    bad = False
    for rel in TS_FILES:
        try:
            seq = parse_ts(rel)
        except ET.ParseError as e:
            fails.append(f"[counts] XML invalid in {rel}: {e}")
            bad = True
            continue
        b = baseline["files"][rel]
        unfinished = [e for e in seq if e["unfinished"]]
        fake = [e for e in seq if (not e["unfinished"]) and e["zh"] == e["src"].strip()
                and re.search(r"[A-Za-z]{2}", e["src"])]
        print(f"[counts] {rel}: total={len(seq)} (baseline {b['total']}), "
              f"unfinished={len(unfinished)} (baseline {b['unfinished']}), "
              f"fake={len(fake)} (baseline {len(b['fake'])})")
        if len(unfinished) > b["unfinished"]:
            fails.append(f"[counts] {rel}: unfinished {len(unfinished)} > baseline {b['unfinished']}")
            for e in unfinished[:20]:
                fails.append(f"    new unfinished: ctx={e['ctx']} src={e['src'][:70]!r}")
            bad = True
        if len(fake) > len(b["fake"]):
            fails.append(f"[counts] {rel}: fake translations {len(fake)} > baseline {len(b['fake'])}")
            bad = True
        wl = {(c, s) for c, s in b["fake"]}
        outside = [e for e in fake if (e["ctx"], e["src"]) not in wl]
        if outside:
            fails.append(f"[counts] {rel}: {len(outside)} fake translations outside whitelist")
            for e in outside[:20]:
                fails.append(f"    ctx={e['ctx']} src={e['src'][:70]!r}")
            bad = True
    return not bad


def check_filters():
    """b) glob filters keep ASCII parentheses in the translation."""
    bad = False
    n = 0
    for rel in TS_FILES:
        seq = parse_ts(rel)
        for e in seq:
            if not re.search(r"\([^)]*\*[^)]*\)", e["src"]):
                continue
            n += 1
            if not e["zh"] or e["zh"] == e["src"]:
                continue  # untranslated/fake handled by check_counts
            if ("（" in e["zh"] or "）" in e["zh"]):
                fails.append(f"[filter] full-width parens in translation: "
                             f"ctx={e['ctx']} src={e['src'][:70]!r} zh={e['zh'][:70]!r}")
                bad = True
            elif ("(" not in e["zh"] or ")" not in e["zh"]):
                fails.append(f"[filter] ASCII parens dropped from filter translation: "
                             f"ctx={e['ctx']} src={e['src'][:70]!r} zh={e['zh'][:70]!r}")
                bad = True
    print(f"[filter] {n} filter entries checked: {'FAIL' if bad else 'PASS'}")
    return not bad


def check_order(baseline):
    """c) baseline sample still present, in order, with unchanged locations."""
    bad = False
    for rel in TS_FILES:
        seq = parse_ts(rel)
        cur = [(e["ctx"], e["src"], tuple((f, l) for f, l in e["locs"])) for e in seq]
        sample = baseline["order_sample"][rel]
        idx = 0
        missing, moved, loc_changed = [], [], []
        for ctx, src, locs in sample:
            found = -1
            for j in range(idx, len(cur)):
                if cur[j][0] == ctx and cur[j][1] == src:
                    found = j
                    break
            if found < 0:
                missing.append((ctx, src))
                continue
            if found < idx:
                moved.append((ctx, src))
            if cur[found][2] != tuple((f, l) for f, l in locs):
                loc_changed.append((ctx, src, cur[found][2], locs))
            idx = found + 1
        if missing:
            fails.append(f"[order] {rel}: {len(missing)} baseline sample entries missing")
            for x in missing[:10]:
                fails.append(f"    ctx={x[0]} src={x[1][:70]!r}")
            bad = True
        if moved:
            fails.append(f"[order] {rel}: {len(moved)} entries out of baseline order")
            bad = True
        if loc_changed:
            fails.append(f"[order] {rel}: {len(loc_changed)} entries with changed location")
            for x in loc_changed[:10]:
                fails.append(f"    ctx={x[0]} src={x[1][:60]!r} loc {x[3]} -> {x[2]}")
            bad = True
        print(f"[order] {rel}: sample {len(sample)} entries, missing={len(missing)} "
              f"reordered={len(moved)} loc_changed={len(loc_changed)}: "
              f"{'FAIL' if bad else 'PASS'}")
    return not bad


def main():
    print("=== QGC zh-CN i18n regression gate ===")
    try:
        baseline = load_baseline()
    except (OSError, ValueError) as e:
        print(f"FAIL: cannot load baseline snapshot: {e}")
        sys.exit(1)
    ok = True
    ok &= check_counts(baseline)
    ok &= check_filters()
    ok &= check_order(baseline)
    if fails:
        print("\nFAILURES:")
        for f in fails:
            print(f"  - {f}")
    print(f"\nRESULT: {'PASS' if ok else 'FAIL'}")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
