#pragma once
// 0xbd CF video target (0x40d031 / FUN_0040c624).
//
// The original tries "avi/<name>.avi" first (0x45db7c) and, when that file cannot
// be opened, rebuilds the path as the mode folder plus the name plus ".avi"
// (FUN_0040c624, folders at 0x45db28..0x45db74). This module only resolves those
// paths and the DAT_004859cd side effect; decoding and presentation stay in the
// caller (the port uses FFmpeg plus the existing JPEG/WAV readers, exactly like
// the conversation clips).
//
// Evidence and hashes: analysis/rea/VIDEO_AND_AUDIO.md, CF_VIDEO_GAP.md.
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace byzantine {

// FUN_0040d031's mode table: 0 video\slide, 1 video\intro, 2 video\site,
// 3 video\transit, 4 video\char, 5 video\anim. The default entry (0x45db74) is
// the bare "video" root, which this reports as nullptr rather than guessing.
const char* video_mode_folder(std::uint16_t mode);

// The two paths 0x40d031 tries, in order. `relative` is slash-separated and
// relative to the game root; `fallback` marks the mode-specific second attempt.
struct VideoCandidate {
    std::string relative;
    bool fallback = false;
};
std::vector<VideoCandidate> video_candidates(std::uint16_t mode,
                                             const std::string& name);

// 0x40d031 sets DAT_004859cd = 1 after the clip played, except for mode 5 and
// for the "mcaft1a1" special case (FUN_0040da1f). The flag is render-only: it
// turns off 0x43b216's snap pass.
bool video_sets_4859cd(std::uint16_t mode, const std::string& name);

// Case-insensitive lookup of a multi-component relative path
// ("video/char/MCANB1A.avi") under `root`. The shipped CD trees mix cases
// (MCANB1A.avi next to mbgbcxa.avi), and 7z matches ISO members
// case-sensitively, so the player resolves the on-disk name the same way the
// backgrounds/animations/cursors already are.
std::optional<std::filesystem::path> find_video_file(
    const std::filesystem::path& root, const std::string& relative);

}  // namespace byzantine
