#include "byzantine/menu.hpp"

namespace byzantine {
namespace {

// 0x45f548, 0x28 entries of 16 bytes. Dumped from the shipped BETRAYAL.EXE
// (image base 0x400000, .data file offset 0x5a600); see analysis/rea/TOP_MENU.md.
const std::vector<MenuControlRect>& control_table() {
    static const std::vector<MenuControlRect> table = {
        {0x00, 0x46, 0x10e, 0xd7, 0x122},
        {0x01, 0x46, 0x127, 0xd7, 0x13b},
        {0x02, 0x46, 0x140, 0xd7, 0x154},
        {0x03, 0x46, 0x159, 0xd7, 0x16d},
        {0x04, 0x46, 0x172, 0xd7, 0x186},
        {0x05, 0x46, 0x18b, 0xd7, 0x19f},
        {0x06, 0x172, 0x71, 0x25a, 0x99},
        {0x07, 0x172, 0x9c, 0x25a, 0xc4},
        {0x08, 0x172, 0xc9, 0x25a, 0xf1},
        {0x09, 0x172, 0xf5, 0x25a, 0x11d},
        {0x0a, 0x172, 0x122, 0x25a, 0x14a},
        {0x0b, 0x172, 0x154, 0x25a, 0x17c},
        {0x0c, 0x172, 0x181, 0x25a, 0x1a9},
        {0x0d, 0x44, 0xae, 0x61, 0x127},
        {0x0e, 0x267, 0xb4, 0x27f, 0x118},
        {0x0f, 0x258, 0x23, 0x277, 0x6f},
        {0x10, 0x258, 0x71, 0x279, 0xb7},
        {0x11, 0x172, 0x1a9, 0x258, 0x1df},
        {0x12, 0x17a, 0x42, 0x242, 0x50},
        {0x13, 0x17a, 0x60, 0x242, 0x6e},
        {0x14, 0x17a, 0x7e, 0x242, 0x8c},
        {0x15, 0x17a, 0x9c, 0x242, 0xaa},
        {0x16, 0x17a, 0xba, 0x242, 0xc8},
        {0x17, 0x17a, 0xd8, 0x242, 0xe6},
        {0x18, 0x17a, 0xf6, 0x242, 0x104},
        {0x19, 0x17a, 0x114, 0x242, 0x122},
        {0x1a, 0x17a, 0x132, 0x242, 0x140},
        {0x1b, 0x17a, 0x150, 0x242, 0x15e},
        {0x1c, 0x230, 0x186, 0x25c, 0x19f},
        {0x1d, 0x226, 0x15e, 0x259, 0x185},
        {0x1e, 0x174, 0x000, 0x27f, 0x046},
        {0x1f, 0x163, 0x04e, 0x251, 0x1d5},
        {0x20, 0x172, 0x06c, 0x230, 0x1ac},
        {0x21, 0x019, 0x103, 0x03a, 0x1bc},
        {0x22, 0x052, 0x113, 0x136, 0x1b8},
        {0x23, 0x02b, 0x016, 0x106, 0x14b},
        // 0x24 (the KEŞİF list) has bottom 0 in the image; 0x420b99 fills it
        // at runtime with top + (DAT_0045f7dc - 0x33) * 0x28.
        {0x24, 0x172, 0x04b, 0x22e, 0x000},
        {0x25, 0x239, 0x03f, 0x25c, 0x076},
        {0x26, 0x053, 0x189, 0x08c, 0x1bd},
        {0x27, 0x24e, 0x118, 0x27f, 0x17c},
    };
    return table;
}

}  // namespace

int top_bar_button(int x, int y) {
    if (y < 0 || y >= kTopBarHeight) return 0;
    if (x < 0) x = 0;
    if (x > kTopBarWidth - 1) x = kTopBarWidth - 1;
    int index = 0;
    for (const int boundary : kTopBarBounds) {
        if (boundary <= x) ++index;
        else break;
    }
    return index;  // 1..4
}

int top_bar_menu_mode(int button) {
    switch (button) {
        case 1: return 1;  // YÜKLE/KAYDET
        case 2: return 3;  // NOTLAR
        case 3: return 2;  // KEŞİF
        case 4: return 0;  // TERCİHLER
        default: return -1;
    }
}

const std::vector<MenuControlRect>& menu_control_rects() { return control_table(); }

const MenuControlRect* menu_control_rect(int id) {
    for (const auto& rect : control_table()) {
        if (rect.id == id) return &rect;
    }
    return nullptr;
}

bool menu_static_control_enabled(int mode, int id) {
    // FUN_0041fe46's cases, written as the byte flags it sets.
    switch (mode) {
        case 0:  // preferences: the seven toggles plus quit/continue
            switch (id) {
                case 0x1e:
                case 0x23:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                case 0x08:
                case 0x09:
                case 0x0a:
                case 0x0b:
                case 0x0c:
                    return true;
                default:
                    return false;
            }
        case 1:  // save/load: save, new game, quit, continue and the tabs
            switch (id) {
                case 0x00:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x0e:
                case 0x10:
                case 0x11:
                case 0x27:
                    return true;
                default:
                    return false;
            }
        case 2:  // keşif: quit/continue, the list, the tabs
            switch (id) {
                case 0x04:
                case 0x05:
                case 0x0e:
                case 0x0f:
                case 0x11:
                case 0x1e:
                case 0x24:
                case 0x27:
                    return true;
                default:
                    return false;
            }
        case 3:  // NOTLAR page
            switch (id) {
                case 0x04:
                case 0x05:
                case 0x0e:
                case 0x1e:
                case 0x1f:
                    return true;
                default:
                    return false;
            }
        case 4:  // TEMAS LİSTESİ
            switch (id) {
                case 0x0e:
                case 0x1e:
                case 0x20:
                case 0x21:
                case 0x22:
                    return true;
                default:
                    return false;
            }
        case 5:  // notebook spread
            switch (id) {
                case 0x0e:
                case 0x1d:
                case 0x1e:
                case 0x21:
                    return true;
                default:
                    return false;
            }
        default:
            return false;
    }
}

bool menu_control_enabled(int mode, int id, bool save_row_present,
                          bool save_selected) {
    if (id >= 0x12 && id <= 0x1b) return mode == 1 && save_row_present;
    if (id == 0x01 || id == 0x02) return mode == 1 && save_selected;
    if (id == 0x05 && mode == 1) return !save_selected;
    return menu_static_control_enabled(mode, id);
}

int save_slot_row_at(int x, int y) {
    // 0x4202ff: right-aligned text at x 0x1de, first row y 0x42, step 0x1e;
    // 0x45f548 rows 0x12..0x1b use x 0x17a..0x242 and a 0x0e-tall rectangle.
    // The hit test is `top < y && y < bottom`.
    if (x < 0x17a || x >= 0x242) return -1;
    for (int row = 0; row < 10; ++row) {
        const int top = 0x42 + row * 0x1e;
        if (y <= top || y >= top + 0x0e) continue;
        return row;
    }
    return -1;
}

int kesif_row_at(int x, int y, int status_count) {
    // 0x422101: control 0x24 at x 0x172..0x22e and rows of 0x28 starting 0x4b.
    // It compares `0x4b <= y && y <= 0x73` for the first row, so the boundary
    // row belongs to the earlier entry.
    if (x < 0x172 || x >= 0x22e) return -1;
    for (int row = 0; row <= 0x3a - 0x34; ++row) {
        const int top = 0x4b + row * 0x28;
        if (y < top || y > top + 0x28) continue;
        const int index = 0x34 + row;
        if (status_count < index) return -1;
        return index;
    }
    return -1;
}

}  // namespace byzantine
