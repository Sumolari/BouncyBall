// Copyright 2015 Lluís Ulzurrun de Asanza Sàez

#include "./fmaw_debug.h"

#include <stdarg.h>  // For va_start, etc.

#include <nds.h>
#include <nds/debug.h>

#include <cstdio>
#include <memory>    // For std::unique_ptr
#include <sstream>
#include <string>

#include "./fmaw_types.h"

namespace FMAW {

void printf(const std::string fmt_str, ...) {
    /* Reserve two times as much as the length of the fmt_str */
    int final_n, n = (static_cast<int>(fmt_str.size()) * 2);
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while (1) {
        /* Wrap the plain char array into the unique_ptr */
        formatted.reset(new char[n]);
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    nocashMessage((std::string(formatted.get())).c_str());
}

std::string byte_to_binary(uint8 x) {
    std::stringstream string {};
    for (uint8 z = 128; z > 0; z >>= 1)
        string << (((x & z) == z) ? "1" : "0");
    return string.str();
}

std::string half_word_to_binary(uint16 x) {
    std::stringstream string {};
    for (uint16 z = (2 << 14); z > 0; z >>= 1) {
        string << (((x & z) == z) ? "1" : "0");
        if (z == (2 << 11) || z == (2 << 7) || z == (2 << 3)) string << " ";
    }
    return string.str();
}

}  // namespace FMAW
