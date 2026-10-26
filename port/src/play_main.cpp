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

#include "byzantine/cb.hpp"
#include "byzantine/cc.hpp"
#include "byzantine/compose.hpp"
#include "byzantine/conversation.hpp"
#include "byzantine/jpeg.hpp"
#include "byzantine/panorama.hpp"
#include "byzantine/panorama_view.hpp"
#include "byzantine/scene_setup.hpp"
#include "byzantine/text.hpp"

#include <SDL.h>

#include <algorithm>
#include <array>
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
#include <vector>

namespace {

using byzantine::kPlayfieldTop;

constexpr int kSceneWidth = 640;
constexpr int kSceneHeight = 400;
constexpr Uint32 kAudioRate = 22050;

std::string lower_ascii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
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

struct Options {
    std::string cc_path;
    std::string cb_path;
    std::string label;
    std::map<std::string, std::string> scene;  // passthrough for load_scene_setup
    std::filesystem::path backgrounds;         // backs/ root or the extracted root
    std::filesystem::path sounds;              // sounds/ root
    std::filesystem::path fonts;               // the DIALOG/klio font files
    std::filesystem::path ccb;                 // split CCB.ALL (engine/<name>.ccb)
    std::filesystem::path ccf;                 // split CCF.ALL (engine/<name>.ccf)
    std::filesystem::path byz;                 // split Byz.all (engine/<name>.byz)
    std::filesystem::path dialog;              // dialog/<conversation>/<clip>.avi root
    std::filesystem::path dialog_cache;        // decoded clip cache (created on demand)
    std::string shot;                          // --shot <file.ppm>
    std::string conversation_shot;             // --conversation-shot <file.ppm>
    std::string conversation_video_shot;       // first decoded media frame
    bool entry = false;                        // --entry: start at instruction 0
    std::optional<int> run_label;              // --run-label <n>: after --entry
    std::optional<int> click;                  // --click <slot>
    std::vector<std::pair<int, int>> click_ats;  // --click-at <x>,<y>, repeatable
    std::optional<int> staged_x;               // --staged-x <0..2495>
    bool overlay = true;
    bool print_audio = false;
    bool audio = true;
    bool video = true;                         // --no-video disables FFmpeg decodes
    bool selftest_events = false;              // --selftest-events (see README)
    // --conversation-choices 1,0,...: deterministic answers for --shot/tests.
    // The interactive player never uses this: an answer is a real mouse click.
    std::vector<std::size_t> conversation_choices;
    // --conversation-object <slot>: a deterministic scene-object click while an
    // answer list is up (0x436b01's other return path; --shot only).
    std::optional<int> conversation_object;
};

const std::set<std::string> kValued = {
    "--save", "--engine", "--lcx", "--lci", "--ci", "--lcy", "--cy", "--co", "--cf",
    "--ca", "--anims", "--gameprefs", "--inventor", "--hotitems", "--cs", "--ce",
    "--backgrounds", "--sounds", "--shot", "--click", "--click-at", "--staged-x",
    "--fonts", "--cd", "--ccb", "--ccf", "--byz", "--dialog", "--dialog-cache",
    "--run-label", "--conversation-choices", "--conversation-shot",
    "--conversation-object", "--conversation-video-shot"};

Options parse_options(int argc, char** argv) {
    Options options;
    std::vector<std::string> positional;
    for (int index = 1; index < argc; ++index) {
        const std::string token(argv[index]);
        if (token == "--no-hotspots") {
            options.overlay = false;
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
        if (token == "--no-video") {
            options.video = false;
            continue;
        }
        if (token == "--selftest-events") {
            options.selftest_events = true;
            continue;
        }
        if (token == "--entry") {
            options.entry = true;
            continue;
        }
        if (kValued.count(token) != 0) {
            if (index + 1 >= argc) throw std::runtime_error(token + " needs a value");
            const std::string value(argv[++index]);
            if (token == "--backgrounds") options.backgrounds = value;
            else if (token == "--sounds") options.sounds = value;
            else if (token == "--fonts") options.fonts = value;
            else if (token == "--ccb") options.ccb = value;
            else if (token == "--ccf") options.ccf = value;
            else if (token == "--byz") options.byz = value;
            else if (token == "--dialog") options.dialog = value;
            else if (token == "--dialog-cache") options.dialog_cache = value;
            else if (token == "--run-label") options.run_label = std::stoi(value);
            else if (token == "--conversation-choices") {
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
            else if (token == "--click") options.click = std::stoi(value);
            else if (token == "--staged-x") options.staged_x = std::stoi(value);
            else if (token == "--click-at") {
                const auto comma = value.find(',');
                if (comma == std::string::npos) {
                    throw std::runtime_error("--click-at expects <x>,<y>");
                }
                options.click_ats.emplace_back(std::stoi(value.substr(0, comma)),
                                               std::stoi(value.substr(comma + 1)));
            } else options.scene[token] = value;
            continue;
        }
        if (token.rfind("--", 0) == 0) throw std::runtime_error("unknown option: " + token);
        positional.push_back(token);
    }
    if (positional.size() != 3 && !(options.entry && positional.size() == 2)) {
        throw std::runtime_error(
            "usage: byzantine-play <scene.cc> <scene.cb> <label> "
            "[--save <btb>] [--engine <dir>] [--backgrounds <dir>] [--sounds <dir>] "
            "[--lcx --lci --ci --lcy --cy --co --cf --ca --anims --gameprefs "
            "--inventor --hotitems --cs <file>] [--shot <file.ppm>] [--click <slot>] "
            "[--entry] [--click-at <x>,<y> ...] [--staged-x <0..2495>] "
            "[--cd <file> --ccb --ccf --byz --dialog --dialog-cache <dir>] "
            "[--run-label <n>] [--conversation-choices <i,...>] "
            "[--conversation-object <slot>] [--conversation-shot <ppm>] "
            "[--conversation-video-shot <ppm>] [--no-video] "
            "[--no-hotspots] [--no-audio] [--print-audio] [--selftest-events]");
    }
    options.cc_path = positional[0];
    options.cb_path = positional[1];
    if (positional.size() == 3) options.label = positional[2];
    return options;
}

// ---------------------------------------------------------------------------
// WAV playback through SDL2's audio device (the ready-made library part).
// ---------------------------------------------------------------------------
struct WavClip {
    std::vector<std::uint8_t> bytes;
    int channels = 0;
    int sample_rate = 0;
    std::uint64_t duration_ms = 0;
};

std::uint32_t read_le32(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::uint32_t>(data.at(at)) |
           (static_cast<std::uint32_t>(data.at(at + 1)) << 8) |
           (static_cast<std::uint32_t>(data.at(at + 2)) << 16) |
           (static_cast<std::uint32_t>(data.at(at + 3)) << 24);
}

std::uint16_t read_le16(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::uint16_t>(data.at(at) | (data.at(at + 1) << 8));
}

// Strict RIFF/WAVE walk: PCM 16-bit only, which is what every shipped
// sounds/*.WAV is (analysis/rea/LIBRARY_STRATEGY.md §5).
WavClip parse_wav(const std::vector<std::uint8_t>& data, const std::string& path) {
    if (data.size() < 12 || std::memcmp(data.data(), "RIFF", 4) != 0 ||
        std::memcmp(data.data() + 8, "WAVE", 4) != 0) {
        throw std::runtime_error("not a RIFF/WAVE file: " + path);
    }
    WavClip clip;
    std::size_t at = 12;
    int bits = 0;
    int format = 0;
    std::size_t data_at = 0;
    std::size_t data_size = 0;
    while (at + 8 <= data.size()) {
        const std::string id(reinterpret_cast<const char*>(data.data() + at), 4);
        const auto size = static_cast<std::size_t>(read_le32(data, at + 4));
        const std::size_t body = at + 8;
        if (body + size > data.size()) throw std::runtime_error("truncated WAVE chunk: " + path);
        if (id == "fmt ") {
            if (size < 16) throw std::runtime_error("short fmt chunk: " + path);
            format = read_le16(data, body);
            clip.channels = read_le16(data, body + 2);
            clip.sample_rate = static_cast<int>(read_le32(data, body + 4));
            bits = read_le16(data, body + 14);
        } else if (id == "data") {
            data_at = body;
            data_size = size;
        }
        at = body + size + (size & 1);
    }
    if (format != 1) throw std::runtime_error("WAV is not PCM: " + path);
    if (bits != 16) throw std::runtime_error("WAV is not 16-bit: " + path);
    if (clip.channels < 1 || clip.channels > 2 || clip.sample_rate <= 0) {
        throw std::runtime_error("unsupported WAV channel layout: " + path);
    }
    if (data_size == 0) throw std::runtime_error("WAV has no samples: " + path);
    clip.bytes.assign(data.begin() + static_cast<std::ptrdiff_t>(data_at),
                      data.begin() + static_cast<std::ptrdiff_t>(data_at + data_size));
    const auto frames = data_size / (static_cast<std::size_t>(clip.channels) * 2);
    clip.duration_ms =
        static_cast<std::uint64_t>(frames) * 1000 / static_cast<std::uint64_t>(clip.sample_rate);
    return clip;
}

class SoundPlayer {
public:
    ~SoundPlayer() {
        if (stream_ != nullptr) SDL_FreeAudioStream(stream_);
        if (device_ != 0) SDL_CloseAudioDevice(device_);
    }

    void open() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
            throw std::runtime_error(std::string("SDL audio init failed: ") + SDL_GetError());
        }
        SDL_AudioSpec wanted{};
        wanted.freq = static_cast<int>(kAudioRate);
        wanted.format = AUDIO_S16SYS;
        wanted.channels = 2;
        wanted.samples = 1024;
        device_ = SDL_OpenAudioDevice(nullptr, 0, &wanted, &obtained_, 0);
        if (device_ == 0) {
            throw std::runtime_error(std::string("no audio device: ") + SDL_GetError());
        }
        SDL_PauseAudioDevice(device_, 0);
        name_ = "SDL2 " + std::to_string(obtained_.freq) + "Hz " +
                std::to_string(static_cast<int>(obtained_.channels)) + "ch";
        std::cout << "audio device freq=" << obtained_.freq
                  << " format=0x" << std::hex << static_cast<int>(obtained_.format)
                  << std::dec << " channels=" << static_cast<int>(obtained_.channels)
                  << " samples=" << obtained_.samples << '\n';
    }

    bool ready() const { return device_ != 0; }
    const std::string& device_name() const { return name_; }

    // Queues the clip; the caller keeps the window alive while `queued()` drains.
    void queue(const WavClip& clip) {
        if (device_ == 0) return;
        if (stream_ != nullptr) SDL_FreeAudioStream(stream_);
        stream_ = SDL_NewAudioStream(AUDIO_S16SYS, static_cast<Uint8>(clip.channels),
                                     clip.sample_rate, AUDIO_S16SYS, 2,
                                     static_cast<int>(kAudioRate));
        if (stream_ == nullptr) throw std::runtime_error(SDL_GetError());
        if (SDL_AudioStreamPut(stream_, clip.bytes.data(),
                               static_cast<int>(clip.bytes.size())) != 0) {
            throw std::runtime_error(std::string("audio stream put failed: ") + SDL_GetError());
        }
        SDL_AudioStreamFlush(stream_);
        const int available = SDL_AudioStreamAvailable(stream_);
        std::vector<std::uint8_t> converted(static_cast<std::size_t>(std::max(0, available)));
        if (available > 0) SDL_AudioStreamGet(stream_, converted.data(), available);
        if (!converted.empty() &&
            SDL_QueueAudio(device_, converted.data(), static_cast<Uint32>(converted.size())) != 0) {
            throw std::runtime_error(std::string("audio queue failed: ") + SDL_GetError());
        }
    }

    std::uint32_t queued() const { return device_ == 0 ? 0 : SDL_GetQueuedAudioSize(device_); }

private:
    SDL_AudioDeviceID device_ = 0;
    SDL_AudioSpec obtained_{};
    SDL_AudioStream* stream_ = nullptr;
    std::string name_ = "closed";
};

// ---------------------------------------------------------------------------
// Scene state
// ---------------------------------------------------------------------------
// The visible scene layer: the last of 0x69 (background) and 0xb9 (node) the
// committed trace contains (see update_layer).
enum class SceneLayer { none, background, panorama };

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
};

// One sound the label queues, in script order.
struct SoundEvent {
    std::size_t step = 0;
    std::size_t instruction = 0;
    std::uint16_t id = 0;
    std::optional<bool> allowed;
    std::uint64_t cap_ms = 0;
};

std::vector<SoundEvent> sound_events(const byzantine::SceneTrace& trace) {
    std::vector<SoundEvent> events;
    for (const auto& start : trace.sound_starts) {
        events.push_back({start.step, start.instruction, start.id, start.allowed, 0});
    }
    for (const auto& session : trace.sound_sessions) {
        std::uint64_t cap = 0;
        for (const auto& timeout : session.timeouts) {
            if (timeout.defined) {
                cap = timeout.milliseconds;
                break;
            }
        }
        events.push_back({session.step, session.instruction, session.id, session.allowed, cap});
    }
    std::stable_sort(events.begin(), events.end(), [](const SoundEvent& left,
                                                      const SoundEvent& right) {
        if (left.step != right.step) return left.step < right.step;
        return left.instruction < right.instruction;
    });
    return events;
}

std::filesystem::path background_path(const Options& options, const std::string& entry_name) {
    if (options.backgrounds.empty() || entry_name.size() < 2) return {};
    auto root = options.backgrounds;
    if (std::filesystem::is_directory(root / "backs")) root /= "backs";
    const auto found = find_child(root / lower_ascii(entry_name.substr(0, 2)), entry_name);
    return found ? *found : std::filesystem::path{};
}

// 0x40186e builds "nodes\\<first two characters>\\<name>.jpg"; the shipped tree
// mixes .JPG/.jpg, so the lookup is case-insensitive like the background one.
std::filesystem::path node_path(const Options& options, const std::string& entry_name) {
    if (options.backgrounds.empty() || entry_name.size() < 2) return {};
    auto root = options.backgrounds;
    if (std::filesystem::is_directory(root / "nodes")) root /= "nodes";
    const auto found = find_child(root / lower_ascii(entry_name.substr(0, 2)), entry_name);
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
    const std::array<std::filesystem::path, 3> candidates = {
        base, base / lower_ascii(scene),
        base / lower_ascii(entry_name.substr(0, std::min<std::size_t>(2, entry_name.size())))};
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
void update_layer(SceneView& view, bool fresh_scene) {
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
    if (fresh_scene || view.layer == SceneLayer::none) {
        view.layer = SceneLayer::background;
    }
}

void set_trace(SceneView& view, byzantine::SceneTrace trace) {
    view.trace = std::move(trace);
    update_layer(view, false);
}

bool panorama_layer(const SceneView& view) { return view.layer == SceneLayer::panorama; }

std::string pick_background(const SceneView& view) {
    if (view.trace.background) return view.trace.background->entry.name;
    const auto static_entries = byzantine::static_backgrounds(view.script, view.entries);
    if (!static_entries.empty()) return static_entries.front().entry.name;
    return {};
}

void refresh_background(const Options& options, SceneView& view) {
    view.background_name = pick_background(view);
    view.background_file = background_path(options, view.background_name);
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
                          << (loaded ? " loaded" : " reused") << '\n';
            }
        } else if (verbose) {
            std::cout << "panorama_load instruction=" << load.instruction
                      << " name=" << load.name << " record=" << load.record
                      << " staged_x=none yaw=" << view.panorama->yaw_degrees()
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
}

// ---------------------------------------------------------------------------
// Composition: background JPEG + records -> RGB565 surface -> SDL texture.
// ---------------------------------------------------------------------------
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
    // draws the answer list with slot 0; the port uses it for the dialogue
    // scrollback too because the original's bottom 60-px band is not part of the
    // 640x400 ported surface.
    std::optional<byzantine::SfnFont> dialog_font;

    void init() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error(std::string("SDL video init failed: ") + SDL_GetError());
        }
        window = SDL_CreateWindow("Byzantine: The Betrayal - scene player",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  kSceneWidth * 2, kSceneHeight * 2,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == nullptr) throw std::runtime_error(SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr) throw std::runtime_error(SDL_GetError());
        SDL_RenderSetLogicalSize(renderer, kSceneWidth, kSceneHeight);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                    SDL_TEXTUREACCESS_STREAMING, kSceneWidth, kSceneHeight);
        if (texture == nullptr) throw std::runtime_error(SDL_GetError());
        surface = byzantine::make_surface(kSceneWidth, kSceneHeight, 0);
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

    void compose(SceneView& view, bool verbose = true) {
        std::fill(surface.pixels.begin(), surface.pixels.end(), 0);
        // The scene layer is whichever of 0x69 (background) / 0xb9 (node) ran
        // LAST (see panorama_layer).
        const bool show_panorama = panorama_layer(view);
        if (show_panorama) {
            // 0x4242bd samples the current node through the camera map; the
            // record pass then draws on top, exactly like the background path.
            view.panorama->render(surface);
            const auto drawn = byzantine::compose_scene(view.state, surface);
            draw_text_windows(view, verbose);
            if (!verbose) return;
            std::cout << "composed panorama=" << view.panorama->path()
                      << " size=" << view.panorama->source_width() << 'x'
                      << view.panorama->source_height()
                      << " padding=" << view.panorama->padding()
                      << " yaw=" << view.panorama->yaw_degrees()
                      << " pitch=" << view.panorama->pitch_degrees()
                      << " drawn=" << drawn.drawn
                      << " skipped_no_frame=" << drawn.skipped_no_frame
                      << " skipped_snap=" << drawn.skipped_snap
                      << " skipped_hidden=" << drawn.skipped_hidden
                      << " pixels=" << drawn.pixels_written
                      << " drawn_ids=" << drawn_ids(drawn) << '\n';
            return;
        }
        load_background(view.background_file);
        if (!background_pixels.empty()) {
            byzantine::blit_image(surface, kSceneWidth, kSceneHeight,
                                  background_pixels.data(), 0, 0);
        }
        const auto drawn = byzantine::compose_scene(view.state, surface);
        draw_text_windows(view, verbose);
        if (!verbose) return;
        std::cout << "composed background="
                  << (view.background_file.empty() ? "none"
                                                   : view.background_file.filename().string())
                  << " drawn=" << drawn.drawn
                  << " skipped_no_frame=" << drawn.skipped_no_frame
                  << " skipped_snap=" << drawn.skipped_snap
                  << " skipped_hidden=" << drawn.skipped_hidden
                  << " pixels=" << drawn.pixels_written
                  << " drawn_ids=" << drawn_ids(drawn) << '\n';
    }

    void present(const SceneView& view, bool overlay) {
        SDL_UpdateTexture(texture, nullptr, surface.pixels.data(), surface.width * 2);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        // A hotspot rectangle lives in the coordinates the click test uses: the
        // 640x400 playfield for a background scene, the panorama SOURCE for a
        // panorama scene. Only the former can be drawn as a rectangle here, so
        // the panorama outline is not invented (the click still works).
        if (overlay && !panorama_layer(view)) {
            for (const auto& hotspot : view.trace.active_hotspots) {
                SDL_Rect bounds{hotspot.x, hotspot.y - kPlayfieldTop, hotspot.width,
                                hotspot.height};
                if (bounds.y < 0) {
                    bounds.h += bounds.y;
                    bounds.y = 0;
                }
                if (bounds.y + bounds.h > kSceneHeight) bounds.h = kSceneHeight - bounds.y;
                if (bounds.x < 0) {
                    bounds.w += bounds.x;
                    bounds.x = 0;
                }
                if (bounds.x + bounds.w > kSceneWidth) bounds.w = kSceneWidth - bounds.x;
                if (bounds.w <= 0 || bounds.h <= 0) continue;
                SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
                SDL_RenderDrawRect(renderer, &bounds);
            }
        }
        SDL_RenderPresent(renderer);
    }

    // 0x4177c7 (examine, font 1, 0x14a, 3 lines, left) and 0x417a17 (message,
    // font 3, 0x26e/0x2d, 3 lines, right-aligned). The original reveals the
    // examine text one line per second and blits a message panel behind the
    // right block; this draws the final block over the scene and says so.
    void draw_text_windows(const SceneView& view, bool verbose) {
        if (view.examine_text.active && examine_font) {
            byzantine::draw_text(surface, *examine_font, view.examine_text.text, 5, 0x14a,
                                 3, 3);
            if (verbose) {
                std::cout << "text_draw window=examine x=5 y=" << 0x14a
                          << " width=" << byzantine::text_width(*examine_font,
                                                               view.examine_text.text)
                          << '\n';
            }
        }
        if (view.message_text.active && message_font) {
            const auto width = byzantine::text_width(*message_font, view.message_text.text);
            byzantine::draw_text(surface, *message_font, view.message_text.text, 0x26e,
                                 0x2d, 3, 4);
            if (verbose) {
                std::cout << "text_draw window=message x=" << (0x26e - width)
                          << " y=" << 0x2d << " width=" << width << '\n';
            }
        }
    }

    // Writes the composed scene (background + records) as PPM. The hotspot
    // outlines are a window-only overlay and are deliberately not in the file.
    // SDL_RenderReadPixels is not used: it reads at the window/render-target
    // resolution (the logical 640x400 is a scaled viewport), so the composed
    // surface is the only exact 640x400 image.
    void write_shot(const std::string& path) {
        if (!byzantine::write_ppm(surface, path)) {
            throw std::runtime_error("cannot write " + path);
        }
        std::cout << "shot=" << path << '\n';
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
    SoundPlayer audio;
    // SHOW.LCC/LCV/LVM from --engine: the scene list that turns a 0x73 index
    // into a scene name, and the per-scene local->global variable rows.
    std::optional<byzantine::VariableMetadata> metadata;
    // engine/<scene>ce, the text entries 0x412206 loads into DAT_00483640.
    // Reloaded on a scene switch from the target's --ce record.
    std::optional<byzantine::SceneTextTable> scene_text;
    // Set by the interactive branch: a scene entry may then wait on audio like
    // any other script. --shot stays deterministic and never waits.
    bool interactive = false;
    bool running = true;
    WaitStats wait;
};

// Processed while a sound plays: the script is waiting, so a click must not
// start another label, but the window still redraws and H/Esc keep working.
void pump_window(Session& session) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ++session.wait.events;
        if (event.type == SDL_QUIT) {
            session.running = false;
            return;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            session.running = false;
            return;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_h) {
            session.options.overlay = !session.options.overlay;
            session.wait.overlay_toggled = true;
            continue;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            ++session.wait.clicks_ignored;
        }
    }
    session.display.present(session.view, session.options.overlay);
    ++session.wait.frames;
}

// Queues the clip, then keeps the window alive until the device drains it.
void play_and_wait(Session& session, const WavClip& clip, std::uint64_t cap_ms) {
    session.audio.queue(clip);
    if (session.options.selftest_events && !session.wait.injected) {
        // Test hook: prove that the wait loop still processes input. H must
        // toggle the overlay and a click must be ignored, not start a label.
        session.wait.injected = true;
        SDL_Event key{};
        key.type = SDL_KEYDOWN;
        key.key.keysym.sym = SDLK_h;
        SDL_PushEvent(&key);
        SDL_Event click{};
        click.type = SDL_MOUSEBUTTONDOWN;
        click.button.button = SDL_BUTTON_LEFT;
        click.button.x = kSceneWidth / 2;
        click.button.y = kSceneHeight / 2;
        SDL_PushEvent(&click);
    }
    const std::uint64_t limit =
        cap_ms == 0 ? clip.duration_ms + 50 : std::min(cap_ms, clip.duration_ms + 50);
    const auto start = std::chrono::steady_clock::now();
    while (session.running && session.audio.queued() != 0) {
        pump_window(session);
        if (!session.running) return;
        const auto waited = static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start)
                .count());
        if (waited >= limit) break;
        SDL_Delay(4);
    }
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
        const auto file = sound_path(session.options.sounds, *name, view.name);
        if (!file) {
            if (verbose) {
                std::cout << "audio step=" << event.step << " id=" << event.id
                          << " name=" << *name << " skipped: no such WAV under "
                          << session.options.sounds.string() << '\n';
            }
            continue;
        }
        WavClip clip;
        try {
            clip = parse_wav(read_file(*file), file->string());
        } catch (const std::exception& error) {
            std::cout << "audio " << error.what() << '\n';
            continue;
        }
        if (verbose) {
            std::cout << "audio step=" << event.step << " instruction=" << event.instruction
                      << " id=" << event.id << " name=" << *name
                      << " file=" << file->filename().string()
                      << " duration_ms=" << clip.duration_ms;
            if (event.cap_ms) std::cout << " session_cap_ms=" << event.cap_ms;
            std::cout << std::flush;
        }
        const auto before = std::chrono::steady_clock::now();
        play_and_wait(session, clip, event.cap_ms);
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
// script's own step count, so the final state equals an unbudgeted run.
bool run_label_gated(Session& session, EntryPoint point,
                     const byzantine::SceneState& entry,
                     const byzantine::SceneTrace& probe, bool verbose) {
    const auto events = sound_events(probe);
    std::vector<std::size_t> budgets;
    for (const auto& event : events) {
        if (event.step != 0) budgets.push_back(event.step);
    }
    budgets.push_back(probe.steps);
    std::sort(budgets.begin(), budgets.end());
    budgets.erase(std::unique(budgets.begin(), budgets.end()), budgets.end());

    std::size_t played = 0;
    for (const auto budget : budgets) {
        auto trace = trace_point(session.view, point, entry, budget);
        set_trace(session.view, std::move(trace));
        const auto all = sound_events(session.view.trace);
        if (all.size() <= played) continue;
        refresh_background(session.options, session.view);
        session.display.compose(session.view, verbose);
        session.display.present(session.view, session.options.overlay);
        if (verbose) {
            std::cout << "gate stopped_after_step=" << session.view.trace.steps
                      << " of=" << probe.steps << " (sound queued here)\n";
        }
        if (!play_events(session, session.view, all, played, all.size(), verbose)) {
            return false;
        }
        played = all.size();
    }
    return session.running;
}

// ---------------------------------------------------------------------------
// Scene travel (0x73)
// ---------------------------------------------------------------------------
bool audio_gated(const Session& session) {
    return session.audio.ready() && !session.options.sounds.empty() &&
           session.view.state.sound_table_known;
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
    const auto found = session.options.scene.find("--ce");
    if (found == session.options.scene.end()) return;
    try {
        session.scene_text = byzantine::parse_scene_text(read_file(found->second));
        if (verbose) {
            std::cout << "scene_text file=" << found->second
                      << " entries=" << session.scene_text->entries.size() << '\n';
        }
    } catch (const std::exception& error) {
        std::cout << "scene_text " << found->second << ": " << error.what() << '\n';
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
bool travel_to(Session& session, std::uint16_t index, bool verbose) {
    auto& options = session.options;
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
    next.erase("--save");
    std::string missing;
    for (const char* key : {"--ci", "--cy", "--cf", "--cs", "--ca", "--co", "--ce"}) {
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
    const auto from_locals = locals_digest(session.view.state);
    const auto pool = byzantine::export_variable_pool(session.view.state, from_layout);
    std::string written;
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

    SceneView next_view;
    next_view.cc_path = cc_file->string();
    next_view.cb_path = cb_file->string();
    next_view.script = byzantine::parse_cc(read_file(next_view.cc_path));
    next_view.entries = byzantine::parse_cb(read_file(next_view.cb_path));
    next_view.name = std::filesystem::path(next_view.cc_path).stem().string();
    auto setup = byzantine::load_scene_setup(next_view.cc_path, next);
    byzantine::apply_variable_pool(setup.state, pool, to_layout);
    // 0x40fd7b: the scene load pre-loads the CA type 2/3 records before the
    // script runs (the save path is not used here, so nothing replaces them).
    const auto preload = byzantine::preload_scene_records(setup.state);
    next_view.entry_state = setup.state;
    next_view.state = setup.state;
    next_view.pool_on_arrival = pool;
    // 0x401256 fixed the node size once at startup; the current node stays in
    // DAT_00464880 until the new scene's own 0x69/0xb9 replaces it, but the
    // visible layer restarts from `none` so the node cannot hide this scene's
    // background.
    next_view.panorama_size = session.view.panorama_size;
    next_view.panorama = std::move(session.view.panorama);
    next_view.layer = SceneLayer::none;

    std::cout << "travel index=" << index << " scene=" << target << " from=" << from
              << " pool=3000 locals=300"
              << " pool_changed=" << written
              << " from_locals_digest=0x" << std::hex << from_locals
              << " pool_digest=0x" << pool_digest(pool)
              << " locals_digest=0x" << locals_digest(setup.state) << std::dec;
    if (preload.created != 0 || preload.failed != 0) {
        std::cout << " record_preload=" << preload.created;
        if (preload.failed != 0) std::cout << " record_preload_failed=" << preload.failed;
    }
    if (!missing.empty()) std::cout << " missing_tables=" << missing;
    std::cout << '\n';

    session.view = std::move(next_view);
    options.scene = std::move(next);
    options.cc_path = session.view.cc_path;
    options.cb_path = session.view.cb_path;
    load_scene_text(session, verbose);

    // The new scene runs from its first instruction (DAT_0047a238), exactly
    // like 0x42c8a0's reload; its sounds are gated like any other script.
    const auto probe = byzantine::trace_scene_entry(
        session.view.script, session.view.entries, session.view.entry_state, 0);
    if (session.interactive && audio_gated(session)) {
        if (!run_label_gated(session, EntryPoint{0, true, std::nullopt}, session.view.entry_state, probe,
                             verbose)) {
            return false;
        }
    } else {
        set_trace(session.view,
                  trace_point(session.view, EntryPoint{0, true, std::nullopt},
                              session.view.entry_state, 0));
    }
    // The entry's own trace selects the layer for this scene (see update_layer).
    update_layer(session.view, true);
    show_message_text(session, verbose);
    apply_panorama_events(session.view, options, verbose);
    refresh_background(options, session.view);
    session.display.compose(session.view, verbose);
    session.display.present(session.view, options.overlay);
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
    return session.running;
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

// ---------------------------------------------------------------------------
// Clicks
// ---------------------------------------------------------------------------
struct ClickPlan {
    bool ok = false;
    bool travel = false;
    bool conversation = false;
    std::uint16_t switch_index = 0;
    std::string message;
    std::uint16_t action = 0;
    EntryPoint point;                    // where the action's segment starts
    byzantine::SceneState entry;
    byzantine::SceneState entry_before;  // before the probe (gated replay input)
    byzantine::SceneTrace probe;
};

// 0x40c02c: the point that 0x401ca6 produced is compared with every active
// DAT_00464300 object rect, ids ascending, first hit wins. For a panorama the
// point is the SOURCE coordinate 0x4240ae's inverse map returns with the 0x14
// playfield bias added back; otherwise it is the screen point.
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

// A hotspot whose action reaches 0x73 is a scene switch: 0x42db22 writes
// DAT_00482428 and ends the script, so the port commits the action's state and
// then performs the scene reload the engine's outer loop would.
ClickPlan plan_hotspot(const SceneView& view, int slot) {
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

// ---------------------------------------------------------------------------
// 0x70 CONVERSE: the real conversation VM and its media
// ---------------------------------------------------------------------------
// 0x410669 turns a scene-cd record's file stem into engine/<stem>.ccb (the
// instruction lists and group tables), .ccf (voice-clip names) and .byz (the
// dialogue lines). The three files are the shipped data; nothing here invents a
// subtitle or a choice.

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

// The game's talking-head clips are Indeo 4 + PCM16. Indeo is a standard codec
// this port must not reimplement, so FFmpeg (the ready decoder available here)
// materialises a per-clip cache of JPEG frames and a PCM WAV once. The runtime
// then uses the port's existing JPEG and WAV readers; a missing FFmpeg only
// suppresses media and is reported, never silently hidden.
std::optional<DecodedClip> dialog_clip_cache(const Options& options,
                                             const std::filesystem::path& avi,
                                             const std::string& conversation,
                                             const std::string& clip, bool verbose) {
    const auto cache_root = options.dialog_cache.empty() ? options.dialog / ".cache"
                                                         : options.dialog_cache;
    if (cache_root.empty()) return std::nullopt;
    const auto directory = cache_root / lower_ascii(conversation) /
                           lower_ascii(clip);
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    if (error) {
        std::cout << "conversation_media clip=" << clip << " cache_error="
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
        std::cout << "conversation_media clip=" << clip
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
        std::cout << "conversation_media clip=" << clip
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
        std::cout << "conversation_media clip=" << clip
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
        std::cout << "conversation_media clip=" << clip << " source=" << avi.filename().string()
                  << " frames=" << decoded.frames.size() << " fps=" << decoded.fps
                  << " size=" << decoded.width << 'x' << decoded.height
                  << (decoded.audio ? " audio=pcm" : " audio=none") << '\n';
    }
    return decoded;
}

struct DialogUi {
    std::vector<std::uint16_t> base;
    std::vector<std::string> scrollback;  // at most 3 visual lines (DAT_0045fdb8)
    const byzantine::SfnFont* font = nullptr;
    int line_height = 16;

    void restore(Session& session) const {
        if (base.size() == session.display.surface.pixels.size()) {
            std::copy(base.begin(), base.end(), session.display.surface.pixels.begin());
        }
    }

    void draw_text_overlay(Session& session) const {
        // The original's dialogue is row 0x1a4 of the 640x480 surface, inside
        // the 60-px band below the playfield. The port has no band, so the same
        // lines are drawn over the top of the playfield with a black panel.
        if (font != nullptr) {
            const int panel_height = static_cast<int>(scrollback.size()) * line_height + 8;
            for (int y = 0; y < panel_height && y < kSceneHeight; ++y) {
                for (int x = 48; x < 592; ++x) session.display.surface.at(x, y) = 0;
            }
            int y = 4;
            for (const auto& line : scrollback) {
                byzantine::draw_text(session.display.surface, *font, line, 55, y - 4,
                                    1, 3);
                y += line_height;
            }
        }
    }

    void draw(Session& session, bool answers) const {
        restore(session);
        draw_text_overlay(session);
        (void)answers;
    }
};

void draw_answer_panel(DialogUi& ui, Session& session,
                       const std::vector<byzantine::ConversationAnswer>& answers,
                       const std::vector<std::filesystem::path>& /*unused*/,
                       std::vector<SDL_Rect>* rects) {
    ui.draw(session, true);
    if (ui.font == nullptr) return;
    rects->clear();
    int bottom = kSceneHeight - 6;
    for (std::size_t index = answers.size(); index-- > 0;) {
        const auto& answer = answers[index];
        const auto lines = wrap_dialog_text(*ui.font, answer.show_text ? answer.text : "",
                                            480);
        const int height = static_cast<int>(lines.size()) * ui.line_height + 4;
        const int top = bottom - height;
        for (int y = std::max(0, top); y < bottom && y < kSceneHeight; ++y) {
            for (int x = 92; x < 592; ++x) session.display.surface.at(x, y) = 0;
        }
        int y = top + 2;
        for (const auto& line : lines) {
            if (!line.empty()) {
                byzantine::draw_text(session.display.surface, *ui.font, line, 100, y, 1, 3);
            }
            y += ui.line_height;
        }
        rects->push_back(SDL_Rect{92, std::max(0, top), 500, height});
        bottom = top - 4;
    }
    std::reverse(rects->begin(), rects->end());
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
    const auto ccf = find_child(options.ccf, stem);
    const auto byz = find_child(options.byz, stem);
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
    if (verbose) {
        std::cout << "conversation record=" << record_index << " name=" << record.name
                  << " stem=" << stem << " clips=" << program.clips.size()
                  << " texts=" << program.texts.size()
                  << " trailing_bytes=" << program.trailing_bytes
                  << " object_context=" << object_context << '\n';
    }
    byzantine::ConversationSession conversation(std::move(program), state);
    conversation.start();

    // The scene layer under the dialogue is whatever the pre-conversation
    // segment committed.
    refresh_background(options, session.view);
    session.display.compose(session.view, false);
    DialogUi ui;
    ui.base = session.display.surface.pixels;
    ui.font = session.display.dialog_font ? &*session.display.dialog_font : nullptr;
    if (ui.font != nullptr) ui.line_height = std::max(8, ui.font->line_height);

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
    const auto wrap = [&ui](const std::string& text) {
        if (ui.font != nullptr) return wrap_dialog_text(*ui.font, text, 520);
        std::vector<std::string> lines{text};
        return lines;
    };
    // 0x436b01 calls 0x424780(selected answer) before 0x43340a(entry list): the
    // player's line enters the same scrollback as the NPC lines.
    const auto show_answer_line = [&](const byzantine::ConversationAnswer& answer) {
        if (!answer.show_text) return;
        const auto lines = wrap(answer.text);
        for (const auto& line : lines) {
            ui.scrollback.push_back(line);
            while (ui.scrollback.size() > 3) ui.scrollback.erase(ui.scrollback.begin());
        }
        ui.draw(session, false);
        session.display.present(session.view, false);
        if (session.interactive && !wait_dialog_ms(
                session, static_cast<std::uint64_t>(lines.size()) * 700)) {
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
            if (verbose) {
                std::cout << "conversation_end record=" << record_index
                          << " name=" << record.name << " result=" << event.result
                          << " lines=" << outcome.lines
                          << " answer_lists=" << outcome.answer_lists
                          << " media=" << outcome.media
                          << " audio_failures=" << outcome.audio_failures
                          << " instructions=" << conversation.executed_instructions() << '\n';
            }
            return outcome;
        }
        if (event.kind == byzantine::ConversationEventKind::line) {
            ++outcome.lines;
            const auto wrapped = wrap(event.line.text);
            if (event.line.show_text) {
                for (const auto& line : wrapped) {
                    ui.scrollback.push_back(line);
                    while (ui.scrollback.size() > 3) ui.scrollback.erase(ui.scrollback.begin());
                }
            }
            ui.draw(session, false);
            session.display.present(session.view, false);
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
            if (event.line.media_allowed && options.dialog.empty()) {
                std::cout << "conversation_media clip=" << event.line.clip
                          << " not configured (--dialog); text stays\n";
            } else if (event.line.media_allowed) {
                const auto conversation_dir =
                    find_directory_child(options.dialog, record.name);
                std::optional<std::filesystem::path> avi;
                if (conversation_dir) {
                    avi = find_child(*conversation_dir, event.line.clip);
                }
                if (!avi) {
                    const auto avi_root = options.dialog.parent_path() / "avi";
                    avi = find_child(avi_root, event.line.clip);
                }
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
                        std::optional<WavClip> sound;
                        if (!decoded->audio) {
                            std::cout << "conversation_audio clip=" << event.line.clip
                                      << " no audio stream in the AVI\n";
                        } else if (!options.audio) {
                            ++outcome.audio_failures;
                            std::cout << "conversation_audio clip=" << event.line.clip
                                      << " ERROR not played: --no-audio\n";
                        } else if (!session.audio.ready()) {
                            ++outcome.audio_failures;
                            std::cout << "conversation_audio clip=" << event.line.clip
                                      << " ERROR not played: the SDL2 audio device is not "
                                         "open (see the startup error)\n";
                        } else {
                            try {
                                sound = parse_wav(read_file(*decoded->audio),
                                                  decoded->audio->string());
                            } catch (const std::exception& error) {
                                ++outcome.audio_failures;
                                std::cout << "conversation_audio clip=" << event.line.clip
                                          << " ERROR WAV: " << error.what() << '\n';
                            }
                        }
                        if (sound) {
                            try {
                                session.audio.queue(*sound);
                                std::cout << "conversation_audio clip=" << event.line.clip
                                          << " wav=" << decoded->audio->filename().string()
                                          << " duration_ms=" << sound->duration_ms
                                          << " queued_bytes=" << session.audio.queued()
                                          << " device=" << session.audio.device_name()
                                          << '\n';
                            } catch (const std::exception& error) {
                                ++outcome.audio_failures;
                                std::cout << "conversation_audio clip=" << event.line.clip
                                          << " ERROR queue: " << error.what() << '\n';
                            }
                        }
                        const auto frame_time = std::chrono::microseconds(
                            1000000 / std::max(1, decoded->fps));
                        auto next_frame = std::chrono::steady_clock::now();
                        for (const auto& frame_file : decoded->frames) {
                            if (!session.running) break;
                            byzantine::JpegImage image;
                            try {
                                image = byzantine::decode_jpeg_file(frame_file.string());
                            } catch (const std::exception& error) {
                                std::cout << "conversation_media clip=" << event.line.clip
                                          << " frame_error=" << error.what() << '\n';
                                break;
                            }
                            const auto pixels = byzantine::rgb_to_rgb565(image);
                            ui.restore(session);
                            bool placed = false;
                            if (fullscreen_video) {
                                placed = byzantine::blit_image_2x(
                                    session.display.surface, image.width, image.height,
                                    pixels.data());
                                if (!placed) {
                                    std::cout << "conversation_video clip="
                                              << event.line.clip << " size="
                                              << image.width << 'x' << image.height
                                              << " unsupported: 0x43a248 mode 2 requires "
                                                 "2x the 640x400 playfield\n";
                                }
                            } else {
                                // Mode 0 destination is (160, 120) in the 640x480
                                // surface; the port surface starts at row 0x14.
                                placed = byzantine::blit_image(
                                             session.display.surface, image.width,
                                             image.height, pixels.data(),
                                             (kSceneWidth - image.width) / 2,
                                             (kSceneHeight - image.height) / 2)
                                             .placed;
                            }
                            if (!placed) break;
                            ui.draw_text_overlay(session);
                            session.display.present(session.view, false);
                            if (!options.conversation_video_shot.empty() &&
                                !video_shot_written) {
                                if (byzantine::write_ppm(
                                        session.display.surface,
                                        options.conversation_video_shot)) {
                                    std::cout << "conversation_video_shot "
                                              << options.conversation_video_shot
                                              << " clip=" << event.line.clip << '\n';
                                }
                                video_shot_written = true;
                            }
                            ++session.wait.frames;
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
                            while (SDL_PollEvent(&event_poll)) {
                                ++session.wait.events;
                                if (event_poll.type == SDL_QUIT) session.running = false;
                                if (event_poll.type == SDL_KEYDOWN &&
                                    event_poll.key.keysym.sym == SDLK_ESCAPE) {
                                    session.running = false;
                                }
                                if (event_poll.type == SDL_KEYDOWN &&
                                    event_poll.key.keysym.sym == SDLK_h) {
                                    session.options.overlay = !session.options.overlay;
                                }
                            }
                        }
                        if (sound && session.running) {
                            while (session.audio.queued() != 0 && session.running) {
                                pump_window(session);
                                SDL_Delay(4);
                            }
                        }
                        media_played = true;
                        ++outcome.media;
                    }
                }
            }
            if (!media_played && event.line.show_text && session.interactive) {
                // 0x424981's reveal: roughly one second per visual line.
                if (!wait_dialog_ms(session,
                                    static_cast<std::uint64_t>(wrapped.size()) * 700))
                    break;
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
            if (!options.conversation_shot.empty() && !answer_shot_written) {
                std::vector<SDL_Rect> shot_rects;
                draw_answer_panel(ui, session, event.answers, {}, &shot_rects);
                session.display.present(session.view, false);
                if (byzantine::write_ppm(session.display.surface,
                                         options.conversation_shot)) {
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
            std::vector<SDL_Rect> rects;
            bool picked = false;
            while (session.running && !picked) {
                draw_answer_panel(ui, session, event.answers, {}, &rects);
                session.display.present(session.view, false);
                if (!options.conversation_shot.empty() && !answer_shot_written) {
                    if (byzantine::write_ppm(session.display.surface,
                                             options.conversation_shot)) {
                        std::cout << "conversation_shot " << options.conversation_shot
                                  << " group=" << event.group << '\n';
                    }
                    answer_shot_written = true;
                }
                if (options.selftest_events && !rects.empty()) {
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
                while (SDL_PollEvent(&event_poll)) {
                    ++session.wait.events;
                    if (event_poll.type == SDL_QUIT) {
                        session.running = false;
                    } else if (event_poll.type == SDL_KEYDOWN &&
                               event_poll.key.keysym.sym == SDLK_ESCAPE) {
                        session.running = false;
                    } else if (event_poll.type == SDL_KEYDOWN &&
                               event_poll.key.keysym.sym == SDLK_h) {
                        session.options.overlay = !session.options.overlay;
                    } else if (event_poll.type == SDL_MOUSEBUTTONUP &&
                               event_poll.button.button == SDL_BUTTON_LEFT) {
                        float logical_x = 0;
                        float logical_y = 0;
                        SDL_RenderWindowToLogical(session.display.renderer,
                                                  event_poll.button.x, event_poll.button.y,
                                                  &logical_x, &logical_y);
                        bool in_answer = false;
                        for (std::size_t index = 0; index < rects.size(); ++index) {
                            const auto& rect = rects[index];
                            if (logical_x >= rect.x && logical_x <= rect.x + rect.w &&
                                logical_y >= rect.y && logical_y <= rect.y + rect.h) {
                                if (verbose) {
                                    std::cout << "conversation_pick record=" << record_index
                                              << " choice=" << index << '\n';
                                }
                                show_answer_line(event.answers[index]);
                                try {
                                    conversation.choose(index);
                                } catch (const std::exception& error) {
                                    std::cout << "conversation record=" << record_index
                                              << " answer_error=" << error.what() << '\n';
                                    return outcome;
                                }
                                in_answer = true;
                                picked = true;
                                break;
                            }
                        }
                        if (in_answer) break;
                        // 0x436b01 also returns a scene-object click to 0x70.
                        const auto hit = hotspot_at(session.view, static_cast<int>(logical_x),
                                                    static_cast<int>(logical_y));
                        if (hit.slot) {
                            if (verbose) {
                                std::cout << "conversation_object record=" << record_index
                                          << " slot=" << *hit.slot << '\n';
                            }
                            conversation.click_object(*hit.slot);
                            picked = true;
                            break;
                        }
                    }
                }
                SDL_Delay(16);
            }
            if (!picked) return outcome;
            continue;
        }
        return outcome;
    }
    return outcome;
}

// Runs a click plan end to end. For a 0x70 plan the pre-conversation segment is
// played first (its sounds included), then the real conversation runs on the
// state at the pause, and the script resumes at the instruction the result
// selects. `run` is the caller's segment runner (audio-gated interactive or one
// unbudgeted pass in --shot).
bool execute_plan(Session& session, const ClickPlan& plan,
                  const std::function<bool(EntryPoint, const byzantine::SceneState&,
                                           const byzantine::SceneTrace&)>& run,
                  bool verbose) {
    if (!plan.conversation) {
        return run(plan.point, plan.entry, plan.probe);
    }
    const auto request = *plan.probe.conversation_request;
    if (!run(plan.point, plan.entry_before, plan.probe)) return false;
    apply_panorama_events(session.view, session.options, verbose);
    refresh_background(session.options, session.view);
    session.display.compose(session.view, false);
    session.display.present(session.view, false);
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
    if (!outcome.ok) {
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
        std::cout << "conversation_state locals=" << before_conversation.variables.size()
                  << " changed_locals=" << changed
                  << " objects=" << before_conversation.objects.size() << '/'
                  << session.view.state.objects.size()
                  << " hotspots=" << before_conversation.hotspot_enabled.size() << '/'
                  << session.view.state.hotspot_enabled.size()
                  << " records=" << before_conversation.records.size() << '/'
                  << session.view.state.records.size() << '\n';
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

}  // namespace

int main(int argc, char** argv) {
    try {
        Session session;
        session.options = parse_options(argc, argv);
        auto& options = session.options;
        const auto label_value = options.label.empty()
                                     ? 0
                                     : std::stoul(options.label, nullptr, 0);
        if (label_value > 0xffff) throw std::runtime_error("label outside 16-bit range");

        session.view = open_scene(options.scene, options.cc_path, options.cb_path,
                                  static_cast<std::uint16_t>(label_value));
        session.view.pool_on_arrival = session.view.state.variable_pool;
        session.metadata = byzantine::read_variable_metadata(options.scene);
        load_scene_text(session, false);
        session.display.init();
        if (!options.fonts.empty()) {
            const auto sfn = find_child(options.fonts, "klio");
            const auto gr = find_child(options.fonts, "gr_klio");
            const auto bk = find_child(options.fonts, "bk_klio");
            if (sfn && gr && bk) {
                try {
                    // 0x406cfe slots 1 and 3: the same klio metrics/atlas, fill
                    // 0x1b<<5 (examine) and (0xb<<11)|(0xe<<6)|2 (message).
                    session.display.examine_font = byzantine::load_sfn_font(
                        sfn->string(), gr->string(), bk->string(), 0x0360);
                    session.display.message_font = byzantine::load_sfn_font(
                        sfn->string(), gr->string(), bk->string(), 0x5b82);
                    std::cout << "fonts klio glyphs="
                              << session.display.examine_font->glyph_count
                              << " atlas=" << session.display.examine_font->atlas_width
                              << 'x' << session.display.examine_font->atlas_height
                              << " line=" << session.display.examine_font->line_height
                              << '\n';
                } catch (const std::exception& error) {
                    std::cout << "fonts disabled: " << error.what() << '\n';
                }
            } else {
                std::cout << "fonts disabled: klio.SFN/GR_KLIO.TGA/BK_KLIO.TGA not under "
                          << options.fonts.string() << '\n';
            }
            const auto dlg = find_child(options.fonts, "dialog");
            const auto dlg_gr = find_child(options.fonts, "gr_dlg");
            const auto dlg_bk = find_child(options.fonts, "bk_dlg");
            if (dlg && dlg_gr && dlg_bk) {
                try {
                    // 0x406cfe slot 0: DIALOG.SFN + GR_DLG/BK_DLG, fill
                    // (0x16 << green) | (0x1c << red) = 0xe2c0 in RGB565.
                    session.display.dialog_font = byzantine::load_sfn_font(
                        dlg->string(), dlg_gr->string(), dlg_bk->string(), 0xe2c0);
                    std::cout << "fonts dialog glyphs="
                              << session.display.dialog_font->glyph_count
                              << " atlas=" << session.display.dialog_font->atlas_width
                              << 'x' << session.display.dialog_font->atlas_height
                              << " line=" << session.display.dialog_font->line_height
                              << '\n';
                } catch (const std::exception& error) {
                    std::cout << "dialog font disabled: " << error.what() << '\n';
                }
            } else {
                std::cout << "dialog font disabled: DIALOG.SFN/GR_DLG.TGA/BK_DLG.TGA "
                             "not under "
                          << options.fonts.string() << '\n';
            }
        } else {
            std::cout << "fonts disabled: no --fonts (the text windows stay blank)\n";
        }
        if (options.audio) {
            try {
                session.audio.open();
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
                          << " id=" << event.id;
                if (session.view.state.sound_table_known &&
                    event.id < session.view.state.sound_records.size()) {
                    std::cout << " name=" << session.view.state.sound_records[event.id].name;
                }
                std::cout << " allowed="
                          << (event.allowed ? (*event.allowed ? "true" : "false") : "unknown")
                          << '\n';
            }
        }

        if (options.shot.empty()) {
            // Interactive: show the scene, then run the label (gated when audio
            // is available) and keep the window alive throughout.
            refresh_background(options, session.view);
            session.display.compose(session.view);
            session.display.present(session.view, options.overlay);
            const auto run = [&](EntryPoint point, const byzantine::SceneState& entry,
                                 const byzantine::SceneTrace& look) {
                if (!audio_ready) {
                    session.view.state = entry;
                    set_trace(session.view, trace_point(session.view, point, entry, 0));
                    return session.running;
                }
                return run_label_gated(session, point, entry, look, true);
            };
            session.interactive = true;
            const EntryPoint startup_point{session.view.label, options.entry, std::nullopt};
            bool startup_ok = false;
            if (probe.conversation_request) {
                ClickPlan startup;
                startup.ok = true;
                startup.conversation = true;
                startup.point = startup_point;
                startup.entry = probe_state;
                startup.entry_before = session.view.entry_state;
                startup.probe = probe;
                startup_ok = execute_plan(session, startup, run, true);
            } else {
                startup_ok = run(startup_point, session.view.entry_state, probe);
            }
            // The label's node loads/rotations belong to the renderer, so they
            // are applied exactly once, from the committed trace.
            apply_panorama_events(session.view, options, true);
            show_message_text(session, true);
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
                } else if (!execute_plan(session, plan, run, true)) {
                    session.view.state = plan.entry_before;
                    set_trace(session.view, before_trace);
                    std::cout << "run_label " << plan.action
                              << " not committed\n";
                } else {
                    std::cout << "run_label " << plan.action << " click_result "
                              << describe_trace(session.view.trace, session.view.state)
                              << '\n';
                    show_message_text(session, true);
                    apply_panorama_events(session.view, options, true);
                    refresh_background(options, session.view);
                    session.display.compose(session.view);
                    session.display.present(session.view, options.overlay);
                    follow_switches(session, true);
                }
                if (options.selftest_events) push_quit();
            }
            refresh_background(options, session.view);
            session.display.compose(session.view);
            session.display.present(session.view, options.overlay);
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
                if (!plan.ok) return;
                const auto before_state = session.view.state;
                const auto before_trace = session.view.trace;
                if (!execute_plan(session, plan, run, true)) {
                    // A conversation that stopped before 0x432d6e returned is
                    // not a completed action: discard its partial variable
                    // writes instead of leaving them in the scene.
                    session.view.state = before_state;
                    set_trace(session.view, before_trace);
                    session.display.compose(session.view, false);
                    session.display.present(session.view, options.overlay);
                    return;
                }
                show_object_text(session, *hit.slot, true);
                show_message_text(session, true);
                apply_panorama_events(session.view, options, true);
                refresh_background(options, session.view);
                session.display.compose(session.view);
                session.display.present(session.view, options.overlay);
                std::cout << "click_result "
                          << describe_trace(session.view.trace, session.view.state)
                          << " active_slots="
                          << session.view.trace.active_hotspots.size() << '\n';
                follow_switches(session, true);
            };
            // Arrow keys turn the panorama (the original uses on-screen cursor
            // buttons; the mapping between those and a host key is ours).
            const auto turn = [&](double yaw, double pitch) {
                if (!panorama_layer(session.view)) return;
                session.view.panorama->advance(yaw, pitch, 1.0);
                session.display.compose(session.view, false);
                session.display.present(session.view, options.overlay);
                std::cout << "panorama yaw=" << session.view.panorama->yaw_degrees()
                          << " pitch=" << session.view.panorama->pitch_degrees()
                          << '\n';
            };

            // Headless verification of the click path: the same handler the
            // mouse uses, run before the event loop.
            if (options.click) {
                const auto plan = plan_hotspot(session.view, *options.click);
                std::cout << "click " << plan.message
                          << " handled=" << (plan.ok ? "true" : "false") << '\n';
                const auto before_state = session.view.state;
                const auto before_trace = session.view.trace;
                if (plan.ok && execute_plan(session, plan, run, true)) {
                    show_object_text(session, static_cast<std::uint16_t>(*options.click), true);
                    show_message_text(session, true);
                    apply_panorama_events(session.view, options, true);
                    refresh_background(options, session.view);
                    session.display.compose(session.view);
                    session.display.present(session.view, options.overlay);
                    std::cout << "click_result "
                              << describe_trace(session.view.trace, session.view.state)
                              << " active_slots="
                              << session.view.trace.active_hotspots.size() << '\n';
                    follow_switches(session, true);
                } else if (plan.ok) {
                    session.view.state = before_state;
                    set_trace(session.view, before_trace);
                    session.display.compose(session.view, false);
                    session.display.present(session.view, options.overlay);
                    std::cout << "click not committed\n";
                }
                if (options.selftest_events) push_quit();
            }
            for (const auto& point : options.click_ats) {
                handle_click(point.first, point.second);
                if (options.selftest_events) push_quit();
            }

            // Left-drag turns a panorama; a press that does not move is a click.
            bool dragging = false;
            bool dragged = false;
            int drag_x = 0;
            while (session.running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT ||
                        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                        session.running = false;
                    } else if (event.type == SDL_KEYDOWN &&
                               event.key.keysym.sym == SDLK_h) {
                        options.overlay = !options.overlay;
                        session.display.present(session.view, options.overlay);
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
                    } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                               event.button.button == SDL_BUTTON_LEFT) {
                        float logical_x = 0;
                        float logical_y = 0;
                        SDL_RenderWindowToLogical(session.display.renderer, event.button.x,
                                                  event.button.y, &logical_x, &logical_y);
                        if (panorama_layer(session.view)) {
                            dragging = true;
                            dragged = false;
                            drag_x = static_cast<int>(logical_x);
                        } else {
                            handle_click(static_cast<int>(logical_x),
                                         static_cast<int>(logical_y));
                        }
                    } else if (event.type == SDL_MOUSEMOTION && dragging) {
                        float logical_x = 0;
                        float logical_y = 0;
                        SDL_RenderWindowToLogical(session.display.renderer, event.motion.x,
                                                  event.motion.y, &logical_x, &logical_y);
                        const int delta = static_cast<int>(logical_x) - drag_x;
                        if (delta != 0) {
                            // One source column per screen pixel, the same
                            // degrees-per-column that 0x402285 uses.
                            const double degrees =
                                static_cast<double>(delta) / session.view.panorama->source_width() *
                                360.0;
                            turn(degrees, 0);
                            drag_x = static_cast<int>(logical_x);
                            dragged = true;
                        }
                    } else if (event.type == SDL_MOUSEBUTTONUP &&
                               event.button.button == SDL_BUTTON_LEFT && dragging) {
                        dragging = false;
                        float logical_x = 0;
                        float logical_y = 0;
                        SDL_RenderWindowToLogical(session.display.renderer, event.button.x,
                                                  event.button.y, &logical_x, &logical_y);
                        if (!dragged) {
                            handle_click(static_cast<int>(logical_x),
                                         static_cast<int>(logical_y));
                        } else {
                            std::cout << "click suppressed: the drag turned the view\n";
                        }
                    }
                }
                SDL_Delay(16);
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
            return true;
        };
        if (probe.conversation_request) {
            ClickPlan startup;
            startup.ok = true;
            startup.conversation = true;
            startup.point = EntryPoint{session.view.label, options.entry, std::nullopt};
            startup.entry = probe_state;
            startup.entry_before = session.view.entry_state;
            startup.probe = probe;
            if (!execute_plan(session, startup, run_shot, true)) {
                session.view.state = startup.entry;
                set_trace(session.view, probe);
                std::cout << "startup_not_committed\n";
            }
        } else {
            session.view.state = session.view.entry_state;
            set_trace(session.view, probe);
            session.view.state = std::move(probe_state);
        }
        apply_panorama_events(session.view, options, true);
        show_message_text(session, true);
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
            } else if (!execute_plan(session, plan, run_shot, true)) {
                session.view.state = plan.entry_before;
                set_trace(session.view, before_trace);
                std::cout << "run_label " << plan.action << " not committed\n";
            } else {
                std::cout << "run_label " << plan.action << " click_result "
                          << describe_trace(session.view.trace, session.view.state) << '\n';
                show_message_text(session, true);
                apply_panorama_events(session.view, options, true);
                follow_switches(session, false);
            }
        }
        if (options.click) {
            const auto plan = plan_hotspot(session.view, *options.click);
            std::cout << "click " << plan.message
                      << " handled=" << (plan.ok ? "true" : "false") << '\n';
            if (plan.ok) {
                const auto before_trace = session.view.trace;
                if (!execute_plan(session, plan, run_shot, true)) {
                    session.view.state = plan.entry_before;
                    set_trace(session.view, before_trace);
                    std::cout << "click_not_committed\n";
                } else {
                    std::cout << "click_result "
                              << describe_trace(session.view.trace, session.view.state) << '\n';
                    show_object_text(session, static_cast<std::uint16_t>(*options.click), true);
                    show_message_text(session, true);
                    apply_panorama_events(session.view, options, true);
                    follow_switches(session, true);
                }
            }
        }
        for (const auto& point : options.click_ats) {
            const auto hit = hotspot_at(session.view, point.first, point.second);
            std::cout << "click_at " << point.first << ',' << point.second << " -> "
                      << hit.test_x << ',' << hit.test_y;
            if (!hit.slot) {
                std::cout << " hit no hotspot\n";
                continue;
            }
            const auto plan = plan_hotspot(session.view, *hit.slot);
            std::cout << ' ' << plan.message;
            if (!plan.ok) {
                std::cout << '\n';
                continue;
            }
            const auto before_trace = session.view.trace;
            if (!execute_plan(session, plan, run_shot, true)) {
                session.view.state = plan.entry_before;
                set_trace(session.view, before_trace);
                std::cout << " click_not_committed\n";
                continue;
            }
            std::cout << " click_result "
                      << describe_trace(session.view.trace, session.view.state) << '\n';
            show_object_text(session, *hit.slot, true);
            show_message_text(session, true);
            apply_panorama_events(session.view, options, false);
            follow_switches(session, false);
        }
        refresh_background(options, session.view);
        session.display.compose(session.view);
        session.display.present(session.view, options.overlay);
        session.display.write_shot(options.shot);
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
