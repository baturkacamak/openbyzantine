#pragma once

#include "byzantine/cc.hpp"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

namespace byzantine {

// The game's own pointer art. 0x40dd7f creates the mouse surfaces and 0x40dae5
// loads 32 named cursors from "cursors\%s*.tga" (0x40dbe8) or, for the
// alternate set, "cursors\klio\%s*.tga" (0x45e090). Each entry stores the
// pointer name at +0x00 and its hotspot at +0x04/+0x08; the remaining fields
// are the up-to-four animation frames (+0x14..+0x20), their count (+0x10) and
// the current frame (+0x0c). The names and hotspots below are the shipped
// binary's table, not a guess: both tables live in BETRAYAL.EXE at the given
// addresses (see analysis/rea/CURSORS.md).
struct CursorAsset {
    std::uint8_t id = 0;
    const char* name = nullptr;  // nullptr: the shipped table leaves this slot empty
    int hotspot_x = 0;           // +0x04
    int hotspot_y = 0;           // +0x08
};

enum class CursorTheme { normal, klio };

// 0x42dad9 -> 0x40dae5: any explicit alternate-mode value switches to the
// klio folder; -1 (the value 0x42c8a0 leaves before the scene loop, and what
// 0x40dd7f loads at startup) is the normal set.
CursorTheme cursor_theme(std::int32_t alternate_mode);

// The 32-entry table of `theme`, indexed by the id the engine passes to
// 0x40df05.
const CursorAsset& cursor_asset(CursorTheme theme, std::uint8_t id);

// 0x40de7b: with the normal set in a non-panorama view (DAT_0045c060 == 0)
// the small variants replace the big ones: 0 and 0x17 become 0x1d ("map") and
// 0x1c becomes 0x1e ("hotsmall"). The klio table is not remapped.
std::uint8_t cursor_slot(CursorTheme theme, bool panorama_active, std::uint8_t id);

// 0x40df05's two sources: an entry of the cursor table, or an inventory record.
// The engine hands it `0x20 + 2*record (+1 = hotitem)` for an item and resolves
// that through DAT_00486f90; the port carries the inventory (SHOW.LCI) id and
// resolves it to inventor/<name>.tga / inventor/hotitems/<h-name>.tga. `id` is
// a cursor-table index for Kind::table and an inventory id for Kind::item.
struct CursorChoice {
    enum class Kind { table, item };
    Kind kind = Kind::table;
    std::uint16_t id = 0;
    // 0x407bd7(1) / record +4 + 1: the item's hotitem art, used when the item
    // in hand hovers a use hotspot (kind 0x12/0x13/0x14) or the strip's
    // examine/channel area.
    bool hotitem = false;
};

// 0x40c329: the pointer for the object under the mouse (nullptr when no active
// object is hit). The hovered object's kind byte (+0x09) is the cursor id, and
// the inventory selection overrides it exactly as the dispatcher does.
CursorChoice hover_cursor(const SceneState& state, const SceneObjectState* object);

// 0x4083d4: the pointer inside the 640x480 inventory band (y >= 0x1a4).
CursorChoice strip_cursor(const SceneState& state, int x, int y);

// 0x407254 truncates the name to eight characters and replaces the first one
// with 'h'; that stem names inventor/hotitems/<stem>.tga.
std::string hotitem_media_name(std::string item_name);

// 0x40e89f on 0x40dae5's "cursors/%s*.tga" pattern: an exact-stem .tga wins,
// otherwise the first .tga whose stem starts with the name (the klio set's
// forward0/1/2, back1..4 and INFO1..4 animation frames). `frames` is the number
// of files the original wildcard would have loaded (at most four).
struct CursorFileMatch {
    std::filesystem::path path;
    std::size_t frames = 0;
};
std::optional<CursorFileMatch> find_cursor_file(const std::filesystem::path& directory,
                                                const std::string& name);

}  // namespace byzantine
