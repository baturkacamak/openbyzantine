// The game's own pointer table and selection rules (0x40dae5/0x40df05/
// 0x40c329/0x4083d4/0x40de7b). Pure data and state logic: no SDL, no window.

#include "byzantine/cursors.hpp"
#include "byzantine/inventory.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

int failures = 0;

void check(bool condition, const std::string& what) {
    if (condition) return;
    std::cout << "FAIL " << what << '\n';
    ++failures;
}

std::string describe(const byzantine::CursorChoice& choice) {
    return (choice.kind == byzantine::CursorChoice::Kind::table ? "table " : "item ") +
           std::to_string(choice.id) + (choice.hotitem ? " hotitem" : "");
}

void expect_table(const byzantine::CursorChoice& choice, unsigned id, const std::string& what) {
    check(choice.kind == byzantine::CursorChoice::Kind::table, what + " kind table");
    check(choice.id == id, what + " id " + std::to_string(id) + " got " + describe(choice));
}

void expect_item(const byzantine::CursorChoice& choice, unsigned id, bool hotitem,
                 const std::string& what) {
    check(choice.kind == byzantine::CursorChoice::Kind::item, what + " kind item");
    check(choice.id == id, what + " item id " + std::to_string(id) + " got " + describe(choice));
    check(choice.hotitem == hotitem, what + " hotitem flag got " + describe(choice));
}

byzantine::SceneObjectState object(std::uint8_t kind, std::uint32_t name_id) {
    byzantine::SceneObjectState entry;
    entry.kind = kind;
    entry.name_id = name_id;
    entry.active = 1;
    return entry;
}

// A scene with the real strip layout: ten channels, one page of six items.
byzantine::SceneState strip_state(std::vector<std::uint16_t> items) {
    byzantine::SceneState state;
    state.dialogue_channels[0] = std::move(items);
    state.dialogue_visible = true;
    return state;
}

void test_table() {
    using byzantine::cursor_asset;
    using byzantine::cursor_slot;
    using byzantine::cursor_theme;
    using byzantine::CursorTheme;

    // 0x42dad9/0x40dae5: only -1 (0x42c8a0's initial value) is the normal set.
    check(cursor_theme(-1) == CursorTheme::normal, "theme -1 normal");
    check(cursor_theme(0) == CursorTheme::klio, "theme 0 klio");
    check(cursor_theme(1) == CursorTheme::klio, "theme 1 klio");

    // Shipped table entries: names and hotspots from 0x45dbe8.
    const auto& def = cursor_asset(CursorTheme::normal, 0);
    check(std::string(def.name) == "default" && def.hotspot_x == 24 && def.hotspot_y == 24,
          "normal 0 default 24,24");
    const auto& take = cursor_asset(CursorTheme::normal, 0x0d);
    check(std::string(take.name) == "take" && take.hotspot_x == 19 && take.hotspot_y == 12,
          "normal 13 take 19,12");
    const auto& talk = cursor_asset(CursorTheme::normal, 0x0a);
    check(std::string(talk.name) == "talk", "normal 10 talk");
    const auto& hotbig = cursor_asset(CursorTheme::normal, 0x1c);
    check(std::string(hotbig.name) == "hotbig", "normal 28 hotbig");
    check(cursor_asset(CursorTheme::normal, 3).name == nullptr, "normal 3 unset");
    const auto& klio_take = cursor_asset(CursorTheme::klio, 0x0d);
    check(std::string(klio_take.name) == "take" && klio_take.hotspot_x == 6 &&
              klio_take.hotspot_y == 2,
          "klio 13 take 6,2");
    check(cursor_asset(CursorTheme::klio, 0x0a).name == nullptr, "klio 10 has no art");

    // 0x40de7b: the small variants replace the big ones outside a panorama.
    check(cursor_slot(CursorTheme::normal, true, 0x00) == 0x00, "slot panorama keeps 0");
    check(cursor_slot(CursorTheme::normal, false, 0x00) == 0x1d, "slot flat 0 -> map");
    check(cursor_slot(CursorTheme::normal, false, 0x17) == 0x1d, "slot flat 0x17 -> map");
    check(cursor_slot(CursorTheme::normal, true, 0x1c) == 0x1c, "slot panorama keeps hotbig");
    check(cursor_slot(CursorTheme::normal, false, 0x1c) == 0x1e, "slot flat 0x1c -> hotsmall");
    check(cursor_slot(CursorTheme::normal, false, 0x0d) == 0x0d, "slot flat keeps take");
    check(cursor_slot(CursorTheme::klio, false, 0x00) == 0x00, "slot klio keeps 0");
    check(cursor_slot(CursorTheme::klio, false, 0x1c) == 0x1c, "slot klio keeps hotbig");
}

void test_hover() {
    using byzantine::hover_cursor;
    byzantine::SceneState state;

    // Nothing under the mouse.
    expect_table(hover_cursor(state, nullptr), 0, "empty playfield");

    // 0x40c329: the object's kind byte is the cursor id.
    auto exit_object = object(0x0b, 0xffffffffu);
    expect_table(hover_cursor(state, &exit_object), 0x0b, "exit kind");
    auto take_object = object(0x0d, 0xffffffffu);
    expect_table(hover_cursor(state, &take_object), 0x0d, "take kind");
    auto cursor_object = object(0x1c, 0xffffffffu);
    expect_table(hover_cursor(state, &cursor_object), 0x1c, "hotbig kind");

    // An empty table slot keeps the default pointer instead of indexing art
    // the shipped table never defines.
    auto undefined_object = object(0x03, 0xffffffffu);
    expect_table(hover_cursor(state, &undefined_object), 0x03, "kind 3 stays its id");
    auto out_of_range = object(0x40, 0xffffffffu);
    expect_table(hover_cursor(state, &out_of_range), 0x00, "kind above the table");

    // Held item, no object: the item's own art is the pointer.
    state.current_dialogue_record = 11;
    expect_item(hover_cursor(state, nullptr), 11, false, "held item empty playfield");

    // Incompatible use: the item pointer stays, the click is rejected later.
    expect_item(hover_cursor(state, &exit_object), 11, false, "held item over exit");

    // Kind 0 accepts any item and stays the item pointer.
    auto generic = object(0x00, 0xffffffffu);
    expect_item(hover_cursor(state, &generic), 11, false, "held item over kind 0");

    // Use hotspots (0x12/0x13/0x14) switch to the item's hotitem art.
    auto use_direct = object(0x12, 11);
    expect_item(hover_cursor(state, &use_direct), 11, true, "held item over use hotspot");
    auto use_mismatch = object(0x12, 6);
    expect_item(hover_cursor(state, &use_mismatch), 11, false, "held item over other use slot");

    // 0x13 names a dialogue chain; the chain containing the item accepts it.
    byzantine::SceneDialogueChain chain;
    chain.count = 1;
    chain.ids[0] = 11;
    state.dialogue_chains[5] = chain;
    auto use_chain = object(0x13, 5);
    expect_item(hover_cursor(state, &use_chain), 11, true, "held item over chain hotspot");

    // A talk hotspot that names the held item keeps the talk cursor.
    auto talk_item = object(0x0a, 11);
    expect_table(hover_cursor(state, &talk_item), 0x0a, "held item over talk hotspot");
    // ... while a talk hotspot that accepts nothing holds the item pointer.
    auto talk_named = object(0x0a, 6);
    expect_item(hover_cursor(state, &talk_named), 11, false, "held item over other talk");
}

void test_strip() {
    using byzantine::strip_cursor;
    const std::vector<std::uint16_t> items{11, 6, 17, 30, 21, 25};

    // Hidden strip: no pointer change (the port shows its default).
    auto hidden = strip_state(items);
    hidden.dialogue_visible = false;
    expect_table(strip_cursor(hidden, 0xa0, 0x1b0), 0, "hidden strip");

    // Empty space inside the bar (between the last slot and the next arrow).
    auto state = strip_state(items);
    expect_table(strip_cursor(state, 0x230, 0x1b0), 0, "empty strip area");

    // Hovering a slot arms the strip pointer (0x1c -> hotbig/hotsmall).
    expect_table(strip_cursor(state, 0xa0, 0x1b0), 0x1c, "first slot");
    expect_table(strip_cursor(state, 0xa0 + 0x3c * 2, 0x1b0), 0x1c, "third slot");
    // The examined item is not a pointer target.
    state.current_use_item = 6;
    expect_table(strip_cursor(state, 0xa0 + 0x3c + 4, 0x1b0), 0, "examined slot");
    expect_table(strip_cursor(state, 0xa0, 0x1b0), 0x1c, "other slot");

    // Page arrows only when there is somewhere to scroll.
    state.current_use_item.reset();
    expect_table(strip_cursor(state, 0x240, 0x1b8), 0, "next arrow at the end");
    expect_table(strip_cursor(state, 0x270, 0x1b8), 0, "prev arrow at the start");
    state.dialogue_channels[0].push_back(99);
    state.dialogue_channels[0].push_back(98);
    expect_table(strip_cursor(state, 0x240, 0x1b8), 0x1c, "next arrow with more items");
    state.dialogue_scroll = 2;
    expect_table(strip_cursor(state, 0x270, 0x1b8), 0x1c, "prev arrow after scrolling");

    // Held item: its own art, and the hotitem only over the examine/channel area.
    state.dialogue_scroll = 0;
    state.current_dialogue_record = 11;
    expect_item(strip_cursor(state, 0xa0 + 0x3c * 4, 0x1b0), 11, false, "held item over slot");
    expect_item(strip_cursor(state, 0x20, 0x1c0), 11, true, "held item over examine button");
    expect_item(strip_cursor(state, 0x60, 0x1c0), 11, true, "held item over channel area");
    expect_item(strip_cursor(state, 0x230, 0x1b0), 11, false, "held item over empty bar");
    // The hotitem variant needs the item on the current page.
    state.dialogue_channels[0].erase(state.dialogue_channels[0].begin());
    expect_item(strip_cursor(state, 0x20, 0x1c0), 11, false, "hotitem off page");
}

void test_hotitem_name() {
    check(byzantine::hotitem_media_name("ibeering") == "hbeering", "hotitem 8 chars");
    check(byzantine::hotitem_media_name("iridnote") == "hridnote", "hotitem second name");
    check(byzantine::hotitem_media_name("") == "", "hotitem empty");
    check(byzantine::hotitem_media_name("abcdefghij") == "hbcdefgh", "hotitem truncates");
}

// 0x40dae5's "cursors/%s*.tga" wildcard against a scratch directory.
void test_cursor_files() {
    namespace fs = std::filesystem;
    std::error_code error;
    const auto root = fs::temp_directory_path() / "byz-cursor-test";
    fs::remove_all(root, error);
    fs::create_directories(root / "klio", error);
    fs::create_directories(root / "empty", error);
    const auto touch = [&](const std::string& path) {
        std::ofstream out(root / path, std::ios::binary);
        out << "x";
    };
    touch("DEFAULT.TGA");
    touch("forward.TGA");
    touch("klio/INFO1.TGA");
    touch("klio/INFO2.TGA");
    touch("klio/infox.jpg");

    const auto exact = byzantine::find_cursor_file(root, "default");
    check(exact.has_value(), "exact file found");
    check(exact && exact->path.filename() == "DEFAULT.TGA", "exact file name");
    check(exact && exact->frames == 1, "exact file frame count");

    const auto prefixed = byzantine::find_cursor_file(root / "klio", "info");
    check(prefixed.has_value(), "prefixed file found");
    check(prefixed && prefixed->path.filename() == "INFO1.TGA", "prefixed file name");
    check(prefixed && prefixed->frames == 2, "prefix counts only .tga matches");

    // An exact name wins over a longer prefixed one.
    const auto forward = byzantine::find_cursor_file(root, "forward");
    check(forward && forward->path.filename() == "forward.TGA", "exact beats prefix");

    check(!byzantine::find_cursor_file(root / "empty", "default").has_value(),
          "missing file");
    check(!byzantine::find_cursor_file(root / "nope", "default").has_value(),
          "missing directory");
    fs::remove_all(root, error);
}

}  // namespace

int main() {
    test_table();
    test_hover();
    test_strip();
    test_hotitem_name();
    test_cursor_files();
    if (failures != 0) {
        std::cout << failures << " failure(s)\n";
        return EXIT_FAILURE;
    }
    std::cout << "cursor-test ok\n";
    return EXIT_SUCCESS;
}
