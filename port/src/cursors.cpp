#include "byzantine/cursors.hpp"

#include "byzantine/inventory.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <utility>
#include <vector>

namespace byzantine {
namespace {

std::string lower_ascii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return value;
}

constexpr std::size_t kCursorCount = 0x20;

// BETRAYAL.EXE 0x45dbe8: the table 0x40dd7f loads at startup ("cursors/<name>
// *.tga"). The hotspot pair is what 0x40d88a/0x40df05 subtract from the mouse
// position before 0x40e29f blits the pointer.
constexpr std::array<CursorAsset, kCursorCount> kNormalCursors{{
    {0, "default", 24, 24},
    {1, "forward", 14, 14},
    {2, "back", 13, 8},
    {3, nullptr, 0, 0},
    {4, nullptr, 0, 0},
    {5, "examup", 16, 12},
    {6, nullptr, 0, 0},
    {7, nullptr, 0, 0},
    {8, "forward", 14, 14},
    {9, "examdwn", 16, 24},
    {10, "talk", 16, 16},
    {11, "exit", 14, 18},
    {12, "magnify", 15, 14},
    {13, "take", 19, 12},
    {14, "manplte", 7, 13},
    {15, "mouseoff", 0, 0},
    {16, "video", 16, 16},
    {17, "timer", 16, 16},
    {18, "apply", 11, 11},
    {19, "apply", 11, 11},
    {20, "talk", 16, 16},
    {21, "info", 24, 20},
    {22, "diminfo", 24, 20},
    {23, "default", 24, 24},
    {24, "up", 11, 24},
    {25, "down", 11, 11},
    {26, "left", 24, 12},
    {27, "right", 11, 12},
    {28, "hotbig", 24, 24},
    {29, "map", 11, 11},
    {30, "hotsmall", 11, 11},
    {31, "sload", 16, 16},
}};

// BETRAYAL.EXE 0x45e090: 0x42dad9's alternate set ("cursors/klio/<name>*.tga").
// Slots 10 (talk), 16 (video) and 20 (talk) carry no name on the shipped CD;
// 0x40df05 would dereference their empty frame pointers, so the player falls
// back to the normal table for those ids instead of inventing art.
constexpr std::array<CursorAsset, kCursorCount> kKlioCursors{{
    {0, "default", 24, 24},
    {1, "forward", 20, 20},
    {2, "back", 24, 24},
    {3, nullptr, 0, 0},
    {4, nullptr, 0, 0},
    {5, "examup", 14, 14},
    {6, nullptr, 0, 0},
    {7, nullptr, 0, 0},
    {8, "forward", 20, 20},
    {9, "examdwn", 14, 14},
    {10, nullptr, 16, 16},
    {11, "exit", 24, 24},
    {12, "magnify", 14, 14},
    {13, "take", 6, 2},
    {14, "manplte", 6, 0},
    {15, "mouseoff", 0, 0},
    {16, nullptr, 16, 16},
    {17, "timer", 11, 13},
    {18, "apply", 24, 24},
    {19, "apply", 24, 24},
    {20, nullptr, 16, 16},
    {21, "info", 19, 19},
    {22, "diminfo", 19, 19},
    {23, "default", 24, 24},
    {24, "up", 24, 24},
    {25, "down", 24, 24},
    {26, "left", 24, 24},
    {27, "right", 24, 24},
    {28, "hotbig", 24, 24},
    {29, "map", 11, 11},
    {30, "hotsmall", 11, 11},
    {31, "sload", 16, 16},
}};

// The inventory strip geometry 0x407420 draws and 0x4083d4 hit-tests:
// six 0x3c-wide slots from x=0xa0, icons at strip row +6, examine button
// x=2..0x3a and page arrows x=0x23b..0x259 / 0x25d..0x27b.
constexpr int kStripTop = 0x1a4;
constexpr int kScreenHeight = 0x1e0;
constexpr int kSlotFirstX = 0xa0;
constexpr int kSlotPitch = 0x3c;
constexpr int kSlotTop = kStripTop + 6;

bool in_rect(int x, int y, int left, int top, int right, int bottom) {
    return x >= left && x <= right && y >= top && y <= bottom;
}

CursorChoice table_choice(std::uint8_t id) {
    CursorChoice choice;
    choice.kind = CursorChoice::Kind::table;
    choice.id = id;
    return choice;
}

CursorChoice item_choice(std::uint16_t id, bool hotitem) {
    CursorChoice choice;
    choice.kind = CursorChoice::Kind::item;
    choice.id = id;
    choice.hotitem = hotitem;
    return choice;
}

}  // namespace

CursorTheme cursor_theme(std::int32_t alternate_mode) {
    // 0x40dae5 compares its argument with -1; every other value selects the
    // klio folder. 0x42c8a0 leaves DAT_00464878 at -1 before the scene loop.
    return alternate_mode == -1 ? CursorTheme::normal : CursorTheme::klio;
}

const CursorAsset& cursor_asset(CursorTheme theme, std::uint8_t id) {
    static const CursorAsset kEmpty{0xff, nullptr, 0, 0};
    if (id >= kCursorCount) return kEmpty;
    return theme == CursorTheme::normal ? kNormalCursors[id] : kKlioCursors[id];
}

std::uint8_t cursor_slot(CursorTheme theme, bool panorama_active, std::uint8_t id) {
    if (theme != CursorTheme::normal || panorama_active || id >= kCursorCount) return id;
    switch (id) {
    case 0x00:
    case 0x17:
        return 0x1d;  // "map"
    case 0x1c:
        return 0x1e;  // "hotsmall"
    default:
        return id;
    }
}

CursorChoice hover_cursor(const SceneState& state, const SceneObjectState* object) {
    const auto& selected = state.current_dialogue_record;
    if (object == nullptr) {
        if (selected) return item_choice(*selected, false);
        return table_choice(0);
    }
    const std::uint8_t kind = object->kind;
    if (selected) {
        // 0x40c329 calls 0x407c5a on every hover; only the branches inside
        // `if (DAT_0045d1c8 != 0)` use the answer.
        const bool compatible = item_use_compatible(state, *object, selected);
        if (!compatible) return item_choice(*selected, false);
        if (kind == 0x12 || kind == 0x13 || kind == 0x14) {
            // 0x407bd7(1): the held item's hotitem art over a use hotspot.
            return item_choice(*selected, true);
        }
        if (kind == 0x00) return item_choice(*selected, false);
    }
    // Every other case keeps the object's kind byte as the cursor id. A kind
    // above the table's 0x20 entries is not produced by the shipped 0x72/0x95
    // writes; the original would index it as an inventory record, so the port
    // keeps the default pointer instead of guessing.
    return table_choice(kind < kCursorCount ? kind : 0);
}

CursorChoice strip_cursor(const SceneState& state, int x, int y) {
    if (!state.dialogue_visible || y < kStripTop || y > kScreenHeight) return table_choice(0);
    const auto held = state.current_dialogue_record;
    if (held) {
        // 0x4083d4: the item's own art is the pointer; over the channel area
        // (x 0x46..0x8c) or the examine button, with the item on the current
        // page, 0x407bd7(1) switches to its hotitem variant.
        const bool channel_region = in_rect(x, y, 0x46, kStripTop, 0x8c, kScreenHeight);
        const bool examine_button = in_rect(x, y, 2, 0x1a8, 0x3a, 0x1dc);
        const bool hotitem = (channel_region || examine_button) &&
                             inventory_on_page(state, *held);
        return item_choice(*held, hotitem);
    }
    const auto page = inventory_page(state);
    const bool next_arrow = in_rect(x, y, 0x23b, 0x1b1, 0x259, 0x1cb);
    const bool previous_arrow = in_rect(x, y, 0x25d, 0x1b1, 0x27b, 0x1cb);
    if (next_arrow || previous_arrow) {
        const auto& stacks = state.dialogue_channels;
        const std::size_t total =
            stacks.empty() ? 0 : stacks[page.page % stacks.size()].size();
        const std::size_t maximum = total > kInventoryVisible ? total - kInventoryVisible : 0;
        const bool available = next_arrow ? page.scroll < maximum : page.scroll > 0;
        // 0x40de7b(0x1c) -> "hotbig" (or "hotsmall" outside a panorama).
        return table_choice(available ? 0x1c : 0);
    }
    if (y >= kSlotTop) {
        for (std::size_t index = 0; index < page.items.size(); ++index) {
            const int left = kSlotFirstX + static_cast<int>(index) * kSlotPitch;
            if (x < left || x > left + kSlotPitch) continue;
            // The slot is only a pointer target while it holds something other
            // than the item the examine button selected (0x4083d4's
            // `item != DAT_0045d1cc` guard).
            const auto id = page.items[index];
            if (state.current_use_item && *state.current_use_item == id) break;
            return table_choice(0x1c);
        }
    }
    return table_choice(0);
}

std::optional<CursorFileMatch> find_cursor_file(const std::filesystem::path& directory,
                                                const std::string& name) {
    std::error_code error;
    if (!std::filesystem::is_directory(directory, error)) return std::nullopt;
    const auto wanted = lower_ascii(name);
    // Directory order is filesystem-dependent; sort by stem so the choice is
    // reproducible (the original's FindFirstFile order is not recovered).
    std::vector<std::pair<std::string, std::filesystem::path>> matches;
    for (const auto& entry : std::filesystem::directory_iterator(directory, error)) {
        if (!entry.is_regular_file()) continue;
        if (lower_ascii(entry.path().extension().string()) != ".tga") continue;
        const auto stem = lower_ascii(entry.path().stem().string());
        if (stem != wanted && stem.rfind(wanted, 0) != 0) continue;
        matches.emplace_back(stem, entry.path());
    }
    if (matches.empty()) return std::nullopt;
    std::sort(matches.begin(), matches.end(),
              [](const auto& left, const auto& right) { return left.first < right.first; });
    CursorFileMatch result;
    // 0x40e89f fills at most four frame handles; 0x40ebe1 then advances them
    // every 250 ms (the host applies the timing).
    result.frames = std::min<std::size_t>(matches.size(), 4);
    for (std::size_t index = 0; index < result.frames; ++index) {
        result.paths.push_back(matches[index].second);
    }
    result.path = result.paths.front();
    for (const auto& match : matches) {
        if (match.first != wanted) continue;
        result.path = match.second;
        break;
    }
    return result;
}

std::string hotitem_media_name(std::string item_name) {
    // 0x407254: FUN_0044c630 copies at most eight characters, then the first
    // character is replaced by 'h'.
    if (item_name.size() > 8) item_name.resize(8);
    if (!item_name.empty()) item_name[0] = 'h';
    return item_name;
}

}  // namespace byzantine
