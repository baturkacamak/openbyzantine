#pragma once

#include "byzantine/cc.hpp"
#include <cstring>
#include <stdexcept>
#include <tuple>
#include <type_traits>

namespace byzantine {
// Port-owned companion, never an extension of or reinterpretation of BTB.
// It preserves host context the original file cannot represent (examine
// parent, exact camera, RNG and conversation/selection state).
struct RuntimeViewSave {
    std::string scene, background, committed_background;
    std::optional<std::pair<double,double>> camera;
    std::uint8_t layer=0;
    std::optional<std::uint16_t> examine_image;
    std::map<std::uint16_t,std::uint16_t> variables;
    std::vector<std::uint16_t> variable_writes;
    std::map<std::uint16_t,bool> enabled, suspended;
    std::map<std::uint16_t,ConversationMemory> conversation;
    std::optional<std::uint16_t> using_item, selected;
    std::optional<std::uint32_t> random, morris_seed;
    MorrisBoard morris;
    bool input_active=false;
};
struct RuntimeSave {
    std::uint32_t version=1;
    // Full byte identity binds the companion to precisely this BTB, including
    // unknown blocks. A stale/cross-save companion must never be applied.
    std::vector<std::uint8_t> btb;
    RuntimeViewSave view;
    std::optional<std::uint16_t> closeup;
    std::vector<std::uint8_t> parent_btb;
    RuntimeViewSave parent;
};

#define BYZ_RUNTIME_FIELDS(Type, ...) inline auto runtime_fields(Type& v) { return std::tie(__VA_ARGS__); }
BYZ_RUNTIME_FIELDS(ConversationEntryKey,v.list,v.group,v.hop,v.sub)
BYZ_RUNTIME_FIELDS(ConversationSlotValue,v.byte,v.entry)
BYZ_RUNTIME_FIELDS(ConversationMemory,v.unspeak,v.disabled,v.line_override,v.line_override_flag,
    v.skip_a,v.skip_b,v.skip_c,v.slot_2a,v.slot_5c,v.slot_8e,v.default_reply,v.default_line,
    v.default_reply_pending,v.done)
BYZ_RUNTIME_FIELDS(MorrisBoard::Node,v.peers,v.score,v.flags)
BYZ_RUNTIME_FIELDS(MorrisBoard,v.nodes,v.turns,v.initialized)
BYZ_RUNTIME_FIELDS(RuntimeViewSave,v.scene,v.background,v.committed_background,v.camera,v.layer,v.examine_image,
    v.variables,v.variable_writes,v.enabled,v.suspended,v.conversation,v.using_item,
    v.selected,v.random,v.morris_seed,v.morris,v.input_active)
BYZ_RUNTIME_FIELDS(RuntimeSave,v.version,v.btb,v.view,v.closeup,v.parent_btb,v.parent)
#undef BYZ_RUNTIME_FIELDS

// Small bounded little-endian archive. No compiler layouts, pointers or
// absolute asset paths enter the companion; C++17 hosts can exchange it.
class RuntimeArchive {
    bool reading_;
    std::vector<std::uint8_t>& bytes_;
    std::size_t at_=0;
    std::size_t elements_=0;
public:
    RuntimeArchive(bool reading,std::vector<std::uint8_t>& bytes):reading_(reading),bytes_(bytes){}
    template<class T,std::enable_if_t<std::is_integral<T>::value,int> =0>
    void io(T& value) {
        constexpr std::size_t width=std::is_same<T,bool>::value ? 1 : sizeof(T);
        std::uint64_t word=static_cast<std::uint64_t>(value);
        if(reading_) {
            if(width>bytes_.size()-at_)throw std::runtime_error("runtime save truncated");
            word=0;for(std::size_t i=0;i<width;++i)word|=std::uint64_t(bytes_[at_++])<<(i*8);
            if constexpr(std::is_same<T,bool>::value)if(word>1)throw std::runtime_error("runtime save boolean");
            value=static_cast<T>(word);
        } else for(std::size_t i=0;i<width;++i)bytes_.push_back(static_cast<std::uint8_t>(word>>(i*8)));
    }
    void io(double& value) {std::uint64_t bits=0;if(!reading_)std::memcpy(&bits,&value,8);io(bits);if(reading_)std::memcpy(&value,&bits,8);}
    std::uint32_t count(std::size_t size) {
        std::uint32_t n=static_cast<std::uint32_t>(size);io(n);
        elements_+=n;
        if(n>1000000 || elements_>2000000)throw std::runtime_error("runtime save count exceeds limit");
        return n;
    }
    void io(std::string& value) {auto n=count(value.size());if(reading_)value.resize(n);for(char& c:value)io(c);}
    template<class T> void io(std::vector<T>& v) {auto n=count(v.size());if(reading_)v.resize(n);for(auto& x:v)io(x);}
    template<class T,std::size_t N> void io(std::array<T,N>& v) {for(auto& x:v)io(x);}
    template<class T> void io(std::optional<T>& v) {bool exists=bool(v);io(exists);if(reading_){if(exists)v.emplace();else v.reset();}if(exists)io(*v);}
    template<class A,class B> void io(std::pair<A,B>& v) {io(v.first);io(v.second);}
    template<class K,class V> void io(std::map<K,V>& v) {
        auto n=count(v.size());if(n>4096)throw std::runtime_error("runtime save map exceeds limit");if(reading_) {v.clear();for(std::uint32_t i=0;i<n;++i){K k{};V value{};io(k);io(value);if(!v.emplace(k,std::move(value)).second)throw std::runtime_error("runtime save duplicate key");}}
        else for(auto& item:v){K k=item.first;io(k);io(item.second);}
    }
    template<class T> void io(std::set<T>& v) {
        auto n=count(v.size());if(reading_){v.clear();for(std::uint32_t i=0;i<n;++i){T x{};io(x);v.insert(x);}}
        else for(auto x:v)io(x);
    }
    template<class T> auto io(T& v)->decltype(runtime_fields(v),void()) {
        std::apply([&](auto&... fields){(io(fields),...);},runtime_fields(v));
    }
    void finish() {if(reading_ && at_!=bytes_.size())throw std::runtime_error("runtime save trailing bytes");}
};
inline std::vector<std::uint8_t> write_runtime_save(RuntimeSave value) {
    std::vector<std::uint8_t> bytes;RuntimeArchive archive(false,bytes);archive.io(value);return bytes;
}
inline RuntimeSave read_runtime_save(std::vector<std::uint8_t> bytes) {
    if(bytes.size()>16000000)throw std::runtime_error("runtime save too large");
    RuntimeSave value;RuntimeArchive archive(true,bytes);archive.io(value.version);
    if(value.version!=1)throw std::runtime_error("unsupported runtime save version");
    // Read the remainder from the same cursor, without reading version twice.
    archive.io(value.btb);archive.io(value.view);archive.io(value.closeup);
    archive.io(value.parent_btb);archive.io(value.parent);archive.finish();return value;
}
} // namespace byzantine
