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
#include "byzantine/inventory.hpp"
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
// The original scene surface is 640x480: the playfield is rows 0x14..0x413
// and the dialogue band is rows 0x1a4..0x1df. The port's composed scene stays
// 640x400; the conversation presents it at kScreenPlayfieldTop inside this
// 640x480 screen, exactly like 0x40d3af's y=0x14 and 0x424981's y=0x1a4.
constexpr int kScreenWidth = 640;
constexpr int kScreenHeight = 480;
constexpr int kScreenPlayfieldTop = byzantine::kPlayfieldTop;
constexpr int kScreenBandTop = 0x1a4;
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
    std::filesystem::path sounds;              // sounds/ root
    std::filesystem::path fonts;               // the DIALOG/klio font files
    std::filesystem::path ccb;                 // split CCB.ALL (engine/<name>.ccb)
    std::filesystem::path ccf;                 // split CCF.ALL (engine/<name>.ccf)
    std::filesystem::path byz;                 // split Byz.all (engine/<name>.byz)
    std::filesystem::path messages;            // copy2hd/setupart/MESSAGES.TXT
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
    std::optional<int> bar_select;             // --bar-select <item id>
    bool bar_use = false;                      // --bar-use: examine selected item
    int bar_scroll = 0;                        // --bar-next/--bar-prev count
    bool overlay = true;
    bool print_audio = false;
    bool audio = true;
    bool video = true;                         // --no-video disables FFmpeg decodes
    bool selftest_events = false;              // --selftest-events (see README)
    bool print_hotspots = false;               // --print-hotspots

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
    "--ca", "--anims", "--gameprefs", "--inventor", "--hotitems", "--cs", "--ce",
    "--backgrounds", "--sounds", "--shot", "--click", "--click-at", "--staged-x",
    "--fonts", "--cd", "--ccb", "--ccf", "--byz", "--messages", "--dialog",
    "--dialog-cache",
    "--run-label", "--conversation-choices", "--conversation-shot",
    "--conversation-object", "--conversation-video-shot", "--conversation-cancel",
    "--bar-select"};

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
        if (token == "--print-hotspots") {
            options.print_hotspots = true;
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
            if (token == "--backgrounds") options.backgrounds = value;
            else if (token == "--sounds") options.sounds = value;
            else if (token == "--fonts") options.fonts = value;
            else if (token == "--ccb") options.ccb = value;
            else if (token == "--ccf") options.ccf = value;
            else if (token == "--byz") options.byz = value;
            else if (token == "--messages") options.messages = value;
            else if (token == "--dialog") options.dialog = value;
            else if (token == "--dialog-cache") options.dialog_cache = value;
            else if (token == "--run-label") options.run_label = std::stoi(value);
            else if (token == "--bar-select") options.bar_select = std::stoi(value);
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
            else if (token == "--conversation-cancel") {
                options.conversation_cancel = std::stoi(value);
                if (options.conversation_cancel < 0) {
                    throw std::runtime_error("--conversation-cancel must be >= 0");
                }
            }
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
            "[--cd <file> --ccb --ccf --byz --messages <file> --dialog --dialog-cache <dir>] "
            "[--run-label <n>] [--conversation-choices <i,...>] "
            "[--conversation-object <slot>] [--conversation-cancel <n>] "
            "[--conversation-shot <ppm>] "
            "[--conversation-video-shot <ppm>] [--bar-select <id>] [--bar-use] "
            "[--bar-next] [--bar-prev] [--no-video] "
            "[--no-hotspots] [--no-audio] [--print-audio] [--print-hotspots] "
            "[--selftest-events]");
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

// 0x40860e/0x41045b: selected item -> inventor\examine\<first 8 chars, first
// replaced by 'e'>.jpg. The item name is not trimmed at eight characters
// anywhere else, so this stays separate from the raw LCI record.
std::filesystem::path examine_background_path(const Options& options,
                                              const byzantine::SceneState& state) {
    if (!state.current_use_item || options.backgrounds.empty()) return {};
    const auto name = byzantine::inventory_name(state, *state.current_use_item);
    if (name.empty()) return {};
    const auto stem = byzantine::examine_image_stem(name);
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
    if (view.trace.examine_image) {
        const auto name = byzantine::inventory_name(view.state, *view.trace.examine_image);
        const auto stem = byzantine::examine_image_stem(name);
        view.background_name = "examine/" + stem;
        view.background_file = examine_background_path(options, view.state);
        if (view.background_file.empty() &&
            view.missing_background_logged != view.background_name) {
            std::cout << "examine_image_missing scene=" << view.name
                      << " item=" << *view.trace.examine_image
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
    // draws the answer list with slot 0.
    std::optional<byzantine::SfnFont> dialog_font;
    // Font slot 4 (fonts/CAPTIONS.SFN + CAPTIONS.TGA, fill 0x03eb). 0x424981
    // draws the conversation subtitle with this slot at (55, 420).
    std::optional<byzantine::SfnFont> caption_font;
    // The original scene surface is 640x480: the playfield starts at row 0x14
    // and the inventory strip occupies rows 0x1a4..0x1df. `surface` stays the
    // 640x400 playfield and `screen` is the composed 640x480 frame.
    byzantine::Rgb16Surface screen;
    const Options* ui_options = nullptr;
    std::optional<UiImage> bottom_bar;
    std::map<std::string, UiImage> item_icons;
    std::set<std::string> missing_ui;
    int logical_width = kScreenWidth;
    int logical_height = kScreenHeight;

    void init(const Options& options) {
        ui_options = &options;
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::runtime_error(std::string("SDL video init failed: ") + SDL_GetError());
        }
        // 640x480 at 2x: the original screen size, including the inventory
        // strip. The conversation already uses the same geometry.
        window = SDL_CreateWindow("Byzantine: The Betrayal - scene player",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  kScreenWidth * 2, kScreenHeight * 2,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == nullptr) throw std::runtime_error(SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr) throw std::runtime_error(SDL_GetError());
        SDL_RenderSetLogicalSize(renderer, kScreenWidth, kScreenHeight);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                    SDL_TEXTUREACCESS_STREAMING, kScreenWidth,
                                    kScreenHeight);
        if (texture == nullptr) throw std::runtime_error(SDL_GetError());
        surface = byzantine::make_surface(kSceneWidth, kSceneHeight, 0);
        screen = byzantine::make_surface(kScreenWidth, kScreenHeight, 0);

        auto root = options.backgrounds;
        if (std::filesystem::is_directory(root / "inventor")) root /= "inventor";
        const auto strip = find_child(root, "instrip");
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

    const UiImage* item_icon(const std::string& name) {
        const auto key = lower_ascii(name);
        const auto cached = item_icons.find(key);
        if (cached != item_icons.end()) return &cached->second;
        if (ui_options == nullptr || ui_options->backgrounds.empty()) return nullptr;
        auto root = ui_options->backgrounds;
        if (std::filesystem::is_directory(root / "inventor")) root /= "inventor";
        const auto found = find_child(root, name);
        if (!found) {
            if (missing_ui.insert(key).second) {
                std::cout << "ui_asset_missing name=inventor/" << name
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
        if (!view.state.dialogue_visible) return;

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
    }

    void present_image(const byzantine::Rgb16Surface& image, const SceneView& view,
                       bool overlay, int hotspot_offset_y) {
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
        if (overlay && !panorama_layer(view)) {
            for (const auto& hotspot : view.trace.active_hotspots) {
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
    }

    void present(const SceneView& view, bool overlay) {
        build_screen(view);
        present_image(screen, view, overlay, kScreenPlayfieldTop);
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
    // Set by the interactive branch: a scene entry may then wait on audio like
    // any other script. --shot stays deterministic and never waits.
    bool interactive = false;
    bool running = true;
    WaitStats wait;
    // While a conversation is running its 640x480 screen is what the window
    // shows, including during audio waits. run_conversation sets and clears it.
    const byzantine::Rgb16Surface* conversation_frame = nullptr;
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
            // During a conversation ESC is 0x436b01's answer-list cancel. The
            // answer loop owns that transition; media/text waits must not quit
            // the whole player.
            if (session.conversation_frame != nullptr) continue;
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
    if (session.conversation_frame != nullptr) {
        session.display.present_image(*session.conversation_frame, session.view,
                                      session.options.overlay, kScreenPlayfieldTop);
    } else {
        session.display.present(session.view, session.options.overlay);
    }
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

// 0x41d45a reads setupart/MESSAGES.TXT line `id` (0-based) into the dialog
// text. The file is CP1254; the font drawer uses the game byte encoding.
void load_messages(Session& session, bool verbose) {
    session.messages.clear();
    if (session.options.messages.empty()) return;
    try {
        const auto bytes = read_file(session.options.messages);
        std::string line;
        for (const auto byte : bytes) {
            if (byte == '\n') {
                if (!line.empty() && line.back() == '\r') line.pop_back();
                session.messages.push_back(byzantine::cp1254_to_game_text(line));
                line.clear();
            } else {
                line.push_back(static_cast<char>(byte));
            }
        }
        if (!line.empty()) {
            if (line.back() == '\r') line.pop_back();
            session.messages.push_back(byzantine::cp1254_to_game_text(line));
        }
        if (verbose) {
            std::cout << "messages file=" << session.options.messages
                      << " lines=" << session.messages.size() << '\n';
        }
    } catch (const std::exception& error) {
        std::cout << "messages " << session.options.messages << ": " << error.what()
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

bool travel_to(Session& session, std::uint16_t index, bool verbose) {
    auto& options = session.options;
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
        carry_inventory(current, session.view.state);
        session.view.state.random_state = current.random_state;
        refresh_background(options, session.view);
        session.display.compose(session.view, verbose);
        session.display.present(session.view, options.overlay);
        std::cout << "closeup_return scene=" << session.view.name << " from=" << from
                  << " requested_index=" << index
                  << " hotspots=" << session.view.trace.active_hotspots.size()
                  << " bar=" << (session.view.state.dialogue_visible ? "visible" : "hidden")
                  << '\n';
        return session.running;
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
    carry_inventory(session.view.state, setup.state);
    // 0x40fd7b: the scene load pre-loads the CA type 2/3 records before the
    // script runs (the save path is not used here, so nothing replaces them).
    const auto preload = byzantine::preload_scene_records(setup.state);
    next_view.entry_state = setup.state;
    // DAT_00462ef8 is process-global and is not reset by a scene load.
    next_view.entry_state.random_state = session.view.state.random_state;
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
    if (preload.created != 0 || preload.failed != 0) {
        std::cout << " record_preload=" << preload.created;
        if (preload.failed != 0) std::cout << " record_preload_failed=" << preload.failed;
    }
    if (!missing.empty()) std::cout << " missing_tables=" << missing;
    std::cout << '\n';

    session.view = std::move(next_view);
    session.reload_available = false;
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

// 0x43c8d2 case 2 + 0x408076 + 0x438c80: the examine button on the inventory
// strip opens the matching close-up scene and snapshots the parent. The scene
// mapping is the original data table (0x438afa/0x438173), not a per-scene port
// branch. A click on the close-up's exit runs 0x73 and travel_to restores the
// snapshot instead of loading the requested index.
bool enter_examine(Session& session, bool verbose) {
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
    if (lower_ascii(session.view.name) == *wanted) {
        std::cout << "bar_examine skipped: scene=" << session.view.name
                  << " is already the close-up\n";
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
    const auto use = session.view.state.current_use_item;
    SceneView parent = session.view;
    if (!travel_to(session, static_cast<std::uint16_t>(target_index), verbose)) {
        return false;
    }
    session.return_view = std::move(parent);
    session.return_armed = true;
    std::cout << "closeup_enter scene=" << session.view.name << " parent=" << parent_name
              << " item=" << (use ? std::to_string(*use) : "none")
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
        std::cout << "ui_click top_bar x=" << x << " y=" << y
                  << " consumed (top menu is not modelled)\n";
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
            session.display.present(session.view, session.options.overlay);
            return true;
        }
    }
    if (x >= 2 && x <= 0x3a && y >= 0x1a8 && y <= 0x1dc) {
        if (!state.current_dialogue_record) {
            std::cout << "bar_examine skipped: no selected item\n";
            return true;
        }
        const auto id = *state.current_dialogue_record;
        if (!byzantine::inventory_mark_using(state, id)) {
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
        session.display.present(session.view, session.options.overlay);
        return true;
    }
    if (x >= 0x25d && x <= 0x27b && y >= 0x1b1 && y <= 0x1cb) {
        byzantine::inventory_scroll(state, -1);
        std::cout << "bar_scroll direction=prev scroll=" << state.dialogue_scroll
                  << " page=" << state.current_dialogue_channel << '\n';
        session.display.present(session.view, session.options.overlay);
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
    session.display.present(session.view, options.overlay);
    if (!options.bar_use) return;
    if (!session.view.state.current_dialogue_record) {
        std::cout << "bar_use skipped: no selected item\n";
        return;
    }
    const auto id = *session.view.state.current_dialogue_record;
    if (!byzantine::inventory_mark_using(session.view.state, id)) {
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
            if (verbose) {
                std::cout << "conversation_media clip=" << clip << " cached=yes source="
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

    void present(Session& session) {
        compose(session);
        session.display.present_image(screen, session.view, session.options.overlay,
                                      kScreenPlayfieldTop);
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
            // 0x424780 only replaces the subtitle for a line with text; a
            // "null"/no-text line leaves the previous line visible.
            if (event.line.show_text) {
                ui.set_caption(event.line.text);
                if (verbose) {
                    std::cout << "conversation_caption source=npc index="
                              << event.line.index
                              << " visual_lines=" << ui.caption.size() << '\n';
                }
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
                        // 0x40d5ff clears the 640x480 surface when gamepref[1]
                        // is 0 (the 320x200 mode-0 window); mode 2's 640x400
                        // frame covers the playfield by itself.
                        if (!fullscreen_video) ui.clear_playfield(session);
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
                            ui.present(session);
                            if (!options.conversation_video_shot.empty() &&
                                !video_shot_written) {
                                if (ui.write(session,
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
                                // 0x413bdd's video loop has no ESC branch; ESC
                                // is 0x436b01's answer-list cancel and must not
                                // terminate the player here.
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
                        // The last frame stays on the surface, exactly like the
                        // original's DirectDraw target; 0x436b01 draws the
                        // answers over it.
                        ui.snapshot_playfield(session);
                        media_played = true;
                        ++outcome.media;
                    }
                }
            }
            if (!media_played && event.line.show_text && session.interactive) {
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
                while (SDL_PollEvent(&event_poll)) {
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
                               event_poll.key.keysym.sym == SDLK_h) {
                        session.options.overlay = !session.options.overlay;
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
    SceneView fresh;
    fresh.name = session.view.name;
    fresh.cc_path = session.view.cc_path;
    fresh.cb_path = session.view.cb_path;
    fresh.script = std::move(session.view.script);
    fresh.entries = std::move(session.view.entries);
    fresh.entry_state = session.view.entry_state;
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
    if (!run(point, session.view.entry_state, probe)) return false;
    apply_panorama_events(session.view, session.options, verbose);
    show_message_text(session, verbose);
    refresh_background(session.options, session.view);
    session.display.compose(session.view, verbose);
    session.display.present(session.view, session.options.overlay);
    std::cout << "scene_restart reason=" << reason << " scene=" << session.view.name
              << " point=" << (point.at_entry ? "entry" : "label")
              << " hotspots=" << session.view.trace.active_hotspots.size()
              << " steps=" << session.view.trace.steps << '\n';
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
                  bool verbose, bool* session_ended = nullptr) {
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
    // The conversation screen is local to run_conversation; stop showing it (and
    // with it the last subtitle) before the game scene returns.
    session.conversation_frame = nullptr;
    // The conversation UI may have left the last media frame in the playfield.
    // Restore the scene layer immediately so the normal 640x480 view (with the
    // inventory strip) is what the caller and the player see on return.
    refresh_background(session.options, session.view);
    session.display.compose(session.view, false);
    session.display.present(session.view, session.options.overlay);
    if (!outcome.ok) {
        refresh_background(session.options, session.view);
        session.display.compose(session.view, false);
        session.display.present(session.view, false);
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
        session.display.present(session.view, false);
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
        session.view.reload_point = SceneReloadPoint{
            static_cast<std::uint16_t>(label_value), options.entry};
        session.metadata = byzantine::read_variable_metadata(options.scene);
        load_scene_text(session, false);
        load_messages(session, true);
        session.display.init(options);
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
            const auto cap = find_filename(options.fonts, "CAPTIONS.SFN");
            const auto cap_tga = find_filename(options.fonts, "CAPTIONS.TGA");
            if (cap && cap_tga) {
                try {
                    // 0x406cfe slot 4: CAPTIONS.SFN + CAPTIONS.TGA, green
                    // 0x03eb. 0x424981 draws the conversation subtitle.
                    session.display.caption_font =
                        byzantine::load_sfn_font_single_atlas(cap->string(),
                                                              cap_tga->string(), 0x03eb);
                    std::cout << "fonts captions glyphs="
                              << session.display.caption_font->glyph_count
                              << " atlas=" << session.display.caption_font->atlas_width
                              << 'x' << session.display.caption_font->atlas_height
                              << " line=" << session.display.caption_font->line_height
                              << '\n';
                    // The shipped Turkish CAPTIONS.SFN/TGA only carries some of
                    // the Turkish glyphs: metrics 0x62..0x66 and 0x69..0x6c are
                    // 2-pixel blanks. Its ASCII glyphs are pixel-identical to
                    // DIALOG's (62 of 63), so the subtitle keeps the verified
                    // position/colour and uses DIALOG's glyph shapes rather
                    // than dropping letters.
                    const char turkish_bytes[] = {'\xa1', '\xa2', '\xa3', '\xa4',
                                                  '\xa5', '\xa6', '\xa7', '\xa8',
                                                  '\xaa', '\xab', '\xac', '\xad'};
                    std::string missing;
                    for (const char byte : turkish_bytes) {
                        const auto* glyph = session.display.caption_font->glyph(byte);
                        if (glyph == nullptr || glyph->advance <= 2) {
                            missing.push_back(byte);
                        }
                    }
                    if (!missing.empty() && session.display.dialog_font) {
                        std::cout << "fonts captions: " << missing.size()
                                  << " Turkish glyph(s) are blank in CAPTIONS.SFN";
                        for (const char byte : missing) {
                            std::cout << " 0x" << std::hex
                                      << static_cast<int>(
                                             static_cast<unsigned char>(byte))
                                      << std::dec;
                        }
                        std::cout << "; using DIALOG.SFN's identical glyph shapes for "
                                     "the green subtitle\n";
                        session.display.caption_font = session.display.dialog_font;
                    }
                } catch (const std::exception& error) {
                    std::cout << "caption font disabled: " << error.what() << '\n';
                }
            } else {
                std::cout << "caption font disabled: CAPTIONS.SFN/CAPTIONS.TGA not under "
                          << options.fonts.string()
                          << " (conversations report an error instead of guessing)\n";
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
                        session.display.present(session.view, options.overlay);
                        follow_switches(session, true);
                        if (options.print_hotspots) print_hotspot_screens(session.view);
                    }
                }
                if (options.selftest_events) push_quit();
            }
            refresh_background(options, session.view);
            session.display.compose(session.view);
            session.display.present(session.view, options.overlay);
            if (options.print_hotspots) print_hotspot_screens(session.view);
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
                bool ended = false;
                if (!execute_plan(session, plan, run, true, &ended)) {
                    // A conversation that stopped before 0x432d6e returned is
                    // not a completed action: discard its partial variable
                    // writes instead of leaving them in the scene.
                    session.view.state = before_state;
                    set_trace(session.view, before_trace);
                    session.display.compose(session.view, false);
                    session.display.present(session.view, options.overlay);
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
                if (options.print_hotspots) print_hotspot_screens(session.view);
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
                    }
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
            int drag_y = 0;
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
                    } else if (event.type == SDL_MOUSEMOTION && dragging) {
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
            session.view.state = session.view.entry_state;
            set_trace(session.view, probe);
            session.view.state = std::move(probe_state);
        }
        if (!startup_ended) {
            apply_panorama_events(session.view, options, true);
            show_message_text(session, true);
        }
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
                    std::cout << "run_label " << plan.action << " click_result "
                              << describe_trace(session.view.trace, session.view.state) << '\n';
                    show_message_text(session, true);
                    apply_panorama_events(session.view, options, true);
                    follow_switches(session, false);
                }
            }
        }
        apply_bar_options(session, options, true);
        if (options.click) {
            const auto plan = plan_hotspot(session.view, *options.click);
            std::cout << "click " << plan.message
                      << " handled=" << (plan.ok ? "true" : "false") << '\n';
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
            bool ended = false;
            if (!execute_plan(session, plan, run_shot, true, &ended)) {
                session.view.state = plan.entry_before;
                set_trace(session.view, before_trace);
                std::cout << " click_not_committed\n";
                continue;
            }
            if (ended) {
                std::cout << " click_result scene_session_ended\n";
                follow_switches(session, false);
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
        if (options.print_hotspots) print_hotspot_screens(session.view);
        session.display.write_shot(session.view, options.shot);
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
