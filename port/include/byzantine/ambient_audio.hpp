#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

namespace byzantine {

// One strict RIFF/WAVE clip (PCM16, mono/stereo). The parser is the same one the
// conversation AVI audio used; moving it here gives the script sounds and the
// video clips one path.
struct WavClip {
    std::vector<std::uint8_t> bytes;
    int channels = 0;
    int sample_rate = 0;
    std::uint64_t duration_ms = 0;
};
WavClip parse_wav(const std::vector<std::uint8_t>& data, const std::string& path);

// A shipped clip can carry a torn final audio buffer. NBEST1.avi (the Nargile
// Kahvesi site video on every CD, ISO bytes verified) ends with 67 ms of
// near-full-scale noise immediately after the picture has faded to black: the
// last 16-bit words are not audio, they are a jagged half-scale sign flip
// followed by a burst that runs to the clip's end. Played as PCM it is a loud
// crack. `trim_torn_audio_tail` removes such a tail and reports it; the test is
// deliberately narrow so ordinary loud endings (music, a slam) survive:
//
//  * a single-sample sign flip from <= -0.45 FS to >= +0.45 FS (or the reverse)
//    inside the last 200 ms,
//  * the 60 ms after it at >= 0.9 FS peak and >= 0.35 FS RMS,
//  * the 100 ms before it quiet (peak <= 0.6 FS, RMS <= 0.12 FS).
//
// The caller logs the trim; nothing is removed silently. `--no-audio-tail-guard`
// disables it for an A/B against the unmodified stream.
struct AudioTailTrim {
    bool trimmed = false;
    std::uint64_t at_ms = 0;         // the detected discontinuity
    std::uint64_t trimmed_at_ms = 0; // where the clip now ends
    std::uint64_t removed_ms = 0;
    int peak = 0;
};
AudioTailTrim trim_torn_audio_tail(WavClip& clip);

// The common SDL2 audio mixer. Script sounds (0x65 records) become tagged voices:
// `0xc4` starts/loops an ambient record without blocking the script, `0xbc`/
// `0xcf`/`0xd0` stop it, and a `0xd1` dialogue voice is waited on only for the
// interactive mode. Conversation AVI audio keeps the untagged `queue`/`queued`
// behaviour so its drain loop is unchanged. The device callback mixes every
// voice; there is no per-sound SDL_QueueAudio.
enum class VoiceBus { speech, ambient, effects };

class AudioMixer {
public:
    AudioMixer();
    ~AudioMixer();
    AudioMixer(const AudioMixer&) = delete;
    AudioMixer& operator=(const AudioMixer&) = delete;

    // Throws std::runtime_error when SDL cannot open the device.
    void open();
    bool ready() const { return device_ != 0; }
    const std::string& device_name() const { return name_; }

    // Conversation/voice clip: one-shot, not tagged with a script id.
    void queue(const WavClip& clip, VoiceBus bus = VoiceBus::speech);
    // Pending bytes of untagged (queued) voices, the old `queued()` contract.
    std::uint32_t queued() const;

    // Script record `id`: replace any existing voice for the same id. `loop`
    // comes from DAT_00486220[id]+0x14 (0x427de1's Play(...,1)).
    bool start(std::uint16_t id, const WavClip& clip, bool loop,
               VoiceBus bus = VoiceBus::effects);
    void stop(std::uint16_t id);
    void stop_all();
    bool playing(std::uint16_t id) const;
    std::size_t active() const;
    std::size_t ambient() const;  // tagged voices
    std::string describe() const;

    // Host-only gains. They scale the mixer output; they never change the
    // script's sound state or the queued-byte contract. Values are clamped to
    // 0..1 and safe to call while the audio callback runs.
    void set_volumes(float master, float speech, float ambient, float effects);
    void set_muted(bool muted);

private:
    struct Voice {
        std::uint16_t id = 0;   // 0 = untagged conversation queue
        std::vector<std::int16_t> pcm;  // interleaved stereo at the device rate
        std::size_t position = 0;       // in samples (frames*2)
        bool loop = false;
        bool tagged = false;
        bool finished = false;
        VoiceBus bus = VoiceBus::speech;
    };

    static void callback(void* user, std::uint8_t* stream, int length);
    void mix(std::int16_t* out, int frames);
    std::vector<std::int16_t> convert(const WavClip& clip) const;
    std::size_t remaining_untagged_bytes() const;

    unsigned int device_ = 0;
    int rate_ = 0;
    int channels_ = 2;
    std::string name_ = "closed";
    std::vector<Voice> voices_;
    std::atomic<float> master_gain_{1.0f};
    std::atomic<float> speech_gain_{1.0f};
    std::atomic<float> ambient_gain_{1.0f};
    std::atomic<float> effects_gain_{1.0f};
    std::atomic<bool> muted_{false};
};

}  // namespace byzantine
