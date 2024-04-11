cmake_minimum_required(VERSION 3.27)
project(Hex)

set(CMAKE_CXX_STANDARD 23)

include_directories(
        utils/include
        board/include
        board/algorithms/include
        dataStructures/include
        googletest/googletest/include
        googletest/googlemock/include)


set(MAIN_SOURCES
        board/src/Board.cpp
        board/src/BoardParser.cpp
        board/src/DistancesKeeper.cpp
        board/src/DoneVerifiers.cpp
        board/src/HexVerifier.cpp
        board/src/NeighboursHelpers.cpp
        board/algorithms/src/Dfs.cpp
        board/algorithms/src/Bfs.cpp
        board/algorithms/src/MinMax.cpp
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

set(BOARD_TEST_SOURCES
        tests_main.cpp
        utils/src/String.cpp
        board/src/Board.cpp
        board/src/BoardParser.cpp
        board/src/DistancesKeeper.cpp
        board/src/DoneVerifiers.cpp
        board/src/NeighboursHelpers.cpp
        board/tests/BoardTest.cpp
        board/tests/BoardParserTest.cpp
        board/algorithms/src/Dfs.cpp
        board/algorithms/src/Bfs.cpp
        board/algorithms/src/MinMax.cpp
        board/algorithms/tests/BfsTest.cpp
        board/algorithms/tests/MinMaxTest.cpp
)

set(DATA_STRUCTURES_TEST_SOURCES
        tests_main.cpp
        dataStructures/tests/ListTest.cpp
        dataStructures/tests/StackTest.cpp
        dataStructures/tests/QueueTest.cpp
        dataStructures/tests/VectorTest.cpp
)

add_executable(Hex main.cpp ${MAIN_SOURCES})

add_subdirectory(googletest)

add_executable(UtilsTests ${UTILS_TEST_SOURCES})
target_link_libraries(UtilsTests gmock)

add_executable(BoardTests ${BOARD_TEST_SOURCES})
target_link_libraries(BoardTests gmock)

add_executable(DataStructuresTests ${DATA_STRUCTURES_TEST_SOURCES})
target_link_libraries(DataStructuresTests gmock)
