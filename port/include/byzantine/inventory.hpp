#pragma once

#include "byzantine/cc.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace byzantine {

// The original bottom bar is the inventory strip: six visible entries at
// screen x=160+60*i, y=426, page arrows and the examine button. The save's
// block 8..12 already carries the data; 0x407420 draws it and 0x408076 handles
// a click. These helpers keep that model in one place so the player and the
// script opcodes read the same state.
constexpr std::size_t kInventoryPages = 10;
constexpr std::size_t kInventoryVisible = 6;

struct InventoryPage {
    std::size_t page = 0;
    std::size_t scroll = 0;
    std::vector<std::uint16_t> items;
};

InventoryPage inventory_page(const SceneState& state);
bool inventory_select(SceneState& state, std::uint16_t id);
bool inventory_on_page(const SceneState& state, std::uint16_t id);
void inventory_scroll(SceneState& state, int delta);
bool inventory_mark_using(SceneState& state, std::uint16_t id);
std::optional<std::uint16_t> inventory_using(const SceneState& state);
std::string inventory_name(const SceneState& state, std::uint16_t id);

// 0x407c5a. The object's kind (+0x09) and name/target id (+0x10) decide whether
// the currently selected inventory record (DAT_0045d1c8) is acceptable. -1
// means the object has no name id. Kinds 0x00/0x01/0x02/0x05/0x08/0x09/0x0c
// accept anything when they carry no id (this is how exits work), 0x0a/0x0b/
// 0x0d/0x0e/0x10 only accept "no item selected", 0x03/0x04/0x06/0x07 and
// 0x0f/0x11/0x18..0x1b/0x1d/0x1e never accept one, 0x13/0x14 search a dialogue
// chain, and everything else must name the selected record.
bool item_use_compatible(const SceneState& state, const SceneObjectState& object,
                         std::optional<std::uint16_t> selected);

// 0x438afa/0x438173: the original maps a scene to one of the five close-up
// scenes before opening the item image. The table is recovered data, not a
// per-scene port special case. "va" reads index 5 of a five-entry table in the
// original and is therefore left unresolved instead of guessing.
std::optional<std::string> examine_scene_for(std::string scene);

// 0x40860e: the first eight characters of the item name, with the first
// character replaced by 'e', name the inventor/examine/<stem>.jpg image.
std::string examine_image_stem(std::string item_name);

}  // namespace byzantine
