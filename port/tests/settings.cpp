#include <array>
#include <vector>
#include "byzantine/window_host.hpp"
#include "byzantine/settings.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <unistd.h>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

std::filesystem::path temporary_directory() {
    const auto path = std::filesystem::temp_directory_path() /
                      ("byzantine-settings-" + std::to_string(
                           static_cast<unsigned long long>(::getpid())));
    std::filesystem::remove_all(path);
    std::filesystem::create_directories(path);
    return path;
}
}  // namespace

int main() {
    try {
        using namespace byzantine;
        const auto directory = temporary_directory();
        const auto path = directory / "byzantine.ini";

        Settings defaults;
        require(defaults.window_mode == WindowMode::windowed, "default window mode");
        require(defaults.scale_mode == ScaleMode::fit, "default scale mode");
        require(defaults.subtitles, "default subtitles");

        Settings saved;
        saved.window_mode = WindowMode::borderless;
        saved.scale_mode = ScaleMode::integer;
        saved.vsync = true;
        saved.window_width = 1600;
        saved.window_height = 1200;
        saved.window_x = 40;
        saved.window_y = 50;
        saved.cursor_scale = 2;
        saved.mute_on_focus_loss = false;
        saved.master_volume = 0.8f;
        saved.speech_volume = 0.7f;
        saved.ambient_volume = 0.6f;
        saved.effects_volume = 0.5f;
        saved.subtitles = false;
        saved.hotspots = true;
        saved.hotspots_all = false;
        saved.text_language = "en";
        saved.voice_language = "de";
        saved.autosave = false;
        saved.skip_seen = true;
        saved.gamepad = false;
        saved.key_settings = "F1";
        saved.key_backlog = "B";
        saved.key_skip = "S";
        saved.key_quicksave = "F6";
        saved.key_quickload = "F7";
        saved.key_autoload = "F8";
        saved.key_screenshot = "P";
        saved.key_hotspots = "O";
        std::string error;
        require(saved.save(path, &error), error.c_str());

        Settings loaded;
        require(loaded.load(path, &error), error.c_str());
        require(loaded.window_mode == WindowMode::borderless, "window mode round trip");
        require(loaded.scale_mode == ScaleMode::integer, "scale mode round trip");
        require(loaded.vsync, "vsync round trip");
        require(loaded.window_width == 1600 && loaded.window_height == 1200,
                "window size round trip");
        require(loaded.window_x == 40 && loaded.window_y == 50, "window position round trip");
        require(loaded.cursor_scale == 2, "cursor scale round trip");
        require(!loaded.mute_on_focus_loss, "focus mute round trip");
        require(std::fabs(loaded.master_volume - 0.8f) < 0.001f, "master volume");
        require(std::fabs(loaded.speech_volume - 0.7f) < 0.001f, "speech volume");
        require(std::fabs(loaded.ambient_volume - 0.6f) < 0.001f, "ambient volume");
        require(std::fabs(loaded.effects_volume - 0.5f) < 0.001f, "effects volume");
        require(!loaded.subtitles, "subtitles round trip");
        require(loaded.hotspots && !loaded.hotspots_all, "hotspot round trip");
        require(loaded.text_language == "en" && loaded.voice_language == "de",
                "language round trip");
        require(!loaded.autosave && loaded.skip_seen && !loaded.gamepad,
                "game toggles round trip");
        require(loaded.key_settings == "F1" && loaded.key_backlog == "B" &&
                    loaded.key_skip == "S" && loaded.key_quicksave == "F6" &&
                    loaded.key_quickload == "F7" && loaded.key_autoload == "F8" &&
                    loaded.key_screenshot == "P" && loaded.key_hotspots == "O",
                "key bindings round trip");

        Settings missing;
        require(!missing.load(directory / "does-not-exist.ini"), "missing file loaded");

        {
            std::ofstream bad(path);
            bad << "[display]\nmode=diagonal\nscale=integer\n"
                   "window_width=1\nwindow_height=99999\n"
                   "[audio]\nmaster=2\nspeech=-3\nsubtitles=off\n"
                   "[language]\ntext=en\n[unknown]\nno_equals\n";
        }
        Settings repaired;
        error.clear();
        require(repaired.load(path, &error), "bad file was not readable");
        require(!error.empty(), "bad file reported no error");
        require(repaired.scale_mode == ScaleMode::integer, "valid key after bad key");
        require(repaired.window_width == 640, "window width clamped up");
        require(repaired.window_height == 4320, "window height clamped down");
        require(std::fabs(repaired.master_volume - 1.0f) < 0.001f, "master clamped high");
        require(std::fabs(repaired.speech_volume - 0.0f) < 0.001f, "speech clamped low");
        require(!repaired.subtitles, "subtitles parsed");
        require(repaired.text_language == "en", "language parsed after invalid lines");

        for (const auto& test : std::vector<std::array<int, 6>>{
                {800,600,1000,600,1000,750}, {800,600,800,750,1000,750},
                {800,600,480,360,480,360}, {800,600,1001,600,1000,750},
                {800,600,100,100,320,240}}) {
            const auto size = aspect_window_size({test[0],test[1]}, {test[2],test[3]});
            require(size.width == test[4] && size.height == test[5], "4:3 resize edge and minimum");
        }
        const auto bounded = aspect_window_size({800,600}, {2000,600}, {1200,800});
        require(bounded.width == 1064 && bounded.height == 798, "aspect fit within usable bounds");
        require(near_game_aspect({1086,814}) && near_game_aspect({1135,851}) &&
                near_game_aspect({1145,859}), "reported native rounding must not trigger snapping");
        require(!near_game_aspect({1100,675}), "genuinely free-aspect fallback needs fitting");
        AspectResizeSettler settle;
        settle.resize({800,600}, 0, true);
        require(!settle.take(1000, false), "native WM resizing never queues a correction");
        settle.resize({800,600}, 0, false);
        settle.resize({900,650}, 100, false);
        require(!settle.take(279, false), "fallback waits after last resize event");
        require(!settle.take(500, true), "held mouse blocks fallback size changes");
        const auto origin = settle.take(501, false);
        require(origin && origin->width == 800 && origin->height == 600,
                "fallback uses drag start geometry only after release and quiet");
        require(!settle.take(1000, false), "settled correction cannot queue itself repeatedly");
        settle.resize({800,600}, 1100, false);
        settle.cancel();
        require(!settle.take(2000, false), "mode changes cancel pending normal resize");
        WindowMode mode = WindowMode::windowed;
        require(parse_window_mode("fullscreen", &mode) && mode == WindowMode::borderless,
                "fullscreen alias");
        require(parse_window_mode("2", &mode) && mode == WindowMode::borderless,
                "legacy exclusive numeric migrates to desktop fullscreen");
        require(!parse_window_mode("sideways", &mode), "accepted an invalid mode");
        require(parse_window_mode("exclusive", &mode) && mode == WindowMode::borderless,
                "legacy exclusive string migrates to desktop fullscreen");
        ScaleMode scale = ScaleMode::fit;
        require(parse_scale_mode("pixel-perfect", &scale) && scale == ScaleMode::integer,
                "pixel-perfect alias");
        require(!parse_scale_mode("stretch", &scale), "accepted an invalid scale");

        std::filesystem::remove_all(directory);
        std::cout << "settings tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "settings-test: " << error.what() << '\n';
        return 1;
    }
}
