#!/usr/bin/env python3
"""Build the interactive IU book slice from the reconstructed CC interpreter."""

import json
import re
import subprocess
from pathlib import Path


PORT = Path(__file__).resolve().parents[1]
ROOT = PORT.parent
CLI = PORT / "byzantine-cc"
CC = PORT / "output/cc_all/IU"
CB = PORT / "output/cb_all/IU"
PAGES = {4: (664, "IUPRO2.jpg"), 5: (660, "IUPRO3.jpg"),
         6: (668, "IUPRO4.jpg"), 7: (676, "IUPRO5.jpg")}


def run(*args):
    return subprocess.run([str(CLI), str(CC), *map(str, args)],
                          check=True, text=True, capture_output=True).stdout


hotspots = {}
for line in run("--hotspots").splitlines():
    data = {key: int(value) for key, value in re.findall(r"(\w+)=(-?\d+)", line)}
    hotspots[data["slot"]] = data


def trace(label):
    lines = run("--trace", label, CB).splitlines()
    if "unsupported_opcode=" in lines[0]:
        return None
    background = next((line for line in lines if line.startswith("background_id=")), None)
    if background is None:
        return None
    background_id = int(re.search(r"background_id=(\d+)", background).group(1))
    slots = [int(match.group(1)) for line in lines
             if (match := re.search(r"^slot=(\d+)", line))]
    return background_id, slots


scenes = {}
for page_id, (label, image) in PAGES.items():
    result = trace(label)
    if result is None or result[0] != page_id:
        raise RuntimeError(f"label {label} did not load background {page_id}")
    if not (PORT / "demo/assets" / image).is_file():
        raise RuntimeError(f"missing image {image}")
    areas = []
    for slot in result[1]:
        hotspot = hotspots[slot]
        destination = trace(hotspot["action"])
        target = destination[0] if destination and destination[0] in PAGES else None
        areas.append({key: hotspot[key] for key in
                      ("slot", "x", "y", "width", "height", "action")}
                     | {"target": target})
    scenes[page_id] = {"label": label, "image": image, "areas": areas}

output = PORT / "demo/scene-data.js"
output.write_text("window.BYZANTINE_SCENES = " +
                  json.dumps(scenes, ensure_ascii=False, indent=2) + ";\n")
print(f"wrote {output.relative_to(ROOT)}: {len(scenes)} scenes")
