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

        // The close-up switch is atomic: selection (DAT_0045d1c8), examined
        // item (DAT_0045d1cc), record state and therefore the image stem all
        // move together, and the previous examined record returns to state 1
        // (0x4085e7's reset). The real case is HSTMED page 5: 66 ihstd and
        // 72 icrwbar are both on the page.
        {
            SceneState switch_state;
            switch_state.dialogue_channels[5] = {66, 70, 67, 64, 68, 72};
            switch_state.current_dialogue_channel = 5;
            switch_state.dialogue_tables.names.resize(80);
            switch_state.dialogue_tables.names[66].name = "ihstd";
            switch_state.dialogue_tables.names[72].name = "icrwbar";
            for (std::uint16_t id = 0; id < 80; ++id) {
                switch_state.dialogue_records[id] = SceneDialogueRecord{1, false};
            }
            require(inventory_examine(switch_state, 72), "close-up examine rejected 72");
            require(switch_state.current_dialogue_record &&
                        *switch_state.current_dialogue_record == 72 &&
                        switch_state.current_use_item &&
                        *switch_state.current_use_item == 72 &&
                        switch_state.dialogue_records[72].state == 4,
                    "close-up examine did not set one item for selection/use/state");
            require(examine_image_stem(inventory_name(switch_state,
                                                      *switch_state.current_use_item)) ==
                        "ecrwbar",
                    "close-up image stem does not follow the examined item");
            require(inventory_examine(switch_state, 66), "close-up switch rejected 66");
            require(switch_state.current_dialogue_record &&
                        *switch_state.current_dialogue_record == 66 &&
                        switch_state.current_use_item &&
                        *switch_state.current_use_item == 66 &&
                        switch_state.dialogue_records[66].state == 4,
                    "close-up switch left selection/use/state disagreeing");
            require(switch_state.dialogue_records[72].state == 1,
                    "previous examined record did not return to state 1");
            require(examine_image_stem(inventory_name(switch_state,
                                                      *switch_state.current_use_item)) ==
                        "ehstd",
                    "close-up image stem did not move to the switched item");
            require(!inventory_examine(switch_state, 5),
                    "close-up examine accepted an off-page item");
        }

        require(examine_scene_for("AM") == "examine", "AM should use EXAMINE");
        require(examine_scene_for("vh") == "examvh", "vh group mismatch");
        require(examine_scene_for("hstmed") == "examvb", "hstmed group mismatch");
        require(examine_scene_for("lintel") == "examvc", "lintel group mismatch");
        require(examine_scene_for("clock") == "examvt", "clock group mismatch");
        require(examine_scene_for("mz2") == "examvt", "mz2 group mismatch");
        require(!examine_scene_for("va"), "va's out-of-range original group was guessed");
        require(examine_scene_for("examine") == "examine", "examine self mapping changed");

        // 0x407c5a: the item-on-object gate. The real CLOCK object 26 is kind
        // 0x12 with name id 21 (ISTAR); HSTMED's crowbar objects are kind 0x12
        // with name id 72 (icrwbar); close-up exits are kind 2 with name id -1.
        {
            SceneObjectState star;
            star.kind = 0x12;
            star.name_id = 21;
            require(item_use_compatible(state, star, std::uint16_t{21}),
                    "0x407c5a rejected the object's own item");
            require(!item_use_compatible(state, star, std::uint16_t{6}),
                    "0x407c5a accepted the wrong item");
            require(!item_use_compatible(state, star, std::nullopt),
                    "0x407c5a accepted an empty selection for kind 0x12");

            SceneObjectState exit_object;
            exit_object.kind = 0x02;
            exit_object.name_id = 0xffffffffu;
            require(item_use_compatible(state, exit_object, std::uint16_t{21}),
                    "0x407c5a blocked a kind-2 exit while an item was selected");
            require(item_use_compatible(state, exit_object, std::nullopt),
                    "0x407c5a blocked a kind-2 exit without a selection");

            SceneObjectState named_exit;
            named_exit.kind = 0x02;
            named_exit.name_id = 7;
            require(item_use_compatible(state, named_exit, std::uint16_t{7}) &&
                        !item_use_compatible(state, named_exit, std::uint16_t{21}),
                    "0x407c5a kind-2 name compare is wrong");

            SceneObjectState item_only;
            item_only.kind = 0x0d;
            item_only.name_id = 0xffffffffu;
            require(item_use_compatible(state, item_only, std::nullopt) &&
                        !item_use_compatible(state, item_only, std::uint16_t{21}),
                    "0x407c5a kind 0x0d 'no item' rule is wrong");

            SceneObjectState never;
            never.kind = 0x03;
            never.name_id = 21;
            require(!item_use_compatible(state, never, std::uint16_t{21}) &&
                        !item_use_compatible(state, never, std::nullopt),
                    "0x407c5a kind 0x03 accepted a selection");

            SceneObjectState hidden;
            hidden.kind = 0x18;
            hidden.name_id = 21;
            require(!item_use_compatible(state, hidden, std::uint16_t{21}),
                    "0x407c5a kind 0x18 accepted a selection");

            // Kind 0x13/0x14 name a dialogue chain; the selected record must be
            // one of the chain's ids (0x407c5a's 0x486a10 walk).
            SceneDialogueChain chain;
            chain.count = 2;
            chain.ids[0] = 21;
            chain.ids[1] = 7;
            state.dialogue_chains[5] = chain;
            SceneObjectState chain_object;
            chain_object.kind = 0x13;
            chain_object.name_id = 5;
            require(item_use_compatible(state, chain_object, std::uint16_t{21}) &&
                        item_use_compatible(state, chain_object, std::uint16_t{7}),
                    "0x407c5a kind 0x13 did not search the chain");
            require(!item_use_compatible(state, chain_object, std::uint16_t{3}),
                    "0x407c5a kind 0x13 accepted an item outside the chain");
            chain_object.kind = 0x14;
            require(item_use_compatible(state, chain_object, std::uint16_t{7}),
                    "0x407c5a kind 0x14 did not search the chain");
            chain_object.name_id = 0xffffffffu;
            require(!item_use_compatible(state, chain_object, std::uint16_t{7}),
                    "0x407c5a kind 0x14 accepted a missing chain");
        }

        std::cout << "inventory tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "inventory-test: " << error.what() << '\n';
        return 1;
    }
}
