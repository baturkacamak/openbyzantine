#include "byzantine/language.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
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

std::optional<std::filesystem::path> exact_or_case_insensitive(
    const std::filesystem::path& directory, const std::string& name) {
    std::error_code error;
    const auto exact = directory / name;
    if (std::filesystem::exists(exact, error)) return exact;
    if (!std::filesystem::is_directory(directory, error)) return std::nullopt;
    const auto wanted = lower(name);
    for (const auto& entry : std::filesystem::directory_iterator(directory, error)) {
        if (lower(entry.path().filename().string()) == wanted) return entry.path();
    }
    return std::nullopt;
}

std::vector<std::string> candidate_paths(const std::string& relative) {
    std::vector<std::string> result;
    const auto add = [&result](const std::string& value) {
        if (value.empty()) return;
        if (std::find(result.begin(), result.end(), value) != result.end()) return;
        result.push_back(value);
    };
    add(relative);
    const auto copy2hd = lower(relative).rfind("copy2hd/", 0) == 0;
    if (!copy2hd) add("copy2hd/" + relative);
    if (copy2hd) add(relative.substr(8));
    return result;
}

std::optional<std::filesystem::path> find_in_root(const std::filesystem::path& root,
                                                  const std::string& relative,
                                                  bool want_directory) {
    if (root.empty()) return std::nullopt;
    for (const auto& candidate : candidate_paths(relative)) {
        const auto found = find_relative(root, candidate);
        if (!found) continue;
        std::error_code error;
        if (want_directory) {
            if (std::filesystem::is_directory(*found, error)) return found;
        } else if (std::filesystem::is_regular_file(*found, error)) {
            return found;
        }
    }
    return std::nullopt;
}

struct Manifest {
    LanguagePack pack;
    bool english_explicit = false;
};

std::optional<Manifest> read_manifest(const std::filesystem::path& directory,
                                      const std::filesystem::path& file) {
    std::ifstream input(file);
    if (!input) return std::nullopt;
    Manifest manifest;
    manifest.pack.code = directory.filename().string();
    manifest.pack.name = manifest.pack.code;
    manifest.pack.root = directory;
    manifest.pack.text = true;
    manifest.pack.voice = false;
    std::string section;
    std::string line;
    while (std::getline(input, line)) {
        line = trim(line);
        if (line.empty() || line.front() == '#' || line.front() == ';') continue;
        if (line.front() == '[' && line.back() == ']') {
            section = lower(trim(line.substr(1, line.size() - 2)));
            continue;
        }
        const auto equals = line.find('=');
        if (equals == std::string::npos) continue;
        const auto key = lower(trim(line.substr(0, equals)));
        const auto value = trim(line.substr(equals + 1));
        if (key == "code") manifest.pack.code = value;
        else if (key == "name" || key == "title") manifest.pack.name = value;
        else if (key == "text" || key == "has_text") {
            if (!parse_bool(value, &manifest.pack.text)) {
                std::cerr << "language_pack invalid " << file << " text=" << value << '\n';
            }
        } else if (key == "voice" || key == "has_voice") {
            if (!parse_bool(value, &manifest.pack.voice)) {
                std::cerr << "language_pack invalid " << file << " voice=" << value << '\n';
            }
        } else if (key == "messages_encoding" || key == "encoding") {
            manifest.pack.messages_encoding = lower(value);
        } else if (key == "english_font_branch" || key == "english") {
            if (parse_bool(value, &manifest.pack.english_font_branch)) {
                manifest.english_explicit = true;
            }
        } else if (key == "font_branch") {
            manifest.pack.english_font_branch = lower(value) == "english";
            manifest.english_explicit = true;
        } else if (key == "section") {
            // Accepted for readability; the section is implicit.
        } else if (key == "description") {
            // Informational only.
        } else {
            std::cerr << "language_pack unknown key=" << key << " file=" << file << '\n';
        }
        (void)section;
    }
    if (manifest.pack.code.empty()) manifest.pack.code = directory.filename().string();
    if (manifest.pack.name.empty()) manifest.pack.name = manifest.pack.code;
    return manifest;
}

}  // namespace

std::optional<std::filesystem::path> find_relative(const std::filesystem::path& root,
                                                   const std::string& relative) {
    if (root.empty() || relative.empty()) return std::nullopt;
    const std::filesystem::path input(relative);
    if (input.is_absolute()) return std::nullopt;
    std::filesystem::path current = root;
    for (const auto& component : input) {
        const auto part = component.string();
        if (part.empty() || part == ".") continue;
        if (part == "..") return std::nullopt;
        const auto found = exact_or_case_insensitive(current, part);
        if (!found) return std::nullopt;
        current = *found;
    }
    std::error_code error;
    if (!std::filesystem::exists(current, error)) return std::nullopt;
    return current;
}

bool messages_english_branch(const std::filesystem::path& messages) {
    std::ifstream input(messages, std::ios::binary);
    if (!input) return false;
    std::string line;
    int index = 0;
    constexpr int kLanguageLine = 0x3e;
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (index == kLanguageLine) return line.find("English") != std::string::npos;
        ++index;
    }
    return false;
}

LanguageSelection LanguageSelection::discover(const std::filesystem::path& base,
                                              const std::filesystem::path& root,
                                              const std::string& text_code,
                                              const std::string& voice_code,
                                              std::ostream* log) {
    LanguageSelection selection;
    selection.base = base;
    selection.root = root.empty() ? base / "lang" : root;
    selection.text_code = text_code.empty() ? "auto" : lower(text_code);
    selection.voice_code = voice_code.empty() ? "auto" : lower(voice_code);

    std::error_code error;
    if (std::filesystem::is_directory(selection.root, error)) {
        std::vector<std::filesystem::path> directories;
        for (const auto& entry : std::filesystem::directory_iterator(selection.root, error)) {
            if (entry.is_directory()) directories.push_back(entry.path());
        }
        std::sort(directories.begin(), directories.end());
        for (const auto& directory : directories) {
            const auto manifest = exact_or_case_insensitive(directory, "pack.ini");
            if (!manifest) continue;
            auto parsed = read_manifest(directory, *manifest);
            if (!parsed) continue;
            if (!parsed->english_explicit) {
                // The pack may keep messages at its root or under copy2hd/.
                for (const auto& relative :
                     {"setupart/MESSAGES.TXT", "copy2hd/setupart/MESSAGES.TXT"}) {
                    const auto messages = find_in_root(directory, relative, false);
                    if (!messages) continue;
                    parsed->pack.english_font_branch =
                        messages_english_branch(*messages);
                    break;
                }
            }
            selection.packs.push_back(std::move(parsed->pack));
        }
    }
    std::sort(selection.packs.begin(), selection.packs.end(),
              [](const LanguagePack& left, const LanguagePack& right) {
                  return left.code < right.code;
              });

    const auto choose = [&](const std::string& code, bool want_text,
                            const char* what) -> const LanguagePack* {
        if (code.empty() || code == "auto" || code == "base") return nullptr;
        for (const auto& pack : selection.packs) {
            if (lower(pack.code) != code) continue;
            if (want_text && !pack.text) {
                if (log != nullptr) {
                    *log << "language_pack " << what << " code=" << code
                         << " ignored: pack has no text data\n";
                }
                return nullptr;
            }
            if (!want_text && !pack.voice) {
                if (log != nullptr) {
                    *log << "language_pack " << what << " code=" << code
                         << " ignored: pack has no voice data\n";
                }
                return nullptr;
            }
            return &pack;
        }
        if (log != nullptr) {
            *log << "language_pack " << what << " code=" << code
                 << " not found; using base data\n";
        }
        return nullptr;
    };
    selection.text = choose(selection.text_code, true, "text");
    selection.voice = choose(selection.voice_code, false, "voice");
    if (log != nullptr) {
        *log << "language_packs root=" << selection.root
             << " found=" << selection.packs.size()
             << " text=" << selection.text_code_or_base()
             << " voice=" << selection.voice_code_or_base() << '\n';
    }
    return selection;
}

std::optional<std::filesystem::path> LanguageSelection::find_text(
    const std::string& relative) const {
    if (text != nullptr) {
        if (const auto found = find_in_root(text->root, relative, false)) return found;
    }
    return find_in_root(base, relative, false);
}

std::optional<std::filesystem::path> LanguageSelection::find_voice(
    const std::string& relative) const {
    if (voice != nullptr) {
        if (const auto found = find_in_root(voice->root, relative, false)) return found;
    }
    return find_in_root(base, relative, false);
}

std::optional<std::filesystem::path> LanguageSelection::find_shared(
    const std::string& relative) const {
    return find_in_root(base, relative, false);
}

std::optional<std::filesystem::path> LanguageSelection::find_text_dir(
    const std::string& relative) const {
    if (text != nullptr) {
        if (const auto found = find_in_root(text->root, relative, true)) return found;
    }
    return find_in_root(base, relative, true);
}

std::optional<std::filesystem::path> LanguageSelection::find_voice_dir(
    const std::string& relative) const {
    if (voice != nullptr) {
        if (const auto found = find_in_root(voice->root, relative, true)) return found;
    }
    return find_in_root(base, relative, true);
}

std::string LanguageSelection::text_code_or_base() const {
    return text != nullptr ? text->code : "base";
}

std::string LanguageSelection::voice_code_or_base() const {
    return voice != nullptr ? voice->code : "base";
}

std::string LanguageSelection::messages_encoding() const {
    if (text != nullptr && !text->messages_encoding.empty()) {
        return text->messages_encoding;
    }
    // The shipped Turkish table is the base with no pack. Existing behaviour
    // converts its CP1254 bytes; a pack can say raw/cp1252 explicitly. An
    // English base is already direct-indexed, so leave its bytes alone.
    if (english_font_branch()) return "raw";
    return "cp1254";
}

bool LanguageSelection::english_font_branch() const {
    if (text != nullptr) return text->english_font_branch;
    for (const auto& relative : {"setupart/MESSAGES.TXT", "copy2hd/setupart/MESSAGES.TXT"}) {
        const auto messages = find_in_root(base, relative, false);
        if (messages) return messages_english_branch(*messages);
    }
    return false;
}

std::string LanguageSelection::describe() const {
    std::string result = "text=" + text_code_or_base() + " voice=" + voice_code_or_base();
    if (text != nullptr) result += " text_root=" + text->root.string();
    if (voice != nullptr) result += " voice_root=" + voice->root.string();
    return result;
}

}  // namespace byzantine
