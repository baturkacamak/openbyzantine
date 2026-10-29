// SDL2 scene player: a real save, its background and its animation records,
// composed by the portable engine and drawn into a window.
//
// 0x43b216's record pass is `compose_scene`; 0x41045b's background is the JPEG
// decoded by `decode_jpeg` (no ffmpeg, no external process). The 640x400
// playfield is surface rows 0x14..0x413 of the original 640x480 scene surface
// (analysis/rea/JPEG_DECODE.md §6), so a hotspot's stored y and a record's
// stored y both carry the +0x14 bias.
//
// Sound: a 0xd1/0xd3 request blocks the SCRIPT, not the window. The label is
// replayed with a step budget up to the instruction that queued the sound, the
// frame is presented, the clip is played while the event loop keeps running
// (so Esc/H still work and clicks are ignored), and the script continues from
// the next instruction. A click-started label uses the same path.
//
// Nothing here invents scene state: the state comes from load_scene_setup (the
// same loader byzantine-cc --trace-save uses) and every click runs a label of
// the shipped script and reports what the VM actually did.

#include "byzantine/ambient_audio.hpp"
#include "byzantine/cb.hpp"
#include "byzantine/cc.hpp"
#include "byzantine/compose.hpp"
#include "byzantine/conversation.hpp"
#include "byzantine/cursors.hpp"
#include "byzantine/diagnostics.hpp"
#include "byzantine/frame_pacing.hpp"
#include "byzantine/inventory.hpp"
#include "byzantine/jpeg.hpp"
#include "byzantine/menu.hpp"
#include "byzantine/menu_typeface.hpp"
#include "byzantine/panorama.hpp"
#include "byzantine/panorama_view.hpp"
#include "byzantine/scene_setup.hpp"
#include "byzantine/text.hpp"
#include "byzantine/transition.hpp"
#include "byzantine/settings.hpp"
#include "byzantine/language.hpp"
#include "byzantine/video.hpp"
#include "byzantine/rle.hpp"

#include <SDL.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace {

using byzantine::kPlayfieldTop;

constexpr int kSceneWidth = 640;
constexpr int kSceneHeight = 400;
// The original scene surface is 640x480: the playfield is rows 0x14..0x413
// and the dialogue band is rows 0x1a4..0x1df. The port's composed scene stays
// 640x400; the conversation presents it at kScreenPlayfieldTop inside this
// 640x480 screen, exactly like 0x40d3af's y=0x14 and 0x424981's y=0x1a4.
constexpr int kScreenWidth = 640;
constexpr int kScreenHeight = 480;
constexpr int kScreenPlayfieldTop = byzantine::kPlayfieldTop;
constexpr int kScreenBandTop = 0x1a4;

std::string lower_ascii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

std::string upper_ascii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::toupper(character));
    });
    return value;
}

std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::string shell_quote(const std::string& text) {
    std::string quoted = "'";
    for (const char ch : text) {
        if (ch == '\'') {
            quoted += "'\\''";
        } else {
            quoted += ch;
        }
    }
    quoted += "'";
    return quoted;
}

// Case-insensitive child lookup: the shipped trees mix .JPG/.jpg and the LCC
// scene names are lowercase while the extracted files are uppercase.
std::optional<std::filesystem::path> find_child(const std::filesystem::path& directory,
                                                const std::string& name) {
    std::error_code error;
    if (!std::filesystem::is_directory(directory, error)) return std::nullopt;
    const auto wanted = lower_ascii(name);
    for (const auto& entry : std::filesystem::directory_iterator(directory, error)) {
        if (entry.is_regular_file() && lower_ascii(entry.path().stem().string()) == wanted) {
            return entry.path();
        }
    }
    return std::nullopt;
}

// CAPTIONS.SFN and CAPTIONS.TGA share a stem, so find_child cannot tell them
// apart. Match the full filename instead.
std::optional<std::filesystem::path> find_filename(const std::filesystem::path& directory,
                                                   const std::string& name) {
    std::error_code error;
    if (!std::filesystem::is_directory(directory, error)) return std::nullopt;
    const auto wanted = lower_ascii(name);
    for (const auto& entry : std::filesystem::directory_iterator(directory, error)) {
        if (entry.is_regular_file() && lower_ascii(entry.path().filename().string()) == wanted) {
            return entry.path();
        }
    }
    return std::nullopt;
}

struct Options {
    std::string cc_path;
    std::string cb_path;
    std::string label;
    std::map<std::string, std::string> scene;  // passthrough for load_scene_setup
    std::filesystem::path backgrounds;         // backs/ root or the extracted root
    std::filesystem::path cursors;             // cursors/ root (default <backgrounds>/cursors)
    std::filesystem::path sounds;              // sounds/ root
    std::filesystem::path fonts;               // the DIALOG/klio font files
    std::filesystem::path ccb;                 // split CCB.ALL (engine/<name>.ccb)
    std::filesystem::path ccf;                 // split CCF.ALL (engine/<name>.ccf)
    std::filesystem::path byz;                 // split Byz.all (engine/<name>.byz)
    std::filesystem::path messages;            // copy2hd/setupart/MESSAGES.TXT
    std::filesystem::path dialog;              // dialog/<conversation>/<clip>.avi root
    std::filesystem::path dialog_cache;        // decoded clip cache (created on demand)
    std::filesystem::path media_isos;          // launcher manifest for lazy cross-scene AVI extraction
    // Host settings and the language overlays resolved from them. These are
    // port choices, never written into the original gamepref/save files.
    byzantine::Settings settings;
    std::filesystem::path config_path;
    std::filesystem::path languages;           // --languages <dir> (default <backgrounds>/lang)
    byzantine::LanguageSelection language;
    bool list_languages = false;               // --list-languages
    std::string shot;                          // --shot <file.ppm>
    std::string conversation_shot;             // --conversation-shot <file.ppm>
    std::string conversation_video_shot;       // first decoded media frame
    std::string video_shot;                    // first decoded 0xbd CF frame
    // --present-dump <dir>: write every newly seen presented 640x480 screen as
    // a PPM (video frames excluded: their JPEGs are already in the clip cache).
    // Bounded by --present-dump-max (or BYZ_PRESENT_DUMP/BYZ_PRESENT_DUMP_MAX).
    std::string present_dump;
    std::size_t present_dump_max = 40;
    // --transition-shot <dir>: write each intermediate frame of a transition as
    // a PPM, so the blend steps can be inspected without a window.
    std::string transition_shot;
    bool entry = false;                        // --entry: start at instruction 0
    std::optional<int> run_label;              // --run-label <n>: after --entry
    std::optional<int> click;                  // --click <slot>
    std::vector<std::pair<int, int>> click_ats;  // --click-at <x>,<y>, repeatable
    // --cursor-at <x>,<y>: install the pointer 0x40c329/0x4083d4 selects for
    // that logical screen point and print it (headless cursor diagnostic).
    std::vector<std::pair<int, int>> cursor_ats;
    std::optional<int> staged_x;               // --staged-x <0..2495>
    // --panorama-yaw [SCENE:]degrees (repeatable): keep the headless camera at
    // an absolute yaw, per scene when a SCENE prefix is given and for every
    // panorama otherwise (a port input for diagnostics, not game state; the
    // original only turns in 0x42423d/0x402285 calls or through the mouse-drag
    // edge logic).
    std::map<std::string, double> panorama_yaw;
    std::optional<int> bar_select;             // --bar-select <item id>
    bool bar_use = false;                      // --bar-use: examine selected item
    int bar_scroll = 0;                        // --bar-next/--bar-prev count
    // The hotspot-outline diagnostic is a port tool, not shipped behavior (the
    // original only changes the cursor). No frames by default: the map's own
    // *Hi art is the hover highlight (hover_highlight). --hotspots-hover draws
    // only the pointer's hotspot, --hotspots/--hotspots-all draw every active
    // one, H toggles. Clips and menus are always excluded (ScreenContent).
    bool overlay = false;
    bool hotspots_all = true;
    // --no-transitions: show the new picture at once instead of running
    // 0x40186e's blend family (0x65..0x6a). --transition <code> overrides the
    // code used when a travel carries none (a flat 0x69 scene load); the
    // original's own code is always preferred when the script has one.
    bool transitions = true;
    std::uint16_t travel_transition = 0x65;
    // True when --transition was given: then a travel whose own data says
    // "no blend" (code 100) still uses that code, because the user asked for a
    // fade. Without it the recovered code wins, including 100 = draw at once.
    bool transition_forced = false;
    bool print_audio = false;
    bool audio = true;
    // --no-audio-tail-guard: keep a clip's unmodified audio stream. Off by
    // default only for a *torn* tail (see trim_torn_audio_tail); every trim is
    // logged as tail_guard=….
    bool audio_tail_guard = true;
    bool video = true;                         // --no-video disables FFmpeg decodes
    bool selftest_events = false;              // --selftest-events (see README)
    bool print_hotspots = false;               // --print-hotspots
    bool print_objects = false;                // --print-objects
    bool print_variables = false;              // --print-variables
    bool print_records = false;                // --print-records
    // --compose-frames N: draw N extra record frames before the shot, so the
    // 0x43bb77 frame walk is observable without a window.
    std::size_t compose_frames = 0;
    // --anim-pass-hz <hz>: reference rate of the original's uncapped record
    // pass (0x43b216 draw + 0x43bb77 step). The shipped +0x04 delays divide
    // this rate, so it sets the animation speed. Default 60 (the delay table's
    // 60 Hz tick scale; see analysis/rea/ANIMATION_RATE_GLOBAL.md).
    unsigned anim_pass_hz = 60;

    // --menu <0..5>: open the top-bar screen at startup (0 preferences,
    // 1 save/load, 2 keşif, 3 NOTLAR, 4 TEMAS LİSTESİ, 5 spread). It is a
    // diagnostic input, like --click-at: the normal path is a real top-bar click.
    std::optional<int> menu_mode;
    // --menu-click-at <x>,<y>: deterministic menu clicks in order.
    std::vector<std::pair<int, int>> menu_clicks;
    std::vector<std::string> menu_texts;       // --menu-text <utf8>
    std::vector<std::string> menu_keys;        // --menu-key enter|escape|backspace
    std::vector<std::pair<int, int>> menu_hovers;  // --menu-hover <x>,<y>
    std::optional<int> credits_frames;         // --credits-frames <n> (headless stop)
    std::string credits_shot;                  // --credits-shot <ppm>
    std::string menu_shot;                     // --menu-shot <file.ppm>
    // --bar-show <0..4>: force the top bar visible with that hovered button for
    // a headless shot (the window path uses the real pointer).
    std::optional<int> bar_show;

    // --conversation-choices 1,0,...: deterministic answers for --shot/tests.
    // The interactive player never uses this: an answer is a real mouse click.
    std::vector<std::size_t> conversation_choices;
    // --conversation-object <slot>: a deterministic scene-object click while an
    // answer list is up (0x436b01's other return path; --shot only).
    std::optional<int> conversation_object;
    // --conversation-cancel N: cancel at the Nth answer list (1-based, all
    // conversations in the run). 0 is off. This is the deterministic counterpart
    // of ESC at 0x436b01; it returns 0xfffe and keeps the scene.
    int conversation_cancel = 0;
};

const std::set<std::string> kValued = {
    "--save", "--engine", "--lcx", "--lci", "--ci", "--lcy", "--cy", "--co", "--cf",
    "--config", "--languages", "--text-lang", "--voice-lang", "--scale",
    "--volume-master", "--volume-speech", "--volume-ambient", "--volume-effects",
    "--cursor-scale",
    "--ca", "--anims", "--anims-overlay", "--gameprefs", "--inventor", "--hotitems", "--cs", "--ce",
    "--backgrounds", "--cursors", "--sounds", "--shot", "--click", "--click-at", "--staged-x",
    "--fonts", "--cd", "--ccb", "--ccf", "--byz", "--messages", "--dialog",
    "--dialog-cache",
    "--run-label", "--conversation-choices", "--conversation-shot",
    "--conversation-object", "--conversation-video-shot", "--conversation-cancel",
    "--video-shot", "--transition", "--transition-shot",
    "--bar-select", "--media-isos", "--anim-pass-hz", "--cursor-at",
    "--panorama-yaw", "--menu", "--menu-click-at", "--menu-shot", "--bar-show",
    "--menu-text", "--menu-key", "--menu-hover", "--credits-frames",
    "--credits-shot"};
std::filesystem::path config_path_from_arguments(int argc, char** argv) {
    for (int index = 1; index < argc; ++index) {
        const std::string token(argv[index]);
        if (token == "--no-config") return {};
        if (token != "--config" || index + 1 >= argc) continue;
        return std::filesystem::path(argv[index + 1]);
    }
    if (const char* environment = std::getenv("BYZ_CONFIG"); environment != nullptr &&
                                                              *environment != '\0') {
        return std::filesystem::path(environment);
    }
    return std::filesystem::path("byzantine.ini");
}

float parse_volume(const std::string& token, const std::string& value) {
    try {
        std::size_t used = 0;
        const auto parsed = std::stof(value, &used);
        if (used != value.size() || !(parsed >= 0.0f && parsed <= 1.0f)) {
            throw std::runtime_error("out of range");
        }
        return parsed;
    } catch (const std::exception&) {
        throw std::runtime_error(token + " expects a value in 0..1");
    }
}

void resolve_language_selection(Options& options) {
    const auto root = options.languages.empty() ? options.backgrounds / "lang"
                                                : options.languages;
    options.language = byzantine::LanguageSelection::discover(
        options.backgrounds, root, options.settings.text_language,
        options.settings.voice_language, &std::cerr);
}

bool save_port_settings(const Options& options, bool verbose) {
    if (options.config_path.empty()) return false;
    std::string error;
    if (!options.settings.save(options.config_path, &error)) {
        std::cout << "settings_write failed file=" << options.config_path
                  << " error=" << error << '\n';
        return false;
    }
    if (verbose) {
        std::cout << "settings_write file=" << options.config_path
                  << " text=" << options.settings.text_language
                  << " voice=" << options.settings.voice_language << '\n';
    }
    return true;
}

// Language-aware asset lookup. The pack is tried first and the base data is the
// fallback, so a text-only pack can leave voice/media shared and a full pack can
// replace every localized file. `dir` is relative to the data root ("fonts",
// "setupart", "inventor/examine", ...); the shipped copy2hd/ prefix is tried too.
// The shipped font indexes Turkish bytes directly (see text.cpp): Ş=0xa2,
// İ=0xa3, Ç=0xa1. Keep the visible title in the game's own encoding.
const std::string& advanced_settings_label() {
    static const std::string label =
        std::string("GEL") + "\xa3" + "\xa2" + "M" + "\xa3" + "\xa2" + " AYARLAR";
    return label;
}

constexpr int kAdvancedGroupCount = 6;

const char* advanced_group_name(int group) {
    switch (group) {
        case 0: return "G\xa6R\xa5NT\xa5";   // GÖRÜNTÜ
        case 1: return "SES";
        case 2: return "D\xa3L";              // DİL
        case 3: return "OTURUM";
        case 4: return "TU\xa2LAR";           // TUŞLAR
        case 5: return "OR\xa3J\xa3NAL";      // ORİJİNAL
        default: return "?";
    }
}

bool key_is(const byzantine::Settings& settings, const std::string& name,
            SDL_Keycode code) {
    (void)settings;
    const auto bound = SDL_GetKeyFromName(name.c_str());
    return bound != SDLK_UNKNOWN && bound == code;
}

std::optional<std::filesystem::path> find_language_child(const Options& options,
                                                         bool voice,
                                                         const std::string& dir,
                                                         const std::string& name) {
    const byzantine::LanguagePack* pack =
        voice ? options.language.voice : options.language.text;
    if (pack != nullptr) {
        for (const auto& candidate : {dir, std::string("copy2hd/") + dir}) {
            const auto directory = byzantine::find_relative(pack->root, candidate);
            if (!directory) continue;
            if (const auto file = find_child(*directory, name)) return file;
        }
    }
    for (const auto& candidate : {dir, std::string("copy2hd/") + dir}) {
        const auto directory = byzantine::find_relative(options.language.base, candidate);
        if (!directory) continue;
        if (const auto file = find_child(*directory, name)) return file;
    }
    return std::nullopt;
}

std::optional<std::filesystem::path> find_language_filename(const Options& options,
                                                            bool voice,
                                                            const std::string& dir,
                                                            const std::string& name) {
    const byzantine::LanguagePack* pack =
        voice ? options.language.voice : options.language.text;
    if (pack != nullptr) {
        for (const auto& candidate : {dir, std::string("copy2hd/") + dir}) {
            const auto directory = byzantine::find_relative(pack->root, candidate);
            if (!directory) continue;
            if (const auto file = find_filename(*directory, name)) return file;
        }
    }
    for (const auto& candidate : {dir, std::string("copy2hd/") + dir}) {
        const auto directory = byzantine::find_relative(options.language.base, candidate);
        if (!directory) continue;
        if (const auto file = find_filename(*directory, name)) return file;
    }
    return std::nullopt;
}

std::string decode_messages_line(const Options& options, const std::string& line) {
    const auto encoding = options.language.messages_encoding();
    if (encoding == "raw") return line;
    if (encoding == "cp1254" || encoding == "turkish") {
        return byzantine::cp1254_to_game_text(line);
    }
    // A pack that declares another encoding must ship text in its own font
    // encoding (or a converter). Do not guess a byte map: leave the bytes and
    // report once so the pack can be fixed without corrupting the text.
    static std::string reported;
    if (reported != encoding) {
        reported = encoding;
        std::cout << "messages encoding=" << encoding
                  << " is not built in; using the raw pack bytes\n";
    }
    return line;
}

Options parse_options(int argc, char** argv) {
    Options options;
    options.config_path = config_path_from_arguments(argc, argv);
    std::string config_error;
    const bool config_loaded =
        !options.config_path.empty() && options.settings.load(options.config_path, &config_error);
    if (!config_error.empty()) {
        std::cerr << "settings_read " << options.config_path << ": " << config_error << '\n';
    }
    if (config_loaded) {
        std::cout << "settings_read file=" << options.config_path
                  << " mode=" << byzantine::window_mode_name(options.settings.window_mode)
                  << " scale=" << byzantine::scale_mode_name(options.settings.scale_mode)
                  << " text=" << options.settings.text_language
                  << " voice=" << options.settings.voice_language << '\n';
    }
    // CLI hotspot flags override the file; without them the saved values apply.
    options.overlay = options.settings.hotspots;
    options.hotspots_all = options.settings.hotspots_all;
    std::vector<std::string> positional;
    for (int index = 1; index < argc; ++index) {
        const std::string token(argv[index]);
        if (token == "--no-hotspots") {
            options.overlay = false;
            options.settings.hotspots = false;
            continue;
        }
        if (token == "--hotspots" || token == "--hotspots-all") {
            options.overlay = true;
            options.hotspots_all = true;
            options.settings.hotspots = true;
            options.settings.hotspots_all = true;
            continue;
        }
        if (token == "--hotspots-hover") {
            options.overlay = true;
            options.hotspots_all = false;
            options.settings.hotspots = true;
            options.settings.hotspots_all = false;
            continue;
        }
        if (token == "--windowed") {
            options.settings.window_mode = byzantine::WindowMode::windowed;
            continue;
        }
        if (token == "--borderless" || token == "--fullscreen") {
            options.settings.window_mode = byzantine::WindowMode::borderless;
            continue;
        }
        if (token == "--exclusive") {
            options.settings.window_mode = byzantine::WindowMode::exclusive;
            continue;
        }
        if (token == "--maximized") {
            options.settings.window_mode = byzantine::WindowMode::maximized;
            continue;
        }
        if (token == "--vsync") {
            options.settings.vsync = true;
            continue;
        }
        if (token == "--no-vsync") {
            options.settings.vsync = false;
            continue;
        }
        if (token == "--subtitles") {
            options.settings.subtitles = true;
            continue;
        }
        if (token == "--no-subtitles") {
            options.settings.subtitles = false;
            continue;
        }
        if (token == "--mute-on-focus-loss") {
            options.settings.mute_on_focus_loss = true;
            continue;
        }
        if (token == "--no-mute-on-focus-loss") {
            options.settings.mute_on_focus_loss = false;
            continue;
        }
        if (token == "--list-languages") {
            options.list_languages = true;
            continue;
        }
        if (token == "--no-config") {
            options.config_path.clear();
            continue;
        }
        if (token == "--print-audio") {
            options.print_audio = true;
            continue;
        }
        if (token == "--no-audio") {
            options.audio = false;
            continue;
        }
        if (token == "--no-audio-tail-guard") {
            options.audio_tail_guard = false;
            continue;
        }
        if (token == "--no-video") {
            options.video = false;
            continue;
        }
        if (token == "--no-transitions") {
            options.transitions = false;
            continue;
        }
        if (token == "--selftest-events") {
            options.selftest_events = true;
            continue;
        }
        if (token == "--print-hotspots") {
            options.print_hotspots = true;
            continue;
        }
        if (token == "--print-objects") {
            options.print_objects = true;
            continue;
        }
        if (token == "--print-variables") {
            options.print_variables = true;
            continue;
        }
        if (token == "--print-records") {
            options.print_records = true;
            continue;
        }
        if (token == "--compose-frames") {
            if (index + 1 >= argc) throw std::runtime_error(token + " needs a value");
            options.compose_frames = static_cast<std::size_t>(
                std::stoul(argv[++index]));
            continue;
        }
        if (token == "--bar-use") {
            options.bar_use = true;
            continue;
        }
        if (token == "--bar-next") {
            ++options.bar_scroll;
            continue;
        }
        if (token == "--bar-prev") {
            --options.bar_scroll;
            continue;
        }
        if (token == "--entry") {
            options.entry = true;
            continue;
        }
        if (kValued.count(token) != 0) {
            if (index + 1 >= argc) throw std::runtime_error(token + " needs a value");
            const std::string value(argv[++index]);
            if (token == "--config") { /* already read by config_path_from_arguments */ }
            else if (token == "--languages") options.languages = value;
            else if (token == "--text-lang") options.settings.text_language = value;
            else if (token == "--voice-lang") options.settings.voice_language = value;
            else if (token == "--scale") {
                if (!byzantine::parse_scale_mode(value, &options.settings.scale_mode)) {
                    throw std::runtime_error("--scale expects fit|integer");
                }
            }
            else if (token == "--volume-master") options.settings.master_volume = parse_volume(token, value);
            else if (token == "--volume-speech") options.settings.speech_volume = parse_volume(token, value);
            else if (token == "--volume-ambient") options.settings.ambient_volume = parse_volume(token, value);
            else if (token == "--volume-effects") options.settings.effects_volume = parse_volume(token, value);
            else if (token == "--cursor-scale") {
                try {
                    options.settings.cursor_scale = std::stoi(value);
                } catch (const std::exception&) {
                    throw std::runtime_error("--cursor-scale expects 1..4");
                }
            }
            else if (token == "--backgrounds") options.backgrounds = value;
            else if (token == "--cursors") options.cursors = value;
            else if (token == "--sounds") options.sounds = value;
            else if (token == "--fonts") options.fonts = value;
            else if (token == "--ccb") options.ccb = value;
            else if (token == "--ccf") options.ccf = value;
            else if (token == "--byz") options.byz = value;
            else if (token == "--messages") options.messages = value;
            else if (token == "--dialog") options.dialog = value;
            else if (token == "--dialog-cache") options.dialog_cache = value;
            else if (token == "--media-isos") options.media_isos = value;
            else if (token == "--run-label") options.run_label = std::stoi(value);
            else if (token == "--bar-select") options.bar_select = std::stoi(value);
            else if (token == "--anim-pass-hz") {
                const auto hz = static_cast<unsigned>(std::stoul(value));
                if (hz == 0 || hz > 2000) {
                    throw std::runtime_error("--anim-pass-hz must be 1..2000");
                }
                options.anim_pass_hz = hz;
            } else if (token == "--conversation-choices") {
                std::size_t at = 0;
                while (at < value.size()) {
                    const auto comma = value.find(',', at);
                    const auto piece = value.substr(
                        at, comma == std::string::npos ? std::string::npos : comma - at);
                    if (piece.empty()) throw std::runtime_error("empty conversation choice");
                    options.conversation_choices.push_back(
                        static_cast<std::size_t>(std::stoul(piece)));
                    if (comma == std::string::npos) break;
                    at = comma + 1;
                }
            }
            else if (token == "--shot") options.shot = value;
            else if (token == "--conversation-shot") options.conversation_shot = value;
            else if (token == "--conversation-object") options.conversation_object = std::stoi(value);
            else if (token == "--conversation-video-shot") options.conversation_video_shot = value;
            else if (token == "--video-shot") options.video_shot = value;
            else if (token == "--transition") {
                const auto code = static_cast<unsigned>(std::stoul(value, nullptr, 0));
                if (code > 0xffff) {
                    throw std::runtime_error("--transition must be a 16-bit code");
                }
                options.travel_transition = static_cast<std::uint16_t>(code);
                options.transition_forced = true;
            }
            else if (token == "--transition-shot") options.transition_shot = value;
            else if (token == "--present-dump") options.present_dump = value;
            else if (token == "--present-dump-max") {
                const auto limit = std::stoul(value);
                if (limit == 0 || limit > 5000) {
                    throw std::runtime_error("--present-dump-max must be 1..5000");
                }
                options.present_dump_max = limit;
            }
            else if (token == "--conversation-cancel") {
                options.conversation_cancel = std::stoi(value);
                if (options.conversation_cancel < 0) {
                    throw std::runtime_error("--conversation-cancel must be >= 0");
                }
            }
            else if (token == "--click") options.click = std::stoi(value);
            else if (token == "--staged-x") options.staged_x = std::stoi(value);
            else if (token == "--menu") {
                const auto mode = std::stoi(value);
                if (mode < 0 || mode > 5) {
                    throw std::runtime_error("--menu must be 0..5");
                }
                options.menu_mode = mode;
            } else if (token == "--menu-shot") options.menu_shot = value;
            else if (token == "--menu-text") options.menu_texts.push_back(value);
            else if (token == "--menu-key") options.menu_keys.push_back(value);
            else if (token == "--menu-hover") {
                const auto comma = value.find(',');
                if (comma == std::string::npos) {
                    throw std::runtime_error("--menu-hover expects <x>,<y>");
                }
                options.menu_hovers.emplace_back(std::stoi(value.substr(0, comma)),
                                                 std::stoi(value.substr(comma + 1)));
            } else if (token == "--credits-frames") {
                options.credits_frames = std::stoi(value);
            } else if (token == "--credits-shot") {
                options.credits_shot = value;
            } else if (token == "--bar-show") {
                const auto button = std::stoi(value);
                if (button < 0 || button > 4) {
                    throw std::runtime_error("--bar-show must be 0..4");
                }
                options.bar_show = button;
            }
            else if (token == "--menu-click-at") {
                const auto comma = value.find(',');
                if (comma == std::string::npos) {
                    throw std::runtime_error("--menu-click-at expects <x>,<y>");
                }
                options.menu_clicks.emplace_back(std::stoi(value.substr(0, comma)),
                                                 std::stoi(value.substr(comma + 1)));
            }
            else if (token == "--panorama-yaw") {
                auto degrees = value;
                std::string scene;
                const auto colon = degrees.find(':');
                if (colon != std::string::npos) {
                    scene = lower_ascii(degrees.substr(0, colon));
                    degrees = degrees.substr(colon + 1);
                }
                const auto yaw = std::stod(degrees);
                if (!(yaw >= -360.0 && yaw <= 360.0)) {
                    throw std::runtime_error("--panorama-yaw must be within -360..360");
                }
                options.panorama_yaw[scene] = yaw;
            }
            else if (token == "--click-at") {
                const auto comma = value.find(',');
                if (comma == std::string::npos) {
                    throw std::runtime_error("--click-at expects <x>,<y>");
                }
                options.click_ats.emplace_back(std::stoi(value.substr(0, comma)),
                                               std::stoi(value.substr(comma + 1)));
            } else if (token == "--cursor-at") {
                const auto comma = value.find(',');
                if (comma == std::string::npos) {
                    throw std::runtime_error("--cursor-at expects <x>,<y>");
                }
                options.cursor_ats.emplace_back(std::stoi(value.substr(0, comma)),
                                                std::stoi(value.substr(comma + 1)));
            } else options.scene[token] = value;
            continue;
        }
        if (token.rfind("--", 0) == 0) throw std::runtime_error("unknown option: " + token);
        positional.push_back(token);
    }
    if (options.list_languages && positional.empty()) {
        options.settings.clamp();
        resolve_language_selection(options);
        return options;
    }
    if (positional.size() != 3 && !(options.entry && positional.size() == 2)) {
        throw std::runtime_error(
            "usage: byzantine-play <scene.cc> <scene.cb> <label> "
            "[--save <btb>] [--engine <dir>] [--backgrounds <dir>] [--cursors <dir>] "
            "[--sounds <dir>] "
            "[--lcx --lci --ci --lcy --cy --co --cf --ca --anims --gameprefs "
            "--inventor --hotitems --cs <file>] [--shot <file.ppm>] [--click <slot>] "
            "[--cursor-at <x>,<y> ...] "
            "[--entry] [--click-at <x>,<y> ...] [--staged-x <0..2495>] "
            "[--panorama-yaw [SCENE:]deg] "
            "[--cd <file> --ccb --ccf --byz --messages <file> --dialog --dialog-cache <dir>] "
            "[--run-label <n>] [--conversation-choices <i,...>] "
            "[--conversation-object <slot>] [--conversation-cancel <n>] "
            "[--conversation-shot <ppm>] "
            "[--conversation-video-shot <ppm>] [--video-shot <ppm>] "
            "[--bar-select <id>] [--bar-use] "
            "[--bar-next] [--bar-prev] [--no-video] "
            "[--hotspots] [--no-hotspots] [--hotspots-all] [--hotspots-hover] "
            "[--no-audio] [--no-audio-tail-guard] "
            "[--present-dump <dir>] [--present-dump-max <n>] "
            "[--print-audio] [--print-hotspots] "
            "[--print-objects] [--print-variables] [--print-records] "
            "[--compose-frames <n>] [--selftest-events] [--anim-pass-hz <hz>] "
            "[--menu <0..5>] [--menu-click-at <x>,<y> ...] [--menu-text <utf8>] "
            "[--menu-key enter|escape|backspace] [--menu-shot <ppm>] "
            "[--config <ini>] [--windowed|--borderless|--fullscreen|--exclusive|--maximized] "
            "[--scale fit|integer] [--vsync|--no-vsync] "
            "[--volume-master|--volume-speech|--volume-ambient|--volume-effects <0..1>] "
            "[--subtitles|--no-subtitles] [--text-lang <code>] [--voice-lang <code>] "
            "[--languages <dir>] [--list-languages]");
    }
    options.cc_path = positional[0];
    options.cb_path = positional[1];
    if (positional.size() == 3) options.label = positional[2];
    options.settings.clamp();
    resolve_language_selection(options);
    return options;
}

// ---------------------------------------------------------------------------
// Scene state
// ---------------------------------------------------------------------------
// The visible scene layer: the last of 0x69 (background) and 0xb9 (node) the
// committed trace contains (see update_layer).
enum class SceneLayer { none, background, panorama };

// Where an explicit reload restarts this scene: the startup label/entry for the
// first scene, the instruction head (entry) after a 0x73 travel, like
// 0x42c8a0's reload. The state it starts from is the scene's arrival
// entry_state, not the global save file (a travel already removed --save).
struct SceneReloadPoint {
    std::uint16_t label = 0;
    bool at_entry = true;
};

struct SceneView {
    std::string name;
    // The scene's own files, kept so a 0x73 switch can derive the target's
    // sibling paths in the same directory (cc_all/VT -> cc_all/MR, ...).
    std::string cc_path;
    std::string cb_path;
    std::vector<byzantine::CcInstruction> script;
    std::vector<byzantine::BackgroundEntry> entries;
    byzantine::SceneState entry_state;  // before the label runs (for replays)
    byzantine::SceneState state;        // after the current trace
    byzantine::SceneTrace trace;
    std::string background_name;
    // The 0x69 background the last committed trace actually loaded. An action
    // that loads none keeps showing it (DAT_00464880 in the original); without
    // this the frame falls back to the scene's first static entry, which is a
    // different picture.
    std::string committed_background;
    std::filesystem::path background_file;
    // DAT_00464880: the panorama renderer state. 0x401256 fixes the expected
    // node size at startup from nodes/sm/sm00.jpg and 0x401d29 refuses a node of
    // another size, so the size is remembered across loads.
    std::optional<byzantine::PanoramaView> panorama;
    std::optional<std::pair<int, int>> panorama_size;
    // Which layer the last committed trace selected (see update_layer).
    SceneLayer layer = SceneLayer::none;
    // The game's two text windows. `examine` is 0x4177c7's 3-line scroller at
    // (5, 0x14a) in font 1 (klio); `message` is 0x417a17's right-aligned block at
    // (0x26e, 0x2d) in font 3. Their content comes from engine/<scene>ce.
    struct TextWindow {
        bool active = false;
        std::string text;
    };
    TextWindow examine_text;
    TextWindow message_text;
    // The DAT_0047e2f0 pool this scene received when it was loaded (the save's
    // pool for the start scene, the exported pool for a travel target). Travel
    // compares its departure pool with this to report what the scene wrote.
    std::vector<std::uint16_t> pool_on_arrival;
    std::uint16_t label = 0;
    // The point an explicit reload restarts this scene from.
    SceneReloadPoint reload_point;
    // The last missing background name logged for this scene, so a missing
    // asset is reported once instead of on every click.
    std::string missing_background_logged;
};

// One sound the label queues, in script order.
struct SoundEvent {
    enum class Kind { start, stop, release, stop_all };
    std::size_t step = 0;
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    std::optional<bool> allowed;
    std::uint64_t cap_ms = 0;
    Kind kind = Kind::start;
    bool blocking = false;  // 0xd1 mode 0 dialogue or a 0xd3 session
    bool loop = false;      // DAT_00486220[id]+0x14
    std::uint8_t opcode = 0;
};

// The script's sound requests in execution order. 0xc4 starts an ambient voice
// without blocking; 0xd1 blocks only in interactive mode 0 (0x428cbd's loop);
// 0xd3 is a timed session; 0xbc/0xcf/0xd0 stop voices. A caller that replays the
// label with a step budget stops exactly at these instructions.
std::vector<SoundEvent> sound_events(const byzantine::SceneTrace& trace) {
    std::vector<SoundEvent> events;
    for (const auto& start : trace.sound_starts) {
        SoundEvent event;
        event.step = start.step;
        event.instruction = start.instruction;
        event.id = start.id;
        event.allowed = start.allowed;
        event.kind = SoundEvent::Kind::start;
        event.opcode = start.opcode;
        event.loop = start.loop;
        event.blocking = start.opcode == 0xd1 && start.mode == 0;
        events.push_back(std::move(event));
    }
    for (const auto& session : trace.sound_sessions) {
        std::uint64_t cap = 0;
        for (const auto& timeout : session.timeouts) {
            if (timeout.defined) {
                cap = timeout.milliseconds;
                break;
            }
        }
        SoundEvent event;
        event.step = session.step;
        event.instruction = session.instruction;
        event.id = session.id;
        event.allowed = session.allowed;
        event.cap_ms = cap;
        event.kind = SoundEvent::Kind::start;
        event.opcode = 0xd3;
        event.blocking = true;
        events.push_back(std::move(event));
    }
    for (const auto& stop : trace.sound_stops) {
        SoundEvent event;
        event.step = stop.step;
        event.instruction = stop.instruction;
        event.id = stop.id;
        event.kind = SoundEvent::Kind::stop;
        events.push_back(std::move(event));
    }
    for (const auto& release : trace.sound_releases) {
        SoundEvent event;
        event.step = release.step;
        event.instruction = release.instruction;
        event.id = release.id;
        event.kind = SoundEvent::Kind::release;
        events.push_back(std::move(event));
    }
    for (const auto& stop : trace.sound_stop_alls) {
        SoundEvent event;
        event.step = stop.step;
        event.instruction = stop.instruction;
        event.kind = SoundEvent::Kind::stop_all;
        events.push_back(std::move(event));
    }
    std::stable_sort(events.begin(), events.end(), [](const SoundEvent& left,
                                                      const SoundEvent& right) {
        if (left.step != right.step) return left.step < right.step;
        return left.instruction < right.instruction;
    });
    return events;
}

std::filesystem::path background_path(const Options& options, const std::string& entry_name) {
    if (entry_name.size() < 2) return {};
    const auto group = lower_ascii(entry_name.substr(0, 2));
    if (const auto file = find_language_child(options, false, "backs/" + group, entry_name)) {
        return *file;
    }
    if (options.backgrounds.empty()) return {};
    auto root = options.backgrounds;
    if (std::filesystem::is_directory(root / "backs")) root /= "backs";
    const auto found = find_child(root / group, entry_name);
    return found ? *found : std::filesystem::path{};
}

// 0x40860e/0x41045b: selected item -> inventor\examine\<first 8 chars, first
// replaced by 'e'>.jpg. The item name is not trimmed at eight characters
// anywhere else, so this stays separate from the raw LCI record.
std::filesystem::path examine_background_path(const Options& options,
                                              const byzantine::SceneState& state,
                                              std::uint16_t item) {
    const auto name = byzantine::inventory_name(state, item);
    if (name.empty()) return {};
    const auto stem = byzantine::examine_image_stem(name);
    if (const auto file = find_language_child(options, false, "inventor/examine", stem)) {
        return *file;
    }
    if (options.backgrounds.empty()) return {};
    auto root = options.backgrounds;
    if (std::filesystem::is_directory(root / "inventor")) root /= "inventor";
    const auto found = find_child(root / "examine", stem);
    return found ? *found : std::filesystem::path{};
}

// The original UI art is TGA (TOP_BAR, INSTRIP and the inventor icons). A TGA
// reader is not part of the port's image layer, so use the same FFmpeg already
// required for the Indeo dialogue clips: decode once to RGB24 and keep the
// RGB565/transparency result in memory. A missing FFmpeg is reported, never
// hidden behind an invented icon.
struct UiImage {
    int width = 0;
    int height = 0;
    int x = 0;  // RLE frame origin (0 for TGA files)
    int y = 0;
    std::vector<std::uint16_t> pixels;
    std::vector<std::uint8_t> alpha;
};

std::optional<UiImage> decode_ui_image(const std::filesystem::path& path,
                                       bool key_magenta, bool verbose) {
    std::ifstream input(path, std::ios::binary);
    if (!input) return std::nullopt;
    std::array<unsigned char, 18> header{};
    input.read(reinterpret_cast<char*>(header.data()), header.size());
    if (input.gcount() != static_cast<std::streamsize>(header.size())) return std::nullopt;
    const int width = header[12] | (header[13] << 8);
    const int height = header[14] | (header[15] << 8);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) return std::nullopt;
    input.close();

    std::string command = "ffmpeg -v error -i \"";
    command += path.string();
    command += "\" -f rawvideo -pix_fmt rgb24 -";
    FILE* pipe = popen(command.c_str(), "r");
    if (pipe == nullptr) return std::nullopt;
    std::vector<unsigned char> rgb(static_cast<std::size_t>(width) * height * 3);
    const auto read = std::fread(rgb.data(), 1, rgb.size(), pipe);
    const auto status = pclose(pipe);
    if (read != rgb.size() || status != 0) {
        if (verbose) {
            std::cout << "ui_asset_decode ERROR file=" << path.string()
                      << " bytes=" << read << '/' << rgb.size() << '\n';
        }
        return std::nullopt;
    }

    UiImage image;
    image.width = width;
    image.height = height;
    image.pixels.resize(static_cast<std::size_t>(width) * height);
    image.alpha.assign(image.pixels.size(), 255);
    for (std::size_t index = 0; index < image.pixels.size(); ++index) {
        const auto r = rgb[index * 3 + 0];
        const auto g = rgb[index * 3 + 1];
        const auto b = rgb[index * 3 + 2];
        image.pixels[index] = static_cast<std::uint16_t>(
            ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
        if (key_magenta && r > 240 && g < 20 && b > 240) image.alpha[index] = 0;
    }
    return image;
}

// 0x408e3f loads setupart/TOP_BAR.TGA and the HOTBAR/DWNBAR RLE sets. The RLE
// frames are RGBA; the UI blitter wants RGB565 plus a coverage byte, and the
// shipped frames leave their transparent pixels at alpha 0.
UiImage ui_image_from_rle(const byzantine::RleFrame& frame) {
    UiImage image;
    image.width = frame.width;
    image.height = frame.height;
    image.x = frame.x;
    image.y = frame.y;
    image.pixels.resize(static_cast<std::size_t>(frame.width) * frame.height);
    image.alpha.assign(image.pixels.size(), 255);
    for (std::size_t index = 0; index < image.pixels.size(); ++index) {
        const auto r = frame.rgba[index * 4 + 0];
        const auto g = frame.rgba[index * 4 + 1];
        const auto b = frame.rgba[index * 4 + 2];
        image.pixels[index] = static_cast<std::uint16_t>(
            ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
        image.alpha[index] = frame.rgba[index * 4 + 3];
    }
    return image;
}

// 0x40186e builds "nodes\\<first two characters>\\<name>.jpg"; the shipped tree
// mixes .JPG/.jpg, so the lookup is case-insensitive like the background one.
std::filesystem::path node_path(const Options& options, const std::string& entry_name) {
    if (entry_name.size() < 2) return {};
    const auto group = lower_ascii(entry_name.substr(0, 2));
    if (const auto file = find_language_child(options, false, "nodes/" + group, entry_name)) {
        return *file;
    }
    if (options.backgrounds.empty()) return {};
    auto root = options.backgrounds;
    if (std::filesystem::is_directory(root / "nodes")) root /= "nodes";
    const auto found = find_child(root / group, entry_name);
    return found ? *found : std::filesystem::path{};
}

// Sound files are either flat (sounds/<name>.wav) or grouped by scene prefix
// (sounds/vt/<name>.wav). Both layouts occur on the CDs.
std::optional<std::filesystem::path> sound_path(const std::filesystem::path& root,
                                                const std::string& entry_name,
                                                const std::string& scene) {
    if (root.empty() || entry_name.empty()) return std::nullopt;
    auto base = root;
    if (std::filesystem::is_directory(base / "sounds")) base /= "sounds";
    std::vector<std::filesystem::path> candidates = {
        base, base / lower_ascii(scene),
        base / lower_ascii(entry_name.substr(0, std::min<std::size_t>(2, entry_name.size())))};
    // copy2hd/sounds holds endGame/Emre2/Pxstart on the shipped CDs and
    // scene.sh flattens it into sounds/; also accept a data dir that keeps the
    // original layout.
    const auto original = root / "copy2hd" / "sounds";
    if (std::filesystem::is_directory(original)) {
        candidates.push_back(original);
        candidates.push_back(original / lower_ascii(scene));
    }
    for (const auto& directory : candidates) {
        const auto found = find_child(directory, entry_name);
        if (found) return found;
    }
    return std::nullopt;
}

// The visible scene layer: the last of 0x69 (background) and 0xb9 (node) the
// committed trace contains. Within one scene a label that has neither keeps the
// current layer, so a node loaded by an earlier label stays visible (the label
// simply does not repaint the surface). A scene ENTRY adopts the layer its own
// trace selects, because the new scene really repaints (a node carried over in
// DAT_00464880 must not hide the new background).
void update_layer(SceneView& view) {
    // A committed 0x69 replaces DAT_00464880 and stays until the next one; the
    // trace only reports the loads this action ran, so remember the picture.
    if (view.trace.background) {
        view.committed_background = view.trace.background->entry.name;
    }
    std::optional<std::size_t> last_load;
    for (const auto& load : view.trace.panorama_loads) last_load = load.instruction;
    if (last_load) {
        view.layer = (!view.trace.background ||
                      view.trace.background->instruction < *last_load)
                         ? SceneLayer::panorama
                         : SceneLayer::background;
        return;
    }
    if (!view.trace.background) return;
    // An explicit 0x69 also leaves an existing panorama. Keeping its layer
    // would show the old node and map flat close-up clicks into source space.
    view.layer = SceneLayer::background;
}

void set_trace(SceneView& view, byzantine::SceneTrace trace) {
    view.trace = std::move(trace);
    update_layer(view);
    byzantine::diagnostics::write("state_adopted", [&](auto& out) {
        out << "scene=" << view.name << " steps=" << view.trace.steps
            << " stopped_at=" << view.trace.stopped_at
            << " blocker=" << byzantine::trace_blocker(view.trace) << " active_slots=";
        for (const auto& hotspot : view.trace.active_hotspots) out << hotspot.slot << ',';
        out << " records=";
        for (const auto& entry : view.state.records)
            out << entry.first << ':' << entry.second.x << ':' << entry.second.y
                << ':' << unsigned(entry.second.flags) << ',';
        out << " held=";
        if (view.state.current_dialogue_record) out << *view.state.current_dialogue_record;
        else out << "none";
        out << " examined=";
        if (view.state.current_use_item) out << *view.state.current_use_item;
        else out << "none";
        out << " locals=";
        for (const auto& entry : view.state.variables) out << entry.first << ':' << entry.second << ',';
        out << " inventory=";
        for (const auto& entry : view.state.dialogue_records)
            out << entry.first << ':' << unsigned(entry.second.state) << ',';
        out << " objects=";
        for (const auto& entry : view.state.objects) {
            const auto& object = entry.second;
            out << entry.first << ':' << object.x << ':' << object.y << ':' << object.width
                << ':' << object.height << ':' << unsigned(object.active) << ':' << object.label << ',';
        }
    });
}

bool panorama_layer(const SceneView& view) { return view.layer == SceneLayer::panorama; }

// 0x40241c's panorama branch needs 0x423e03: while a 0xb9 node is the current
// layer a record's +0x00/+0x02 are panorama SOURCE coordinates (the shipped
// scripts place characters exactly at their 0x72 object rects), so the record
// rect has to be projected onto the viewport before it is drawn. Only scenes
// showing a background/close-up use the record coordinates as screen pixels.
const byzantine::SceneProjector* scene_projector(const SceneView& view) {
    return panorama_layer(view) && view.panorama.has_value() ? &*view.panorama : nullptr;
}

// 0x40c02c: the point that 0x401ca6 produced is compared with every active
// DAT_00464300 object rect, ids ascending, first hit wins. For a panorama the
// point is the SOURCE coordinate 0x4240ae's inverse map returns with the 0x14
// playfield bias added back; otherwise it is the screen point. Both the click
// path and 0x40c329's hover pointer use this one test.
struct SceneHit {
    std::optional<int> slot;
    int test_x = 0;
    int test_y = 0;
    bool panorama_mapped = false;
};

SceneHit hotspot_at(const SceneView& view, int screen_x, int screen_y) {
    SceneHit hit;
    hit.test_x = screen_x;
    hit.test_y = screen_y + kPlayfieldTop;
    if (panorama_layer(view)) {
        // 0x401ca6 subtracts the playfield bias, 0x4240ae maps the point and
        // 0x401ca6 adds the bias back to the result.
        const auto point = view.panorama->canvas_point(screen_x, screen_y);
        if (point.mapped) {
            hit.test_x = point.source_x;
            hit.test_y = point.source_y + kPlayfieldTop;
            hit.panorama_mapped = true;
        }
    }
    const byzantine::SceneHotspot* best = nullptr;
    for (const auto& hotspot : view.trace.active_hotspots) {
        // 0x40c02c uses inclusive bounds on both edges.
        if (hit.test_x < hotspot.x || hit.test_x > hotspot.x + hotspot.width) continue;
        if (hit.test_y < hotspot.y || hit.test_y > hotspot.y + hotspot.height) continue;
        if (best == nullptr || hotspot.slot < best->slot) best = &hotspot;
    }
    if (best != nullptr) hit.slot = best->slot;
    return hit;
}

// The object 0x40c329 reads for the point under the mouse. 0x40c02c iterates
// DAT_00464300, so the hover state and the pointer art come from the same
// object record, not from the hotspot mirror alone.
const byzantine::SceneObjectState* hovered_object(const SceneView& view,
                                                  const SceneHit& hit) {
    if (!hit.slot) return nullptr;
    const auto found = view.state.objects.find(
        static_cast<std::uint16_t>(*hit.slot));
    if (found == view.state.objects.end()) return nullptr;
    return &found->second;
}

std::string pick_background(const SceneView& view) {
    return byzantine::resolve_frame_background(view.trace, view.committed_background,
                                               view.script, view.entries);
}

void refresh_background(const Options& options, SceneView& view) {
    if (view.trace.examine_image) {
        const auto item = *view.trace.examine_image;
        const auto name = byzantine::inventory_name(view.state, item);
        const auto stem = byzantine::examine_image_stem(name);
        view.background_name = "examine/" + stem;
        view.background_file = examine_background_path(options, view.state, item);
        if (view.state.current_use_item && *view.state.current_use_item != item) {
            // The script's 0x69 read DAT_0045d1cc and the live examine pointer
            // names another item: that is the drift the close-up switch fixes.
            std::cout << "examine_state_mismatch image_item=" << item
                      << " current_use_item=" << *view.state.current_use_item
                      << " (the image follows the script request)\n";
        }
        if (view.background_file.empty() &&
            view.missing_background_logged != view.background_name) {
            std::cout << "examine_image_missing scene=" << view.name
                      << " item=" << item
                      << " name=" << name << " stem=" << stem
                      << " root=" << options.backgrounds
                      << " (extract inventor/examine with port/scene.sh; the "
                         "close-up stays black until the asset exists)\n";
            view.missing_background_logged = view.background_name;
        }
        if (!view.background_file.empty()) view.missing_background_logged.clear();
        return;
    }
    view.background_name = pick_background(view);
    view.background_file = background_path(options, view.background_name);
    if (view.trace.background) {
        // 0x431ec2's blend code for this 0x69 (100 = the picture appears at
        // once). Printed once per applied load, so a fade can be traced to the
        // script operand that asked for it.
        std::cout << "background_load instruction=" << view.trace.background->instruction
                  << " id=" << view.trace.background->id
                  << " transition=0x" << std::hex << view.trace.background->transition
                  << std::dec << '\n';
    }
    if (!panorama_layer(view) && !view.background_name.empty() &&
        view.background_file.empty()) {
        if (view.missing_background_logged != view.background_name) {
            std::cout << "background_missing scene=" << view.name
                      << " name=" << view.background_name
                      << " root=" << options.backgrounds
                      << " (extract/backfill backs/ with port/scene.sh; the scene "
                         "stays black until the asset exists)\n";
            view.missing_background_logged = view.background_name;
        }
    } else {
        view.missing_background_logged.clear();
    }
}

SceneView open_scene(const std::map<std::string, std::string>& scene_options,
                     const std::string& cc_path, const std::string& cb_path,
                     std::uint16_t label) {
    SceneView view;
    view.cc_path = cc_path;
    view.cb_path = cb_path;
    view.script = byzantine::parse_cc(read_file(cc_path));
    view.entries = byzantine::parse_cb(read_file(cb_path));
    view.name = std::filesystem::path(cc_path).stem().string();
    view.label = label;
    auto setup = byzantine::load_scene_setup(cc_path, scene_options);
    // 0x40fd7b runs at every scene load; a save then replaces the whole record
    // table (0x425ab4 frees it), so only a scene without --save is preloaded.
    if (scene_options.count("--save") == 0) {
        const auto preload = byzantine::preload_scene_records(setup.state);
        if (preload.created != 0 || preload.failed != 0) {
            std::cout << "record_preload scene=" << view.name
                      << " created=" << preload.created
                      << " failed=" << preload.failed << '\n';
        }
    }
    // 0x41535a -> 0x415a4e calls FUN_0044d060(0xbeef) (srand) on the normal
    // startup path. The save does not carry DAT_00462ef8, so the exact stream
    // of a save made during an original session cannot be reconstructed; the
    // original startup seed is still the correct initialization, not a guess.
    setup.state.random_state = 0xbeefu;
    std::cout << "random_seed=0xbeef source=0x415a4e "
                 "note=save_does_not_carry_DAT_00462ef8\n";
    view.entry_state = setup.state;
    view.state = std::move(setup.state);
    std::cout << "scene=" << view.name << " label=" << label << ' ' << setup.summary << '\n';
    return view;
}

// A 0x73 target is loaded from the same directories as the current scene: every
// per-scene table the player was given (cc/cb/co/ci/cy/cf/cs/ca) has the scene
// name as its file name. The SHOW.LCC list is lowercase and the extracted files
// are uppercase, so the lookup is case-insensitive (like the backgrounds).
std::optional<std::filesystem::path> sibling_scene_file(
    const std::filesystem::path& current, const std::string& target) {
    std::error_code error;
    const auto directory = current.parent_path();
    if (directory.empty() || !std::filesystem::is_directory(directory, error)) {
        return std::nullopt;
    }
    const auto wanted = lower_ascii(target);
    for (const auto& entry : std::filesystem::directory_iterator(directory, error)) {
        if (entry.is_regular_file() && lower_ascii(entry.path().stem().string()) == wanted) {
            return entry.path();
        }
    }
    return std::nullopt;
}

std::uint64_t mix_word(std::uint64_t hash, std::uint32_t value) {
    for (int byte = 0; byte < 4; ++byte) {
        hash ^= (value >> (byte * 8)) & 0xffu;
        hash *= 1099511628211ull;
    }
    return hash;
}

// FNV-1a over the current scene's 300-local projection. Comparing the value at
// a departure with the value after a return is the direct check that the
// carried DAT_0047e2f0 pool survived the round trip.
std::uint64_t locals_digest(const byzantine::SceneState& state) {
    std::uint64_t hash = 1469598103934665603ull;
    for (const auto& [key, value] : state.variables) hash = mix_word(hash, (key << 16) | value);
    return hash;
}

std::uint64_t pool_digest(const std::vector<std::uint16_t>& pool) {
    std::uint64_t hash = 1469598103934665603ull;
    for (const auto value : pool) hash = mix_word(hash, value);
    return hash;
}

// Ids the record pass actually painted, for the scene-render comparison.
std::string drawn_ids(const byzantine::SceneCompose& composed) {
    std::string text;
    for (const auto& record : composed.records) {
        if (!record.blit.drawn) continue;
        if (!text.empty()) text += ',';
        text += std::to_string(record.id);
    }
    return text;
}

// Where a trace starts: a hotspot action label, or the scene's first
// instruction (0x42c8a0's post-switch entry). Both share one runner so the
// audio-gated replay path is identical for clicks and scene loads.
struct EntryPoint {
    std::uint16_t label = 0;
    bool at_entry = false;
    // A conversation's 0x70 result resumes inside the same script (the original
    // replaces pc), so this is an instruction index, not a label.
    std::optional<std::size_t> at_instruction;
};

byzantine::SceneTrace trace_point(SceneView& view, EntryPoint point,
                                  const byzantine::SceneState& entry,
                                  std::size_t step_budget) {
    byzantine::diagnostics::write("vm_context", [&](auto& out) {
        out << "scene=" << view.name << " label=" << point.label
            << " entry=" << point.at_entry << " budget=" << step_budget;
        if (point.at_instruction) out << " instruction=" << *point.at_instruction;
    });
    view.state = entry;
    if (point.at_entry) {
        return byzantine::trace_scene_entry(view.script, view.entries, view.state,
                                            step_budget);
    }
    if (point.at_instruction) {
        return byzantine::trace_scene_from_instruction(view.script, view.entries,
                                                       view.state, *point.at_instruction,
                                                       step_budget);
    }
    return byzantine::trace_scene_label(view.script, view.entries, view.state, point.label,
                                        step_budget);
}

// ---------------------------------------------------------------------------
// Panorama view (0xb9/0xba/0xcb -> DAT_00464880)
// ---------------------------------------------------------------------------
// The renderer lives outside the VM: the script only stages a pixel position
// (0xba -> DAT_0045c064) or asks for a rotation toward a source column (0xcb ->
// 0x402285). A load is 0x40186e: resolve nodes\<2ch>\<name>.jpg, decode it
// through 0x40f135 against the size DAT_0046487c/0x4654d8 that 0x401256 fixed
// from nodes/sm/sm00.jpg at startup, rebuild the seam rows (0x42382b) and then
// consume the staged position. A failed load leaves the current node in place.
void apply_panorama_events(SceneView& view, Options& options, bool verbose) {
    for (const auto& load : view.trace.panorama_loads) {
        const auto file = node_path(options, load.name);
        if (file.empty()) {
            if (verbose) {
                std::cout << "panorama instruction=" << load.instruction
                          << " name=" << load.name << " missing under "
                          << options.backgrounds.string() << '\n';
            }
            continue;
        }
        const auto key = "nodes/" + lower_ascii(load.name.substr(0, 2)) + "/" +
                         file.filename().string();
        bool loaded = false;
        if (!view.panorama || view.panorama->path() != key) {
            byzantine::JpegImage image;
            try {
                image = byzantine::decode_jpeg_file(file.string());
            } catch (const std::exception& error) {
                std::cout << "panorama " << file.string() << ": " << error.what() << '\n';
                continue;
            }
            const std::pair<int, int> size{image.width, image.height};
            if (view.panorama_size && *view.panorama_size != size) {
                // 0x40f135 fails unless the decoded size equals *param_2/3.
                std::cout << "panorama " << key << " size=" << image.width << 'x'
                          << image.height << " differs from the renderer's "
                          << view.panorama_size->first << 'x'
                          << view.panorama_size->second
                          << " (0x401256/0x401d29 reject it)\n";
                continue;
            }
            if (!view.panorama_size) view.panorama_size = size;
            view.panorama.emplace(key, image.width, image.height,
                                  byzantine::rgb_to_rgb565(image), kSceneWidth,
                                  kSceneHeight);
            std::cout << "panorama_view load=" << key << " viewport="
                      << view.panorama->viewport_width() << 'x'
                      << view.panorama->viewport_height() << "\n";
            loaded = true;
        }
        const bool alternate_mode = false;  // DAT_00464878 == 0 outside a transition
        if (load.horizontal) {
            const auto setup = view.panorama->apply_staged_x(*load.horizontal,
                                                             alternate_mode);
            if (verbose) {
                std::cout << "panorama_load instruction=" << load.instruction
                          << " name=" << load.name << " record=" << load.record
                          << " staged_x=" << *load.horizontal
                          << " yaw_degrees=" << setup.horizontal_degrees
                          << " vertical_degrees=" << setup.vertical_degrees
                          << " yaw=" << view.panorama->yaw_degrees()
                          << " transition=0x" << std::hex << load.transition << std::dec
                          << (loaded ? " loaded" : " reused") << '\n';
            }
        } else if (verbose) {
            std::cout << "panorama_load instruction=" << load.instruction
                      << " name=" << load.name << " record=" << load.record
                      << " staged_x=none yaw=" << view.panorama->yaw_degrees()
                      << " transition=0x" << std::hex << load.transition << std::dec
                      << (loaded ? " loaded" : " reused") << '\n';
        }
        if (load.vertical != 0) {
            // 0x432295 uses operand 2 only to move the message text (it scales
            // DAT_0047a8dc), not the camera; it is reported, not applied.
            std::cout << "panorama_text_offset=" << load.vertical << '\n';
        }
    }
    for (const auto& rotation : view.trace.panorama_rotations) {
        if (!view.panorama) {
            if (verbose) {
                std::cout << "panorama_rotation instruction=" << rotation.instruction
                          << " column=" << rotation.id << " skipped: no node loaded\n";
            }
            continue;
        }
        // 0x402285(column, step): rotate toward `column` in `step`-pixel slices.
        // The direction comes from the source column the viewport CENTRE
        // (0x140,0xf0) samples, and a distance past half the source wraps.
        const auto centre = view.panorama->canvas_point(kSceneWidth / 2, 240);
        if (!centre.mapped) {
            std::cout << "panorama_rotation instruction=" << rotation.instruction
                      << " skipped: centre outside the viewport\n";
            continue;
        }
        const int width = view.panorama->source_width();
        const int step = rotation.step < 1 ? 1 : rotation.step;
        int distance = std::abs(rotation.id - centre.source_x);
        bool forward = centre.source_x < static_cast<int>(rotation.id);
        if (width / 2 < distance) {
            forward = !forward;
            distance = width - distance;
        }
        int slices = 0;
        for (int offset = 0; offset < distance; offset += step) {
            const int remaining = std::min(step, distance - offset);
            const double degrees =
                static_cast<double>(remaining) / width * 360.0;
            view.panorama->advance(forward ? degrees : -degrees, 0, 1.0);
            ++slices;
        }
        if (verbose) {
            std::cout << "panorama_rotation instruction=" << rotation.instruction
                      << " column=" << rotation.id << " step=" << step
                      << " slices=" << slices << " distance=" << distance
                      << " yaw=" << view.panorama->yaw_degrees() << '\n';
        }
    }
    if (options.staged_x) {
        // Explicit port input (the CLI counterpart of a 0xba staging), so a
        // headless run can be asked for a known view. It is not game state.
        if (!view.panorama) {
            std::cout << "--staged-x ignored: no panorama is loaded\n";
            return;
        }
        const auto setup =
            view.panorama->apply_staged_x(*options.staged_x, false);
        if (verbose) {
            std::cout << "staged_x=" << *options.staged_x
                      << " yaw_degrees=" << setup.horizontal_degrees
                      << " yaw=" << view.panorama->yaw_degrees() << '\n';
        }
        options.staged_x.reset();
    }
    if (!options.panorama_yaw.empty()) {
        // Explicit port input: after the trace's own 0xb9/0xcb camera work,
        // turn to an absolute yaw so a headless run can look at a known part of
        // the node. Like --staged-x this is a diagnostic view, not game state.
        if (!view.panorama) {
            if (verbose) std::cout << "--panorama-yaw ignored: no panorama is loaded\n";
            return;
        }
        const auto scene = lower_ascii(view.name);
        auto wanted = options.panorama_yaw.find(scene);
        if (wanted == options.panorama_yaw.end()) {
            wanted = options.panorama_yaw.find(std::string());
        }
        if (wanted == options.panorama_yaw.end()) return;
        const double delta = wanted->second - view.panorama->yaw_degrees();
        view.panorama->advance(delta, 0, 1.0);
        if (verbose) {
            std::cout << "panorama_yaw=" << wanted->second
                      << " yaw=" << view.panorama->yaw_degrees() << '\n';
        }
    }
}

// ---------------------------------------------------------------------------
// Composition: background JPEG + records -> RGB565 surface -> SDL texture.
// ---------------------------------------------------------------------------

// What is on screen right now. This exists because the hotspot-outline
// diagnostic (`H`, `--hotspots`, off by default) is a *scene* tool: the original engine has
// no outlines at all and draws everything into one surface, while the port has
// several presenters (scene, clip media, conversation, menu). Every presenter
// ends in Display::present_image, which applies the one rule below, so no path
// has to remember to pass overlay=false (a clip's audio drain used to forget).
enum class ScreenContent { scene, conversation, clip, menu, transition };

// The detail trace's name for what owns the screen (see ScreenContentScope).
const char* screen_content_name(ScreenContent content) {
    switch (content) {
        case ScreenContent::conversation: return "conversation";
        case ScreenContent::clip: return "clip";
        case ScreenContent::menu: return "menu";
        case ScreenContent::transition: return "transition";
        case ScreenContent::scene: break;
    }
    return "scene";
}

struct Display {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    byzantine::Rgb16Surface surface;
    std::string background_key;
    std::vector<std::uint16_t> background_pixels;
    // Font slots 1 (examine, klio green 0x1b<<5) and 3 (message, klio with
    // 0x406cfe's (0xb<<11)|(0xe<<5)|2) share the klio atlas.
    std::optional<byzantine::SfnFont> examine_font;
    std::optional<byzantine::SfnFont> message_font;
    // Font slot 0 (fonts/DIALOG.SFN + GR_DLG/BK_DLG, fill 0xe2c0). 0x436b01
    // draws the answer list with slot 0.
    std::optional<byzantine::SfnFont> dialog_font;
    // Font slot 4 (fonts/CAPTIONS.SFN + CAPTIONS.TGA, fill 0x03eb). 0x424981
    // draws the conversation subtitle with this slot at (55, 420).
    std::optional<byzantine::SfnFont> caption_font;
    // Font slot 2 (fonts/setup.sfn + setup.tga): the menu list text.
    std::optional<byzantine::SfnFont> setup_font;
    // The original scene surface is 640x480: the playfield starts at row 0x14
    // and the inventory strip occupies rows 0x1a4..0x1df. `surface` stays the
    // 640x400 playfield and `screen` is the composed 640x480 frame.
    byzantine::Rgb16Surface screen;
    const Options* ui_options = nullptr;
    // ScreenContentScope keeps this current (see play_decoded_clip/MenuUi). The
    // hotspot outline is drawn only for scene/conversation content.
    ScreenContent content = ScreenContent::scene;
    bool overlay_allowed() const {
        if (ui_options == nullptr || !ui_options->overlay) return false;
        return content == ScreenContent::scene || content == ScreenContent::conversation;
    }
    bool overlay_all() const { return ui_options != nullptr && ui_options->hotspots_all; }
    // Records who owns the screen in the detail trace, then switches to it.
    // ScreenContentScope wraps this for media/menu presenters; a transition
    // calls it directly because that scope type is declared after Display.
    void enter_content(ScreenContent next) {
        const auto previous = content;
        content = next;
        byzantine::diagnostics::write("screen_content", [&](auto& out) {
            out << "now=" << screen_content_name(next)
                << " previous=" << screen_content_name(previous);
        });
    }
    void restore_content(ScreenContent previous) {
        const auto now = content;
        content = previous;
        byzantine::diagnostics::write("screen_content", [&](auto& out) {
            out << "now=" << screen_content_name(previous)
                << " previous=" << screen_content_name(now);
        });
    }
    // The active hotspot under the pointer, or -1. Only the script-enabled set
    // counts: an object the script has not enabled is not clickable.
    int hovered_overlay_slot(const SceneView& view, int x, int y) const {
        if (y < kScreenPlayfieldTop || y >= kScreenBandTop) return -1;
        const auto hit = hotspot_at(view, x, y - kScreenPlayfieldTop);
        return hit.slot ? *hit.slot : -1;
    }
    // Updates the outlined slot; true when it changed (the caller re-presents).
    bool set_hover_overlay(const SceneView& view, int x, int y) {
        if (!overlay_allowed() || overlay_all()) return false;
        const int slot = hovered_overlay_slot(view, x, y);
        if (slot == hover_overlay_slot) return false;
        hover_overlay_slot = slot;
        return true;
    }
    int hover_overlay_slot = -1;
    std::optional<UiImage> bottom_bar;
    // 0x408e3f/0x408f32: setupart/TOP_BAR.TGA plus the HOTBAR/DWNBAR RLE
    // states. The bar is a roll-over UI: it appears when the pointer enters the
    // 0x14-row band and is cleared when it leaves (0x40c329/0x408fe7).
    std::optional<UiImage> top_bar;
    std::vector<UiImage> hotbar_frames;   // HOTBAR.RLE frames 1..4 are the buttons
    std::vector<UiImage> dwnbar_frames;   // DWNBAR.RLE frames 1..4 (pressed)
    std::vector<UiImage> marker_frames;   // on_off.rle frames 0..12
    // 0x41d7ff: ROLL.RLE (0x46c620, 15 frames) is the hover art for controls
    // 0..14; yellow.rle (0x46c7f0, 4 frames) highlights the save-list/KEŞİF/
    // hint tabs.
    std::vector<UiImage> roll_frames;
    std::vector<UiImage> yellow_frames;
    // on_off.rle frame i+6 is the "off" overlay of preference row i (0x41f94e is
    // called when the stored value is 0, see FUN_0041fe46 case 0).
    bool top_bar_visible = false;
    int top_bar_hover = 0;   // 0x408fe7's index 1..4, 0 when none
    int top_bar_press = 0;   // index 1..4 whose DWNBAR frame is drawn
    std::map<std::string, UiImage> item_icons;
    // 0x40dae5's cursor art and 0x407254's item art are decoded once and kept.
    // cursor_key_ is the id of the pointer currently installed on the window.
    std::string cursor_key_;
    std::map<std::string, SDL_Cursor*> cursors_;
    // 0x40ebe1's animation: the frame images of each multi-frame pointer and the
    // pointer currently cycling (if any).
    std::map<std::string, std::map<std::string, UiImage>> cursor_frame_images;
    std::map<std::string, std::vector<const UiImage*>> cursor_frame_groups;
    struct CursorAnimation {
        std::string key;                         // identity of the pointer
        std::vector<const UiImage*> frames;
        int hot_x = 0;
        int hot_y = 0;
        std::size_t frame = 0;
        std::chrono::steady_clock::time_point since{};
    };
    std::optional<CursorAnimation> cursor_animation;
    SDL_Cursor* default_cursor_ = nullptr;
    // Host gamepad bridge. It injects the same SDL mouse/key events the scene
    // and conversation loops already understand, so game logic stays unchanged.
    SDL_GameController* gamepad = nullptr;
    std::array<Uint8, SDL_CONTROLLER_BUTTON_MAX> gamepad_buttons{};
    std::uint64_t gamepad_axis_at = 0;
    std::set<std::string> missing_ui;
    // Host window state. The gamepref[1] "fullscreen" flag used by
    // VideoPlacement is the original 2x movie mode, not this.
    byzantine::WindowMode window_mode = byzantine::WindowMode::windowed;
    int windowed_width = kScreenWidth * 2;
    int windowed_height = kScreenHeight * 2;
    int windowed_x = -1;
    int windowed_y = -1;
    int logical_width = kScreenWidth;
    int logical_height = kScreenHeight;
    // Records the last compose drew and stepped through 0x43b216's normal pass.
    std::vector<std::uint16_t> drawn_record_ids;
    // Host diagnostics for a reported "another picture flashes for an instant":
    // every SDL_RenderPresent checks what it actually shows (tag, scene layer,
    // dialogue/bar state, a sampled checksum). A detail-trace line is written
    // when the picture changes, while a capture window is open, or as a slow
    // heartbeat; the window is armed by a scene load and by a CF clip's end, so
    // the frames that can flash are named without logging a 60 Hz animation.
    // `present_detail` is set by the caller just before presenting (e.g. a
    // clip's frame number).
    std::uint64_t present_index = 0;
    std::uint64_t present_logged = 0;
    std::uint64_t present_hash = 0;
    bool present_hash_known = false;
    std::string present_state_key;
    // The outgoing 640x400 playfield of the next transition: present_image keeps
    // it from every full-screen frame. 0x40186e's blends cover (0,0x14)-(0x280,
    // 0x1a4) only, so the strip and the top bar are never part of the mix.
    byzantine::Rgb16Surface last_playfield;
    bool last_playfield_valid = false;
    std::string present_detail;
    std::chrono::steady_clock::time_point picture_since{};
    std::chrono::steady_clock::time_point present_logged_at{};
    std::chrono::steady_clock::time_point present_capture_until{};
    std::size_t present_dumps = 0;
    bool present_summary_printed = false;
    std::set<std::uint64_t> dumped_hashes;

    ~Display() {
        if (gamepad != nullptr) {
            SDL_GameControllerClose(gamepad);
            gamepad = nullptr;
        }
        for (auto& entry : cursors_) {
            if (entry.second != nullptr) SDL_FreeCursor(entry.second);
        }
        cursors_.clear();
        if (default_cursor_ != nullptr) SDL_FreeCursor(default_cursor_);
        log_present_summary();
    }

    // End-of-run marker for the per-present trace lines (detail file). The
    // paths that print their own final state call it first, so `scene=…
    // slots=…` stays the last stdout line for the verification scripts.
    void log_present_summary() {
        if (present_summary_printed || present_index == 0) return;
        present_summary_printed = true;
        std::cout << "present_summary frames=" << present_index
                  << " logged=" << present_logged
                  << " dumps=" << present_dumps << '\n';
    }

    void close_gamepad() {
        if (gamepad == nullptr) return;
        SDL_GameControllerClose(gamepad);
        gamepad = nullptr;
        std::cout << "gamepad close\n";
    }

    void open_gamepad() {
        if (gamepad != nullptr) return;
        if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
            std::cout << "gamepad init_failed error=" << SDL_GetError() << '\n';
            return;
        }
        for (int index = 0; index < SDL_NumJoysticks(); ++index) {
            if (!SDL_IsGameController(index)) continue;
            gamepad = SDL_GameControllerOpen(index);
            if (gamepad == nullptr) continue;
            SDL_GameControllerEventState(SDL_ENABLE);
            gamepad_buttons.fill(0);
            std::cout << "gamepad open name=" << SDL_GameControllerName(gamepad)
                      << " index=" << index << '\n';
            return;
        }
        std::cout << "gamepad none (no SDL game controller detected)\n";
    }

    void set_gamepad_enabled(bool enabled) {
        if (enabled) open_gamepad();
        else close_gamepad();
    }

    static void push_key_event(Uint32 type, SDL_Keycode key) {
        SDL_Event event{};
        event.type = type;
        event.key.keysym.sym = key;
        event.key.state = type == SDL_KEYDOWN ? SDL_PRESSED : SDL_RELEASED;
        SDL_PushEvent(&event);
    }

    void push_controller_click() {
        int x = 0;
        int y = 0;
        SDL_GetMouseState(&x, &y);
        SDL_Event down{};
        down.type = SDL_MOUSEBUTTONDOWN;
        down.button.button = SDL_BUTTON_LEFT;
        down.button.state = SDL_PRESSED;
        down.button.x = x;
        down.button.y = y;
        SDL_PushEvent(&down);
        SDL_Event up = down;
        up.type = SDL_MOUSEBUTTONUP;
        up.button.state = SDL_RELEASED;
        SDL_PushEvent(&up);
    }

    // Axis movement is applied on the host's 16 ms frame. The mouse warp
    // generates the normal motion event, so hover/click mapping is unchanged.
    void poll_gamepad() {
        if (gamepad == nullptr) return;
        SDL_GameControllerUpdate();
        const auto axis = [&](SDL_GameControllerAxis which) {
            const int value = SDL_GameControllerGetAxis(gamepad, which);
            return std::abs(value) < 10000 ? 0 : value / 32767;
        };
        const double dx = axis(SDL_CONTROLLER_AXIS_LEFTX);
        const double dy = axis(SDL_CONTROLLER_AXIS_LEFTY);
        if (dx != 0.0 || dy != 0.0) {
            int x = 0;
            int y = 0;
            SDL_GetMouseState(&x, &y);
            SDL_WarpMouseInWindow(window, x + static_cast<int>(dx * 12.0),
                                  y + static_cast<int>(dy * 12.0));
        }
        const auto pressed = [&](SDL_GameControllerButton button) {
            return SDL_GameControllerGetButton(gamepad, button) != 0;
        };
        const auto edge = [&](SDL_GameControllerButton button) {
            const bool now = pressed(button);
            const bool before = gamepad_buttons[button] != 0;
            gamepad_buttons[button] = now ? 1 : 0;
            return now && !before;
        };
        const auto push_bound = [&](const std::string& name, SDL_Keycode fallback) {
            SDL_Keycode key = SDL_GetKeyFromName(name.c_str());
            if (key == SDLK_UNKNOWN) key = fallback;
            push_key_event(SDL_KEYDOWN, key);
            push_key_event(SDL_KEYUP, key);
        };
        if (edge(SDL_CONTROLLER_BUTTON_A)) push_controller_click();
        if (edge(SDL_CONTROLLER_BUTTON_B)) push_bound("Escape", SDLK_ESCAPE);
        if (edge(SDL_CONTROLLER_BUTTON_START)) {
            push_bound(ui_options != nullptr ? ui_options->settings.key_settings : "F10",
                       SDLK_F10);
        }
        if (edge(SDL_CONTROLLER_BUTTON_X)) {
            push_bound(ui_options != nullptr ? ui_options->settings.key_backlog : "F3",
                       SDLK_F3);
        }
        if (edge(SDL_CONTROLLER_BUTTON_Y)) {
            push_bound(ui_options != nullptr ? ui_options->settings.key_skip : "F4",
                       SDLK_F4);
        }
    }

    void apply_scale_mode(byzantine::ScaleMode mode) {
        if (renderer == nullptr) return;
        SDL_RenderSetIntegerScale(renderer,
                                  mode == byzantine::ScaleMode::integer ? SDL_TRUE
                                                                        : SDL_FALSE);
        std::cout << "window_scale mode=" << byzantine::scale_mode_name(mode)
                  << " filter=nearest\n";
    }

    void apply_vsync(bool enabled) {
        if (renderer == nullptr) return;
        if (SDL_RenderSetVSync(renderer, enabled ? 1 : 0) != 0) {
            std::cout << "window_vsync unsupported requested=" << (enabled ? 1 : 0)
                      << " error=" << SDL_GetError() << '\n';
            return;
        }
        std::cout << "window_vsync enabled=" << (enabled ? 1 : 0) << '\n';
    }

    // Saves the windowed geometry before entering a fullscreen mode. The
    // caller persists Settings after the mode change.
    void remember_windowed_geometry() {
        if (window == nullptr || window_mode != byzantine::WindowMode::windowed) return;
        SDL_GetWindowPosition(window, &windowed_x, &windowed_y);
        SDL_GetWindowSize(window, &windowed_width, &windowed_height);
    }

    void store_window_settings(byzantine::Settings& settings) const {
        settings.window_mode = window_mode;
        if (window_mode == byzantine::WindowMode::windowed && window != nullptr) {
            int width = 0;
            int height = 0;
            int x = 0;
            int y = 0;
            SDL_GetWindowSize(window, &width, &height);
            SDL_GetWindowPosition(window, &x, &y);
            settings.window_width = width;
            settings.window_height = height;
            settings.window_x = x;
            settings.window_y = y;
            return;
        }
        settings.window_width = windowed_width;
        settings.window_height = windowed_height;
        settings.window_x = windowed_x;
        settings.window_y = windowed_y;
    }

    // Re-copies the current texture after a window/renderer change so a mode
    // switch never flashes an unpainted backbuffer.
    void repaint_window() {
        if (renderer == nullptr || texture == nullptr) return;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    void apply_window_mode(byzantine::WindowMode mode) {
        if (window == nullptr) return;
        int fullscreen_result = 0;
        if (mode == byzantine::WindowMode::windowed) {
            remember_windowed_geometry();
            SDL_SetWindowFullscreen(window, 0);
            SDL_SetWindowResizable(window, SDL_TRUE);
            SDL_SetWindowSize(window, windowed_width, windowed_height);
            if (windowed_x >= 0 && windowed_y >= 0) {
                SDL_SetWindowPosition(window, windowed_x, windowed_y);
            }
        } else if (mode == byzantine::WindowMode::borderless) {
            remember_windowed_geometry();
            fullscreen_result = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else if (mode == byzantine::WindowMode::exclusive) {
            remember_windowed_geometry();
            fullscreen_result = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        } else if (mode == byzantine::WindowMode::maximized) {
            remember_windowed_geometry();
            SDL_SetWindowFullscreen(window, 0);
            SDL_SetWindowResizable(window, SDL_TRUE);
            SDL_SetWindowSize(window, windowed_width, windowed_height);
            SDL_MaximizeWindow(window);
        }
        if (fullscreen_result != 0) {
            std::cout << "window_mode_failed requested=" << byzantine::window_mode_name(mode)
                      << " error=" << SDL_GetError() << "\n";
            SDL_SetWindowFullscreen(window, 0);
            mode = byzantine::WindowMode::windowed;
        }
        window_mode = mode;
        int width = 0;
        int height = 0;
        SDL_GetWindowSize(window, &width, &height);
        std::cout << "window_mode mode=" << byzantine::window_mode_name(mode)
                  << " size=" << width << 'x' << height
                  << " fullscreen=" << (mode == byzantine::WindowMode::windowed ? 0 : 1)
                  << '\n';
        repaint_window();
    }

    void init(const Options& options) {
        ui_options = &options;
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error(std::string("SDL video init failed: ") + SDL_GetError());
        }
        // 640x480 at 2x: the original screen size, including the inventory
        // strip. A saved window size/position wins; fullscreen modes still use
        // SDL_RenderSetLogicalSize below so the 4:3 frame is letterboxed on a
        // 16:9 monitor instead of being stretched.
        const auto& settings = options.settings;
        window_mode = settings.window_mode;
        windowed_width = settings.window_width;
        windowed_height = settings.window_height;
        windowed_x = settings.window_x;
        windowed_y = settings.window_y;
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
        Uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
        if (window_mode == byzantine::WindowMode::windowed ||
            window_mode == byzantine::WindowMode::maximized) {
            window_flags |= SDL_WINDOW_RESIZABLE;
        }
        window = SDL_CreateWindow("Byzantine: The Betrayal - scene player",
                                  windowed_x >= 0 ? windowed_x : SDL_WINDOWPOS_CENTERED,
                                  windowed_y >= 0 ? windowed_y : SDL_WINDOWPOS_CENTERED,
                                  windowed_width, windowed_height, window_flags);
        if (window == nullptr) throw std::runtime_error(SDL_GetError());
        if (settings.monitor >= 0) {
            const int displays = SDL_GetNumVideoDisplays();
            if (settings.monitor < displays) {
                SDL_Rect bounds{};
                if (SDL_GetDisplayBounds(settings.monitor, &bounds) == 0) {
                    SDL_SetWindowPosition(window, bounds.x + (bounds.w - windowed_width) / 2,
                                          bounds.y + (bounds.h - windowed_height) / 2);
                }
            }
        }
        Uint32 renderer_flags = SDL_RENDERER_ACCELERATED;
        if (settings.vsync) renderer_flags |= SDL_RENDERER_PRESENTVSYNC;
        renderer = SDL_CreateRenderer(window, -1, renderer_flags);
        if (renderer == nullptr) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr) throw std::runtime_error(SDL_GetError());
        SDL_RenderSetLogicalSize(renderer, kScreenWidth, kScreenHeight);
        apply_scale_mode(settings.scale_mode);
        apply_vsync(settings.vsync);
        if (settings.gamepad) open_gamepad();
        if (window_mode != byzantine::WindowMode::windowed) {
            apply_window_mode(window_mode);
        }
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                    SDL_TEXTUREACCESS_STREAMING, kScreenWidth,
                                    kScreenHeight);
        if (texture == nullptr) throw std::runtime_error(SDL_GetError());
        surface = byzantine::make_surface(kSceneWidth, kSceneHeight, 0);
        screen = byzantine::make_surface(kScreenWidth, kScreenHeight, 0);

        auto root = options.backgrounds;
        if (std::filesystem::is_directory(root / "inventor")) root /= "inventor";
        std::optional<std::filesystem::path> strip =
            find_language_child(options, false, "inventor", "instrip");
        if (!strip) strip = find_child(root, "instrip");
        if (strip) {
            bottom_bar = decode_ui_image(*strip, true, true);
            if (!bottom_bar) {
                std::cout << "ui_asset_missing file=" << strip->string()
                          << " (inventory strip will be a plain block)\n";
            }
        } else if (!options.backgrounds.empty()) {
            std::cout << "ui_asset_missing name=inventor/instrip.tga root="
                      << options.backgrounds
                      << " (extract setupart/inventor with port/scene.sh)\n";
        }
        load_top_bar_assets();

        int window_width = 0;
        int window_height = 0;
        int output_width = 0;
        int output_height = 0;
        SDL_GetWindowSize(window, &window_width, &window_height);
        SDL_GetRendererOutputSize(renderer, &output_width, &output_height);
        std::cout << "window geometry=" << window_width << 'x' << window_height
                  << " output=" << output_width << 'x' << output_height
                  << " logical=" << logical_width << 'x' << logical_height
                  << " (mouse events arrive in logical coordinates)\n";
    }

    // 0x408e3f: setupart/TOP_BAR.TGA + setupart/hotbar.rle + setupart/dwnbar.rle.
    // The directory is `setupart` in the shipped tree; the game's own string is
    // "setupart\\top_bar.tga" (0x45d2e8), so accept the case-insensitive child.
    // on_off.rle supplies the row markers and the preference "off" overlays
    // (0x41f94e/0x41f96e).
    // Rebuilds the localized top bar/strip after a text-language switch. The
    // images are cached in members, so they must be cleared first.
    void reload_ui_assets(const Options& options) {
        top_bar.reset();
        bottom_bar.reset();
        hotbar_frames.clear();
        dwnbar_frames.clear();
        marker_frames.clear();
        roll_frames.clear();
        yellow_frames.clear();
        auto root = options.backgrounds;
        if (std::filesystem::is_directory(root / "inventor")) root /= "inventor";
        auto strip = find_language_child(options, false, "inventor", "instrip");
        if (!strip) strip = find_child(root, "instrip");
        if (strip) bottom_bar = decode_ui_image(*strip, true, true);
        load_top_bar_assets();
    }

    void load_top_bar_assets() {
        if (ui_options == nullptr) return;
        const auto setup_asset = [&](const char* stem) -> std::optional<std::filesystem::path> {
            if (const auto file = find_language_child(*ui_options, false, "setupart", stem)) {
                return file;
            }
            if (ui_options->backgrounds.empty()) return std::nullopt;
            for (const auto& root : {ui_options->backgrounds,
                                     ui_options->backgrounds / "copy2hd"}) {
                std::error_code error;
                if (std::filesystem::is_directory(root / "setupart", error)) {
                    if (const auto file = find_child(root / "setupart", stem)) return file;
                }
                for (const auto& entry :
                     std::filesystem::directory_iterator(root, error)) {
                    if (!entry.is_directory()) continue;
                    if (lower_ascii(entry.path().filename().string()) != "setupart") continue;
                    if (const auto file = find_child(entry.path(), stem)) return file;
                }
            }
            return std::nullopt;
        };
        const auto load_rle_set = [&](const char* stem, std::vector<UiImage>& target) {
            const auto file = setup_asset(stem);
            if (!file) {
                std::cout << "ui_asset_missing name=setupart/" << stem << ".rle"
                          << " (top bar asset lookup failed)\n";
                return;
            }
            try {
                for (const auto& frame : byzantine::decode_rle(read_file(*file))) {
                    target.push_back(ui_image_from_rle(frame));
                }
                std::cout << "ui_rle file=" << file->filename().string()
                          << " frames=" << target.size() << '\n';
            } catch (const std::exception& decode_error) {
                std::cout << "ui_asset_decode_failed file=" << file->string()
                          << " error=" << decode_error.what() << '\n';
            }
        };
        const auto bar = setup_asset("top_bar");
        if (bar) {
            top_bar = decode_ui_image(*bar, true, true);
            if (top_bar) {
                std::cout << "ui_asset top_bar=" << bar->filename().string()
                          << " size=" << top_bar->width << 'x' << top_bar->height
                          << '\n';
            }
        } else {
            std::cout << "ui_asset_missing name=setupart/top_bar.tga"
                      << " (the top bar stays hidden)\n";
        }
        load_rle_set("hotbar", hotbar_frames);
        load_rle_set("dwnbar", dwnbar_frames);
        load_rle_set("on_off", marker_frames);
        // 0x41d7ff: FUN_004200bb(0x46c620, "roll.rle") expects 0xf frames and
        // FUN_004200bb(0x46c7f0, "yellow.rle") expects 4.
        load_rle_set("roll", roll_frames);
        load_rle_set("yellow", yellow_frames);
        // 0x4090c4 indexes the RLE set with 0x408fe7's 1..4 button number; a
        // missing frame is reported instead of silently shifting the ids.
        if (hotbar_frames.size() < 5 || dwnbar_frames.size() < 5) {
            std::cout << "ui_top_bar incomplete hotbar=" << hotbar_frames.size()
                      << " dwnbar=" << dwnbar_frames.size()
                      << " (expected 5 frames each)\n";
        }
        if (marker_frames.size() < 13) {
            std::cout << "ui_menu_markers incomplete frames=" << marker_frames.size()
                      << " (expected 13 in on_off.rle)\n";
        }
    }

    // 0x408f32(1) + 0x4090c4: the bar image, then the hovered/pressed button
    // frame at the RLE frame's own x/y (each frame carries its screen origin).
    void draw_top_bar() {
        if (!top_bar_visible || !top_bar) return;
        blit_ui(*top_bar, 0, 0);
        const auto& set = top_bar_press != 0 ? dwnbar_frames : hotbar_frames;
        const int index = top_bar_press != 0 ? top_bar_press : top_bar_hover;
        if (index <= 0 || static_cast<std::size_t>(index) >= set.size()) return;
        const auto& frame = set[static_cast<std::size_t>(index)];
        // Each RLE frame carries its own screen origin (0,0 / 182,0 / 325,0 /
        // 501,0); the shipped art depends on it.
        blit_ui(frame, frame.x, frame.y);
    }

    void load_background(const std::filesystem::path& file) {
        if (file.empty()) {
            background_key.clear();
            background_pixels.clear();
            return;
        }
        if (background_key == file.string()) return;
        const auto image = byzantine::decode_jpeg_file(file.string());
        if (image.width != kSceneWidth || image.height != kSceneHeight) {
            throw std::runtime_error("background is not 640x400: " + file.string());
        }
        background_pixels = byzantine::rgb_to_rgb565(image);
        background_key = file.string();
    }

    // The projector is 0x40241c's panorama branch: a 0xb9 node makes the
    // record's +0x00/+0x02 source coordinates (0x423e03 projects them onto the
    // viewport). Without a panorama they are already surface coordinates.
    void compose(SceneView& view, bool verbose = true, bool advance = true) {
        std::fill(surface.pixels.begin(), surface.pixels.end(), 0);
        // The scene layer is whichever of 0x69 (background) / 0xb9 (node) ran
        // LAST (see panorama_layer).
        // A gated replay composes a frame at the instruction that queued a
        // sound, before the committed trace's panorama node has been loaded by
        // apply_panorama_events. Do not dereference an empty optional: fall
        // back to the background for that intermediate frame and report it.
        const bool show_panorama = panorama_layer(view) && view.panorama.has_value();
        if (panorama_layer(view) && !view.panorama.has_value()) {
            if (verbose) std::cout << "panorama_pending (node not loaded yet)\n";
        }
        if (show_panorama) {
            // 0x4242bd samples the current node through the camera map; the
            // record pass then draws on top, exactly like the background path.
            view.panorama->render(surface);
        } else {
            load_background(view.background_file);
            if (!background_pixels.empty()) {
                byzantine::blit_image(surface, kSceneWidth, kSceneHeight,
                                      background_pixels.data(), 0, 0);
            }
        }
        const auto drawn = byzantine::compose_scene(view.state, surface, scene_projector(view), advance);
        // Records this pass drew and stepped (0x43b216's normal pass). The
        // animation clock applies any further passes the original's uncapped
        // loop would have run between two host frames to exactly these.
        drawn_record_ids.clear();
        for (const auto& entry : drawn.records) {
            // 0x43b216 runs 0x43bb77 after every record it accepted, even when
            // the current frame covers no pixels (a blank delta frame still
            // advances); only rejected/hidden/snap records are skipped.
            if (entry.snap || !entry.visible) continue;
            drawn_record_ids.push_back(entry.id);
        }
        draw_text_windows(view, verbose);
        if (!verbose) return;
        if (show_panorama) {
            std::cout << "composed panorama=" << view.panorama->path()
                      << " size=" << view.panorama->source_width() << 'x'
                      << view.panorama->source_height()
                      << " padding=" << view.panorama->padding()
                      << " yaw=" << view.panorama->yaw_degrees()
                      << " pitch=" << view.panorama->pitch_degrees()
                      << " drawn=" << drawn.drawn
                      << " projected=" << drawn.projected
                      << " skipped_no_frame=" << drawn.skipped_no_frame
                      << " skipped_snap=" << drawn.skipped_snap
                      << " snap_drawn=" << drawn.snap_drawn
                      << " skipped_hidden=" << drawn.skipped_hidden
                      << " pixels=" << drawn.pixels_written
                      << " drawn_ids=" << drawn_ids(drawn) << '\n';
            return;
        }
        std::cout << "composed background="
                  << (view.background_file.empty() ? "none"
                                                   : view.background_file.filename().string())
                  << " drawn=" << drawn.drawn
                  << " skipped_no_frame=" << drawn.skipped_no_frame
                  << " skipped_snap=" << drawn.skipped_snap
                  << " snap_drawn=" << drawn.snap_drawn
                  << " skipped_hidden=" << drawn.skipped_hidden
                  << " pixels=" << drawn.pixels_written
                  << " drawn_ids=" << drawn_ids(drawn) << '\n';
    }

    // True when the visible part of the scene can still change without input:
    // a record with a loaded frame list, a frame index and at least two frames
    // that is not frozen by 0x04. Used to decide whether the event loop needs a
    // 60 Hz recompose; a static scene keeps redrawing only on events.
    bool scene_animates(const SceneView& view) const {
        const byzantine::SceneProjector* projector = scene_projector(view);
        for (const auto& [id, record] : view.state.records) {
            (void)id;
            if (!record.frames_known || !record.current_frame) continue;
            if (record.frame_count < 2) continue;
            if ((record.flags & (0x04 | 0x20 | 0x40)) != 0) continue;
            if (record.kind >= 5) continue;
            // 0x43b216 only steps records it drew: an off-window origin is
            // rejected by 0x40241c and never advances, so it must not keep the
            // window repainting. In a panorama scene the origin is in source
            // space, so the same projection decides visibility.
            if (projector != nullptr) {
                // Same 0x40241c anchor test the composer uses, with the current
                // frame's geometry (0x43c114 reads it from the frame node).
                int width = 1;
                int height = 1;
                if (record.frames_known && record.current_frame &&
                    *record.current_frame < record.frames.size()) {
                    width = record.frames[*record.current_frame].width;
                    height = record.frames[*record.current_frame].height;
                }
                // The source-space patch path can draw a partially visible
                // record whose five anchors all fail (it is clipped by the
                // viewport, not dropped); use its bbox when available.
                int x0 = 0;
                int y0 = 0;
                int x1 = -1;
                int y1 = -1;
                if (projector->source_rect_viewport_bbox(
                        record.x, record.y - byzantine::kPlayfieldTop, width,
                        height, x0, y0, x1, y1)) {
                    if (x0 <= x1 && y0 <= y1) return true;
                    continue;
                }
                if (!byzantine::project_record_position(*projector, record.x, record.y,
                                                        width, height)) {
                    continue;
                }
                return true;
            }
            if (record.x < 0 || record.x >= kSceneWidth) continue;
            const int y = record.y - byzantine::kPlayfieldTop;
            if (y < 0 || y >= kSceneHeight) continue;
            return true;
        }
        return false;
    }

    // Shows any RGB565 image, switching the renderer's logical size when the
    // image size differs. The conversation uses this for its 640x480 screen.
    // `hotspot_offset_y` maps the playfield-only hotspot coordinates into the
    // image (0 for the 640x400 playfield, kScreenPlayfieldTop for the
    // 640x480 conversation screen, whose playfield starts at row 0x14).
    void fill_rect(int x, int y, int width, int height, std::uint16_t colour) {
        for (int row = 0; row < height; ++row) {
            const int screen_y = y + row;
            if (screen_y < 0 || screen_y >= screen.height) continue;
            for (int column = 0; column < width; ++column) {
                const int screen_x = x + column;
                if (screen_x < 0 || screen_x >= screen.width) continue;
                screen.pixels[static_cast<std::size_t>(screen_y) * screen.width + screen_x] =
                    colour;
            }
        }
    }

    void blit_ui(const UiImage& image, int x, int y) {
        for (int row = 0; row < image.height; ++row) {
            const int screen_y = y + row;
            if (screen_y < 0 || screen_y >= screen.height) continue;
            for (int column = 0; column < image.width; ++column) {
                const int screen_x = x + column;
                if (screen_x < 0 || screen_x >= screen.width) continue;
                const auto source = static_cast<std::size_t>(row) * image.width + column;
                if (image.alpha[source] == 0) continue;
                screen.pixels[static_cast<std::size_t>(screen_y) * screen.width + screen_x] =
                    image.pixels[source];
            }
        }
    }

    // 0x407254: inventor/<name>.tga for the ordinary pointer and
    // inventor/hotitems/<h-name>.tga for 0x407bd7(1)'s hotitem variant. Both
    // are decoded once; a missing file is reported, never invented.
    const UiImage* inventory_image(const std::string& name, bool hotitem) {
        const auto stem = hotitem ? byzantine::hotitem_media_name(name) : name;
        const auto key = (hotitem ? "h:" : "i:") + lower_ascii(stem);
        const auto cached = item_icons.find(key);
        if (cached != item_icons.end()) return &cached->second;
        if (ui_options == nullptr || ui_options->backgrounds.empty()) return nullptr;
        auto root = ui_options->backgrounds;
        if (std::filesystem::is_directory(root / "inventor")) root /= "inventor";
        if (hotitem) root /= "hotitems";
        const auto found = find_child(root, stem);
        if (!found) {
            if (missing_ui.insert(key).second) {
                std::cout << "ui_asset_missing name=inventor/"
                          << (hotitem ? "hotitems/" : "") << stem
                          << ".tga root=" << ui_options->backgrounds << '\n';
            }
            return nullptr;
        }
        auto decoded = decode_ui_image(*found, true, false);
        if (!decoded) {
            if (missing_ui.insert(key).second) {
                std::cout << "ui_asset_decode_failed file=" << found->string() << '\n';
            }
            return nullptr;
        }
        const auto inserted = item_icons.emplace(key, std::move(*decoded));
        return &inserted.first->second;
    }

    const UiImage* item_icon(const std::string& name) { return inventory_image(name, false); }

    // 0x40dae5 formats "cursors/%s*.tga" (or "cursors/klio/%s*.tga") and lets
    // 0x40e89f resolve the wildcard to up to four animation frames. The port
    // shows the exact-name file, or the first file whose stem starts with the
    // name (klio's forward0/1/2, back1..4, INFO1..4). Multi-frame animation is
    // not played; the count is reported once.
    std::filesystem::path cursors_root() const {
        if (ui_options == nullptr) return {};
        if (!ui_options->cursors.empty()) return ui_options->cursors;
        if (ui_options->backgrounds.empty()) return {};
        auto root = ui_options->backgrounds;
        std::error_code error;
        if (std::filesystem::is_directory(root / "cursors", error)) return root / "cursors";
        return {};
    }

    // 0x40e89f fills up to four frame handles and 0x40ebe1 advances the current
    // one every 250 ms. The klio set is the only shipped set with more than one
    // file per pointer (forward/info/back, four frames each); the normal set has
    // one file per stem, so this returns a single image there.
    const std::vector<const UiImage*>& cursor_frames(byzantine::CursorTheme theme,
                                                     const std::string& name) {
        static const std::vector<const UiImage*> none;
        const bool klio = theme == byzantine::CursorTheme::klio;
        const auto group = std::string(klio ? "kf:" : "f:") + lower_ascii(name);
        const auto cached = cursor_frame_groups.find(group);
        if (cached != cursor_frame_groups.end()) {
            if (cached->second.empty()) return none;
            return cached->second;
        }
        auto& frames = cursor_frame_groups[group];
        auto root = cursors_root();
        if (root.empty()) return none;
        if (klio) root /= "klio";
        const auto match = byzantine::find_cursor_file(root, name);
        if (!match) return none;
        for (const auto& path : match->paths) {
            auto& slot = cursor_frame_images[group][path.string()];
            if (slot.width == 0) {
                auto decoded = decode_ui_image(path, true, false);
                if (!decoded) continue;
                slot = std::move(*decoded);
            }
            frames.push_back(&slot);
        }
        return frames;
    }

    const UiImage* cursor_image(byzantine::CursorTheme theme, const std::string& name) {
        const bool klio = theme == byzantine::CursorTheme::klio;
        const auto key = std::string(klio ? "kc:" : "c:") + lower_ascii(name);
        const auto cached = item_icons.find(key);
        if (cached != item_icons.end()) return &cached->second;
        auto root = cursors_root();
        if (root.empty()) return nullptr;
        if (klio) root /= "klio";
        const auto match = byzantine::find_cursor_file(root, name);
        if (!match) {
            if (missing_ui.insert(key).second) {
                std::cout << "cursor_missing theme=" << (klio ? "klio" : "normal")
                          << " name=" << name << " root=" << root.string() << '\n';
            }
            return nullptr;
        }
        auto decoded = decode_ui_image(match->path, true, false);
        if (!decoded) {
            if (missing_ui.insert(key).second) {
                std::cout << "cursor_decode_failed file=" << match->path.string() << '\n';
            }
            return nullptr;
        }
        if (match->frames > 1) {
            std::cout << "cursor_frames theme=" << (klio ? "klio" : "normal")
                      << " name=" << name << " files=" << match->frames
                      << " shown=" << match->path.filename().string()
                      << " (0x40ebe1's 250 ms cycling is not modelled)\n";
        }
        const auto inserted = item_icons.emplace(key, std::move(*decoded));
        return &inserted.first->second;
    }

    // The original 640x480 scene surface: playfield at row 0x14, then the
    // 0x4075fe strip at 0x1a4. 0x407420 draws at most six 60-pixel item slots
    // (x = 0xa0 + 0x3c*i), 0x407f6a is the examine button and 0x407df9/0x407e74
    // are the two page arrows. Only the entries present in the current page are
    // drawn; nothing is enabled or invented.
    void build_screen(const SceneView& view) {
        std::fill(screen.pixels.begin(), screen.pixels.end(), 0);
        for (int row = 0; row < surface.height; ++row) {
            for (int column = 0; column < surface.width; ++column) {
                screen.pixels[static_cast<std::size_t>(row + kScreenPlayfieldTop) *
                                  screen.width + column] =
                    surface.pixels[static_cast<std::size_t>(row) * surface.width + column];
            }
        }
        if (!view.state.dialogue_visible) {
            // 0x408f32's bar sits at rows 0..0x13 regardless of the strip's
            // visibility; it is a roll-over overlay, not part of the playfield.
            draw_top_bar();
            return;
        }

        if (bottom_bar) {
            blit_ui(*bottom_bar, 0, kScreenBandTop);
        } else {
            fill_rect(0, kScreenBandTop, kScreenWidth,
                      kScreenHeight - kScreenBandTop, 0x6a00);
            fill_rect(0, kScreenBandTop, kScreenWidth, 1, 0xce79);
        }

        const auto page = byzantine::inventory_page(view.state);
        for (std::size_t index = 0; index < page.items.size(); ++index) {
            const auto id = page.items[index];
            const int x = 0xa0 + static_cast<int>(index) * 0x3c;
            const auto name = byzantine::inventory_name(view.state, id);
            const auto* icon = name.empty() ? nullptr : item_icon(name);
            if (icon != nullptr) {
                blit_ui(*icon, x, 0x1a4 + 6);
            } else {
                // Keep the real slot geometry visible; report the missing asset
                // instead of drawing an invented item picture.
                fill_rect(x, 0x1a4 + 6, 0x3c, 0x36, 0x3186);
                fill_rect(x + 1, 0x1a4 + 7, 0x3a, 0x34, 0x4208);
            }
            if (view.state.current_dialogue_record &&
                *view.state.current_dialogue_record == id) {
                fill_rect(x, 0x1a4 + 6, 0x3c, 2, 0xffe0);
                fill_rect(x, 0x1a4 + 0x3b, 0x3c, 2, 0xffe0);
                fill_rect(x, 0x1a4 + 6, 2, 0x36, 0xffe0);
                fill_rect(x + 0x3a, 0x1a4 + 6, 2, 0x36, 0xffe0);
            }
        }
        draw_top_bar();
    }

    // The only presentation entry point. Whether the hotspot-outline diagnostic
    // is drawn is decided here from ScreenContent, never per caller: a clip (and
    // its audio drain) and a menu screen are never outlined.
    void present_image(const byzantine::Rgb16Surface& image, const SceneView& view,
                       int hotspot_offset_y, const char* tag = "scene") {
        tick_cursor();
        if (logical_width != image.width || logical_height != image.height) {
            SDL_DestroyTexture(texture);
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                        SDL_TEXTUREACCESS_STREAMING, image.width,
                                        image.height);
            if (texture == nullptr) throw std::runtime_error(SDL_GetError());
            logical_width = image.width;
            logical_height = image.height;
            SDL_RenderSetLogicalSize(renderer, logical_width, logical_height);
            int window_width = 0;
            int window_height = 0;
            int output_width = 0;
            int output_height = 0;
            SDL_GetWindowSize(window, &window_width, &window_height);
            SDL_GetRendererOutputSize(renderer, &output_width, &output_height);
            std::cout << "window geometry=" << window_width << 'x' << window_height
                      << " output=" << output_width << 'x' << output_height
                      << " logical=" << logical_width << 'x' << logical_height
                      << " (logical size changed)\n";
        }
        SDL_UpdateTexture(texture, nullptr, image.pixels.data(), image.width * 2);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        // A hotspot rectangle lives in the coordinates the click test uses: the
        // 640x400 playfield for a background scene, the panorama SOURCE for a
        // panorama scene. Only the former can be drawn as a rectangle here, so
        // the panorama outline is not invented (the click still works).
        // The diagnostic belongs to the scene: ScreenContent keeps it off a
        // clip (including its audio drain) and off a menu. By default only the
        // hotspot under the pointer is drawn (--hotspots-all draws every one).
        const bool draw_overlay =
            overlay_allowed() && !panorama_layer(view) &&
            (overlay_all() || hover_overlay_slot >= 0);
        if (draw_overlay) {
            for (const auto& hotspot : view.trace.active_hotspots) {
                if (!overlay_all() && hotspot.slot != hover_overlay_slot) continue;
                SDL_Rect bounds{hotspot.x,
                                hotspot.y - kPlayfieldTop + hotspot_offset_y,
                                hotspot.width, hotspot.height};
                if (bounds.y < 0) {
                    bounds.h += bounds.y;
                    bounds.y = 0;
                }
                if (bounds.y + bounds.h > image.height) {
                    bounds.h = image.height - bounds.y;
                }
                if (bounds.x < 0) {
                    bounds.w += bounds.x;
                    bounds.x = 0;
                }
                if (bounds.x + bounds.w > image.width) bounds.w = image.width - bounds.x;
                if (bounds.w <= 0 || bounds.h <= 0) continue;
                SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
                SDL_RenderDrawRect(renderer, &bounds);
            }
        }
        SDL_RenderPresent(renderer);
        log_present(image, view, tag, draw_overlay);
        update_last_playfield(image, hotspot_offset_y, tag);
    }

    // Keeps the playfield of the frame that is on screen, so the next travel or
    // panorama load has a real departing picture to blend from. A presentation
    // that is not a whole 640x480 screen (or a bare 640x400 playfield) is not a
    // scene frame, so it invalidates the buffer instead of guessing.
    void update_last_playfield(const byzantine::Rgb16Surface& image,
                               int hotspot_offset_y, const char* tag) {
        if (std::strcmp(tag, "transition") == 0) return;  // keep the departing frame
        const bool full_screen = image.width == kScreenWidth &&
                                 image.height == kScreenHeight &&
                                 hotspot_offset_y == kScreenPlayfieldTop;
        const bool playfield = image.width == kSceneWidth &&
                               image.height == kSceneHeight && hotspot_offset_y == 0;
        if (!full_screen && !playfield) {
            last_playfield_valid = false;
            return;
        }
        const int top = full_screen ? kScreenPlayfieldTop : 0;
        last_playfield.width = kSceneWidth;
        last_playfield.height = kSceneHeight;
        last_playfield.pixels.resize(static_cast<std::size_t>(kSceneWidth) * kSceneHeight);
        for (int row = 0; row < kSceneHeight; ++row) {
            const auto begin = image.pixels.begin() +
                               static_cast<std::size_t>(row + top) * image.width;
            std::copy_n(begin, kSceneWidth,
                        last_playfield.pixels.begin() +
                            static_cast<std::size_t>(row) * kSceneWidth);
        }
        last_playfield_valid = true;
    }

    // 0x40186e's transition family (0x65..0x6a): the new picture arrives as the
    // blend steps the original presents, not as one cut. `code` is the engine
    // code the script carried; `source` is where it came from, so a port default
    // is never mistaken for a recovered value. The final frame goes through
    // present(), which keeps the single SDL_RenderPresent path.
    void present_transition(const SceneView& view, std::uint16_t code,
                            const char* source) {
        const auto kind = byzantine::transition_kind(code);
        if (kind == byzantine::TransitionKind::none) {
            std::cout << "transition code=0x" << std::hex << code << std::dec
                      << " source=" << source << " kind=none (the picture appears at once)\n";
            present(view);
            return;
        }
        if (!last_playfield_valid || ui_options == nullptr || !ui_options->transitions) {
            std::cout << "transition code=0x" << std::hex << code << std::dec
                      << " source=" << source << " skipped reason="
                      << (!last_playfield_valid ? "no_departing_frame"
                                                : "--no-transitions")
                      << '\n';
            present(view);
            return;
        }
        update_cursor(view);
        build_screen(view);
        const auto new_screen = screen;
        const int steps = byzantine::transition_steps(kind);
        const int step_ms = byzantine::transition_step_ms(kind);
        // The playfield of the arrival frame, in the blend's own coordinates.
        byzantine::Rgb16Surface target;
        target.width = kSceneWidth;
        target.height = kSceneHeight;
        target.pixels.resize(static_cast<std::size_t>(kSceneWidth) * kSceneHeight);
        for (int row = 0; row < kSceneHeight; ++row) {
            std::copy_n(new_screen.pixels.begin() +
                            static_cast<std::size_t>(row + kScreenPlayfieldTop) *
                                kScreenWidth,
                        kSceneWidth,
                        target.pixels.begin() + static_cast<std::size_t>(row) * kSceneWidth);
        }
        byzantine::TransitionPlayer player(kind, last_playfield, target);
        const auto started = std::chrono::steady_clock::now();
        std::size_t presented = 0;
        // A transition frame is not the scene (the hotspot-outline diagnostic
        // stays off it), and the trace names the owner once for the family.
        const auto previous_content = content;
        enter_content(ScreenContent::transition);
        for (int index = 0; index < steps && window != nullptr; ++index) {
            player.next();  // produces this step: 0x419a51's phases need it
            const auto& step = player.surface();
            auto frame = new_screen;
            for (int row = 0; row < kSceneHeight; ++row) {
                std::copy_n(step.pixels.begin() + static_cast<std::size_t>(row) * kSceneWidth,
                            kSceneWidth,
                            frame.pixels.begin() +
                                static_cast<std::size_t>(row + kScreenPlayfieldTop) *
                                    kScreenWidth);
            }
            present_detail = "code=0x" + hex_word(code) + " step=" +
                             std::to_string(index + 1) + "/" + std::to_string(steps);
            present_image(frame, view, kScreenPlayfieldTop, "transition");
            if (ui_options != nullptr && !ui_options->transition_shot.empty()) {
                std::error_code error;
                const std::filesystem::path directory(ui_options->transition_shot);
                std::filesystem::create_directories(directory, error);
                const std::filesystem::path path =
                    directory / ("transition-0x" + hex_word(code) + "-" +
                                 std::to_string(index + 1) + ".ppm");
                byzantine::write_ppm(frame, path.string());
            }
            present_detail.clear();
            ++presented;
            // 0x41920f's own gate: one step every 60 ms; the presenting
            // routines only blit, so one 60 Hz tick is their spacing.
            std::this_thread::sleep_until(
                started + std::chrono::milliseconds(step_ms * (index + 1)));
        }
        restore_content(previous_content);
        present(view);
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                                 std::chrono::steady_clock::now() - started)
                                 .count();
        std::cout << "transition code=0x" << std::hex << code << std::dec
                  << " kind=" << transition_kind_name(kind)
                  << " source=" << source << " steps=" << presented
                  << " step_ms=" << step_ms << " elapsed_ms=" << elapsed << '\n';
    }

    static const char* transition_kind_name(byzantine::TransitionKind kind) {
        switch (kind) {
            case byzantine::TransitionKind::fade8: return "fade8";
            case byzantine::TransitionKind::fade4: return "fade4";
            case byzantine::TransitionKind::blink: return "blink";
            case byzantine::TransitionKind::reveal: return "reveal";
            case byzantine::TransitionKind::interleave27: return "interleave27";
            case byzantine::TransitionKind::none: break;
        }
        return "none";
    }

    static std::string hex_word(std::uint16_t value) {
        static const char* digits = "0123456789abcdef";
        std::string text;
        bool started = false;
        for (int shift = 12; shift >= 0; shift -= 4) {
            const int nibble = (value >> shift) & 0xf;
            if (nibble != 0) started = true;
            if (started || shift == 0) text.push_back(digits[nibble]);
        }
        return text;
    }

    // Called after each SDL_RenderPresent. The checksum samples every 4th
    // pixel: enough to tell two different pictures apart without hashing
    // 300k pixels per frame in the animation loop.
    void log_present(const byzantine::Rgb16Surface& image, const SceneView& view,
                     const char* tag, bool draw_overlay) {
        std::uint64_t hash = 1469598103934665603ull;
        for (std::size_t index = 0; index < image.pixels.size(); index += 4) {
            hash ^= image.pixels[index];
            hash *= 1099511628211ull;
        }
        const auto now = std::chrono::steady_clock::now();
        const std::string tag_name(tag);
        const int bar = top_bar_visible
                            ? (top_bar_press != 0 ? top_bar_press : top_bar_hover)
                            : 0;
        // "Which kind of picture is up" without the camera angle: a panorama
        // rotation changes the pixels (and so the hash) but not this key.
        const std::string state_key =
            tag_name + '|' + layer_key(view) + '|' +
            (view.state.dialogue_visible ? "1" : "0") + '|' + std::to_string(bar);
        const bool state_changed = !present_hash_known || state_key != present_state_key;
        const bool picture_changed = state_changed || hash != present_hash;
        const auto held_prev =
            present_hash_known
                ? std::chrono::duration_cast<std::chrono::milliseconds>(now - picture_since)
                      .count()
                : 0;
        const auto since_log = std::chrono::duration_cast<std::chrono::milliseconds>(
                                   now - present_logged_at)
                                   .count();
        const bool captured = now < present_capture_until;
        ++present_index;
        // Dump the picture when a capture window is open (a scene load or a clip
        // end) or when the state changed, once per distinct checksum and up to
        // --present-dump-max. Video frames are skipped: their JPEGs are already
        // in the clip cache and the trace line names the exact frame.
        std::string dump_path;
        if (ui_options != nullptr && !ui_options->present_dump.empty() &&
            present_dumps < ui_options->present_dump_max && present_detail.empty() &&
            (captured || state_changed) && dumped_hashes.insert(hash).second) {
            std::error_code error;
            const std::filesystem::path directory(ui_options->present_dump);
            std::filesystem::create_directories(directory, error);
            if (!error) {
                char name[96];
                std::snprintf(name, sizeof(name), "p%05llu-%s-0x%016llx.ppm",
                              static_cast<unsigned long long>(present_index),
                              tag_name.c_str(),
                              static_cast<unsigned long long>(hash));
                if (byzantine::write_ppm(image, (directory / name).string())) {
                    dump_path = (directory / name).string();
                    ++present_dumps;
                    std::cout << "present_dump file=" << dump_path
                              << " index=" << present_index << " tag=" << tag_name
                              << " layer=" << layer_name(view) << '\n';
                }
            }
        }
        // A detail line per changed picture, per dumped frame and a 5 s
        // heartbeat. A 60 Hz animation therefore does not flood the trace.
        if (picture_changed || since_log >= 5000 || !dump_path.empty()) {
            ++present_logged;
            byzantine::diagnostics::write("present", [&](auto& out) {
                out << "index=" << present_index << " tag=" << tag_name;
                if (!present_detail.empty()) out << " detail=" << present_detail;
                out << " scene=" << view.name << " label=" << view.label
                    << " layer=" << layer_name(view)
                    << " dialogue=" << (view.state.dialogue_visible ? 1 : 0)
                    << " bar=" << bar << " overlay=" << (draw_overlay ? 1 : 0)
                    << " size=" << image.width << 'x'
                    << image.height << " hash=0x" << std::hex << hash;
                if (present_hash_known) {
                    out << " prev_hash=0x" << present_hash;
                }
                out << std::dec << " changed=" << (picture_changed ? 1 : 0)
                    << " prev_held_ms=" << held_prev
                    << " since_log_ms=" << since_log
                    << " captured=" << (captured ? 1 : 0);
                if (!dump_path.empty()) out << " dump=" << dump_path;
            });
            present_logged_at = now;
        }
        present_detail.clear();
        if (picture_changed) {
            picture_since = now;
            present_state_key = state_key;
        }
        present_hash = hash;
        present_hash_known = true;
    }

    // Arms the capture window used by log_present before a scene load or after
    // a 0xbd clip, i.e. exactly where a stale/other picture can flash.
    void arm_present_capture(int milliseconds, const char* reason) {
        const auto until =
            std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
        if (until > present_capture_until) present_capture_until = until;
        byzantine::diagnostics::write("present_capture", [&](auto& out) {
            out << "reason=" << reason << " ms=" << milliseconds;
        });
    }

    // What the scene layer is at this instant: the panorama node (with the
    // camera) when 0xb9 is the last layer, otherwise the background JPEG.
    static std::string layer_name(const SceneView& view) {
        if (panorama_layer(view) && view.panorama) {
            std::ostringstream out;
            out << "panorama:"
                << std::filesystem::path(view.panorama->path()).filename().string()
                << " yaw=" << view.panorama->yaw_degrees()
                << " pitch=" << view.panorama->pitch_degrees();
            return out.str();
        }
        if (!view.background_file.empty()) {
            return "background:" +
                   std::filesystem::path(view.background_file).filename().string();
        }
        return "none";
    }

    // layer_name without the camera: the key an unchanged picture keeps while
    // the panorama turns.
    static std::string layer_key(const SceneView& view) {
        if (panorama_layer(view) && view.panorama) {
            return "panorama:" +
                   std::filesystem::path(view.panorama->path()).filename().string();
        }
        if (!view.background_file.empty()) {
            return "background:" +
                   std::filesystem::path(view.background_file).filename().string();
        }
        return "none";
    }

    // 0x40df05 installs an SDL colour cursor from the game's own art. A table
    // entry carries its hotspot (+0x04/+0x08); an inventory item pointer is
    // centred, exactly like 0x40df05's `mouse - (width >> 1)`.
    SDL_Cursor* cursor_from_image(const UiImage& image, int hot_x, int hot_y) {
        if (image.width <= 0 || image.height <= 0) return nullptr;
        const int scale = ui_options != nullptr
                              ? std::clamp(ui_options->settings.cursor_scale, 1, 4)
                              : 1;
        const int width = image.width * scale;
        const int height = image.height * scale;
        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
            0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);
        if (surface == nullptr) return nullptr;
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                const int source_row = row / scale;
                const int source_column = column / scale;
                const auto index = static_cast<std::size_t>(source_row) * image.width +
                                   source_column;
                const auto pixel = image.pixels[index];
                const auto r = static_cast<std::uint32_t>((pixel >> 11) & 0x1f);
                const auto g = static_cast<std::uint32_t>((pixel >> 5) & 0x3f);
                const auto b = static_cast<std::uint32_t>(pixel & 0x1f);
                const auto argb =
                    (static_cast<std::uint32_t>(image.alpha[index]) << 24) |
                    (((r << 3) | (r >> 2)) << 16) |
                    (((g << 2) | (g >> 4)) << 8) | ((b << 3) | (b >> 2));
                auto* row_pixels = reinterpret_cast<std::uint32_t*>(
                    static_cast<std::uint8_t*>(surface->pixels) +
                    static_cast<std::ptrdiff_t>(row) * surface->pitch);
                row_pixels[column] = argb;
            }
        }
        hot_x = std::clamp(hot_x * scale, 0, width - 1);
        hot_y = std::clamp(hot_y * scale, 0, height - 1);
        auto* cursor = SDL_CreateColorCursor(surface, hot_x, hot_y);
        SDL_FreeSurface(surface);
        return cursor;
    }

    void clear_cursor_cache() {
        for (auto& entry : cursors_) {
            if (entry.second != nullptr) SDL_FreeCursor(entry.second);
        }
        cursors_.clear();
        cursor_animation.reset();
        cursor_key_.clear();
    }

    // 0x40df05: resolve the choice to art, then install it. `choice.kind ==
    // item` is 0x407bd7's inventory pointer (inventor/<name>.tga) and its
    // hotitem variant; anything else is an entry of one of the two cursor
    // tables, passed through 0x40de7b's small-variant remap.
    void set_cursor(const SceneView& view, const byzantine::CursorChoice& choice) {
        const UiImage* image = nullptr;
        std::vector<const UiImage*> frames;
        std::string key;
        std::string label;
        std::string missing_item;
        int hot_x = 0;
        int hot_y = 0;
        if (choice.kind == byzantine::CursorChoice::Kind::item) {
            const auto name = byzantine::inventory_name(view.state, choice.id);
            const auto stem = name.empty() || !choice.hotitem
                                  ? name
                                  : byzantine::hotitem_media_name(name);
            if (!name.empty()) {
                image = inventory_image(name, choice.hotitem);
                if (image != nullptr) {
                    key = (choice.hotitem ? "h:" : "i:") + lower_ascii(stem);
                    label = std::string("item name=") + stem +
                            " variant=" + (choice.hotitem ? "hotitem" : "normal");
                    hot_x = image->width / 2;
                    hot_y = image->height / 2;
                } else {
                    // A missing item TGA must not degrade to a host arrow: fall
                    // back to the game's table pointer below, and say why.
                    missing_item = std::string(" item name=") + stem + " variant=" +
                                   (choice.hotitem ? "hotitem" : "normal") +
                                   " (inventor art missing)";
                }
            }
        }
        if (key.empty()) {
            // An item without art (or without a name) falls back to the
            // default table pointer. Its inventory id must never be read as a
            // cursor id: the engine keeps them apart by encoding item ids as
            // 0x20 + 2*record (+1 = hotitem) before 0x40df05 sees them.
            const auto table_id = static_cast<std::uint8_t>(
                choice.kind == byzantine::CursorChoice::Kind::item ? 0 : choice.id);
            auto theme = byzantine::cursor_theme(view.state.alternate_mode);
            auto id = byzantine::cursor_slot(theme, panorama_layer(view), table_id);
            auto asset = byzantine::cursor_asset(theme, id);
            if (asset.name == nullptr && theme == byzantine::CursorTheme::klio) {
                // The klio table leaves talk/video empty on the shipped CD; the
                // original would dereference a NULL frame pointer.
                theme = byzantine::CursorTheme::normal;
                asset = byzantine::cursor_asset(theme, id);
            }
            if (asset.name == nullptr && id != 0) {
                id = 0;
                asset = byzantine::cursor_asset(byzantine::CursorTheme::normal, id);
            }
            key = "t:" + std::to_string(static_cast<int>(theme)) + ':' + std::to_string(id);
            label = std::string("table id=") + std::to_string(id) +
                    " name=" + (asset.name != nullptr ? asset.name : "default") +
                    " theme=" + (theme == byzantine::CursorTheme::klio ? "klio" : "normal");
            image = asset.name != nullptr ? cursor_image(theme, asset.name) : nullptr;
            if (asset.name != nullptr) frames = cursor_frames(theme, asset.name);
            hot_x = asset.hotspot_x;
            hot_y = asset.hotspot_y;
            if (image != nullptr) {
                label += " size=" + std::to_string(image->width) + "x" +
                         std::to_string(image->height) + " hot=" +
                         std::to_string(hot_x) + "," + std::to_string(hot_y);
            } else {
                label += " missing (system arrow)";
            }
            label += missing_item;
        }
        if (key == cursor_key_) return;
        cursor_key_ = key;
        SDL_Cursor* cursor = nullptr;
        const auto cached = cursors_.find(key);
        if (cached != cursors_.end()) {
            cursor = cached->second;
        } else {
            if (image != nullptr) cursor = cursor_from_image(*image, hot_x, hot_y);
            cursors_.emplace(key, cursor);  // a missing asset is remembered too
        }
        if (cursor == nullptr) {
            if (default_cursor_ == nullptr) {
                default_cursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            }
            SDL_SetCursor(default_cursor_);
        } else {
            SDL_SetCursor(cursor);
        }
        if (frames.size() > 1) {
            cursor_animation = CursorAnimation{key, frames, hot_x, hot_y, 0,
                                               std::chrono::steady_clock::now()};
            label += " frames=" + std::to_string(frames.size()) + " cycling_ms=250";
        } else {
            cursor_animation.reset();
        }
        std::cout << "cursor " << label << '\n';
    }

    // 0x40ebe1: the pointer's frames advance every 250 ms while one is
    // installed. Cheap when the pointer is static (one optional check).
    void tick_cursor() {
        if (!cursor_animation) return;
        const auto now = std::chrono::steady_clock::now();
        if (now - cursor_animation->since < std::chrono::milliseconds(250)) return;
        auto& animation = *cursor_animation;
        animation.since = now;
        animation.frame = (animation.frame + 1) % animation.frames.size();
        const auto frame_key = animation.key + "#" + std::to_string(animation.frame);
        SDL_Cursor* cursor = nullptr;
        const auto cached = cursors_.find(frame_key);
        if (cached != cursors_.end()) {
            cursor = cached->second;
        } else {
            cursor = cursor_from_image(*animation.frames[animation.frame],
                                       animation.hot_x, animation.hot_y);
            cursors_.emplace(frame_key, cursor);
        }
        if (cursor != nullptr) SDL_SetCursor(cursor);
        byzantine::diagnostics::write("cursor_frame", [&](auto& out) {
            out << "pointer=" << animation.key << " frame=" << animation.frame
                << " of=" << animation.frames.size() << " interval_ms=250";
        });
    }

    // 0x40c329/0x4083d4 for the point under the mouse. Rows 0x14..0x1a3 are
    // the playfield (hovered object), 0x1a4.. the inventory band; rows 0..0x13
    // are the top bar and use 0x408fe7's hotsmall pointer.
    void update_cursor_at(const SceneView& view, int x, int y) {
        byzantine::CursorChoice choice;
        if (y < kScreenPlayfieldTop) {
            // 0x408fe7 hands 0x40df05 the id 0x1c; 0x40de7b maps that to 0x1e
            // ("hotsmall") in the normal set outside a panorama.
            choice.id = 0x1c;
        } else if (y < kScreenBandTop) {
            const auto hit = hotspot_at(view, x, y - kScreenPlayfieldTop);
            choice = byzantine::hover_cursor(view.state, hovered_object(view, hit));
        } else {
            choice = byzantine::strip_cursor(view.state, x, y);
        }
        set_cursor(view, choice);
    }

    void update_cursor(const SceneView& view) {
        float logical_x = 0;
        float logical_y = 0;
        mouse_logical(&logical_x, &logical_y);
        update_cursor_at(view, static_cast<int>(logical_x), static_cast<int>(logical_y));
    }

    void present(const SceneView& view) {
        update_cursor(view);
        build_screen(view);
        present_image(screen, view, kScreenPlayfieldTop);
    }

    // SDL_RenderSetLogicalSize installs SDL_RendererEventWatch, which rewrites
    // every queued SDL_MOUSEMOTION/BUTTON coordinate from window points to the
    // logical size (SDL_render.c's logical_w branch: subtract the viewport and
    // divide by scale*dpi_scale). Those event fields are therefore ALREADY
    // logical and must not be scaled again. SDL_GetMouseState, however, returns
    // window coordinates; SDL_RenderWindowToLogical is its inverse and applies
    // dpi_scale (window points -> renderer output pixels) itself. Multiplying
    // the window point by output/window first would apply the DPI factor twice.
    void mouse_logical(float* logical_x, float* logical_y) const {
        int window_x = 0;
        int window_y = 0;
        SDL_GetMouseState(&window_x, &window_y);
        SDL_RenderWindowToLogical(renderer, window_x, window_y, logical_x, logical_y);
    }

    // Evidence for a real click: the event coordinate SDL delivered (already
    // logical) together with the window/output geometry that produced it.
    void log_mouse_click(float logical_x, float logical_y, const char* phase) const {
        int window_width = 0;
        int window_height = 0;
        int output_width = 0;
        int output_height = 0;
        SDL_GetWindowSize(window, &window_width, &window_height);
        SDL_GetRendererOutputSize(renderer, &output_width, &output_height);
        std::cout << "mouse_click phase=" << phase
                  << " logical=" << static_cast<int>(logical_x) << ','
                  << static_cast<int>(logical_y) << " window=" << window_width << 'x'
                  << window_height << " output=" << output_width << 'x'
                  << output_height << " logical_size=" << logical_width << 'x'
                  << logical_height << std::endl;
    }

    // 0x4177c7 (examine, font 1, 0x14a, 3 lines, left) and 0x417a17 (message,
    // font 3, 0x26e/0x2d, 3 lines, right-aligned). The original reveals the
    // examine text one line per second and blits a message panel behind the
    // right block; this draws the final block over the scene and says so.
    void draw_text_windows(const SceneView& view, bool verbose) {
        if (view.examine_text.active && examine_font) {
            byzantine::draw_text(surface, *examine_font, view.examine_text.text, 5,
                                 0x14a - kScreenPlayfieldTop, 3, 3);
            if (verbose) {
                std::cout << "text_draw window=examine x=5 y=" << (0x14a - kScreenPlayfieldTop)
                          << " width=" << byzantine::text_width(*examine_font,
                                                               view.examine_text.text)
                          << '\n';
            }
        }
        if (view.message_text.active && message_font) {
            const auto width = byzantine::text_width(*message_font, view.message_text.text);
            byzantine::draw_text(surface, *message_font, view.message_text.text, 0x26e,
                                 0x2d - kScreenPlayfieldTop, 3, 4);
            if (verbose) {
                std::cout << "text_draw window=message x=" << (0x26e - width)
                          << " y=" << (0x2d - kScreenPlayfieldTop)
                          << " width=" << width << '\n';
            }
        }
    }

    // Writes the composed scene (background + records) as PPM. The hotspot
    // outlines are a window-only overlay and are deliberately not in the file.
    // SDL_RenderReadPixels is not used: it reads at the window/render-target
    // resolution (the logical 640x400 is a scaled viewport), so the composed
    // surface is the only exact 640x400 image.
    void write_shot(const SceneView& view, const std::string& path) {
        build_screen(view);
        if (!byzantine::write_ppm(screen, path)) {
            throw std::runtime_error("cannot write " + path);
        }
        std::cout << "shot=" << path << '\n';
    }
};

// Sets what is on screen for the duration of a media/menu presenter and
// restores it on every exit path. One rule in Display::present_image then keeps
// the hotspot-outline diagnostic off anything that is not the scene.
struct ScreenContentScope {
    Display& display;
    ScreenContent previous;
    ScreenContent entered;
    bool logged_scope;
    ScreenContentScope(Display& target, ScreenContent next)
        : display(target), previous(target.content), entered(next),
          logged_scope(next == ScreenContent::clip || next == ScreenContent::transition) {
        display.content = next;
        if (logged_scope) {
            byzantine::diagnostics::write("screen_content", [&](auto& out) {
                out << "now=" << name(entered) << " previous=" << name(previous);
            });
        }
    }
    ~ScreenContentScope() {
        display.content = previous;
        if (logged_scope) {
            // The detail trace now shows exactly which presenter owned the
            // screen, so a stray outline or overlay is attributable.
            byzantine::diagnostics::write("screen_content", [&](auto& out) {
                out << "now=" << name(previous) << " previous=" << name(entered);
            });
        }
    }
    ScreenContentScope(const ScreenContentScope&) = delete;
    ScreenContentScope& operator=(const ScreenContentScope&) = delete;

    static const char* name(ScreenContent content) {
        switch (content) {
            case ScreenContent::conversation: return "conversation";
            case ScreenContent::clip: return "clip";
            case ScreenContent::menu: return "menu";
            case ScreenContent::transition: return "transition";
            default: return "scene";
        }
    }
};
// ---------------------------------------------------------------------------
// Session: the window, the audio device and the wait bookkeeping.
// ---------------------------------------------------------------------------
struct WaitStats {
    std::size_t frames = 0;
    std::size_t events = 0;
    std::size_t clicks_ignored = 0;
    bool overlay_toggled = false;
    bool injected = false;
};

struct Session {
    Options options;
    SceneView view;
    Display display;
    byzantine::AudioMixer audio;
    // SHOW.LCC/LCV/LVM from --engine: the scene list that turns a 0x73 index
    // into a scene name, and the per-scene local->global variable rows.
    std::optional<byzantine::VariableMetadata> metadata;
    // engine/<scene>ce, the text entries 0x412206 loads into DAT_00483640.
    // Reloaded on a scene switch from the target's --ce record.
    std::optional<byzantine::SceneTextTable> scene_text;
    // copy2hd/setupart/MESSAGES.TXT, 0-based lines (0x41d45a reads line id).
    // The conversation exit dialog uses id 9 and labels 0x4c..0x4f.
    std::vector<std::string> messages;
    // Set by a 0xfffe conversation exit: the scene session has ended. The user
    // must explicitly choose R to reload; nothing is reloaded silently.
    bool reload_available = false;
    // DAT_004603d0: entering an examine close-up snapshots the parent scene;
    // a 0x73 inside the close-up restores it instead of loading its operand.
    // The port stores the whole SceneView (script, state, camera, background)
    // and carries the examine scene's inventory edits back into it.
    SceneView return_view;
    bool return_armed = false;
    // The SHOW.LCC index of the active close-up scene. 0x408076's examine
    // branch + 0x438c80 reload that same scene when another item is examined.
    std::optional<std::uint16_t> closeup_index;
    // Set by the interactive branch: a scene entry may then wait on audio like
    // any other script. --shot stays deterministic and never waits.
    bool interactive = false;
    bool running = true;
    // Hover highlight: the destination the pointer is on and the exact state
    // captured before its select label ran. Leaving restores that snapshot, so
    // a highlight can never accumulate or survive a mouse-out. The label is the
    // map's own select (0x6e disable the destination, 0x6d enable its travel
    // object), which is what makes a single click travel.
    int hover_select_slot = -1;
    bool hover_highlight_active = false;
    byzantine::SceneState hover_highlight_base;
    byzantine::SceneTrace hover_highlight_trace;
    WaitStats wait;
    // While a conversation is running its 640x480 screen is what the window
    // shows, including during audio waits. run_conversation sets and clears it.
    const byzantine::Rgb16Surface* conversation_frame = nullptr;
    // The save this session was started or last loaded from (FUN_00425ab4).
    // The writer (FUN_00424dc0) preserves the blocks the port does not model
    // from this image and updates the ones it does.
    std::optional<byzantine::SaveGame> save_base;
    // Port-only dialogue history and "seen" keys. They never enter a save file.
    struct DialogueHistoryEntry {
        std::string speaker;
        std::string text;
        bool answer = false;
    };
    std::vector<DialogueHistoryEntry> dialogue_history;
    std::set<std::string> seen_dialogue;
    // Native menu bridge: the TERCICHLER screen's PORT tab asks the host to
    // open the port settings overlay; closing it returns to TERCICHLER.
    bool open_port_settings = false;
    bool reopen_prefs = false;
    int port_settings_group = 0;
};

bool backlog_overlay(Session& session);
bool write_autosave(Session& session);

std::filesystem::path seen_dialogue_path(const Options& options) {
    if (!options.config_path.empty()) {
        return options.config_path.parent_path() / "seen_dialogue.txt";
    }
    if (!options.backgrounds.empty()) {
        return options.backgrounds / "copy2hd" / "savegame" / "byzantine_seen.txt";
    }
    return {};
}

void load_seen_dialogue(Session& session) {
    const auto path = seen_dialogue_path(session.options);
    if (path.empty()) return;
    std::ifstream input(path);
    if (!input) return;
    std::string line;
    std::size_t count = 0;
    while (std::getline(input, line) && count < 200000) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        session.seen_dialogue.insert(line);
        ++count;
    }
    std::cout << "dialogue_seen file=" << path << " entries="
              << session.seen_dialogue.size() << '\n';
}

void save_seen_dialogue(const Session& session) {
    const auto path = seen_dialogue_path(session.options);
    if (path.empty()) return;
    std::error_code error;
    if (!path.parent_path().empty()) {
        std::filesystem::create_directories(path.parent_path(), error);
    }
    std::ofstream output(path, std::ios::trunc);
    if (!output) {
        std::cout << "dialogue_seen write_failed file=" << path << '\n';
        return;
    }
    for (const auto& key : session.seen_dialogue) output << key << '\n';
    std::cout << "dialogue_seen saved file=" << path << " entries="
              << session.seen_dialogue.size() << '\n';
}

void append_dialogue_history(Session& session, const std::string& speaker,
                             const std::string& text, bool answer) {
    if (text.empty()) return;
    session.dialogue_history.push_back({speaker, text, answer});
    constexpr std::size_t kMaxHistory = 400;
    if (session.dialogue_history.size() > kMaxHistory) {
        session.dialogue_history.erase(
            session.dialogue_history.begin(),
            session.dialogue_history.begin() +
                static_cast<std::ptrdiff_t>(session.dialogue_history.size() - kMaxHistory));
    }
}

// 0xbd playback (defined after the clip decoder, below). The script runners need
// them because the original blocks inside 0x413bdd.
bool play_cf_video(Session& session, const byzantine::SceneVideoRequest& request,
                   bool verbose);
bool play_trace_videos(Session& session, const byzantine::SceneTrace& trace,
                       bool verbose);

// Processed while a sound plays: the script is waiting, so a click must not
// start another label, but the window still redraws and H/Esc keep working.
int poll_logged_event(SDL_Event* event, const char* phase, Session& session) {
    session.display.poll_gamepad();
    const int found = SDL_PollEvent(event);
    if (!found) return found;
    byzantine::diagnostics::write("input", [&](auto& out) {
        out << "phase=" << phase << " scene=" << session.view.name
            << " type=" << event->type << " sdl_ms=" << event->common.timestamp;
        switch (event->type) {
        case SDL_MOUSEMOTION:
            out << " motion x=" << event->motion.x << " y=" << event->motion.y
                << " dx=" << event->motion.xrel << " dy=" << event->motion.yrel
                << " buttons=" << event->motion.state; break;
        case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
            out << " button=" << unsigned(event->button.button)
                << " x=" << event->button.x << " y=" << event->button.y
                << " clicks=" << unsigned(event->button.clicks); break;
        case SDL_MOUSEWHEEL:
            out << " wheel_x=" << event->wheel.x << " wheel_y=" << event->wheel.y; break;
        case SDL_KEYDOWN: case SDL_KEYUP:
            out << " key=" << SDL_GetKeyName(event->key.keysym.sym)
                << " code=" << event->key.keysym.sym << " modifiers=" << event->key.keysym.mod
                << " repeat=" << unsigned(event->key.repeat); break;
        case SDL_WINDOWEVENT:
            out << " window_event=" << unsigned(event->window.event)
                << " data=" << event->window.data1 << ',' << event->window.data2; break;
        default: break;
        }
    });
    return found;
}

void pump_window(Session& session) {
    session.display.tick_cursor();
    SDL_Event event;
    while (poll_logged_event(&event, "sound_wait", session)) {
        ++session.wait.events;
        if (event.type == SDL_QUIT) {
            session.running = false;
            return;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            // During a conversation ESC is 0x436b01's answer-list cancel. The
            // answer loop owns that transition; media/text waits must not quit
            // the whole player.
            if (session.conversation_frame != nullptr) continue;
            session.running = false;
            return;
        }
        if (event.type == SDL_KEYDOWN &&
            key_is(session.options.settings, session.options.settings.key_hotspots,
                   event.key.keysym.sym)) {
            session.options.overlay = !session.options.overlay;
            session.wait.overlay_toggled = true;
            float pointer_x = 0;
            float pointer_y = 0;
            session.display.mouse_logical(&pointer_x, &pointer_y);
            session.display.set_hover_overlay(session.view, static_cast<int>(pointer_x),
                                              static_cast<int>(pointer_y));
            continue;
        }
        if (event.type == SDL_KEYDOWN &&
            key_is(session.options.settings, session.options.settings.key_backlog,
                   event.key.keysym.sym)) {
            backlog_overlay(session);
            continue;
        }
        if (event.type == SDL_KEYDOWN &&
            key_is(session.options.settings, session.options.settings.key_skip,
                   event.key.keysym.sym)) {
            session.options.settings.skip_seen = !session.options.settings.skip_seen;
            std::cout << "dialogue_skip_seen enabled="
                      << (session.options.settings.skip_seen ? 1 : 0) << '\n';
            continue;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            ++session.wait.clicks_ignored;
            byzantine::diagnostics::write("input_ignored", [&](auto& out) {
                out << "reason=sound_wait scene=" << session.view.name;
            });
        }
    }
    if (session.conversation_frame != nullptr) {
        session.display.present_image(*session.conversation_frame, session.view,
                                      kScreenPlayfieldTop, "conversation");
    } else {
        session.display.present(session.view);
    }
    ++session.wait.frames;
}

// Ends a --selftest-events run so the counters print and the process exits
// instead of entering the interactive loop.
void push_quit() {
    SDL_Event quit{};
    quit.type = SDL_QUIT;
    SDL_PushEvent(&quit);
}

// Resolves and plays events [from, to). Returns false when the user quit.
bool play_events(Session& session, const SceneView& view, const std::vector<SoundEvent>& events,
                 std::size_t from, std::size_t to, bool verbose) {
    const auto resolve = [&view](std::uint16_t id) -> std::optional<std::string> {
        if (!view.state.sound_table_known || id >= view.state.sound_records.size()) {
            return std::nullopt;
        }
        const auto& record = view.state.sound_records[id];
        if (record.name.empty() || record.name == "endgame") return std::nullopt;
        return record.name;
    };
    for (std::size_t index = from; index < to; ++index) {
        const auto& event = events[index];
        if (event.kind == SoundEvent::Kind::stop) {
            session.audio.stop(event.id);
            if (verbose) {
                std::cout << "ambient_stop step=" << event.step << " id=" << event.id
                          << " " << session.audio.describe() << '\n';
            }
            continue;
        }
        if (event.kind == SoundEvent::Kind::release) {
            // 0x427a01: only a loop is queued on the persisted chain; the voice
            // is stopped either way.
            session.audio.stop(event.id);
            if (verbose) {
                std::cout << "ambient_release step=" << event.step << " id=" << event.id
                          << " " << session.audio.describe() << '\n';
            }
            continue;
        }
        if (event.kind == SoundEvent::Kind::stop_all) {
            session.audio.stop_all();
            if (verbose) {
                std::cout << "ambient_stop_all step=" << event.step << " "
                          << session.audio.describe() << '\n';
            }
            continue;
        }
        if (event.allowed && !*event.allowed) {
            if (verbose) {
                std::cout << "audio step=" << event.step << " id=" << event.id
                          << " skipped: gamepref gate is off\n";
            }
            continue;
        }
        const auto name = resolve(event.id);
        if (!name) {
            if (verbose) {
                std::cout << "audio step=" << event.step << " id=" << event.id
                          << " skipped: no name in the per-scene sound table (--cs)\n";
            }
            continue;
        }
        std::optional<std::filesystem::path> file;
        if (session.options.language.voice != nullptr) {
            file = sound_path(session.options.language.voice->root, *name, view.name);
        }
        if (!file) file = sound_path(session.options.sounds, *name, view.name);
        if (!file) {
            if (verbose) {
                std::cout << "audio step=" << event.step << " id=" << event.id
                          << " name=" << *name << " skipped: no such WAV under "
                          << session.options.sounds.string() << '\n';
            }
            continue;
        }
        byzantine::WavClip clip;
        try {
            clip = byzantine::parse_wav(read_file(*file), file->string());
        } catch (const std::exception& error) {
            std::cout << "audio " << error.what() << '\n';
            continue;
        }
        if (verbose) {
            std::cout << "audio step=" << event.step << " instruction=" << event.instruction
                      << " opcode=0x" << std::hex << static_cast<unsigned>(event.opcode)
                      << std::dec << " id=" << event.id << " name=" << *name
                      << " file=" << file->filename().string()
                      << " duration_ms=" << clip.duration_ms
                      << " loop=" << (event.loop ? 1 : 0)
                      << " blocking=" << (event.blocking ? 1 : 0);
            if (event.cap_ms) std::cout << " session_cap_ms=" << event.cap_ms;
            std::cout << std::flush;
        }
        if (!session.audio.ready()) {
            if (verbose) std::cout << " skipped: no audio device\n";
            continue;
        }
        try {
            const auto bus = event.loop || event.opcode == 0xc4
                                 ? byzantine::VoiceBus::ambient
                                 : (event.opcode == 0xd1 ? byzantine::VoiceBus::speech
                                                         : byzantine::VoiceBus::effects);
            if (!session.audio.start(event.id, clip, event.loop, bus)) {
                if (verbose) std::cout << " skipped: mixer rejected the clip\n";
                continue;
            }
        } catch (const std::exception& error) {
            std::cout << "audio " << error.what() << '\n';
            continue;
        }
        if (!event.blocking) {
            // 0xc4 is an ambient activation: the script continues immediately.
            if (verbose) {
                std::cout << " (ambient started) " << session.audio.describe() << '\n';
            }
            continue;
        }
        const auto before = std::chrono::steady_clock::now();
        const std::uint64_t limit =
            event.cap_ms == 0 ? clip.duration_ms + 50 : event.cap_ms;
        while (session.running && session.audio.playing(event.id)) {
            pump_window(session);
            if (!session.running) break;
            const auto waited = static_cast<std::uint64_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - before)
                    .count());
            if (waited >= limit) break;
            SDL_Delay(4);
        }
        const auto waited = static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - before)
                .count());
        if (verbose) {
            std::cout << " waited_ms=" << waited
                      << " frames=" << session.wait.frames
                      << " events=" << session.wait.events
                      << (session.running ? " (script continues)\n" : " (aborted)\n");
        }
        if (!session.running) return false;
    }
    return true;
}

// Runs a label (or a scene entry) so that each queued sound is played at the
// instruction that queued it: the script is replayed with a step budget up to
// that step, the frame is presented, the clip plays while the window stays
// alive, and the replay continues with the next budget. The last budget is the
// script's own step count, so the final state equals an unbudgeted run. A 0xbd
// CF video gets its own budget for the same reason: the original blocks inside
// 0x413bdd, so the clip must play before the following instructions commit.
bool run_label_gated(Session& session, EntryPoint point,
                     const byzantine::SceneState& entry,
                     const byzantine::SceneTrace& probe, bool verbose) {
    const auto events = sound_events(probe);
    std::vector<std::size_t> budgets;
    for (const auto& event : events) {
        if (event.step != 0) budgets.push_back(event.step);
    }
    for (const auto& video : probe.video_requests) {
        if (video.step != 0) budgets.push_back(video.step);
    }
    budgets.push_back(probe.steps);
    std::sort(budgets.begin(), budgets.end());
    budgets.erase(std::unique(budgets.begin(), budgets.end()), budgets.end());

    std::size_t played = 0;
    std::size_t played_videos = 0;
    std::size_t applied_panorama_loads = 0;
    for (const auto budget : budgets) {
        auto trace = trace_point(session.view, point, entry, budget);
        set_trace(session.view, std::move(trace));
        const auto all = sound_events(session.view.trace);
        const auto& videos = session.view.trace.video_requests;
        if (all.size() <= played && videos.size() <= played_videos) continue;
        if (session.view.trace.panorama_loads.size() > applied_panorama_loads) {
            apply_panorama_events(session.view, session.options, verbose);
            applied_panorama_loads = session.view.trace.panorama_loads.size();
        }
        // A new scene has no selected image until its script actually loads one.
        // Keep the previous presented frame while early audio events run.
        if (session.view.layer != SceneLayer::none &&
            (!panorama_layer(session.view) || session.view.panorama)) {
            refresh_background(session.options, session.view);
            session.display.compose(session.view, verbose);
            session.display.present(session.view);
        } else if (verbose) {
            std::cout << "gate_frame retained (scene layer not ready)\n";
        }
        if (all.size() > played) {
            if (verbose) {
                std::cout << "gate stopped_after_step=" << session.view.trace.steps
                          << " of=" << probe.steps << " (sound queued here)\n";
            }
            if (!play_events(session, session.view, all, played, all.size(), verbose)) {
                return false;
            }
            played = all.size();
        }
        for (; played_videos < videos.size(); ++played_videos) {
            play_cf_video(session, videos[played_videos], verbose);
            if (!session.running) return false;
        }
    }
    return session.running;
}

// ---------------------------------------------------------------------------
// Clicks
// ---------------------------------------------------------------------------
// One click/label/entry plan. `entry_before` is the state the probe started
// from; a plan is always re-executed from it, never from the probe's output.
struct ClickPlan {
    bool ok = false;
    bool travel = false;
    bool conversation = false;
    bool item_use_checked = false;   // the strip had a selected item
    bool item_use_ok = false;        // 0x407c5a accepted it
    std::uint16_t switch_index = 0;
    std::string message;
    std::uint16_t action = 0;
    EntryPoint point;                    // where the action's segment starts
    byzantine::SceneState entry;
    byzantine::SceneState entry_before;  // before the probe (gated replay input)
    byzantine::SceneTrace probe;
};

bool execute_plan(Session& session, const ClickPlan& plan,
                  const std::function<bool(EntryPoint, const byzantine::SceneState&,
                                           const byzantine::SceneTrace&)>& run,
                  bool verbose, bool* session_ended = nullptr);

// A scene with no active hotspot and no exit cannot be left: every click is
// rejected by hotspot_at. That state is legitimate while a 0x70 dialogue runs
// (0xb8 suspends the hotspots and the continuation restores them), so report it
// instead of leaving the player guessing whether the click was wrong.
void warn_if_nothing_clickable(const SceneView& view, const char* where) {
    if (!view.trace.active_hotspots.empty()) return;
    std::cout << where << "_no_hotspots scene=" << view.name
              << " (nothing is clickable; stopped_at=" << view.trace.stopped_at
              << " blocker=" << byzantine::trace_blocker(view.trace) << ')' << '\n';
}

// ---------------------------------------------------------------------------
// Scene travel (0x73)
// ---------------------------------------------------------------------------
bool audio_gated(const Session& session) {
    return session.audio.ready() && !session.options.sounds.empty() &&
           session.view.state.sound_table_known;
}

// 0x425ab4 / 0x439349 restore DAT_00486220 including +0x14 (loop) and +0x34
// (active). The original's saved loop chain (DAT_0046019c) is not modelled, but
// every active looping record of the arrival state is a sound the original
// keeps playing, so the mixer starts them here.
void resume_ambient(Session& session, bool verbose) {
    if (!audio_gated(session)) return;
    std::size_t started = 0;
    std::size_t active_loops = 0;
    for (const auto& [id, sound] : session.view.state.sounds) {
        if (!sound.defined || !sound.active || sound.field14 == 0) continue;
        ++active_loops;
        if (session.audio.playing(id)) continue;
        if (id >= session.view.state.sound_records.size()) continue;
        const auto& name = session.view.state.sound_records[id].name;
        if (name.empty() || name == "endgame") continue;
        std::optional<std::filesystem::path> file;
        if (session.options.language.voice != nullptr) {
            file = sound_path(session.options.language.voice->root, name, session.view.name);
        }
        if (!file) file = sound_path(session.options.sounds, name, session.view.name);
        if (!file) {
            if (verbose) {
                std::cout << "ambient_resume id=" << id << " name=" << name
                          << " skipped: no such WAV under "
                          << session.options.sounds.string() << '\n';
            }
            continue;
        }
        try {
            const auto clip = byzantine::parse_wav(read_file(*file), file->string());
            if (session.audio.start(id, clip, true, byzantine::VoiceBus::ambient)) {
                ++started;
                if (verbose) {
                    std::cout << "ambient_resume id=" << id << " name=" << name
                              << " file=" << file->filename().string()
                              << " duration_ms=" << clip.duration_ms << " loop=1\n";
                }
            }
        } catch (const std::exception& error) {
            std::cout << "ambient_resume " << error.what() << '\n';
        }
    }
    if (verbose) {
        std::cout << "ambient_resume loops=" << active_loops
                  << " started=" << started << '\n';
    }
}

// Compact stop reason for the new scene's entry line: the shared blocker
// classification (so a partial entry can never look like a clean one).
std::string stop_reason(const byzantine::SceneTrace& trace) {
    const auto blocker = byzantine::trace_blocker(trace);
    if (!blocker.empty()) return " stopped: " + blocker;
    if (trace.script_end_requested) return " script_end";
    return {};
}

std::size_t text_lines(const std::string& text) {
    return 1 + static_cast<std::size_t>(std::count(text.begin(), text.end(), '\n'));
}

// 0x412206: engine/<scene>ce becomes the DAT_00483640 text table.
void load_scene_text(Session& session, bool verbose) {
    session.scene_text.reset();
    session.view.examine_text = {};
    session.view.message_text = {};
    std::optional<std::filesystem::path> path;
    // The text pack's split CE record is the text-language source; the
    // --ce path from the launcher is the base fallback.
    const auto scene = upper_ascii(session.view.name);
    for (const auto& relative : {std::string("ce/") + scene,
                                 std::string("copy2hd/engine/") + scene + "ce"}) {
        if (const auto file = session.options.language.find_text(relative)) {
            path = *file;
            break;
        }
    }
    if (!path) {
        const auto found = session.options.scene.find("--ce");
        if (found == session.options.scene.end()) return;
        path = found->second;
    }
    try {
        session.scene_text = byzantine::parse_scene_text(read_file(*path));
        if (verbose) {
            std::cout << "scene_text file=" << *path
                      << " entries=" << session.scene_text->entries.size()
                      << " text_lang=" << session.options.language.text_code_or_base()
                      << '\n';
        }
    } catch (const std::exception& error) {
        std::cout << "scene_text " << *path << ": " << error.what() << '\n';
    }
}

// 0x41d45a reads setupart/MESSAGES.TXT line `id` (0-based) into the dialog
// text. The file is CP1254; the font drawer uses the game byte encoding.
void load_messages(Session& session, bool verbose) {
    session.messages.clear();
    std::optional<std::filesystem::path> path;
    for (const auto& relative : {std::string("setupart/MESSAGES.TXT"),
                                 std::string("copy2hd/setupart/MESSAGES.TXT")}) {
        if (const auto file = session.options.language.find_text(relative)) {
            path = *file;
            break;
        }
    }
    if (!path && !session.options.messages.empty()) path = session.options.messages;
    if (!path) return;
    try {
        const auto bytes = read_file(*path);
        std::string line;
        const auto add = [&]() {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            session.messages.push_back(decode_messages_line(session.options, line));
            line.clear();
        };
        for (const auto byte : bytes) {
            if (byte == '\n') add();
            else line.push_back(static_cast<char>(byte));
        }
        if (!line.empty()) add();
        if (verbose) {
            std::cout << "messages file=" << *path
                      << " lines=" << session.messages.size()
                      << " text_lang=" << session.options.language.text_code_or_base()
                      << " encoding=" << session.options.language.messages_encoding()
                      << " english_font="
                      << (session.options.language.english_font_branch() ? 1 : 0)
                      << '\n';
        }
    } catch (const std::exception& error) {
        std::cout << "messages " << *path << ": " << error.what()
                  << " (conversation exit dialog uses fallback labels)\n";
    }
}

// 0x40c329 sets DAT_004603d4 from the clicked object's +0x14 and 0x4177c7 shows
// engine/<scene>ce[+0x14] in the 3-line scroll window.
void show_object_text(Session& session, std::uint16_t slot, bool verbose) {
    const auto object = session.view.state.objects.find(slot);
    if (object == session.view.state.objects.end()) return;
    const auto index = object->second.argument;
    if (index == 0xffffffffu) {
        // 0x40c329 clears DAT_004603d4 (0x4177c7(0)) before it reads +0x14, so a
        // click on an object without a text index blanks the window.
        const bool had = session.view.examine_text.active;
        session.view.examine_text = {};
        if (verbose && had) std::cout << "text object=" << slot << " cleared\n";
        return;
    }
    if (!session.scene_text) {
        std::cout << "text object=" << slot << " index=" << index
                  << " skipped: no --ce text table\n";
        return;
    }
    if (index >= session.scene_text->entries.size()) {
        std::cout << "text object=" << slot << " index=" << index
                  << " skipped: outside the " << session.scene_text->entries.size()
                  << "-entry table\n";
        return;
    }
    session.view.examine_text = {true, session.scene_text->entries[index]};
    if (verbose) {
        std::cout << "text object=" << slot << " index=" << index
                  << " lines=" << text_lines(session.view.examine_text.text) << '\n';
    }
}

// 0xcd -> 0x417a17 shows DAT_00483640[id] right-aligned at (0x26e,0x2d).
void show_message_text(Session& session, bool verbose) {
    if (session.view.trace.text_requests.empty()) return;
    const auto& request = session.view.trace.text_requests.back();
    if (!session.scene_text) {
        std::cout << "message id=" << request.id << " mode=" << request.mode
                  << " skipped: no --ce text table\n";
        return;
    }
    if (request.id >= session.scene_text->entries.size()) {
        std::cout << "message id=" << request.id << " mode=" << request.mode
                  << " skipped: outside the " << session.scene_text->entries.size()
                  << "-entry table\n";
        return;
    }
    session.view.message_text = {true, session.scene_text->entries[request.id]};
    if (verbose) {
        std::cout << "message id=" << request.id << " mode=" << request.mode
                  << " lines=" << text_lines(session.view.message_text.text)
                  << " bytes=" << session.view.message_text.text.size() << '\n';
    }
}

// 0x73 writes DAT_00482428 and returns 0; 0x42c8a0's scene loop then reloads
// that scene and 0x42d594 runs it from instruction 0. Every per-scene table is
// freed on each load (0x40f3f0 clears objects/sounds/dialogue chains/media,
// 0x40fd7b the DAT_004649e8 records), and DAT_0047e2f0 is cleared only ONCE
// before the loop, so the carried variable pool is the state that crosses.
// The inventory strip is process-global in the original (DAT_00468cc0/
// DAT_00468d78/DAT_00486f90); it is not reloaded from the save on a 0x73.
// Keep the current page, stack, item states and examine selection across a
// scene switch.
void carry_inventory(const byzantine::SceneState& from, byzantine::SceneState& to) {
    to.dialogue_channels = from.dialogue_channels;
    to.dialogue_records = from.dialogue_records;
    to.dialogue_visible = from.dialogue_visible;
    to.dialogue_scroll = from.dialogue_scroll;
    to.current_dialogue_channel = from.current_dialogue_channel;
    to.current_dialogue_record = from.current_dialogue_record;
    to.current_use_item = from.current_use_item;
}

// 0x426935's resume: a save load does not replay the scene entry. The script
// pointer is 0x432464's first 0x6f node (the end), and the visible layer comes
// from the save's mid-block:
//   DAT_0045c060 == 0  -> DAT_0048253e indexes the scene's CB background table
//                         (0x431ec2 -> 0x41045b)
//   DAT_0045c060 != 0  -> DAT_00480330 indexes CO.ALL and DAT_0045c064 is the
//                         staged panorama x (0x432295 -> 0x40186e)
void restore_saved_layer(Session& session, const byzantine::SaveGame& save,
                         bool verbose) {
    auto& view = session.view;
    auto& options = session.options;
    if (save.field_45c060 != 0) {
        const auto record = static_cast<std::size_t>(save.field_480330);
        if (record >= view.state.panorama_records.size()) {
            std::cout << "load_layer panorama record=" << record
                      << " skipped: outside the "
                      << view.state.panorama_records.size() << "-entry CO table\n";
            return;
        }
        const auto& entry = view.state.panorama_records[record];
        const auto file = node_path(options, entry.name);
        if (file.empty()) {
            std::cout << "load_layer panorama record=" << record
                      << " name=" << entry.name << " missing under "
                      << options.backgrounds << '\n';
            return;
        }
        try {
            const auto image = byzantine::decode_jpeg_file(file.string());
            if (view.panorama_size &&
                *view.panorama_size != std::pair<int, int>{image.width, image.height}) {
                std::cout << "load_layer panorama name=" << entry.name << " size="
                          << image.width << 'x' << image.height
                          << " differs from the renderer's " << view.panorama_size->first
                          << 'x' << view.panorama_size->second << " (0x401256 rejects it)\n";
                return;
            }
            view.panorama_size = std::pair<int, int>{image.width, image.height};
            const auto key = "nodes/" + lower_ascii(entry.name.substr(0, 2)) + "/" +
                             file.filename().string();
            view.panorama.emplace(key, image.width, image.height,
                                  byzantine::rgb_to_rgb565(image), kSceneWidth,
                                  kSceneHeight);
            view.layer = SceneLayer::panorama;
            int staged = static_cast<int>(save.field_45c064);
            if (staged < -1 || staged > 2495) staged = -1;
            view.panorama->apply_staged_x(staged, false);
            if (verbose) {
                std::cout << "load_layer panorama record=" << record
                          << " name=" << entry.name << " staged_x=" << staged
                          << " yaw=" << view.panorama->yaw_degrees() << '\n';
            }
        } catch (const std::exception& error) {
            std::cout << "load_layer panorama " << file.string() << ": "
                      << error.what() << '\n';
        }
        return;
    }
    const auto id = static_cast<std::size_t>(save.field_48253e);
    if (save.field_48253e != 0xffff && id < view.entries.size() &&
        !view.entries[id].name.empty()) {
        view.committed_background = view.entries[id].name;
        view.layer = SceneLayer::background;
        if (verbose) {
            std::cout << "load_layer background=" << view.committed_background
                      << " cb_index=" << id << '\n';
        }
    } else {
        view.layer = SceneLayer::none;
        if (verbose) {
            std::cout << "load_layer background=none cb_index=" << save.field_48253e
                      << '\n';
        }
    }
}

// 0x41d7ff loads the per-disc animation overlay the launcher prepared. A save
// from another disc needs that disc's `anims/` tree, because the same file name
// can carry different art (TKNUXA is the known case). The launcher's Python
// helper already owns the cache identity; call it instead of duplicating the
// hashing, and report a failure instead of silently mixing discs.
std::optional<std::filesystem::path> disc_iso(const Options& options, int disc) {
    if (options.media_isos.empty()) return std::nullopt;
    std::ifstream sources(options.media_isos);
    if (!sources) return std::nullopt;
    const auto wanted = "byzantine_" + std::to_string(disc) + ".iso";
    std::optional<std::filesystem::path> fallback;
    std::string line;
    while (std::getline(sources, line)) {
        if (line.empty()) continue;
        const auto path = std::filesystem::path(line);
        if (!std::filesystem::is_regular_file(path)) continue;
        const auto name = lower_ascii(path.filename().string());
        if (name == wanted) return path;
        if (!fallback &&
            (name.find("_cd" + std::to_string(disc)) != std::string::npos ||
             name.find("_" + std::to_string(disc) + ".") != std::string::npos)) {
            fallback = path;
        }
    }
    return fallback;
}

std::optional<std::filesystem::path> prepare_disc_overlay(const Options& options,
                                                          int disc) {
    if (options.backgrounds.empty()) return std::nullopt;
    const auto iso = disc_iso(options, disc);
    if (!iso) {
        std::cout << "anims_overlay disc=" << disc
                  << " skipped: no matching ISO in " << options.media_isos << '\n';
        return std::nullopt;
    }
    std::vector<std::filesystem::path> scripts;
    std::error_code error;
    const auto self = std::filesystem::read_symlink("/proc/self/exe", error);
    if (!error && !self.empty()) {
        scripts.push_back(self.parent_path() / "prepare_disc_anims.py");
    }
    scripts.push_back(std::filesystem::current_path() / "port" /
                      "prepare_disc_anims.py");
    std::optional<std::filesystem::path> script;
    for (const auto& candidate : scripts) {
        if (std::filesystem::is_regular_file(candidate, error)) {
            script = candidate;
            break;
        }
    }
    if (!script) {
        std::cout << "anims_overlay disc=" << disc
                  << " skipped: prepare_disc_anims.py not found beside the player\n";
        return std::nullopt;
    }
    const auto command = "python3 " + shell_quote(script->string()) + " " +
                         shell_quote(options.backgrounds.string()) + " " +
                         shell_quote(iso->string()) + " " + std::to_string(disc) +
                         " 2>/dev/null";
    FILE* pipe = popen(command.c_str(), "r");
    if (pipe == nullptr) {
        std::cout << "anims_overlay disc=" << disc << " failed: popen\n";
        return std::nullopt;
    }
    std::string output;
    std::array<char, 512> buffer{};
    while (std::fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        output += buffer.data();
    }
    const auto status = pclose(pipe);
    while (!output.empty() && (output.back() == '\n' || output.back() == '\r')) {
        output.pop_back();
    }
    const auto target = std::filesystem::path(output);
    if (status != 0 || output.empty() ||
        !std::filesystem::is_regular_file(target / "ready.json", error)) {
        std::cout << "anims_overlay disc=" << disc << " failed status=" << status
                  << " output=" << output << '\n';
        return std::nullopt;
    }
    std::cout << "anims_overlay disc=" << disc << " iso=" << iso->filename().string()
              << " path=" << target << '\n';
    return target;
}

int overlay_disc(const Options& options) {
    const auto overlay = options.scene.find("--anims-overlay");
    if (overlay == options.scene.end()) return 0;
    const auto folder = std::filesystem::path(overlay->second).filename().string();
    const auto at = folder.find("cd");
    if (at == std::string::npos) return 0;
    int disc = 0;
    std::size_t index = at + 2;
    while (index < folder.size() &&
           std::isdigit(static_cast<unsigned char>(folder[index]))) {
        disc = disc * 10 + (folder[index] - '0');
        ++index;
    }
    return disc;
}

// The engine transition code a scene arrival carries. 0x69 backgrounds and
// 0xb9 nodes both go through the blend family (`0x431ec2` and `0x40186e`), so
// the destination's own load supplies the code: `0x432133` of its operand, or
// the DAT_00460664 a preceding 0xbd left, or 100 for "draw at once".
//
// 100 is common in the shipped entries (MR, MAP and EA all load with it), so a
// faithful run would still cut almost everywhere. The port therefore uses its
// own code when the data says 100 and reports source=default/forced, never
// source=background/panorama. `--transition 0x64` forces the 100 the scripts
// carry and reproduces the shipped behaviour exactly; `--no-transitions`
// disables the blends altogether.
std::uint16_t arrival_transition_code(const SceneView& view, const Options& options,
                                      const char** source) {
    if (!view.trace.panorama_loads.empty()) {
        const auto code = view.trace.panorama_loads.back().transition;
        if (code != 100) {
            *source = "panorama";
            return code;
        }
    } else if (view.trace.background && view.trace.background->transition != 100) {
        *source = "background";
        return view.trace.background->transition;
    }
    *source = options.transition_forced ? "forced" : "default";
    return options.travel_transition;
}

// Composes the arrival frame and presents it through 0x40186e's transition
// family when the scene carries a code. Every arriving path (0x73 travel, the
// close-up return, a scene reload) goes through this one presenter.
void present_arrival(Session& session, bool verbose) {
    session.display.compose(session.view, verbose);
    const char* source = "none";
    const auto code = arrival_transition_code(session.view, session.options, &source);
    session.display.present_transition(session.view, code, source);
}

bool travel_to(Session& session, std::uint16_t index, bool verbose,
               const std::string& save_path = {},
               const byzantine::SaveGame* loaded_save = nullptr) {
    auto& options = session.options;
    if (!save_path.empty()) {
        // A load starts a new session: the close-up parent snapshot and the
        // 0x439349 restore belong to the previous scene.
        session.return_armed = false;
        session.return_view = SceneView{};
        session.closeup_index.reset();
    }
    std::optional<byzantine::SaveGame> parsed_save;
    if (!save_path.empty() && loaded_save == nullptr) {
        parsed_save = byzantine::parse_save(read_file(save_path));
        loaded_save = &*parsed_save;
    }
    if (session.return_armed && !session.return_view.name.empty()) {
        // 0x42db22 case 0x73: when DAT_004603d0 is non-NULL, the operand is
        // ignored and the close-up returns to its parent scene. 0x439349 then
        // restores the captured subsystem state. The inventory strip is not in
        // that snapshot, so keep the examine scene's item state.
        const auto from = session.view.name;
        const auto current = session.view.state;
        session.view = std::move(session.return_view);
        session.return_view = SceneView{};
        session.return_armed = false;
        session.closeup_index.reset();
        carry_inventory(current, session.view.state);
        // 0x439349 calls 0x4085e7: DAT_0045d1cc is cleared and the examined
        // record returns to state 1. The strip's own selection (DAT_0045d1c8)
        // is process-global and stays, but no item is "being examined" in the
        // parent any more.
        if (current.current_use_item) {
            auto& record =
                session.view.state.dialogue_records[*current.current_use_item];
            if (record.state == 4) record.state = 1;
            session.view.state.current_use_item.reset();
        }
        session.view.state.random_state = current.random_state;
        resume_ambient(session, verbose);
        refresh_background(options, session.view);
        present_arrival(session, verbose);
        std::cout << "closeup_return scene=" << session.view.name << " from=" << from
                  << " requested_index=" << index
                  << " hotspots=" << session.view.trace.active_hotspots.size()
                  << " bar=" << (session.view.state.dialogue_visible ? "visible" : "hidden")
                  << " ambient=" << session.audio.ambient() << '\n';
        return session.running;
    }
    // 0x40f3f0 frees the per-scene sound table and its handles on a scene load;
    // the target entry starts its own ambience. The original's saved loop chain
    // (DAT_0046019c) is not modelled, so no loop crosses the 0x73.
    if (session.audio.ready() && session.audio.ambient() != 0) {
        const auto voices = session.audio.ambient();
        session.audio.stop_all();
        std::cout << "ambient_scene_stop from=" << session.view.name
                  << " voices=" << voices << '\n';
    }
    if (!session.metadata) {
        std::cout << "travel index=" << index
                  << " skipped: --engine (SHOW.LCC) is required to resolve a scene index\n";
        return false;
    }
    const auto& scenes = session.metadata->scenes;
    if (index >= scenes.size()) {
        std::cout << "travel index=" << index << " skipped: outside the "
                  << scenes.size() << "-entry SHOW.LCC list\n";
        return false;
    }
    const auto target = scenes[index].scene;
    const auto cc_file = sibling_scene_file(session.view.cc_path, target);
    const auto cb_file = sibling_scene_file(session.view.cb_path, target);
    if (!cc_file || !cb_file) {
        std::cout << "travel index=" << index << " scene=" << target
                  << " skipped: no scene script beside " << session.view.cc_path << '\n';
        return false;
    }
    auto next = options.scene;
    if (save_path.empty()) next.erase("--save");
    else next["--save"] = save_path;
    if (loaded_save != nullptr) {
        const auto current = overlay_disc(options);
        if (current != 0 && current != static_cast<int>(loaded_save->cd_number)) {
            std::cout << "anims_overlay load cd=" << loaded_save->cd_number
                      << " current=" << current << '\n';
            if (const auto overlay = prepare_disc_overlay(options,
                                                          static_cast<int>(
                                                              loaded_save->cd_number))) {
                next["--anims-overlay"] = overlay->string();
                options.scene["--anims-overlay"] = overlay->string();
            } else {
                std::cout << "anims_overlay load cd=" << loaded_save->cd_number
                          << " kept current overlay=" << current
                          << " (a different disc may use different animation art)\n";
            }
        }
    }
    std::string missing;
    for (const char* key : {"--ci", "--cy", "--cf", "--cs", "--ca", "--co", "--ce", "--cd"}) {
        const auto found = next.find(key);
        if (found == next.end()) continue;
        const auto file = sibling_scene_file(found->second, target);
        if (!file) {
            if (!missing.empty()) missing += ',';
            missing += key + 2;
            next.erase(found);
            continue;
        }
        found->second = file->string();
    }

    const auto& from_layout =
        byzantine::scene_variable_layout(*session.metadata, session.view.name);
    const auto& to_layout = byzantine::scene_variable_layout(*session.metadata, target);
    const auto from = session.view.name;
    // Bracket the scene load in the detail trace: the frames presented between
    // these markers are what the window shows while the next scene is being
    // read/decoded (a reported flash of an unexpected picture comes from here).
    byzantine::diagnostics::write("travel_begin", [&](auto& out) {
        out << "index=" << index << " from=" << from << " target=" << target;
    });
    // The departing scene stays presented while the target is read/decoded;
    // capture those frames and the target's first ones.
    session.display.arm_present_capture(800, "travel_begin");
    const auto from_locals = locals_digest(session.view.state);
    std::vector<std::uint16_t> pool;
    std::string written;
    if (save_path.empty()) {
        pool = byzantine::export_variable_pool(session.view.state, from_layout);
        if (session.view.pool_on_arrival.size() == pool.size()) {
            std::size_t changed = 0;
            for (std::size_t at = 0; at < pool.size(); ++at) {
                if (pool[at] == session.view.pool_on_arrival[at]) continue;
                ++changed;
                if (changed <= 8) {
                    written += (written.empty() ? "" : ",") + std::to_string(at);
                }
            }
            written = std::to_string(changed) + (written.empty() ? "" : " (" + written + ")");
        } else {
            written = "unknown";
        }
    }

    SceneView next_view;
    next_view.cc_path = cc_file->string();
    next_view.cb_path = cb_file->string();
    next_view.script = byzantine::parse_cc(read_file(next_view.cc_path));
    next_view.entries = byzantine::parse_cb(read_file(next_view.cb_path));
    next_view.name = std::filesystem::path(next_view.cc_path).stem().string();
    auto setup = byzantine::load_scene_setup(next_view.cc_path, next);
    if (save_path.empty()) {
        byzantine::apply_variable_pool(setup.state, pool, to_layout);
        carry_inventory(session.view.state, setup.state);
    } else {
        // The save already carried its variable pool and inventory strip
        // (0x425ab4's blocks 8..12 and the 0x424dc0 pool); do not overwrite
        // them with the departing scene's values.
        pool = byzantine::export_variable_pool(setup.state, to_layout);
        written = "save";
    }
    // 0x40fd7b: the scene load pre-loads the CA type 2/3 records before the
    // script runs. A save load already replaced the node table (0x425ab4 frees
    // it first), so its records must not be preloaded again.
    byzantine::SceneRecordPreload preload;
    if (save_path.empty()) preload = byzantine::preload_scene_records(setup.state);
    next_view.entry_state = setup.state;
    // DAT_00462ef8 is process-global and is not reset by a scene load.
    next_view.entry_state.random_state = session.view.state.random_state;
    // DAT_00460664 is process-global too: a 0xbd in the departing scene (a site
    // video) hands its transition code to the destination's first 0xb9, so it
    // must cross the 0x73 like the RNG state does.
    next_view.entry_state.pending_panorama_transition =
        session.view.state.pending_panorama_transition;
    next_view.state = next_view.entry_state;
    next_view.pool_on_arrival = pool;
    // 0x401256 fixed the node size once at startup; the current node stays in
    // DAT_00464880 until the new scene's own 0x69/0xb9 replaces it, but the
    // visible layer restarts from `none` so the node cannot hide this scene's
    // background.
    next_view.panorama_size = session.view.panorama_size;
    next_view.panorama = std::move(session.view.panorama);
    next_view.layer = SceneLayer::none;
    // 0x42c8a0's scene reload starts the target at its instruction head.
    next_view.reload_point = SceneReloadPoint{0, true};

    // 0x42c8a0 -> 0x41316f -> 0x40fbf6 -> 0x40f98c frees every conversation
    // state on a scene load; next_view.state is built fresh, so this map cannot
    // cross the 0x73.
    std::cout << "conversation_memory reset=scene_load previous_entries="
              << session.view.state.conversation_memory.size() << '\n';
    std::cout << "travel index=" << index << " scene=" << target << " from=" << from
              << " pool=3000 locals=300"
              << " pool_changed=" << written
              << " from_locals_digest=0x" << std::hex << from_locals
              << " pool_digest=0x" << pool_digest(pool)
              << " locals_digest=0x" << locals_digest(setup.state) << std::dec;
    if (!save_path.empty()) std::cout << " save=" << save_path;
    if (preload.created != 0 || preload.failed != 0) {
        std::cout << " record_preload=" << preload.created;
        if (preload.failed != 0) std::cout << " record_preload_failed=" << preload.failed;
    }
    if (!missing.empty()) std::cout << " missing_tables=" << missing;
    std::cout << '\n';

    session.view = std::move(next_view);
    session.display.hover_overlay_slot = -1;
    session.hover_select_slot = -1;
    session.hover_highlight_active = false;
    session.hover_highlight_base = {};
    session.hover_highlight_trace = {};
    byzantine::diagnostics::write("travel_loaded", [&](auto& out) {
        out << "index=" << index << " scene=" << session.view.name
            << " layer=none (0x69/0xb9 not applied yet)";
    });
    if (loaded_save != nullptr) session.save_base = *loaded_save;
    session.reload_available = false;
    options.scene = std::move(next);
    options.cc_path = session.view.cc_path;
    options.cb_path = session.view.cb_path;
    load_scene_text(session, verbose);

    if (!save_path.empty()) {
        // 0x425ab4 loads the state; 0x432464 puts the script pointer at the
        // first 0x6f end node, so the entry head is NOT replayed. 0x426935
        // then restores the saved background/panorama, and 0x40c02c rebuilds
        // the click rectangles from the saved object table.
        session.view.trace = byzantine::SceneTrace{};
        session.view.trace.active_hotspots = byzantine::active_hotspots_for_state(
            session.view.script, session.view.state);
        restore_saved_layer(session, *loaded_save, verbose);
        resume_ambient(session, verbose);
        refresh_background(options, session.view);
        session.display.compose(session.view, verbose);
        session.display.present(session.view);
        if (session.display.window != nullptr) {
            SDL_SetWindowTitle(session.display.window,
                               ("Byzantine - " + session.view.name + " save").c_str());
        }
        std::cout << "load scene=" << session.view.name
                  << " save=" << save_path
                  << " entry_skipped=1 (0x432464 script end pointer)"
                  << " background=" << (session.view.committed_background.empty()
                                            ? "none"
                                            : session.view.committed_background)
                  << " panorama=" << (panorama_layer(session.view) ? "yes" : "no")
                  << " hotspots=" << session.view.trace.active_hotspots.size()
                  << " records=" << session.view.state.records.size()
                  << " locals_digest=0x" << std::hex
                  << locals_digest(session.view.state) << std::dec << '\n';
        warn_if_nothing_clickable(session.view, "load");
        return session.running;
    }

    // The new scene runs from its first instruction (DAT_0047a238), exactly
    // like 0x42c8a0's reload; its sounds are gated like any other script. The
    // probe runs on a COPY: a probe must never become the action's entry state
    // (replaying from its own output applies every write twice).
    byzantine::SceneState entry_probe_state = session.view.entry_state;
    const auto probe = byzantine::trace_scene_entry(
        session.view.script, session.view.entries, entry_probe_state, 0);
    const auto run_entry = [&](EntryPoint point, const byzantine::SceneState& entry,
                               const byzantine::SceneTrace& look) {
        if (!session.interactive || !audio_gated(session)) {
            session.view.state = entry;
            set_trace(session.view, trace_point(session.view, point, entry, 0));
            // A scene entry may open with a 0xbd (an establishing shot); it
            // still blocks the script, so play it before the frame settles.
            return play_trace_videos(session, session.view.trace, verbose);
        }
        return run_label_gated(session, point, entry, look, verbose);
    };
    bool entry_session_ended = false;
    if (probe.conversation_request) {
        // 0x42c8a0's reload starts at the instruction head, so a 0x70 there
        // opens the scene's arrival dialogue before the view is interactive
        // (MR's MMhmr1a). The 0xb8 just before it suspends every hotspot and
        // only the conversation's continuation (0xb7) brings them back; skipping
        // the request left the scene silent with zero hotspots, i.e. no exit.
        ClickPlan plan;
        plan.ok = true;
        plan.conversation = true;
        plan.point = EntryPoint{0, true, std::nullopt};
        plan.entry = entry_probe_state;
        plan.entry_before = session.view.entry_state;
        plan.probe = probe;
        std::cout << "entry_conversation record="
                  << probe.conversation_request->record
                  << " instruction=" << probe.conversation_request->instruction
                  << " object_context=" << probe.conversation_request->object_context
                  << '\n';
        if (!execute_plan(session, plan, run_entry, verbose, &entry_session_ended)) {
            // The original would still be inside 0x70 here. Do not pretend the
            // view is ready: keep the pre-dialogue state and say so.
            std::cout << "entry_conversation not committed (scene keeps the "
                         "pre-dialogue state)\n";
        }
    } else if (!run_entry(EntryPoint{0, true, std::nullopt}, session.view.entry_state,
                          probe)) {
        return false;
    }
    // The entry's own trace selects the layer for this scene (see update_layer).
    update_layer(session.view);
    if (!entry_session_ended) show_message_text(session, verbose);
    apply_panorama_events(session.view, options, verbose);
    resume_ambient(session, verbose);
    refresh_background(options, session.view);
    present_arrival(session, verbose);
    if (session.display.window != nullptr) {
        SDL_SetWindowTitle(session.display.window,
                           ("Byzantine - " + session.view.name + " entry").c_str());
    }
    std::cout << "entry scene=" << session.view.name
              << " steps=" << session.view.trace.steps
              << " stopped_at=" << session.view.trace.stopped_at
              << stop_reason(session.view.trace)
              << " hotspots=" << session.view.trace.active_hotspots.size()
              << " records=" << session.view.state.records.size()
              << " locals_digest=0x" << std::hex << locals_digest(session.view.state)
              << std::dec << '\n';
    warn_if_nothing_clickable(session.view, "entry");
    if (save_path.empty() && session.interactive &&
        session.options.settings.autosave && !entry_session_ended) {
        write_autosave(session);
    }
    return session.running;
}

// 0x43c8d2 case 2 + 0x408076 + 0x438c80: the examine button on the inventory
// strip opens the matching close-up scene and snapshots the parent. The scene
// mapping is the original data table (0x438afa/0x438173), not a per-scene port
// branch. A click on the close-up's exit runs 0x73 and travel_to restores the
// snapshot instead of loading the requested index.
//
// Pressing examine *inside* a close-up is the original's 0x408076 -> 0x438c80
// path: 0x438c80 maps the scene to itself, 0x43c8d2 stops the script, and
// 0x42c8a0 reloads the same close-up scene, whose 0x69 transition-4 request now
// reads the newly examined DAT_0045d1cc. The parent snapshot stays armed, so
// the exit still returns to the original parent.
bool reload_closeup(Session& session, bool verbose) {
    if (!session.return_armed || !session.closeup_index) return false;
    auto parent = std::move(session.return_view);
    session.return_view = SceneView{};
    session.return_armed = false;
    const auto index = *session.closeup_index;
    session.closeup_index.reset();
    const bool ok = travel_to(session, index, verbose);
    session.return_view = std::move(parent);
    session.return_armed = true;
    session.closeup_index = index;
    return ok;
}

bool enter_examine(Session& session, bool verbose) {
    const auto use = session.view.state.current_use_item;
    if (!use) {
        std::cout << "bar_examine skipped: no examined item (DAT_0045d1cc)\n";
        return false;
    }
    if (session.return_armed) {
        if (!reload_closeup(session, verbose)) return false;
        std::cout << "closeup_switch scene=" << session.view.name
                  << " item=" << *use
                  << " name=" << byzantine::inventory_name(session.view.state, *use)
                  << " image="
                  << byzantine::examine_image_stem(
                         byzantine::inventory_name(session.view.state, *use))
                  << " index="
                  << (session.closeup_index ? std::to_string(*session.closeup_index)
                                            : "none")
                  << '\n';
        return true;
    }
    if (!session.metadata) {
        std::cout << "bar_examine skipped: --engine (SHOW.LCC) is required\n";
        return false;
    }
    const auto wanted = byzantine::examine_scene_for(session.view.name);
    if (!wanted) {
        std::cout << "bar_examine skipped: scene=" << session.view.name
                  << " has the original's out-of-range va group\n";
        return false;
    }
    std::size_t target_index = 0;
    bool found = false;
    for (std::size_t index = 0; index < session.metadata->scenes.size(); ++index) {
        if (lower_ascii(session.metadata->scenes[index].scene) == *wanted) {
            target_index = index;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "bar_examine skipped: close-up scene " << *wanted
                  << " is not in SHOW.LCC\n";
        return false;
    }
    const auto parent_name = session.view.name;
    SceneView parent = session.view;
    if (!travel_to(session, static_cast<std::uint16_t>(target_index), verbose)) {
        return false;
    }
    session.return_view = std::move(parent);
    session.return_armed = true;
    session.closeup_index = static_cast<std::uint16_t>(target_index);
    std::cout << "closeup_enter scene=" << session.view.name << " parent=" << parent_name
              << " item=" << *use
              << " name=" << byzantine::inventory_name(session.view.state, *use)
              << " image="
              << byzantine::examine_image_stem(
                     byzantine::inventory_name(session.view.state, *use))
              << " index=" << target_index << '\n';
    return true;
}

// 0x4083d4/0x408076: bottom-strip hit test and actions. The coordinates are
// the shipped DAT_0045f548/DAT_00468d00 tables: six slots at x=0xa0+0x3c*i,
// y=0x1aa; examine button 0x2..0x3a, page arrows 0x23b..0x259 and
// 0x25d..0x27b. A click outside those stays inside the strip and is consumed,
// so it can never start a scene hotspot through the 60-pixel band.
bool handle_bar_click(Session& session, int x, int y, bool verbose) {
    if (y < kScreenPlayfieldTop) {
        const int button = byzantine::top_bar_button(x, y);
        std::cout << "ui_click top_bar x=" << x << " y=" << y
                  << " button=" << button
                  << " mode=" << byzantine::top_bar_menu_mode(button)
                  << " consumed (the window path opens the screen)\n";
        return true;
    }
    if (y < kScreenBandTop) return false;
    auto& state = session.view.state;
    if (!state.dialogue_visible) {
        std::cout << "ui_click bar_hidden x=" << x << " y=" << y << " consumed\n";
        return true;
    }
    const auto page = byzantine::inventory_page(state);
    if (y >= 0x1a4 + 6 && y <= kScreenHeight) {
        for (std::size_t index = 0; index < page.items.size(); ++index) {
            const int left = 0xa0 + static_cast<int>(index) * 0x3c;
            if (x < left || x > left + 0x3c) continue;
            const auto id = page.items[index];
            const auto name = byzantine::inventory_name(state, id);
            if (session.return_armed) {
                // Inside a close-up the strip is the item switch of
                // 0x408076+0x438c80: selection, DAT_0045d1cc, the image and the
                // close-up hitboxes all move together. Clicking the item that is
                // already being examined is a no-op (0x408076's
                // `pcVar1 != DAT_0045d1cc` guard), not a deselect.
                if (state.current_use_item && *state.current_use_item == id) {
                    std::cout << "closeup_item slot=" << index << " id=" << id
                              << " name=" << name << " already_examined=yes\n";
                    return true;
                }
                if (!byzantine::inventory_examine(state, id)) {
                    std::cout << "closeup_item slot=" << index << " id=" << id
                              << " skipped: item is not on the current page\n";
                    return true;
                }
                std::cout << "closeup_item slot=" << index << " id=" << id
                          << " name=" << name << " selected=no examined=yes\n";
                enter_examine(session, verbose);
                return true;
            }
            const bool selected = byzantine::inventory_select(state, id);
            std::cout << "bar_item slot=" << index << " id=" << id
                      << " name=" << name << " selected=" << (selected ? "yes" : "no")
                      << " current="
                      << (state.current_dialogue_record
                              ? std::to_string(*state.current_dialogue_record)
                              : "none")
                      << '\n';
            if (verbose) {
                std::cout << "bar_page page=" << page.page << " scroll=" << page.scroll
                          << " count=" << page.items.size() << '\n';
            }
            session.display.present(session.view);
            return true;
        }
    }
    if (x >= 2 && x <= 0x3a && y >= 0x1a8 && y <= 0x1dc) {
        if (!state.current_dialogue_record) {
            std::cout << "bar_examine skipped: no selected item\n";
            return true;
        }
        const auto id = *state.current_dialogue_record;
        if (!byzantine::inventory_examine(state, id)) {
            std::cout << "bar_examine skipped: item=" << id
                      << " is not on the current page\n";
            return true;
        }
        std::cout << "bar_use id=" << id
                  << " name=" << byzantine::inventory_name(state, id) << '\n';
        enter_examine(session, verbose);
        return true;
    }
    if (x >= 0x23b && x <= 0x259 && y >= 0x1b1 && y <= 0x1cb) {
        byzantine::inventory_scroll(state, 1);
        std::cout << "bar_scroll direction=next scroll=" << state.dialogue_scroll
                  << " page=" << state.current_dialogue_channel << '\n';
        session.display.present(session.view);
        return true;
    }
    if (x >= 0x25d && x <= 0x27b && y >= 0x1b1 && y <= 0x1cb) {
        byzantine::inventory_scroll(state, -1);
        std::cout << "bar_scroll direction=prev scroll=" << state.dialogue_scroll
                  << " page=" << state.current_dialogue_channel << '\n';
        session.display.present(session.view);
        return true;
    }
    std::cout << "ui_click bar_empty x=" << x << " y=" << y << " consumed\n";
    return true;
}

void apply_bar_options(Session& session, const Options& options, bool verbose) {
    if (!options.bar_select && options.bar_scroll == 0 && !options.bar_use) return;
    if (options.bar_scroll != 0) {
        byzantine::inventory_scroll(session.view.state, options.bar_scroll);
        std::cout << "bar_scroll delta=" << options.bar_scroll
                  << " scroll=" << session.view.state.dialogue_scroll
                  << " page=" << session.view.state.current_dialogue_channel << '\n';
    }
    if (options.bar_select) {
        const auto inventory = byzantine::inventory_page(session.view.state);
        std::cout << "bar_page page=" << inventory.page
                  << " scroll=" << inventory.scroll << " count="
                  << inventory.items.size() << " ids=";
        for (const auto id : inventory.items) std::cout << id << ',';
        std::cout << '\n';
        const auto id = static_cast<std::uint16_t>(*options.bar_select);
        const auto selected = byzantine::inventory_select(session.view.state, id);
        std::cout << "bar_select id=" << id
                  << " name=" << byzantine::inventory_name(session.view.state, id)
                  << " selected=" << (selected ? "yes" : "no") << '\n';
    }
    session.display.compose(session.view, verbose);
    session.display.present(session.view);
    if (!options.bar_use) return;
    if (!session.view.state.current_dialogue_record) {
        std::cout << "bar_use skipped: no selected item\n";
        return;
    }
    const auto id = *session.view.state.current_dialogue_record;
    if (!byzantine::inventory_examine(session.view.state, id)) {
        std::cout << "bar_use skipped: item=" << id
                  << " is not on the current page\n";
        return;
    }
    std::cout << "bar_use id=" << id
              << " name=" << byzantine::inventory_name(session.view.state, id) << '\n';
    enter_examine(session, verbose);
}

constexpr std::size_t kTravelHopLimit = 8;

// The engine's scene loop: after a 0x73-ended script it reloads the target and
// runs its entry, which may itself end in another 0x73. The hop bound only
// protects the port from a script that would travel forever.
void follow_switches(Session& session, bool verbose) {
    std::size_t hops = 0;
    while (session.running && session.view.trace.scene_switch_requested) {
        const auto index = *session.view.trace.scene_switch_requested;
        if (++hops > kTravelHopLimit) {
            std::cout << "travel index=" << index << " skipped: hop limit "
                      << kTravelHopLimit << " reached\n";
            return;
        }
        if (!travel_to(session, index, verbose)) return;
    }
}

// 0x42db22's 0x73 writes DAT_00482428 and ends the script; 0x42c8a0 then reloads
// the target. The original's loop draws between script calls, so inside one
// action the departing scene is never drawn after the last instruction: a
// blocking 0xbd clip keeps its last frame until the target's first frame.
// The port used to compose+present the action's result before follow_switches,
// which showed the old scene (for a MAP site action: the map with its markers
// closed and the strip on) between the site video and the target scene. See
// analysis/rea/PRESENT_TRACE.md (reported from the Topkapı entry).
bool action_switches_scene(const Session& session) {
    return session.view.trace.scene_switch_requested.has_value();
}

// Presents the committed click only when the scene stays. `travel_to` presents
// the target scene itself.
void present_action_result(Session& session, bool verbose) {
    if (action_switches_scene(session)) return;
    session.display.compose(session.view, verbose);
    // A 0xb9 in the clicked label loads a node and a 0x69 loads a background;
    // both run 0x40186e's/0x431ec2's blend with the code the script carried.
    // Only a load this label actually ran counts: the trace's background is
    // empty when the frame was retained (resolve_frame_background), and a
    // retained picture has no transition of its own.
    const bool loaded = !session.view.trace.panorama_loads.empty() ||
                        session.view.trace.background.has_value();
    if (!loaded) {
        session.display.present(session.view);
        return;
    }
    const char* source = "none";
    const auto code = arrival_transition_code(session.view, session.options, &source);
    session.display.present_transition(session.view, code, source);
}

// A requested 0x73 was refused (missing scene index/script): the departing
// scene is still current, so show the click's own committed frame rather than
// leaving the clip's last frame behind.
void present_refused_switch(Session& session) {
    if (!action_switches_scene(session)) return;
    session.display.compose(session.view);
    session.display.present(session.view);
}

// Diagnostic: the logical playfield rectangle that actually hits each active
// hotspot under the current camera. This is the same hotspot_at path the mouse
// uses, so the printed centre is a valid real click coordinate.
void print_hotspot_screens(const SceneView& view) {
    std::cout << "hotspot_camera yaw="
              << (view.panorama ? view.panorama->yaw_degrees() : -999.0)
              << " panorama_loads=" << view.trace.panorama_loads.size()
              << " rotations=" << view.trace.panorama_rotations.size() << '\n';
    for (const auto& hotspot : view.trace.active_hotspots) {
        int min_x = kSceneWidth;
        int min_y = kSceneHeight;
        int max_x = -1;
        int max_y = -1;
        for (int y = 0; y < kSceneHeight; ++y) {
            for (int x = 0; x < kSceneWidth; ++x) {
                const auto hit = hotspot_at(view, x, y);
                if (!hit.slot || *hit.slot != hotspot.slot) continue;
                min_x = std::min(min_x, x);
                min_y = std::min(min_y, y);
                max_x = std::max(max_x, x);
                max_y = std::max(max_y, y);
            }
        }
        if (max_x < min_x || max_y < min_y) {
            std::cout << "hotspot_screen slot=" << hotspot.slot
                      << " not_visible action=" << hotspot.action << '\n';
            continue;
        }
        std::cout << "hotspot_screen slot=" << hotspot.slot << " x=" << min_x
                  << " y=" << min_y << " w=" << (max_x - min_x + 1)
                  << " h=" << (max_y - min_y + 1)
                  << " centre=" << ((min_x + max_x) / 2) << ','
                  << ((min_y + max_y) / 2) << " action=" << hotspot.action
                  << '\n';
    }
}

// Diagnostic for the common item-use/content layers: every live object of the
// current scene together with the fields 0x40c329/0x407c5a consume. `active`
// is DAT_00464300[id]+0x08, `kind` +0x09, `name` +0x10 and `argument` +0x14.
void print_objects(const SceneView& view) {
    const auto page = byzantine::inventory_page(view.state);
    std::size_t page_count = 0;
    if (!view.state.dialogue_channels.empty()) {
        page_count = view.state.dialogue_channels[
                         page.page % view.state.dialogue_channels.size()]
                         .size();
    }
    std::cout << "bar_state page=" << page.page << " scroll=" << page.scroll
              << " count=" << page_count
              << " visible=" << (view.state.dialogue_visible ? 1 : 0)
              << " selected="
              << (view.state.current_dialogue_record
                      ? std::to_string(*view.state.current_dialogue_record)
                      : "none")
              << " examined="
              << (view.state.current_use_item ? std::to_string(*view.state.current_use_item)
                                              : "none")
              << " ids=";
    for (const auto id : page.items) std::cout << id << ',';
    std::cout << '\n';
    for (const auto& [slot, object] : view.state.objects) {
        std::cout << "object slot=" << slot << " x=" << object.x << " y=" << object.y
                  << " w=" << object.width << " h=" << object.height
                  << " active=" << static_cast<unsigned>(object.active)
                  << " kind=0x" << std::hex << static_cast<unsigned>(object.kind)
                  << std::dec << " label=" << object.label
                  << " name_id=" << object.name_id
                  << " argument=" << object.argument;
        if (object.name_id != 0xffffffffu) {
            std::cout << " name=" << byzantine::inventory_name(
                                        view.state,
                                        static_cast<std::uint16_t>(object.name_id));
        }
        std::cout << '\n';
    }
    for (const auto& hotspot : view.trace.active_hotspots) {
        const auto found = view.state.objects.find(
            static_cast<std::uint16_t>(hotspot.slot));
        std::cout << "hotspot slot=" << hotspot.slot << " action=" << hotspot.action
                  << " state="
                  << (found == view.state.objects.end()
                          ? "no_object"
                          : (found->second.active ? "active" : "inactive"))
                  << '\n';
    }
}

// Diagnostic for the record/animation layer: the live DAT_004649e8 slots with
// the fields 0x43b216/0x43c114 read (kind +0x08, flags +0x09, current frame
// +0x1c, the mode-1 position +0x24/+0x26 and its +0x20 frame).
void print_records(const SceneView& view) {
    for (const auto& [id, record] : view.state.records) {
        std::cout << "record id=" << id << " kind=" << static_cast<unsigned>(record.kind)
                  << " flags=0x" << std::hex << static_cast<unsigned>(record.flags)
                  << std::dec << " x=" << record.x << " y=" << record.y
                  << " snap_x=" << record.snap_x << " snap_y=" << record.snap_y
                  << " frame="
                  << (record.current_frame ? std::to_string(*record.current_frame) : "none")
                  << " snap_frame="
                  << (record.frame_index_20 == 0xffff
                          ? "none"
                          : std::to_string(record.frame_index_20))
                  << " frames=" << record.frame_count
                  << " frames_known=" << (record.frames_known ? 1 : 0)
                  << " delay=" << record.field04 << " count=" << record.field06
                  << " trail=" << record.trail.size();
        if (id < view.state.actor_assets.size()) {
            std::cout << " asset=" << view.state.actor_assets[id].name
                      << " type=" << static_cast<unsigned>(view.state.actor_assets[id].type);
        }
        std::cout << '\n';
    }
}

// Diagnostic: the scene's local variables (DAT_0047e2f0 projection) and, when
// the engine tables are loaded, their global id and declared name. This is the
// state a scene entry rebuilds its visible objects from.
void print_variables(const Session& session) {
    const auto* layout =
        session.metadata ? &byzantine::scene_variable_layout(*session.metadata,
                                                            session.view.name)
                         : nullptr;
    for (const auto& [local, value] : session.view.state.variables) {
        std::cout << "variable local=" << local << " value=" << value;
        if (layout != nullptr && local < layout->global_ids.size()) {
            const auto global = layout->global_ids[local];
            std::cout << " global=" << global;
            if (session.metadata.has_value() &&
                global < session.metadata->names.size() &&
                !session.metadata->names[global].empty()) {
                std::cout << " name=" << session.metadata->names[global];
            }
        }
        std::cout << '\n';
    }
}

// Entry summary for the startup trace (a 0x73 travel prints its own line in
// travel_to). Counts and the stop reason come from the shared trace/blocker
// classification, so a partially loaded entry cannot look complete.
void log_entry_summary(const Session& session) {
    std::cout << "entry scene=" << session.view.name
              << " steps=" << session.view.trace.steps
              << " stopped_at=" << session.view.trace.stopped_at
              << stop_reason(session.view.trace)
              << " hotspots=" << session.view.trace.active_hotspots.size()
              << " records=" << session.view.state.records.size() << '\n';
    warn_if_nothing_clickable(session.view, "entry");
}

// A hotspot whose action reaches 0x73 is a scene switch: 0x42db22 writes
// DAT_00482428 and ends the script, so the port commits the action's state and
// then performs the scene reload the engine's outer loop would.
ClickPlan plan_hotspot(const SceneView& view, int slot) {
    byzantine::diagnostics::write("click_plan", [&](auto& out) {
        out << "scene=" << view.name << " slot=" << slot << " active_slots=";
        for (const auto& hotspot : view.trace.active_hotspots) out << hotspot.slot << ',';
    });
    ClickPlan plan;
    const auto found = std::find_if(view.trace.active_hotspots.begin(),
                                    view.trace.active_hotspots.end(),
                                    [slot](const byzantine::SceneHotspot& hotspot) {
                                        return hotspot.slot == slot;
                                    });
    if (found == view.trace.active_hotspots.end()) {
        plan.message = "slot " + std::to_string(slot) + " is not active";
        return plan;
    }
    if (found->action == 0) {
        plan.message = "slot " + std::to_string(slot) + " has no action label";
        return plan;
    }
    // 0x40c329: while an inventory record is selected (DAT_0045d1c8 != 0), the
    // clicked object's kind/name id must satisfy 0x407c5a before its action
    // label runs; otherwise the dispatcher's result is 0xfffe and 0x43c8d2
    // never reaches the label. This is the common item-on-object gate, not a
    // scene rule: the data comes from the object table and the strip.
    if (const auto selected = view.state.current_dialogue_record; selected.has_value()) {
        plan.item_use_checked = true;
        const auto object = view.state.objects.find(static_cast<std::uint16_t>(slot));
        if (object == view.state.objects.end()) {
            plan.message = "slot " + std::to_string(slot) +
                           " item_use rejected: no object record for the clicked slot";
            return plan;
        }
        const auto& entry_object = object->second;
        const bool compatible =
            byzantine::item_use_compatible(view.state, entry_object, selected);
        plan.item_use_ok = compatible;
        std::cout << "item_use slot=" << slot << " item=" << *selected
                  << " name=" << byzantine::inventory_name(view.state, *selected)
                  << " object_kind=0x" << std::hex
                  << static_cast<unsigned>(entry_object.kind) << std::dec
                  << " object_name=" << entry_object.name_id
                  << " compatible=" << (compatible ? "yes" : "no") << '\n';
        if (!compatible) {
            plan.message = "slot " + std::to_string(slot) + " action " +
                           std::to_string(found->action) + " item_use rejected item=" +
                           std::to_string(*selected) + " kind=0x";
            {
                std::ostringstream kind;
                kind << std::hex << static_cast<unsigned>(entry_object.kind);
                plan.message += kind.str();
            }
            plan.message += " (item in hand; click its strip slot again to cancel)";
            return plan;
        }
    }
    plan.entry = view.state;
    plan.entry_before = view.state;
    plan.action = found->action;
    plan.point = EntryPoint{found->action, false, std::nullopt};
    plan.probe = byzantine::trace_scene_label(view.script, view.entries, plan.entry,
                                              found->action);
    // Every stop that means "the port lacks data or settings, or hit a safety
    // bound" is a failed click: the original would fault or do something this
    // port cannot reproduce, so neither the partial state nor a scene switch may
    // be committed as a success. 0x6f return and 0x79 script end are real ends;
    // 0x73 is a real switch.
    const auto blocker = byzantine::trace_blocker(plan.probe);
    if (!blocker.empty()) {
        plan.message = "slot " + std::to_string(slot) + " action " +
                       std::to_string(plan.action) + " stopped: " + blocker +
                       " at instruction " + std::to_string(plan.probe.stopped_at);
        return plan;
    }
    if (plan.probe.scene_switch_requested) {
        plan.ok = true;
        plan.travel = true;
        plan.switch_index = *plan.probe.scene_switch_requested;
        plan.message = "slot " + std::to_string(slot) + " action " +
                       std::to_string(plan.action) + " scene_switch=" +
                       std::to_string(plan.switch_index);
        return plan;
    }
    if (plan.probe.conversation_request) {
        // 0x70 pauses the script while 0x432d6e runs. The caller must run the
        // real conversation and then resume at the instruction the result
        // selects; committing the probe's partial state as a finished click
        // would skip the conversation.
        plan.ok = true;
        plan.conversation = true;
        plan.message = "slot " + std::to_string(slot) + " action " +
                       std::to_string(plan.action) + " conversation record=" +
                       std::to_string(plan.probe.conversation_request->record) +
                       " instruction=" +
                       std::to_string(plan.probe.conversation_request->instruction);
        return plan;
    }
    plan.ok = true;
    plan.message = "slot " + std::to_string(slot) + " action " +
                   std::to_string(plan.action);
    return plan;
}

// 0x40c329 calls 0x4177c7(0) before it evaluates the item gate, so even a
// rejected item use blanks the examine window. No other scene state changes:
// the label never ran, so variables, objects, records and the strip stay.
void clear_text_after_rejected_use(Session& session, const ClickPlan& plan,
                                   bool verbose) {
    if (!plan.item_use_checked || plan.item_use_ok) return;
    const bool had = session.view.examine_text.active;
    session.view.examine_text = {};
    if (verbose) {
        std::cout << "item_use_text window=examine cleared="
                  << (had ? "true" : "false") << '\n';
    }
    session.display.compose(session.view, false);
    session.display.present(session.view);
}

// FNV-1a over the parts of SceneState a label can change. Used to prove that the
// budgeted (audio-gated) run ends in the same state as one unbudgeted run.
std::uint64_t state_fingerprint(const byzantine::SceneState& state) {
    std::uint64_t hash = 1469598103934665603ull;
    const auto mix = [&hash](std::uint32_t value) {
        for (int byte = 0; byte < 4; ++byte) {
            hash ^= (value >> (byte * 8)) & 0xffu;
            hash *= 1099511628211ull;
        }
    };
    for (const auto& [key, value] : state.variables) mix((key << 16) | value);
    for (const auto& [id, record] : state.records) {
        mix(id);
        mix(record.kind);
        mix(static_cast<std::uint32_t>(record.x));
        mix(static_cast<std::uint32_t>(record.y));
        mix(record.current_frame ? *record.current_frame : 0xffffu);
        mix(record.flags);
    }
    mix(state.dialogue_visible ? 1u : 0u);
    mix(static_cast<std::uint32_t>(state.dialogue_channels.size()));
    for (const auto& channel : state.dialogue_channels) {
        mix(static_cast<std::uint32_t>(channel.size()));
    }
    mix(static_cast<std::uint32_t>(state.objects.size()));
    mix(static_cast<std::uint32_t>(state.hotspot_enabled.size()));
    return hash;
}

std::string describe_trace(const byzantine::SceneTrace& trace,
                           const byzantine::SceneState& state) {
    std::string text = "steps=" + std::to_string(trace.steps);
    if (state.dialogue_visible) text += " dialogue_visible";
    if (!trace.text_requests.empty()) {
        text += " text=" + std::to_string(trace.text_requests.front().id);
    }
    text += " sounds=" + std::to_string(sound_events(trace).size());
    {
        std::ostringstream formatted;
        formatted << " fingerprint=0x" << std::hex << state_fingerprint(state);
        text += formatted.str();
    }
    return text;
}

// Committing one executed click/label. The window path, `--click`,
// `--click-at` and `--run-label` used to repeat this sequence with different
// ordering and presentation, which is how a presentation fix could land in one
// path and miss another (the departing-scene flash, then the clip audio drain).
// The flags keep each path's observable output identical; the *policy* lives
// here once.
struct ClickCommit {
    std::optional<std::uint16_t> object_slot;  // 0x40c329's object text window
    bool result_first = false;   // --shot paths print click_result before the texts
    bool refresh = false;        // refresh_background (interactive paths only)
    bool present_frame = false;  // compose/present the result unless a 0x73 follows
    bool active_slots = false;   // " active_slots=N" suffix on the result line
    bool panorama_verbose = true;
    std::string prefix;          // "run_label N " for a headless label run
};

void commit_click(Session& session, Options& options, const ClickCommit& commit,
                  bool verbose) {
    // The click keeps the state the hover preview produced (that is what makes
    // one click travel), so only the rewind snapshot is dropped here.
    session.hover_select_slot = -1;
    session.hover_highlight_active = false;
    session.hover_highlight_base = {};
    session.hover_highlight_trace = {};
    const bool switching = action_switches_scene(session);
    const auto print_result = [&] {
        std::cout << commit.prefix << "click_result "
                  << describe_trace(session.view.trace, session.view.state);
        if (commit.active_slots) {
            std::cout << " active_slots=" << session.view.trace.active_hotspots.size();
        }
        std::cout << '\n';
    };
    const auto show_text = [&] {
        if (commit.object_slot) show_object_text(session, *commit.object_slot, true);
        show_message_text(session, true);
        apply_panorama_events(session.view, options, commit.panorama_verbose);
    };
    if (!commit.result_first) {
        show_text();
        if (commit.refresh) refresh_background(options, session.view);
        if (commit.present_frame) present_action_result(session, verbose);
    }
    print_result();
    if (commit.result_first) show_text();
    follow_switches(session, verbose);
    // travel_to presents the target. Only a path that presented the result can
    // fall back to it when the requested 0x73 is refused.
    if (switching && commit.present_frame) present_refused_switch(session);
}

// The map's destinations carry their own highlight art: each 0x72 select object
// (kind 0x17, label 433/450/…) draws a *Hi record when it runs (0x64/0x95).
// Hovering a destination should show that art without the click's side effects,
// so run the label on a COPY of the state and keep only its record changes; the
// object/hotspot state stays untouched, which keeps the script's click model
// (select → travel) intact. Restoring the saved records on the next hover
// removes the preview.
// Rewinds the hover preview: the state captured when the pointer entered the
// destination is put back exactly (records, objects, enabled hotspots and the
// trace), so the *Hi highlight disappears instead of accumulating.
bool restore_hover_preview(Session& session, bool verbose) {
    if (!session.hover_highlight_active) return false;
    session.view.state = std::move(session.hover_highlight_base);
    session.hover_highlight_base = {};
    set_trace(session.view, std::move(session.hover_highlight_trace));
    session.hover_highlight_trace = {};
    session.hover_highlight_active = false;
    if (verbose) std::cout << "hover_highlight slot=-1 restored\n";
    return true;
}

// The destination under the pointer, picked from the PREVIEW state's base when
// a preview runs: the select label swaps the destination (41) for its travel
// object (7) and enables the full-playfield cancel (24), so picking from the
// live trace would lose the destination as soon as it was highlighted and make
// the highlight flicker with every motion event.
int hover_select_pick(const Session& session, int screen_x, int screen_y) {
    if (screen_y < kScreenPlayfieldTop || screen_y >= kScreenBandTop) return -1;
    const auto& hotspots = session.hover_highlight_active
        ? session.hover_highlight_trace.active_hotspots
        : session.view.trace.active_hotspots;
    const auto& objects = session.hover_highlight_active
        ? session.hover_highlight_base.objects
        : session.view.state.objects;
    // A destination rectangle wins over the map's full-playfield "click
    // anywhere" handler; the plain smallest-slot hit test would return 24/32.
    int picked = -1;
    for (const auto& hotspot : hotspots) {
        if (hotspot.width >= kSceneWidth && hotspot.height >= kSceneHeight) continue;
        if (screen_x < hotspot.x || screen_x > hotspot.x + hotspot.width) continue;
        if (screen_y < hotspot.y || screen_y > hotspot.y + hotspot.height) continue;
        const auto object = objects.find(static_cast<std::uint16_t>(hotspot.slot));
        if (object == objects.end()) continue;
        if (!object->second.active || object->second.kind != 0x17) continue;
        if (object->second.label == 0) continue;
        if (picked < 0 || hotspot.slot < picked) picked = hotspot.slot;
    }
    return picked;
}

bool hover_select(Session& session, int screen_x, int screen_y, bool verbose) {
    const int picked = hover_select_pick(session, screen_x, screen_y);
    // Staying on the same destination must not rewind and re-apply (that would
    // flash the plain map on every motion event); only a change rewinds.
    if (picked == session.hover_select_slot) return false;
    bool dirty = restore_hover_preview(session, verbose);
    session.hover_select_slot = picked;
    if (picked < 0) return dirty;
    const auto object = session.view.state.objects.at(static_cast<std::uint16_t>(picked));
    auto before = session.view.state;
    auto before_trace = session.view.trace;
    byzantine::SceneState probe_state = session.view.state;
    byzantine::SceneTrace probe;
    try {
        probe = byzantine::trace_scene_label(session.view.script, session.view.entries,
                                             probe_state, object.label);
    } catch (const std::exception& error) {
        std::cout << "hover_highlight slot=" << picked << " label=" << object.label
                  << " trace_failed=" << error.what() << '\n';
        return dirty;
    }
    const auto blocker = byzantine::trace_blocker(probe);
    // A hover must stay a highlight: no travel, dialogue, clip, sound or
    // unsupported command may start from it.
    if (probe.scene_switch_requested || probe.conversation_request ||
        !probe.video_requests.empty() || !blocker.empty() ||
        !sound_events(probe).empty()) {
        if (verbose) {
            std::cout << "hover_highlight slot=" << picked << " label=" << object.label
                      << " skipped reason=not_a_highlight"
                      << " switch=" << (probe.scene_switch_requested ? "yes" : "no")
                      << " conversation=" << (probe.conversation_request ? "yes" : "no")
                      << " videos=" << probe.video_requests.size()
                      << " sounds=" << sound_events(probe).size()
                      << " blocker=" << blocker << '\n';
        }
        return dirty;
    }
    // Commit the select label's state (highlight + travel object) and remember
    // the previous state for the rewind.
    session.view.state = std::move(probe_state);
    session.hover_highlight_base = std::move(before);
    session.hover_highlight_trace = std::move(before_trace);
    session.hover_highlight_active = true;
    set_trace(session.view, probe);
    if (verbose) {
        std::cout << "hover_highlight slot=" << picked << " label=" << object.label
                  << " steps=" << session.view.trace.steps << " at=" << screen_x << ','
                  << screen_y << '\n';
    }
    return true;
}

// ---------------------------------------------------------------------------
// 0x70 CONVERSE: the real conversation VM and its media
// ---------------------------------------------------------------------------
// 0x410669 turns a scene-cd record's file stem into engine/<stem>.ccb (the
// instruction lists and group tables), .ccf (voice-clip names) and .byz (the
// dialogue lines). The three files are the shipped data; nothing here invents a
// subtitle or a choice.

std::optional<std::filesystem::path> find_directory_child(
    const std::filesystem::path& directory, const std::string& name) {
    std::error_code error;
    if (!std::filesystem::is_directory(directory, error)) return std::nullopt;
    const auto wanted = lower_ascii(name);
    for (const auto& entry : std::filesystem::directory_iterator(directory, error)) {
        if (entry.is_directory() && lower_ascii(entry.path().filename().string()) == wanted) {
            return entry.path();
        }
    }
    return std::nullopt;
}

std::string strip_ccd(std::string file) {
    const auto lowered = lower_ascii(file);
    if (lowered.size() >= 4 && lowered.substr(lowered.size() - 4) == ".ccd") {
        file.resize(file.size() - 4);
    }
    return file;
}

// Scene travel changes CD records, but scene.sh initially extracts only the
// starting scene's dialogue. Fetch a missing clip from the user's ISO copies,
// without unpacking all videos or replacing a missing clip with another sound.
std::optional<std::filesystem::path> conversation_clip_file(
    const Options& options, const std::string& conversation, const std::string& clip) {
    const auto lookup = [&]() -> std::optional<std::filesystem::path> {
        // The voice pack is the speech authority. Its dialog/<conversation>
        // tree (or the flat avi/ tree) wins over the base language.
        for (const auto& relative :
             {std::string("dialog/") + conversation + "/" + clip + ".avi",
              std::string("avi/") + clip + ".avi"}) {
            if (const auto file = options.language.find_voice(relative)) return file;
        }
        const auto directory = find_directory_child(options.dialog, conversation);
        if (directory) {
            const auto file = find_filename(*directory, clip + ".avi");
            if (file) return file;
        }
        return find_filename(options.dialog.parent_path() / "avi", clip + ".avi");
    };
    if (const auto file = lookup()) return file;
    if (options.media_isos.empty() || options.dialog.empty()) return std::nullopt;
    // Archive patterns must be leaf names, not paths or wildcards.
    const auto safe_leaf = [](const std::string& name) {
        return !name.empty() && std::all_of(name.begin(), name.end(), [](unsigned char ch) {
            return std::isalnum(ch) || ch == '_' || ch == '-';
        });
    };
    if (!safe_leaf(conversation) || !safe_leaf(clip)) return std::nullopt;
    std::ifstream sources(options.media_isos);
    if (!sources) {
        std::cout << "conversation_media manifest_unreadable=" << options.media_isos << '\n';
        return std::nullopt;
    }
    std::string iso;
    while (std::getline(sources, iso)) {
        if (iso.empty() || !std::filesystem::is_regular_file(iso)) continue;
        const auto command = "7z x -y -ssc- " + shell_quote(iso) + " " +
            shell_quote("dialog/" + conversation + "/" + clip + ".avi") + " " +
            shell_quote("avi/" + clip + ".avi") + " " +
            shell_quote("-o" + options.dialog.parent_path().string()) +
            " >/dev/null 2>&1";
        const auto result = std::system(command.c_str());
        if (const auto file = lookup()) {
            std::cout << "conversation_media extracted=" << file->string()
                      << " source=" << std::filesystem::path(iso).filename().string() << '\n';
            return file;
        }
        if (result != 0) {
            std::cout << "conversation_media extract_failed=" << iso << " status=" << result << '\n';
        }
    }
    return std::nullopt;
}

// Word wrap with 0x40701f's rule: break at the last space that fits the width.
std::vector<std::string> wrap_dialog_text(const byzantine::SfnFont& font,
                                          const std::string& text, int width) {
    std::vector<std::string> lines;
    std::string line;
    std::size_t start = 0;
    while (start <= text.size()) {
        const auto space = text.find(' ', start);
        const auto word = text.substr(start, space == std::string::npos ? std::string::npos
                                                                       : space - start);
        const auto candidate = line.empty() ? word : line + " " + word;
        if (byzantine::text_width(font, candidate) > width && !line.empty()) {
            lines.push_back(line);
            line = word;
        } else {
            line = candidate;
        }
        if (space == std::string::npos) break;
        start = space + 1;
    }
    if (!line.empty()) lines.push_back(line);
    if (lines.empty()) lines.push_back(std::string());
    return lines;
}

struct DecodedClip {
    std::vector<std::filesystem::path> frames;
    std::optional<std::filesystem::path> audio;
    int fps = 15;
    int width = 0;
    int height = 0;
};

// Log prefixes for one clip family: the conversation's talking heads
// (conversation_media/_video/_audio/_shot) and 0xbd's CF clips
// (video_media/video/video_audio/video_shot).
struct ClipLogTags {
    std::string media;
    std::string video;
    std::string audio;
    std::string shot;
};

const ClipLogTags kConversationClipTags{"conversation_media", "conversation_video",
                                        "conversation_audio", "conversation_video_shot"};
const ClipLogTags kSceneClipTags{"video_media", "video", "video_audio", "video_shot"};

// The game's talking-head clips are Indeo 4 + PCM16. Indeo is a standard codec
// this port must not reimplement, so FFmpeg (the ready decoder available here)
// materialises a per-clip cache of JPEG frames and a PCM WAV once. The runtime
// then uses the port's existing JPEG and WAV readers; a missing FFmpeg only
// suppresses media and is reported, never silently hidden.
std::optional<DecodedClip> decode_clip_cache(const std::filesystem::path& directory,
                                             const std::filesystem::path& avi,
                                             const std::string& clip,
                                             const ClipLogTags& tags, bool verbose) {
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    if (error) {
        std::cout << tags.media << " clip=" << clip << " cache_error="
                  << error.message() << '\n';
        return std::nullopt;
    }
    DecodedClip decoded;
    const auto meta = directory / "meta.txt";
    if (std::filesystem::is_regular_file(meta)) {
        std::ifstream input(meta);
        std::string key;
        while (input >> key) {
            const auto equals = key.find('=');
            if (equals == std::string::npos) continue;
            const auto name = key.substr(0, equals);
            const auto value = key.substr(equals + 1);
            if (name == "fps") decoded.fps = std::max(1, std::stoi(value));
            if (name == "width") decoded.width = std::stoi(value);
            if (name == "height") decoded.height = std::stoi(value);
        }
        for (std::uint32_t index = 1;; ++index) {
            char name[32];
            std::snprintf(name, sizeof(name), "f%04u.jpg", index);
            const auto frame = directory / name;
            if (!std::filesystem::is_regular_file(frame)) break;
            decoded.frames.push_back(frame);
        }
        const auto audio = directory / "audio.wav";
        if (std::filesystem::is_regular_file(audio)) decoded.audio = audio;
        if (!decoded.frames.empty() && decoded.width > 0 && decoded.height > 0) {
            if (verbose) {
                std::cout << tags.media << " clip=" << clip << " cached=yes source="
                          << avi.filename().string()
                          << " frames=" << decoded.frames.size()
                          << " fps=" << decoded.fps << " size=" << decoded.width << 'x'
                          << decoded.height
                          << (decoded.audio ? " audio=pcm" : " audio=none") << '\n';
            }
            return decoded;
        }
        // A half-written cache is rebuilt rather than used.
    }

    // Frame rate comes from the source stream, not from an assumption.
    const auto fps_file = directory / "fps.txt";
    const auto probe = "ffprobe -v error -select_streams v:0 "
                       "-show_entries stream=avg_frame_rate -of default=nw=1:nk=1 " +
                       shell_quote(avi.string()) + " > " + shell_quote(fps_file.string()) +
                       " 2>/dev/null";
    if (std::system(probe.c_str()) != 0 || !std::filesystem::is_regular_file(fps_file)) {
        std::cout << tags.media << " clip=" << clip
                  << " decode=ffprobe_failed source=" << avi << '\n';
        return std::nullopt;
    }
    {
        std::ifstream input(fps_file);
        std::string rate;
        input >> rate;
        const auto slash = rate.find('/');
        if (slash != std::string::npos) {
            const int numerator = std::stoi(rate.substr(0, slash));
            const int denominator = std::stoi(rate.substr(slash + 1));
            if (denominator != 0) decoded.fps = std::max(1, (numerator + denominator / 2) / denominator);
        }
    }
    const auto frames_command =
        "ffmpeg -y -loglevel error -i " + shell_quote(avi.string()) +
        " -vsync 0 -q:v 3 " + shell_quote((directory / "f%04d.jpg").string()) +
        " >/dev/null 2>&1";
    if (std::system(frames_command.c_str()) != 0) {
        std::cout << tags.media << " clip=" << clip
                  << " decode=ffmpeg_video_failed source=" << avi << '\n';
        return std::nullopt;
    }
    const auto audio_command =
        "ffmpeg -y -loglevel error -i " + shell_quote(avi.string()) +
        " -vn -acodec pcm_s16le " + shell_quote((directory / "audio.wav").string()) +
        " >/dev/null 2>&1";
    if (std::system(audio_command.c_str()) == 0 &&
        std::filesystem::is_regular_file(directory / "audio.wav")) {
        decoded.audio = directory / "audio.wav";
    }
    for (std::uint32_t index = 1;; ++index) {
        char name[32];
        std::snprintf(name, sizeof(name), "f%04u.jpg", index);
        const auto frame = directory / name;
        if (!std::filesystem::is_regular_file(frame)) break;
        decoded.frames.push_back(frame);
    }
    if (decoded.frames.empty()) {
        std::cout << tags.media << " clip=" << clip
                  << " decode=no_frames source=" << avi << '\n';
        return std::nullopt;
    }
    {
        const auto image = byzantine::decode_jpeg_file(decoded.frames.front().string());
        decoded.width = image.width;
        decoded.height = image.height;
    }
    {
        std::ofstream output(meta);
        output << "fps=" << decoded.fps << '\n'
               << "width=" << decoded.width << '\n'
               << "height=" << decoded.height << '\n';
    }
    if (verbose) {
        std::cout << tags.media << " clip=" << clip << " source=" << avi.filename().string()
                  << " frames=" << decoded.frames.size() << " fps=" << decoded.fps
                  << " size=" << decoded.width << 'x' << decoded.height
                  << (decoded.audio ? " audio=pcm" : " audio=none") << '\n';
    }
    return decoded;
}

// Conversation clips live under --dialog-cache/<conversation>/<clip>.
std::optional<DecodedClip> dialog_clip_cache(const Options& options,
                                             const std::filesystem::path& avi,
                                             const std::string& conversation,
                                             const std::string& clip, bool verbose) {
    const auto cache_root = options.dialog_cache.empty() ? options.dialog / ".cache"
                                                         : options.dialog_cache;
    if (cache_root.empty()) return std::nullopt;
    // The same conversation/clip name can carry different dubbed audio. Keep
    // the decoded frames under the selected voice pack so switching languages
    // never reuses the other language's AVI.
    return decode_clip_cache(cache_root / lower_ascii(options.language.voice_code_or_base()) /
                                 lower_ascii(conversation) / lower_ascii(clip),
                             avi, clip, kConversationClipTags, verbose);
}

// One decoded clip's playback outcome.
struct ClipPlayback {
    bool played = false;
    std::size_t frames = 0;
    std::size_t audio_failures = 0;
    bool shot_written = false;
};

// 0x413bdd's playback loop: draw every decoded frame at the destination
// 0x40d3af chose, pace it at the source frame rate, keep the window responsive
// and return only after the last frame (and its audio) finished - the original
// blocks the script inside that loop. `fullscreen` selects 0x43a248 mode 2
// (2x pixel doubling, 640x400) over mode 0 (1:1); `x`/`y` are playfield
// coordinates, i.e. 0x40d3af's screen x/y minus the 0x14 playfield top. The last
// frame stays on the surface, exactly like the original's DirectDraw target;
// `present` is the caller's compositor (the conversation adds its subtitle
// band, a 0xbd clip presents the plain scene).
ClipPlayback play_decoded_clip(Session& session, const DecodedClip& decoded,
                               bool fullscreen, std::int32_t x, std::int32_t y,
                               const std::string& shot_path, const ClipLogTags& tags,
                               const std::string& clip,
                               const std::function<void()>& present,
                               const std::function<bool(const std::string&)>& write_shot) {
    ClipPlayback result;
    if (decoded.frames.empty()) return result;
    // A clip is not the scene: no hotspot outline on its frames or on the audio
    // drain's re-present (the drain used to draw the map's rectangles).
    ScreenContentScope content(session.display, ScreenContent::clip);
    std::optional<byzantine::WavClip> sound;
    if (!decoded.audio) {
        std::cout << tags.media << " clip=" << clip
                  << " no audio stream in the AVI\n";
    } else if (!session.options.audio) {
        ++result.audio_failures;
        std::cout << tags.audio << " clip=" << clip
                  << " ERROR not played: --no-audio\n";
    } else if (!session.audio.ready()) {
        ++result.audio_failures;
        std::cout << tags.audio << " clip=" << clip
                  << " ERROR not played: the SDL2 audio device is not open "
                     "(see the startup error)\n";
    } else {
        try {
            sound = byzantine::parse_wav(read_file(*decoded.audio),
                                         decoded.audio->string());
        } catch (const std::exception& error) {
            ++result.audio_failures;
            std::cout << tags.audio << " clip=" << clip
                      << " ERROR WAV: " << error.what() << '\n';
        }
    }
    if (sound && session.options.audio_tail_guard) {
        // A torn final buffer is not audio (NBEST1.avi: 67 ms of full-scale
        // noise after the fade to black). Trim it and say so; the clip's video
        // still runs its full length, only the defective tail is silent.
        const auto guard = byzantine::trim_torn_audio_tail(*sound);
        if (guard.trimmed) {
            std::cout << tags.audio << " clip=" << clip
                      << " tail_guard trimmed_at_ms=" << guard.trimmed_at_ms
                      << " removed_ms=" << guard.removed_ms
                      << " flip_at_ms=" << guard.at_ms
                      << " peak=" << guard.peak
                      << " reason=single-sample-sign-flip\n";
        }
    }
    if (sound) {
        try {
            session.audio.queue(*sound, byzantine::VoiceBus::speech);
            std::cout << tags.audio << " clip=" << clip
                      << " wav=" << decoded.audio->filename().string()
                      << " duration_ms=" << sound->duration_ms
                      << " queued_bytes=" << session.audio.queued()
                      << " device=" << session.audio.device_name() << '\n';
        } catch (const std::exception& error) {
            ++result.audio_failures;
            std::cout << tags.audio << " clip=" << clip
                      << " ERROR queue: " << error.what() << '\n';
        }
    }
    const auto frame_time = std::chrono::microseconds(
        1000000 / std::max(1, decoded.fps));
    auto next_frame = std::chrono::steady_clock::now();
    std::size_t frame_index = 0;
    for (const auto& frame_file : decoded.frames) {
        if (!session.running) break;
        byzantine::JpegImage image;
        try {
            image = byzantine::decode_jpeg_file(frame_file.string());
        } catch (const std::exception& error) {
            std::cout << tags.media << " clip=" << clip
                      << " frame_error=" << error.what() << '\n';
            break;
        }
        const auto pixels = byzantine::rgb_to_rgb565(image);
        bool placed = false;
        if (fullscreen) {
            placed = byzantine::blit_image_2x_at(session.display.surface,
                                                 image.width, image.height,
                                                 pixels.data(), x, y);
            if (!placed) {
                std::cout << tags.video << " clip=" << clip << " size="
                          << image.width << 'x' << image.height
                          << " unsupported: 0x43a248 mode 2 needs a 2x frame inside "
                             "the 640x400 playfield; the clip is not shown\n";
            }
        } else {
            placed = byzantine::blit_image(session.display.surface, image.width,
                                           image.height, pixels.data(), x, y)
                         .placed;
        }
        if (!placed) break;
        result.played = true;
        // The presentation trace line names the exact cached JPEG the clip
        // showed, so a bright/wrong frame can be checked against f%04u.jpg.
        ++frame_index;
        session.display.present_detail =
            clip + ":" + std::to_string(frame_index) + "/" +
            std::to_string(decoded.frames.size()) + ":" +
            frame_file.filename().string();
        present();
        if (!shot_path.empty() && !result.shot_written) {
            if (write_shot(shot_path)) {
                std::cout << tags.shot << ' ' << shot_path << " clip=" << clip << '\n';
            }
            result.shot_written = true;
        }
        ++session.wait.frames;
        ++result.frames;
        next_frame += frame_time;
        const auto now = std::chrono::steady_clock::now();
        if (next_frame > now) {
            SDL_Delay(static_cast<Uint32>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    next_frame - now)
                    .count()));
        } else {
            next_frame = now;
        }
        SDL_Event event_poll;
        while (poll_logged_event(&event_poll, tags.video.c_str(), session)) {
            ++session.wait.events;
            if (event_poll.type == SDL_QUIT) session.running = false;
            // 0x413bdd's video loop has no ESC branch; ESC is 0x436b01's
            // answer-list cancel and must not terminate the player here.
            if (event_poll.type == SDL_KEYDOWN &&
                key_is(session.options.settings,
                       session.options.settings.key_hotspots,
                       event_poll.key.keysym.sym)) {
                session.options.overlay = !session.options.overlay;
            }
            if (event_poll.type == SDL_KEYDOWN &&
                key_is(session.options.settings,
                       session.options.settings.key_backlog,
                       event_poll.key.keysym.sym)) {
                backlog_overlay(session);
            }
            if (event_poll.type == SDL_KEYDOWN &&
                key_is(session.options.settings,
                       session.options.settings.key_skip,
                       event_poll.key.keysym.sym)) {
                session.options.settings.skip_seen = !session.options.settings.skip_seen;
                std::cout << "dialogue_skip_seen enabled="
                          << (session.options.settings.skip_seen ? 1 : 0) << '\n';
            }
        }
    }
    if (sound && session.running) {
        while (session.audio.queued() != 0 && session.running) {
            pump_window(session);
            SDL_Delay(4);
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// 0xbd CF videos
// ---------------------------------------------------------------------------
// The clip a 0xbd request names: 0x40d031's "avi/<name>.avi" first, then the
// mode folder (FUN_0040c624). The shipped CDs only carry video\<mode>\ files, so
// the second attempt is the real one. A clip that is not in the prepared tree is
// fetched from the user's ISOs by its exact member name (the CD file systems mix
// case), the saved disc first because scene.sh orders the manifest that way.
std::optional<std::filesystem::path> video_clip_file(const Session& session,
                                                     const byzantine::SceneVideoRequest& request);

// Builds (once per ISO) a lowercase-relative -> exact-member index. 7z matches
// ISO members case-sensitively, so the exact name is required to extract one.
std::optional<std::string> video_member_name(const std::filesystem::path& iso,
                                             const std::filesystem::path& cache_root,
                                             const std::string& relative) {
    std::error_code error;
    std::filesystem::create_directories(cache_root, error);
    if (error) return std::nullopt;
    auto key = static_cast<std::uint64_t>(1469598103934665603ull);
    const auto mix = [&key](const std::string& text) {
        for (const auto character : text) {
            key ^= static_cast<unsigned char>(character);
            key *= 1099511628211ull;
        }
    };
    mix(iso.string());
    if (const auto size = std::filesystem::file_size(iso, error); !error) {
        mix(std::to_string(size));
    }
    std::ostringstream name;
    name << iso.stem().string() << '-' << std::hex << key << ".tsv";
    const auto index_file = cache_root / lower_ascii(name.str());
    if (!std::filesystem::is_regular_file(index_file, error)) {
        const auto listing = cache_root / (lower_ascii(name.str()) + ".listing");
        const auto command = "7z l -slt " + shell_quote(iso.string()) + " > " +
                             shell_quote(listing.string()) + " 2>/dev/null";
        if (std::system(command.c_str()) != 0 ||
            !std::filesystem::is_regular_file(listing, error)) {
            std::cout << "video_index iso=" << iso.filename().string()
                      << " failed (7z l)\n";
            return std::nullopt;
        }
        std::ifstream input(listing);
        std::ofstream output(index_file);
        std::size_t entries = 0;
        std::string line;
        while (std::getline(input, line)) {
            constexpr std::string_view marker = "Path = ";
            if (line.compare(0, marker.size(), marker) != 0) continue;
            const auto member = line.substr(marker.size());
            const auto lowered = lower_ascii(member);
            if (lowered.rfind("video/", 0) != 0 && lowered.rfind("avi/", 0) != 0) {
                continue;
            }
            output << lowered << '\t' << member << '\n';
            ++entries;
        }
        output.close();
        std::filesystem::remove(listing, error);
        std::cout << "video_index iso=" << iso.filename().string()
                  << " entries=" << entries << '\n';
    }
    std::ifstream index(index_file);
    const auto wanted = lower_ascii(relative);
    std::string line;
    while (std::getline(index, line)) {
        const auto tab = line.find('\t');
        if (tab == std::string::npos) continue;
        if (line.substr(0, tab) == wanted) return line.substr(tab + 1);
    }
    return std::nullopt;
}

std::optional<std::filesystem::path> video_clip_file(
    const Session& session, const byzantine::SceneVideoRequest& request) {
    const auto& options = session.options;
    // The prepared root holds backs/, nodes/, video/ ...; an older extraction
    // may still have copy2hd/ around.
    std::vector<std::filesystem::path> roots;
    if (!options.backgrounds.empty()) {
        roots.push_back(options.backgrounds);
        roots.push_back(options.backgrounds / "copy2hd");
    }
    const auto candidates = byzantine::video_candidates(request.mode, request.name);
    for (const auto& candidate : candidates) {
        if (const auto file = options.language.find_voice(candidate.relative)) return file;
        for (const auto& root : roots) {
            if (const auto file = byzantine::find_video_file(root, candidate.relative)) {
                return file;
            }
        }
    }
    if (options.media_isos.empty() || options.backgrounds.empty()) return std::nullopt;
    std::ifstream sources(options.media_isos);
    if (!sources) {
        std::cout << "video_manifest unreadable=" << options.media_isos << '\n';
        return std::nullopt;
    }
    const auto cache_root = options.backgrounds / "video-index";
    std::string iso;
    while (std::getline(sources, iso)) {
        if (iso.empty() || !std::filesystem::is_regular_file(iso)) continue;
        for (const auto& candidate : candidates) {
            const auto member =
                video_member_name(iso, cache_root, candidate.relative);
            if (!member) continue;
            const auto command = "7z x -y -ssc- " + shell_quote(iso) + " " +
                                 shell_quote(*member) + " -o" +
                                 shell_quote(options.backgrounds.string()) +
                                 " >/dev/null 2>&1";
            const auto status = std::system(command.c_str());
            for (const auto& root : roots) {
                if (const auto file = byzantine::find_video_file(root, *member)) {
                    std::cout << "video_media extracted=" << file->string()
                              << " source=" << std::filesystem::path(iso).filename().string()
                              << " name=" << request.name << '\n';
                    return file;
                }
            }
            if (status != 0) {
                std::cout << "video_media extract_failed=" << iso
                          << " status=" << status << '\n';
            }
        }
    }
    return std::nullopt;
}

// 0x40d031: place the clip where 0x40d3af would. The mode only changes the
// destination (x, y) and whether 0x43a248 doubles the frame; the Indeo IV41
// branch of 0x43a248 sizes the destination from the source.
struct VideoPlacement {
    bool fullscreen = false;
    std::int32_t x = 0;
    std::int32_t y = 0;
};

VideoPlacement video_placement(const byzantine::SceneState& state,
                               const byzantine::SceneVideoRequest& request) {
    // 0x40d3af's param_3/param_4 (the operands) are the destination x/y; every
    // mode adds 0x14 to y (the 640x480 playfield top).
    VideoPlacement placement;
    placement.x = request.position_x ? *request.position_x : 0;
    placement.y = (request.position_y ? *request.position_y : 0) + kScreenPlayfieldTop;
    switch (request.mode) {
        case 0:
        case 3:
            // local_8 = 1: 2x at (x, y+0x14), no gamepref check.
            placement.fullscreen = true;
            break;
        case 1:
        case 2:
        case 4:
            // local_8 = 2, then FUN_0041d440(1): when gamepref[1] is 0 the
            // window is 320x200 at (x+0xa0, y+100) instead.
            placement.fullscreen = state.gameprefs ? (*state.gameprefs)[1] != 0 : false;
            if (!placement.fullscreen) {
                placement.x += 0xa0;
                placement.y += 100;
            }
            break;
        default:
            // Modes 5 and any other value keep local_8 = 0 (1:1).
            placement.fullscreen = false;
            break;
    }
    // The caller works in playfield coordinates.
    placement.y -= kScreenPlayfieldTop;
    return placement;
}

// Plays one 0xbd request and blocks until the clip ends, like 0x413bdd. Returns
// false when nothing was shown (missing file, --no-video, decode failure); the
// script still continues, because 0x40d031 returns before touching 0x4859cd.
bool play_cf_video(Session& session, const byzantine::SceneVideoRequest& request,
                   bool verbose) {
    const char* folder = byzantine::video_mode_folder(request.mode);
    std::cout << "video_request scene=" << session.view.name
              << " instruction=" << request.instruction
              << " step=" << request.step
              << " record=" << request.record
              << " name=" << request.name
              << " mode=" << request.mode
              << " folder=" << (folder != nullptr ? folder : "(video)")
              << '\n';
    if (!session.options.video) {
        std::cout << "video skipped scene=" << session.view.name
                  << " name=" << request.name << " mode=" << request.mode
                  << " reason=--no-video\n";
        return false;
    }
    const auto file = video_clip_file(session, request);
    if (!file) {
        std::cout << "video skipped scene=" << session.view.name
                  << " name=" << request.name << " mode=" << request.mode
                  << " reason=missing root=" << session.options.backgrounds.string()
                  << '\n';
        return false;
    }
    std::cout << "video_media name=" << request.name << " file=" << file->string()
              << '\n';
    const auto cache_root = session.options.dialog_cache.empty()
                                ? session.options.backgrounds / ".cache"
                                : session.options.dialog_cache;
    const auto directory = cache_root / lower_ascii(session.options.language.voice_code_or_base()) /
                           "cf" / lower_ascii(session.view.name) /
                           lower_ascii(request.name);
    const auto decoded =
        decode_clip_cache(directory, *file, request.name, kSceneClipTags, verbose);
    if (!decoded) {
        std::cout << "video skipped scene=" << session.view.name
                  << " name=" << request.name << " mode=" << request.mode
                  << " reason=decode_failed\n";
        return false;
    }
    const auto placement = video_placement(session.view.state, request);
    if (!session.view.state.gameprefs &&
        (request.mode == 1 || request.mode == 2 || request.mode == 4)) {
        // 0x40d3af asks FUN_0041d440(1) where to put the clip; without the
        // preference array that read cannot be reproduced, so report the
        // fallback instead of pretending the window is correct.
        std::cout << "video placement_unknown name=" << request.name
                  << " (no --gameprefs; using the 320x200 window)\n";
    }
    std::cout << "video name=" << request.name << " mode=" << request.mode
              << " placement=" << (placement.fullscreen ? "2x640x400" : "1x")
              << " at=" << placement.x << ',' << placement.y
              << " frames=" << decoded->frames.size()
              << " fps=" << decoded->fps << '\n';
    const auto started = std::chrono::steady_clock::now();
    const auto playback = play_decoded_clip(
        session, *decoded, placement.fullscreen, placement.x, placement.y,
        session.options.video_shot, kSceneClipTags, request.name,
        // 0x413bdd presents the clip alone; the hotspot outline is a port
        // diagnostic and must not be drawn over the video (the reported
        // "map markers left on the video" while travelling).
        [&] { session.display.present(session.view); },
        [&](const std::string& path) {
            session.display.build_screen(session.view);
            return byzantine::write_ppm(session.display.screen, path);
        });
    const auto elapsed = static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - started)
            .count());
    std::cout << " elapsed_ms=" << elapsed
              << " written=" << playback.frames
              << " events=" << session.wait.events
              << (session.running ? " (script continues)\n" : " (aborted)\n");
    if (!playback.played) {
        std::cout << "video skipped scene=" << session.view.name
                  << " name=" << request.name << " reason=not_shown\n";
        return false;
    }
    // 0x40d031: DAT_004859cd = 1 after the clip played (except mode 5 and the
    // "mcaft1a1" special case). The flag is render-only: 0x43b216's snap pass.
    if (byzantine::video_sets_4859cd(request.mode, request.name)) {
        session.view.state.flag_4859cd = true;
    }
    // The frames right after a clip are where a stale picture was reported: the
    // script continues with the next instructions (often the 0x73 travel) and
    // this build may still present the departing scene for a moment.
    session.display.arm_present_capture(800, "video_end");
    return true;
}

// Runs every 0xbd request a finished segment produced, in script order.
bool play_trace_videos(Session& session, const byzantine::SceneTrace& trace,
                       bool verbose) {
    for (const auto& request : trace.video_requests) {
        play_cf_video(session, request, verbose);
        if (!session.running) return false;
    }
    return true;
}

// The conversation screen is the original 640x480 surface: the composed
// 640x400 playfield at rows 0x14..0x413, the black 60-px dialogue band at rows
// 0x1a4..0x1df, and the subtitle drawn by 0x424981 at (0x37, 0x1a4) with font
// slot 4 (green 0x03eb). The answers are 0x436b01: x=100, wrap width 440, a
// 0x7f star prefix, font slot 0 (orange 0xe2c0 / white 0xffff when hovered),
// and the block bottom at 0x19f - 0x2d = 370.
struct ConversationUi {
    byzantine::Rgb16Surface screen =
        byzantine::make_surface(kScreenWidth, kScreenHeight, 0);
    std::vector<std::uint16_t> answer_base;  // playfield under the answer list
    std::vector<std::string> caption;        // wrapped subtitle, at most 3 lines
    const byzantine::SfnFont* answers_font = nullptr;
    const byzantine::SfnFont* caption_font = nullptr;
    int line_height = 20;
    static constexpr int kCaptionLines = 3;

    // 0x40d5ff clears all 0x4b000 words of the 640x480 surface when a character
    // video starts. The port keeps the playfield (640x400) as its scene surface
    // and blacks it, so the shop JPEG is not visible through the dialogue.
    void clear_playfield(Session& session) {
        std::fill(session.display.surface.pixels.begin(),
                  session.display.surface.pixels.end(), 0);
        answer_base = session.display.surface.pixels;
    }

    void snapshot_playfield(Session& session) {
        answer_base = session.display.surface.pixels;
    }

    // 0x424780 wraps at 0x212 (530) and 0x424b15 keeps the last three lines.
    void set_caption(const std::string& text) {
        caption.clear();
        if (caption_font == nullptr || text.empty()) return;
        caption = wrap_dialog_text(*caption_font, text, 530);
        if (static_cast<int>(caption.size()) > kCaptionLines) {
            caption.resize(kCaptionLines);
        }
    }

    void clear_caption() { caption.clear(); }

    // 0x424981 clears 0x12c00 bytes (rows 0x1a4..0x1df) on every draw; the port
    // clears the whole 60-px band so the old line cannot survive a redraw.
    void draw_caption(byzantine::Rgb16Surface& target) const {
        for (int y = kScreenBandTop; y < kScreenHeight; ++y) {
            for (int x = 0; x < kScreenWidth; ++x) target.at(x, y) = 0;
        }
        if (caption_font == nullptr) return;
        int y = kScreenBandTop;
        for (const auto& line : caption) {
            byzantine::draw_text(target, *caption_font, line, 0x37, y, 1, 3, 0x03eb);
            y += caption_font->line_height;
        }
    }

    void draw_answers(Session& session,
                      const std::vector<byzantine::ConversationAnswer>& answers,
                      int hovered, std::vector<SDL_Rect>* rects) {
        if (answer_base.size() == session.display.surface.pixels.size()) {
            std::copy(answer_base.begin(), answer_base.end(),
                      session.display.surface.pixels.begin());
        }
        rects->clear();
        if (answers_font == nullptr) return;
        std::vector<std::vector<std::string>> wrapped;
        int total_lines = 0;
        for (const auto& answer : answers) {
            std::string text;
            if (answer.show_text) {
                // 0x436b01 builds "0x7f ' '" + answer text before wrapping;
                // 0x7f selects the DIALOG font's star glyph.
                text.push_back(static_cast<char>(0x7f));
                text.push_back(' ');
                text += answer.text;
            }
            wrapped.push_back(wrap_dialog_text(*answers_font, text, 440));
            total_lines += static_cast<int>(wrapped.back().size());
        }
        // 0x19f - 0x2d: 0x436b01 leaves 45 rows for the subtitle band.
        int top = 0x19f - 0x2d - total_lines * line_height;
        for (std::size_t index = 0; index < answers.size(); ++index) {
            const auto& lines = wrapped[index];
            int width = 0;
            for (const auto& line : lines) {
                width = std::max(width, byzantine::text_width(*answers_font, line));
            }
            int y = top - kScreenPlayfieldTop;  // playfield coordinate
            for (const auto& line : lines) {
                if (!line.empty()) {
                    byzantine::draw_text(session.display.surface, *answers_font, line,
                                         100, y, 1, 3,
                                         index == static_cast<std::size_t>(hovered)
                                             ? 0xffff
                                             : 0xe2c0);
                }
                y += line_height;
            }
            rects->push_back(SDL_Rect{100, top, width,
                                      static_cast<int>(lines.size()) * line_height});
            top += static_cast<int>(lines.size()) * line_height;
        }
    }

    // The original 0x436b01 ESC path opens FUN_0041d6df's 4-button dialog. We
    // draw the real MESSAGES.TXT id 9 message and the id 0x4c..0x4f labels in
    // the common conversation screen; the layout is the port's (a vertical
    // list), the button ids and actions are the original's.
    void draw_menu(Session& session, const std::string& message,
                   const std::vector<std::pair<std::string, int>>& buttons,
                   int hovered, std::vector<SDL_Rect>* rects) {
        rects->clear();
        if (answers_font == nullptr) return;
        const int left = 40;
        const int right = 600;
        const int top = 80;
        const int bottom = 370;
        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                session.display.surface.at(x, y) = 0;
            }
        }
        const auto lines = wrap_dialog_text(*answers_font, message, 480);
        int y = top + 20;
        for (const auto& line : lines) {
            if (!line.empty()) {
                byzantine::draw_text(session.display.surface, *answers_font, line,
                                     left + 20, y, 1, 3, 0xffff);
            }
            y += line_height;
        }
        y += 18;
        for (std::size_t index = 0; index < buttons.size(); ++index) {
            const auto width = byzantine::text_width(*answers_font, buttons[index].first);
            byzantine::draw_text(session.display.surface, *answers_font,
                                 buttons[index].first, left + 40, y, 1, 3,
                                 index == static_cast<std::size_t>(hovered) ? 0xffff
                                                                            : 0xe2c0);
            rects->push_back(SDL_Rect{left + 40, y + kScreenPlayfieldTop, width,
                                      line_height});
            y += line_height + 8;
        }
    }

    void compose(Session& session) {
        std::fill(screen.pixels.begin(), screen.pixels.end(), 0);
        for (int y = 0; y < kSceneHeight; ++y) {
            for (int x = 0; x < kSceneWidth; ++x) {
                screen.at(x, y + kScreenPlayfieldTop) =
                    session.display.surface.at(x, y);
            }
        }
        draw_caption(screen);
    }

    // The conversation screen is scene content: the hotspot-outline diagnostic
    // (H) stays available here, exactly as on the scene. A clip presented inside
    // a conversation is ScreenContent::clip, so its frames and their audio drain
    // are never outlined (0x413bdd blits over the scene surface).
    void present(Session& session) {
        compose(session);
        ScreenContentScope content(session.display, ScreenContent::conversation);
        session.display.present_image(screen, session.view, kScreenPlayfieldTop,
                                      "conversation");
    }



    bool write(Session& session, const std::string& path) {
        compose(session);
        return byzantine::write_ppm(screen, path);
    }
};

int answer_at(const std::vector<SDL_Rect>& rects, float x, float y) {
    for (std::size_t index = 0; index < rects.size(); ++index) {
        const auto& rect = rects[index];
        if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Save writing (0x424dc0) from the live state
// ---------------------------------------------------------------------------
std::filesystem::path savegame_directory(const Options& options) {
    const auto save = options.scene.find("--save");
    if (save != options.scene.end() && !save->second.empty()) {
        return std::filesystem::path(save->second).parent_path();
    }
    if (!options.backgrounds.empty()) {
        return options.backgrounds / "copy2hd" / "savegame";
    }
    return {};
}

template <std::size_t N>
void put_u16(std::array<std::uint8_t, N>& raw, std::size_t at, std::uint16_t value) {
    raw[at] = static_cast<std::uint8_t>(value & 0xff);
    raw[at + 1] = static_cast<std::uint8_t>((value >> 8) & 0xff);
}

template <std::size_t N>
void put_u32(std::array<std::uint8_t, N>& raw, std::size_t at, std::uint32_t value) {
    for (int shift = 0; shift < 32; shift += 8) {
        raw[at + static_cast<std::size_t>(shift / 8)] =
            static_cast<std::uint8_t>((value >> shift) & 0xff);
    }
}

// The per-scene CB background id of the committed picture (0x431ec2's
// DAT_0048253e); 0xffff when the current layer is a close-up/none.
std::uint16_t saved_background_id(const Session& session) {
    const auto& view = session.view;
    if (view.background_name.empty() || panorama_layer(view)) return 0xffff;
    const auto wanted = lower_ascii(view.background_name);
    for (std::size_t index = 0; index < view.entries.size(); ++index) {
        if (lower_ascii(view.entries[index].name) == wanted) {
            return static_cast<std::uint16_t>(index);
        }
    }
    return 0xffff;
}

// The CO.ALL record of the visible panorama (0x432295's DAT_00480330) and a
// source x that reproduces the current centre (DAT_0045c064).
std::pair<std::int32_t, std::int32_t> saved_panorama(const Session& session) {
    const auto& view = session.view;
    if (!panorama_layer(view) || !view.panorama) return {-1, -1};
    const auto path = view.panorama->path();
    const auto file = std::filesystem::path(path).filename().stem().string();
    for (std::size_t index = 0; index < view.state.panorama_records.size(); ++index) {
        if (lower_ascii(view.state.panorama_records[index].name) == lower_ascii(file)) {
            const auto centre = view.panorama->canvas_point(kSceneWidth / 2,
                                                            kSceneHeight / 2);
            const int x = centre.mapped ? centre.source_x : -1;
            return {static_cast<std::int32_t>(index), x};
        }
    }
    return {-1, -1};
}

// Builds the image FUN_00424dc0 would write for the current state. Blocks the
// port does not model (per-scene CCB records, the sound chain, the thumbnail,
// the optional text object, DAT_0045f210/0x46066c) come from `base`, the save
// this session was started/loaded from; with no base they stay empty/zero and
// the writer still emits a structurally valid image.
byzantine::SaveGame build_save_from_state(const Session& session,
                                          const byzantine::SaveGame* base,
                                          const std::string& description,
                                          std::uint16_t scene_index,
                                          std::uint32_t cd_number) {
    using byzantine::kSaveDialogueStackDepth;
    using byzantine::kSaveThumbnailBytes;
    using byzantine::SaveDialogueChain;
    using byzantine::SaveMediaRecord;
    using byzantine::SaveNodeSlot;
    using byzantine::SaveObjectRecord;
    using byzantine::SaveSoundRecord;

    byzantine::SaveGame save = base != nullptr ? *base : byzantine::SaveGame{};
    save.cd_number = cd_number;
    if (save.chapter.empty()) save.chapter = "show";
    save.description = description;
    if (save.thumbnail.size() != kSaveThumbnailBytes) {
        save.thumbnail.assign(kSaveThumbnailBytes, 0);
    }
    // 0x420888 writes a 168x168 RGB16 thumbnail from image id 0x6c. The port
    // does not have that map image; use the current playfield as a cosmetic
    // thumbnail (the loader skips this block anyway).
    {
        const auto& source = session.display.surface;
        save.thumbnail.assign(kSaveThumbnailBytes, 0);
        if (source.width > 0 && source.height > 0) {
            for (std::size_t row = 0; row < byzantine::kSaveThumbnailHeight; ++row) {
                const int source_y = static_cast<int>(row) * source.height /
                                     static_cast<int>(byzantine::kSaveThumbnailHeight);
                for (std::size_t column = 0; column < byzantine::kSaveThumbnailWidth;
                     ++column) {
                    const int source_x =
                        static_cast<int>(column) * source.width /
                        static_cast<int>(byzantine::kSaveThumbnailWidth);
                    const auto pixel = source.at(source_x, source_y);
                    const auto at = (row * byzantine::kSaveThumbnailWidth + column) * 2;
                    save.thumbnail[at] = static_cast<std::uint8_t>(pixel & 0xff);
                    save.thumbnail[at + 1] =
                        static_cast<std::uint8_t>((pixel >> 8) & 0xff);
                }
            }
        }
    }
    if (base != nullptr && base->scene_index != scene_index) {
        // Block 6 is the per-scene CCB record table (DAT_00482ad0), rebuilt on
        // every scene load. A base save from another scene would name the wrong
        // conversations; the port has no equivalent of the 0xcc/0x70-created
        // records, so the block stays empty rather than stale.
        save.scene_records.clear();
        std::cout << "save_scene_records cleared base_scene=" << base->scene_index
                  << " current=" << scene_index
                  << " (per-scene CCB records are not modelled)\n";
    }
    save.scene_index = scene_index;

    const auto& state = session.view.state;
    if (session.metadata) {
        const auto& layout =
            byzantine::scene_variable_layout(*session.metadata, session.view.name);
        save.node_values = byzantine::export_variable_pool(state, layout);
    } else if (state.variable_pool.size() == byzantine::kSaveNodeValueCount) {
        save.node_values = state.variable_pool;
    }
    save.node_values.resize(byzantine::kSaveNodeValueCount, 0);

    save.field_45d1c0 = state.dialogue_visible ? 1 : 0;
    save.field_45d1c4 = state.current_dialogue_channel;
    save.field_45d1bc = state.dialogue_scroll < 0
                            ? 0
                            : static_cast<std::uint32_t>(state.dialogue_scroll);
    save.dialogue_depths = {};
    save.dialogue_stacks = {};
    std::uint32_t max_item = 0;
    for (std::size_t channel = 0; channel < state.dialogue_channels.size(); ++channel) {
        const auto& stack = state.dialogue_channels[channel];
        save.dialogue_depths[channel] = static_cast<std::uint32_t>(stack.size());
        for (std::size_t index = 0;
             index < stack.size() && index < kSaveDialogueStackDepth; ++index) {
            save.dialogue_stacks[channel * kSaveDialogueStackDepth + index] = stack[index];
            max_item = std::max(max_item, static_cast<std::uint32_t>(stack[index]));
        }
    }
    save.dialogue_index =
        state.current_dialogue_record ? *state.current_dialogue_record : -1;
    std::uint32_t mark_count =
        std::max(max_item + 1, static_cast<std::uint32_t>(state.dialogue_records.size()));
    for (const auto& [id, record] : state.dialogue_records) {
        mark_count = std::max(mark_count, static_cast<std::uint32_t>(id) + 1);
    }
    save.dialogue_marks.assign(mark_count, std::pair<std::uint8_t, std::uint8_t>{0, 0});
    for (const auto& [id, record] : state.dialogue_records) {
        if (id < save.dialogue_marks.size()) save.dialogue_marks[id].second = record.state;
    }

    save.field_460668 = state.field_460668;
    const auto [panorama_record, panorama_x] = saved_panorama(session);
    if (panorama_record >= 0) {
        save.field_45c060 = 1;
        save.field_480330 = static_cast<std::uint32_t>(panorama_record);
        save.field_45c064 = panorama_x < 0 ? 0xffffffffu
                                           : static_cast<std::uint32_t>(panorama_x);
    } else if (panorama_layer(session.view)) {
        // A panorama is displayed but its CO record could not be named (no
        // --co or the node is not in the table). Keep the base save's block 16'
        // instead of claiming the current CB background is visible.
        std::cout << "save_panorama unresolved node="
                  << (session.view.panorama ? session.view.panorama->path() : "none")
                  << " (block 16' kept from the base save)\n";
    } else {
        save.field_45c060 = 0;
        save.field_48253e = saved_background_id(session);
    }

    std::map<std::uint16_t, const SaveNodeSlot*> base_nodes;
    for (const auto& node : save.nodes) base_nodes[node.slot] = &node;
    save.nodes.clear();
    for (const auto& [slot, record] : state.records) {
        SaveNodeSlot node;
        node.present = true;
        node.slot = static_cast<std::uint16_t>(slot);
        const auto cached = base_nodes.find(static_cast<std::uint16_t>(slot));
        if (cached != base_nodes.end()) {
            node = *cached->second;
            node.slot = static_cast<std::uint16_t>(slot);
            node.present = true;
        } else {
            node.raw.fill(0);
            // 0x40ffee's initialiser defaults for a record the port created.
            put_u16(node.raw, 0x02, 0x14);
            put_u16(node.raw, 0x04, 10);
            node.raw[0x09] = 0x20;
            node.frame_index_20 = 0xffff;
            node.field_2c = 0xffffffffu;
            node.field_30 = 0xffffffffu;
        }
        put_u16(node.raw, 0x00, static_cast<std::uint16_t>(record.x));
        put_u16(node.raw, 0x02, static_cast<std::uint16_t>(record.y));
        put_u16(node.raw, 0x04, static_cast<std::uint16_t>(record.field04));
        put_u16(node.raw, 0x06, static_cast<std::uint16_t>(record.field06));
        node.raw[0x08] = record.kind;
        node.raw[0x09] = record.flags;
        put_u16(node.raw, 0x14, record.frame_count);
        put_u16(node.raw, 0x24, static_cast<std::uint16_t>(record.snap_x));
        put_u16(node.raw, 0x26, static_cast<std::uint16_t>(record.snap_y));
        node.frame_index_1c = record.current_frame ? *record.current_frame : 0xffff;
        node.field_2c = record.field_2c;
        node.field_30 = record.field_30;
        node.extra.clear();
        for (const auto& extra : record.extra) {
            byzantine::SaveNodeExtra entry;
            entry.control = extra.control;
            entry.first = extra.first;
            entry.second = extra.second;
            node.extra.push_back(entry);
        }
        save.nodes.push_back(std::move(node));
    }

    std::map<std::uint16_t, const SaveObjectRecord*> base_objects;
    for (const auto& object : save.objects) base_objects[object.slot] = &object;
    save.objects.clear();
    for (const auto& [slot, object] : state.objects) {
        SaveObjectRecord record;
        record.slot = static_cast<std::uint16_t>(slot);
        const auto cached = base_objects.find(record.slot);
        if (cached != base_objects.end()) record = *cached->second;
        record.slot = static_cast<std::uint16_t>(slot);
        record.x = object.x;
        record.y = object.y;
        record.width = object.width;
        record.height = object.height;
        record.active = object.active;
        record.kind = object.kind;
        record.label = object.label;
        record.name_id = object.name_id;
        record.argument = object.argument;
        put_u16(record.raw, 0x00, static_cast<std::uint16_t>(object.x));
        put_u16(record.raw, 0x02, static_cast<std::uint16_t>(object.y));
        put_u16(record.raw, 0x04, static_cast<std::uint16_t>(object.width));
        put_u16(record.raw, 0x06, static_cast<std::uint16_t>(object.height));
        record.raw[0x08] = object.active;
        record.raw[0x09] = object.kind;
        put_u32(record.raw, 0x0c, object.label);
        put_u32(record.raw, 0x10, object.name_id);
        put_u32(record.raw, 0x14, object.argument);
        save.objects.push_back(record);
    }

    std::map<std::uint16_t, const SaveSoundRecord*> base_sounds;
    for (const auto& sound : save.sounds) base_sounds[sound.slot] = &sound;
    save.sounds.clear();
    for (const auto& [slot, sound] : state.sounds) {
        SaveSoundRecord record;
        record.slot = static_cast<std::uint16_t>(slot);
        const auto cached = base_sounds.find(record.slot);
        if (cached != base_sounds.end()) record = *cached->second;
        record.slot = static_cast<std::uint16_t>(slot);
        const auto put = [&record](std::size_t at, std::uint16_t value) {
            record.raw[at] = static_cast<std::uint8_t>(value & 0xff);
            record.raw[at + 1] = static_cast<std::uint8_t>((value >> 8) & 0xff);
        };
        put(0x04, sound.enabled);
        put(0x08, sound.field08);
        put(0x0c, sound.field0c);
        put(0x10, sound.field10);
        put(0x14, sound.field14);
        put(0x18, sound.field18);
        put(0x1c, sound.field1c);
        put(0x20, sound.field20);
        put(0x24, sound.field24);
        put(0x28, sound.field28);
        put(0x2c, sound.field2c);
        put(0x30, sound.field30);
        record.raw[0x34] = sound.active ? 1 : 0;
        save.sounds.push_back(record);
    }

    std::map<std::uint16_t, const SaveMediaRecord*> base_media;
    for (const auto& media : save.media) base_media[media.slot] = &media;
    save.media.clear();
    for (const auto& [slot, media] : state.media) {
        SaveMediaRecord record;
        record.slot = static_cast<std::uint16_t>(slot);
        const auto cached = base_media.find(record.slot);
        if (cached != base_media.end()) record = *cached->second;
        record.slot = static_cast<std::uint16_t>(slot);
        record.value = media.value;
        record.frame = media.frame;
        record.timestamp = media.timestamp;
        record.media_id = media.media_id;
        record.active = media.active;
        record.type = media.type;
        put_u16(record.raw, 0x00, media.value);
        put_u32(record.raw, 0x04, media.frame);
        put_u32(record.raw, 0x08, media.timestamp);
        put_u32(record.raw, 0x0c, media.media_id);
        record.raw[0x10] = media.active;
        record.raw[0x11] = media.type;
        save.media.push_back(record);
    }

    std::map<std::uint16_t, const SaveDialogueChain*> base_chains;
    for (const auto& chain : save.dialogue_chains) base_chains[chain.slot] = &chain;
    save.dialogue_chains.clear();
    for (const auto& [slot, chain] : state.dialogue_chains) {
        SaveDialogueChain record;
        record.slot = static_cast<std::uint16_t>(slot);
        const auto cached = base_chains.find(record.slot);
        if (cached != base_chains.end()) record = *cached->second;
        record.slot = static_cast<std::uint16_t>(slot);
        for (std::size_t index = 0; index < chain.ids.size(); ++index) {
            put_u32(record.raw, index * 4, chain.ids[index]);
        }
        put_u32(record.raw, 0x64, chain.count);
        save.dialogue_chains.push_back(record);
    }

    save.current_scene = static_cast<std::uint32_t>(state.alternate_mode);

    // 0x42118c copies savegame/notebook.000 line by line; read the current file
    // so the tail matches what the engine would append next.
    const auto notebook = find_filename(savegame_directory(session.options), "notebook.000");
    if (notebook) {
        try {
            const auto bytes = read_file(*notebook);
            std::string text(bytes.begin(), bytes.end());
            if (text.find('\0') == std::string::npos &&
                (text.empty() || text.back() == '\n')) {
                save.notebook = std::move(text);
            }
        } catch (const std::exception&) {
            // Keep the base save's tail; the writer still validates it.
        }
    }
    return save;
}

// SDL_TEXTINPUT is UTF-8; the save description on disk is CP1254, the same
// single-byte encoding MESSAGES.TXT uses. ASCII and the twelve Turkish letters
// (plus the circumflexed a/i/u) are mapped; anything else is dropped and
// reported by the caller.
std::string utf8_to_cp1254(const std::string& text) {
    static const std::map<std::uint32_t, std::uint8_t> table = {
        {0x00e2, 0xe2}, {0x00c2, 0xc2}, {0x00e7, 0xe7}, {0x00c7, 0xc7},
        {0x011f, 0xf0}, {0x011e, 0xd0}, {0x0131, 0xfd}, {0x0130, 0xdd},
        {0x00ee, 0xee}, {0x00ce, 0xce}, {0x00f6, 0xf6}, {0x00d6, 0xd6},
        {0x015f, 0xfe}, {0x015e, 0xde}, {0x00fc, 0xfc}, {0x00dc, 0xdc},
        {0x00fb, 0xfb}, {0x00db, 0xdb}};
    std::string result;
    for (std::size_t at = 0; at < text.size();) {
        const auto lead = static_cast<unsigned char>(text[at]);
        if (lead < 0x80) {
            result.push_back(static_cast<char>(lead));
            ++at;
            continue;
        }
        std::uint32_t code = 0;
        std::size_t length = 0;
        if ((lead & 0xe0) == 0xc0) {
            code = lead & 0x1f;
            length = 2;
        } else if ((lead & 0xf0) == 0xe0) {
            code = lead & 0x0f;
            length = 3;
        } else if ((lead & 0xf8) == 0xf0) {
            code = lead & 0x07;
            length = 4;
        } else {
            ++at;
            continue;
        }
        if (at + length > text.size()) break;
        for (std::size_t index = 1; index < length; ++index) {
            code = (code << 6) | (static_cast<unsigned char>(text[at + index]) & 0x3f);
        }
        at += length;
        const auto found = table.find(code);
        if (found != table.end()) result.push_back(static_cast<char>(found->second));
    }
    return result;
}

// ---------------------------------------------------------------------------
// The top-bar screens (0x41e289 + 0x41f9ea)
// ---------------------------------------------------------------------------
// Clicking a top-bar button leaves the scene loop and enters FUN_0041e289's
// screen loop. The screen art is the shipped JPEG (setup00/preffs/s_note*), the
// row/text rectangles are 0x45f548, and the enable sets are FUN_0041fe46 +
// 0x42044d. OYUNA DEVAM closes the screen; OYUNDAN ÇIK ends the session.
//
// The save writer (0x424dc0) is not reconstructed, so OYUN KAYIT reports that
// instead of writing a partial save. Everything else here uses real state:
// preferences come from --gameprefs and are written back as
// savegame/gamepref_000 (0x41f7be), the save list is autosave.btb + btb.sav
// (0x41f065/0x41f41c), OYUN YÜKLE loads the selected NN.BTB through the same
// parser and travel path, KEŞİF replays the shipped movies/simulations and the
// notebook pages show their shipped art.
// Defined after run_conversation (0x41e002 needs the conversation runner).
bool play_hints_conversation(Session& session);

// Host captions follow the serif style baked into PREFFS/ROLL. These are
// bundled regular-serif masks, not a reconstructed font from those images.
std::string menu_uppercase(std::string text) {
    for (auto& ch : text) {
        const auto code = static_cast<unsigned char>(ch);
        switch (code) {
            case 0xa7: ch = static_cast<char>(0xa1); break;
            case 0xa8: ch = static_cast<char>(0xa2); break;
            case 0xaa: ch = 'I'; break;
            case 0xab: ch = static_cast<char>(0xa4); break;
            case 0xac: ch = static_cast<char>(0xa5); break;
            case 0xad: ch = static_cast<char>(0xa6); break;
            case 'i': ch = static_cast<char>(0xa3); break;
            default:
                if (code >= 'a' && code <= 'z') ch = static_cast<char>(code - 'a' + 'A');
                break;
        }
    }
    return text;
}

// Host menu decoration, drawn over the shipped notebook at logical 640x480.
void notebook_frame(Display& display, SDL_Rect r, bool selected = false) {
    for (int y = r.y; y < r.y + r.h; ++y)
        for (int x = r.x; x < r.x + r.w; ++x) {
            const auto pixel = display.screen.at(x, y);
            display.screen.at(x, y) = byzantine::shade_pixel(pixel);
        }
    const std::uint16_t gold = selected ? 0xF6AC : 0xA423;
    display.fill_rect(r.x + 3, r.y, r.w - 6, 1, gold);
    display.fill_rect(r.x + 3, r.y + r.h - 1, r.w - 6, 1, gold);
    display.fill_rect(r.x, r.y + 3, 1, r.h - 6, gold);
    display.fill_rect(r.x + r.w - 1, r.y + 3, 1, r.h - 6, gold);
    for (int dx : {0, r.w - 5}) for (int dy : {0, r.h - 5}) {
        display.fill_rect(r.x + dx + 1, r.y + dy + 1, 3, 1, gold);
        display.fill_rect(r.x + dx + 1, r.y + dy + 1, 1, 3, gold);
    }
}

void notebook_arrow(Display& display, int x, int y, int direction) {
    for (int column = 0; column < 5; ++column) {
        const int height = direction > 0 ? 5 - column : column + 1;
        display.fill_rect(x + column, y - height, 1, height * 2 - 1, 0xEDAA);
    }
}

void notebook_rule(Display& display, int x, int y, int width) {
    display.fill_rect(x, y, width, 1, 0xA423);
    for (int center : {x + 8, x + width / 2 - 9, x + width / 2,
                       x + width / 2 + 9, x + width - 8}) {
        for (int d = -3; d <= 3; ++d) {
            const int h = 3 - std::abs(d);
            display.fill_rect(center + d, y - h, 1, 1, 0xEDAA);
            display.fill_rect(center + d, y + h, 1, 1, 0xEDAA);
        }
    }
}

struct MenuUi {
    enum class Confirm { none, delete_slot, quit, overwrite };

    bool open = false;
    int mode = 2;
    int hover = -1;
    int slot = -1;
    int page = 0;
    int kesif_hover = -1;
    int status_count = 0x34;
    bool credits = false;
    Confirm confirm = Confirm::none;
    std::string notice;
    // OYUN KAYIT's name dialog (0x421fa2). The original uses a Windows edit
    // box; the port draws the same MESSAGES id 0x3c prompt and reads
    // SDL_TEXTINPUT into the CP1254 description.
    bool editing = false;
    std::string edit_text;
    std::string edit_file;
    int edit_slot = -1;
    // TEMAS LİSTESİ / notebook pages (0x420c51, 0x4215d6): the ids and blocks
    // come from savegame/notebook.000, which the engine appends to as the game
    // runs.
    struct NoteEntry {
        int line = 0;
        int id = 0;
    };
    struct NotePage {
        int scene = 0;
        int dialog = 0;
        std::vector<NoteEntry> entries;
    };
    std::vector<int> contact_ids;
    std::vector<NotePage> note_pages;
    int hint_index = 0;
    int hint_scroll = 0;
    bool hints = false;  // 0x41e002's HINTS.JPG screen
    std::filesystem::path savegame_dir;
    std::vector<std::string> slot_names;
    std::map<std::string, std::string> descriptions;
    std::map<int, byzantine::Rgb16Surface> art_cache;
    bool fonts_reported = false;

    static const char* mode_name(int value) {
        switch (value) {
            case 0: return "preferences";
            case 1: return "save_load";
            case 2: return "kesif";
            case 3: return "notes";
            case 4: return "contacts";
            case 5: return "spread";
            default: return "?";
        }
    }

    static std::optional<std::filesystem::path> setup_dir(const Options& options) {
        // The text pack's localized menu/notebook art wins; the base tree is
        // the fallback. A pack that omits a file falls back only when the
        // caller uses find_language_child, not through this directory helper.
        for (const auto& relative : {std::string("setupart"),
                                     std::string("copy2hd/setupart")}) {
            if (const auto directory = byzantine::find_relative(
                    options.language.text != nullptr ? options.language.text->root
                                                     : options.language.base,
                    relative)) {
                return *directory;
            }
        }
        if (options.backgrounds.empty()) return std::nullopt;
        std::error_code error;
        if (std::filesystem::is_directory(options.backgrounds / "setupart", error)) {
            return options.backgrounds / "setupart";
        }
        for (const auto& entry :
             std::filesystem::directory_iterator(options.backgrounds, error)) {
            if (!entry.is_directory()) continue;
            if (lower_ascii(entry.path().filename().string()) == "setupart") {
                return entry.path();
            }
        }
        return std::nullopt;
    }

    void init(Session& session) {
        auto& options = session.options;
        const auto save = options.scene.find("--save");
        if (save != options.scene.end() && !save->second.empty()) {
            savegame_dir = std::filesystem::path(save->second).parent_path();
        }
        if (savegame_dir.empty() && !options.backgrounds.empty()) {
            savegame_dir = options.backgrounds / "copy2hd" / "savegame";
        }
    }

    std::optional<std::filesystem::path> slot_path(int index) const {
        if (index < 0 || index >= static_cast<int>(slot_names.size())) return std::nullopt;
        return find_filename(savegame_dir, slot_names[static_cast<std::size_t>(index)]);
    }

    std::optional<std::filesystem::path> game_file(const std::string& name) const {
        return find_filename(savegame_dir, name);
    }

    void refresh_slots() {
        slot_names.clear();
        std::error_code error;
        if (savegame_dir.empty() || !std::filesystem::is_directory(savegame_dir, error)) {
            return;
        }
        const auto add = [&](std::string name) {
            if (name.empty()) return;
            if (!find_filename(savegame_dir, name)) return;
            for (const auto& existing : slot_names) {
                if (lower_ascii(existing) == lower_ascii(name)) return;
            }
            slot_names.push_back(std::move(name));
        };
        // 0x41f065: autosave.btb first, then every line of btb.sav.
        add("autosave.btb");
        const auto list = find_filename(savegame_dir, "btb.sav");
        if (list) {
            try {
                const auto bytes = read_file(*list);
                std::string line;
                for (const auto byte : bytes) {
                    if (byte == '\r') continue;
                    if (byte == '\n') {
                        add(line);
                        line.clear();
                    } else {
                        line.push_back(static_cast<char>(byte));
                    }
                }
                add(line);
            } catch (const std::exception& error_value) {
                std::cout << "menu_slots btb.sav: " << error_value.what() << '\n';
            }
        } else {
            // No slot list: the shipped CD has 00.btb..73.btb. Report the
            // fallback instead of inventing a different ordering.
            char name[16];
            for (int index = 0; index < 74; ++index) {
                std::snprintf(name, sizeof(name), "%02d.btb", index);
                add(name);
            }
        }
        std::cout << "menu_slots dir=" << savegame_dir
                  << " count=" << slot_names.size() << '\n';
    }

    std::string slot_description(int index) {
        const auto path = slot_path(index);
        if (!path) return {};
        const auto key = path->filename().string();
        const auto cached = descriptions.find(key);
        if (cached != descriptions.end()) return cached->second;
        std::string text;
        try {
            const auto save = byzantine::parse_save(read_file(*path));
            text = save.description;
        } catch (const std::exception& error_value) {
            std::cout << "menu_slot file=" << path->filename().string()
                      << " unreadable: " << error_value.what() << '\n';
        }
        descriptions.emplace(key, text);
        return text;
    }

    bool control_enabled(const int id) const {
        bool present = false;
        if (id >= 0x12 && id <= 0x1b) {
            const int index = page * 10 + (id - 0x12);
            present = index >= 0 && index < static_cast<int>(slot_names.size());
        }
        // 0x42044d: DAT_0046c5ec/ed are the save-list page arrows.
        if (mode == 1 && id == 0x1c) {
            return (page + 1) * 10 < static_cast<int>(slot_names.size());
        }
        if (mode == 1 && id == 0x1d) return page > 0;
        // 0x421949 / 0x41fe46 case 5: the notebook page arrows are enabled by
        // the current page (DAT_0046c5f5/0xf6 = controls 0x25/0x26).
        if (mode == 5 && id == 0x25) {
            return hint_index + 2 < static_cast<int>(note_pages.size());
        }
        if (mode == 5 && id == 0x26) return hint_index > 0;
        return byzantine::menu_control_enabled(mode, id, present, slot >= 0);
    }

    int hit_control(int x, int y) const {
        for (const auto& rect : byzantine::menu_control_rects()) {
            if (!control_enabled(rect.id)) continue;
            int bottom = rect.bottom;
            if (rect.id == 0x24) {
                bottom = 0x4b + (status_count - 0x33) * 0x28;
            }
            if (x > rect.left && x < rect.right && y > rect.top && y < bottom) {
                return rect.id;
            }
        }
        return -1;
    }

    void log_open() const {
        std::cout << "menu_open mode=" << mode << " name=" << mode_name(mode)
                  << " page=" << page << " slots=" << slot_names.size()
                  << " save_dir=" << savegame_dir << '\n';
    }

    void open_screen(int value, Session& session) {
        // A text-language change can replace setupart/*; never reuse the old
        // decoded screen for this menu opening.
        art_cache.clear();
        mode = value;
        hover = -1;
        kesif_hover = -1;
        notice.clear();
        credits = false;
        hints = false;
        confirm = Confirm::none;
        editing = false;
        if (mode == 1) {
            page = 0;
            slot = -1;
            refresh_slots();
        }
        if (mode == 2) status_count = read_status_count();
        if (mode >= 3 && mode <= 5) {
            load_notebook(session);
            hint_index = 0;
            hint_scroll = 0;
        }
        open = true;
        log_open();
        present(session);
    }

    void close(Session& session, const char* reason) {
        std::cout << "menu_close reason=" << reason << " mode=" << mode << '\n';
        open = false;
        credits = false;
        hints = false;
        confirm = Confirm::none;
        editing = false;
        SDL_StopTextInput();
        session.display.top_bar_visible = false;
        session.display.top_bar_hover = 0;
        session.display.top_bar_press = 0;
        session.display.compose(session.view, false, false);
        session.display.present(session.view);
    }

    int read_status_count() const {
        // 0x4273f4: savegame/status.000, one little-endian dword. A missing
        // file leaves 0x34 (the credits row only).
        const auto file = game_file("status.000");
        if (!file) return 0x34;
        try {
            const auto bytes = read_file(*file);
            if (bytes.size() < 4) return 0x34;
            return static_cast<int>(static_cast<std::uint32_t>(bytes[0]) |
                                    (static_cast<std::uint32_t>(bytes[1]) << 8) |
                                    (static_cast<std::uint32_t>(bytes[2]) << 16) |
                                    (static_cast<std::uint32_t>(bytes[3]) << 24));
        } catch (const std::exception&) {
            return 0x34;
        }
    }

    const byzantine::Rgb16Surface* art(Session& session, int value) {
        const auto cached = art_cache.find(value);
        if (cached != art_cache.end()) return &cached->second;
        const char* names[] = {"preffs", "setup00", "setup00", "s_note00",
                               "s_note01", "s_note02", "credbk", "hints"};
        const int index = (value >= 0 && value <= 7) ? value : -1;
        if (index < 0) return nullptr;
        std::optional<std::filesystem::path> file =
            find_language_child(session.options, false, "setupart", names[index]);
        if (!file) {
            const auto root = MenuUi::setup_dir(session.options);
            if (root) file = find_child(*root, names[index]);
            if (!file) {
                std::cout << "menu_art_missing mode=" << value << " name=" << names[index]
                          << " root=" << (root ? root->string() : std::string()) << '\n';
                return nullptr;
            }
        }
        try {
            const auto image = byzantine::decode_jpeg_file(file->string());
            byzantine::Rgb16Surface surface;
            surface.width = image.width;
            surface.height = image.height;
            surface.pixels = byzantine::rgb_to_rgb565(image);
            if (surface.width != kScreenWidth || surface.height != kScreenHeight) {
                std::cout << "menu_art file=" << file->filename().string()
                          << " size=" << surface.width << 'x' << surface.height
                          << " (expected 640x480)\n";
            }
            const auto inserted = art_cache.emplace(value, std::move(surface));
            return &inserted.first->second;
        } catch (const std::exception& error_value) {
            std::cout << "menu_art file=" << file->string()
                      << " error=" << error_value.what() << '\n';
            return nullptr;
        }
    }

    void draw_art(Session& session, int value) {
        auto& screen = session.display.screen;
        std::fill(screen.pixels.begin(), screen.pixels.end(), 0);
        const auto* image = art(session, value);
        if (image == nullptr) return;
        const int width = std::min(screen.width, image->width);
        const int height = std::min(screen.height, image->height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                screen.at(x, y) = image->at(x, y);
            }
        }
    }

    void draw_marker(int id) {
        auto& display = *display_;
        if (id < 0 || id >= static_cast<int>(display.marker_frames.size())) return;
        const auto& frame = display.marker_frames[static_cast<std::size_t>(id)];
        display.blit_ui(frame, frame.x, frame.y);
    }

    void draw_preferences(Session& session) {
        const auto prefs = session.view.state.gameprefs;
        if (!prefs) {
            if (!fonts_reported) {
                std::cout << "menu_prefs unavailable: --gameprefs was not supplied\n";
                fonts_reported = true;
            }
            return;
        }
        // 0x41fe46 case 0: a stored 0 draws the frame at on_off index i+6
        // (0x41f94e is the "draw" branch in case 6..0xc).
        for (int index = 0; index < 7; ++index) {
            if ((*prefs)[static_cast<std::size_t>(index)] != 0) continue;
            draw_marker(6 + index);
        }
    }

    const byzantine::SfnFont* menu_font(const Session& session) const {
        if (session.display.setup_font) return &*session.display.setup_font;
        return session.display.dialog_font ? &*session.display.dialog_font : nullptr;
    }

    void draw_save_slots(Session& session) {
        const auto* font = menu_font(session);
        if (font == nullptr) return;
        auto& screen = session.display.screen;
        for (int row = 0; row < 10; ++row) {
            const int index = page * 10 + row;
            if (index >= static_cast<int>(slot_names.size())) break;
            const auto description = slot_description(index);
            const auto& text = description.empty()
                                   ? slot_names[static_cast<std::size_t>(index)]
                                   : description;
            const bool selected = index == slot;
            const int y = 0x42 + row * 0x1e;
            if (selected) {
                for (int row_y = y; row_y < y + 0x0e; ++row_y) {
                    for (int x = 0x17a; x < 0x242; ++x) screen.at(x, row_y) = 0x4208;
                }
            }
            byzantine::draw_text(screen, *font, text, 0x1de, y, 1, 1,
                                 selected ? 0xffff : 0x0000);
        }
        std::cout.flush();
    }

    void draw_kesif(Session& session) {
        const auto* font = menu_font(session);
        if (font == nullptr) return;
        auto& screen = session.display.screen;
        for (int index = 0x34; index <= 0x3a && index <= status_count; ++index) {
            if (index >= static_cast<int>(session.messages.size())) break;
            const int row = index - 0x34;
            const int y = 0x55 + row * 0x28;
            const bool selected = index == kesif_hover;
            if (selected) {
                for (int row_y = y; row_y < y + 0x28; ++row_y) {
                    for (int x = 0x172; x < 0x22e; ++x) screen.at(x, row_y) = 0x4208;
                }
            }
            byzantine::draw_text(screen, *font, session.messages[index], 0x1d0, y, 1, 1,
                                 selected ? 0xffff : 0x0000);
        }
    }

    // 0x420c51: unique dialogue ids from the notebook's `%d: %d` lines, in the
    // order first seen, skipping -1 and -3. 0x41d7ff stores the 16 contact
    // names from MESSAGES.TXT lines 29..44; an id above 15 has no name.
    void load_notebook(Session& session) {
        contact_ids.clear();
        note_pages.clear();
        const auto file = game_file("notebook.000");
        if (!file) return;
        std::vector<std::string> lines;
        try {
            const auto bytes = read_file(*file);
            std::string line;
            for (const auto byte : bytes) {
                if (byte == '\r') continue;
                if (byte == '\n') {
                    lines.push_back(line);
                    line.clear();
                } else {
                    line.push_back(static_cast<char>(byte));
                }
            }
            if (!line.empty()) lines.push_back(line);
        } catch (const std::exception& error) {
            std::cout << "menu_notebook " << *file << ": " << error.what() << '\n';
            return;
        }
        NotePage* page = nullptr;
        for (const auto& line : lines) {
            int first = 0;
            int second = 0;
            if (std::sscanf(line.c_str(), "Scene: %d\tDialog: %d", &first, &second) == 2) {
                note_pages.push_back(NotePage{first, second, {}});
                page = &note_pages.back();
                continue;
            }
            if (line == "EndScene") {
                page = nullptr;
                continue;
            }
            if (page != nullptr &&
                std::sscanf(line.c_str(), "%d: %d", &first, &second) == 2) {
                page->entries.push_back(NoteEntry{first, second});
                if (second != -1 && second != -3) {
                    const bool seen =
                        std::find(contact_ids.begin(), contact_ids.end(), second) !=
                        contact_ids.end();
                    if (!seen && contact_ids.size() < 16) contact_ids.push_back(second);
                }
            }
        }
        std::cout << "menu_notebook file=" << *file << " pages=" << note_pages.size()
                  << " contacts=" << contact_ids.size() << '\n';
        (void)session;
    }

    std::string contact_name(int id, const Session& session) const {
        const int index = 29 + id;
        if (id >= 0 && id < 16 && index >= 0 &&
            index < static_cast<int>(session.messages.size())) {
            return session.messages[static_cast<std::size_t>(index)];
        }
        return {};
    }

    std::string entry_name(int id, const Session& session) const {
        if (id == -1) {
            return session.messages.size() > 28 ? session.messages[28] : "-";
        }
        if (id == -3) {
            return session.messages.size() > 51 ? session.messages[51] : "-";
        }
        const auto name = contact_name(id, session);
        return name.empty() ? "-" : name;
    }

    void draw_contacts(Session& session) {
        const auto* font = menu_font(session);
        if (font == nullptr) return;
        auto& screen = session.display.screen;
        for (std::size_t index = 0; index < contact_ids.size(); ++index) {
            const auto name = contact_name(contact_ids[index], session);
            if (name.empty()) continue;
            const int y = 0x6c + static_cast<int>(index) * 0x14;
            byzantine::draw_text(screen, *font, name, 0x172, y, 1, 3, 0x0000);
        }
    }

    void draw_hint_page(Session& session, const NotePage& page, int page_number,
                        int left, int top) {
        const auto* font = menu_font(session);
        if (font == nullptr) return;
        auto& screen = session.display.screen;
        // 0x4216fe: the page number + place name (0x421690's 15-prefix table,
        // MESSAGES.TXT lines 13..27), then each entry's speaker and line text.
        static const char* const keys[15] = {
            "hs", "as", "am", "bc", "bb", "ea", "ft", "gt", "hl",
            "iu", "mr", "nb", "tk", "sp", "sm"};
        std::string title;
        std::string scene_name;
        if (session.metadata && page.scene >= 0 &&
            page.scene < static_cast<int>(session.metadata->scenes.size())) {
            const auto& scene =
                session.metadata->scenes[static_cast<std::size_t>(page.scene)].scene;
            scene_name = scene;
            const auto prefix = lower_ascii(scene.substr(0, 2));
            for (int index = 0; index < 15; ++index) {
                if (prefix != keys[index]) continue;
                const int line = 13 + index;
                if (line < static_cast<int>(session.messages.size())) {
                    title = session.messages[static_cast<std::size_t>(line)];
                }
                break;
            }
        }
        if (title.empty()) {
            title = !scene_name.empty()
                        ? scene_name
                        : (session.messages.size() > 59 ? session.messages[59]
                                                         : "(devam edecek)");
        }
        byzantine::draw_text(screen, *font, std::to_string(page_number) + " " + title,
                             left, top, 1, 3, 0x0000);
        int y = top + 0x1b;
        const int first = std::clamp(hint_scroll, 0,
                                     std::max(0, static_cast<int>(page.entries.size()) - 1));
        for (int index = first; index < static_cast<int>(page.entries.size()); ++index) {
            const auto& entry = page.entries[static_cast<std::size_t>(index)];
            const auto name = entry_name(entry.id, session);
            if (y + font->line_height >= 0x1df) break;
            byzantine::draw_text(screen, *font, name, left, y, 1, 3, 0x0000);
            y += font->line_height;
            const auto text = note_line_text(session, page.dialog, entry.line);
            if (!text.empty()) {
                const auto lines = wrap_dialog_text(*font, text, 200);
                for (const auto& line : lines) {
                    if (y + font->line_height >= 0x1df) break;
                    byzantine::draw_text(screen, *font, line, left + 10, y, 1, 3,
                                         0x0000);
                    y += font->line_height;
                }
            }
            y += 2;
        }
    }

    // 0x41db33: line `line` (0-based) of engine\<name>.byz, where the name is
    // the block-6 record at slot `dialog`. The split `--byz` tree holds the same
    // files. Without the matching save record the text is not invented.
    std::string note_line_text(Session& session, int dialog, int line) {
        if (!session.save_base) return {};
        const auto record = std::find_if(
            session.save_base->scene_records.begin(),
            session.save_base->scene_records.end(),
            [dialog](const byzantine::SaveSceneRecord& entry) {
                return entry.slot == static_cast<std::uint16_t>(dialog);
            });
        if (record == session.save_base->scene_records.end()) return {};
        std::optional<std::filesystem::path> file =
            find_language_child(session.options, false, "byz", record->name);
        if (!file && !session.options.byz.empty()) {
            file = find_child(session.options.byz, record->name);
        }
        if (!file) return {};
        try {
            const auto bytes = read_file(*file);
            std::string text;
            int index = 0;
            for (const auto byte : bytes) {
                if (byte == '\n') {
                    if (index == line) break;
                    ++index;
                    text.clear();
                } else if (byte != '\r') {
                    text.push_back(static_cast<char>(byte));
                }
            }
            if (index != line) return {};
            return decode_messages_line(session.options, text);
        } catch (const std::exception&) {
            return {};
        }
    }

    void draw_notebook_hints(Session& session) {
        if (note_pages.empty()) {
            const auto* font = menu_font(session);
            if (font != nullptr) {
                byzantine::draw_text(session.display.screen, *font, "NOTLAR bos",
                                     0x172, 0x6c, 1, 3, 0x0000);
            }
            return;
        }
        hint_index = std::clamp(hint_index, 0,
                                static_cast<int>(note_pages.size()) - 1);
        const auto& left = note_pages[static_cast<std::size_t>(hint_index)];
        draw_hint_page(session, left, hint_index + 1, 0x93, 0x46);
        if (hint_index + 1 < static_cast<int>(note_pages.size())) {
            const auto& right = note_pages[static_cast<std::size_t>(hint_index + 1)];
            draw_hint_page(session, right, hint_index + 2, 0x180, 0x4d);
        }
    }

    void draw_box(int left, int top, int right, int bottom) {
        auto& screen = display_->screen;
        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) screen.at(x, y) = 0x0000;
        }
        for (int x = left; x < right; ++x) {
            screen.at(x, top) = 0xffe0;
            screen.at(x, bottom - 1) = 0xffe0;
        }
        for (int y = top; y < bottom; ++y) {
            screen.at(left, y) = 0xffe0;
            screen.at(right - 1, y) = 0xffe0;
        }
    }

    void draw_notice(Session& session) {
        if (notice.empty()) return;
        const auto* font = menu_font(session);
        if (font == nullptr) return;
        draw_box(50, 392, 590, 474);
        const auto lines = wrap_dialog_text(*font, notice, 520);
        int y = 400;
        for (std::size_t index = 0; index < lines.size() && index < 3; ++index) {
            byzantine::draw_text(session.display.screen, *font, lines[index], 60, y, 1, 3,
                                 0xffff);
            y += font->line_height;
        }
    }

    void draw_confirm(Session& session) {
        const auto* font = menu_font(session);
        if (font == nullptr) return;
        draw_box(120, 170, 520, 340);
        int message_id = 11;  // "Kaydedilen oyun silinsin mi?"
        if (confirm == Confirm::quit) message_id = 9;  // "Kaydetmeden oyundan çıkmak istiyor musunuz?"
        if (confirm == Confirm::overwrite) message_id = 12;  // "Oyunun üzerine kaydetmek..."
        const std::string message =
            message_id < static_cast<int>(session.messages.size())
                ? session.messages[static_cast<std::size_t>(message_id)]
                : (confirm == Confirm::quit
                       ? "Oyundan cikmak istiyor musunuz?"
                       : (confirm == Confirm::overwrite
                              ? "Oyunun uzerine kaydetmek istiyor musunuz?"
                              : "Kaydedilen oyun silinsin mi?"));
        const auto lines = wrap_dialog_text(*font, message, 360);
        int y = 190;
        for (const auto& line : lines) {
            byzantine::draw_text(session.display.screen, *font, line, 140, y, 1, 3, 0xffff);
            y += font->line_height;
        }
        const auto label = [&](std::size_t index) {
            return index < session.messages.size() ? session.messages[index]
                                                   : (index == 76 ? "EVET" : "HAYIR");
        };
        const std::pair<int, int> buttons[] = {{180, 270}, {300, 270}};
        for (int index = 0; index < 2; ++index) {
            const int left = buttons[index].first;
            const int top = buttons[index].second;
            for (int row = top; row < top + 34; ++row) {
                for (int x = left; x < left + 90; ++x) {
                    session.display.screen.at(x, row) = 0x3186;
                }
            }
            byzantine::draw_text(session.display.screen, *font, label(76 + index),
                                 left + 45, top + 8, 1, 1, 0xffff);
        }
    }

    bool advanced_hover = false;
    std::optional<byzantine::SfnFont> notebook_face;

    static SDL_Rect port_banner_rect() {
        return SDL_Rect{380, 412, 226, 29};
    }

    void draw_port_tab(Session& session) {
        if (!notebook_face) notebook_face = byzantine::menu_typeface(true);
        const auto* font = &*notebook_face;
        auto& display = session.display;
        const auto rect = port_banner_rect();
        if (advanced_hover) notebook_frame(display, rect, true);
        byzantine::draw_text(display.screen, *font,
                             std::string("> ") + advanced_settings_label(),
                             rect.x + 12, rect.y + 5, 1, 3,
                             advanced_hover ? 0xF6AC : 0x0000);
        notebook_arrow(display, rect.x + rect.w - 14, rect.y + 14, 1);
        if (!advanced_hover) return;
        const auto* prose = font;
        byzantine::draw_text(display.screen, *prose,
                             byzantine::utf8_to_turkish_game_text("AÇIKLAMA"),
                             54, 62, 1, 3, 0xFFE0);
        byzantine::draw_text(display.screen, *font, advanced_settings_label(),
                             54, 89, 1, 3, 0xFF39);
        const auto lines = wrap_dialog_text(*prose,
            byzantine::utf8_to_turkish_game_text(
                "Görüntü, ses, dil ve kontrol seçeneklerini düzenleyin. "
                "Kendi sisteminize en uygun ayarları bu menüden seçebilirsiniz."), 183);
        int y = 120;
        for (const auto& line : lines) {
            if (y + prose->line_height > 326) break;
            byzantine::draw_text(display.screen, *prose, line, 54, y, 1, 3, 0xFF39);
            y += prose->line_height + 2;
        }
    }

    void draw(Session& session) {
        display_ = &session.display;
        if (editing) {
            draw_art(session, mode);
            draw_edit(session);
            return;
        }
        if (hints) {
            draw_art(session, 7);
            draw_notebook_hints(session);
            return;
        }
        if (credits) {
            draw_art(session, 6);
            draw_notice(session);
            return;
        }
        draw_art(session, mode);
        // 0x420587's tab highlight: yellow frames 0/1 for the save-list page,
        // 2 for KEŞİF. The gray OYUN TERCİHLERİ tab is baked in the art.
        if (mode == 1 && !display_->yellow_frames.empty()) {
            const auto index = std::min<std::size_t>(page == 0 ? 0 : 1,
                                                     display_->yellow_frames.size() - 1);
            const auto& frame = display_->yellow_frames[index];
            display_->blit_ui(frame, frame.x, frame.y);
        } else if (mode == 2 && display_->yellow_frames.size() > 2) {
            const auto& frame = display_->yellow_frames[2];
            display_->blit_ui(frame, frame.x, frame.y);
        }
        // 0x41f90e: ROLL.RLE[control] is the hover art for ids 0..14.
        if (hover >= 0 && hover < static_cast<int>(display_->roll_frames.size())) {
            const auto& frame = display_->roll_frames[static_cast<std::size_t>(hover)];
            display_->blit_ui(frame, frame.x, frame.y);
        }
        // 0x41f98e: the enabled rows 0..5 get their on_off marker.
        for (int id = 0; id <= 5; ++id) {
            if (control_enabled(id)) draw_marker(id);
        }
        if (mode == 0) {
            draw_preferences(session);
            draw_port_tab(session);
        }
        if (mode == 1) draw_save_slots(session);
        if (mode == 2) draw_kesif(session);
        if (mode == 4) draw_contacts(session);
        if (mode == 5) draw_notebook_hints(session);
        if (confirm != Confirm::none) draw_confirm(session);
        draw_notice(session);
    }

    void present(Session& session) {
        draw(session);
        ScreenContentScope content(session.display, ScreenContent::menu);
        session.display.present_image(session.display.screen, session.view, 0, "menu");
        float x = 0;
        float y = 0;
        session.display.mouse_logical(&x, &y);
        bool clickable = hit_control(static_cast<int>(x), static_cast<int>(y)) >= 0;
        if (!clickable && mode == 0) {
            const auto tab = port_banner_rect();
            clickable = x >= tab.x && x < tab.x + tab.w && y >= tab.y &&
                        y < tab.y + tab.h;

        }
        byzantine::CursorChoice choice;
        choice.id = clickable ? 0x1e : 0;
        session.display.set_cursor(session.view, choice);
    }

    bool hover_at(Session& session, int x, int y) {
        const bool previous_advanced = advanced_hover;
        const auto banner = port_banner_rect();
        advanced_hover = mode == 0 && x >= banner.x && x < banner.x + banner.w &&
                         y >= banner.y && y < banner.y + banner.h;
        const int previous = hover;
        const int previous_kesif = kesif_hover;
        hover = hit_control(x, y);
        if (hover == 0x24) {
            kesif_hover = byzantine::kesif_row_at(x, y, status_count);
        } else {
            kesif_hover = -1;
        }
        if (hover == previous && kesif_hover == previous_kesif &&
            advanced_hover == previous_advanced) return false;
        if (hover >= 0) {
            std::cout << "menu_hover control=" << hover
                      << (kesif_hover >= 0 ? " row=" + std::to_string(kesif_hover) : "")
                      << '\n';
        }
        present(session);
        return true;
    }

    bool set_notice(Session& session, const std::string& text, const char* log) {
        notice = text;
        std::cout << "menu_notice reason=" << log << " text=" << text << '\n';
        present(session);
        return false;
    }

    bool save_preferences(Session& session) {
        const auto& prefs = session.view.state.gameprefs;
        if (!prefs) return false;
        if (savegame_dir.empty()) {
            std::cout << "menu_prefs_write skipped: no savegame directory\n";
            return false;
        }
        std::error_code error;
        std::filesystem::create_directories(savegame_dir, error);
        const auto path = savegame_dir / "gamepref_000";
        std::ofstream output(path, std::ios::binary | std::ios::trunc);
        if (!output) {
            std::cout << "menu_prefs_write failed file=" << path << '\n';
            return false;
        }
        for (const auto value : *prefs) {
            const std::uint32_t word = value;
            const char bytes[4] = {
                static_cast<char>(word & 0xff),
                static_cast<char>((word >> 8) & 0xff),
                static_cast<char>((word >> 16) & 0xff),
                static_cast<char>((word >> 24) & 0xff)};
            output.write(bytes, 4);
        }
        std::cout << "menu_prefs_write file=" << path << '\n';
        // A travel loads its target setup from this path. If scene.sh initially
        // fell back to the CD's GAMEPREF.000, switch the live option to the file
        // we just wrote so the next scene actually sees the toggle.
        session.options.scene["--gameprefs"] = path.string();
        std::cout << "menu_prefs_live_path=" << path.string() << '\n';
        return static_cast<bool>(output);
    }

    std::uint32_t current_disc(const Session& session) const {
        if (session.save_base) return session.save_base->cd_number;
        const auto overlay = session.options.scene.find("--anims-overlay");
        if (overlay != session.options.scene.end()) {
            const auto folder = std::filesystem::path(overlay->second).filename().string();
            const auto at = folder.find("cd");
            if (at != std::string::npos) {
                int disc = 0;
                std::size_t index = at + 2;
                while (index < folder.size() &&
                       std::isdigit(static_cast<unsigned char>(folder[index]))) {
                    disc = disc * 10 + (folder[index] - '0');
                    ++index;
                }
                if (disc != 0) return static_cast<std::uint32_t>(disc);
            }
        }
        return 2;  // CD1 install default; reported when a save is written
    }

    int first_free_slot() const {
        // 0x41effe: the first NN.btb that does not exist, 00 upward.
        for (int index = 0; index < 100; ++index) {
            char name[16];
            std::snprintf(name, sizeof(name), "%02d.btb", index);
            if (!find_filename(savegame_dir, name)) return index;
        }
        return -1;
    }

    void begin_name_edit(Session& session, std::string file, std::string text, int index) {
        editing = true;
        edit_file = std::move(file);
        edit_text = std::move(text);
        edit_slot = index;
        notice.clear();
        SDL_StartTextInput();
        std::cout << "menu_save_name file=" << edit_file
                  << " existing=" << (index >= 0 ? "yes" : "no") << '\n';
        present(session);
    }

    void append_slot_list(const std::string& name) {
        // 0x41f1ac appends the bare file name to savegame/btb.sav. The shipped
        // list already names 00.btb..73.btb even when a file is missing, so do
        // not add a duplicate line when the name is already listed.
        auto path = find_filename(savegame_dir, "btb.sav");
        const auto target = path ? *path : savegame_dir / "btb.sav";
        if (std::filesystem::exists(target)) {
            try {
                const auto bytes = read_file(target);
                std::string line;
                for (const auto byte : bytes) {
                    if (byte == '\r') continue;
                    if (byte == '\n') {
                        if (lower_ascii(line) == lower_ascii(name)) {
                            std::cout << "menu_slot_list already_listed=" << name << '\n';
                            return;
                        }
                        line.clear();
                    } else {
                        line.push_back(static_cast<char>(byte));
                    }
                }
                if (lower_ascii(line) == lower_ascii(name)) {
                    std::cout << "menu_slot_list already_listed=" << name << '\n';
                    return;
                }
            } catch (const std::exception&) {
                // Fall through and append; the list is repaired by refresh.
            }
        }
        std::ofstream output(target, std::ios::binary | std::ios::app);
        if (!output) {
            std::cout << "menu_slot_list write_failed file=" << target << '\n';
            return;
        }
        output << name << '\n';
        std::cout << "menu_slot_list appended=" << name << " file=" << target << '\n';
    }

    bool commit_save(Session& session) {
        if (edit_file.empty()) return false;
        std::string description = edit_text;
        if (description.empty()) {
            description = std::filesystem::path(edit_file).stem().string();
        }
        if (description.size() > 69) description.resize(69);
        std::uint16_t scene_index = 0;
        bool found_scene = false;
        if (session.metadata) {
            for (std::size_t index = 0; index < session.metadata->scenes.size(); ++index) {
                if (lower_ascii(session.metadata->scenes[index].scene) ==
                    lower_ascii(session.view.name)) {
                    scene_index = static_cast<std::uint16_t>(index);
                    found_scene = true;
                    break;
                }
            }
        }
        if (!found_scene) {
            return set_notice(session, "SHOW.LCC yok: sahne indeksi yazilamadi.",
                              "save_scene_index");
        }
        std::vector<std::uint8_t> bytes;
        byzantine::SaveGame save;
        try {
            save = build_save_from_state(session,
                                         session.save_base ? &*session.save_base : nullptr,
                                         description, scene_index,
                                         current_disc(session));
            bytes = byzantine::write_save(save);
        } catch (const std::exception& error) {
            std::cout << "menu_save failed: " << error.what() << '\n';
            return set_notice(session, std::string("Kayit yazilamadi: ") + error.what(),
                              "save_write");
        }
        std::error_code error;
        std::filesystem::create_directories(savegame_dir, error);
        const auto path = savegame_dir / edit_file;
        const bool is_new = !std::filesystem::exists(path);
        std::ofstream output(path, std::ios::binary | std::ios::trunc);
        if (!output) {
            return set_notice(session, "Kayit dosyasi acilamadi: " + edit_file,
                              "save_open");
        }
        output.write(reinterpret_cast<const char*>(bytes.data()),
                     static_cast<std::streamsize>(bytes.size()));
        output.close();
        if (!output) {
            return set_notice(session, "Kayit dosyasi yazilamadi: " + edit_file,
                              "save_io");
        }
        if (is_new) append_slot_list(edit_file);
        session.save_base = save;
        descriptions.erase(edit_file);
        refresh_slots();
        int selected = -1;
        for (std::size_t index = 0; index < slot_names.size(); ++index) {
            if (lower_ascii(slot_names[index]) == lower_ascii(edit_file)) {
                selected = static_cast<int>(index);
                break;
            }
        }
        slot = selected;
        page = selected >= 0 ? selected / 10 : 0;
        editing = false;
        SDL_StopTextInput();
        std::cout << "menu_save file=" << path << " bytes=" << bytes.size()
                  << " description=" << description << " scene=" << session.view.name
                  << " scene_index=" << scene_index << " new=" << (is_new ? "yes" : "no")
                  << " cd=" << current_disc(session) << '\n';
        notice = "Kayit yazildi: " + edit_file;
        present(session);
        return true;
    }

    void request_save(Session& session) {
        if (editing) return;
        if (slot >= 0) {
            confirm = Confirm::overwrite;
            std::cout << "menu_confirm open action=overwrite\n";
            present(session);
            return;
        }
        const int index = first_free_slot();
        if (index < 0) {
            set_notice(session, "Bos kayit yuvasi yok.", "save_no_slot");
            return;
        }
        char name[16];
        std::snprintf(name, sizeof(name), "%02d.btb", index);
        begin_name_edit(session, name, "", -1);
    }

    bool text_input(Session& session, const std::string& text) {
        if (!editing) return false;
        auto converted = utf8_to_cp1254(text);
        for (const char character : converted) {
            if (edit_text.size() >= 69) break;
            edit_text.push_back(character);
        }
        present(session);
        return true;
    }

    bool key_event(Session& session, const SDL_KeyboardEvent& key) {
        if (!editing) return false;
        switch (key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (!edit_text.empty()) edit_text.pop_back();
                present(session);
                return true;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                commit_save(session);
                return true;
            case SDLK_ESCAPE:
                editing = false;
                SDL_StopTextInput();
                std::cout << "menu_save_name cancelled file=" << edit_file << '\n';
                present(session);
                return true;
            default:
                return true;
        }
    }

    void draw_edit(Session& session) {
        const auto* font = menu_font(session);
        if (font == nullptr) return;
        draw_box(90, 170, 550, 320);
        const std::string prompt =
            session.messages.size() > 60 ? session.messages[60]
                                         : "Kaydettiginiz oyuna isim verin:";
        byzantine::draw_text(session.display.screen, *font, prompt, 110, 190, 2, 3,
                             0xffff);
        const std::string shown = edit_text + "_";
        byzantine::draw_text(session.display.screen, *font, shown, 110, 230, 1, 3,
                             0xffe0);
        byzantine::draw_text(session.display.screen, *font,
                             "Dosya: " + edit_file + "   ENTER: kaydet   ESC: iptal",
                             110, 285, 1, 3, 0xffff);
    }


    bool load_slot(Session& session, int index, const char* reason) {
        const auto path = slot_path(index);
        if (!path) return set_notice(session, "Kayit dosyasi bulunamadi.", reason);
        byzantine::SaveGame save;
        try {
            save = byzantine::parse_save(read_file(*path));
        } catch (const std::exception& error_value) {
            return set_notice(session, std::string("Kayit okunamadi: ") + error_value.what(),
                              reason);
        }
        std::cout << "menu_load slot=" << index << " file=" << path->filename().string()
                  << " description=" << save.description
                  << " scene_index=" << save.scene_index
                  << " reason=" << reason << '\n';
        const bool was_open = open;
        open = false;
        const bool ok = travel_to(session, save.scene_index, true, path->string(), &save);
        if (!ok) {
            open = was_open;
            return set_notice(session, "Kayit sahnesi yuklenemedi.", "load_failed");
        }
        follow_switches(session, true);
        std::cout << "menu_load committed scene=" << session.view.name
                  << " hotspots=" << session.view.trace.active_hotspots.size() << '\n';
        return true;
    }

    bool new_game(Session& session) {
        const auto path = game_file("new.can");
        if (!path) return set_notice(session, "Yeni oyun dosyasi (new.can) yok.", "new_game");
        byzantine::SaveGame save;
        try {
            save = byzantine::parse_save(read_file(*path));
        } catch (const std::exception& error_value) {
            return set_notice(session, std::string("new.can okunamadi: ") + error_value.what(),
                              "new_game");
        }
        std::cout << "menu_new_game file=" << path->filename().string()
                  << " scene_index=" << save.scene_index << '\n';
        const bool was_open = open;
        open = false;
        const bool ok = travel_to(session, save.scene_index, true, path->string(), &save);
        if (!ok) {
            open = was_open;
            return set_notice(session, "Yeni oyun baslatilamadi.", "new_game_failed");
        }
        follow_switches(session, true);
        return true;
    }

    bool delete_slot(Session& session) {
        (void)session;
        const auto path = slot_path(slot);
        if (!path) return false;
        std::error_code error;
        const bool removed = std::filesystem::remove(*path, error);
        std::cout << "menu_delete slot=" << slot << " file=" << path->filename().string()
                  << " removed=" << (removed ? "yes" : "no");
        if (error) std::cout << " error=" << error.message();
        std::cout << '\n';
        descriptions.erase(path->filename().string());
        refresh_slots();
        slot = -1;
        page = std::min(page, std::max(0, (static_cast<int>(slot_names.size()) - 1) / 10));
        return removed;
    }

    bool play_movie(Session& session, const std::string& name) {
        byzantine::SceneVideoRequest request;
        request.mode = 1;  // video\intro
        request.name = name;
        request.instruction = 0;
        request.step = 0;
        std::string clip = name;
        std::cout << "menu_movie name=" << name << " mode=1\n";
        if (!session.options.video) {
            return set_notice(session, "Video kapali (--no-video).", "movie_no_video");
        }
        const auto file = video_clip_file(session, request);
        if (!file) {
            return set_notice(session, "Film dosyasi bulunamadi: " + name, "movie_missing");
        }
        const auto cache_root = session.options.dialog_cache.empty()
                                    ? session.options.backgrounds / ".cache"
                                    : session.options.dialog_cache;
        const auto decoded = decode_clip_cache(
            cache_root / lower_ascii(session.options.language.voice_code_or_base()) / "menu" / name,
            *file, name, kSceneClipTags, true);
        if (!decoded) {
            return set_notice(session, "Film cozulemedi: " + name, "movie_decode");
        }
        session.display.top_bar_visible = false;
        const auto present_clip = [&session]() {
            auto& display = session.display;
            std::fill(display.screen.pixels.begin(), display.screen.pixels.end(), 0);
            for (int row = 0; row < display.surface.height; ++row) {
                for (int column = 0; column < display.surface.width; ++column) {
                    display.screen.at(column, row + kScreenPlayfieldTop) =
                        display.surface.at(column, row);
                }
            }
            ScreenContentScope content(display, ScreenContent::menu);
            display.present_image(display.screen, session.view, 0, "menu_movie");
        };
        const auto playback = play_decoded_clip(
            session, *decoded, true, 0, 0, session.options.video_shot, kSceneClipTags,
            clip, present_clip, [](const std::string&) { return false; });
        std::cout << "menu_movie end name=" << name << " frames=" << playback.frames
                  << " played=" << (playback.played ? "yes" : "no") << '\n';
        if (!playback.played) {
            return set_notice(session, "Film gosterilemedi: " + name, "movie_not_shown");
        }
        // 0x41fc87 returns to the screen it was opened from; the last clip frame
        // is still on the surface, so repaint the menu.
        present(session);
        return true;
    }

    bool play_credits(Session& session) {
        // 0x422500: setupart/credbk.jpg is the backdrop; text/CREDITS.TXT is
        // read into 600 lines and scrolled up two pixels every 25 ms. A click
        // or ESC leaves the scroll (the original watches DAT_0045f050/58).
        const auto* font = menu_font(session);
        if (font == nullptr) return false;
        std::optional<std::filesystem::path> text;
        for (const auto& relative : {std::string("text/CREDITS.TXT"),
                                     std::string("copy2hd/text/CREDITS.TXT")}) {
            if (const auto file = session.options.language.find_text(relative)) {
                text = *file;
                break;
            }
        }
        if (!text) for (const auto& candidate :
             {session.options.backgrounds / "text" / "CREDITS.TXT",
              session.options.backgrounds / "copy2hd" / "text" / "CREDITS.TXT",
              session.options.backgrounds / "setupart" / "CREDITS.TXT"}) {
            std::error_code error;
            if (std::filesystem::is_regular_file(candidate, error)) {
                text = candidate;
                break;
            }
        }
        if (!text) {
            return set_notice(session, "text/CREDITS.TXT bulunamadi.",
                              "credits_missing");
        }
        std::vector<std::string> lines;
        std::vector<bool> headings;
        try {
            const auto bytes = read_file(*text);
            std::string line;
            const auto flush = [&]() {
                bool heading = !line.empty() && line.front() == '$';
                if (heading) line.erase(line.begin());
                lines.push_back(decode_messages_line(session.options, line));
                headings.push_back(heading);
                line.clear();
            };
            for (const auto byte : bytes) {
                if (byte == '\r') continue;
                if (byte == '\n') flush();
                else line.push_back(static_cast<char>(byte));
            }
            if (!line.empty()) flush();
        } catch (const std::exception& error) {
            std::cout << "menu_credits read " << *text << ": " << error.what() << '\n';
            return set_notice(session, "Jenerik dosyasi okunamadi.", "credits_read");
        }
        const auto* backdrop = art(session, 6);
        if (backdrop == nullptr) {
            return set_notice(session, "CREDBK.JPG bulunamadi.", "credits_art");
        }
        if (session.audio.ready() && !session.options.sounds.empty()) {
            const auto wav = sound_path(session.options.sounds, "credit", "");
            if (wav) {
                try {
                    session.audio.queue(byzantine::parse_wav(read_file(*wav),
                                                             wav->string()),
                                        byzantine::VoiceBus::ambient);
                    std::cout << "menu_credits audio=" << wav->filename().string()
                              << '\n';
                } catch (const std::exception& error) {
                    std::cout << "menu_credits audio skipped: " << error.what() << '\n';
                }
            }
        }
        const int line_height = std::max(1, font->line_height);
        int y = 0x1de;
        const auto started = std::chrono::steady_clock::now();
        std::cout << "menu_credits file=" << *text << " lines=" << lines.size()
                  << " line_height=" << line_height << '\n';
        session.display.top_bar_visible = false;
        int frames = 0;
        while (session.running) {
            auto& display = session.display;
            std::fill(display.screen.pixels.begin(), display.screen.pixels.end(), 0);
            const int width = std::min(display.screen.width, backdrop->width);
            const int height = std::min(display.screen.height, backdrop->height);
            for (int row = 0; row < height; ++row) {
                for (int column = 0; column < width; ++column) {
                    display.screen.at(column, row) = backdrop->at(column, row);
                }
            }
            for (std::size_t index = 0; index < lines.size(); ++index) {
                const int line_y = y + static_cast<int>(index) * line_height;
                if (line_y <= -line_height || line_y >= display.screen.height) continue;
                byzantine::draw_text(display.screen, *font, lines[index],
                                     kScreenWidth / 2, line_y, 1, 1,
                                     headings[index] ? 0xffe0 : 0xffff);
            }
            ScreenContentScope content(display, ScreenContent::menu);
            display.present_image(display.screen, session.view, 0, "credits");
            ++frames;
            if (session.options.credits_frames &&
                frames >= *session.options.credits_frames) {
                if (!session.options.credits_shot.empty()) {
                    byzantine::write_ppm(display.screen, session.options.credits_shot);
                    std::cout << "menu_credits shot=" << session.options.credits_shot
                              << '\n';
                }
                std::cout << "menu_credits frames=" << frames
                          << " (diagnostic stop)\n";
                break;
            }
            SDL_Event event;
            bool stop = false;
            while (poll_logged_event(&event, "credits", session)) {
                if (event.type == SDL_QUIT) {
                    session.running = false;
                    stop = true;
                } else if (event.type == SDL_KEYDOWN &&
                           event.key.keysym.sym == SDLK_ESCAPE) {
                    stop = true;
                } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                           event.button.button == SDL_BUTTON_LEFT) {
                    stop = true;
                }
            }
            if (stop) break;
            if (y + static_cast<int>(lines.size()) * line_height < 0) break;
            SDL_Delay(25);
            y -= 2;
        }
        const auto elapsed = static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - started)
                .count());
        std::cout << "menu_credits end elapsed_ms=" << elapsed
                  << " lines=" << lines.size() << '\n';
        present(session);
        return true;
    }

    bool travel_to_named(Session& session, const std::string& scene) {
        if (!session.metadata) {
            return set_notice(session, "SHOW.LCC yok: sahne indeksi cozulemiyor.", "scene_index");
        }
        for (std::size_t index = 0; index < session.metadata->scenes.size(); ++index) {
            if (session.metadata->scenes[index].scene != scene) continue;
            std::cout << "menu_kesif scene=" << scene << " index=" << index << '\n';
            const bool was_open = open;
            open = false;
            const bool ok = travel_to(session, static_cast<std::uint16_t>(index), true);
            if (!ok) {
                open = was_open;
                return set_notice(session, "Sahne yuklenemedi: " + scene, "scene_failed");
            }
            // The disc scenes open with a 0x73 chain (vtdisc -> vt); the
            // original's 0x42c8a0 loop follows it, so the menu must too.
            follow_switches(session, true);
            return true;
        }
        return set_notice(session, "Sahne bulunamadi: " + scene, "scene_missing");
    }

    bool click(Session& session, int x, int y) {
        notice.clear();
        if (hints) {
            hints = false;
            std::cout << "menu_hints close\n";
            present(session);
            return true;
        }
        if (confirm != Confirm::none) {
            for (int index = 0; index < 2; ++index) {
                const int left = 180 + index * 120;
                if (x >= left && x <= left + 90 && y >= 270 && y <= 304) {
                    if (index == 1) {
                        std::cout << "menu_confirm result=cancel\n";
                        confirm = Confirm::none;
                        present(session);
                        return true;
                    }
                    const auto action = confirm;
                    confirm = Confirm::none;
                    std::cout << "menu_confirm result=yes\n";
                    if (action == Confirm::delete_slot) {
                        delete_slot(session);
                    } else if (action == Confirm::quit) {
                        std::cout << "menu_quit\n";
                        session.running = false;
                        return true;
                    } else if (action == Confirm::overwrite) {
                        if (slot >= 0 && slot < static_cast<int>(slot_names.size())) {
                            begin_name_edit(session, slot_names[static_cast<std::size_t>(slot)],
                                            slot_description(slot), slot);
                            return true;
                        }
                    }
                    present(session);
                    return true;
                }
            }
            std::cout << "menu_confirm result=none\n";
            return false;
        }
        if (credits) {
            credits = false;
            std::cout << "menu_credits close\n";
            present(session);
            return true;
        }
        if (mode == 0) {
            const auto banner = port_banner_rect();
            if (x >= banner.x && x < banner.x + banner.w && y >= banner.y &&
                y < banner.y + banner.h) {
                session.port_settings_group = -1;
                session.open_port_settings = true;
                open = false;
                std::cout << "menu_port_settings open from TERCICHLER hub\n";
                return true;
            }
        }
        const int hit = hit_control(x, y);
        std::cout << "menu_click x=" << x << " y=" << y << " control=" << hit
                  << " mode=" << mode << '\n';
        if (hit < 0) return false;
        switch (hit) {
            case 0x00:  // OYUN KAYIT
                request_save(session);
                return true;
            case 0x01:  // OYUN YÜKLE
                return load_slot(session, slot, "oyun_yukle");
            case 0x02:  // OYUN İPTAL
                confirm = Confirm::delete_slot;
                std::cout << "menu_confirm open action=delete_slot\n";
                present(session);
                return true;
            case 0x03:  // YENİ OYUN
                return new_game(session);
            case 0x04:  // OYUNDAN ÇIK
                confirm = Confirm::quit;
                std::cout << "menu_confirm open action=quit\n";
                present(session);
                return true;
            case 0x05:  // OYUNA DEVAM
                close(session, "oyuna_devam");
                return true;
            case 0x06:
            case 0x07:
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0c: {
                auto& prefs = session.view.state.gameprefs;
                if (!prefs) {
                    return set_notice(session, "Tercihler yuklenmedi (--gameprefs).",
                                      "prefs_missing");
                }
                const int index = hit - 0x06;
                (*prefs)[static_cast<std::size_t>(index)] =
                    (*prefs)[static_cast<std::size_t>(index)] == 0 ? 1 : 0;
                std::cout << "menu_pref index=" << index
                          << " value=" << (*prefs)[static_cast<std::size_t>(index)] << '\n';
                save_preferences(session);
                present(session);
                return true;
            }
            case 0x0e: open_screen(0, session); return true;
            case 0x10: open_screen(2, session); return true;
            case 0x11: open_screen(3, session); return true;
            case 0x0f: open_screen(1, session); return true;
            case 0x1c:  // next save page (0x42062a's param 0x1c)
                if (mode == 1 && (page + 1) * 10 < static_cast<int>(slot_names.size())) {
                    ++page;
                    slot = -1;
                    std::cout << "menu_page page=" << page << '\n';
                    present(session);
                }
                return true;
            case 0x1d:  // previous save page
                if (mode == 1 && page > 0) {
                    --page;
                    slot = -1;
                    std::cout << "menu_page page=" << page << '\n';
                    present(session);
                }
                return true;
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x1a:
            case 0x1b: {
                const int index = page * 10 + (hit - 0x12);
                if (slot == index) {
                    slot = -1;
                } else {
                    slot = index;
                }
                std::cout << "menu_slot index=" << index
                          << " file=" << slot_names[static_cast<std::size_t>(index)]
                          << " description=" << slot_description(index)
                          << " selected=" << (slot == index ? "yes" : "no") << '\n';
                present(session);
                return true;
            }
            case 0x1e: open_screen(1, session); return true;
            case 0x1f: open_screen(4, session); return true;
            case 0x20: open_screen(5, session); return true;
            case 0x21: open_screen(1, session); return true;
            case 0x22:
            case 0x23: open_screen(3, session); return true;
            case 0x24: {
                if (kesif_hover < 0) {
                    // A click without a preceding motion still picks the row
                    // (0x422101's hover normally runs every loop).
                    const int row = byzantine::kesif_row_at(x, y, status_count);
                    if (row >= 0) kesif_hover = row;
                }
                if (kesif_hover < 0) return false;
                const int index = kesif_hover;
                std::cout << "menu_kesif_click index=" << index
                          << " text="
                          << (index < static_cast<int>(session.messages.size())
                                  ? session.messages[static_cast<std::size_t>(index)]
                                  : std::string("?"))
                          << '\n';
                if (index == 0x34) {
                    std::cout << "menu_credits image=setupart/CREDBK.JPG"
                              << " text=text/CREDITS.TXT (0x422500 scroll)\n";
                    return play_credits(session);
                }
                if (index == 0x35) return play_movie(session, "mintro");
                if (index == 0x3a) return play_movie(session, "mepilog");
                if (index >= 0x36 && index <= 0x39) {
                    static const char* scenes[] = {"vtdisc", "vbdisc", "vcdisc", "vhdisc"};
                    return travel_to_named(session, scenes[index - 0x36]);
                }
                return false;
            }
            case 0x25:
            case 0x26: {
                if (mode != 5 || note_pages.empty()) {
                    return set_notice(session, "Defter sayfa kaydirma bu ekranda yok.",
                                      "notes_scroll");
                }
                // 0x421949: DAT_0045f7e4 += 2 / -= 2; two pages are drawn.
                hint_index += (hit == 0x25 ? 2 : -2);
                hint_index = std::clamp(hint_index, 0,
                                        std::max(0, static_cast<int>(note_pages.size()) - 1));
                hint_scroll = 0;
                std::cout << "menu_notes_page page=" << hint_index + 1 << '/'
                          << note_pages.size() << '\n';
                present(session);
                return true;
            }
            case 0x27:
                // 0x41e002: HINTS.JPG + the Hints1 conversation.
                load_notebook(session);
                if (play_hints_conversation(session)) {
                    present(session);
                    return true;
                }
                hints = true;
                hint_index = 0;
                hint_scroll = 0;
                std::cout << "menu_hints image=setupart/HINTS.JPG pages="
                          << note_pages.size()
                          << " fallback=notebook (Hints1 conversation unavailable)\n";
                present(session);
                return true;
            default:
                return false;
        }
    }

    Display* display_ = nullptr;
};

// Deterministic input for --menu-click-at/--menu-text/--menu-key: the same
// handlers the window's event loop uses, in the order the arguments appear on
// the command line is not preserved; clicks, then text, then keys.
struct SettingsUi;
void apply_headless_menu_input(Session&, MenuUi&, const Options&, SettingsUi&);

// Port-only dialogue log. It freezes the caller (scene, text wait, clip or
// answer list) and draws the remembered lines on the 640x480 screen. The game
// state is untouched; closing simply returns to the caller's presenter.
bool backlog_overlay(Session& session) {
    const auto* font = session.display.setup_font
                           ? &*session.display.setup_font
                           : (session.display.dialog_font ? &*session.display.dialog_font
                                                          : nullptr);
    if (font == nullptr) {
        std::cout << "backlog skipped: no DIALOG/setup font\n";
        return false;
    }
    int scroll = 0;
    const auto previous = session.display.content;
    session.display.enter_content(ScreenContent::menu);
    while (session.running) {
        auto& display = session.display;
        display.fill_rect(0, 0, kScreenWidth, kScreenHeight, 0x10a2);
        display.fill_rect(20, 18, 600, 444, 0x0000);
        display.fill_rect(22, 20, 596, 440, 0x0861);
        byzantine::draw_text(display.screen, *font, "DIYALOG GECMISI", 40, 30, 1, 3,
                             0xffff);
        std::vector<std::string> lines;
        std::vector<bool> answers;
        for (const auto& entry : session.dialogue_history) {
            std::string text;
            if (!entry.speaker.empty()) text = entry.speaker + ": ";
            if (entry.answer) text = "> " + text;
            text += entry.text;
            const auto wrapped = wrap_dialog_text(*font, text, 560);
            for (const auto& line : wrapped) {
                lines.push_back(line);
                answers.push_back(entry.answer);
            }
        }
        const int line_height = std::max(12, font->line_height);
        const int visible = std::max(1, (420 - 72) / line_height);
        const int maximum = std::max(0, static_cast<int>(lines.size()) - visible);
        scroll = std::clamp(scroll, 0, maximum);
        const int first = std::max(0, static_cast<int>(lines.size()) - visible - scroll);
        int y = 72;
        if (lines.empty()) {
            byzantine::draw_text(display.screen, *font, "Henuz diyalog yok.", 40, y, 1,
                                 3, 0xce79);
        }
        for (int index = first;
             index < static_cast<int>(lines.size()) && y + line_height <= 420; ++index) {
            byzantine::draw_text(display.screen, *font, lines[static_cast<std::size_t>(index)],
                                 40, y, 1, 3,
                                 answers[static_cast<std::size_t>(index)] ? 0x03eb
                                                                          : 0xffff);
            y += line_height;
        }
        byzantine::draw_text(display.screen, *font,
                             "F3 / ESC kapat    yukari-asagi kaydir    tekerlek",
                             40, 438, 1, 3, 0xce79);
        display.present_image(display.screen, session.view, 0, "backlog");
        SDL_Event event;
        while (poll_logged_event(&event, "backlog", session)) {
            if (event.type == SDL_QUIT) {
                session.running = false;
            } else if (event.type == SDL_KEYDOWN) {
                const auto key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE ||
                    key_is(session.options.settings,
                           session.options.settings.key_backlog, key)) {
                    session.display.restore_content(previous);
                    session.display.present(session.view);
                    return true;
                }
                if (key == SDLK_UP) ++scroll;
                else if (key == SDLK_DOWN) --scroll;
                else if (key == SDLK_PAGEUP) scroll += visible;
                else if (key == SDLK_PAGEDOWN) scroll -= visible;
                else if (key == SDLK_HOME) scroll = maximum;
                else if (key == SDLK_END) scroll = 0;
            } else if (event.type == SDL_MOUSEWHEEL) {
                scroll += event.wheel.y > 0 ? 1 : -1;
            }
            scroll = std::clamp(scroll, 0, maximum);
        }
        SDL_Delay(16);
    }
    session.display.restore_content(previous);
    return true;
}

bool wait_dialog_ms(Session& session, std::uint64_t milliseconds) {
    const auto start = std::chrono::steady_clock::now();
    while (session.running) {
        pump_window(session);
        const auto waited = static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start)
                .count());
        if (waited >= milliseconds) break;
        SDL_Delay(8);
    }
    return session.running;
}

struct ConversationOutcome {
    bool ok = false;
    bool cancelled = false;
    std::uint16_t result = 0xffff;
    std::string name;
    std::size_t lines = 0;
    std::size_t answer_lists = 0;
    std::size_t media = 0;
    std::size_t audio_failures = 0;
};

// Runs the conversation until 0x39/0x436b01 end it. An answer is a real mouse
// click for the interactive player; --shot consumes --conversation-choices so a
// deterministic acceptance run can be reproduced. It never invents a choice.
ConversationOutcome run_conversation(Session& session, std::uint16_t record_index,
                                     std::uint16_t object_context, bool verbose) {
    ConversationOutcome outcome;
    const auto& options = session.options;
    auto& state = session.view.state;
    if (!state.conversation_table_known) {
        std::cout << "conversation record=" << record_index
                  << " stopped: no --cd conversation table\n";
        return outcome;
    }
    if (record_index >= state.conversation_records.size()) {
        std::cout << "conversation record=" << record_index << " outside the "
                  << state.conversation_records.size() << " engine/<scene>cd records\n";
        return outcome;
    }
    const auto& record = state.conversation_records[record_index];
    outcome.name = record.name;
    const auto stem = strip_ccd(record.file);
    const auto ccb = find_child(options.ccb, stem);
    std::optional<std::filesystem::path> ccf =
        find_language_child(options, true, "ccf", stem);
    if (!ccf && !options.ccf.empty()) ccf = find_child(options.ccf, stem);
    std::optional<std::filesystem::path> byz =
        find_language_child(options, false, "byz", stem);
    if (!byz && !options.byz.empty()) byz = find_child(options.byz, stem);
    if (!ccb || !ccf || !byz) {
        std::cout << "conversation record=" << record_index << " name=" << record.name
                  << " stopped: missing"
                  << (ccb ? "" : " ccb") << (ccf ? "" : " ccf") << (byz ? "" : " byz")
                  << " for " << stem << '\n';
        return outcome;
    }
    byzantine::ConversationProgram program;
    try {
        program = byzantine::parse_conversation_program(
            read_file(*ccb), byzantine::parse_conversation_lines(read_file(*ccf), "CCF"),
            byzantine::parse_conversation_lines(read_file(*byz), "BYZ"), record.name);
    } catch (const std::exception& error) {
        std::cout << "conversation record=" << record_index << " name=" << record.name
                  << " parse_error=" << error.what() << '\n';
        return outcome;
    }
    if (program.clips.size() != program.texts.size()) {
        // The CCF list comes from the voice pack and the BYZ list from the text
        // pack, so a mixed selection can expose a translation that changed the
        // line count. Keep the conversation runnable but name the mismatch.
        std::cout << "conversation_pairing record=" << record_index
                  << " name=" << record.name << " clips=" << program.clips.size()
                  << " texts=" << program.texts.size()
                  << " text_lang=" << options.language.text_code_or_base()
                  << " voice_lang=" << options.language.voice_code_or_base()
                  << " (a line may be missing in one pack)\n";
    }
    if (verbose) {
        std::cout << "conversation record=" << record_index << " name=" << record.name
                  << " stem=" << stem << " clips=" << program.clips.size()
                  << " texts=" << program.texts.size()
                  << " trailing_bytes=" << program.trailing_bytes
                  << " text_lang=" << options.language.text_code_or_base()
                  << " voice_lang=" << options.language.voice_code_or_base()
                  << " object_context=" << object_context << '\n';
    }
    // DAT_00482ad0[record_index]: 0x410669 allocates it once and 0x432d6e
    // keeps the entry flags/defaults across repeated 0x70 calls in one scene.
    // travel_to builds a fresh SceneState, so a 0x73 scene load clears this map
    // (0x42c8a0 -> 0x41316f -> 0x40fbf6 -> 0x40f98c).
    auto& conversation_memory = state.conversation_memory[record_index];
    if (verbose) {
        std::cout << "conversation_memory record=" << record_index
                  << " asked=" << conversation_memory.unspeak.size()
                  << " disabled=" << conversation_memory.disabled.size()
                  << " done=" << (conversation_memory.done ? "yes" : "no")
                  << " default_line=" << static_cast<int>(conversation_memory.default_line)
                  << " default_reply="
                  << static_cast<int>(conversation_memory.default_reply) << '\n';
    }
    byzantine::ConversationSession conversation(std::move(program), state,
                                               &conversation_memory);
    conversation.start();

    // The scene layer under the dialogue is whatever the pre-conversation
    // segment committed.
    refresh_background(options, session.view);
    session.display.compose(session.view, false);
    ConversationUi ui;
    ui.answers_font =
        session.display.dialog_font ? &*session.display.dialog_font : nullptr;
    ui.caption_font =
        session.display.caption_font ? &*session.display.caption_font : nullptr;
    if (ui.answers_font == nullptr) {
        std::cout << "conversation record=" << record_index
                  << " ERROR: fonts/DIALOG.SFN is not loaded; the answer list "
                     "cannot be drawn\n";
        return outcome;
    }
    if (ui.caption_font == nullptr) {
        std::cout << "conversation record=" << record_index
                  << " ERROR: fonts/CAPTIONS.SFN + CAPTIONS.TGA are not loaded; "
                     "the original green subtitle cannot be drawn\n";
        return outcome;
    }
    ui.line_height = std::max(1, ui.answers_font->line_height);
    ui.snapshot_playfield(session);
    session.conversation_frame = &ui.screen;
    if (verbose) {
        std::cout << "conversation_caption source=cleared reason=start\n";
    }
    ui.present(session);

    std::size_t choice_cursor = 0;
    bool answer_shot_written = false;
    bool object_click_used = false;
    bool video_shot_written = false;
    // 0x40d3af type 4 reads FUN_0041d440(1) = gamepref[1]: 0 keeps mode 0
    // (320x200 at x=160, y=120 in the 640x480 surface), nonzero keeps mode 2
    // (source*2 at x=0, y=0x14). The shipped GAMEPREF.000 has 1.
    const bool fullscreen_video = state.gameprefs ? (*state.gameprefs)[1] != 0 : false;
    if (!state.gameprefs) {
        std::cout << "conversation_video mode=unknown (no --gameprefs; using the "
                     "BSS mode 0, 320x200)\n";
    }
    // 0x436b01 calls 0x424780(selected answer) before 0x43340a(entry list): the
    // player's line becomes the subtitle, like any other spoken line.
    const auto show_answer_line = [&](const byzantine::ConversationAnswer& answer) {
        if (!answer.show_text) return;
        append_dialogue_history(session, std::string(), answer.text, true);
        if (!session.options.settings.subtitles) {
            ui.clear_caption();
            ui.present(session);
            return;
        }
        ui.set_caption(answer.text);
        if (verbose) {
            std::cout << "conversation_caption source=answer line=" << answer.line
                      << " visual_lines=" << ui.caption.size() << '\n';
        }
        ui.present(session);
        const auto lines = wrap_dialog_text(*ui.caption_font, answer.text, 530);
        if (session.interactive &&
            !wait_dialog_ms(session,
                            static_cast<std::uint64_t>(lines.size()) * 700)) {
            session.running = false;
        }
    };
    while (session.running) {
        byzantine::ConversationEvent event;
        try {
            event = conversation.next();
        } catch (const std::exception& error) {
            std::cout << "conversation record=" << record_index
                      << " error=" << error.what() << '\n';
            return outcome;
        }
        if (event.kind == byzantine::ConversationEventKind::error) {
            std::cout << "conversation record=" << record_index
                      << " error=" << event.error << '\n';
            return outcome;
        }
        if (event.kind == byzantine::ConversationEventKind::finished) {
            outcome.ok = true;
            outcome.result = event.result;
            outcome.cancelled = event.result == 0xfffe;
            // The subtitle belongs to the conversation screen only; 0x432d6e
            // clears DAT_0045fdb4/0x424c4e before the scene returns.
            ui.clear_caption();
            if (verbose) {
                std::cout << "conversation_caption source=cleared reason="
                          << (outcome.cancelled ? "cancel" : "end")
                          << " result=" << event.result << '\n';
                std::cout << "conversation_end record=" << record_index
                          << " name=" << record.name << " result=" << event.result
                          << " lines=" << outcome.lines
                          << " answer_lists=" << outcome.answer_lists
                          << " media=" << outcome.media
                          << " audio_failures=" << outcome.audio_failures
                          << " instructions=" << conversation.executed_instructions() << '\n';
                std::cout << "conversation_memory_after record=" << record_index
                          << " asked=" << conversation_memory.unspeak.size()
                          << " disabled=" << conversation_memory.disabled.size()
                          << " done=" << (conversation_memory.done ? "yes" : "no")
                          << " default_line="
                          << static_cast<int>(conversation_memory.default_line) << '\n';
            }
            return outcome;
        }
        if (event.kind == byzantine::ConversationEventKind::line) {
            ++outcome.lines;
            const auto line_key = record.name + "#" + std::to_string(event.line.index) +
                                  "#" + event.line.clip;
            const bool seen_before = session.seen_dialogue.count(line_key) != 0;
            const bool skip_line = session.options.settings.skip_seen && seen_before;
            if (event.line.show_text) {
                append_dialogue_history(session, event.line.speaker, event.line.text, false);
            }
            session.seen_dialogue.insert(line_key);
            if (skip_line) {
                std::cout << "conversation_skip record=" << record_index
                          << " index=" << event.line.index
                          << " clip=" << event.line.clip
                          << " reason=seen_before\n";
            }
            // 0x424780 only replaces the subtitle for a line with text; a
            // "null"/no-text line leaves the previous line visible.
            if (event.line.show_text && session.options.settings.subtitles && !skip_line) {
                ui.set_caption(event.line.text);
                if (verbose) {
                    std::cout << "conversation_caption source=npc index="
                              << event.line.index
                              << " visual_lines=" << ui.caption.size() << '\n';
                }
            } else if (!session.options.settings.subtitles || skip_line) {
                ui.clear_caption();
            }
            ui.present(session);
            if (verbose) {
                std::cout << "conversation_line record=" << record_index
                          << " index=" << event.line.index
                          << " clip=" << event.line.clip
                          << " speaker=" << event.line.speaker
                          << " text=" << (event.line.show_text ? "yes" : "no")
                          << " media=" << (event.line.media_allowed ? "yes" : "no")
                          << " group=" << event.group << '\n';
            }
            bool media_played = false;
            if (skip_line) {
                // A replayed line is skipped completely (text and AVI) when the
                // session's skip-seen toggle is on. First-time lines are never
                // skipped; the key is inserted only after this check.
            } else if (event.line.media_allowed && options.dialog.empty()) {
                std::cout << "conversation_media clip=" << event.line.clip
                          << " not configured (--dialog); text stays\n";
            } else if (event.line.media_allowed) {
                const auto avi = conversation_clip_file(options, record.name, event.line.clip);
                if (!avi) {
                    std::cout << "conversation_media clip=" << event.line.clip
                              << " missing (0x40d2a1 logs and returns 0; text stays)\n";
                } else if (!options.video) {
                    std::cout << "conversation_media clip=" << event.line.clip
                              << " skipped (--no-video)\n";
                } else {
                    const auto decoded = dialog_clip_cache(options, *avi, record.name,
                                                           event.line.clip, verbose);
                    if (!decoded) {
                        std::cout << "conversation_media clip=" << event.line.clip
                                  << " unavailable (text stays)\n";
                    } else {
                        // 0x40d5ff clears the 640x480 surface when gamepref[1]
                        // is 0 (the 320x200 mode-0 window); mode 2's 640x400
                        // frame covers the playfield by itself.
                        if (!fullscreen_video) ui.clear_playfield(session);
                        // Mode 0 destination is (160, 120) in the 640x480
                        // surface; the port surface starts at row 0x14.
                        const std::int32_t x = fullscreen_video
                                                   ? 0
                                                   : (kSceneWidth - decoded->width) / 2;
                        const std::int32_t y = fullscreen_video
                                                   ? 0
                                                   : (kSceneHeight - decoded->height) / 2;
                        const auto playback = play_decoded_clip(
                            session, *decoded, fullscreen_video, x, y,
                            video_shot_written ? std::string()
                                               : options.conversation_video_shot,
                            kConversationClipTags, event.line.clip,
                            // 0x413bdd draws the clip over the scene surface;
                            // the port's hotspot outline is a diagnostic and
                            // must not sit on top of the FMV.
                            [&] { ui.present(session); },
                            [&](const std::string& path) {
                                return ui.write(session, path);
                            });
                        outcome.audio_failures += playback.audio_failures;
                        if (playback.shot_written) video_shot_written = true;
                        // The last frame stays on the surface, exactly like the
                        // original's DirectDraw target; 0x436b01 draws the
                        // answers over it.
                        ui.snapshot_playfield(session);
                        media_played = true;
                        ++outcome.media;
                    }
                }
            }
            if (!media_played && event.line.show_text && session.interactive &&
                !skip_line) {
                // 0x424981's reveal: roughly one second per visual line.
                if (!wait_dialog_ms(
                        session, static_cast<std::uint64_t>(ui.caption.size()) * 700)) {
                    break;
                }
            }
            conversation.line_shown();
            continue;
        }
        if (event.kind == byzantine::ConversationEventKind::answers) {
            ++outcome.answer_lists;
            if (verbose) {
                std::cout << "conversation_answers record=" << record_index
                          << " group=" << event.group
                          << " choices=" << event.answers.size() << '\n';
                for (const auto& answer : event.answers) {
                    std::cout << "  choice=" << answer.choice
                              << " line=" << answer.line
                              << " clip=" << answer.clip
                              << " text=" << (answer.show_text ? "yes" : "no")
                              << " media=" << (answer.media_allowed ? "yes" : "no") << '\n';
                }
            }
            if (event.answers.empty()) {
                std::cout << "conversation record=" << record_index
                          << " no answers (0x436b01 sets done)\n";
                return outcome;
            }
            if (options.conversation_cancel > 0 &&
                outcome.answer_lists ==
                    static_cast<std::size_t>(options.conversation_cancel)) {
                conversation.cancel();
                ui.clear_caption();
                outcome.ok = true;
                outcome.cancelled = true;
                outcome.result = 0xfffe;
                std::cout << "conversation_cancel record=" << record_index
                          << " answer_list=" << outcome.answer_lists
                          << " result=0xfffe (scene session ends; no automatic reload)\n";
                std::cout << "conversation_memory_after record=" << record_index
                          << " asked=" << conversation_memory.unspeak.size()
                          << " disabled=" << conversation_memory.disabled.size()
                          << " done=" << (conversation_memory.done ? "yes" : "no")
                          << " default_line="
                          << static_cast<int>(conversation_memory.default_line) << '\n';
                return outcome;
            }
            if (!options.conversation_shot.empty() && !answer_shot_written) {
                std::vector<SDL_Rect> shot_rects;
                ui.draw_answers(session, event.answers, -1, &shot_rects);
                ui.present(session);
                if (verbose) {
                    for (std::size_t index = 0; index < shot_rects.size(); ++index) {
                        const auto& rect = shot_rects[index];
                        std::cout << "conversation_answer_rect choice=" << index
                                  << " x=" << rect.x << " y=" << rect.y
                                  << " w=" << rect.w << " h=" << rect.h << '\n';
                    }
                }
                if (ui.write(session, options.conversation_shot)) {
                    std::cout << "conversation_shot " << options.conversation_shot
                              << " group=" << event.group << '\n';
                }
                answer_shot_written = true;
            }
            if (!session.interactive) {
                if (options.conversation_object && !object_click_used) {
                    object_click_used = true;
                    std::cout << "conversation_object record=" << record_index
                              << " slot=" << *options.conversation_object << '\n';
                    conversation.click_object(
                        static_cast<std::uint16_t>(*options.conversation_object));
                    continue;
                }
                if (choice_cursor >= options.conversation_choices.size()) {
                    std::cout << "conversation record=" << record_index
                              << " waiting for an answer but --conversation-choices "
                                 "is exhausted\n";
                    return outcome;
                }
                const auto choice = options.conversation_choices[choice_cursor++];
                if (choice >= event.answers.size()) {
                    std::cout << "conversation record=" << record_index
                              << " choice=" << choice << " outside the "
                              << event.answers.size() << " answers\n";
                    return outcome;
                }
                // Play the selected answer's line before its script (0x436b01
                // calls 0x424780 before 0x43340a).
                std::cout << "conversation_choice record=" << record_index
                          << " choice=" << choice
                          << " line=" << event.answers[choice].line << '\n';
                show_answer_line(event.answers[choice]);
                try {
                    conversation.choose(choice);
                } catch (const std::exception& error) {
                    std::cout << "conversation record=" << record_index
                              << " answer_error=" << error.what() << '\n';
                    return outcome;
                }
                continue;
            }
            // 0x436b01's ESC branch is FUN_0041d6df: a 4-button dialog whose
            // message is MESSAGES.TXT id 9 and whose labels are ids 0x4c..0x4f.
            // Only result 6 (EVET) takes the 0xfffe path; 1/2/7 (TAMAM/İPTAL/
            // HAYIR) close the dialog and return to the same answer list.
            std::string menu_message = "Kaydetmeden oyundan cikmak istiyor musunuz?";
            std::vector<std::pair<std::string, int>> menu_buttons;
            {
                const char* const fallback[4] = {"EVET", "HAYIR", "TAMAM", "IPTAL"};
                const std::size_t label_ids[4] = {0x4c, 0x4d, 0x4e, 0x4f};
                const int results[4] = {6, 7, 1, 2};
                if (session.messages.size() > 9) menu_message = session.messages[9];
                for (int i = 0; i < 4; ++i) {
                    menu_buttons.emplace_back(
                        session.messages.size() > label_ids[i] ? session.messages[label_ids[i]]
                                                               : fallback[i],
                        results[i]);
                }
                if (verbose) {
                    std::cout << "conversation_menu_ready message_id=9 buttons=4"
                              << " source=" << (session.messages.empty() ? "fallback"
                                                                         : "messages")
                              << '\n';
                }
            }
            std::vector<SDL_Rect> rects;
            std::vector<SDL_Rect> menu_rects;
            int hovered = -1;
            int last_hovered = -2;
            bool menu_open = false;
            bool picked = false;
            bool cancelled = false;
            const auto terminate_from_menu = [&]() {
                // 0x436b01 result 6: 0xfffe, DAT_0045f48c = 1. The caller ends
                // the scene session; nothing is reloaded here.
                conversation.cancel();
                ui.clear_caption();
                outcome.ok = true;
                outcome.cancelled = true;
                outcome.result = 0xfffe;
                cancelled = true;
                picked = true;
            };
            while (session.running && !picked) {
                // 0x436b01 redraws the list on every pointer move: the option
                // under the pointer uses its white (0xffff) colour. The rects
                // are screen coordinates, so the hit area is exactly the drawn
                // text's width and line block.
                float mouse_x = 0;
                float mouse_y = 0;
                session.display.mouse_logical(&mouse_x, &mouse_y);
                hovered = menu_open ? answer_at(menu_rects, mouse_x, mouse_y)
                                    : answer_at(rects, mouse_x, mouse_y);
                if (verbose && hovered != last_hovered) {
                    std::cout << (menu_open ? "conversation_menu_hover button="
                                            : "conversation_hover choice=")
                              << hovered << " logical="
                              << static_cast<int>(mouse_x) << ','
                              << static_cast<int>(mouse_y) << '\n';
                    last_hovered = hovered;
                }
                ui.draw_answers(session, event.answers, menu_open ? -1 : hovered, &rects);
                if (menu_open) {
                    ui.draw_menu(session, menu_message, menu_buttons, hovered,
                                 &menu_rects);
                }
                ui.present(session);
                if (!options.conversation_shot.empty() && !answer_shot_written) {
                    if (ui.write(session, options.conversation_shot)) {
                        std::cout << "conversation_shot " << options.conversation_shot
                                  << " group=" << event.group << '\n';
                    }
                    answer_shot_written = true;
                }
                if (options.selftest_events && !menu_open && !rects.empty()) {
                    // Prove the real answer hit test: a logical click at the
                    // centre of the first answer row, delivered through SDL.
                    int window_x = 0;
                    int window_y = 0;
                    SDL_RenderLogicalToWindow(
                        session.display.renderer,
                        static_cast<float>(rects.front().x + rects.front().w / 2),
                        static_cast<float>(rects.front().y + rects.front().h / 2),
                        &window_x, &window_y);
                    SDL_Event injected{};
                    injected.type = SDL_MOUSEBUTTONUP;
                    injected.button.button = SDL_BUTTON_LEFT;
                    injected.button.x = window_x;
                    injected.button.y = window_y;
                    SDL_PushEvent(&injected);
                }
                SDL_Event event_poll;
                while (poll_logged_event(&event_poll, "conversation_answers", session)) {
                    ++session.wait.events;
                    if (event_poll.type == SDL_QUIT) {
                        session.running = false;
                    } else if (event_poll.type == SDL_KEYDOWN &&
                               event_poll.key.keysym.sym == SDLK_ESCAPE) {
                        // The original ESC opens/cancels the dialog; it never
                        // cancels the conversation directly.
                        if (!menu_open) {
                            menu_open = true;
                            menu_rects.clear();
                            last_hovered = -2;
                            std::cout << "conversation_menu open message_id=9 "
                                         "action=return_to_conversation_or_terminate\n";
                        } else {
                            menu_open = false;
                            std::cout << "conversation_menu result=2 action=resume "
                                         "(same answer list, memory and variables)\n";
                        }
                    } else if (menu_open && event_poll.type == SDL_KEYDOWN) {
                        const auto key = event_poll.key.keysym.sym;
                        const int result = key == SDLK_1   ? 1
                                           : key == SDLK_2 ? 2
                                           : key == SDLK_6 ? 6
                                           : key == SDLK_7 ? 7
                                                           : 0;
                        if (result != 0) {
                            std::cout << "conversation_menu result=" << result
                                      << " action="
                                      << (result == 6 ? "terminate" : "resume")
                                      << '\n';
                            menu_open = false;
                            if (result == 6) {
                                terminate_from_menu();
                                break;
                            }
                        }
                    } else if (event_poll.type == SDL_KEYDOWN &&
                               key_is(session.options.settings,
                                      session.options.settings.key_hotspots,
                                      event_poll.key.keysym.sym)) {
                        session.options.overlay = !session.options.overlay;
                    } else if (event_poll.type == SDL_KEYDOWN &&
                               key_is(session.options.settings,
                                      session.options.settings.key_backlog,
                                      event_poll.key.keysym.sym)) {
                        backlog_overlay(session);
                    } else if (event_poll.type == SDL_KEYDOWN &&
                               key_is(session.options.settings,
                                      session.options.settings.key_skip,
                                      event_poll.key.keysym.sym)) {
                        session.options.settings.skip_seen = !session.options.settings.skip_seen;
                        std::cout << "dialogue_skip_seen enabled="
                                  << (session.options.settings.skip_seen ? 1 : 0) << '\n';
                    } else if (event_poll.type == SDL_MOUSEBUTTONUP &&
                               event_poll.button.button == SDL_BUTTON_LEFT) {
                        // SDL_RendererEventWatch already made button.x/y logical.
                        const float logical_x = static_cast<float>(event_poll.button.x);
                        const float logical_y = static_cast<float>(event_poll.button.y);
                        session.display.log_mouse_click(logical_x, logical_y, "conversation");
                        if (menu_open) {
                            const auto index = answer_at(menu_rects, logical_x, logical_y);
                            if (index >= 0) {
                                const int result = menu_buttons[index].second;
                                std::cout << "conversation_menu result=" << result
                                          << " action="
                                          << (result == 6 ? "terminate" : "resume")
                                          << '\n';
                                menu_open = false;
                                if (result == 6) {
                                    terminate_from_menu();
                                    break;
                                }
                            }
                            continue;
                        }
                        const auto index = answer_at(rects, logical_x, logical_y);
                        if (index >= 0) {
                            if (verbose) {
                                std::cout << "conversation_pick record=" << record_index
                                          << " choice=" << index << '\n';
                            }
                            show_answer_line(event.answers[index]);
                            try {
                                conversation.choose(static_cast<std::size_t>(index));
                            } catch (const std::exception& error) {
                                std::cout << "conversation record=" << record_index
                                          << " answer_error=" << error.what() << '\n';
                                return outcome;
                            }
                            picked = true;
                            break;
                        }
                        // 0x436b01 also returns a scene-object click to 0x70.
                        // The conversation screen places the playfield at row
                        // 0x14, while hotspot_at wants playfield coordinates.
                        if (logical_y >= kScreenPlayfieldTop) {
                            const auto hit = hotspot_at(
                                session.view, static_cast<int>(logical_x),
                                static_cast<int>(logical_y) - kScreenPlayfieldTop);
                            if (hit.slot) {
                                if (verbose) {
                                    std::cout << "conversation_object record="
                                              << record_index << " slot=" << *hit.slot
                                              << '\n';
                                }
                                conversation.click_object(*hit.slot);
                                picked = true;
                                break;
                            }
                        }
                    }
                }
                SDL_Delay(16);
            }
            if (cancelled) return outcome;
            if (!picked) return outcome;
            continue;
        }
        return outcome;
    }
    return outcome;
}

bool play_hints_conversation(Session& session) {
    // 0x41e002: load HINTS.JPG, then 0x410669("Hints1") + 0x432d6e runs the
    // hints conversation with the HINTS scene's own cd/ci/cy tables.
    if (!session.metadata) return false;
    int hint_scene = -1;
    for (std::size_t index = 0; index < session.metadata->scenes.size(); ++index) {
        if (lower_ascii(session.metadata->scenes[index].scene) == "hints") {
            hint_scene = static_cast<int>(index);
            break;
        }
    }
    if (hint_scene < 0) return false;
    const auto cc = sibling_scene_file(session.view.cc_path, "hints");
    const auto cb = sibling_scene_file(session.view.cb_path, "hints");
    if (!cc || !cb) return false;
    auto next = session.options.scene;
    next.erase("--save");
    for (const char* key : {"--ci", "--cy", "--cf", "--cs", "--ca", "--co", "--ce", "--cd"}) {
        const auto found = next.find(key);
        if (found == next.end()) continue;
        const auto file = sibling_scene_file(found->second, "hints");
        if (file) found->second = file->string();
        else next.erase(found);
    }
    const auto saved_scene = session.options.scene;
    const auto saved_cc = session.options.cc_path;
    const auto saved_cb = session.options.cb_path;
    SceneView saved_view = session.view;
    const auto saved_surface = session.display.surface;
    SceneView hints;
    hints.cc_path = cc->string();
    hints.cb_path = cb->string();
    try {
        hints.script = byzantine::parse_cc(read_file(hints.cc_path));
        hints.entries = byzantine::parse_cb(read_file(hints.cb_path));
    } catch (const std::exception& error) {
        std::cout << "menu_hints tables: " << error.what() << '\n';
        return false;
    }
    hints.name = "HINTS";
    auto setup = byzantine::load_scene_setup(hints.cc_path, next);
    hints.entry_state = setup.state;
    hints.state = setup.state;
    hints.pool_on_arrival = setup.state.variable_pool;
    session.view = std::move(hints);
    session.options.scene = next;
    session.options.cc_path = session.view.cc_path;
    session.options.cb_path = session.view.cb_path;
    // HINTS.JPG is a 640x480 screen; the conversation compositor places the
    // playfield at row 0x14, so copy the matching rows.
    std::optional<std::filesystem::path> hints_file =
        find_language_child(session.options, false, "setupart", "hints");
    if (!hints_file) {
        const auto root = MenuUi::setup_dir(session.options);
        if (root) hints_file = find_child(*root, "hints");
    }
    if (hints_file) {
        {
            const auto& file = hints_file;
            try {
                const auto image = byzantine::decode_jpeg_file(file->string());
                byzantine::Rgb16Surface surface =
                    byzantine::make_surface(kSceneWidth, kSceneHeight, 0);
                for (int row = 0; row < kSceneHeight && row + kScreenPlayfieldTop < image.height;
                     ++row) {
                    for (int column = 0; column < kSceneWidth && column < image.width;
                         ++column) {
                        const auto source =
                            (static_cast<std::size_t>(row + kScreenPlayfieldTop) *
                                 static_cast<std::size_t>(image.width) +
                             static_cast<std::size_t>(column)) *
                            3;
                        const auto r = image.rgb[source];
                        const auto g = image.rgb[source + 1];
                        const auto b = image.rgb[source + 2];
                        surface.pixels[static_cast<std::size_t>(row) * kSceneWidth +
                                        column] = static_cast<std::uint16_t>(
                            ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
                    }
                }
                session.display.surface = std::move(surface);
            } catch (const std::exception& error) {
                std::cout << "menu_hints image: " << error.what() << '\n';
            }
        }
    }
    std::cout << "menu_hints conversation=Hints1 record=0 scene=HINTS\n";
    const auto outcome = run_conversation(session, 0, 0, true);
    session.view = std::move(saved_view);
    session.display.surface = std::move(saved_surface);
    session.options.scene = saved_scene;
    session.options.cc_path = saved_cc;
    session.options.cb_path = saved_cb;
    std::cout << "menu_hints conversation_end ok=" << (outcome.ok ? "yes" : "no")
              << " name=" << outcome.name << " lines=" << outcome.lines
              << " answers=" << outcome.answer_lists << " media=" << outcome.media
              << '\n';
    return outcome.ok;
}

// 0xfffe makes 0x42db22 return 1; 0x42d594 stores DAT_00484e28 and clears the
// script pointer, then 0x42c8a0 calls FUN_0041e289(FUN_0041de4b()). The ESC
// path set DAT_0045f48c, so FUN_0041e289(6) returns 1 and the scene session
// ends. The original next shows the main menu; the port has no main menu, so a
// fresh scene load is offered only as an explicit user choice (R), never
// silently. It is the same 0x42c8a0 head load: clear the tables and run the
// entry from the supplied save.
bool restart_scene(Session& session,
                   const std::function<bool(EntryPoint, const byzantine::SceneState&,
                                            const byzantine::SceneTrace&)>& run,
                   bool verbose, const char* reason) {
    // Reload this scene from its arrival entry_state, not from the global save:
    // a 0x73 travel removed --save and carries its own pool. The startup scene's
    // entry_state is the save state, so this is the same fresh load there.
    session.return_armed = false;
    session.return_view = SceneView{};
    if (session.audio.ready() && session.audio.ambient() != 0) {
        const auto voices = session.audio.ambient();
        session.audio.stop_all();
        std::cout << "ambient_scene_stop reason=" << reason
                  << " voices=" << voices << '\n';
    }
    SceneView fresh;
    fresh.name = session.view.name;
    fresh.cc_path = session.view.cc_path;
    fresh.cb_path = session.view.cb_path;
    fresh.script = std::move(session.view.script);
    fresh.entries = std::move(session.view.entries);
    fresh.entry_state = session.view.entry_state;
    // A TERCICHLER toggle edits the live state; a reload must not revive the
    // prefs snapshot taken when the scene was first entered.
    if (session.view.state.gameprefs) {
        fresh.entry_state.gameprefs = session.view.state.gameprefs;
    }
    fresh.state = fresh.entry_state;
    fresh.pool_on_arrival = session.view.pool_on_arrival;
    fresh.panorama_size = session.view.panorama_size;
    fresh.panorama = std::move(session.view.panorama);
    fresh.layer = SceneLayer::none;
    fresh.reload_point = session.view.reload_point;
    fresh.label = session.view.reload_point.label;
    session.view = std::move(fresh);
    session.reload_available = false;
    load_scene_text(session, verbose);
    const EntryPoint point{session.view.reload_point.label,
                           session.view.reload_point.at_entry, std::nullopt};
    byzantine::SceneState probe_state = session.view.entry_state;
    const auto probe = point.at_entry
        ? byzantine::trace_scene_entry(session.view.script, session.view.entries,
                                       probe_state, 0)
        : byzantine::trace_scene_label(session.view.script, session.view.entries,
                                       probe_state, point.label);
    bool entry_session_ended = false;
    if (probe.conversation_request) {
        // Same as travel_to: a 0x70 at the head opens the arrival dialogue, and
        // only its continuation restores the hotspots 0xb8 suspended.
        ClickPlan plan;
        plan.ok = true;
        plan.conversation = true;
        plan.point = point;
        plan.entry = probe_state;
        plan.entry_before = session.view.entry_state;
        plan.probe = probe;
        std::cout << "entry_conversation record="
                  << probe.conversation_request->record
                  << " instruction=" << probe.conversation_request->instruction
                  << " object_context=" << probe.conversation_request->object_context
                  << '\n';
        if (!execute_plan(session, plan, run, verbose, &entry_session_ended)) {
            std::cout << "entry_conversation not committed (scene keeps the "
                         "pre-dialogue state)\n";
        }
    } else if (!run(point, session.view.entry_state, probe)) {
        return false;
    }
    apply_panorama_events(session.view, session.options, verbose);
    if (!entry_session_ended) show_message_text(session, verbose);
    resume_ambient(session, verbose);
    refresh_background(session.options, session.view);
    present_arrival(session, verbose);
    std::cout << "scene_restart reason=" << reason << " scene=" << session.view.name
              << " point=" << (point.at_entry ? "entry" : "label")
              << " hotspots=" << session.view.trace.active_hotspots.size()
              << " steps=" << session.view.trace.steps << '\n';
    warn_if_nothing_clickable(session.view, "restart");
    return true;
}

// Runs a click plan end to end. For a 0x70 plan the pre-conversation segment is
// played first (its sounds included), then the real conversation runs on the
// state at the pause, and the script resumes at the instruction the result
// selects. `run` is the caller's segment runner (audio-gated interactive or one
// unbudgeted pass in --shot). A 0xfffe result ends the scene session; the caller
// must skip the old session's post-click updates and offer the explicit reload.
bool execute_plan(Session& session, const ClickPlan& plan,
                  const std::function<bool(EntryPoint, const byzantine::SceneState&,
                                           const byzantine::SceneTrace&)>& run,
                  bool verbose, bool* session_ended) {
    if (!plan.conversation) {
        // plan.entry is the already-mutated probe result. Re-executing from it
        // applies writes twice and can choose a different branch on the replay.
        return run(plan.point, plan.entry_before, plan.probe);
    }
    const auto request = *plan.probe.conversation_request;
    if (!run(plan.point, plan.entry_before, plan.probe)) return false;
    apply_panorama_events(session.view, session.options, verbose);
    refresh_background(session.options, session.view);
    session.display.compose(session.view, false);
    session.display.present(session.view);
    {
        std::ostringstream entry_state;
        entry_state << "conversation_entry locals_digest=0x" << std::hex
                    << locals_digest(session.view.state)
                    << " state_fingerprint=0x" << state_fingerprint(session.view.state);
        std::cout << entry_state.str() << std::dec << '\n';
    }

    const auto before_conversation = session.view.state;
    const auto outcome = run_conversation(session, request.record, request.object_context,
                                          verbose);
    // The conversation screen is local to run_conversation; stop showing it (and
    // with it the last subtitle) before the game scene returns.
    session.conversation_frame = nullptr;
    // The conversation UI may have left the last media frame in the playfield.
    // Restore the scene layer immediately so the normal 640x480 view (with the
    // inventory strip) is what the caller and the player see on return.
    refresh_background(session.options, session.view);
    session.display.compose(session.view, false);
    session.display.present(session.view);
    if (!outcome.ok) {
        refresh_background(session.options, session.view);
        session.display.compose(session.view, false);
        session.display.present(session.view);
        std::cout << "conversation stopped before completion; the click is not committed\n";
        return false;
    }
    // The scene state outside the conversation's own variable writes must
    // survive it: 0x432d6e only edits DAT_00482ad0 plus whatever the .ccb's
    // 0x67/0x74 commands write.
    {
        std::size_t changed = 0;
        for (const auto& [local, value] : before_conversation.variables) {
            const auto now = session.view.state.variables.find(local);
            if (now != session.view.state.variables.end() && now->second != value) {
                if (verbose) {
                    std::cout << "conversation_variable local=" << local
                              << " before=" << value << " after=" << now->second << '\n';
                }
                ++changed;
            }
        }
        const auto enabled_count = [](const byzantine::SceneState& state) {
            std::size_t count = 0;
            for (const auto& [slot, enabled] : state.hotspot_enabled) {
                (void)slot;
                if (enabled) ++count;
            }
            return count;
        };
        std::cout << "conversation_state locals=" << before_conversation.variables.size()
                  << " changed_locals=" << changed
                  << " objects=" << before_conversation.objects.size() << '/'
                  << session.view.state.objects.size()
                  << " hotspots=" << before_conversation.hotspot_enabled.size() << '/'
                  << session.view.state.hotspot_enabled.size()
                  << " enabled=" << enabled_count(before_conversation) << '/'
                  << enabled_count(session.view.state)
                  << " suspended=" << before_conversation.suspended_hotspots.size() << '/'
                  << session.view.state.suspended_hotspots.size()
                  << " records=" << before_conversation.records.size() << '/'
                  << session.view.state.records.size() << '\n';
    }
    if (outcome.cancelled || outcome.result == 0xfffe) {
        // 0x42db22 case 0x70: 0xfffe sets local_8 = 1. 0x42d594 stores the
        // script pointer and 0x42c8a0's t==1 branch runs
        // FUN_0041e289(FUN_0041de4b()); DAT_0045f48c makes that return 1, so
        // the scene session ends. The port leaves the scene exactly as it is
        // (the 0xb8 snapshot is not restored) and offers a fresh load only as
        // the explicit R choice. No progress is rolled back silently.
        std::cout << "conversation_branch terminate (0xfffe: 0x42db22 returns 1, "
                     "0x42c8a0 ends the scene session)\n";
        session.reload_available = true;
        // The original returns to the main menu. The port has no main menu, so
        // the scene message window carries the explicit choice. This is port
        // UI text, not a recovered game string.
        session.view.message_text.active = true;
        session.view.message_text.text = byzantine::cp1254_to_game_text(
            "OTURUM SONLANDI\nYENİDEN YÜKLE: R\nÇIKIŞ: ESC");
        refresh_background(session.options, session.view);
        session.display.compose(session.view, false);
        session.display.present(session.view);
        std::cout << "scene_session_ended reason=conversation_exit hotspots="
                  << session.view.trace.active_hotspots.size() << '\n';
        std::cout << "reload_choice available=yes key=R scene=" << session.view.name
                  << " (explicit user choice; not automatic)\n";
        if (session_ended != nullptr) *session_ended = true;
        return true;
    }
    std::size_t resume = 0;
    try {
        resume = byzantine::apply_conversation_result(session.view.script,
                                                      session.view.state,
                                                      request.instruction,
                                                      outcome.result);
    } catch (const std::exception& error) {
        std::cout << "conversation result=" << outcome.result
                  << " cannot be applied: " << error.what() << '\n';
        return false;
    }
    std::cout << "conversation record=" << request.record << " name=" << outcome.name
              << " result=" << outcome.result << " lines=" << outcome.lines
              << " answer_lists=" << outcome.answer_lists << " media=" << outcome.media
              << " audio_failures=" << outcome.audio_failures
              << " resume_instruction=" << resume << '\n';
    if (outcome.audio_failures != 0) {
        std::cout << "conversation_audio ERROR " << outcome.audio_failures
                  << " clip(s) had no playable audio; see the per-clip errors above\n";
    }
    if (outcome.result == 0xffff) {
        std::cout << "conversation_branch none (0xffff: script continues after the 0x70)\n";
    } else {
        std::cout << "conversation_branch object=" << outcome.result << " label="
                  << (session.view.state.objects.count(outcome.result) != 0
                          ? std::to_string(
                                session.view.state.objects.at(outcome.result).label)
                          : std::string("?"))
                  << " target=" << resume << '\n';
    }
    // The conversation wrote scene variables through 0x67/0x74; those writes are
    // in session.view.state and are the entry for the resumed segment.
    const auto resume_entry = session.view.state;
    byzantine::SceneState resume_probe_state = resume_entry;
    const auto resumed_probe = byzantine::trace_scene_from_instruction(
        session.view.script, session.view.entries, resume_probe_state, resume, 0);
    std::cout << "conversation_resume_probe steps=" << resumed_probe.steps
              << " stopped_at=" << resumed_probe.stopped_at
              << " switch="
              << (resumed_probe.scene_switch_requested
                      ? std::to_string(*resumed_probe.scene_switch_requested)
                      : std::string("none"))
              << " sounds=" << sound_events(resumed_probe).size()
              << " blocker=" << byzantine::trace_blocker(resumed_probe) << '\n';
    if (!run(EntryPoint{0, false, resume}, resume_entry, resumed_probe)) return false;
    // The resumed segment is a real continuation of the same script; if it
    // stops on data the port lacks, the action must not be committed as a
    // success (the caller restores the pre-click state).
    const auto resumed_blocker = byzantine::trace_blocker(session.view.trace);
    if (!resumed_blocker.empty()) {
        std::cout << "conversation resumed segment stopped: " << resumed_blocker
                  << " at instruction " << session.view.trace.stopped_at
                  << "; the click is not committed\n";
        return false;
    }
    return true;
}

void load_fonts(Session& session, bool verbose) {
    auto& options = session.options;
    session.display.examine_font.reset();
    session.display.message_font.reset();
    session.display.dialog_font.reset();
    session.display.setup_font.reset();
    session.display.caption_font.reset();
    if (options.fonts.empty() && options.language.text == nullptr) {
        std::cout << "fonts disabled: no --fonts and no text pack (the text windows stay blank)\n";
        return;
    }
    const auto child = [&](const char* name) -> std::optional<std::filesystem::path> {
        if (const auto file = find_language_child(options, false, "fonts", name)) return file;
        if (!options.fonts.empty()) return find_child(options.fonts, name);
        return std::nullopt;
    };
    const auto exact = [&](const char* name) -> std::optional<std::filesystem::path> {
        if (const auto file = find_language_filename(options, false, "fonts", name)) return file;
        if (!options.fonts.empty()) return find_filename(options.fonts, name);
        return std::nullopt;
    };
    const bool english = options.language.english_font_branch();
    const auto mark = [&](auto& font) {
        if (font) font->english_high_bytes = english;
    };

    const auto sfn = child("klio");
    const auto gr = child("gr_klio");
    const auto bk = child("bk_klio");
    if (sfn && gr && bk) {
        try {
            // 0x406cfe slots 1 and 3: the same klio metrics/atlas, fill
            // 0x1b<<5 (examine) and (0xb<<11)|(0xe<<6)|2 (message).
            session.display.examine_font = byzantine::load_sfn_font(
                sfn->string(), gr->string(), bk->string(), 0x0360);
            session.display.message_font = byzantine::load_sfn_font(
                sfn->string(), gr->string(), bk->string(), 0x5b82);
            mark(session.display.examine_font);
            mark(session.display.message_font);
            if (verbose) {
                std::cout << "fonts klio glyphs="
                          << session.display.examine_font->glyph_count
                          << " atlas=" << session.display.examine_font->atlas_width
                          << 'x' << session.display.examine_font->atlas_height
                          << " line=" << session.display.examine_font->line_height
                          << " language=" << options.language.text_code_or_base()
                          << " english_high=" << (english ? 1 : 0) << '\n';
            }
        } catch (const std::exception& error) {
            std::cout << "fonts disabled: " << error.what() << '\n';
        }
    } else {
        std::cout << "fonts disabled: klio.SFN/GR_KLIO.TGA/BK_KLIO.TGA not under "
                  << options.fonts.string() << '\n';
    }

    const auto dlg = child("dialog");
    const auto dlg_gr = child("gr_dlg");
    const auto dlg_bk = child("bk_dlg");
    if (dlg && dlg_gr && dlg_bk) {
        try {
            // 0x406cfe slot 0: DIALOG.SFN + GR_DLG/BK_DLG, fill
            // (0x16 << green) | (0x1c << red) = 0xe2c0 in RGB565.
            session.display.dialog_font = byzantine::load_sfn_font(
                dlg->string(), dlg_gr->string(), dlg_bk->string(), 0xe2c0);
            mark(session.display.dialog_font);
            if (verbose) {
                std::cout << "fonts dialog glyphs="
                          << session.display.dialog_font->glyph_count
                          << " atlas=" << session.display.dialog_font->atlas_width
                          << 'x' << session.display.dialog_font->atlas_height
                          << " line=" << session.display.dialog_font->line_height
                          << '\n';
            }
        } catch (const std::exception& error) {
            std::cout << "dialog font disabled: " << error.what() << '\n';
        }
    } else {
        std::cout << "dialog font disabled: DIALOG.SFN/GR_DLG.TGA/BK_DLG.TGA not under "
                  << options.fonts.string() << '\n';
    }

    // 0x406cfe slot 2: fonts/setup.sfn + setup.tga. The menu lists
    // (0x4202ff save slots, 0x421ffc keşif rows) draw with this slot.
    const auto setup_sfn = exact("setup.sfn");
    const auto setup_tga = exact("setup.tga");
    if (setup_sfn && setup_tga) {
        try {
            session.display.setup_font = byzantine::load_sfn_font_single_atlas(
                setup_sfn->string(), setup_tga->string(), 0x000f);
            mark(session.display.setup_font);
            if (verbose) {
                std::cout << "fonts setup glyphs="
                          << session.display.setup_font->glyph_count
                          << " atlas=" << session.display.setup_font->atlas_width
                          << 'x' << session.display.setup_font->atlas_height
                          << " line=" << session.display.setup_font->line_height
                          << '\n';
            }
        } catch (const std::exception& error) {
            std::cout << "setup font disabled: " << error.what() << '\n';
        }
    } else {
        std::cout << "setup font disabled: setup.sfn/setup.tga not under "
                  << options.fonts << " (menu lists use DIALOG)\n";
    }

    const auto cap = exact("CAPTIONS.SFN");
    const auto cap_tga = exact("CAPTIONS.TGA");
    if (cap && cap_tga) {
        try {
            // 0x406cfe slot 4: CAPTIONS.SFN + CAPTIONS.TGA, green 0x03eb.
            // 0x424981 draws the conversation subtitle.
            session.display.caption_font =
                byzantine::load_sfn_font_single_atlas(cap->string(),
                                                      cap_tga->string(), 0x03eb);
            mark(session.display.caption_font);
            if (verbose) {
                std::cout << "fonts captions glyphs="
                          << session.display.caption_font->glyph_count
                          << " atlas=" << session.display.caption_font->atlas_width
                          << 'x' << session.display.caption_font->atlas_height
                          << " line=" << session.display.caption_font->line_height
                          << '\n';
            }
            // The shipped Turkish CAPTIONS.SFN/TGA carries only some Turkish
            // glyphs. Only check that set for the non-English branch; the
            // Turkish bytes are not meaningful in an English/font pack.
            if (!english) {
                const char turkish_bytes[] = {'\xa1', '\xa2', '\xa3', '\xa4',
                                              '\xa5', '\xa6', '\xa7', '\xa8',
                                              '\xaa', '\xab', '\xac', '\xad'};
                std::string missing;
                for (const char byte : turkish_bytes) {
                    const auto* glyph = session.display.caption_font->glyph(byte);
                    if (glyph == nullptr || glyph->advance <= 2) missing.push_back(byte);
                }
                if (!missing.empty() && session.display.dialog_font) {
                    std::cout << "fonts captions: " << missing.size()
                              << " Turkish glyph(s) are blank in CAPTIONS.SFN"
                              << "; using DIALOG.SFN's glyph shapes for the green subtitle\n";
                    session.display.caption_font = session.display.dialog_font;
                }
            }
        } catch (const std::exception& error) {
            std::cout << "caption font disabled: " << error.what() << '\n';
        }
    } else {
        std::cout << "caption font disabled: CAPTIONS.SFN/CAPTIONS.TGA not under "
                  << options.fonts.string()
                  << " (conversations report an error instead of guessing)\n";
    }
}



// Host-only settings overlay. It is deliberately separate from the original
// original preference values. Both entry points use the shipped notebook;
// ESC returns through category, hub and preferences. Host changes are written
// to the port INI and applied immediately where possible.
struct SettingsUi {
    bool open = false;
    int group = 0;
    int row = 0;
    bool capturing = false;
    int capture_action = -1;
    bool from_prefs = false;
    std::optional<byzantine::SfnFont> notebook_face, notebook_prose;
    std::optional<byzantine::Rgb16Surface> backdrop;
    std::vector<SDL_Rect> tab_rects;
    std::vector<SDL_Rect> row_rects;

    enum Action {
        window_mode = 0,
        scale_mode,
        vsync,
        cursor_scale,
        master,
        speech,
        ambient,
        effects,
        subtitles,
        text_language,
        voice_language,
        hotspots,
        autosave,
        skip_seen,
        gamepad,
        key_settings,
        key_backlog,
        key_skip,
        key_quicksave,
        key_quickload,
        key_autoload,
        key_screenshot,
        key_hotspots,
        original_autosave,
        original_video,
        original_audio,
        original_notes,
        original_tutorial,
        original_location,
        original_scope
    };

    enum Group { display, audio, language, session, keys, original, group_count };

    static const char* group_name(int value) { return advanced_group_name(value); }

    static std::vector<Action> actions_for(int value) {
        switch (value) {
            case display:
                return {window_mode, scale_mode, vsync, cursor_scale};
            case audio:
                return {master, speech, ambient, effects};
            case language:
                return {text_language, voice_language, subtitles};
            case session:
                return {autosave, skip_seen, hotspots, gamepad};
            case keys:
                return {key_settings, key_backlog, key_skip, key_quicksave,
                        key_quickload, key_autoload, key_screenshot, key_hotspots};
            case original:
                return {original_autosave, original_video, original_audio,
                        original_notes, original_tutorial, original_location};
            default:
                return {};
        }
    }

    static std::vector<Action> all_actions() {
        auto result = actions_for(display);
        for (int g = audio; g < group_count; ++g) {
            const auto rows = actions_for(g);
            result.insert(result.end(), rows.begin(), rows.end());
        }
        return result;
    }

    static const byzantine::SfnFont* font(const Session& session) {
        if (session.display.setup_font) return &*session.display.setup_font;
        return session.display.dialog_font ? &*session.display.dialog_font : nullptr;
    }

    static const byzantine::SfnFont* description_font(const Session& session) {
        if (session.display.dialog_font) return &*session.display.dialog_font;
        return session.display.setup_font ? &*session.display.setup_font : nullptr;
    }

    static std::string scope(Action action) {
        if (action >= original_autosave && action <= original_scope) return "OYUN";
        return "PORT";
    }

    static const char* label(Action action) {
        switch (action) {
            case window_mode: return "Pencere";
            case scale_mode: return "\xa6l" "\xa7" "ekleme";
            case vsync: return "VSync";
            case cursor_scale: return "\xa3mle\xa7 \xadl\xa7" "e\xabi";
            case text_language: return "Oyun Metni";
            case voice_language: return "Konu\xa8malar";
            case master: return "Ana Ses";
            case speech: return "Konu\xa8ma";
            case ambient: return "Ambiyans";
            case effects: return "Efektler";
            case subtitles: return "Altyaz\xaa";
            case hotspots: return "Hotspot";
            case autosave: return "Port oto kayd\xaa";
            case skip_seen: return "G\xadr\xaclen sat\xaar";
            case gamepad: return "Gamepad";
            case key_settings: return "ayarlar";
            case key_backlog: return "ge\xa7mi\xa8";
            case key_skip: return "atlama";
            case key_quicksave: return "h\xaazl\xaa kay\xaat";
            case key_quickload: return "H. y\xackle";
            case key_autoload: return "Oto y\xackle";
            case key_screenshot: return "ekran";
            case key_hotspots: return "hotspot";
            case original_autosave: return "oto kay\xaat";
            case original_video: return "video boyutu";
            case original_audio: return "ses/m\xaczik";
            case original_notes: return "konu\xa8ma notu";
            case original_tutorial: return "y\xadnlendirici";
            case original_location: return "yer tan\xaat\xaam\xaa";
            case original_scope: return "tercihler";
            default: return "";
        }
    }

    static std::string description(Action action) {
        switch (action) {
            case window_mode: return "Oyunu pencere veya tam ekran modunda çalıştırır. Pencere modu diğer uygulamalara geçmenizi kolaylaştırır.";
            case scale_mode: return "Görüntünün en ve boy oranını korur. Tamsayı seçeneği pikselleri keskin tutar; kenarlarda siyah boşluk bırakabilir.";
            case vsync: return "Ekrandaki yatay yırtılmayı azaltmak için görüntüyü ekran yenilemesiyle eşler.";
            case cursor_scale: return "Oyun imlecinin boyutunu ayarlar. Büyük ekranlarda daha rahat görünmesini sağlar.";
            case master: return "Oyun içindeki tüm seslerin genel seviyesini ayarlar. Diğer ses seviyelerini de bu menüden değiştirebilirsiniz.";
            case speech: return "Karakterlerin konuşma seslerinin seviyesini ayarlar.";
            case ambient: return "Bulunduğunuz ortamın arka plan seslerinin seviyesini ayarlar.";
            case effects: return "Etkileşim ve diğer ses efektlerinin seviyesini ayarlar.";
            case subtitles: return "Konuşmalar sırasında ekranın altındaki yazıları gösterir veya gizler.";
            case text_language: return "Oyun metni için kurulu dil paketlerinden birini seçer. Ek dil paketleri kurulduğunda burada görünür.";
            case voice_language: return "Konuşmalar için kurulu ses paketlerinden birini seçer. Ek ses paketleri kurulduğunda burada görünür.";
            case hotspots: return "Etkileşim alanlarını çerçeveleyen yardımcı gösterimi açar. Normal oyun için kapalı bırakabilirsiniz.";
            case autosave: return "Her sahne geçişinde otomatik kayıt oluşturur. Bu kaydı otomatik yükleme kısayoluyla açabilirsiniz.";
            case skip_seen: return "Daha önce dinlediğiniz konuşmaları atlar. İlk kez karşılaştığınız konuşmalar normal oynatılır.";
            case gamepad: return "Bağlı oyun kumandasını etkinleştirir. Sol çubuk imleci hareket ettirir; A seçer, B geri döner.";
            case key_settings: return "Gelişmiş ayarlar menüsünü açan tuş. Yeni bir atama için seçip istediğiniz tuşa basın.";
            case key_backlog: return "Bu oturumdaki konuşma geçmişini açan tuş.";
            case key_skip: return "Görülen konuşmaları atlamayı açan veya kapatan tuş.";
            case key_quicksave: return "Oyunu hızlı kayıt dosyasına kaydeden tuş.";
            case key_quickload: return "Son hızlı kaydı yükleyen tuş.";
            case key_autoload: return "Son otomatik kaydı yükleyen tuş.";
            case key_screenshot: return "Ekran görüntüsü kaydeden tuş.";
            case key_hotspots: return "Etkileşim alanlarının yardımcı çerçevelerini açan veya kapatan tuş.";
            default: return "Oyunun özgün tercihi. Bu değer ana tercihler sayfasından değiştirilebilir. Geri düğmesiyle o sayfaya dönebilirsiniz.";
        }
    }

    std::string value(const Session& session, Action action) const {
        const auto& settings = session.options.settings;
        const auto prefs = session.view.state.gameprefs;
        switch (action) {
            case window_mode:
                switch (settings.window_mode) {
                    case byzantine::WindowMode::windowed: return "Pencere";
                    case byzantine::WindowMode::borderless: return "Tam ekran";
                    case byzantine::WindowMode::exclusive: return "\xa6zel tam";
                    case byzantine::WindowMode::maximized: return "B\xacy\xack";
                }
                return {};
            case scale_mode: return settings.scale_mode == byzantine::ScaleMode::fit ? "Oranl\xaa" : "Tamsay\xaa";
            case vsync: return settings.vsync ? "a\xa7\xaak" : "kapal\xaa";
            case cursor_scale: return std::to_string(settings.cursor_scale) + "x";
            case master: return std::to_string(static_cast<int>(settings.master_volume * 100)) + "%";
            case speech: return std::to_string(static_cast<int>(settings.speech_volume * 100)) + "%";
            case ambient: return std::to_string(static_cast<int>(settings.ambient_volume * 100)) + "%";
            case effects: return std::to_string(static_cast<int>(settings.effects_volume * 100)) + "%";
            case text_language: return byzantine::utf8_to_turkish_game_text(language_label(session, true));
            case voice_language: return byzantine::utf8_to_turkish_game_text(language_label(session, false));
            case subtitles: return settings.subtitles ? "a\xa7\xaak" : "kapal\xaa";
            case hotspots:
                if (!settings.hotspots) return "kapal\xaa";
                return settings.hotspots_all ? "t\xacm\xac" : "imle\xa7te";
            case autosave: return settings.autosave ? "a\xa7\xaak" : "kapal\xaa";
            case skip_seen: return settings.skip_seen ? "a\xa7\xaak" : "kapal\xaa";
            case gamepad: return settings.gamepad ? "a\xa7\xaak" : "kapal\xaa";
            case key_settings: return settings.key_settings;
            case key_backlog: return settings.key_backlog;
            case key_skip: return settings.key_skip;
            case key_quicksave: return settings.key_quicksave;
            case key_quickload: return settings.key_quickload;
            case key_autoload: return settings.key_autoload;
            case key_screenshot: return settings.key_screenshot;
            case key_hotspots: return settings.key_hotspots;
            case original_autosave:
                if (!prefs) return "bilinmiyor";
                return (*prefs)[0] != 0 ? "a\xa7\xaak" : "kapal\xaa";
            case original_video:
                if (!prefs) return "bilinmiyor";
                return (*prefs)[1] != 0 ? "tam (2x)" : "yar\xaam (1x)";
            case original_audio:
                if (!prefs) return "bilinmiyor";
                return (*prefs)[2] != 0 ? "a\xa7\xaak" : "kapal\xaa";
            case original_notes:
                if (!prefs) return "bilinmiyor";
                return (*prefs)[3] != 0 ? "a\xa7\xaak" : "kapal\xaa";
            case original_tutorial:
                if (!prefs) return "bilinmiyor";
                return (*prefs)[4] != 0 ? "a\xa7\xaak" : "kapal\xaa";
            case original_location:
                if (!prefs) return "bilinmiyor";
                return (*prefs)[5] != 0 ? "a\xa7\xaak" : "kapal\xaa";
            case original_scope: return "TERC\xa3HLER";
            default: return {};
        }
    }

    static bool editable(Action action) {
        return action < original_autosave || action > original_scope;
    }

    static bool is_key_action(Action action) {
        return action >= key_settings && action <= key_hotspots;
    }

    void apply_audio(Session& session) {
        const auto& settings = session.options.settings;
        session.audio.set_volumes(settings.master_volume, settings.speech_volume,
                                  settings.ambient_volume, settings.effects_volume);
    }

    void apply_hotspots(Session& session) {
        session.options.overlay = session.options.settings.hotspots;
        session.options.hotspots_all = session.options.settings.hotspots_all;
        session.display.set_hover_overlay(session.view, -1, -1);
    }

    void reload_text(Session& session) {
        backdrop.reset();
        notebook_face.reset();
        notebook_prose.reset();
        resolve_language_selection(session.options);
        load_messages(session, true);
        load_scene_text(session, true);
        load_fonts(session, true);
        session.display.reload_ui_assets(session.options);
        refresh_background(session.options, session.view);
        session.display.compose(session.view, false);
    }

    void change(Session& session, Action action, int direction) {
        auto& settings = session.options.settings;
        if (!editable(action)) {
            std::cout << "settings_readonly action=" << label(action) << '\n';
            present(session);
            return;
        }
        switch (action) {
            case window_mode: {
                static const byzantine::WindowMode modes[] = {
                    byzantine::WindowMode::windowed,
                    byzantine::WindowMode::borderless,
                    byzantine::WindowMode::exclusive,
                    byzantine::WindowMode::maximized};
                int index = 0;
                for (int i = 0; i < 4; ++i) {
                    if (modes[i] == settings.window_mode) index = i;
                }
                index = (index + (direction > 0 ? 1 : 3)) % 4;
                settings.window_mode = modes[index];
                session.display.apply_window_mode(settings.window_mode);
                settings.window_mode = session.display.window_mode;
                break;
            }
            case scale_mode:
                settings.scale_mode =
                    settings.scale_mode == byzantine::ScaleMode::fit
                        ? byzantine::ScaleMode::integer
                        : byzantine::ScaleMode::fit;
                session.display.apply_scale_mode(settings.scale_mode);
                break;
            case vsync:
                settings.vsync = !settings.vsync;
                session.display.apply_vsync(settings.vsync);
                break;
            case cursor_scale:
                settings.cursor_scale = settings.cursor_scale >= 4
                                            ? 1
                                            : settings.cursor_scale + 1;
                session.display.clear_cursor_cache();
                {
                    float x = 0;
                    float y = 0;
                    session.display.mouse_logical(&x, &y);
                    session.display.update_cursor_at(session.view, static_cast<int>(x),
                                                     static_cast<int>(y));
                }
                break;
            case master:
                settings.master_volume = std::clamp(
                    settings.master_volume + direction * 0.1f, 0.0f, 1.0f);
                apply_audio(session);
                break;
            case speech:
                settings.speech_volume = std::clamp(
                    settings.speech_volume + direction * 0.1f, 0.0f, 1.0f);
                apply_audio(session);
                break;
            case ambient:
                settings.ambient_volume = std::clamp(
                    settings.ambient_volume + direction * 0.1f, 0.0f, 1.0f);
                apply_audio(session);
                break;
            case effects:
                settings.effects_volume = std::clamp(
                    settings.effects_volume + direction * 0.1f, 0.0f, 1.0f);
                apply_audio(session);
                break;
            case subtitles:
                settings.subtitles = !settings.subtitles;
                break;
            case autosave:
                settings.autosave = !settings.autosave;
                break;
            case skip_seen:
                settings.skip_seen = !settings.skip_seen;
                std::cout << "dialogue_skip_seen enabled="
                          << (settings.skip_seen ? 1 : 0) << '\n';
                break;
            case gamepad:
                settings.gamepad = !settings.gamepad;
                session.display.set_gamepad_enabled(settings.gamepad);
                break;
            case hotspots:
                if (!settings.hotspots) {
                    settings.hotspots = true;
                    settings.hotspots_all = false;
                } else if (!settings.hotspots_all) {
                    settings.hotspots_all = true;
                } else {
                    settings.hotspots = false;
                    settings.hotspots_all = true;
                }
                apply_hotspots(session);
                break;
            case text_language:
            case voice_language: {
                const bool text = action == text_language;
                auto codes = languages(session, text);
                const auto current = text ? settings.text_language : settings.voice_language;
                auto at = std::find(codes.begin(), codes.end(), current);
                int index = at == codes.end() ? 0 : static_cast<int>(at - codes.begin());
                index = (index + (direction > 0 ? 1 : static_cast<int>(codes.size()) - 1)) %
                        static_cast<int>(codes.size());
                const auto code = codes[static_cast<std::size_t>(index)];
                if (text) settings.text_language = code;
                else settings.voice_language = code;
                reload_text(session);
                break;
            }
            case key_settings:
            case key_backlog:
            case key_skip:
            case key_quicksave:
            case key_quickload:
            case key_autoload:
            case key_screenshot:
            case key_hotspots:
                capturing = true;
                capture_action = action;
                break;
            default:
                break;
        }
        settings.clamp();
        save_port_settings(session.options, false);
        present(session);
    }

    static std::vector<std::string> languages(const Session& session, bool text) {
        std::vector<std::string> codes{"auto"};
        for (const auto& pack : session.options.language.packs) {
            if (text ? pack.text : pack.voice) codes.push_back(pack.code);
        }
        return codes;
    }

    static std::string language_label(const Session& session, bool text) {
        const auto code = text ? session.options.settings.text_language
                               : session.options.settings.voice_language;
        if (code.empty() || code == "auto") return "Varsayılan";
        for (const auto& pack : session.options.language.packs) {
            if (lower_ascii(pack.code) != lower_ascii(code)) continue;
            return pack.name.empty() ? pack.code : pack.name + " (" + pack.code + ")";
        }
        return code;
    }

    void load_backdrop(Session& session) {
        if (backdrop) return;
        const auto file = find_language_child(session.options, false, "setupart", "preffs");
        if (!file) return;
        try {
            const auto image = byzantine::decode_jpeg_file(file->string());
            if (image.width != kScreenWidth || image.height != kScreenHeight) return;
            backdrop = byzantine::make_surface(kScreenWidth, kScreenHeight, 0);
            backdrop->pixels = byzantine::rgb_to_rgb565(image);
            const auto original_pixels = backdrop->pixels;
            // Extend a clean strip of PREFFS' own gray paper over the baked
            // preference labels. Preserve the curved upper edge, rolled sheet,
            // pen, binding, tabs and left leather exactly from the source art.
            for (int y = 92; y < 413; ++y) {
                const int left = std::max(374, 474 - (y - 70));
                for (int x = left; x < 608; ++x) {
                    const int px = (x - 374) % 80;
                    const int py = (y - 92) % 88;
                    const int sx = 379 + (px < 40 ? px : 79 - px);
                    const int sy = 185 + (py < 44 ? py : 87 - py);
                    backdrop->at(x, y) = original_pixels[sy * kScreenWidth + sx];
                }
            }
            std::cout << "settings_backdrop file=" << file->filename().string()
                      << " paper=preffs_clean_strip\n";
        } catch (const std::exception& error) {
            std::cout << "settings_backdrop failed: " << error.what() << '\n';
        }
    }

    static bool contains(SDL_Rect rect, int x, int y) {
        return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
    }

    static SDL_Rect back_rect() { return {382, 412, 224, 29}; }

    static const char* group_description(int g) {
        switch (g) {
            case display: return "Pencere, ölçekleme, dikey senkronizasyon ve imleç boyutunu ayarlar.";
            case audio: return "Ana ses ile konuşma, ambiyans ve efekt seviyelerini ayrı ayrı ayarlar.";
            case language: return "Kurulu dil paketlerinden oyun metni ve konuşma dilini seçer. Altyazıları açar veya kapatır.";
            case session: return "Otomatik kayıt, görülen konuşmaları atlama ve oyun kumandası seçeneklerini ayarlar.";
            case keys: return "Ayarlar, geçmiş, hızlı kayıt ve diğer kısayollar için bir satır seçip yeni tuşa basın.";
            case original: return "Oyunun özgün tercihlerinin geçerli değerlerini gösterir. Değiştirmek için ana tercihler sayfasına dönün.";
            default: return "Görüntü, ses, dil ve kontrol seçeneklerini kendi tercihlerinize göre düzenleyin.";
        }
    }

    static std::optional<bool> toggle_value(const Session& session, Action action) {
        const auto& settings = session.options.settings;
        switch (action) {
            case vsync: return settings.vsync;
            case subtitles: return settings.subtitles;
            case autosave: return settings.autosave;
            case skip_seen: return settings.skip_seen;
            case gamepad: return settings.gamepad;
            default: break;
        }
        if (action >= original_autosave && action <= original_location &&
            action != original_video && session.view.state.gameprefs) {
            const int index = static_cast<int>(action) - static_cast<int>(original_autosave);
            return (*session.view.state.gameprefs)[static_cast<std::size_t>(index)] != 0;
        }
        return std::nullopt;
    }

    static constexpr int kSliderWidth = 70;

    static float volume(const Session& session, Action action) {
        const auto& settings = session.options.settings;
        switch (action) {
            case master: return settings.master_volume;
            case speech: return settings.speech_volume;
            case ambient: return settings.ambient_volume;
            case effects: return settings.effects_volume;
            default: return 0;
        }
    }

    void draw(Session& session) {
        auto& display = session.display;
        load_backdrop(session);
        if (backdrop) display.screen.pixels = backdrop->pixels;
        else display.fill_rect(0, 0, kScreenWidth, kScreenHeight, 0x18C3);
        if (!notebook_face) notebook_face = byzantine::menu_typeface();
        if (!notebook_prose) notebook_prose = byzantine::menu_typeface(true);
        const auto* face = &*notebook_face;
        const auto* prose = &*notebook_prose;
        const auto text = [&](const std::string& value, int x, int y,
                              std::uint16_t color, int align = 3) {
            if (color != 0) byzantine::draw_text(display.screen, *face, value,
                x + 1, y + 1, 1, align, 0x0000);
            byzantine::draw_text(display.screen, *face, value, x, y, 1, align, color);
        };
        byzantine::draw_text(display.screen, *prose,
             group < 0 ? advanced_settings_label() : group_name(group),
             500, 103, 1, 1, 0x0000);
        notebook_rule(display, 388, 137, 211);
        tab_rects.clear();
        row_rects.clear();
        const auto actions = actions_for(group);
        const int count = group < 0 ? group_count : static_cast<int>(actions.size());
        // Two-line choices need the full font height, shadow and padding.
        // The same rectangle owns the frame, hover and click target. Keep six
        // such rows above GERİ; single-line category/key rows stay compact.
        const bool two_line_rows = std::any_of(actions.begin(), actions.end(),
            [&](Action action) { return toggle_value(session, action).has_value(); });
        const int step = two_line_rows ? 42 : (count > 6 ? 28 : 39);
        const int first_y = two_line_rows ? 158 : 163;
        const int choice_height = 17 + face->line_height + 1; // includes shadow
        for (int i = 0; i < count; ++i) {
            const int y = first_y + i * step;
            SDL_Rect rect{380, y - (two_line_rows ? 4 : 5), 226,
                          two_line_rows ? choice_height + 6 : step - 4};
            if (group < 0) tab_rects.push_back(rect);
            else row_rects.push_back(rect);
            const bool selected = row == i;
            if (selected) notebook_frame(display, rect, true);
            const auto color = selected ? 0xF6AC : 0x0000;
            if (group < 0) {
                text(std::string("> ") + group_name(i), 387, y + 2, color);
                notebook_arrow(display, 593, y + 9, 1);
                continue;
            }
            const auto action = actions[static_cast<std::size_t>(i)];
            const auto toggle = toggle_value(session, action);
            if (toggle) {
                text(std::string("> ") + menu_uppercase(label(action)), 493, y, color, 1);
                text("a\xa7", 477, y + 17, *toggle ? 0xF6AC : 0x0000);
                text("/", 498, y + 17, 0x0000);
                text("kapat", 509, y + 17, *toggle ? 0x0000 : 0xF6AC);
                continue;
            }
            std::string row_label = label(action);
            while (!row_label.empty() && byzantine::text_width(*face, row_label) > 88)
                row_label.pop_back();
            text(row_label, 387, y + 2, color);
            if (action >= master && action <= effects) {
                const int sy = y + 10;
                display.fill_rect(492, sy, kSliderWidth, 2, 0x39C6);
                display.fill_rect(492, sy, static_cast<int>(kSliderWidth * volume(session, action)), 2, 0xEDAA);
                const int knob = 492 + static_cast<int>(kSliderWidth * volume(session, action));
                display.fill_rect(knob - 2, sy - 3, 5, 7, 0xD526);
                display.fill_rect(knob - 1, sy - 2, 3, 4, 0xFF39);
                text(std::to_string(static_cast<int>(std::lround(volume(session, action) * 100))),
                     589, y + 2, 0xEDAA, 2);
            } else {
                // Constrain long language-pack names and key labels to their
                // column instead of allowing them to overwrite the row label.
                auto caption = value(session, action);
                while (!caption.empty() && byzantine::text_width(*face, caption) > 87)
                    caption.pop_back();
                text(capturing && capture_action == action ? "..." : caption,
                     549, y + 2, selected && editable(action) ? 0xF6AC : 0x0000, 1);
            }
            if (editable(action)) {
                notebook_arrow(display, 480, y + 10, -1);
                notebook_arrow(display, 595, y + 10, 1);
            }
        }
        notebook_frame(display, back_rect());
        text("< GER\xa3", 396, 420, 0xEDAA);
        if (row >= 0 && row < count) {
            byzantine::draw_text(display.screen, *prose,
                byzantine::utf8_to_turkish_game_text("AÇIKLAMA"), 54, 62, 1, 3, 0xFFE0);
            const auto title = group < 0 ? std::string(group_name(row)) :
                std::string(label(actions[static_cast<std::size_t>(row)]));
            text(title, 54, 92, 0xFF39);
            const auto desc = group < 0 ? group_description(row) :
                description(actions[static_cast<std::size_t>(row)]);
            const auto lines = wrap_dialog_text(*prose,
                byzantine::utf8_to_turkish_game_text(desc), 183);
            int y = 122;
            for (const auto& line : lines) {
                if (y + prose->line_height > 328) break;
                byzantine::draw_text(display.screen, *prose, line, 54, y, 1, 3, 0xFF39);
                y += prose->line_height + 2;
            }
        }
        if (capturing) text("Yeni tu\xa8: ESC iptal", 390, 393, 0xFFE0);
    }

    void back(Session& session) {
        if (group >= 0) {
            group = -1;
            row = -1;
            present(session);
        } else close(session);
    }

    void present(Session& session) {
        draw(session);
        ScreenContentScope content(session.display, ScreenContent::menu);
        session.display.present_image(session.display.screen, session.view, 0, "settings");
    }

    void open_overlay(Session& session, bool native_entry = false,
                      int initial_group = -1) {
        open = true;
        from_prefs = native_entry;
        backdrop.reset();
        notebook_face.reset();
        notebook_prose.reset();
        group = std::clamp(initial_group, -1, group_count - 1);
        row = -1;
        capturing = false;
        capture_action = -1;
        std::cout << "settings_open file=" << session.options.config_path << '\n';
        present(session);
    }

    void close(Session& session) {
        open = false;
        capturing = false;
        session.display.store_window_settings(session.options.settings);
        save_port_settings(session.options, true);
        std::cout << "settings_close from_prefs=" << (from_prefs ? 1 : 0) << '\n';
        session.reopen_prefs = from_prefs;
        from_prefs = false;
        session.display.present(session.view);
    }

    bool key(Session& session, const SDL_KeyboardEvent& event) {
        if (!open) return false;
        const auto key = event.keysym.sym;
        if (capturing) {
            if (key != SDLK_ESCAPE) {
                const std::string name = SDL_GetKeyName(key);
                auto& settings = session.options.settings;
                switch (capture_action) {
                    case key_settings: settings.key_settings = name; break;
                    case key_backlog: settings.key_backlog = name; break;
                    case key_skip: settings.key_skip = name; break;
                    case key_quicksave: settings.key_quicksave = name; break;
                    case key_quickload: settings.key_quickload = name; break;
                    case key_autoload: settings.key_autoload = name; break;
                    case key_screenshot: settings.key_screenshot = name; break;
                    case key_hotspots: settings.key_hotspots = name; break;
                    default: break;
                }
                std::cout << "settings_key action=" << capture_action << " name=" << name
                          << '\n';
                save_port_settings(session.options, false);
            }
            capturing = false;
            capture_action = -1;
            present(session);
            return true;
        }
        if (key == SDLK_ESCAPE ||
            key_is(session.options.settings, session.options.settings.key_settings, key)) {
            if (key == SDLK_ESCAPE) back(session);
            else close(session);
            return true;
        }
        if (key == SDLK_TAB) {
            const int direction = (event.keysym.mod & KMOD_SHIFT) ? -1 : 1;
            group = (group + group_count + direction) % group_count;
            row = 0;
            present(session);
            return true;
        }
        const auto actions = actions_for(group);
        const int count = group < 0 ? group_count : static_cast<int>(actions.size());
        if (key == SDLK_UP) {
            row = row < 0 ? count - 1 : (row + count - 1) % count;
            present(session);
            return true;
        }
        if (key == SDLK_DOWN) {
            row = (row + 1) % count;
            present(session);
            return true;
        }
        if (group < 0) {
            if ((key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_RIGHT) && row >= 0) {
                group = row;
                row = -1;
                present(session);
            }
            return true;
        }
        if (row < 0) row = 0;
        if (key == SDLK_LEFT) {
            change(session, actions[static_cast<std::size_t>(row)], -1);
            return true;
        }
        if (key == SDLK_RIGHT || key == SDLK_RETURN || key == SDLK_KP_ENTER) {
            change(session, actions[static_cast<std::size_t>(row)], 1);
            return true;
        }
        return true;
    }

    bool quit_requested = false;
    int slider_drag = -1;

    void set_slider(Session& session, Action action, int x) {
        const float target = std::clamp((x - 492) / static_cast<float>(kSliderWidth), 0.0f, 1.0f);
        auto& settings = session.options.settings;
        switch (action) {
            case master: settings.master_volume = target; break;
            case speech: settings.speech_volume = target; break;
            case ambient: settings.ambient_volume = target; break;
            case effects: settings.effects_volume = target; break;
            default: return;
        }
        apply_audio(session);
        save_port_settings(session.options, false);
        present(session);
    }

    bool mouse_button(Session& session, int x, int y) {
        if (!open || capturing) return false;
        slider_drag = -1;
        if (contains(back_rect(), x, y)) { back(session); return true; }
        // The baked left-page actions stay live on every notebook page.
        if (contains({72, 394, 183, 24}, x, y)) {
            close(session);
            session.reopen_prefs = false;
            return true;
        }
        if (contains({72, 367, 183, 27}, x, y)) {
            close(session);
            session.reopen_prefs = true;
            quit_requested = true;
            return true;
        }
        for (std::size_t i = 0; i < tab_rects.size(); ++i) {
            if (!contains(tab_rects[i], x, y)) continue;
            group = static_cast<int>(i);
            row = -1;
            present(session);
            return true;
        }
        const auto actions = actions_for(group);
        for (std::size_t i = 0; i < row_rects.size(); ++i) {
            if (!contains(row_rects[i], x, y)) continue;
            row = static_cast<int>(i);
            const auto action = actions[i];
            const auto toggle = toggle_value(session, action);
            if (toggle && y >= row_rects[i].y + 20 && x >= 471 && x < 548) {
                const bool wanted = x < 500;
                if (editable(action) && wanted != *toggle) change(session, action, wanted ? 1 : -1);
                else present(session);
                return true;
            }
            if (action >= master && action <= effects && x >= 492 && x <= 574) {
                slider_drag = action;
                set_slider(session, action, x);
            } else if (x >= 477) change(session, action, x < 492 ? -1 : 1);
            else present(session);
            return true;
        }
        return true;
    }

    bool mouse_motion(Session& session, int x, int y, bool left_down = false) {
        if (!open || capturing) return false;
        if (slider_drag >= 0 && left_down) {
            set_slider(session, static_cast<Action>(slider_drag), x);
            return true;
        }
        if (!left_down) slider_drag = -1;
        const auto& rects = group < 0 ? tab_rects : row_rects;
        int next = -1;
        for (std::size_t i = 0; i < rects.size(); ++i) {
            if (contains(rects[i], x, y)) { next = static_cast<int>(i); break; }
        }
        if (row != next) { row = next; present(session); }
        return true;
    }

    bool wheel(Session& session, int direction) {
        if (!open || capturing || direction == 0) return false;
        const int count = group < 0 ? group_count : static_cast<int>(actions_for(group).size());
        row = (row + (direction > 0 ? 1 : count - 1) + count) % count;
        present(session);
        return true;
    }

};

// Deterministic notebook navigation uses the same handlers as SDL input.
void apply_headless_menu_input(Session& session, MenuUi& menu, const Options& options,
                               SettingsUi& settings) {
    const auto settle = [&]() {
        if (session.open_port_settings) {
            session.open_port_settings = false;
            settings.open_overlay(session, true, session.port_settings_group);
        }
        if (session.reopen_prefs) {
            session.reopen_prefs = false;
            menu.open_screen(0, session);
            if (settings.quit_requested) {
                settings.quit_requested = false;
                menu.click(session, 120, 380);
            }
        }
    };
    for (const auto& point : options.menu_clicks) {
        if (settings.open) settings.mouse_button(session, point.first, point.second);
        else if (menu.open) menu.click(session, point.first, point.second);
        else break;
        settle();
    }
    for (const auto& text : options.menu_texts) {
        if (!menu.open || !menu.editing) break;
        menu.text_input(session, text);
    }
    for (const auto& name : options.menu_keys) {
        if (!menu.open && !settings.open) break;
        SDL_KeyboardEvent event{};
        event.type = SDL_KEYDOWN;
        event.keysym.sym = SDL_GetKeyFromName(name.c_str());
        if (name == "enter" || name == "return") event.keysym.sym = SDLK_RETURN;
        if (name == "escape" || name == "esc") event.keysym.sym = SDLK_ESCAPE;
        if (name == "backspace") event.keysym.sym = SDLK_BACKSPACE;
        if (settings.open) settings.key(session, event);
        else menu.key_event(session, event);
        settle();
    }
    for (const auto& point : options.menu_hovers) {
        if (settings.open) settings.mouse_motion(session, point.first, point.second);
        else if (menu.open && !menu.editing) menu.hover_at(session, point.first, point.second);
    }
}


std::uint32_t save_disc_number(const Session& session) {
    if (session.save_base) return session.save_base->cd_number;
    const auto overlay = session.options.scene.find("--anims-overlay");
    if (overlay != session.options.scene.end()) {
        const auto folder = std::filesystem::path(overlay->second).filename().string();
        const auto at = folder.find("cd");
        if (at != std::string::npos) {
            int disc = 0;
            std::size_t index = at + 2;
            while (index < folder.size() &&
                   std::isdigit(static_cast<unsigned char>(folder[index]))) {
                disc = disc * 10 + (folder[index] - '0');
                ++index;
            }
            if (disc != 0) return static_cast<std::uint32_t>(disc);
        }
    }
    return 1;
}

bool write_autosave(Session& session) {
    if (!session.options.settings.autosave) return false;
    const auto directory = savegame_directory(session.options);
    if (directory.empty()) return false;
    std::uint16_t scene_index = 0;
    bool found = false;
    if (session.metadata) {
        for (std::size_t index = 0; index < session.metadata->scenes.size(); ++index) {
            if (lower_ascii(session.metadata->scenes[index].scene) ==
                lower_ascii(session.view.name)) {
                scene_index = static_cast<std::uint16_t>(index);
                found = true;
                break;
            }
        }
    }
    if (!found) return false;
    try {
        auto save = build_save_from_state(
            session, session.save_base ? &*session.save_base : nullptr,
            "Otomatik kayit (port)", scene_index, save_disc_number(session));
        const auto bytes = byzantine::write_save(save);
        std::error_code error;
        std::filesystem::create_directories(directory, error);
        const auto path = directory / "autosave-port.btb";
        const auto temporary = directory / "autosave-port.tmp";
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output) return false;
        output.write(reinterpret_cast<const char*>(bytes.data()),
                     static_cast<std::streamsize>(bytes.size()));
        if (!output) return false;
        output.close();
        std::filesystem::remove(path, error);
        std::filesystem::rename(temporary, path, error);
        if (error) {
            std::cout << "autosave rename_failed file=" << path
                      << " error=" << error.message() << '\n';
            return false;
        }
        session.save_base = std::move(save);
        std::cout << "autosave file=" << path << " bytes=" << bytes.size()
                  << " scene=" << session.view.name << '\n';
        return true;
    } catch (const std::exception& error) {
        std::cout << "autosave failed: " << error.what() << '\n';
        return false;
    }
}

bool quick_save(Session& session) {
    const auto directory = savegame_directory(session.options);
    if (directory.empty()) {
        std::cout << "quicksave skipped: no savegame directory\n";
        return false;
    }
    std::uint16_t scene_index = 0;
    bool found = false;
    if (session.metadata) {
        for (std::size_t index = 0; index < session.metadata->scenes.size(); ++index) {
            if (lower_ascii(session.metadata->scenes[index].scene) ==
                lower_ascii(session.view.name)) {
                scene_index = static_cast<std::uint16_t>(index);
                found = true;
                break;
            }
        }
    }
    if (!found) {
        std::cout << "quicksave skipped: scene " << session.view.name
                  << " is not in SHOW.LCC\n";
        return false;
    }
    try {
        auto save = build_save_from_state(
            session, session.save_base ? &*session.save_base : nullptr, "Hizli kayit",
            scene_index, save_disc_number(session));
        const auto bytes = byzantine::write_save(save);
        std::error_code error;
        std::filesystem::create_directories(directory, error);
        const auto path = directory / "quicksave.btb";
        const auto temporary = directory / "quicksave.tmp";
        {
            std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
            if (!output) {
                std::cout << "quicksave open_failed file=" << temporary << '\n';
                return false;
            }
            output.write(reinterpret_cast<const char*>(bytes.data()),
                         static_cast<std::streamsize>(bytes.size()));
            if (!output) {
                std::cout << "quicksave write_failed file=" << temporary << '\n';
                return false;
            }
        }
        std::filesystem::remove(path, error);
        std::filesystem::rename(temporary, path, error);
        if (error) {
            std::cout << "quicksave rename_failed file=" << path
                      << " error=" << error.message() << '\n';
            return false;
        }
        session.save_base = std::move(save);
        std::cout << "quicksave file=" << path << " bytes=" << bytes.size()
                  << " scene=" << session.view.name
                  << " scene_index=" << scene_index
                  << " description=Hizli kayit\n";
        return true;
    } catch (const std::exception& error) {
        std::cout << "quicksave failed: " << error.what() << '\n';
        return false;
    }
}

bool load_save_named(Session& session, const std::string& filename, const char* tag) {
    const auto directory = savegame_directory(session.options);
    if (directory.empty()) return false;
    const auto path = find_filename(directory, filename);
    if (!path) {
        std::cout << tag << " skipped: no " << filename << " under " << directory << '\n';
        return false;
    }
    byzantine::SaveGame save;
    try {
        save = byzantine::parse_save(read_file(*path));
    } catch (const std::exception& error) {
        std::cout << tag << " parse_failed file=" << *path
                  << " error=" << error.what() << '\n';
        return false;
    }
    std::cout << tag << " file=" << *path << " scene_index=" << save.scene_index
              << " description=" << save.description << '\n';
    if (!travel_to(session, save.scene_index, true, path->string(), &save)) {
        std::cout << tag << " failed: scene " << save.scene_index << " not loaded\n";
        return false;
    }
    follow_switches(session, true);
    std::cout << tag << " committed scene=" << session.view.name
              << " hotspots=" << session.view.trace.active_hotspots.size() << '\n';
    return true;
}

bool quick_load(Session& session) {
    return load_save_named(session, "quicksave.btb", "quickload");
}

bool load_autosave(Session& session) {
    return load_save_named(session, "autosave-port.btb", "autoload");
}

bool take_screenshot(Session& session) {
    std::filesystem::path directory = "port/output/screenshots";
    if (const char* environment = std::getenv("BYZ_SCREENSHOT_DIR");
        environment != nullptr && *environment != '\0') {
        directory = environment;
    }
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    const auto path = directory /
                      ("screenshot-" + std::to_string(SDL_GetTicks64()) + ".ppm");
    if (!byzantine::write_ppm(session.display.screen, path.string())) {
        std::cout << "screenshot failed file=" << path << '\n';
        return false;
    }
    std::cout << "screenshot file=" << path << " scene=" << session.view.name << '\n';
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    // The launcher captures a pipe; keep the final diagnostics visible even
    // when the process crashes or is killed before the normal stream flush.
    if (std::getenv("BYZ_LOG_ACTIVE")) std::cout << std::unitbuf;
    try {
        Session session;
        session.options = parse_options(argc, argv);
        auto& options = session.options;
        load_seen_dialogue(session);
        if (options.list_languages) {
            std::cout << "language_root=" << options.language.root << '\n';
            std::cout << "base=" << options.language.base
                      << " text=" << options.language.text_code_or_base()
                      << " voice=" << options.language.voice_code_or_base() << '\n';
            for (const auto& pack : options.language.packs) {
                std::cout << "language_pack code=" << pack.code
                          << " name=" << pack.name
                          << " text=" << (pack.text ? 1 : 0)
                          << " voice=" << (pack.voice ? 1 : 0)
                          << " encoding=" << pack.messages_encoding
                          << " english_font=" << (pack.english_font_branch ? 1 : 0)
                          << " root=" << pack.root << '\n';
            }
            return 0;
        }
        if (options.present_dump.empty()) {
            if (const char* env = std::getenv("BYZ_PRESENT_DUMP"); env && *env) {
                options.present_dump = env;
            }
        }
        if (const char* env = std::getenv("BYZ_PRESENT_DUMP_MAX"); env && *env) {
            try {
                const auto limit = std::stoul(env);
                if (limit != 0) options.present_dump_max = limit;
            } catch (const std::exception&) {
                std::cout << "present_dump_max env ignored=" << env << '\n';
            }
        }
        if (!options.present_dump.empty()) {
            std::cout << "present_dump dir=" << options.present_dump
                      << " max=" << options.present_dump_max
                      << " (each distinct non-video presented frame)\n";
        }
        const auto label_value = options.label.empty()
                                     ? 0
                                     : std::stoul(options.label, nullptr, 0);
        if (label_value > 0xffff) throw std::runtime_error("label outside 16-bit range");

        session.view = open_scene(options.scene, options.cc_path, options.cb_path,
                                  static_cast<std::uint16_t>(label_value));
        const auto initial_save = options.scene.find("--save");
        if (initial_save != options.scene.end()) {
            try {
                session.save_base = byzantine::parse_save(read_file(initial_save->second));
            } catch (const std::exception& error) {
                std::cout << "save_base " << initial_save->second << ": " << error.what()
                          << " (a new save keeps the empty unmodelled blocks)\n";
            }
        }
        session.view.pool_on_arrival = session.view.state.variable_pool;
        session.view.reload_point = SceneReloadPoint{
            static_cast<std::uint16_t>(label_value), options.entry};
        session.metadata = byzantine::read_variable_metadata(options.scene);
        load_scene_text(session, false);
        load_messages(session, true);
        session.display.init(options);
        load_fonts(session, true);
        if (options.audio) {
            try {
                session.audio.open();
                session.audio.set_volumes(options.settings.master_volume,
                                          options.settings.speech_volume,
                                          options.settings.ambient_volume,
                                          options.settings.effects_volume);
                std::cout << "audio_gain master=" << options.settings.master_volume
                          << " speech=" << options.settings.speech_volume
                          << " ambient=" << options.settings.ambient_volume
                          << " effects=" << options.settings.effects_volume << '\n';
            } catch (const std::exception& error) {
                std::cerr << "audio disabled: " << error.what() << '\n';
            }
        }
        const bool audio_ready = audio_gated(session);
        if (!audio_ready && options.audio) {
            std::cout << "audio not gated: "
                      << (session.audio.ready() ? "" : "no device, ")
                      << (options.sounds.empty() ? "no --sounds, " : "")
                      << (session.view.state.sound_table_known ? "" : "no --cs")
                      << " the label runs in one pass\n";
        }

        // Probe (on a copy): total steps and the sounds the label queues.
        byzantine::SceneState probe_state = session.view.entry_state;
        const auto probe = options.entry
            ? byzantine::trace_scene_entry(session.view.script, session.view.entries,
                                           probe_state, 0)
            : byzantine::trace_scene_label(session.view.script, session.view.entries,
                                           probe_state, session.view.label);
        if (options.print_audio) {
            for (const auto& event : sound_events(probe)) {
                std::cout << "audio step=" << event.step << " instruction=" << event.instruction
                          << " kind="
                          << (event.kind == SoundEvent::Kind::start
                                  ? "start"
                                  : (event.kind == SoundEvent::Kind::stop
                                         ? "stop"
                                         : (event.kind == SoundEvent::Kind::release
                                                ? "release"
                                                : "stop_all")))
                          << " opcode=0x" << std::hex << static_cast<unsigned>(event.opcode)
                          << std::dec << " id=" << event.id;
                if (session.view.state.sound_table_known &&
                    event.id < session.view.state.sound_records.size()) {
                    std::cout << " name=" << session.view.state.sound_records[event.id].name;
                }
                std::cout << " loop=" << (event.loop ? 1 : 0)
                          << " blocking=" << (event.blocking ? 1 : 0)
                          << " allowed="
                          << (event.allowed ? (*event.allowed ? "true" : "false") : "unknown")
                          << '\n';
            }
        }

        if (options.shot.empty()) {
            // Interactive: show the scene, then run the label (gated when audio
            // is available) and keep the window alive throughout.
            if (!options.entry) {
                refresh_background(options, session.view);
                session.display.compose(session.view);
                session.display.present(session.view);
            }
            const auto run = [&](EntryPoint point, const byzantine::SceneState& entry,
                                 const byzantine::SceneTrace& look) {
                if (!audio_ready && look.video_requests.empty()) {
                    session.view.state = entry;
                    set_trace(session.view, trace_point(session.view, point, entry, 0));
                    return session.running;
                }
                return run_label_gated(session, point, entry, look, true);
            };
            session.interactive = true;
            const EntryPoint startup_point{session.view.label, options.entry, std::nullopt};
            bool startup_ok = false;
            bool startup_ended = false;
            if (probe.conversation_request) {
                ClickPlan startup;
                startup.ok = true;
                startup.conversation = true;
                startup.point = startup_point;
                startup.entry = probe_state;
                startup.entry_before = session.view.entry_state;
                startup.probe = probe;
                startup_ok = execute_plan(session, startup, run, true, &startup_ended);
            } else {
                startup_ok = run(startup_point, session.view.entry_state, probe);
            }
            // The label's node loads/rotations belong to the renderer, so they
            // are applied exactly once, from the committed trace. A 0xfffe
            // session end already composed the explicit reload notice.
            if (!startup_ended) {
                apply_panorama_events(session.view, options, true);
                show_message_text(session, true);
            }
            log_entry_summary(session);
            resume_ambient(session, true);
            if (options.selftest_events && !options.click && options.click_ats.empty() &&
                !options.run_label) {
                push_quit();
            }
            if (!startup_ok) {
                SDL_DestroyTexture(session.display.texture);
                SDL_DestroyRenderer(session.display.renderer);
                SDL_DestroyWindow(session.display.window);
                SDL_Quit();
                return 0;
            }
            apply_bar_options(session, options, true);
            if (options.run_label) {
                // The CLI counterpart of a committed hotspot action: run the
                // label (and its conversation) on the current scene state.
                ClickPlan plan;
                plan.ok = true;
                plan.action = static_cast<std::uint16_t>(*options.run_label);
                plan.point = EntryPoint{plan.action, false, std::nullopt};
                plan.entry_before = session.view.state;
                plan.entry = session.view.state;
                plan.probe = byzantine::trace_scene_label(session.view.script,
                                                          session.view.entries,
                                                          plan.entry, plan.action);
                plan.conversation = plan.probe.conversation_request.has_value();
                const auto blocker = byzantine::trace_blocker(plan.probe);
                const auto before_trace = session.view.trace;
                if (!blocker.empty()) {
                    std::cout << "run_label " << plan.action << " stopped: " << blocker
                              << " at instruction " << plan.probe.stopped_at << '\n';
                } else {
                    bool ended = false;
                    if (!execute_plan(session, plan, run, true, &ended)) {
                        session.view.state = plan.entry_before;
                        set_trace(session.view, before_trace);
                        std::cout << "run_label " << plan.action
                                  << " not committed\n";
                    } else if (ended) {
                        std::cout << "run_label " << plan.action
                                  << " click_result scene_session_ended\n";
                        follow_switches(session, true);
                        if (options.print_hotspots) print_hotspot_screens(session.view);
                    } else {
                        std::cout << "run_label " << plan.action << " click_result "
                                  << describe_trace(session.view.trace, session.view.state)
                                  << '\n';
                        show_message_text(session, true);
                        apply_panorama_events(session.view, options, true);
                        refresh_background(options, session.view);
                        session.display.compose(session.view);
                        session.display.present(session.view);
                        follow_switches(session, true);
                        if (options.print_hotspots) print_hotspot_screens(session.view);
                    }
                }
                if (options.selftest_events) push_quit();
            }
            refresh_background(options, session.view);
            session.display.compose(session.view);
            session.display.present(session.view);
            if (options.bar_show) {
                session.display.top_bar_visible = *options.bar_show != 0;
                session.display.top_bar_hover = *options.bar_show;
                session.display.top_bar_press = 0;
                session.display.present(session.view);
                std::cout << "bar_show button=" << *options.bar_show
                          << " visible=" << (session.display.top_bar_visible ? "yes" : "no")
                          << '\n';
            }
            if (options.print_hotspots) print_hotspot_screens(session.view);
            if (options.print_objects) print_objects(session.view);
            if (options.print_variables) print_variables(session);
            if (options.print_records) print_records(session.view);
            SDL_SetWindowTitle(session.display.window,
                               ("Byzantine - " + session.view.name + " label " +
                                std::to_string(session.view.label)).c_str());

            // One click at a logical playfield coordinate, shared by the mouse,
            // --click-at and the overlay-independent hit test.
            const auto handle_click = [&](int screen_x, int screen_y) {
                const auto hit = hotspot_at(session.view, screen_x, screen_y);
                if (!hit.slot) {
                    std::cout << "click at " << hit.test_x << ',' << hit.test_y
                              << (hit.panorama_mapped ? " (panorama)" : "")
                              << " hit no hotspot\n";
                    return;
                }
                const auto plan = plan_hotspot(session.view, *hit.slot);
                std::cout << "click at " << hit.test_x << ',' << hit.test_y
                          << (hit.panorama_mapped ? " (panorama)" : "") << ' '
                          << plan.message << '\n';
                if (!plan.ok) {
                    clear_text_after_rejected_use(session, plan, true);
                    return;
                }
                const auto before_state = session.view.state;
                const auto before_trace = session.view.trace;
                bool ended = false;
                if (!execute_plan(session, plan, run, true, &ended)) {
                    // A conversation that stopped before 0x432d6e returned is
                    // not a completed action: discard its partial variable
                    // writes instead of leaving them in the scene.
                    session.view.state = before_state;
                    set_trace(session.view, before_trace);
                    session.display.compose(session.view, false);
                    session.display.present(session.view);
                    return;
                }
                if (ended) {
                    // The scene session ended; the explicit R/ESC notice is
                    // already composed. Do not touch the old click's object.
                    std::cout << "click_result scene_session_ended active_slots="
                              << session.view.trace.active_hotspots.size() << '\n';
                    follow_switches(session, true);
                    if (options.print_hotspots) print_hotspot_screens(session.view);
                    return;
                }
                ClickCommit commit;
                commit.object_slot = *hit.slot;
                commit.refresh = true;
                commit.present_frame = true;
                commit.active_slots = true;
                commit_click(session, options, commit, true);
                if (options.print_hotspots) print_hotspot_screens(session.view);
            };
            // Arrow keys turn the panorama (the original uses on-screen cursor
            // buttons; the mapping between those and a host key is ours).
            const auto turn = [&](double yaw, double pitch) {
                if (!panorama_layer(session.view)) return;
                session.view.panorama->advance(yaw, pitch, 1.0);
                session.display.compose(session.view, false, false);
                session.display.present(session.view);
                std::cout << "panorama yaw=" << session.view.panorama->yaw_degrees()
                          << " pitch=" << session.view.panorama->pitch_degrees()
                          << '\n';
                if (options.print_hotspots) print_hotspot_screens(session.view);
            };

            // Headless verification of the click path: the same handler the
            // mouse uses, run before the event loop.
            if (options.click) {
                const auto plan = plan_hotspot(session.view, *options.click);
                std::cout << "click " << plan.message
                          << " handled=" << (plan.ok ? "true" : "false") << '\n';
                const auto before_state = session.view.state;
                const auto before_trace = session.view.trace;
                bool ended = false;
                if (plan.ok && execute_plan(session, plan, run, true, &ended)) {
                    if (ended) {
                        std::cout << "click_result scene_session_ended active_slots="
                                  << session.view.trace.active_hotspots.size() << '\n';
                        follow_switches(session, true);
                    } else {
                        ClickCommit commit;
                        commit.object_slot = static_cast<std::uint16_t>(*options.click);
                        commit.refresh = true;
                        commit.present_frame = true;
                        commit.active_slots = true;
                        commit_click(session, options, commit, true);
                    }
                } else if (plan.ok) {
                    session.view.state = before_state;
                    set_trace(session.view, before_trace);
                    session.display.compose(session.view, false);
                    session.display.present(session.view);
                    std::cout << "click not committed\n";
                }
                if (!plan.ok) clear_text_after_rejected_use(session, plan, true);
                if (options.selftest_events) push_quit();
            }
            for (const auto& point : options.click_ats) {
                if (point.second >= kScreenBandTop) {
                    // A playfield coordinate can never reach the strip, so a
                    // stored y >= 0x1a4 is the 640x480 screen's bottom band. Use
                    // the same handler the real mouse uses (strip slots, examine,
                    // page arrows), not the scene hit test.
                    handle_bar_click(session, point.first, point.second, true);
                } else {
                    handle_click(point.first, point.second);
                }
                if (options.selftest_events) push_quit();
            }

            // Left-drag turns a panorama; a press that does not move is a click.
            bool dragging = false;
            bool dragged = false;
            int drag_x = 0;
            int drag_y = 0;
            // 0x408f32/0x4091e5: a top-bar click draws the DWNBAR frame for
            // 100 ms (Sleep(100)) and then FUN_0041e289's screen opens.
            MenuUi menu;
            menu.init(session);
            SettingsUi settings_ui;
            int pending_menu_mode = -1;
            std::uint64_t pending_menu_at = 0;
            if (options.menu_mode) {
                menu.open_screen(*options.menu_mode, session);
            }
            apply_headless_menu_input(session, menu, options, settings_ui);
            if (!options.menu_shot.empty() && (menu.open || settings_ui.open)) {
                if (settings_ui.open) settings_ui.draw(session);
                else menu.draw(session);
                if (byzantine::write_ppm(session.display.screen, options.menu_shot)) {
                    std::cout << "menu_shot file=" << options.menu_shot
                              << " mode=" << menu.mode << " (interactive)\n";
                }
            }
            // 0x43b216 draws once per frame and 0x43bb77 steps the frame list
            // after every visible record. The original's loop is uncapped, so
            // its pass rate (not the host's compose cost) decides how fast an
            // animation plays. The host renders on a 16 ms tick but keeps the
            // record-pass clock separate (`--anim-pass-hz`): the compose draws
            // the current frame without stepping, then exactly the passes the
            // clock owes are applied to the records that compose drew.
            byzantine::FramePacer pacer(SDL_GetTicks64());
            byzantine::PassClock pass_clock(SDL_GetTicks64(), options.anim_pass_hz);
            const auto stall_passes = static_cast<std::uint64_t>(options.anim_pass_hz);
            while (session.running) {
                SDL_Event event;
                while (poll_logged_event(&event, "scene", session)) {
                    if (settings_ui.open) {
                        if (event.type == SDL_QUIT) {
                            session.running = false;
                        } else if (event.type == SDL_KEYDOWN) {
                            settings_ui.key(session, event.key);
                        } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                                   event.button.button == SDL_BUTTON_LEFT) {
                            settings_ui.mouse_button(session, event.button.x, event.button.y);
                        } else if (event.type == SDL_MOUSEMOTION) {
                            settings_ui.mouse_motion(session, event.motion.x, event.motion.y,
                                (event.motion.state & SDL_BUTTON_LMASK) != 0);
                        } else if (event.type == SDL_MOUSEBUTTONUP) {
                            settings_ui.slider_drag = -1;
                        } else if (event.type == SDL_MOUSEWHEEL) {
                            settings_ui.wheel(session, event.wheel.y);
                        }
                        if (!settings_ui.open && session.reopen_prefs) {
                            session.reopen_prefs = false;
                            menu.open_screen(0, session);
                            if (settings_ui.quit_requested) {
                                settings_ui.quit_requested = false;
                                menu.click(session, 120, 380);
                            }
                        }
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        key_is(session.options.settings,
                               session.options.settings.key_backlog,
                               event.key.keysym.sym) && !menu.open) {
                        backlog_overlay(session);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        key_is(session.options.settings,
                               session.options.settings.key_skip,
                               event.key.keysym.sym) && !menu.open) {
                        session.options.settings.skip_seen =
                            !session.options.settings.skip_seen;
                        std::cout << "dialogue_skip_seen enabled="
                                  << (session.options.settings.skip_seen ? 1 : 0) << '\n';
                        save_port_settings(session.options, false);
                        session.display.present(session.view);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        key_is(session.options.settings,
                               session.options.settings.key_quicksave,
                               event.key.keysym.sym) && !menu.open) {
                        quick_save(session);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        key_is(session.options.settings,
                               session.options.settings.key_quickload,
                               event.key.keysym.sym) && !menu.open) {
                        quick_load(session);
                        session.display.present(session.view);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        key_is(session.options.settings,
                               session.options.settings.key_autoload,
                               event.key.keysym.sym) && !menu.open) {
                        load_autosave(session);
                        session.display.present(session.view);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        key_is(session.options.settings,
                               session.options.settings.key_screenshot,
                               event.key.keysym.sym) && !menu.open) {
                        take_screenshot(session);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        key_is(session.options.settings,
                               session.options.settings.key_settings,
                               event.key.keysym.sym) && !menu.open) {
                        settings_ui.open_overlay(session);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11 &&
                        !menu.open) {
                        auto& settings = session.options.settings;
                        settings.window_mode =
                            settings.window_mode == byzantine::WindowMode::borderless
                                ? byzantine::WindowMode::windowed
                                : byzantine::WindowMode::borderless;
                        session.display.apply_window_mode(settings.window_mode);
                        settings.window_mode = session.display.window_mode;
                        save_port_settings(session.options, true);
                        session.display.present(session.view);
                        continue;
                    }
                    if (event.type == SDL_KEYDOWN &&
                        event.key.keysym.sym == SDLK_RETURN &&
                        (event.key.keysym.mod & KMOD_ALT) && !menu.open) {
                        auto& settings = session.options.settings;
                        settings.window_mode =
                            settings.window_mode == byzantine::WindowMode::exclusive
                                ? byzantine::WindowMode::windowed
                                : byzantine::WindowMode::exclusive;
                        session.display.apply_window_mode(settings.window_mode);
                        settings.window_mode = session.display.window_mode;
                        save_port_settings(session.options, true);
                        session.display.present(session.view);
                        continue;
                    }
                    if (menu.open) {
                        // 0x41e289's screen loop owns input until it returns:
                        // ESC closes it, clicks hit its own 0x45f548 controls.
                        // OYUN KAYIT's name box (0x421fa2) takes text input.
                        if (event.type == SDL_QUIT) {
                            session.running = false;
                        } else if (event.type == SDL_TEXTINPUT) {
                            menu.text_input(session, event.text.text);
                        } else if (event.type == SDL_KEYDOWN) {
                            if (menu.editing) {
                                menu.key_event(session, event.key);
                            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                                menu.close(session, "escape");
                            }
                        } else if (event.type == SDL_MOUSEMOTION) {
                            if (!menu.editing) {
                                menu.hover_at(session, event.motion.x, event.motion.y);
                            }
                        } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                                   event.button.button == SDL_BUTTON_LEFT) {
                            if (!menu.editing) {
                                menu.click(session, event.button.x, event.button.y);
                            }
                        }
                        continue;
                    }
                    if (event.type == SDL_QUIT ||
                        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                        session.running = false;
                    } else if (event.type == SDL_WINDOWEVENT &&
                               (event.window.event == SDL_WINDOWEVENT_LEAVE ||
                                event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)) {
                        // The pointer left the window (or the window lost focus),
                        // so no motion event will rewind the hover highlight.
                        if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST &&
                            session.options.settings.mute_on_focus_loss) {
                            session.audio.set_muted(true);
                        }
                        session.hover_select_slot = -1;
                        if (restore_hover_preview(session, true)) {
                            session.display.compose(session.view, false, false);
                            session.display.present(session.view);
                        }
                    } else if (event.type == SDL_WINDOWEVENT &&
                               event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                        session.audio.set_muted(false);
                    } else if (event.type == SDL_KEYDOWN &&
                               key_is(session.options.settings,
                                      session.options.settings.key_hotspots,
                                      event.key.keysym.sym)) {
                        options.overlay = !options.overlay;
                        // Turning the hover outline back on follows the live
                        // pointer; the outlined slot is not tracked while off.
                        float pointer_x = 0;
                        float pointer_y = 0;
                        session.display.mouse_logical(&pointer_x, &pointer_y);
                        session.display.set_hover_overlay(
                            session.view, static_cast<int>(pointer_x),
                            static_cast<int>(pointer_y));
                        session.display.present(session.view);
                    } else if (event.type == SDL_KEYDOWN &&
                               event.key.keysym.sym == SDLK_r &&
                               session.reload_available) {
                        // The explicit choice after a conversation session end.
                        // The scene is reloaded from the supplied save/entry;
                        // no automatic rollback happened before this key.
                        if (restart_scene(session, run, true,
                                          "conversation_session_exit_user_choice")) {
                            follow_switches(session, true);
                            SDL_SetWindowTitle(
                                session.display.window,
                                ("Byzantine - " + session.view.name + " label " +
                                 std::to_string(session.view.label)).c_str());
                        }
                    } else if (event.type == SDL_KEYDOWN &&
                               (event.key.keysym.sym == SDLK_LEFT ||
                                event.key.keysym.sym == SDLK_RIGHT ||
                                event.key.keysym.sym == SDLK_UP ||
                                event.key.keysym.sym == SDLK_DOWN)) {
                        double yaw = 0;
                        double pitch = 0;
                        if (event.key.keysym.sym == SDLK_LEFT) yaw = -10;
                        if (event.key.keysym.sym == SDLK_RIGHT) yaw = 10;
                        if (event.key.keysym.sym == SDLK_UP) pitch = 5;
                        if (event.key.keysym.sym == SDLK_DOWN) pitch = -5;
                        turn(yaw, pitch);
                    } else if (event.type == SDL_MOUSEWHEEL) {
                        // Wheel is the second mouse path for the original's
                        // up/down look; the arrow keys keep working.
                        if (event.wheel.y != 0 && panorama_layer(session.view)) {
                            turn(0, event.wheel.y > 0 ? 5.0 : -5.0);
                        }
                    } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                               event.button.button == SDL_BUTTON_LEFT) {
                        // SDL_RendererEventWatch already converted button.x/y
                        // to the renderer's logical size; use them as they are.
                        // The screen is 640x480: rows 0x14..0x1a3 are the
                        // playfield, rows 0x1a4..0x1df the inventory strip.
                        const float logical_x = static_cast<float>(event.button.x);
                        const float logical_y = static_cast<float>(event.button.y);
                        if (logical_y < kScreenPlayfieldTop) {
                            const int button = byzantine::top_bar_button(
                                static_cast<int>(logical_x), static_cast<int>(logical_y));
                            if (button > 0) {
                                pending_menu_mode = byzantine::top_bar_menu_mode(button);
                                pending_menu_at = SDL_GetTicks64() + 100;
                                session.display.top_bar_visible = true;
                                session.display.top_bar_hover = button;
                                session.display.top_bar_press = button;
                                session.display.present(session.view);
                                std::cout << "ui_click top_bar x="
                                          << static_cast<int>(logical_x) << " y="
                                          << static_cast<int>(logical_y) << " button="
                                          << button << " mode=" << pending_menu_mode
                                          << " pressed (0x4091e5 DWNBAR 100 ms)\n";
                                continue;
                            }
                        }
                        session.display.top_bar_press = 0;
                        if (handle_bar_click(session, static_cast<int>(logical_x),
                                             static_cast<int>(logical_y), true)) {
                            // The strip and the top band are not scene clicks.
                        } else if (panorama_layer(session.view)) {
                            dragging = true;
                            dragged = false;
                            drag_x = static_cast<int>(logical_x);
                            drag_y = static_cast<int>(logical_y);
                        } else {
                            session.display.log_mouse_click(logical_x, logical_y, "down");
                            handle_click(static_cast<int>(logical_x),
                                         static_cast<int>(logical_y) - kScreenPlayfieldTop);
                        }
                    } else if (event.type == SDL_MOUSEMOTION) {
                        // 0x40c329/0x4083d4 select the game's own pointer for
                        // whatever is under the mouse. Coordinates are already
                        // logical (SDL_RendererEventWatch rewrites them).
                        // 0x408fe7/0x40c329: the top bar is a roll-over UI; it
                        // is drawn on entry and cleared when the pointer leaves.
                        const int bar_button = byzantine::top_bar_button(
                            event.motion.x, event.motion.y);
                        const bool bar_changed =
                            session.display.top_bar_visible != (bar_button > 0) ||
                            session.display.top_bar_hover != bar_button;
                        session.display.top_bar_visible = bar_button > 0;
                        session.display.top_bar_hover = bar_button;
                        session.display.top_bar_press = 0;
                        if (bar_changed) {
                            session.display.present(session.view);
                        }
                        session.display.update_cursor_at(
                            session.view, event.motion.x, event.motion.y);
                        // The hover outline follows the pointer; redraw only
                        // when it moves to another active hotspot (or leaves).
                        bool hover_dirty = session.display.set_hover_overlay(
                            session.view, event.motion.x, event.motion.y);
                        // Show the destination's own *Hi art while hovered.
                        if (!dragging &&
                            hover_select(session, event.motion.x, event.motion.y, true)) {
                            session.display.compose(session.view, false, false);
                            hover_dirty = true;
                        }
                        if (hover_dirty) session.display.present(session.view);
                        if (!dragging) continue;
                        const float logical_x = static_cast<float>(event.motion.x);
                        const float logical_y = static_cast<float>(event.motion.y);
                        const int delta = static_cast<int>(logical_x) - drag_x;
                        const int delta_y = static_cast<int>(logical_y) - drag_y;
                        if (delta != 0 || delta_y != 0) {
                            // One source column per screen pixel, the same
                            // degrees-per-column that 0x402285 uses. The
                            // vertical scale is a port input decision; the
                            // original's 0x4013ae used cursor edge bands.
                            const double degrees =
                                static_cast<double>(delta) / session.view.panorama->source_width() *
                                360.0;
                            const double pitch = -static_cast<double>(delta_y) * 0.15;
                            turn(degrees, pitch);
                            drag_x = static_cast<int>(logical_x);
                            drag_y = static_cast<int>(logical_y);
                            dragged = true;
                        }
                    } else if (event.type == SDL_MOUSEBUTTONUP &&
                               event.button.button == SDL_BUTTON_LEFT && dragging) {
                        dragging = false;
                        const float logical_x = static_cast<float>(event.button.x);
                        const float logical_y = static_cast<float>(event.button.y);
                        if (!dragged) {
                            session.display.log_mouse_click(logical_x, logical_y, "up");
                            handle_click(static_cast<int>(logical_x),
                                         static_cast<int>(logical_y) - kScreenPlayfieldTop);
                        } else {
                            std::cout << "click suppressed: the drag turned the view\n";
                        }
                    }
                }
                // 0x43b216 draws once per frame and 0x43bb77 steps the frame
                // list after every visible record. The host composes on its
                // 16 ms render tick; the record-pass clock decides how many
                // passes the original would have run in that time. A scene with
                // no animated record keeps the event-driven redraw.
                const auto now_ms = SDL_GetTicks64();
                if (pending_menu_mode >= 0 && now_ms >= pending_menu_at) {
                    const int mode = pending_menu_mode;
                    pending_menu_mode = -1;
                    session.display.top_bar_press = 0;
                    session.display.top_bar_visible = false;
                    menu.open_screen(mode, session);
                }
                if (session.open_port_settings) {
                    session.open_port_settings = false;
                    menu.open = false;
                    settings_ui.open_overlay(session, true, session.port_settings_group);
                }
                if (settings_ui.open) {
                    pass_clock.skip_to(now_ms);
                    pacer.consumed(now_ms, 1);
                    if (const auto wait = pacer.wait_ms(SDL_GetTicks64()); wait != 0) {
                        SDL_Delay(static_cast<Uint32>(wait));
                    }
                    continue;
                }
                if (menu.open) {
                    // 0x41e289 owns the screen; the scene keeps its state but
                    // does not animate behind it.
                    pass_clock.skip_to(now_ms);
                    pacer.consumed(now_ms, 1);
                    if (const auto wait = pacer.wait_ms(SDL_GetTicks64()); wait != 0) {
                        SDL_Delay(static_cast<Uint32>(wait));
                    }
                    continue;
                }
                if (std::getenv("BYZ_ANIM_TRACE") != nullptr) {
                    static std::uint64_t last_anim_trace = 0;
                    if (now_ms - last_anim_trace >= 100) {
                        last_anim_trace = now_ms;
                        for (const auto& [id, record] : session.view.state.records) {
                            if (!record.frames_known || !record.current_frame) continue;
                            std::cout << "anim_trace ms=" << now_ms << " id=" << id
                                      << " frame=" << *record.current_frame
                                      << " count=" << record.field06
                                      << " delay=" << record.field04
                                      << " passes=" << pass_clock.owed(now_ms)
                                      << " applied=" << pass_clock.applied_total()
                                      << std::endl;
                        }
                    }
                }
                session.display.tick_cursor();
                const bool animates = session.display.scene_animates(session.view);
                if (!animates) {
                    // Nothing steps the frame lists: drop the outstanding passes
                    // so a later animation does not start with a backlog.
                    pass_clock.skip_to(now_ms);
                }
                if (pacer.due(now_ms) != 0) {
                    if (animates) {
                        // Draw the current frame without stepping it. The
                        // record pass clock owns the step: the old "+1 per
                        // compose" made the rate depend on the host's compose
                        // count (and event composes stepped the animation).
                        session.display.compose(session.view, false, false);
                        const auto owed = pass_clock.remaining(now_ms);
                        if (owed > stall_passes) {
                            // A scene load, conversation or media clip is not
                            // animation debt; do not replay it pass by pass.
                            pass_clock.skip_to(now_ms);
                        } else if (owed != 0) {
                            byzantine::advance_record_passes(
                                session.view.state, session.display.drawn_record_ids,
                                static_cast<std::size_t>(owed));
                            pass_clock.applied(owed);
                        }
                        session.display.present(session.view);
                    }
                    pacer.consumed(now_ms, 1);
                }
                if (const auto wait = pacer.wait_ms(SDL_GetTicks64()); wait != 0) {
                    SDL_Delay(static_cast<Uint32>(wait));
                }
            }
            if (options.selftest_events) {
                std::cout << "wait_selftest frames=" << session.wait.frames
                          << " events=" << session.wait.events
                          << " clicks_ignored=" << session.wait.clicks_ignored
                          << " overlay_toggled="
                          << (session.wait.overlay_toggled ? "true" : "false")
                          << " injected=" << (session.wait.injected ? "true" : "false")
                          << '\n';
            }
            session.display.log_present_summary();
            session.display.store_window_settings(session.options.settings);
            save_port_settings(session.options, true);
            save_seen_dialogue(session);
            SDL_DestroyTexture(session.display.texture);
            SDL_DestroyRenderer(session.display.renderer);
            SDL_DestroyWindow(session.display.window);
            SDL_Quit();
            return 0;
        }

        // --shot: one deterministic frame, no audio device wait.
        // One unbudgeted script segment; a conversation still runs for real,
        // using --conversation-choices because there is no window input loop.
        const auto run_shot = [&](EntryPoint point, const byzantine::SceneState& entry,
                                  const byzantine::SceneTrace&) {
            set_trace(session.view, trace_point(session.view, point, entry, 0));
            // 0xbd blocks in 0x413bdd; a headless run plays the clip the same
            // way so the log and the last frame match the interactive path
            // (--no-video skips it and says so).
            return play_trace_videos(session, session.view.trace, true);
        };
        bool startup_ended = false;
        if (probe.conversation_request) {
            ClickPlan startup;
            startup.ok = true;
            startup.conversation = true;
            startup.point = EntryPoint{session.view.label, options.entry, std::nullopt};
            startup.entry = probe_state;
            startup.entry_before = session.view.entry_state;
            startup.probe = probe;
            if (!execute_plan(session, startup, run_shot, true, &startup_ended)) {
                session.view.state = startup.entry;
                set_trace(session.view, probe);
                std::cout << "startup_not_committed\n";
            }
        } else {
            session.view.state = std::move(probe_state);
            set_trace(session.view, probe);
            // The entry script itself may open with a 0xbd; play it here because
            // this branch bypasses run_shot.
            play_trace_videos(session, session.view.trace, true);
        }
        if (!startup_ended) {
            apply_panorama_events(session.view, options, true);
            show_message_text(session, true);
        }
        log_entry_summary(session);
        resume_ambient(session, true);
        if (options.run_label) {
            ClickPlan plan;
            plan.ok = true;
            plan.action = static_cast<std::uint16_t>(*options.run_label);
            plan.point = EntryPoint{plan.action, false, std::nullopt};
            plan.entry_before = session.view.state;
            plan.entry = session.view.state;
            plan.probe = byzantine::trace_scene_label(session.view.script,
                                                      session.view.entries, plan.entry,
                                                      plan.action);
            plan.conversation = plan.probe.conversation_request.has_value();
            const auto blocker = byzantine::trace_blocker(plan.probe);
            const auto before_trace = session.view.trace;
            if (!blocker.empty()) {
                std::cout << "run_label " << plan.action << " stopped: " << blocker
                          << " at instruction " << plan.probe.stopped_at << '\n';
            } else {
                bool ended = false;
                if (!execute_plan(session, plan, run_shot, true, &ended)) {
                    session.view.state = plan.entry_before;
                    set_trace(session.view, before_trace);
                    std::cout << "run_label " << plan.action << " not committed\n";
                } else if (ended) {
                    std::cout << "run_label " << plan.action
                              << " click_result scene_session_ended\n";
                    follow_switches(session, false);
                } else {
                    ClickCommit commit;
                    commit.result_first = true;
                    commit.prefix = "run_label " + std::to_string(plan.action) + " ";
                    commit_click(session, options, commit, false);
                }
            }
        }
        apply_bar_options(session, options, true);
        if (options.click) {
            const auto plan = plan_hotspot(session.view, *options.click);
            std::cout << "click " << plan.message
                      << " handled=" << (plan.ok ? "true" : "false") << '\n';
            if (!plan.ok) clear_text_after_rejected_use(session, plan, true);
            if (plan.ok) {
                const auto before_trace = session.view.trace;
                bool ended = false;
                if (!execute_plan(session, plan, run_shot, true, &ended)) {
                    session.view.state = plan.entry_before;
                    set_trace(session.view, before_trace);
                    std::cout << "click_not_committed\n";
                } else if (ended) {
                    std::cout << "click_result scene_session_ended\n";
                    follow_switches(session, false);
                } else {
                    ClickCommit commit;
                    commit.object_slot = static_cast<std::uint16_t>(*options.click);
                    commit.result_first = true;
                    commit_click(session, options, commit, true);
                }
            }
        }
        for (const auto& point : options.click_ats) {
            if (point.second >= kScreenBandTop) {
                handle_bar_click(session, point.first, point.second, true);
                continue;
            }
            const auto hit = hotspot_at(session.view, point.first, point.second);
            std::cout << "click_at " << point.first << ',' << point.second << " -> "
                      << hit.test_x << ',' << hit.test_y;
            if (!hit.slot) {
                std::cout << " hit no hotspot\n";
                continue;
            }
            const auto plan = plan_hotspot(session.view, *hit.slot);
            // The action's own log lines (sounds, 0xbd videos) belong between
            // this hit line and its result, not appended to it.
            std::cout << ' ' << plan.message << '\n';
            if (!plan.ok) {
                clear_text_after_rejected_use(session, plan, true);
                continue;
            }
            const auto before_trace = session.view.trace;
            bool ended = false;
            if (!execute_plan(session, plan, run_shot, true, &ended)) {
                session.view.state = plan.entry_before;
                set_trace(session.view, before_trace);
                std::cout << "click_not_committed\n";
                continue;
            }
            if (ended) {
                std::cout << "click_result scene_session_ended\n";
                follow_switches(session, false);
                continue;
            }
            ClickCommit commit;
            commit.object_slot = *hit.slot;
            commit.result_first = true;
            commit.panorama_verbose = false;
            commit_click(session, options, commit, false);
        }
        refresh_background(options, session.view);
        session.display.compose(session.view);
        if (options.bar_show) {
            // Headless counterpart of 0x408fe7's hover: set the bar state the
            // pointer path would set, then compose the same 640x480 screen.
            session.display.top_bar_visible = *options.bar_show != 0;
            session.display.top_bar_hover = *options.bar_show;
            session.display.top_bar_press = 0;
            std::cout << "bar_show button=" << *options.bar_show
                      << " visible=" << (session.display.top_bar_visible ? "yes" : "no")
                      << '\n';
        }
        // --compose-frames N: keep drawing without input, exactly like the
        // interactive loop's 0x43b216 cadence, to observe the frame walk.
        for (std::size_t frame = 0; frame < options.compose_frames; ++frame) {
            session.display.compose(session.view, false);
            session.display.present(session.view);
        }
        session.display.present(session.view);
        // --cursor-at: the same 0x40c329/0x4083d4 selection the mouse uses, for
        // a deterministic headless check of the installed pointer.
        for (const auto& point : options.cursor_ats) {
            session.display.update_cursor_at(session.view, point.first, point.second);
        }
        if (options.print_hotspots) print_hotspot_screens(session.view);
        if (options.print_objects) print_objects(session.view);
        if (options.print_variables) print_variables(session);
        if (options.print_records) print_records(session.view);
        session.display.write_shot(session.view, options.shot);
        // --menu/--menu-click-at/--menu-shot: the same MenuUi the window uses,
        // driven without SDL input. This is the deterministic path for the
        // menu screens and their actions (a load/new game still runs the same
        // travel_to, so its scene becomes the current view).
        if (options.menu_mode || !options.menu_shot.empty() || !options.menu_clicks.empty() ||
            !options.menu_texts.empty() || !options.menu_keys.empty()) {
            MenuUi shot_menu;
            shot_menu.init(session);
            shot_menu.open_screen(options.menu_mode.value_or(2), session);
            SettingsUi shot_settings;
            apply_headless_menu_input(session, shot_menu, options, shot_settings);
            if (!options.menu_shot.empty()) {
                if (shot_menu.open || shot_settings.open) {
                    if (shot_settings.open) shot_settings.draw(session);
                    else shot_menu.draw(session);
                    if (byzantine::write_ppm(session.display.screen, options.menu_shot)) {
                        std::cout << "menu_shot file=" << options.menu_shot
                                  << " mode=" << shot_menu.mode << '\n';
                    }
                } else {
                    std::cout << "menu_shot skipped: the menu closed before the shot\n";
                }
            }
        }
        session.display.log_present_summary();
        std::cout << "scene=" << session.view.name
                  << " background=" << session.view.background_name
                  << " hotspots=" << session.view.trace.active_hotspots.size() << " slots=";
        for (const auto& hotspot : session.view.trace.active_hotspots) {
            std::cout << hotspot.slot << ',';
        }
        std::cout << '\n';
        SDL_DestroyTexture(session.display.texture);
        SDL_DestroyRenderer(session.display.renderer);
        SDL_DestroyWindow(session.display.window);
        SDL_Quit();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        SDL_Quit();
        return 1;
    }
}
