//
// Created by stefan on 20/03/24.
//

#ifndef CALCULATORAADS_STRING_H
#define CALCULATORAADS_STRING_H

#include <cstdio>
#include <cstring>

#include "List.h"

namespace utils {

    class String {
    public:
        String(const String &other);

        explicit String(size_t n);

        explicit String(char c);

        explicit String(const char *s = nullptr);

        ~String() {
            delete[] mStr;
        }

        String &operator=(const String &other);

        [[nodiscard]] constexpr size_t size() const { return mSize; }

        String operator+(const String &other) const;

        String operator+=(const String &other);

        String operator+(int number) const;

        String operator+=(int number);

        [[nodiscard]] inline const char *c_str() const { return mStr; }

        [[nodiscard]] inline char operator[](unsigned int index) { return mStr[index]; }

        [[nodiscard]] inline char last() { return mStr[mSize - 1]; }

        bool operator==(const String &other) const { return 0 == strcmp(mStr, other.mStr); }

        bool operator==(const char *other) const { return 0 == strcmp(mStr, other); }

        String &rstrip(char c = ' ');

        String &lstrip(char c = ' ');

        String &strip(char c = ' ');

        data_structures::List<String> split(char delimiter = ' ') const;

    private:
        size_t mSize;
        size_t mCapacity;
        char *mStr;
    };

} // utils


#endif //CALCULATORAADS_STRING_H
