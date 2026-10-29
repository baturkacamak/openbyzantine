#pragma once

#include <filesystem>
#include <string>

namespace byzantine {

// Host presentation choices. These are port settings, not game state: the
// original's savegame/gamepref_000 is never rewritten with them.
enum class WindowMode { windowed = 0, borderless = 1, maximized = 3 };
enum class ScaleMode { fit, integer };

struct Settings {
    WindowMode window_mode = WindowMode::windowed;
    ScaleMode scale_mode = ScaleMode::fit;
    bool vsync = false;
    int window_width = 1280;
    int window_height = 960;
    int window_x = -1;
    int window_y = -1;
    int monitor = -1;
    int cursor_scale = 1;
    bool mute_on_focus_loss = true;

    float master_volume = 1.0f;
    float speech_volume = 1.0f;
    float ambient_volume = 1.0f;
    float effects_volume = 1.0f;
    bool subtitles = true;

    bool hotspots = false;
    bool hotspots_all = true;
    bool autosave = true;
    bool skip_seen = false;
    bool gamepad = true;

    // Port action keys. They are stored as SDL key names (F10, F3, H, ...) and
    // resolved by play_main.cpp; the original game's own input is unchanged.
    std::string key_settings = "F10";
    std::string key_backlog = "F3";
    std::string key_skip = "F4";
    std::string key_quicksave = "F5";
    std::string key_quickload = "F9";
    std::string key_autoload = "F8";
    std::string key_screenshot = "F12";
    std::string key_hotspots = "H";

    // "auto" keeps the base data. A language pack is an overlay selected by
    // the code written in its pack.ini (LanguagePacks in language.hpp).
    std::string text_language = "auto";
    std::string voice_language = "auto";

    static Settings defaults();
    // Keeps every field inside its documented range. Called by load() and
    // with_settings(); never throws.
    void clamp();

    // Reads a simple INI file. A missing file returns false and leaves the
    // defaults intact; a malformed value is reported in `error` but valid
    // keys still load. The file is not created here.
    bool load(const std::filesystem::path& path, std::string* error = nullptr);
    bool save(const std::filesystem::path& path, std::string* error = nullptr) const;
};

const char* window_mode_name(WindowMode mode);
const char* scale_mode_name(ScaleMode mode);
bool parse_window_mode(const std::string& text, WindowMode* mode);
bool parse_scale_mode(const std::string& text, ScaleMode* mode);

}  // namespace byzantine
