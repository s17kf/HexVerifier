//strncpy
// Created by stefan on 20/03/24.
//

#include "gtest/gtest.h"

#include <cstring>
#include "String.h"

using utils::String;

class StringTest : public ::testing::Test {
protected:
};

TEST_F(StringTest, emptyString) {
    String s;
    ASSERT_EQ(0, s.size());
    ASSERT_EQ('\0', s.c_str()[0]);
}

TEST_F(StringTest, oneCharString) {
    String s('f');
    ASSERT_EQ(1, s.size());
    ASSERT_EQ('f', s.c_str()[0]);
}

TEST_F(StringTest, stringTest) {
    const char testString[] = "Test string";
    const String s(testString);
    ASSERT_EQ(strlen(testString), s.size());
    ASSERT_EQ(0, std::strcmp(testString, s.c_str()))
                                << "verified str: " << s.c_str();
}

TEST_F(StringTest, stringConcatenation) {
    const String s1("Part 1");
    const String s2 = String("Part 2");
    const char expectedResult[] = "Part 1Part 2";
    String result = s1 + s2;

    const size_t expectedSize = s1.size() + s2.size();
    ASSERT_EQ(expectedSize, result.size());
    ASSERT_EQ(0, std::strcmp(expectedResult, result.c_str()))
                                << "verified str: " << result.c_str();
}

TEST_F(StringTest, stringSelfConcatenation) {
    String s1("text");
    const String s2 = String("other");
    const char expectedResult[] = "textother";
    const size_t expectedSize = s1.size() + s2.size();
    s1 += s2;
    ASSERT_EQ(expectedSize, s1.size());
    ASSERT_EQ(0, std::strcmp(expectedResult, s1.c_str()))
                                << "verified str: " << s1.c_str();
}

TEST_F(StringTest, stringSelfConcatenationWithCapacityInitialization) {
    String testedString((size_t) 50);
    const String s1("text");
    const String s2 = String("other");
    const char expectedResult[] = "textother";
    const size_t expectedSize = s1.size() + s2.size();
    testedString += s1;
    ASSERT_EQ(s1.size(), testedString.size());
    ASSERT_EQ(0, std::strcmp(s1.c_str(), testedString.c_str()))
                                << "verified str: " << testedString.c_str();
    testedString += s2;
    ASSERT_EQ(expectedSize, testedString.size());
    ASSERT_EQ(0, std::strcmp(expectedResult, testedString.c_str()))
                                << "verified str: " << testedString.c_str();
}

TEST_F(StringTest, stringAndNumberConcatenation) {
    const String s("str");
    const int oneDigitNumber = 5;
    const int twoDigitNumber = 24;
    const int negativeNumber = -37;
    const char expectedOneDigitResult[] = "str5";
    const char expectedTwoDigitResult[] = "str24";
    const char expectedNegativeNumberResult[] = "str-37";

    String oneDigitResult = s + oneDigitNumber;
    String twoDigitResult = s + twoDigitNumber;
    String negativeNumberResult = s + negativeNumber;

    ASSERT_EQ(4, oneDigitResult.size());
    ASSERT_EQ(0, std::strcmp(expectedOneDigitResult, oneDigitResult.c_str()))
                                << "verified str: " << oneDigitResult.c_str();
    ASSERT_EQ(5, twoDigitResult.size());
    ASSERT_EQ(0, std::strcmp(expectedTwoDigitResult, twoDigitResult.c_str()))
                                << "verified str: " << twoDigitResult.c_str();
    ASSERT_EQ(6, negativeNumberResult.size());
    ASSERT_EQ(0, std::strcmp(expectedNegativeNumberResult, negativeNumberResult.c_str()))
                                << "verified str: " << negativeNumberResult.c_str();
}

TEST_F(StringTest, stringSelfConcatenationWithNumber) {
    String s1("text");
    const int number = 56;
    const char expectedResult[] = "text56";
    const size_t expectedSize = s1.size() + 2;
    s1 += number;
    ASSERT_EQ(expectedSize, s1.size());
    ASSERT_EQ(0, std::strcmp(expectedResult, s1.c_str()))
                                << "verified str: " << s1.c_str();
}


TEST_F(StringTest, stringSelfConcatenationWithNumberWithCapacityInitialization) {
    String testedString((size_t) 50);
    const String s1("text");
    const int number = 145;
    const char expectedResult[] = "text145";
    const size_t expectedSize = s1.size() + 3;
    testedString += s1;
    testedString += number;
    ASSERT_EQ(expectedSize, testedString.size());
    ASSERT_EQ(0, std::strcmp(expectedResult, testedString.c_str()))
                                << "verified str: " << testedString.c_str();
}

TEST_F(StringTest, rstripTest) {
    String testedString("text   ");
    ASSERT_EQ(7, testedString.size());
    ASSERT_EQ(0, std::strcmp("text   ", testedString.c_str()))
                                << "verified str: " << testedString.c_str();
    testedString.rstrip();
    ASSERT_EQ(4, testedString.size());
    ASSERT_EQ(0, std::strcmp("text", testedString.c_str()))
                                << "verified str: " << testedString.c_str();
}