//
// Created by stefan on 20/03/24.
//

#include "String.h"

#include <charconv>

namespace utils {

    String::String(const String &other) : mSize(other.mSize), mCapacity(other.mCapacity) {
        mStr = new char[mCapacity + 1];
        mStr[mCapacity] = '\0';
        strncpy(mStr, other.mStr, mSize + 1);
    }

    String::String(const char c) : mSize(1), mCapacity(1) {
        mStr = new char[2]{c, '\0'};
    }

    String::String(const char *s) {
        if (s == nullptr) {
            mSize = mCapacity = 0;
            mStr = new char[1];
            mStr[0] = '\0';
            return;
        }
        const size_t charactersToCopy = strlen(s);
        mSize = mCapacity = charactersToCopy;
        mStr = new char[charactersToCopy + 1];
        mStr[charactersToCopy] = '\0';
        strncpy(mStr, s, charactersToCopy);
    }

    String::String(size_t n) : mSize(0), mCapacity(n) {
        mStr = new char[n + 1];
    }

    String &String::operator=(const String &other) {
        if (this == &other)
            return *this;
        delete[] mStr;
        mStr = new char[other.mCapacity + 1];
        strncpy(mStr, other.mStr, other.size() + 1);
        mSize = other.size();
        mCapacity = other.mCapacity;
        return *this;
    }

    String String::operator+(const String &other) const {
        String result(size() + other.size() + 1);
        strncpy(result.mStr, mStr, size());
        strncpy(&result.mStr[size()], other.mStr, other.size() + 1);
        result.mSize = mSize + other.mSize;
        return result;
    }

    String String::operator+=(const String &other) {
        if (mCapacity < mSize + other.size()) {
            char *oldStr = mStr;
            mCapacity = mSize + other.size();
            mStr = new char[mCapacity + 1];
            mStr[mCapacity] = '\0';
            strncpy(mStr, oldStr, mSize + 1);
            delete[] oldStr;
            strncpy(&mStr[mSize], other.mStr, other.size());
            mSize = mCapacity;
        } else {
            strncpy(&mStr[mSize], other.mStr, other.size() + 1);
            mSize += other.mSize;
        }
        return *this;
    }

    String String::operator+(int number) const {
        char numberStr[8] = {};
        std::to_chars(numberStr, numberStr + 8, number);
        size_t numSize = strlen(numberStr);
        String result(size() + numSize);
        strncpy(result.mStr, mStr, size());
        strncpy(&result.mStr[size()], numberStr, numSize + 1);
        result.mSize = mSize + numSize;
        return result;
    }

    String String::operator+=(int number) {
        char numberStr[8] = {};
        std::to_chars(numberStr, numberStr + 8, number);
        size_t numSize = strlen(numberStr);
        if (mCapacity < mSize + numSize) {
            char *oldStr = mStr;
            mCapacity = mSize + numSize;
            mStr = new char[mCapacity + 1];
            mStr[mCapacity] = '\0';
            strncpy(mStr, oldStr, mSize + 1);
            delete[] oldStr;
            strncpy(&mStr[mSize], numberStr, numSize + 1);
            mSize = mCapacity;
        } else {
            strncpy(&mStr[mSize], numberStr, numSize + 1);
            mSize += numSize;
        }
        return *this;
    }

    void String::rstrip() {
        int current = size() - 1;
        while (current >= 0 && mStr[current] == ' ') {
            mStr[current] = '\0';
            --mSize;
            --current;
        }
    }

} // utils