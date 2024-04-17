//strncpy
// Created by stefan on 20/03/24.
//

#include "gtest/gtest.h"

#include <cstring>
#include <list>
#include "String.h"
#include "List.h"

using utils::String;
using data_structures::List;

class StringTest : public ::testing::Test {
protected:
    static void verifySplitResult(const std::string &testedString, const std::list<std::string> &s1ExpectedParts) {
        List<std::string> splitResult;
        String::split(&testedString, splitResult);
        std::list<std::string> splitResultAsListOfStrings;
        for (auto &s: splitResult) {
            splitResultAsListOfStrings.emplace_back(s.c_str());
        }
        ASSERT_EQ(s1ExpectedParts, splitResultAsListOfStrings) << "tested string: \"" << testedString << "\"";
    }
};

TEST_F(StringTest, rstripTest) {
    std::string testedString = "text   ";
    ASSERT_EQ(7, testedString.size());
    ASSERT_EQ(0, std::strcmp("text   ", testedString.c_str()))
                                << "verified str: " << testedString;
    String::rstrip(&testedString);
    ASSERT_EQ(4, testedString.size()) << "tested str: " << testedString;
    ASSERT_EQ(0, std::strcmp("text", testedString.c_str()))
                                << "verified str: " << testedString;
}

TEST_F(StringTest, lstripTest) {
    std::string testedString = "   text";
    ASSERT_EQ(7, testedString.size());
    ASSERT_EQ(0, std::strcmp("   text", testedString.c_str()))
                                << "verified str: " << testedString.c_str();
    String::lstrip(&testedString);
    ASSERT_EQ(4, testedString.size());
    ASSERT_EQ(0, std::strcmp("text", testedString.c_str()))
                                << "verified str: " << testedString.c_str();
}

TEST_F(StringTest, stripTest) {
    std::string testedString="   text    ";
    ASSERT_EQ(11, testedString.size());
    ASSERT_EQ(0, std::strcmp("   text    ", testedString.c_str()))
                                << "verified str: " << testedString.c_str();
    String::strip(&testedString);
    ASSERT_EQ(4, testedString.size());
    ASSERT_EQ(0, std::strcmp("text", testedString.c_str()))
                                << "verified str: " << testedString.c_str();
}

TEST_F(StringTest, splitTest) {
    const std::string s1 = "p1 pp2 part3 ppp4";
    const std::list<std::string> s1ExpectedParts = {"p1", "pp2", "part3", "ppp4"};
    verifySplitResult(s1, s1ExpectedParts);

    const std::string s2 = "  p1 pp2     part3 ppp4    ";
    verifySplitResult(s2, s1ExpectedParts);

    const std::string s3 = "";
    const std::list<std::string> emptyList;
    verifySplitResult(s3, emptyList);

    const std::string s4 = "   ";
    verifySplitResult(s4, emptyList);
}
