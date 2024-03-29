cmake_minimum_required(VERSION 3.27)
project(Hex)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        utils/include
        googletest/googletest/include
        googletest/googlemock/include)


set(MAIN_SOURCES
        utils/src/String.cpp
        utils/src/InputReaderImpl.cpp
)

set(UTILS_TEST_SOURCES
        tests_main.cpp
        utils/src/String.cpp
        utils/src/InputReaderImpl.cpp
        utils/tests/StringTest.cpp
        utils/tests/InputReaderImplTest.cpp
)


add_executable(Hex main.cpp ${MAIN_SOURCES})

add_subdirectory(googletest)

add_executable(UtilsTests ${UTILS_TEST_SOURCES})
target_link_libraries(UtilsTests gmock)