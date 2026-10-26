#!/usr/bin/env python3
"""Static + runtime content coverage census for the Byzantine extraction.

Reads the extracted per-scene engine tables (CA/CB/CO/CI/CY/CS/CF/CD/CE), the
CC scripts and the six ISO file listings, then runs the player's scene entry
once per scene (using a shipped save when one exists for that scene) and
combines the results.

Usage:
  python3 build_content_coverage.py <data-dir> <port-dir> <iso-listing-glob> \
      <out.json> <out.md>

The report deliberately distinguishes:
  * required assets that ARE shipped and load;
  * required assets that are MISSING from the six CDs;
  * declared sprite records that can never be created (CA type 0xff);
  * objects the shipped script itself keeps disabled (legitimate hidden);
  * records the port loads but does not draw, with the port's skip reason.
A background that opens with no object/character content is reported as such.
"""
import glob
import json
import tempfile
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict

ARG_COUNTS = [
    1, 1, 2, 2, 1, 1, 1, 0, 6, 1, 2, 1, 1, 1, 1, 3,
    3, 3, 3, 3, 4, 1, 3, 3, 1, 1, 3, 3, 2, 1, 2, 3,
    4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 0,
    0, 0, 5, 7, 3, 2, 2, 2, 4, 1, 1, 8, 8, 0, 2, 3,
    8, 3, 2, 2, 1, 3, 8, 0, 2, 2, 3, 2, 2, 2, 2, 2,
    1, 1, 3, 8, 8, 4, 4, 2, 3, 2, 1, 1, 1, 2, 1, 2,
    3, 3, 3, 2, 3, 3, 3, 3, 5, 1, 8, 3, 3, 3, 3, 3,
    3, 2, 2, 1, 1, 0, 0, 1, 1, 3, 3, 3, 3, 3, 4, 4,
    4, 4, 0, 0, 2, 0, 0, 4, 1, 1, 8, 4, 1, 4, 4, 2,
    4, 2, 8, 2, 8, 8, 8, 0, 0, 2, 2, 2, 2, 8, 0, 3,
    0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
]
VIDEO_DIRS = {0: "slide", 1: "intro", 2: "site", 3: "transit", 4: "char", 5: "anim"}


def alen(t):
    return {0: 0, 1: 1, 4: 1, 2: 2, 8: 2, 16: 2, 24: 2, 64: 2, 32: 4}[t]


def parse_cc(path):
    data = open(path, "rb").read()
    count = data[0] | (data[1] << 8)
    at = 2
    ins = []
    for _ in range(count):
        op = data[at]
        at += 1
        args = []
        for _ in range(ARG_COUNTS[op - 0x32]):
            t = data[at]
            at += 1
            n = alen(t)
            value = int.from_bytes(data[at:at + n], "little")
            at += n
            args.append((t, value))
        ins.append({"op": op, "args": args})
    return ins


def parse_records(path):
    """<name>\\0<source>\\0<type byte> repeated."""
    data = open(path, "rb").read()
    out = []
    at = 0
    while at < len(data):
        e = data.index(b"\0", at)
        name = data[at:e].decode("latin1")
        at = e + 1
        e = data.index(b"\0", at)
        source = data[at:e].decode("latin1")
        at = e + 1
        typ = data[at]
        at += 1
        out.append({"name": name, "source": source, "type": typ})
    return out


def parse_listing(pattern):
    files = set()
    for path in glob.glob(pattern):
        with open(path, errors="replace") as handle:
            for line in handle:
                line = line.strip()
                if line:
                    files.add(line.lower().replace("\\", "/"))
    return files


class Coverage:
    def __init__(self, data, port, listing):
        self.data = data
        self.port = port
        self.listing = listing
        self.lcc = [x for x in open(os.path.join(
            data, "copy2hd/engine/SHOW.LCC"), "rb").read().decode(
                "latin1").replace("\r\n", "\n").split("\n") if x.strip()]
        self.lci = [r["name"] for r in parse_records(
            os.path.join(data, "copy2hd/engine/SHOW.LCI"))]
        self.lcy = [r["name"] for r in parse_records(
            os.path.join(data, "copy2hd/engine/SHOW.LCY"))]
        self.lcx = []
        raw = open(os.path.join(data, "copy2hd/engine/SHOW.LCX"), "rb").read()
        count = raw[0] | (raw[1] << 8)
        self.lcx = [raw[2 + 2 * i] | (raw[3 + 2 * i] << 8) for i in range(count)]
        self.anim_set = {f for f in listing if f.startswith("anims/")}
        self.back_set = {f for f in listing if f.startswith("backs/")}
        self.node_set = {f for f in listing
                         if f.startswith(("nodes/", "copy2hd/nodes/"))}
        self.sound_set = {f for f in listing
                          if f.startswith(("sounds/", "copy2hd/sounds/"))}
        self.video_set = {f for f in listing if f.startswith(("video/", "avi/"))}
        self.inventor_set = {f for f in listing if f.startswith("inventor/")}
        self.tables = {}
        for scene in self.lcc:
            self.tables[scene] = self.load_tables(scene)
        self.scene_saves = self.find_saves()

    def load_tables(self, scene):
        def maybe(path):
            if not os.path.exists(path):
                return []
            return parse_records(path)
        # cb/co are the player's extracted copies; the rest come from the
        # per-scene engine tables (byzantine-all split of the .ALL archives).
        return {
            "ca": maybe(os.path.join(self.data, "ca", scene.upper())),
            "cb": maybe(os.path.join(self.port, "output/cb_all", scene.upper())),
            "co": maybe(os.path.join(self.port, "output/co_all", scene.upper())),
            "ci": maybe(os.path.join(self.data, "ci", scene.upper())),
            "cy": maybe(os.path.join(self.data, "cy", scene.upper())),
            "cs": maybe(os.path.join(self.data, "cs", scene.upper())),
            "cf": maybe(os.path.join(self.data, "cf", scene.upper())),
            "cd": maybe(os.path.join(self.data, "cd", scene.upper())),
            "ce": os.path.exists(os.path.join(self.data, "ce", scene.upper())),
        }

    def find_saves(self):
        saves = defaultdict(list)
        savegame = os.path.join(self.data, "copy2hd/savegame")
        if not os.path.isdir(savegame):
            return saves
        for path in sorted(glob.glob(os.path.join(savegame, "*.btb"))):
            try:
                out = subprocess.run(
                    [os.path.join(self.port, "byzantine-cc"), "--save-dump", path,
                     "--lcc", os.path.join(self.data, "copy2hd/engine/SHOW.LCC")],
                    capture_output=True, text=True, timeout=120).stdout
            except Exception:
                continue
            match = re.search(r"scene=(\w+)", out)
            if match:
                saves[match.group(1)].append(os.path.basename(path))
        return saves

    # ---- asset lookups -------------------------------------------------
    def anim_exists(self, name):
        key = name.lower()
        return f"anims/{key[:2]}/{key}.rle" in self.anim_set or \
            f"anims/{key[:2]}/{key}.avi" in self.anim_set

    def back_exists(self, name):
        key = name.lower()
        return f"backs/{key[:2]}/{key}.jpg" in self.back_set

    def node_exists(self, name):
        key = name.lower()
        # The CDs store panoramas at copy2hd/nodes/<scene>/; scene.sh flattens
        # them to nodes/<scene>/ for the player.
        return (f"copy2hd/nodes/{key[:2]}/{key}.jpg" in self.node_set or
                f"nodes/{key[:2]}/{key}.jpg" in self.node_set)

    def inventor_exists(self, name, hot=False):
        # 0x407254 truncates to 8 chars, loads inventor/<name>.tga and then
        # inventor/hotitems/<name with the FIRST character replaced by 'h'>.tga.
        stem = name[:8]
        wanted = ("h" + stem[1:]) if hot and stem else stem
        folder = "inventor/hotitems/" if hot else "inventor/"
        return f"{folder}{wanted.lower()}.tga" in self.inventor_set

    def sound_exists(self, name):
        key = name.lower()
        return any(f.startswith(("sounds/", "copy2hd/sounds/")) and
                   os.path.basename(f) == key + ".wav" for f in self.sound_set)

    def video_exists(self, name, mode=None):
        key = name.lower()
        if f"avi/{key}.avi" in self.video_set:
            return True
        return any(f.startswith("video/") and os.path.basename(f) == key + ".avi"
                   for f in self.video_set)

    def dialog_files(self, name):
        prefix = f"dialog/{name.lower()}/"
        return sorted(f for f in self.listing if f.startswith(prefix))

    def resolve_ci(self, scene, raw):
        table = self.tables[scene]["ci"]
        if raw >= len(table):
            return None
        wanted = table[raw]["name"].lower()
        for index, name in enumerate(self.lci):
            if name.lower() == wanted:
                return index
        return None

    def resolve_cy(self, scene, raw):
        table = self.tables[scene]["cy"]
        if raw >= len(table):
            return None
        wanted = table[raw]["name"].lower()
        for index, name in enumerate(self.lcy):
            if name.lower() == wanted:
                return index
        return None

    # ---- static script census -------------------------------------------
    def static_scene(self, scene):
        ins = parse_cc(os.path.join(self.port, "output/cc_all", scene.upper()))
        labels = {}
        for index, instruction in enumerate(ins):
            if instruction["op"] == 0xe1:
                labels[instruction["args"][0][1]] = index + 1
        enabled = set()
        disabled = set()
        for instruction in ins:
            if instruction["op"] in (0x6d, 0x6e):
                for t, v in instruction["args"]:
                    if t == 2:
                        (enabled if instruction["op"] == 0x6d else disabled).add(v)
        objects = []
        record_ops = []
        backgrounds = []
        panoramas = []
        conversations = []
        videos = []
        text_requests = 0
        sound_defs = 0
        sound_starts = 0
        media_defs = 0
        for index, instruction in enumerate(ins):
            op = instruction["op"]
            args = instruction["args"]
            if op == 0x72 and len(args) == 8:
                vals = [v if t in (2, 64) else None for t, v in args]
                if any(v is None for v in vals):
                    continue
                slot, x, y, w, h, label, kind, name = vals
                objects.append({"slot": slot, "x": x, "y": y, "w": w, "h": h,
                                "label": label, "kind": kind, "name_operand": name,
                                "enabled": slot in enabled, "disabled": slot in disabled})
            elif op in (0x64, 0x84, 0x85, 0x86, 0x8a, 0x8b, 0x91, 0x95) and args:
                if args[0][0] in (2, 64):
                    record_ops.append({"op": op, "id": args[0][1],
                                       "instruction": index})
            elif op == 0x69 and args:
                backgrounds.append({"instruction": index, "index": args[0][1],
                                    "transition": args[1][1] if len(args) > 1 else 0})
            elif op == 0xb9 and args:
                panoramas.append({"instruction": index, "record": args[0][1]})
            elif op == 0x70 and args:
                conversations.append({"instruction": index, "record": args[0][1],
                                      "object": args[1][1] if len(args) > 1 else 0})
            elif op == 0xbd and args:
                videos.append({"instruction": index, "record": args[0][1],
                               "mode": args[1][1] if len(args) > 1 else 0})
            elif op == 0xcd:
                text_requests += 1
            elif op == 0x65:
                if args:
                    sound_defs += 1
            elif op in (0xc4, 0xd1, 0xd3, 0xbc, 0xcf, 0xd0):
                sound_starts += 1
            elif op in (0x6a, 0x6c):
                media_defs += 1
        return {
            "labels": len(labels), "objects": objects, "record_ops": record_ops,
            "backgrounds": backgrounds, "panoramas": panoramas,
            "conversations": conversations, "videos": videos,
            "text_requests": text_requests, "sound_defs": sound_defs,
            "sound_starts": sound_starts, "media_defs": media_defs,
            "enabled_slots": sorted(enabled), "disabled_slots": sorted(disabled),
        }

    # ---- runtime entry ---------------------------------------------------
    def runtime_entry(self, scene):
        save = self.scene_saves.get(scene)
        save_name = save[0] if save else None
        env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy",
                   BYZ_DATA=self.data)
        if save_name:
            command = [os.path.join(self.port, "scene.sh"), ".", scene.upper(),
                       "0", save_name, "--entry", "--no-audio", "--print-objects",
                       "--shot", os.path.join(tempfile.gettempdir(), "byz-coverage.ppm")]
        else:
            command = self.direct_command(scene)
        try:
            result = subprocess.run(command, cwd=os.path.dirname(self.port),
                                    env=env, capture_output=True, text=True,
                                    timeout=240)
        except Exception as error:
            return {"error": str(error), "save": save_name}
        return self.parse_runtime(result.stdout, save_name)

    def direct_command(self, scene):
        d = self.data
        port = self.port
        args = [os.path.join(port, "byzantine-play"),
                os.path.join(port, "output/cc_all", scene.upper()),
                os.path.join(port, "output/cb_all", scene.upper()),
                "0",
                "--engine", os.path.join(d, "copy2hd/engine"),
                "--lcx", os.path.join(d, "copy2hd/engine/SHOW.LCX"),
                "--lci", os.path.join(d, "copy2hd/engine/SHOW.LCI"),
                "--lcy", os.path.join(d, "copy2hd/engine/SHOW.LCY"),
                "--ccb", os.path.join(d, "ccb"),
                "--ccf", os.path.join(d, "ccf"),
                "--byz", os.path.join(d, "byz"),
                "--dialog", os.path.join(d, "dialog"),
                "--dialog-cache", os.path.join(d, "dialog_cache"),
                "--anims", d, "--sounds", d, "--backgrounds", d,
                "--gameprefs", os.path.join(d, "copy2hd/goldsave/GAMEPREF.000"),
                "--inventor", os.path.join(d, "inventor.txt"),
                "--hotitems", os.path.join(d, "hotitems.txt"),
                "--fonts", os.path.join(d, "copy2hd/fonts"),
                "--messages", os.path.join(d, "copy2hd/setupart/MESSAGES.TXT"),
                "--entry", "--no-audio", "--print-objects",
                "--shot", os.path.join(tempfile.gettempdir(), "byz-coverage.ppm")]
        optional = [("--ci", os.path.join(d, "ci", scene.upper())),
                    ("--cy", os.path.join(d, "cy", scene.upper())),
                    ("--cf", os.path.join(d, "cf", scene.upper())),
                    ("--cs", os.path.join(d, "cs", scene.upper())),
                    ("--ca", os.path.join(d, "ca", scene.upper())),
                    ("--co", os.path.join(port, "output/co_all", scene.upper())),
                    ("--cd", os.path.join(d, "cd", scene.upper())),
                    ("--ce", os.path.join(d, "ce", scene.upper()))]
        for key, path in optional:
            if os.path.exists(path):
                args += [key, path]
        return [a for a in args if a]

    def parse_runtime(self, out, save_name):
        data = {"save": save_name}
        match = re.search(
            r"entry scene=(\w+) steps=(\d+) stopped_at=(\d+)(.*?)hotspots=(\d+) "
            r"records=(\d+)", out)
        if match:
            data.update(scene=match.group(1), steps=int(match.group(2)),
                        stopped_at=int(match.group(3)), stop=match.group(4).strip(),
                        hotspots=int(match.group(5)), records=int(match.group(6)))
        else:
            match = re.search(r"^click_result.*|^scene=.*", out, re.M)
            data["entry_line"] = match.group(0) if match else "no entry line"
        for line in out.splitlines():
            if line.startswith("composed "):
                m = re.search(r"composed (?:background|panorama)=(\S+).*?drawn=(\d+)"
                              r" skipped_no_frame=(\d+) skipped_snap=(\d+) "
                              r"skipped_hidden=(\d+) pixels=(\d+)(?:.*?drawn_ids=([\d,]*))?",
                              line)
                if m:
                    data.setdefault("composed", []).append({
                        "layer": m.group(1), "drawn": int(m.group(2)),
                        "skipped_no_frame": int(m.group(3)),
                        "skipped_snap": int(m.group(4)),
                        "skipped_hidden": int(m.group(5)),
                        "pixels": int(m.group(6)),
                        "drawn_ids": [int(x) for x in m.group(7).split(",") if x],
                    })
            elif line.startswith("background_missing"):
                data.setdefault("missing", []).append(line)
            elif line.startswith("panorama_load"):
                data.setdefault("panorama_loads", []).append(line)
        objects = []
        for line in out.splitlines():
            m = re.match(r"object slot=(\d+) x=(-?\d+) y=(-?\d+) w=(\d+) h=(\d+) "
                         r"active=(\d+) kind=0x([0-9a-f]+) label=(\d+) "
                         r"name_id=(\d+) argument=(\d+)(?: name=(.*))?", line)
            if m:
                objects.append({"slot": int(m.group(1)), "kind": int(m.group(7), 16),
                                "label": int(m.group(8)), "name_id": int(m.group(9)),
                                "active": int(m.group(6)),
                                "name": m.group(11) or ""})
        data["objects"] = objects
        bar = [l for l in out.splitlines() if l.startswith("bar_state")]
        if bar:
            data["bar_state"] = bar[-1]
        final = [l for l in out.splitlines()
                 if l.startswith("scene=") and " slots=" in l]
        if final:
            line = final[-1]
            m = re.match(r"scene=(\w+) background=(\S*) hotspots=(\d+) slots=(.*)", line)
            if m:
                data["final"] = {"scene": m.group(1), "background": m.group(2),
                                 "hotspots": int(m.group(3)),
                                 "slots": [int(x) for x in m.group(4).split(",") if x]}
        return data


def main():
    data, port, listing_glob, out_json, out_md = sys.argv[1:6]
    listing = parse_listing(listing_glob)
    coverage = Coverage(data, port, listing)
    report = {}
    for scene in coverage.lcc:
        static = coverage.static_scene(scene)
        tables = coverage.tables[scene]
        ca = tables["ca"]
        ca_by_name = {r["name"].lower(): r for r in ca}
        declared = []
        for obj in static["objects"]:
            entry = None
            resolved = None
            asset = None
            if obj["kind"] not in (0x13, 0x14):
                if obj["name_operand"] < len(tables["ci"]):
                    entry = tables["ci"][obj["name_operand"]]
                resolved = coverage.resolve_ci(scene, obj["name_operand"])
                if resolved is not None and resolved < len(coverage.lci):
                    asset = coverage.lci[resolved]
            declared.append(dict(obj, ci_name=entry["name"] if entry else None,
                                 record=resolved, asset=asset,
                                 chain_id=(obj["name_operand"]
                                           if obj["kind"] in (0x13, 0x14) else None),
                                 icon_present=(coverage.inventor_exists(asset)
                                               if asset and obj["kind"] not in (0x13, 0x14)
                                               else False),
                                 hotitem_present=(coverage.inventor_exists(asset, True)
                                                  if asset and obj["kind"] not in (0x13, 0x14)
                                                  else False)))
        backgrounds = []
        for index, entry in enumerate(tables["cb"]):
            backgrounds.append({"index": index, "name": entry["name"],
                                "present": coverage.back_exists(entry["name"])})
        nodes = []
        for index, entry in enumerate(tables["co"]):
            nodes.append({"index": index, "name": entry["name"],
                          "present": coverage.node_exists(entry["name"])})
        items = []
        for index, entry in enumerate(tables["ci"]):
            items.append({"index": index, "name": entry["name"],
                          "icon": coverage.inventor_exists(entry["name"]),
                          "hotitem": coverage.inventor_exists(entry["name"], True)})
        sounds = [{"index": i, "name": e["name"],
                   "present": coverage.sound_exists(e["name"])}
                  for i, e in enumerate(tables["cs"])]
        videos = [{"index": i, "name": e["name"],
                   "present": coverage.video_exists(e["name"])}
                  for i, e in enumerate(tables["cf"])]
        conversations = []
        for index, entry in enumerate(tables["cd"]):
            files = coverage.dialog_files(entry["name"])
            conversations.append({"index": index, "name": entry["name"],
                                  "clips": len(files),
                                  "ccb": os.path.exists(os.path.join(
                                      data, "ccb", entry["name"].upper())),
                                  "ccf": os.path.exists(os.path.join(
                                      data, "ccf", entry["name"].upper())),
                                  "byz": os.path.exists(os.path.join(
                                      data, "byz", entry["name"].upper()))})
        static["ca"] = [{"index": i, **r, "anim": coverage.anim_exists(r["name"])}
                        for i, r in enumerate(ca)]
        runtime = coverage.runtime_entry(scene)
        report[scene] = {"index": coverage.lcc.index(scene), "static": static,
                         "backgrounds": backgrounds, "nodes": nodes,
                         "declared_objects": declared, "items": items,
                         "sounds": sounds, "videos": videos,
                         "conversations": conversations,
                         "saves": coverage.scene_saves.get(scene, []),
                         "runtime": runtime,
                         "ce": tables["ce"]}
        print(scene, "declared", len(declared), "runtime",
              runtime.get("hotspots", "?"), runtime.get("stop", ""))
    json.dump(report, open(out_json, "w"), indent=1)
    print("wrote", out_json)


if __name__ == "__main__":
    main()
