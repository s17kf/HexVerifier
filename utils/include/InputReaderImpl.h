//
// Created by stefan on 27/03/24.
//

#ifndef HEX_INPUTREADERIMPL_H
#define HEX_INPUTREADERIMPL_H

#include "InputReader.h"

#include <istream>

namespace utils {
    class InputReaderImpl : public InputReader {
    public:
        explicit InputReaderImpl(FILE *input = stdin) : mInput(input) {}

        String *getLine() override;

    private:
        FILE *mInput;
    };

} // utils


#endif //HEX_INPUTREADERIMPL_H
