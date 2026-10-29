#pragma once

#include <filesystem>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace byzantine {

// One localized data set. A pack is deliberately an overlay, not a full game
// copy: a resolver asks the selected pack first and then the base data root.
// The same layout works for a text-only pack (fonts/ce/byz/setupart) and for a
// voice pack (dialog/video/sounds/ccf).
struct LanguagePack {
    std::string code;
    std::string name;
    std::filesystem::path root;
    bool text = true;
    bool voice = false;
    // How setupart/MESSAGES.TXT stores its high bytes. The Turkish shipped file
    // mixes CP1254 and the game font encoding, so "raw" is the safe default.
    std::string messages_encoding = "raw";
    // The original 0x406cbf reads MESSAGES.TXT line 0x3e and switches the font
    // index rule when that line contains "English". An explicit manifest value
    // overrides the detected line.
    bool english_font_branch = false;
};

// The active text/voice overlays. `base` is the flat prepared data directory
// (the same one scene.sh builds); packs live under <base>/lang/<code>/.
struct LanguageSelection {
    std::filesystem::path base;
    std::filesystem::path root;
    std::vector<LanguagePack> packs;
    std::string text_code = "auto";
    std::string voice_code = "auto";
    const LanguagePack* text = nullptr;
    const LanguagePack* voice = nullptr;

    // Scans root/lang/*/pack.ini. Missing files are ignored; malformed packs
    // are reported to `log` and skipped. `auto` means the base data.
    static LanguageSelection discover(const std::filesystem::path& base,
                                      const std::filesystem::path& root,
                                      const std::string& text_code,
                                      const std::string& voice_code,
                                      std::ostream* log = nullptr);

    // Both helpers search the selected pack first and then base. `relative` is
    // a path such as "fonts/klio.sfn", "byz/IMHMR2A" or
    // "setupart/MESSAGES.TXT". The "copy2hd/" prefix is also tried because the
    // shipped tree keeps the UI assets there.
    std::optional<std::filesystem::path> find_text(const std::string& relative) const;
    std::optional<std::filesystem::path> find_voice(const std::string& relative) const;
    std::optional<std::filesystem::path> find_shared(const std::string& relative) const;

    std::optional<std::filesystem::path> find_text_dir(const std::string& relative) const;
    std::optional<std::filesystem::path> find_voice_dir(const std::string& relative) const;

    std::string text_code_or_base() const;
    std::string voice_code_or_base() const;
    std::string messages_encoding() const;
    bool english_font_branch() const;

    std::string describe() const;
};

// Reads setupart/MESSAGES.TXT line 0x3e (zero-based) and returns true when it
// contains "English", mirroring 0x406cbf. A missing file returns false.
bool messages_english_branch(const std::filesystem::path& messages);

// Resolves a relative path case-insensitively under `root`. It never escapes
// `root` (an absolute path or a `..` component returns empty).
std::optional<std::filesystem::path> find_relative(const std::filesystem::path& root,
                                                   const std::string& relative);

}  // namespace byzantine
