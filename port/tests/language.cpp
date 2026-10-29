#include "byzantine/language.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <unistd.h>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

void write_text(const std::filesystem::path& path, const std::string& text) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    output << text;
}

std::filesystem::path temporary_directory() {
    const auto path = std::filesystem::temp_directory_path() /
                      ("byzantine-language-" + std::to_string(
                           static_cast<unsigned long long>(::getpid())));
    std::filesystem::remove_all(path);
    std::filesystem::create_directories(path);
    return path;
}
}  // namespace

int main() {
    try {
        using namespace byzantine;
        const auto root = temporary_directory();
        const auto base = root / "data";
        const auto en = base / "lang" / "en";
        const auto de = base / "lang" / "de";
        const auto tr = base / "lang" / "tr";
        write_text(base / "byz" / "SHARED", "base line\n");
        write_text(base / "fonts" / "klio.sfn", "base font\n");
        write_text(base / "setupart" / "MESSAGES.TXT", "base messages\n");

        write_text(en / "pack.ini",
                   "[pack]\ncode=en\nname=English\ntext=1\nvoice=1\n"
                   "messages_encoding=cp1252\nfont_branch=english\n");
        write_text(en / "byz" / "IMHMR2A", "english line\n");
        write_text(en / "fonts" / "klio.sfn", "english font\n");
        write_text(en / "dialog" / "imhmr2a" / "mr2meh02.avi", "english clip\n");
        write_text(en / "setupart" / "MESSAGES.TXT", "english messages\n");

        write_text(de / "pack.ini",
                   "[pack]\ncode=de\nname=Deutsch\ntext=1\nvoice=0\n");
        write_text(de / "byz" / "IMHMR2A", "deutsche zeile\n");

        write_text(tr / "pack.ini", "[pack]\ncode=tr\nname=Turkce\ntext=1\nvoice=1\n");
        write_text(tr / "byz" / "IMHMR2A", "turkce satir\n");

        std::ostringstream log;
        const auto selection = LanguageSelection::discover(base, base / "lang", "en", "en", &log);
        require(selection.packs.size() == 3, "pack count");
        require(selection.text != nullptr && selection.text->code == "en", "text pack");
        require(selection.voice != nullptr && selection.voice->code == "en", "voice pack");
        require(selection.english_font_branch(), "English font branch from manifest");

        const auto byz = selection.find_text("byz/IMHMR2A");
        require(byz && std::filesystem::equivalent(*byz, en / "byz" / "IMHMR2A"),
                "text file did not come from the en pack");
        const auto shared = selection.find_text("byz/SHARED");
        require(shared && std::filesystem::equivalent(*shared, base / "byz" / "SHARED"),
                "base fallback failed");
        const auto clip = selection.find_voice("dialog/imhmr2a/mr2meh02.avi");
        require(clip && std::filesystem::equivalent(*clip,
                                                    en / "dialog" / "imhmr2a" / "mr2meh02.avi"),
                "voice file did not come from the en pack");
        require(selection.find_text("fonts/KLIO.SFN").has_value(), "case-insensitive lookup failed");
        require(!selection.find_text("../byz/SHARED"), "path traversal accepted");
        require(!selection.find_text("/etc/passwd"), "absolute path accepted");
        require(selection.messages_encoding() == "cp1252", "messages encoding");
        require(selection.find_text_dir("fonts").has_value(), "text directory lookup");

        // A voice-only request against a text-only pack falls back to base.
        const auto text_only = LanguageSelection::discover(base, base / "lang", "de", "de", &log);
        require(text_only.text != nullptr && text_only.text->code == "de", "de text pack");
        require(text_only.voice == nullptr, "text-only pack used as voice");
        require(text_only.voice_code_or_base() == "base", "voice fallback label");
        require(text_only.find_voice("dialog/imhmr2a/mr2meh02.avi") == std::nullopt,
                "voice lookup invented a file");

        // The language marker is MESSAGES.TXT line 0x3e (zero based).
        const auto marker = root / "marker.txt";
        {
            std::ofstream output(marker);
            for (int index = 0; index < 0x3e; ++index) output << "line\n";
            output << "Language: English\n";
        }
        require(messages_english_branch(marker), "English marker not detected");
        {
            std::ofstream output(marker);
            for (int index = 0; index < 0x3e; ++index) output << "line\n";
            output << "Dil: Turkce\n";
        }
        require(!messages_english_branch(marker), "Turkish marker read as English");

        const auto auto_selection =
            LanguageSelection::discover(base, base / "lang", "auto", "auto", &log);
        require(auto_selection.text == nullptr && auto_selection.voice == nullptr,
                "auto did not select the base");
        require(auto_selection.text_code_or_base() == "base", "auto text label");

        std::filesystem::remove_all(root);
        std::cout << "language tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "language-test: " << error.what() << '\n';
        return 1;
    }
}
