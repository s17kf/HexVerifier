# HexVerifier

HexVerifier is a C++ project (developed for academic purposes as part of the Algorithms and Data Structures course at Gdańsk University of Technology) designed to analyze given HEX game's board in specific state. The repository includes various data structures, utility functions, and tests to ensure robust verification.

## Table of Contents
- Installation
- Usage
- Project Structure
- License

## Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/s17kf/HexVerifier.git
   ```

2. Navigate to the project directory:
    ```sh
    cd HexVerifier
    ```

3. Build the project using CMake:
    ```sh
    mkdir cmake-build-debug
    cd cmake-build-debug
    cmake ..
    make
    ```

## Usage

To run the main application:
  ```sh
  ./cmake-build-debug/Hex
  ```

To run unit tests:
  ```sh
  ./cmake-build-debug/tests/BoardTests
  ./cmake-build-debug/tests/DataStructuresTests
  ./cmake-build-debug/tests/UtilsTests
  ```

All unit tests can be run with:
  ```sh
  ./tests/unit_tests.sh
  #(This script checks for memory leaks as well)
  ```

Functional tests can be run with:
  ```sh
  ./tests/test.sh
  #(To enable memory leak checks, use the -v flag)
  ```

## Project Structure

- `board/`: Contains board-related files (includes verification methods).
- `dataStructures/`: Includes various data structures used in the project.
- `utils/`: Utility functions and helpers.
- `tests/`: Functional tests for the project.
- `CMakeLists.txt`: Build configuration file.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
