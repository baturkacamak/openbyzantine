#include "byzantine/ambient_audio.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace byzantine {
namespace {

constexpr int kDeviceRate = 22050;

std::uint32_t read_le32(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::uint32_t>(data.at(at)) |
           (static_cast<std::uint32_t>(data.at(at + 1)) << 8) |
           (static_cast<std::uint32_t>(data.at(at + 2)) << 16) |
           (static_cast<std::uint32_t>(data.at(at + 3)) << 24);
}

std::uint16_t read_le16(const std::vector<std::uint8_t>& data, std::size_t at) {
    return static_cast<std::uint16_t>(data.at(at) | (data.at(at + 1) << 8));
}

}  // namespace

// Strict RIFF/WAVE walk: PCM 16-bit only, which is what every shipped
// sounds/*.WAV is (analysis/rea/LIBRARY_STRATEGY.md §5).
WavClip parse_wav(const std::vector<std::uint8_t>& data, const std::string& path) {
    if (data.size() < 12 || std::memcmp(data.data(), "RIFF", 4) != 0 ||
        std::memcmp(data.data() + 8, "WAVE", 4) != 0) {
        throw std::runtime_error("not a RIFF/WAVE file: " + path);
    }
    WavClip clip;
    std::size_t at = 12;
    int bits = 0;
    int format = 0;
    std::size_t data_at = 0;
    std::size_t data_size = 0;
    while (at + 8 <= data.size()) {
        const std::string id(reinterpret_cast<const char*>(data.data() + at), 4);
        const auto size = static_cast<std::size_t>(read_le32(data, at + 4));
        const std::size_t body = at + 8;
        if (body + size > data.size()) throw std::runtime_error("truncated WAVE chunk: " + path);
        if (id == "fmt ") {
            if (size < 16) throw std::runtime_error("short fmt chunk: " + path);
            format = read_le16(data, body);
            clip.channels = read_le16(data, body + 2);
            clip.sample_rate = static_cast<int>(read_le32(data, body + 4));
            bits = read_le16(data, body + 14);
        } else if (id == "data") {
            data_at = body;
            data_size = size;
        }
        at = body + size + (size & 1);
    }
    if (format != 1) throw std::runtime_error("WAV is not PCM: " + path);
    if (bits != 16) throw std::runtime_error("WAV is not 16-bit: " + path);
    if (clip.channels < 1 || clip.channels > 2 || clip.sample_rate <= 0) {
        throw std::runtime_error("unsupported WAV channel layout: " + path);
    }
    if (data_size == 0) throw std::runtime_error("WAV has no samples: " + path);
    clip.bytes.assign(data.begin() + static_cast<std::ptrdiff_t>(data_at),
                      data.begin() + static_cast<std::ptrdiff_t>(data_at + data_size));
    const auto frames = data_size / (static_cast<std::size_t>(clip.channels) * 2);
    clip.duration_ms =
        static_cast<std::uint64_t>(frames) * 1000 / static_cast<std::uint64_t>(clip.sample_rate);
    return clip;
}

namespace {

// Signed 16-bit PCM word at `frame`, channel `channel` of `clip`.
std::int16_t pcm_word(const WavClip& clip, std::size_t frame, int channel) {
    const auto at = (frame * static_cast<std::size_t>(clip.channels) +
                     static_cast<std::size_t>(channel)) * 2;
    return static_cast<std::int16_t>(
        static_cast<std::uint16_t>(clip.bytes[at]) |
        (static_cast<std::uint16_t>(clip.bytes[at + 1]) << 8));
}

struct Level {
    int peak = 0;
    double rms = 0.0;
};

Level level_of(const WavClip& clip, std::size_t first, std::size_t last) {
    Level level;
    if (last <= first) return level;
    double squares = 0.0;
    std::size_t count = 0;
    for (std::size_t frame = first; frame < last; ++frame) {
        for (int channel = 0; channel < clip.channels; ++channel) {
            const auto value = pcm_word(clip, frame, channel);
            const auto magnitude = std::abs(static_cast<int>(value));
            if (magnitude > level.peak) level.peak = magnitude;
            squares += static_cast<double>(value) * value;
            ++count;
        }
    }
    if (count != 0) level.rms = std::sqrt(squares / static_cast<double>(count));
    return level;
}

}  // namespace

AudioTailTrim trim_torn_audio_tail(WavClip& clip) {
    AudioTailTrim result;
    if (clip.channels < 1 || clip.sample_rate <= 0) return result;
    const auto frame_size = static_cast<std::size_t>(clip.channels) * 2;
    const auto frames = clip.bytes.size() / frame_size;
    const auto rate = static_cast<std::size_t>(clip.sample_rate);
    if (frames < rate / 4) return result;  // too short to judge
    constexpr double kFullScale = 32767.0;
    // 1. A ragged single-sample sign flip (both sides at least 0.45 FS) in the
    // last 200 ms. A torn buffer starts with one; real audio rarely does.
    const auto window_start = frames - std::min(frames, rate * 2 / 10);
    int flip_at = -1;
    int flip_delta = 0;
    for (std::size_t frame = window_start + 1; frame < frames; ++frame) {
        for (int channel = 0; channel < clip.channels; ++channel) {
            const auto previous = static_cast<int>(pcm_word(clip, frame - 1, channel));
            const auto current = static_cast<int>(pcm_word(clip, frame, channel));
            if ((previous > 0) == (current > 0)) continue;
            if (std::abs(previous) < 0.45 * kFullScale) continue;
            if (std::abs(current) < 0.45 * kFullScale) continue;
            const int delta = std::abs(current - previous);
            if (delta > flip_delta) {
                flip_delta = delta;
                flip_at = static_cast<int>(frame);
            }
        }
    }
    if (flip_at < 0) return result;
    const auto at = static_cast<std::size_t>(flip_at);
    // 2. Where the burst starts: the flip sits a few samples inside it, so walk
    // back to the last 10 ms window that is quiet. No quiet lead-in means the
    // clip simply ends loudly (a slam, music) and must be left alone.
    const auto step = std::max<std::size_t>(1, rate / 1000);
    const auto window = std::max<std::size_t>(1, rate / 100);
    const auto quiet = 0.06 * kFullScale;
    std::size_t burst_start = 0;
    bool found = false;
    for (std::size_t frame = at; frame > window && at - frame < rate / 5;
         frame -= step) {
        if (level_of(clip, frame - window, frame).rms <= quiet) {
            burst_start = frame;
            found = true;
            break;
        }
    }
    if (!found) return result;
    if (frames - burst_start > rate * 15 / 100) return result;  // not a tail
    // 3. The burst itself is near full scale and the audio before it is quiet.
    const auto after =
        level_of(clip, burst_start, std::min(frames, burst_start + rate * 6 / 100));
    const auto before =
        level_of(clip, burst_start > rate / 10 ? burst_start - rate / 10 : 0, burst_start);
    if (after.peak < 0.90 * kFullScale || after.rms < 0.35 * kFullScale) return result;
    if (before.peak > 0.60 * kFullScale || before.rms > 0.12 * kFullScale) return result;
    result.at_ms = static_cast<std::uint64_t>(at) * 1000 / rate;
    result.trimmed_at_ms = static_cast<std::uint64_t>(burst_start) * 1000 / rate;
    result.removed_ms = static_cast<std::uint64_t>(frames - burst_start) * 1000 / rate;
    result.peak = after.peak;
    clip.bytes.resize(burst_start * frame_size);
    clip.duration_ms = result.trimmed_at_ms;
    result.trimmed = true;
    return result;
}

AudioMixer::AudioMixer() = default;
AudioMixer::~AudioMixer() {
    if (device_ != 0) {
        SDL_PauseAudioDevice(device_, 1);
        SDL_CloseAudioDevice(device_);
    }
}

void AudioMixer::callback(void* user, std::uint8_t* stream, int length) {
    auto* mixer = static_cast<AudioMixer*>(user);
    const int frames = length / static_cast<int>(sizeof(std::int16_t) * 2);
    mixer->mix(reinterpret_cast<std::int16_t*>(stream), frames);
}

void AudioMixer::open() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        throw std::runtime_error(std::string("SDL audio init failed: ") + SDL_GetError());
    }
    SDL_AudioSpec wanted{};
    wanted.freq = kDeviceRate;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = 2;
    wanted.samples = 1024;
    wanted.callback = &AudioMixer::callback;
    wanted.userdata = this;
    SDL_AudioSpec obtained{};
    device_ = SDL_OpenAudioDevice(nullptr, 0, &wanted, &obtained, 0);
    if (device_ == 0) {
        throw std::runtime_error(std::string("no audio device: ") + SDL_GetError());
    }
    rate_ = obtained.freq;
    channels_ = obtained.channels;
    SDL_PauseAudioDevice(device_, 0);
    name_ = "SDL2 mixer " + std::to_string(rate_) + "Hz " + std::to_string(channels_) + "ch";
    std::cout << "audio device freq=" << rate_ << " format=0x" << std::hex
              << static_cast<int>(obtained.format) << std::dec << " channels=" << channels_
              << " samples=" << obtained.samples << " mixer=callback\n";
}

std::vector<std::int16_t> AudioMixer::convert(const WavClip& clip) const {
    if (device_ == 0) return {};
    SDL_AudioStream* stream = SDL_NewAudioStream(
        AUDIO_S16SYS, static_cast<Uint8>(clip.channels), clip.sample_rate, AUDIO_S16SYS, 2,
        rate_);
    if (stream == nullptr) throw std::runtime_error(SDL_GetError());
    if (SDL_AudioStreamPut(stream, clip.bytes.data(), static_cast<int>(clip.bytes.size())) != 0) {
        const std::string error = SDL_GetError();
        SDL_FreeAudioStream(stream);
        throw std::runtime_error("audio stream put failed: " + error);
    }
    SDL_AudioStreamFlush(stream);
    const int available = SDL_AudioStreamAvailable(stream);
    std::vector<std::int16_t> pcm(
        static_cast<std::size_t>(std::max(0, available)) / sizeof(std::int16_t));
    if (available > 0) {
        SDL_AudioStreamGet(stream, pcm.data(), available);
    }
    SDL_FreeAudioStream(stream);
    return pcm;
}

void AudioMixer::queue(const WavClip& clip) {
    if (device_ == 0) return;
    auto pcm = convert(clip);
    if (pcm.empty()) return;
    Voice voice;
    voice.id = 0;
    voice.tagged = false;
    voice.loop = false;
    voice.pcm = std::move(pcm);
    SDL_LockAudioDevice(device_);
    voices_.push_back(std::move(voice));
    SDL_UnlockAudioDevice(device_);
}

std::uint32_t AudioMixer::queued() const {
    if (device_ == 0) return 0;
    SDL_LockAudioDevice(device_ == 0 ? 0 : device_);
    const auto bytes = remaining_untagged_bytes();
    SDL_UnlockAudioDevice(device_);
    return static_cast<std::uint32_t>(bytes);
}

std::size_t AudioMixer::remaining_untagged_bytes() const {
    std::size_t bytes = 0;
    for (const auto& voice : voices_) {
        if (voice.tagged || voice.finished) continue;
        if (voice.position >= voice.pcm.size()) continue;
        bytes += (voice.pcm.size() - voice.position) * sizeof(std::int16_t);
    }
    return bytes;
}

bool AudioMixer::start(std::uint16_t id, const WavClip& clip, bool loop) {
    if (device_ == 0) return false;
    auto pcm = convert(clip);
    if (pcm.empty()) return false;
    Voice voice;
    voice.id = id;
    voice.tagged = true;
    voice.loop = loop;
    voice.pcm = std::move(pcm);
    SDL_LockAudioDevice(device_);
    voices_.erase(std::remove_if(voices_.begin(), voices_.end(),
                                 [id](const Voice& existing) {
                                     return existing.tagged && existing.id == id;
                                 }),
                  voices_.end());
    voices_.push_back(std::move(voice));
    SDL_UnlockAudioDevice(device_);
    return true;
}

void AudioMixer::stop(std::uint16_t id) {
    if (device_ == 0) return;
    SDL_LockAudioDevice(device_);
    voices_.erase(std::remove_if(voices_.begin(), voices_.end(),
                                 [id](const Voice& voice) {
                                     return voice.tagged && voice.id == id;
                                 }),
                  voices_.end());
    SDL_UnlockAudioDevice(device_);
}

void AudioMixer::stop_all() {
    if (device_ == 0) return;
    SDL_LockAudioDevice(device_);
    voices_.clear();
    SDL_UnlockAudioDevice(device_);
}

bool AudioMixer::playing(std::uint16_t id) const {
    if (device_ == 0) return false;
    SDL_LockAudioDevice(device_);
    bool playing = false;
    for (const auto& voice : voices_) {
        if (voice.tagged && voice.id == id && !voice.finished) playing = true;
    }
    SDL_UnlockAudioDevice(device_);
    return playing;
}

std::size_t AudioMixer::active() const {
    if (device_ == 0) return 0;
    SDL_LockAudioDevice(device_);
    std::size_t count = 0;
    for (const auto& voice : voices_) {
        if (!voice.finished) ++count;
    }
    SDL_UnlockAudioDevice(device_);
    return count;
}

std::size_t AudioMixer::ambient() const {
    if (device_ == 0) return 0;
    SDL_LockAudioDevice(device_);
    std::size_t count = 0;
    for (const auto& voice : voices_) {
        if (voice.tagged && !voice.finished) ++count;
    }
    SDL_UnlockAudioDevice(device_);
    return count;
}

std::string AudioMixer::describe() const {
    return "voices=" + std::to_string(active()) + " ambient=" + std::to_string(ambient());
}

void AudioMixer::mix(std::int16_t* out, int frames) {
    SDL_memset(out, 0, static_cast<std::size_t>(frames) * 2 * sizeof(std::int16_t));
    for (auto& voice : voices_) {
        if (voice.finished) continue;
        for (int frame = 0; frame < frames; ++frame) {
            if (voice.position + 1 >= voice.pcm.size()) {
                if (voice.loop && !voice.pcm.empty()) {
                    voice.position = 0;
                } else {
                    voice.finished = true;
                    break;
                }
            }
            const auto left = static_cast<std::int32_t>(out[frame * 2]) +
                              voice.pcm[voice.position];
            const auto right = static_cast<std::int32_t>(out[frame * 2 + 1]) +
                               voice.pcm[voice.position + 1];
            out[frame * 2] = static_cast<std::int16_t>(std::clamp(left, -32768, 32767));
            out[frame * 2 + 1] = static_cast<std::int16_t>(std::clamp(right, -32768, 32767));
            voice.position += 2;
        }
    }
}

}  // namespace byzantine
