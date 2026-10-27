// 0xbd CF video path rules (0x40d031 / FUN_0040c624) and the case-insensitive
// lookup that finds the shipped mixed-case video trees.
#include "byzantine/video.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

int failures = 0;

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void test_mode_folders() {
    require(std::string(byzantine::video_mode_folder(0)) == "slide", "mode 0 folder");
    require(std::string(byzantine::video_mode_folder(1)) == "intro", "mode 1 folder");
    require(std::string(byzantine::video_mode_folder(2)) == "site", "mode 2 folder");
    require(std::string(byzantine::video_mode_folder(3)) == "transit", "mode 3 folder");
    require(std::string(byzantine::video_mode_folder(4)) == "char", "mode 4 folder");
    require(std::string(byzantine::video_mode_folder(5)) == "anim", "mode 5 folder");
    // The default entry (0x45db74) is the bare "video" root: no sub-folder.
    require(byzantine::video_mode_folder(6) == nullptr, "mode 6 has no folder");
    require(byzantine::video_mode_folder(0xffff) == nullptr, "0xffff has no folder");
}

void test_candidates() {
    // NB label 337's request: record 1 "MCaNB1a", mode 4 -> video/char.
    const auto nb = byzantine::video_candidates(4, "MCaNB1a");
    require(nb.size() == 2, "two attempts per request");
    require(nb[0].relative == "avi/MCaNB1a.avi" && !nb[0].fallback,
            "0x40d031's primary avi/<name>.avi");
    require(nb[1].relative == "video/char/MCaNB1a.avi" && nb[1].fallback,
            "mode 4 falls back to video/char");
    // MAP's establishing shots are mode 2 (video/site).
    const auto map = byzantine::video_candidates(2, "NBEst1");
    require(map[1].relative == "video/site/NBEst1.avi", "mode 2 falls back to video/site");
    // The default mode keeps the bare video root.
    const auto other = byzantine::video_candidates(9, "foo");
    require(other[1].relative == "video/foo.avi", "default mode uses video/<name>.avi");
    require(byzantine::video_candidates(4, "").empty(), "no name, no candidate");
}

void test_4859cd() {
    // 0x40d031: mode 5 calls 0x40da1f instead of setting DAT_004859cd.
    require(byzantine::video_sets_4859cd(4, "MCaNB1a"), "mode 4 sets 0x4859cd");
    require(byzantine::video_sets_4859cd(2, "NBEst1"), "mode 2 sets 0x4859cd");
    require(byzantine::video_sets_4859cd(0, "exCam"), "mode 0 sets 0x4859cd");
    require(!byzantine::video_sets_4859cd(5, "hsain1"), "mode 5 leaves 0x4859cd");
    // __strcmpi(name, "mcaft1a1") is checked before the mode test.
    require(!byzantine::video_sets_4859cd(0, "mcaft1a1"), "mcaft1a1 leaves 0x4859cd");
    require(!byzantine::video_sets_4859cd(4, "McAfT1A1"), "mcaft1a1 is case-insensitive");
}

void test_lookup_case_insensitive() {
    std::error_code error;
    const auto root = std::filesystem::temp_directory_path(error) /
                      "byz-video-test" /
                      std::to_string(static_cast<unsigned long>(
                          std::hash<std::string>{}("byz-video")));
    std::filesystem::remove_all(root, error);
    std::filesystem::create_directories(root / "video" / "char", error);
    // The shipped CDs really do mix these cases in one directory.
    {
        std::ofstream(root / "video" / "char" / "MCANB1A.avi") << "nb";
        std::ofstream(root / "video" / "char" / "mbgbcxa.avi") << "bg";
        std::filesystem::create_directories(root / "video" / "SITE", error);
        std::ofstream(root / "video" / "SITE" / "MREST1.AVI") << "mr";
    }
    const auto nb = byzantine::find_video_file(root, "video/char/MCANB1A.avi");
    require(nb && nb->filename().string() == "MCANB1A.avi",
            "exact-case member found under the mixed-case tree");
    const auto lower = byzantine::find_video_file(root, "video/char/mcanb1a.avi");
    require(lower && lower->filename().string() == "MCANB1A.avi",
            "lowercase request finds the uppercase member");
    const auto site = byzantine::find_video_file(root, "video/site/MRest1.avi");
    require(site && site->filename().string() == "MREST1.AVI",
            "directory and file case do not have to match");
    require(!byzantine::find_video_file(root, "video/char/missing.avi"),
            "a missing member is not invented");
    require(!byzantine::find_video_file(root / "nope", "video/char/MCANB1A.avi"),
            "a missing root is not invented");
    std::filesystem::remove_all(root, error);
}

}  // namespace

int main() {
    test_mode_folders();
    test_candidates();
    test_4859cd();
    test_lookup_case_insensitive();
    if (failures != 0) {
        std::cerr << failures << " video check(s) failed\n";
        return 1;
    }
    std::cout << "video-test ok\n";
    return 0;
}
