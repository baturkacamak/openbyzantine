#include "byzantine/ambient_audio.hpp"

#include <SDL.h>

#include <array>
#include <cmath>
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

        // Torn-tail guard (NBEST1.avi's last 67 ms is full-scale noise). The
        // rule must fire for a ragged buffer and stay away from real endings.
        const auto pcm_wav = [](const std::vector<std::int16_t>& samples, int channels) {
            const auto data_size = static_cast<std::uint32_t>(samples.size() * 2);
            std::vector<std::uint8_t> bytes;
            const auto put32 = [&bytes](std::uint32_t value) {
                for (int i = 0; i < 4; ++i) {
                    bytes.push_back(static_cast<std::uint8_t>(value >> (i * 8)));
                }
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
            put16(1);
            put16(static_cast<std::uint16_t>(channels));
            put32(22050);
            put32(static_cast<std::uint32_t>(22050 * channels * 2));
            put16(static_cast<std::uint16_t>(channels * 2));
            put16(16);
            text("data");
            put32(data_size);
            for (const auto sample : samples) {
                put16(static_cast<std::uint16_t>(sample));
            }
            return parse_wav(bytes, "synthetic-tail.wav");
        };
        const auto push_stereo = [](std::vector<std::int16_t>& out, int value) {
            out.push_back(static_cast<std::int16_t>(value));
            out.push_back(static_cast<std::int16_t>(value));
        };
        {
            std::vector<std::int16_t> samples;
            for (int frame = 0; frame < 11025; ++frame) {  // 500 ms quiet
                push_stereo(samples, ((frame * 37) % 600) - 300);
            }
            const int torn[] = {25341, 29437, 31997, -23555, 32765, 31485, -29699, 30979};
            for (int frame = 0; frame < 1456; ++frame) {  // ~66 ms torn buffer
                push_stereo(samples, torn[frame % 8]);
            }
            for (int frame = 0; frame < 20; ++frame) push_stereo(samples, 0);
            auto clip = pcm_wav(samples, 2);
            const auto before = clip.duration_ms;
            const auto guard = trim_torn_audio_tail(clip);
            require(guard.trimmed, "torn audio tail was not detected");
            require(guard.trimmed_at_ms >= 495 && guard.trimmed_at_ms <= 505,
                    "torn tail trimmed at the wrong point");
            require(clip.duration_ms < before && clip.duration_ms == guard.trimmed_at_ms,
                    "torn tail trim did not shorten the clip");
            require(guard.peak > 30000, "torn tail peak not reported");
        }
        {
            // A loud but continuous ending (0.5 FS sine) is real audio.
            std::vector<std::int16_t> samples;
            for (int frame = 0; frame < 11025; ++frame) push_stereo(samples, 300);
            for (int frame = 0; frame < 2205; ++frame) {
                push_stereo(samples, static_cast<int>(16000 * std::sin(frame * 0.2)));
            }
            auto clip = pcm_wav(samples, 2);
            require(!trim_torn_audio_tail(clip).trimmed,
                    "trimmed a continuous loud ending");
        }
        {
            // A loud slam that decays (0.95 FS for 40 ms, then silence).
            std::vector<std::int16_t> samples;
            for (int frame = 0; frame < 11025; ++frame) push_stereo(samples, 200);
            for (int frame = 0; frame < 882; ++frame) {
                push_stereo(samples, static_cast<int>(31000 * std::sin(frame * 0.9)));
            }
            for (int frame = 0; frame < 2205; ++frame) push_stereo(samples, 0);
            auto clip = pcm_wav(samples, 2);
            require(!trim_torn_audio_tail(clip).trimmed,
                    "trimmed a decaying sound effect");
        }
        {
            // A single-sample click, not a torn buffer.
            std::vector<std::int16_t> samples;
            for (int frame = 0; frame < 11025; ++frame) push_stereo(samples, 120);
            push_stereo(samples, 32000);
            for (int frame = 0; frame < 220; ++frame) push_stereo(samples, 120);
            auto clip = pcm_wav(samples, 2);
            require(!trim_torn_audio_tail(clip).trimmed, "trimmed a one-sample click");
        }

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

        // Host gains and mute must be safe while voices exist (the actual
        // amplitude is applied in the callback; this checks the public API).
        mixer.set_volumes(0.5f, 0.4f, 0.3f, 0.2f);
        require(mixer.start(7, clip, true, VoiceBus::ambient),
                "mixer rejected a voice after setting gains");
        mixer.set_muted(true);
        mixer.set_muted(false);
        mixer.set_volumes(1.0f, 1.0f, 1.0f, 1.0f);
        mixer.stop(7);
        std::cout << "audio tests passed (" << mixer.device_name() << ")\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "audio-test: " << error.what() << '\n';
        return 1;
    }
}
