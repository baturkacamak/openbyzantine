#include "byzantine/inventory.hpp"

#include <algorithm>
#include <cctype>

namespace byzantine {
namespace {

std::string lower_ascii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

}  // namespace

InventoryPage inventory_page(const SceneState& state) {
    InventoryPage page;
    if (state.dialogue_channels.empty()) return page;
    page.page = state.current_dialogue_channel % state.dialogue_channels.size();
    const auto& stack = state.dialogue_channels[page.page];
    const std::size_t maximum = stack.size() > kInventoryVisible
                                    ? stack.size() - kInventoryVisible
                                    : 0;
    page.scroll = std::min<std::size_t>(
        static_cast<std::size_t>(std::max<std::int32_t>(0, state.dialogue_scroll)),
        maximum);
    const std::size_t end = std::min(stack.size(), page.scroll + kInventoryVisible);
    page.items.assign(stack.begin() + static_cast<std::ptrdiff_t>(page.scroll),
                      stack.begin() + static_cast<std::ptrdiff_t>(end));
    return page;
}

bool inventory_on_page(const SceneState& state, std::uint16_t id) {
    const auto page = inventory_page(state);
    return std::find(page.items.begin(), page.items.end(), id) != page.items.end();
}

bool inventory_select(SceneState& state, std::uint16_t id) {
    if (!inventory_on_page(state, id)) return false;
    if (state.current_dialogue_record && *state.current_dialogue_record == id) {
        state.current_dialogue_record.reset();
        return true;
    }
    state.current_dialogue_record = id;
    // 0x408076: selecting a new (state 3) entry marks it seen.
    auto& record = state.dialogue_records[id];
    if (record.state == 3) record.state = 1;
    return true;
}

void inventory_scroll(SceneState& state, int delta) {
    if (state.dialogue_channels.empty()) return;
    const auto& stack = state.dialogue_channels[
        state.current_dialogue_channel % state.dialogue_channels.size()];
    const std::int32_t maximum = stack.size() > kInventoryVisible
                                     ? static_cast<std::int32_t>(stack.size() - kInventoryVisible)
                                     : 0;
    state.dialogue_scroll = std::clamp<std::int32_t>(
        state.dialogue_scroll + delta, 0, maximum);
}

bool inventory_mark_using(SceneState& state, std::uint16_t id) {
    if (!inventory_on_page(state, id)) return false;
    // 0x408076: the examine button sets the entry state to 4 and remembers it
    // in DAT_0045d1cc; 0x40860e then derives inventor/examine/<stem>.jpg.
    state.current_use_item = id;
    state.dialogue_records[id].state = 4;
    return true;
}

std::optional<std::uint16_t> inventory_using(const SceneState& state) {
    return state.current_use_item;
}

std::string inventory_name(const SceneState& state, std::uint16_t id) {
    if (id >= state.dialogue_tables.names.size()) return {};
    return state.dialogue_tables.names[id].name;
}

std::optional<std::string> examine_scene_for(std::string scene) {
    scene = lower_ascii(std::move(scene));
    // 0x438afa compares the current scene name; 0x438173 stores the five
    // examine scene indices. These names are the shipped data's own groups.
    if (scene == "vh") return "examvh";
    if (scene == "vb" || scene == "hstmed") return "examvb";
    if (scene == "vc" || scene == "lintel" || scene == "morris") return "examvc";
    if (scene == "vt" || scene == "clock" || scene == "voice" ||
        scene == "star" || scene == "mz2") {
        return "examvt";
    }
    // The original reads index 5 of a five-entry table for "va"; that is an
    // out-of-bounds read with no recovered scene meaning, so do not guess it.
    if (scene == "va") return std::nullopt;
    return "examine";
}

std::string examine_image_stem(std::string item_name) {
    // 0x40860e: FUN_0044c630 copies at most eight characters and then writes
    // param[8] = 0; it replaces the first character with 'e'.
    item_name.resize(std::min<std::size_t>(item_name.size(), 8));
    if (!item_name.empty()) item_name[0] = 'e';
    return item_name;
}

}  // namespace byzantine
