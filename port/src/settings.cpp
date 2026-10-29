#include "byzantine/settings.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>

namespace byzantine {
namespace {

std::string trim(std::string value) {
    const auto not_space = [](unsigned char character) {
        return std::isspace(character) == 0;
    };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), not_space));
    value.erase(std::find_if(value.rbegin(), value.rend(), not_space).base(), value.end());
    return value;
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

bool parse_bool(const std::string& value, bool* result) {
    const auto text = lower(trim(value));
    if (text == "1" || text == "true" || text == "yes" || text == "on") {
        *result = true;
        return true;
    }
    if (text == "0" || text == "false" || text == "no" || text == "off") {
        *result = false;
        return true;
    }
    return false;
}

bool parse_int(const std::string& value, int* result) {
    try {
        std::size_t used = 0;
        const auto parsed = std::stoi(trim(value), &used, 0);
        if (used != trim(value).size() || !std::isfinite(parsed)) return false;
        *result = parsed;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool parse_float(const std::string& value, float* result) {
    try {
        std::size_t used = 0;
        const auto parsed = std::stof(trim(value), &used);
        if (used != trim(value).size() || !std::isfinite(parsed)) return false;
        *result = parsed;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void append_error(std::string* error, const std::string& text) {
    if (error == nullptr) return;
    if (!error->empty()) *error += "; ";
    *error += text;
}

}  // namespace

const char* window_mode_name(WindowMode mode) {
    switch (mode) {
        case WindowMode::windowed: return "windowed";
        case WindowMode::borderless: return "borderless";
        case WindowMode::exclusive: return "exclusive";
        case WindowMode::maximized: return "maximized";
    }
    return "windowed";
}

const char* scale_mode_name(ScaleMode mode) {
    switch (mode) {
        case ScaleMode::fit: return "fit";
        case ScaleMode::integer: return "integer";
    }
    return "fit";
}

bool parse_window_mode(const std::string& text, WindowMode* mode) {
    const auto value = lower(trim(text));
    if (value == "windowed" || value == "window" || value == "0") {
        *mode = WindowMode::windowed;
        return true;
    }
    if (value == "borderless" || value == "fullscreen" || value == "desktop" ||
        value == "1") {
        *mode = WindowMode::borderless;
        return true;
    }
    if (value == "exclusive" || value == "2") {
        *mode = WindowMode::exclusive;
        return true;
    }
    if (value == "maximized" || value == "maximise" || value == "maximize" ||
        value == "3") {
        *mode = WindowMode::maximized;
        return true;
    }
    return false;
}

bool parse_scale_mode(const std::string& text, ScaleMode* mode) {
    const auto value = lower(trim(text));
    if (value == "fit" || value == "aspect" || value == "letterbox" || value == "0") {
        *mode = ScaleMode::fit;
        return true;
    }
    if (value == "integer" || value == "pixel" || value == "pixel-perfect" ||
        value == "1") {
        *mode = ScaleMode::integer;
        return true;
    }
    return false;
}

Settings Settings::defaults() { return Settings{}; }

void Settings::clamp() {
    window_width = std::clamp(window_width, 640, 7680);
    window_height = std::clamp(window_height, 480, 4320);
    // Keep the default 4:3 window an even multiple of the 640x480 logical
    // screen; arbitrary user sizes are allowed, the renderer letterboxes.
    cursor_scale = std::clamp(cursor_scale, 1, 4);
    monitor = std::clamp(monitor, -1, 15);
    const auto clamp_volume = [](float value) {
        return std::clamp(value, 0.0f, 1.0f);
    };
    master_volume = clamp_volume(master_volume);
    speech_volume = clamp_volume(speech_volume);
    ambient_volume = clamp_volume(ambient_volume);
    effects_volume = clamp_volume(effects_volume);
    if (text_language.empty()) text_language = "auto";
    if (voice_language.empty()) voice_language = "auto";
    if (key_settings.empty()) key_settings = "F10";
    if (key_backlog.empty()) key_backlog = "F3";
    if (key_skip.empty()) key_skip = "F4";
    if (key_quicksave.empty()) key_quicksave = "F5";
    if (key_quickload.empty()) key_quickload = "F9";
    if (key_autoload.empty()) key_autoload = "F8";
    if (key_screenshot.empty()) key_screenshot = "F12";
    if (key_hotspots.empty()) key_hotspots = "H";
}

bool Settings::load(const std::filesystem::path& path, std::string* error) {
    std::ifstream input(path);
    if (!input) return false;
    std::string section;
    std::string line;
    int line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        line = trim(line);
        if (line.empty() || line.front() == '#' || line.front() == ';') continue;
        if (line.front() == '[' && line.back() == ']') {
            section = lower(trim(line.substr(1, line.size() - 2)));
            continue;
        }
        const auto equals = line.find('=');
        if (equals == std::string::npos) {
            append_error(error, "line " + std::to_string(line_number) +
                                    ": expected key=value");
            continue;
        }
        const auto key = lower(trim(line.substr(0, equals)));
        const auto value = trim(line.substr(equals + 1));
        const auto where = section + "." + key;
        const auto bad = [&](const char* expected) {
            append_error(error, "line " + std::to_string(line_number) + " (" +
                                    where + "): expected " + expected);
        };
        if (where == "display.mode" || where == "display.window_mode") {
            if (!parse_window_mode(value, &window_mode)) bad("windowed|borderless|exclusive|maximized");
        } else if (where == "display.scale" || where == "display.scale_mode") {
            if (!parse_scale_mode(value, &scale_mode)) bad("fit|integer");
        } else if (where == "display.vsync") {
            if (!parse_bool(value, &vsync)) bad("0|1");
        } else if (where == "display.window_width" || where == "display.width") {
            if (!parse_int(value, &window_width)) bad("integer");
        } else if (where == "display.window_height" || where == "display.height") {
            if (!parse_int(value, &window_height)) bad("integer");
        } else if (where == "display.window_x" || where == "display.x") {
            if (!parse_int(value, &window_x)) bad("integer");
        } else if (where == "display.window_y" || where == "display.y") {
            if (!parse_int(value, &window_y)) bad("integer");
        } else if (where == "display.monitor") {
            if (!parse_int(value, &monitor)) bad("integer");
        } else if (where == "display.cursor_scale") {
            if (!parse_int(value, &cursor_scale)) bad("1..4");
        } else if (where == "display.mute_on_focus_loss") {
            if (!parse_bool(value, &mute_on_focus_loss)) bad("0|1");
        } else if (where == "audio.master" || where == "audio.master_volume") {
            if (!parse_float(value, &master_volume)) bad("0..1");
        } else if (where == "audio.speech" || where == "audio.speech_volume") {
            if (!parse_float(value, &speech_volume)) bad("0..1");
        } else if (where == "audio.ambient" || where == "audio.ambient_volume") {
            if (!parse_float(value, &ambient_volume)) bad("0..1");
        } else if (where == "audio.effects" || where == "audio.effects_volume") {
            if (!parse_float(value, &effects_volume)) bad("0..1");
        } else if (where == "audio.subtitles") {
            if (!parse_bool(value, &subtitles)) bad("0|1");
        } else if (where == "game.hotspots") {
            if (!parse_bool(value, &hotspots)) bad("0|1");
        } else if (where == "game.hotspots_all") {
            if (!parse_bool(value, &hotspots_all)) bad("0|1");
        } else if (where == "game.autosave") {
            if (!parse_bool(value, &autosave)) bad("0|1");
        } else if (where == "game.skip_seen") {
            if (!parse_bool(value, &skip_seen)) bad("0|1");
        } else if (where == "game.gamepad") {
            if (!parse_bool(value, &gamepad)) bad("0|1");
        } else if (where == "keys.settings" || where == "keys.settings_key") {
            key_settings = value;
        } else if (where == "keys.backlog" || where == "keys.backlog_key") {
            key_backlog = value;
        } else if (where == "keys.skip" || where == "keys.skip_key") {
            key_skip = value;
        } else if (where == "keys.quicksave" || where == "keys.quicksave_key") {
            key_quicksave = value;
        } else if (where == "keys.quickload" || where == "keys.quickload_key") {
            key_quickload = value;
        } else if (where == "keys.autoload" || where == "keys.autoload_key" ||
                   where == "keys.continue") {
            key_autoload = value;
        } else if (where == "keys.screenshot" || where == "keys.screenshot_key") {
            key_screenshot = value;
        } else if (where == "keys.hotspots" || where == "keys.hotspots_key") {
            key_hotspots = value;
        } else if (where == "language.text" || where == "language.text_language") {
            text_language = value;
        } else if (where == "language.voice" || where == "language.voice_language") {
            voice_language = value;
        } else {
            append_error(error, "line " + std::to_string(line_number) + ": unknown key " +
                                    where);
        }
    }
    clamp();
    return true;
}

bool Settings::save(const std::filesystem::path& path, std::string* error) const {
    std::error_code filesystem_error;
    if (!path.parent_path().empty()) {
        std::filesystem::create_directories(path.parent_path(), filesystem_error);
        if (filesystem_error) {
            if (error != nullptr) *error = filesystem_error.message();
            return false;
        }
    }
    std::ofstream output(path, std::ios::trunc);
    if (!output) {
        if (error != nullptr) *error = "cannot open " + path.string();
        return false;
    }
    output << "# Byzantine: The Betrayal - port settings\n"
           << "# This file is not read by the original game.\n\n"
           << "[display]\n"
           << "mode=" << window_mode_name(window_mode) << '\n'
           << "scale=" << scale_mode_name(scale_mode) << '\n'
           << "vsync=" << (vsync ? 1 : 0) << '\n'
           << "window_width=" << window_width << '\n'
           << "window_height=" << window_height << '\n'
           << "window_x=" << window_x << '\n'
           << "window_y=" << window_y << '\n'
           << "monitor=" << monitor << '\n'
           << "cursor_scale=" << cursor_scale << '\n'
           << "mute_on_focus_loss=" << (mute_on_focus_loss ? 1 : 0) << "\n\n"
           << "[audio]\n"
           << "master=" << master_volume << '\n'
           << "speech=" << speech_volume << '\n'
           << "ambient=" << ambient_volume << '\n'
           << "effects=" << effects_volume << '\n'
           << "subtitles=" << (subtitles ? 1 : 0) << "\n\n"
           << "[game]\n"
           << "hotspots=" << (hotspots ? 1 : 0) << '\n'
           << "hotspots_all=" << (hotspots_all ? 1 : 0) << '\n'
           << "autosave=" << (autosave ? 1 : 0) << '\n'
           << "skip_seen=" << (skip_seen ? 1 : 0) << '\n'
           << "gamepad=" << (gamepad ? 1 : 0) << "\n\n"
           << "[keys]\n"
           << "settings=" << key_settings << '\n'
           << "backlog=" << key_backlog << '\n'
           << "skip=" << key_skip << '\n'
           << "quicksave=" << key_quicksave << '\n'
           << "quickload=" << key_quickload << '\n'
           << "autoload=" << key_autoload << '\n'
           << "screenshot=" << key_screenshot << '\n'
           << "hotspots=" << key_hotspots << "\n\n"
           << "[language]\n"
           << "text=" << text_language << '\n'
           << "voice=" << voice_language << '\n';
    if (!output) {
        if (error != nullptr) *error = "cannot write " + path.string();
        return false;
    }
    return true;
}

}  // namespace byzantine
