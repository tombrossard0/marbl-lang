#pragma once
#include <algorithm>
#include <string>

namespace Marbl {
inline std::string trim(const std::string &s) {
    auto start = s.find_first_not_of(" \t\n\r");
    auto end = s.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

} // namespace Marbl
