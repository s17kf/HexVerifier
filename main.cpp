#include "InputReaderImpl.h"
#include "HexVerifier.h"

int main() {
    utils::InputReaderImpl inputReader;
    board::HexVerifier::handleInput(inputReader);

    return 0;
}
