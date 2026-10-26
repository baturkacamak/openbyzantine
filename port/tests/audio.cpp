#include "byzantine/ambient_audio.hpp"

#include <SDL.h>

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

std::vector<std::uint8_t> make_wav(int frames, int channels = 2, int rate = 22050,
                                   int bits = 16, int format = 1) {
    const auto data_size = static_cast<std::uint32_t>(frames * channels * (bits / 8));
    std::vector<std::uint8_t> bytes;
    const auto put32 = [&bytes](std::uint32_t value) {
        for (int i = 0; i < 4; ++i) bytes.push_back(static_cast<std::uint8_t>(value >> (i * 8)));
    };
    const auto put16 = [&bytes](std::uint16_t value) {
        bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
        bytes.push_back(static_cast<std::uint8_t>(value >> 8));
    };
    const auto text = [&bytes](const char* value) {
        for (int i = 0; i < 4; ++i) bytes.push_back(static_cast<std::uint8_t>(value[i]));
    };
    text("RIFF");
    put32(36 + data_size);
    text("WAVE");
    text("fmt ");
    put32(16);
    put16(static_cast<std::uint16_t>(format));
    put16(static_cast<std::uint16_t>(channels));
    put32(static_cast<std::uint32_t>(rate));
    put32(static_cast<std::uint32_t>(rate * channels * (bits / 8)));
    put16(static_cast<std::uint16_t>(channels * (bits / 8)));
    put16(static_cast<std::uint16_t>(bits));
    text("data");
    put32(data_size);
    for (int frame = 0; frame < frames; ++frame) {
        for (int channel = 0; channel < channels; ++channel) {
            const auto value = static_cast<std::int16_t>((frame * 64) % 3000);
            put16(static_cast<std::uint16_t>(value));
        }
    }
    return bytes;
}
}  // namespace

int main() {
    try {
        using namespace byzantine;
        SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
        const auto bytes = make_wav(2205);
        const auto clip = parse_wav(bytes, "synthetic.wav");
        require(clip.channels == 2 && clip.sample_rate == 22050 &&
                    clip.duration_ms == 100 && clip.bytes.size() == 2205 * 4,
                "WAV header/duration parse mismatch");

        const auto rejects = [](std::vector<std::uint8_t> data, const char* message) {
            bool rejected = false;
            try {
                parse_wav(data, "bad.wav");
            } catch (const std::runtime_error&) {
                rejected = true;
            }
            require(rejected, message);
        };
        rejects(make_wav(10, 2, 22050, 8), "accepted 8-bit WAV");
        rejects(make_wav(10, 2, 22050, 16, 3), "accepted non-PCM WAV");
        rejects(make_wav(0), "accepted a WAV without samples");
        {
            auto truncated = make_wav(10);
            truncated.resize(30);
            rejects(truncated, "accepted a truncated WAV");
        }
        rejects({'R', 'I', 'F', 'F'}, "accepted garbage");

        AudioMixer mixer;
        mixer.open();
        require(mixer.ready(), "SDL dummy audio device did not open");

        // 0xc4 ambient: a loop stays active until the script stops it.
        require(mixer.start(5, clip, true), "mixer rejected the loop");
        require(mixer.playing(5) && mixer.ambient() == 1 && mixer.active() >= 1,
                "loop voice not active");
        require(mixer.start(5, clip, true), "mixer rejected the same-id restart");
        require(mixer.ambient() == 1, "same-id restart duplicated the voice");
        // A second tagged voice is a separate record (two AM ambience loops).
        require(mixer.start(6, clip, true), "mixer rejected the second loop");
        require(mixer.ambient() == 2, "second loop did not add an ambient voice");
        mixer.stop(5);
        require(!mixer.playing(5) && mixer.playing(6) && mixer.ambient() == 1,
                "stop removed the wrong voice");
        // Conversation audio keeps the untagged queue contract: the drain
        // loop sees the queued bytes and then zero when the voice ends.
        mixer.queue(clip);
        const auto start = SDL_GetTicks();
        while (mixer.queued() != 0 && SDL_GetTicks() - start < 2000) SDL_Delay(5);
        require(SDL_GetTicks() - start < 2000,
                "queued conversation clip did not drain");
        mixer.stop_all();
        require(!mixer.playing(6) && mixer.active() == 0,
                "stop_all left voices behind");
        std::cout << "audio tests passed (" << mixer.device_name() << ")\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "audio-test: " << error.what() << '\n';
        return 1;
    }
}
