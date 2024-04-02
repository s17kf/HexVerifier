//
// Created by stefan on 27/03/24.
//

#include "InputReaderImpl.h"

namespace utils {

    String *InputReaderImpl::getLine() {
        const size_t maxLen = 100;
        char line[100];
        mStream.getline(line, maxLen);
        auto *result = new String(line);
        result->rstrip('\n');
        result->strip();
        return result;
    }

} // utils