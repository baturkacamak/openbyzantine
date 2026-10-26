#!/usr/bin/env python3
"""Measure which opcodes block the portable VM on the real scene scripts.

Runs `port/byzantine-cc <scene.cc> --census <scene.cb>` for every extracted
scene and aggregates the stopping outcome. Each label starts from a fresh
SceneState, so this is static-data interpreter coverage, not original runtime
behavior: carried-over save/scene state is deliberately not invented.

Writes analysis/rea/reconstruction/vm-census.{md,json}.
"""

from __future__ import annotations

import json
import shutil
import subprocess
import tempfile
from collections import Counter
from pathlib import Path

REA = Path(__file__).resolve().parent
ROOT = REA.parent.parent
CC_DIR = ROOT / "port/output/cc_all"
CB_DIR = ROOT / "port/output/cb_all"
CO_DIR = ROOT / "port/output/co_all"
BINARY = ROOT / "port/byzantine-cc"
OUT = REA / "reconstruction"
ISO_GLOB = "*CD*/*.iso"


def prepare_engine_tables() -> tuple[Path | None, tempfile.TemporaryDirectory | None]:
    """Extract the per-scene engine tables behind the recovered opcodes.

    CS.ALL holds the per-scene sound-name table behind 0x47a8e0 (0xc4 compares one
    name with "endgame"). CI.ALL/CY.ALL hold the per-scene CI (0x485400) and CY
    (0x47a2b0) record tables the dialogue opcodes index. CA.ALL holds the
    per-scene animation asset table behind DAT_0047dd70 whose +0x08 type byte
    decides whether a DAT_004649e8 record can exist (0x40fd7b/0x431b0f). SHOW.LCI/SHOW.LCY are the
    global 0x438173 lists whose order is the dialogue record/channel id. The
    inventor and hotitems TGA listings decide 0x407254's media load. The shipped
    savegame set supplies the DAT_00464300/00464f60/00486220/004649e8 tables that
    0xcc/0x70/0x6c read (restored per scene, not invented).
    Returns (directory, tempdir handle) or (None, None).
    """
    if shutil.which("7z") is None:
        return None, None
    isos = sorted(ROOT.glob(ISO_GLOB))
    if not isos:
        return None, None
    temp = tempfile.TemporaryDirectory(prefix="byz-census-")
    members = ["copy2hd/engine/CS.ALL", "copy2hd/engine/CI.ALL",
               "copy2hd/engine/CY.ALL", "copy2hd/engine/CF.ALL",
               "copy2hd/engine/CA.ALL",
               "copy2hd/engine/SHOW.LCI", "copy2hd/engine/SHOW.LCY",
               "copy2hd/engine/SHOW.LCC", "copy2hd/engine/SHOW.LCV",
               "copy2hd/engine/SHOW.LVM", "copy2hd/engine/SHOW.LCX",
               "copy2hd/goldsave/GAMEPREF.000", "copy2hd/savegame/*.btb"]
    worked = subprocess.run(["7z", "x", "-y", str(isos[0]), *members, f"-o{temp.name}"],
                            capture_output=True, text=True)
    engine = Path(temp.name) / "copy2hd/engine"
    if worked.returncode != 0 or not (engine / "SHOW.LCI").exists():
        temp.cleanup()
        return None, None
    listing = subprocess.run(["7z", "l", str(isos[0])], capture_output=True, text=True)
    if listing.returncode != 0:
        temp.cleanup()
        return None, None
    inventor: list[str] = []
    hotitems: list[str] = []
    for line in listing.stdout.splitlines():
        fields = line.split()
        if not fields:
            continue
        name = fields[-1].lower()
        if not name.endswith(".tga"):
            continue
        if name.startswith("inventor/hotitems/"):
            hotitems.append(name.rsplit("/", 1)[1][:-4])
        elif name.startswith("inventor/"):
            inventor.append(name.rsplit("/", 1)[1][:-4])
    # Animation assets behind 0x40fe70: "anims/<first2>/<name>.rle". A scene's
    # asset can live on any CD of the set, so extract the tree from every ISO.
    for iso in isos:
        subprocess.run(["7z", "x", "-y", str(iso), "anims/*", f"-o{temp.name}"],
                       capture_output=True, text=True)

    (Path(temp.name) / "inventor.txt").write_text("\n".join(sorted(inventor)) + "\n")
    (Path(temp.name) / "hotitems.txt").write_text("\n".join(sorted(hotitems)) + "\n")
    for archive, target in (("CS.ALL", "cs"), ("CI.ALL", "ci"), ("CY.ALL", "cy"),
                            ("CF.ALL", "cf"), ("CA.ALL", "ca")):
        entries = Path(temp.name) / target
        entries.mkdir()
        split = subprocess.run(
            [str(BINARY.with_name("byzantine-all")), str(engine / archive), str(entries)],
            capture_output=True, text=True)
        if split.returncode != 0:
            temp.cleanup()
            return None, None
    return Path(temp.name), temp


def map_scene_saves(engine_dir: Path) -> dict[str, Path]:
    """Pick one shipped save per scene via its DAT_00482428 scene index.

    Saves are per-scene snapshots, so a save only carries the object/media/sound
    tables for the scene it was written in. Using a MAX-filename save per scene
    keeps the choice deterministic; a scene with no save stays without object
    state (the trace then reports its absence instead of inventing it).
    """
    save_dir = engine_dir / "copy2hd/savegame"
    lcc = engine_dir / "copy2hd/engine/SHOW.LCC"
    chosen: dict[str, Path] = {}
    if not save_dir.is_dir():
        return chosen
    for path in sorted(save_dir.iterdir()):
        if path.suffix.lower() != ".btb":
            continue
        run = subprocess.run(
            [str(BINARY), str(path), "--save-dump", "--lcc", str(lcc)],
            capture_output=True, text=True, errors="replace")
        if run.returncode != 0:
            continue
        for line in run.stdout.splitlines():
            if line.startswith("scene_index=") and " scene=" in line:
                scene = line.split(" scene=", 1)[1].split()[0]
                if not scene.startswith("<"):
                    chosen[scene] = path
    return chosen


def main() -> int:
    if not BINARY.exists():
        raise SystemExit("build the port first: make -C port -j2 all")

    command_names: dict[str, str] = {}
    command_path = OUT / "vm-commands.json"
    if command_path.exists():
        for entry in json.loads(command_path.read_text())["commands"]:
            if entry.get("name"):
                command_names[entry["opcode"]] = entry["name"]

    engine_dir, engine_temp = prepare_engine_tables()
    scene_saves = map_scene_saves(engine_dir) if engine_dir is not None else {}
    totals: Counter[str] = Counter()
    per_scene: dict[str, dict[str, int]] = {}
    labels_total = 0

    for scene in sorted(p for p in CC_DIR.iterdir() if p.is_file()):
        background = CB_DIR / scene.name
        if not background.exists():
            per_scene[scene.name] = {"_error": "missing CB record"}
            continue
        scene_save = scene_saves.get(scene.name.lower())
        command = [str(BINARY), str(scene), "--census", str(background)]
        if scene_save is not None:
            command += ["--save", str(scene_save)]
        if engine_dir is not None:
            sound_entry = engine_dir / "cs" / scene.name
            if sound_entry.exists():
                command.append(str(sound_entry))
            command += [
                "--lci", str(engine_dir / "copy2hd/engine/SHOW.LCI"),
                "--lcy", str(engine_dir / "copy2hd/engine/SHOW.LCY"),
                "--inventor", str(engine_dir / "inventor.txt"),
                "--hotitems", str(engine_dir / "hotitems.txt"),
                "--gameprefs", str(engine_dir / "copy2hd/goldsave/GAMEPREF.000"),
                "--engine", str(engine_dir / "copy2hd/engine"),
                "--lcx", str(engine_dir / "copy2hd/engine/SHOW.LCX"),
            ]
            command += ["--anims", str(engine_dir)] if engine_temp is not None else []
            if (CO_DIR / scene.name).exists():
                command += ["--co", str(CO_DIR / scene.name)]
            for flag, directory in (("--ci", "ci"), ("--cy", "cy"), ("--cf", "cf"),
                                    ("--ca", "ca")):
                entry = engine_dir / directory / scene.name
                if entry.exists():
                    command += [flag, str(entry)]
        run = subprocess.run(command, capture_output=True, text=True)
        if run.returncode != 0:
            per_scene[scene.name] = {"_error": run.stderr.strip() or "census failed"}
            continue
        counts: dict[str, int] = {}
        for line in run.stdout.splitlines():
            if line.startswith("labels="):
                counts["_labels"] = int(line.split("=", 1)[1])
                labels_total += counts["_labels"]
            elif line.startswith("skipped_node_commands="):
                fields = dict(part.split("=", 1) for part in line.split())
                counts["_skipped_node_commands"] = int(fields["skipped_node_commands"])
                counts["_records_created"] = int(fields.get("records_created", 0))
                counts["_anims_load_failures"] = int(fields.get("anims_load_failures", 0))
                counts["_frame_overflow_logs"] = int(fields.get("frame_overflow_logs", 0))
                totals["_skipped_node_commands"] += counts["_skipped_node_commands"]
                totals["_records_created"] += counts["_records_created"]
                totals["_anims_load_failures"] += counts["_anims_load_failures"]
                totals["_frame_overflow_logs"] += counts["_frame_overflow_logs"]
            elif line.startswith("outcome="):
                key, value = line.split()
                counts[key.split("=", 1)[1]] = int(value.split("=", 1)[1])
                totals[key.split("=", 1)[1]] += counts[key.split("=", 1)[1]]
        per_scene[scene.name] = counts

    state_outcomes = ("return", "script_end", "conversation_request",
                      "unresolved_variable",
                      "unresolved_random", "unresolved_sound_name",
                      "unresolved_dialogue", "unresolved_dialogue_inventory",
                      "unresolved_video_name", "unresolved_video_setting",
                      "unresolved_panorama_node", "unresolved_node_asset",
                      "missing_dialogue_chain", "unresolved_system_input",
                      "unresolved_save_setting", "unresolved_scene_table",
                      "scene_switch", "missing_object",
                      "unresolved_object_label", "missing_media_record",
                      "unresolved_disc_request", "step_limit", "error")
    blocking = [
        (key, count) for key, count in totals.items()
        if key not in state_outcomes and not key.startswith("_")
    ]
    blocking.sort(key=lambda item: (-item[1], item[0]))

    (OUT / "vm-census.json").write_text(
        json.dumps(
            {
                "scope": "fresh SceneState per label; static scripts only",
                "engine_tables": engine_dir is not None,
                "scene_saves": {name: str(path) for name, path in sorted(scene_saves.items())},
                "scenes": len(per_scene),
                "labels": labels_total,
                "totals": dict(totals),
                "skipped_node_commands": totals.get("_skipped_node_commands", 0),
        "records_created": totals.get("_records_created", 0),
        "anims_load_failures": totals.get("_anims_load_failures", 0),
        "frame_overflow_logs": totals.get("_frame_overflow_logs", 0),
                "records_created": totals.get("_records_created", 0),
        "anims_load_failures": totals.get("_anims_load_failures", 0),
        "frame_overflow_logs": totals.get("_frame_overflow_logs", 0),
                "anims_load_failures": totals.get("_anims_load_failures", 0),
                "frame_overflow_logs": totals.get("_frame_overflow_logs", 0),
                "blocking_opcodes": [
                    {
                        "opcode": opcode,
                        "name": command_names.get(opcode),
                        "count": count,
                    }
                    for opcode, count in blocking
                ],
                "per_scene": per_scene,
            },
            indent=1,
        ),
        encoding="utf-8",
    )

    md = [
        "# VM blocking-opcode census",
        "",
        "Generated by `build_vm_census.py` from the extracted scene scripts and the",
        "port's `--census` mode. Each label is traced from a fresh `SceneState`;",
        "state carried between scenes/saves is not invented. When the CD1 set",
        "contains a save for the scene, its 0x464300/0x464f60/0x486220/0x4649e8",
        "tables and the 0x408adc dialogue window state are restored first",
        "(`--save`), and DAT_0047e2f0 is projected through the scene's LCV map",
        "(`--engine`), so 0xcc/0x70/0x6c, the 0x486a10 dialogue chains and the",
        "variable opcodes run against the shipped state instead of an empty table. `return` = the label",
        "reached a supported end, `script_end` = 0x79 ended the script (its",
        "dispatcher result 1 makes 0x42d594 store DAT_00484e28 and stop); `unresolved_variable` / `unresolved_random` = a",
        "variable or RNG read predates the trace; `unresolved_dialogue` /",
        "`unresolved_dialogue_inventory` = the dialogue tables or media listing were",
        "not supplied, `unresolved_video_name` / `unresolved_video_setting` = the",
        "CF video table or the gamepref gate was missing,",
        "`unresolved_panorama_node` = the CO node table was missing,",
        "`unresolved_node_asset` = a record command ran without the CA asset table",
        "(`--ca`), so existence cannot be decided, `missing_node_asset` = the CA",
        "table has no entry for the id and 0x431b0f would read NULL+8,",
        "`missing_dialogue_chain` = 0xc7 saw no DAT_00486a10",
        "chain, `unresolved_system_input` = 0xc8 sub 0x1f needed the input query,",
        "`unresolved_save_setting` = 0xbe/0xd2 had no examine-scene/gamepref gate,",
        "`unresolved_scene_table` = 0x73 had no SHOW.LCX table, `scene_switch` =",
        "0x73 asked for another DAT_00482428 and the trace stopped instead of",
        "continuing with the old scene row,",
        "`missing_object` = 0xcc/0x70 named an absent 0x464300 slot (the original",
        "would dereference NULL), `unresolved_object_label` = 0x70's object label is",
        "not in this scene, `missing_media_record` = 0x7f named an absent 0x464f60",
        "slot (the original loads it before the NULL check), `unresolved_disc_request`",
        "= 0x8d asked for a CD and the drive check is host state,",
        "`_skipped_node_commands` / `_records_created` are diagnostics, not",
        "outcomes: 0x431b0f skips a record command whose CA type is not 0..3 and",
        "the script continues, and a CA type of 0..3 creates the 0x40ffee record.",
        "`step_limit` =",
        "0x70's modelled conversation jump kept returning to the same label, and",
        "everything else is an opcode the port stops on.",
        "",
        f"- Scenes: **{len(per_scene)}**",
        f"- Scenes with a shipped save restored: **{len(scene_saves)}**",
        f"- Labels traced: **{labels_total}**",
        f"- Completed: **{totals.get('return', 0) + totals.get('script_end', 0)}**"
        f" (`return` {totals.get('return', 0)} + `script_end` {totals.get('script_end', 0)})",
        f"- 0x70 conversation pauses (interactive input, not a missing branch): **{totals.get('conversation_request', 0)}**",
        f"- Blocked by unknown variable state: **{totals.get('unresolved_variable', 0)}**",
        f"- Blocked by an unknown RNG seed: **{totals.get('unresolved_random', 0)}**",
        f"- Blocked by an unknown panorama node: **{totals.get('unresolved_panorama_node', 0)}**",
        f"- Blocked by an absent/unresolved object table entry: "
        f"**{totals.get('missing_object', 0) + totals.get('unresolved_object_label', 0)}**",
        f"- Blocked by an undecidable/absent record asset: "
        f"**{totals.get('unresolved_node_asset', 0) + totals.get('missing_node_asset', 0)}**",
        f"- Blocked by a missing dialogue chain: "
        f"**{totals.get('missing_dialogue_chain', 0)}**",
        f"- Blocked by an absent media record: "
        f"**{totals.get('missing_media_record', 0)}**",
        f"- Stopped on a CD request (0x8d, host drive state): "
        f"**{totals.get('unresolved_disc_request', 0)}**",
        f"- Blocked by an input/timer or save-gate unknown: "
        f"**{totals.get('unresolved_system_input', 0) + totals.get('unresolved_save_setting', 0)}**",
        f"- Stopped by the instruction bound after a 0x70 loop: "
        f"**{totals.get('step_limit', 0)}**",
        f"- Scene switches requested by 0x73: **{totals.get('scene_switch', 0)}**",
        f"- Records created from the CA asset table, with their animation "
        f"frame count/offsets (0x40fd7b/0x40fe70): "
        f"**{totals.get('_records_created', 0)}**",
        f"- Record commands skipped because the animation file did not load "
        f"(0x40fe70 failed): **{totals.get('_anims_load_failures', 0)}**",
        f"- `0x91` frame walks past the last frame (0x40cf73 logged): "
        f"**{totals.get('_frame_overflow_logs', 0)}**",
        f"- Record commands skipped because the CA asset type is not 0..3 "
        f"(0x431b0f returns 1; the script continues): "
        f"**{totals.get('_skipped_node_commands', 0)}**",
        f"- Blocked by an unknown video name/setting: "
        f"**{totals.get('unresolved_video_name', 0) + totals.get('unresolved_video_setting', 0)}**",
        f"- Blocked by an unknown dialogue table/style: "
        f"**{totals.get('unresolved_dialogue', 0) + totals.get('unresolved_dialogue_inventory', 0)}**",
        f"- Blocked by an unsupported opcode: **{sum(c for _, c in blocking)}**",
        "",
        "## Blocking opcodes",
        "",
        "| Opcode | Mnemonic | Labels blocked |",
        "| --- | --- | ---: |",
    ]
    for opcode, count in blocking:
        md.append(f"| `{opcode}` | {command_names.get(opcode) or '—'} | {count} |")

    md += ["", "## Per scene", "", "| Scene | Labels | Return | Unresolved | Blocking opcodes |",
           "| --- | ---: | ---: | ---: | --- |"]
    for name in sorted(per_scene):
        counts = per_scene[name]
        if "_error" in counts:
            md.append(f"| {name} | — | — | — | error: {counts['_error']} |")
            continue
        blockers = ", ".join(
            f"`{key}`×{value}" for key, value in counts.items()
            if not key.startswith("_") and key not in state_outcomes
        )
        if counts.get("unresolved_random"):
            blockers = (blockers + ", " if blockers else "") + \
                f"rng×{counts['unresolved_random']}"
        md.append(
            f"| {name} | {counts.get('_labels', 0)} | "
            f"{counts.get('return', 0) + counts.get('script_end', 0)} | "
            f"{counts.get('unresolved_variable', 0)} | {blockers or '—'} |"
        )
    (OUT / "vm-census.md").write_text("\n".join(md) + "\n", encoding="utf-8")
    if engine_temp is not None:
        engine_temp.cleanup()

    print(json.dumps({
        "scenes": len(per_scene),
        "labels": labels_total,
        "return": totals.get("return", 0),
        "script_end": totals.get("script_end", 0),
        "unresolved_variable": totals.get("unresolved_variable", 0),
        "skipped_node_commands": totals.get("_skipped_node_commands", 0),
        "records_created": totals.get("_records_created", 0),
        "anims_load_failures": totals.get("_anims_load_failures", 0),
        "frame_overflow_logs": totals.get("_frame_overflow_logs", 0),
        "blocking_total": sum(c for _, c in blocking),
        "top": blocking[:8],
    }))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
