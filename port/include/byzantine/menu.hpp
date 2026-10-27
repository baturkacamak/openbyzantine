#pragma once

// The in-game top bar and its full-screen menu screens.
//
// The bar itself is the shipped `setupart/TOP_BAR.TGA` (640x20) plus the
// highlighted/pressed states of `setupart/HOTBAR.RLE` and `setupart/DWNBAR.RLE`.
// This header carries the parts that are pure geometry and therefore testable
// without SDL: the 0x408fe7 x boundaries, the 0x41de4b button -> screen mapping,
// the 0x45f548 control rectangles and the FUN_0041fe46/0x42044d enable sets.
//
// Evidence: analysis/rea/TOP_MENU.md. The screen art, text and per-scene
// actions are drawn/executed by the player; nothing here invents a rectangle.

#include <array>
#include <cstdint>
#include <vector>

namespace byzantine {

// 0x408fe7's local_18[5] boundaries and the 0x408f32 bar rectangle.
constexpr std::array<int, 5> kTopBarBounds{0x00, 0xb5, 0x144, 0x1f4, 0x280};
constexpr int kTopBarWidth = 0x280;
constexpr int kTopBarHeight = 0x14;

// 0x408fe7: how many boundaries are <= x, i.e. the value stored in
// DAT_0045d1d4 (1..4), or 0 when the point is outside the 0x14-row bar.
int top_bar_button(int x, int y);

// 0x41de4b: DAT_0045d1d4 -> FUN_0041e289's screen mode. The four top buttons
// map to 1 (YÜKLE/KAYDET), 3 (NOTLAR), 2 (KEŞİF) and 0 (TERCİHLER).
int top_bar_menu_mode(int button);

// One entry of the 0x28-rectangle table at 0x45f548 (16 bytes per entry:
// left, top, right, bottom). The ids are the ones FUN_0041fe46 and the
// 0x41e289 click switch use.
struct MenuControlRect {
    int id = 0;
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
};

const std::vector<MenuControlRect>& menu_control_rects();
const MenuControlRect* menu_control_rect(int id);

// FUN_0041fe46's static enable table for a screen mode (0 preferences,
// 1 save/load, 2 keşif, 3/4/5 the notebook pages).
bool menu_static_control_enabled(int mode, int id);

// 0x42044d: the ten save-slot rows (0x12..0x1b) are enabled only while the
// slot exists. Rows 1/2 (load/delete) need a selected slot, and row 5
// (continue) is disabled while one is selected.
bool menu_control_enabled(int mode, int id, bool save_row_present,
                          bool save_selected);

// 0x4202ff's ten rows: x 0x17a..0x242, y 0x42 + 0x1e*i, 0x0e rows tall.
int save_slot_row_at(int x, int y);

// 0x422101: the KEŞİF list starts at y 0x4b with 0x28-tall rows for
// 0x34..0x3a, and only rows <= status_count are shown.
int kesif_row_at(int x, int y, int status_count);

}  // namespace byzantine
