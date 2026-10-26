#include "byzantine/inventory.hpp"

#include <iostream>
#include <stdexcept>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main() {
    try {
        using namespace byzantine;
        SceneState state;
        state.dialogue_channels[0] = {45, 5, 16, 33, 10, 29, 49, 32,
                                      48, 11, 6, 50, 20, 4, 12, 1, 0};
        state.dialogue_scroll = 8;
        state.dialogue_tables.names.resize(60);
        state.dialogue_tables.names[11].name = "ibeering";
        for (std::uint16_t id = 0; id < 60; ++id) {
            state.dialogue_records[id] = SceneDialogueRecord{3, false};
        }

        const auto page = inventory_page(state);
        require(page.page == 0 && page.scroll == 8 && page.items.size() == 6,
                "inventory page did not expose six AM entries");
        require(page.items[0] == 48 && page.items[1] == 11 && page.items[5] == 4,
                "inventory page item order changed");

        require(!inventory_select(state, 5), "off-page item was selectable");
        require(inventory_select(state, 11), "on-page item was not selectable");
        require(state.current_dialogue_record && *state.current_dialogue_record == 11,
                "selection did not store the item id");
        require(state.dialogue_records[11].state == 1,
                "state-3 selection did not mark the item seen");
        require(inventory_select(state, 11), "second click did not deselect");
        require(!state.current_dialogue_record, "second click left the item selected");

        inventory_scroll(state, 5);
        require(state.dialogue_scroll == 11, "scroll did not clamp to count-6");
        inventory_scroll(state, -100);
        require(state.dialogue_scroll == 0, "scroll did not clamp to zero");

        state.dialogue_scroll = 8;
        require(inventory_select(state, 11), "selection for use failed");
        require(inventory_mark_using(state, 11), "examine button rejected an item");
        require(state.current_use_item && *state.current_use_item == 11 &&
                    state.dialogue_records[11].state == 4,
                "examine button did not set state 4 / DAT_0045d1cc");
        require(examine_image_stem(inventory_name(state, 11)) == "ebeering",
                "0x40860e examine stem mismatch");

        require(examine_scene_for("AM") == "examine", "AM should use EXAMINE");
        require(examine_scene_for("vh") == "examvh", "vh group mismatch");
        require(examine_scene_for("hstmed") == "examvb", "hstmed group mismatch");
        require(examine_scene_for("lintel") == "examvc", "lintel group mismatch");
        require(examine_scene_for("clock") == "examvt", "clock group mismatch");
        require(examine_scene_for("mz2") == "examvt", "mz2 group mismatch");
        require(!examine_scene_for("va"), "va's out-of-range original group was guessed");
        require(examine_scene_for("examine") == "examine", "examine self mapping changed");

        std::cout << "inventory tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "inventory-test: " << error.what() << '\n';
        return 1;
    }
}
