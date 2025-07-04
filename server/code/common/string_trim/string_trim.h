#ifndef STRING_TRIM_H
#define STRING_TRIM_H

#include "return_type.h"

#include <string>
#include <algorithm>

namespace common
{
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    inline void trim(std::string &s) {
        rtrim(s);
        ltrim(s);
    }
};

#endif