//
// Created by stefan on 20/03/24.
//

#include "String.h"

#include <charconv>

using data_structures::List;

namespace utils {

    void String::rstrip(std::string *str, const std::string &toRemove) {
        size_t pos = str->find_last_not_of(toRemove);
        if (pos != std::string::npos) {
            str->erase(pos + 1, str->size());
        }
    }

    void String::lstrip(std::string *str, const std::string &toRemove) {
        size_t pos = str->find_first_not_of(toRemove);
        if (pos != std::string::npos) {
            str->erase(0, pos);
        }
    }

    void String::strip(std::string *str, const std::string &toRemove) {
        rstrip(str, toRemove);
        lstrip(str, toRemove);
    }

    void String::split(const std::string *str, List <std::string> &result, const std::string &delimiter) {
        size_t startPos = 0u;
        size_t endPos;
        while ((endPos = str->find(delimiter, startPos)) != std::string::npos) {
            if (endPos == startPos) {
                ++startPos;
                continue;
            }
            result.pushBack("");
            result.back() += str->substr(startPos, endPos - startPos);
            startPos = endPos + delimiter.size();
        }
        if (startPos + 1 < str->size()) {
            size_t newPartSize = str->size() - startPos;
            result.pushBack(str->substr(startPos, newPartSize));
        }
    }

} // utils
