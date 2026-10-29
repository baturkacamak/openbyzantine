> Layout update, 28 October 2026: the banner drawer and tabbed overlays below
> have been replaced by a shared notebook category hub and separate pages for
> native entry and F10. See [NOTEBOOK_MENU_DESIGN.md](NOTEBOOK_MENU_DESIGN.md).
> The setting semantics, persistence and language-pack details below still apply.

# Port settings, display modes and language-pack plumbing

Status 28 October 2026. Scope: the host-only features requested after the
original reconstruction was already playable. None of these settings are written
into `savegame/gamepref_000` or a save file: the original seven-row prefs screen
and its `gamepref_000`/`on_off.rle` data stay untouched. The port keeps its own
INI file instead.

## 1. In-game settings (F10)

There are two entry points to the same page:

- **Native entry:** top-bar `TERCICHLER` (`PREFFS.JPG`) now has a
  `GELİŞMİŞ AYARLAR` gold banner/drawer on its lower-right parchment area. It
  opens expanded by default and shows the six group shortcuts (`GÖRÜNTÜ`,
  `SES`, `DİL`, `OTURUM`, `TUŞLAR`, `ORİJİNAL`). Clicking a group opens that
  group on the port page; `ESC`/the settings key returns to `TERCICHLER`.
  The drawer is expanded by default on the lower-right parchment area so the
  six original rows above it stay visible and clickable; the banner itself
  collapses/expands it. No fifth top-bar button and no change to the original
  `gamepref_000` write path.
- **Shortcut:** `F10` from the scene opens the same page over the frozen scene
  with the `CREDBK` backdrop.

The screen is grouped into tabs; the selected row's explanation is shown on the
right, with a `PORT`/`OYUN` scope tag:

| Tab | Rows | Scope |
| --- | --- | --- |
| `GÖRÜNTÜ` | Pencere mode, Olcekleme, VSync, Imlec olcegi | PORT |
| `SES` | Ana ses, Konusma, Ambiyans, Efektler | PORT |
| `DİL` | Metin dili, Ses dili, Altyazi | PORT |
| `OTURUM` | Port oto kaydi, Gorulen satir, Hotspot, Gamepad | PORT |
| `TUŞLAR` | Sekiz port tus atamasi | PORT |
| `ORİJİNAL` | Orijinal oto kayit, video boyutu, ses/muzik, konusma notu, yonlendirici, yer tanitimi, genel aciklama | OYUN (read-only) |

The `ORİJİNAL` tab is deliberately read-only. Those are the six shipped
preferences in `gamepref_000` (labels recovered from `PREFFS.JPG` and the
`0x41f753` loader); the port shows their current values but never writes that
file. The original screen is still the top-bar `TERCICHLER` screen. This is the
answer to the conflicting settings:

- **Video size:** the original `gamepref[1]` is read by the recovered movie
  placement (`0x40d3af`/`0x43a248`); the port's window mode is separate.
- **Autosave:** the port's `Port oto kaydi` writes `autosave-port.btb`; the
  original `OTOMATIK KAYIT` preference remains in `gamepref_000`.
- **Sound/music:** the original `SES VE MUZIK` preference is shown read-only;
  the port mixer levels only scale the port's output and do not rewrite the
  original file.
- **Conversation notes vs subtitles:** the original `KONUSMA NOTLARI` is shown
  read-only; the port `Altyazi` row controls only the green conversation band.
- **Tutorial/location videos:** shown read-only in this page; the original
  TERCICHLER page can toggle them. A toggle writes `gamepref_000` and updates
  the live `--gameprefs` path, so the next trace/travel reads it; a same-scene
  reload also copies the live prefs into the reload snapshot. `0xbd` modes 3/2
  are gated by `gamepref[4]`/`[5]` in the recovered trace code, which fixes the
  previously reported "YER TANITIM GÖRÜNTÜLERİ" switch having no effect after a
  travel.

Navigation: `Tab`/`Shift+Tab` switch groups, Up/Down select, Left/Right change,
Enter activates. Clicking a tab or the left side of a row selects; clicking the
value side cycles. The wheel moves the row. A key row enters capture mode; the
next key is written to the `[keys]` section, `ESC` cancels. Descriptions are
wrapped in the right panel. When opened from TERCICHLER, the native page
composites `PREFFS.JPG` (leather/pen) with the blank right-hand paper page from
`SETUP00.JPG`, then writes the rows on that paper and the description on the
left notebook page. The `F10` shortcut uses a dimmed `CREDBK.JPG`. Both use the
game's `setup`/`dialog` fonts and neither reuses the baked original preference
labels. If either image is unavailable, a leather/paper fallback is drawn.

Defaults: windowed 1280x960, fit, VSync off, all volumes 100 %, subtitles on,
hotspots off, focus mute on, cursor scale 1, autosave on, skip-seen off,
gamepad on.

The original `VideoPlacement::fullscreen` flag is **not** the window mode. It
selects the original's 2x 640x400 movie placement from `gamepref[1]` and remains
part of the recovered game behaviour.

## 2. Keys and persistence

| Key | Action |
| --- | --- |
| `F10` | port settings overlay |
| `F11` | toggle windowed / borderless fullscreen |
| `Alt+Enter` | toggle windowed / exclusive fullscreen |
| `F5` | quick save (`savegame/quicksave.btb`) |
| `F9` | quick load |
| `F8` | load the automatic save (`autosave-port.btb`) |
| `F3` | dialogue backlog (last 400 lines) |
| `F4` | toggle skip-seen dialogue |
| `F12` | screenshot to `port/output/screenshots/` (or `BYZ_SCREENSHOT_DIR`) |
| `Esc` | close overlay / quit the scene as before |
| `H` | hotspot diagnostic toggle as before |

The INI path is `--config <file>`, else `$BYZ_CONFIG`, else
`byzantine.ini` in the working directory. `scene.sh` passes
`port/byzantine.ini` by default. `--no-config` disables loading and saving.
CLI flags override the file (`--windowed`, `--borderless`, `--exclusive`,
`--maximized`, `--scale`, `--vsync`, volume flags, `--subtitles`, and so on).

Window geometry, monitor, mode, scale, cursor scale, focus-mute, autosave,
skip-seen, gamepad and the port action keys are stored in the INI. The
`[keys]` section holds SDL key names (`F10`, `F3`, `H`, ...); the settings
overlay's key rows capture a new binding and write it immediately, e.g.
`settings=F1`, `backlog=B`, `skip=K`, `quicksave=F6`, `quickload=F7`,
`autoload=F8`, `screenshot=P`, `hotspots=O`. `F11` and
`Alt+Enter` remain fixed window shortcuts.

`F3` shows the current session's dialogue history (speaker/answer lines, up to
400 entries) and scrolls with the arrow keys, Page Up/Down and the wheel. It is
available from the scene, media playback, text waits and answer lists. `F4`
toggles skip-seen: a line already recorded in `seen_dialogue.txt` is skipped
(text and AVI) until the toggle is turned off; first-time lines are never
skipped. The seen file lives beside the INI (`seen_dialogue.txt`), or under
`copy2hd/savegame/byzantine_seen.txt` when `--no-config` is used. The display is still composed internally at 640x480 and scaled through
`SDL_RenderSetLogicalSize`; the 16:9 desktop therefore gets bars rather than a
stretched 4:3 picture.

## 3. Audio

`AudioMixer` now tags each voice with a bus (`speech`, `ambient`, `effects`) and
applies master × bus gain in the callback. Conversation AVI audio is speech;
looping `0xc4` records are ambient; other script records are effects (a looping
record is ambient regardless of opcode). The queued-byte contract is unchanged,
so the dialogue drain loop still waits for the actual clip. Gains and mute are
atomic, so changing them from the UI is safe while the callback runs.

`--no-audio` still skips device creation; the settings values remain in the INI
and are applied the next time audio opens.

## 4. Language packs: prepared plumbing

The text/voice split is implemented as an overlay resolver, not as a branch per
language:

- `byzantine::LanguageSelection` (`port/include/byzantine/language.hpp`,
  `port/src/language.cpp`) discovers `<base>/lang/<code>/pack.ini` and resolves
  each asset with the selected pack first and the base data second.
- Text: `MESSAGES.TXT`, split `ce/<scene>`, split `byz/<conversation>`,
  `fonts/`, `setupart/`, `text/CREDITS.TXT`, `inventor/examine/`, `backs/`,
  `nodes/` (the last two for localized signs/documents).
- Voice: split `ccf/`, `dialog/`, `video/`, `sounds/`, `avi/`.
- Scripts, animations, shared backgrounds and music fall back to the base.
- The split-table layout is the one `scene.sh` already builds; a language tree
  must be prepared the same way (split `CE.ALL`, `Byz.all`, `CCF.ALL`, ...).
- The original language marker is modelled: `MESSAGES.TXT` line `0x3e` containing
  `English` selects the direct high-byte font index, otherwise the
  `byte - 0x41` path is used (`SfnFont::english_high_bytes`).
- Text and voice can be mixed, e.g. Turkish text with English voice. The CCF
  line list comes from the voice pack while the BYZ list comes from the text
  pack; this must remain index-compatible. The port logs a missing/extra clip
  instead of silently inventing a line.
- Decoded dialogue and movie caches are keyed by the voice code, so switching
  voice language never reuses the other language's frames/audio.
- `MESSAGES.TXT` encoding is declared by the pack (`raw` or `cp1254`). An
  unknown encoding is logged and passed through raw rather than guessed. The
  shipped Turkish base is the existing `cp1254` path.
- Localized UI art is resolved per file; a pack that contains only the files
  that differ from the base still gets base art for identical files.

`port/language_pack.py` builds such an overlay automatically from a prepared
localized tree:

```sh
./port/language_pack.py --base /tmp/byz-scene --code en \
    --source /tmp/byz-scene-en --name English
./port/scene.sh . MAP 87 tempsave.btb --text-lang en --voice-lang tr
./port/byzantine-play --list-languages --backgrounds /tmp/byz-scene
```

It compares file hashes and copies only files that are added or different. It
does not OCR or classify images; a localized release that ships a different
`preffs.jpg`/document image is the evidence that the image is localized. A file
that is identical in every set stays shared. The manifest also records the
`0x3e` language marker and the resulting font branch. The generated
`build-report.json` keeps counts and examples for review.

**Missing until the localized discs are mounted:** real packs and the
cross-language line-count audit. The code path is exercised with the Turkish
base and a synthetic pack in `port/tests/language.cpp`; no other language is
claimed to work until its files are checked. Movies have no separate subtitle
track, so a movie follows the voice pack; changing only the text language cannot
translate a movie's spoken lines.

## 5. Language audit

`port/language_audit.py` is the read-only step before building packs. It
compares a prepared base root with one or more prepared localized roots and
reports the language marker, the English/non-English font branch, differing
files by category and size, missing files, and BYZ/CCF/CE line-count mismatches:

```sh
./port/language_audit.py --base /tmp/byz-scene \
    --pack en=/tmp/byz-scene-en --pack de=/tmp/byz-scene-de
```

It writes nothing; `--json` gives the same report for tooling. The counts are
the evidence for deciding whether a text/voice mix is line-compatible; the port
also logs `conversation_pairing` at runtime when a mixed conversation does not
match.

## 6. Autosave and continue

When `Oto kayit` is on, a successful scene travel writes
`savegame/autosave-port.btb` with the current scene state. `F8` loads it from
inside the running player. From a shell, `port/continue.sh "<game-dir>"` reads
its scene name through `byzantine-cc`/`SHOW.LCC` and launches `scene.sh` with
that save, so the normal startup state is restored. This is a launcher flow,
not a save-format change: the file is an ordinary save image.

## 7. Full install helper

`port/full_install.sh "<game-dir>" [data-dir]` copies `dialog/`, `video/` and
`avi/` from all six discs into the already prepared data directory. After that
the player no longer needs the ISO for playback; the launcher's `--media-isos`
manifest can stay because existing files are used first. This is also the
base-media step before building voice overlays for other languages.

## 8. Verification

- `make -C port -j2 all test` passes, including `settings-test`,
  `language-test`, `tests/language_pack.py` and `tests/language_audit.py`.
- CMake build and `ctest` pass (14 tests; the Python audit tools are run by the
  Makefile suite).
- Real-window checks on the MAP scene:
  - `TERCICHLER` showed the `GELİŞMİŞ AYARLAR` tab; clicking it logged
    `menu_port_settings open from TERCICHLER`, loaded `PREFFS.JPG` as the
    backdrop, and `ESC` logged `settings_close from_prefs=1` followed by
    `menu_open mode=0 name=preferences` (back on page one).
  - The native page drew the rows on the right paper page and the selected
    row's description on the left notebook page; it did not use the modern
    card/frame.
  - `F10` logged `settings_open`, changing the language row logged
    `messages file="/tmp/byz-scene/lang/en/setupart/MESSAGES.TXT"` and
    `text_lang=en`.
  - Changing the window row logged `window_mode mode=borderless ...` and wrote
    `mode=borderless` to the INI.
  - `F5` logged `quicksave file=.../quicksave.btb bytes=100960 scene=MAP
    scene_index=25`; `F9` reloaded the same scene.
  - A MAP click travelled to MR and wrote
    `autosave file=.../autosave-port.btb bytes=83341 scene=MR`; `F8` then
    logged `autoload ... load scene=MR ... hotspots=2`.
  - `port/continue.sh` resolved the same save to `MAP`/`MR` through
    `SHOW.LCC` and opened the scene with `--entry`.
  - `F3` drew the backlog overlay (empty-history path measured; populated
    history is recorded by `run_conversation`); `F4`
    logged `dialogue_skip_seen enabled=1`; a rebind capture logged
    `settings_key action=17 name=K` and wrote `skip=K`.
  - `F12` wrote a PPM; a gamepad was not attached, so only the
    `gamepad none (no SDL game controller detected)` path is measured, not
    physical controller input.
- Known limitation: the settings page is reachable from the scene (`F10`) and
  from the native `TERCICHLER` screen, not from inside a conversation/clip.
  Text/voice changes from the scene reload the text assets immediately; a
  conversation already in progress must finish first.

## 9. Not included yet

- automatic per-language pack installation (the full-install and pack builder
  are separate commands);
- real language packs themselves: the paths are not mounted in this workspace;
  run `language_audit.py` on the discs, then `language_pack.py` to generate
  the overlays. The audit reports line-count mismatches but not glyph coverage;
- text-only/voice-only pack selection UI beyond the two language rows (the
  manifest already supports a pack with `text=0` or `voice=0`).

These are separate increments; the resolver and INI already have the shape they
need.
