//
// Created by stefan on 27/03/24.
//

#ifndef HEX_INPUTREADERIMPL_H
#define HEX_INPUTREADERIMPL_H

#include "InputReader.h"

#include <istream>
#include <iostream>

namespace utils {
    class InputReaderImpl : public InputReader {
    public:
        explicit InputReaderImpl(std::istream &stream = std::cin) : mStream(stream) {}

        [[nodiscard]] bool eof() const override {
            return mStream.eof();
        }

        std::string *getLine() override {
            auto *line = new std::string();
            std::getline(mStream, *line);
            String::strip(line);
            return line;
        }

    private:
        std::istream &mStream;
    };

} // utils


#endif //HEX_INPUTREADERIMPL_H
