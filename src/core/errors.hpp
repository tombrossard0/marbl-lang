#pragma once

#include <sysexits.h>

namespace Errors {
inline void report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message;
}
} // namespace Errors
