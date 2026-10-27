#include "byzantine/menu.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

int failures = 0;

void check(bool condition, const std::string& what) {
    if (condition) return;
    std::fprintf(stderr, "FAIL: %s\n", what.c_str());
    ++failures;
}

void check_equal(long long got, long long want, const std::string& what) {
    if (got == want) return;
    std::fprintf(stderr, "FAIL: %s (got %lld, want %lld)\n", what.c_str(), got, want);
    ++failures;
}

}  // namespace

int main() {
    using namespace byzantine;

    // 0x408fe7: the five boundaries and the 0x14-row band.
    check_equal(top_bar_button(0, 0), 1, "x=0 is the first button");
    check_equal(top_bar_button(180, 19), 1, "x=180 is still the first button");
    check_equal(top_bar_button(181, 10), 2, "x=181 is the second button");
    check_equal(top_bar_button(323, 10), 2, "x=323 is still the second");
    check_equal(top_bar_button(324, 10), 3, "x=324 is the third");
    check_equal(top_bar_button(499, 10), 3, "x=499 is still the third");
    check_equal(top_bar_button(500, 0), 4, "x=500 is the fourth");
    check_equal(top_bar_button(639, 19), 4, "x=639 is the fourth");
    check_equal(top_bar_button(100, -1), 0, "y<0 is outside the bar");
    check_equal(top_bar_button(100, 20), 0, "row 0x14 is the playfield");
    check_equal(top_bar_button(-50, 5), 1, "negative x clamps to the bar");

    // 0x41de4b's four mappings.
    check_equal(top_bar_menu_mode(1), 1, "YÜKLE/KAYDET -> mode 1");
    check_equal(top_bar_menu_mode(2), 3, "NOTLAR -> mode 3");
    check_equal(top_bar_menu_mode(3), 2, "KEŞİF -> mode 2");
    check_equal(top_bar_menu_mode(4), 0, "TERCİHLER -> mode 0");
    check_equal(top_bar_menu_mode(0), -1, "no button has no mode");

    // The 0x28 rectangles are the shipped table.
    check_equal(static_cast<long long>(menu_control_rects().size()), 0x28,
                "0x28 control rectangles");
    const auto* row0 = menu_control_rect(0x00);
    check(row0 != nullptr, "control 0 exists");
    check_equal(row0->left, 0x46, "OYUN KAYIT left");
    check_equal(row0->top, 0x10e, "OYUN KAYIT top");
    check_equal(row0->right, 0xd7, "OYUN KAYIT right");
    check_equal(row0->bottom, 0x122, "OYUN KAYIT bottom");
    const auto* first_pref = menu_control_rect(0x06);
    check_equal(first_pref->top, 0x71, "first preference row top");
    const auto* last_pref = menu_control_rect(0x0c);
    check_equal(last_pref->bottom, 0x1a9, "last preference row bottom");
    const auto* first_slot = menu_control_rect(0x12);
    check_equal(first_slot->top, 0x42, "first save slot top");
    const auto* last_slot = menu_control_rect(0x1b);
    check_equal(last_slot->bottom, 0x15e, "last save slot bottom");
    const auto* kesif = menu_control_rect(0x24);
    check_equal(kesif->bottom, 0, "0x24 bottom is filled at runtime");
    check(menu_control_rect(0x28) == nullptr, "0x28 is not in the table");

    // FUN_0041fe46 enable flags.
    check(menu_static_control_enabled(0, 0x06), "prefs enable row 6");
    check(menu_static_control_enabled(0, 0x0c), "prefs enable row 12");
    check(!menu_static_control_enabled(0, 0x00), "prefs do not enable save");
    check(menu_static_control_enabled(1, 0x00), "save/load enables OYUN KAYIT");
    check(menu_static_control_enabled(1, 0x03), "save/load enables YENİ OYUN");
    check(menu_static_control_enabled(1, 0x05), "save/load enables OYUNA DEVAM");
    check(menu_static_control_enabled(2, 0x24), "keşif enables the list");
    check(!menu_static_control_enabled(2, 0x00), "keşif does not enable save");
    check(menu_static_control_enabled(3, 0x1f), "NOTLAR enables the page");
    check(menu_static_control_enabled(4, 0x20), "TEMAS enables the spread");
    check(menu_static_control_enabled(5, 0x1d), "spread enables the corner");

    // 0x42044d's save-slot rules.
    check(menu_control_enabled(1, 0x12, true, false), "existing slot row");
    check(!menu_control_enabled(1, 0x12, false, false), "missing slot row");
    check(!menu_control_enabled(1, 0x01, true, false), "load needs a selection");
    check(menu_control_enabled(1, 0x01, true, true), "load with a selection");
    check(!menu_control_enabled(1, 0x02, true, false), "delete needs a selection");
    check(menu_control_enabled(1, 0x05, true, false), "continue without selection");
    check(!menu_control_enabled(1, 0x05, true, true), "continue disabled by a selection");
    check(!menu_control_enabled(0, 0x12, true, false), "slots are mode 1 only");

    // 0x4202ff slots and 0x422101 list rows.
    check_equal(save_slot_row_at(0x1de, 0x43), 0, "first save row");
    check_equal(save_slot_row_at(0x1de, 0x4f), 0, "first row still inside");
    check_equal(save_slot_row_at(0x1de, 0x42), -1, "save row top is exclusive");
    check_equal(save_slot_row_at(0x1de, 0x50), -1, "save row bottom is exclusive");
    check_equal(save_slot_row_at(0x1de, 0x61), 1, "second save row");
    check_equal(save_slot_row_at(0x1de, 0x159), 9, "tenth save row");
    check_equal(save_slot_row_at(0x1de, 0x15e), -1, "save row bottom is exclusive");
    check_equal(save_slot_row_at(0x100, 0x42), -1, "save row x outside");
    check_equal(kesif_row_at(0x1de, 0x4b, 0x3a), 0x34, "first keşif row");
    check_equal(kesif_row_at(0x1de, 0x73, 0x3a), 0x34, "boundary belongs to the first row");
    check_equal(kesif_row_at(0x1de, 0x74, 0x3a), 0x35, "second keşif row");
    check_equal(kesif_row_at(0x1de, 0x4b, 0x34), 0x34, "credits row with default status");
    check_equal(kesif_row_at(0x1de, 0x74, 0x34), -1, "second row needs status");
    check_equal(kesif_row_at(0x100, 0x4b, 0x3a), -1, "keşif x outside");
    check_equal(kesif_row_at(0x1de, 0x164, 0x3a), -1, "keşif below the last row");

    if (failures != 0) {
        std::fprintf(stderr, "%d menu check(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    std::printf("menu-test: all checks passed\n");
    return EXIT_SUCCESS;
}
