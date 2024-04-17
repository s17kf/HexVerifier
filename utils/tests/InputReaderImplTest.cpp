//
// Created by stefan on 28/03/24.
//

#include "gtest/gtest.h"

#include <sstream>
#include <fstream>
#include "InputReaderImpl.h"


using utils::InputReaderImpl;

class InputReaderImplTest : public ::testing::Test {
protected:
    InputReaderImplTest() {
        writeLinesToFile(SAMPLE_FILENAME, SAMPLE_FILE_LINES);
    }

    ~InputReaderImplTest() override {
        remove(SAMPLE_FILENAME.c_str());
    }


    const std::string SAMPLE_FILENAME = "sampleBoard1Filename";

    const std::vector<std::string> SAMPLE_FILE_LINES = {
            "       ---    ",
            "    --< r >--     ",
            " --< b >-<   >--",
            "<   >-< b >-<   >  ",
            " --<   >-< r >--",
            "    --< r >--",
            "       ---"
    };

    const std::vector<std::string> SAMPLE_FILE_LINES_STRIPPED = {
            "---",
            "--< r >--",
            "--< b >-<   >--",
            "<   >-< b >-<   >",
            "--<   >-< r >--",
            "--< r >--",
            "---"
    };
private:
    void writeLinesToFile(const std::string &fileName, const std::vector<std::string> &lines) {
        std::ofstream fileStream;
        fileStream.open(fileName, std::ios_base::out);
        for (const auto &line: lines) {
            fileStream << line << std::endl;
        }
        fileStream.close();
    }
};

using utils::String;

TEST_F(InputReaderImplTest, getlineReturnsStrippedLine) {
    std::stringstream stream;
    for (const auto &line: SAMPLE_FILE_LINES) {
        stream << line << std::endl;
    }
    InputReaderImpl inputReader(stream);
    size_t loopCounter = 0;
    for (auto &expectedLine: SAMPLE_FILE_LINES_STRIPPED) {
        ++loopCounter;
        std::unique_ptr<std::string> testedLine(inputReader.getLine());
        ASSERT_EQ(0, strcmp(expectedLine.c_str(), testedLine->c_str()))
                                    << "loopCounter=" << loopCounter << ": "
                                    << "expected: '" << expectedLine << "', tested: '" << *testedLine << "'";
    }
}
