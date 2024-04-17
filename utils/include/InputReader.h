//
// Created by stefan on 27/03/24.
//

#ifndef HEX_INPUTREADER_H
#define HEX_INPUTREADER_H

#include "String.h"
#include <string>

namespace utils {
    class InputReader {
    public:
        virtual std::string *getLine() = 0;

        virtual bool eof() const = 0;
    };

} // utils

#endif //HEX_INPUTREADER_H
