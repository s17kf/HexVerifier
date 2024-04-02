//
// Created by stefan on 02/04/24.
//

#ifndef HEX_INPUTREADERMOCK_H
#define HEX_INPUTREADERMOCK_H

#include <gmock/gmock.h>

#include "InputReader.h"

namespace utils {

    class InputReaderMock : public InputReader {
    public:
        MOCK_METHOD(String *, getLine, (), (override));
        MOCK_METHOD(bool, eof, (), (const, override));
    };

} // utils

#endif //HEX_INPUTREADERMOCK_H
