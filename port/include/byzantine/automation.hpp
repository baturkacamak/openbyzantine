// Diagnostic input scripts. These do not execute VM labels or edit game state.
#pragma once
#include <algorithm>
#include <cctype>
#include <istream>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
namespace byzantine {
struct AutomationDecision {
    std::string scene;
    unsigned record = 0, group = 0;
    std::size_t action = 0, choice = 0;
    std::vector<unsigned> lines;
};
struct AutomationAction {
    std::string kind, name;
    int x = 0, y = 0;
};
inline std::vector<AutomationAction> parse_automation(std::istream& input) {
    std::vector<AutomationAction> actions;
    std::set<std::string> names;
    std::string line;
    unsigned number = 0;
    while (std::getline(input, line)) {
        ++number;
        std::istringstream row(line);
        AutomationAction a;
        if (!(row >> a.kind) || a.kind[0] == '#') continue;
        const auto bad = [&] { throw std::runtime_error("invalid automation line " + std::to_string(number)); };
        if (a.kind == "click" || a.kind == "hover") {
            if (!(row >> a.x >> a.y) || a.x < 0 || a.x >= 640 || a.y < 0 || a.y >= 480) bad();
        } else if (a.kind == "resize") {
            if (!(row >> a.x >> a.y) || a.x < 320 || a.x > 2560 || a.y < 240 || a.y > 1920) bad();
        } else if (a.kind == "focus") {
            if (!(row >> a.name) || (a.name != "lost" && a.name != "gained")) bad();
        } else if (a.kind == "wait") {
            if (!(row >> a.x) || a.x < 0 || a.x > 600000) bad();
        } else if (a.kind == "key" || a.kind == "snapshot") {
            if (!(row >> a.name)) bad();
            if (a.kind == "snapshot") {
                if (!std::all_of(a.name.begin(), a.name.end(), [](unsigned char c) {
                        return std::isalnum(c) || c == '-' || c == '_'; }) || !names.insert(a.name).second) bad();
            }
        } else if (a.kind != "quit") bad();
        std::string extra;
        if (row >> extra) bad();
        if (!actions.empty() && actions.back().kind == "quit") bad();
        actions.push_back(a);
        if (actions.size() > 100000) bad();
    }
    if (actions.empty() || actions.back().kind != "quit" || names.empty())
        throw std::runtime_error("automation needs a snapshot and a final quit");
    return actions;
}
inline void automation_json_string(std::ostream& out, const std::string& value) {
    static const char hex[] = "0123456789abcdef";
    out << '"';
    for (unsigned char c : value) {
        if (c == '"' || c == '\\') out << '\\' << static_cast<char>(c);
        else if (c < 32) out << "\\u00" << hex[c >> 4] << hex[c & 15];
        else out << static_cast<char>(c);
    }
    out << '"';
}
} // namespace byzantine
