#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>

namespace byzantine { namespace diagnostics {

// Host diagnostics only; never part of the reconstructed game's state.
// Configured by run_logged.py, or explicitly by direct-player callers.
struct Sink {
    std::ofstream file;
    std::mutex mutex;
    std::uint64_t sequence = 0;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    Sink() {
        const char* path = std::getenv("BYZ_TRACE_LOG");
        if (path && *path) {
            file.open(path, std::ios::app);
            if (!file) std::cerr << "diagnostic ERROR cannot open trace log: " << path << '\n';
        }
    }
};
inline Sink& sink() { static Sink value; return value; }
template<class Writer> void write(const char* category, Writer writer) {
    auto& target = sink();
    if (!target.file.is_open()) return;
    std::lock_guard<std::mutex> lock(target.mutex);
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - target.start).count();
    target.file << "seq=" << ++target.sequence << " elapsed_ms=" << ms << ' ' << category << ' ';
    writer(target.file);
    target.file << '\n';
    target.file.flush();
    if (!target.file) {
        std::cerr << "diagnostic ERROR trace log write failed\n";
        target.file.close();
    }
}
inline std::uint64_t evaluation_id() {
    static std::atomic<std::uint64_t> next{0};
    return ++next;
}

}} // namespace byzantine::diagnostics
