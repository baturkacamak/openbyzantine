#include "byzantine/video.hpp"

#include <cctype>
#include <system_error>
#include <utility>

namespace byzantine {
namespace {

std::string lower_ascii(std::string text) {
    for (auto& character : text) {
        character = static_cast<char>(
            std::tolower(static_cast<unsigned char>(character)));
    }
    return text;
}

}  // namespace

const char* video_mode_folder(std::uint16_t mode) {
    switch (mode) {
        case 0: return "slide";
        case 1: return "intro";
        case 2: return "site";
        case 3: return "transit";
        case 4: return "char";
        case 5: return "anim";
        default: return nullptr;
    }
}

std::vector<VideoCandidate> video_candidates(std::uint16_t mode,
                                             const std::string& name) {
    std::vector<VideoCandidate> candidates;
    if (name.empty()) return candidates;
    // 0x40d031's primary attempt: _sprintf(local_10c, "avi\\%s.avi", name).
    candidates.push_back({"avi/" + name + ".avi", false});
    // FUN_0040c624: <mode folder>\<name>.avi; the default entry is "video".
    std::string folder = "video";
    if (const char* sub = video_mode_folder(mode)) {
        folder += '/';
        folder += sub;
    }
    candidates.push_back({folder + "/" + name + ".avi", true});
    return candidates;
}

bool video_sets_4859cd(std::uint16_t mode, const std::string& name) {
    // FUN_0040d031: if (mode == 5) FUN_0040da1f(); else DAT_004859cd = 1.
    if (mode == 5) return false;
    // The name test is __strcmpi(name, "mcaft1a1") before the mode test.
    return lower_ascii(name) != "mcaft1a1";
}

std::optional<std::filesystem::path> find_video_file(
    const std::filesystem::path& root, const std::string& relative) {
    std::error_code error;
    if (!std::filesystem::is_directory(root, error)) return std::nullopt;
    auto current = root;
    std::size_t start = 0;
    while (start < relative.size()) {
        const auto slash = relative.find('/', start);
        const auto part = relative.substr(
            start, slash == std::string::npos ? std::string::npos : slash - start);
        if (part.empty()) {
            if (slash == std::string::npos) return std::nullopt;
            start = slash + 1;
            continue;
        }
        if (slash == std::string::npos) {
            const auto wanted = lower_ascii(part);
            for (const auto& entry : std::filesystem::directory_iterator(current, error)) {
                if (entry.is_regular_file() &&
                    lower_ascii(entry.path().filename().string()) == wanted) {
                    return entry.path();
                }
            }
            return std::nullopt;
        }
        const auto wanted = lower_ascii(part);
        std::optional<std::filesystem::path> next;
        for (const auto& entry : std::filesystem::directory_iterator(current, error)) {
            if (entry.is_directory() &&
                lower_ascii(entry.path().filename().string()) == wanted) {
                next = entry.path();
                break;
            }
        }
        if (!next) return std::nullopt;
        current = std::move(*next);
        start = slash + 1;
    }
    return std::nullopt;
}

}  // namespace byzantine
