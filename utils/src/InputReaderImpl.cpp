//
// Created by stefan on 27/03/24.
//

#include "InputReaderImpl.h"

#include <cstdio>
#include <ios>

namespace utils {

    String *InputReaderImpl::getLine() {
        char *lineRaw = nullptr;
        size_t len;
        if (getline(&lineRaw, &len, mInput) < 0) {
            delete lineRaw;
            throw std::ios_base::failure("getline error");
        }
        auto *result = new String(lineRaw);
        free(lineRaw);
        result->rstrip('\n');
        result->strip();
        return result;
    }

} // utils