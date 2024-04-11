//
// Created by stefan on 20/03/24.
//

#include "String.h"

#include <charconv>

using data_structures::List;

namespace utils {

    String::String(const String &other) : mSize(other.mSize), mCapacity(other.mCapacity) {
        mStr = new char[mCapacity + 1];
        mStr[mCapacity] = '\0';
        strncpy(mStr, other.mStr, mSize);
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
        mStr[n] = '\0';
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

    String& String::rstrip(char c) {
        int current = size() - 1;
        while (current >= 0 && mStr[current] == c) {
            mStr[current] = '\0';
            --mSize;
            --current;
        }
        return *this;
    }

    String& String::lstrip(char c) {
        size_t firstNotSpaceIndex = 0;
        while (firstNotSpaceIndex < size() && mStr[firstNotSpaceIndex] == c) {
            ++firstNotSpaceIndex;
        }
        if (firstNotSpaceIndex == 0)
            return *this;
        mSize -= firstNotSpaceIndex;
        char *oldStr = mStr;
        mStr = new char[mCapacity + 1];
        strncpy(mStr, &oldStr[firstNotSpaceIndex], mSize + 1);
        delete[] oldStr;
        return *this;
    }

    String& String::strip(char c) {
        return rstrip(c).lstrip(c);
    }

    void String::split(List<String> &result, char delimiter) const {
        size_t startPos = 0u;
        for (size_t endPos = 0u; endPos < mSize; ++endPos) {
            if (mStr[endPos] != delimiter)
                continue;
            if (endPos == startPos) {
                ++startPos;
                continue;
            }
            size_t newPartSize = endPos - startPos;
            result.pushBack(String(newPartSize));
            strncpy(result.back().mStr, &mStr[startPos], newPartSize);
            result.back().mSize = newPartSize;
            startPos = endPos + 1;
        }
        if (startPos < mSize){
            size_t newPartSize = mSize - startPos;
            result.pushBack(String(newPartSize));
            strncpy(result.back().mStr, &mStr[startPos], newPartSize);
            result.back().mSize = newPartSize;
        }
    }

} // utils
