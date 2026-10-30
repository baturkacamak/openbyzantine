#include "byzantine/automation.hpp"
#include <cassert>
int main() {
    std::istringstream valid("# comment\nhover 639 479\nclick 0 0\nkey F5\nwait 10\nsnapshot first\nquit\n");
    assert(byzantine::parse_automation(valid).size() == 6);
    for (const auto* text : {"snapshot ../escape\nquit", "snapshot a\nsnapshot a\nquit",
                            "click 640 0\nsnapshot a\nquit", "wait -1\nsnapshot a\nquit",
                            "snapshot a\nquit\nkey F5", "snapshot a", "quit",
                            "snapshot a extra\nquit", "unknown\nsnapshot a\nquit"}) {
        bool rejected = false;
        try { std::istringstream input(text); byzantine::parse_automation(input); }
        catch (const std::runtime_error&) { rejected = true; }
        assert(rejected);
    }
    std::istringstream window_events("resize 641 481\nfocus lost\nfocus gained\nsnapshot resized\nquit\n");
    assert(byzantine::parse_automation(window_events).size()==5);
    for(const char* text:{"resize 99999 99999\nsnapshot x\nquit", "focus nowhere\nsnapshot x\nquit"}) {
        bool refused=false;try {std::istringstream input(text);byzantine::parse_automation(input);}
        catch(const std::runtime_error&){refused=true;}assert(refused);
    }
    std::ostringstream json;
    byzantine::automation_json_string(json, std::string("\"\\\n\t") + char(1));
    assert(json.str() == "\"\\\"\\\\\\u000a\\u0009\\u0001\"");
}
