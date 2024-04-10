#! /usr/bin/bash

# This script is copying all files needed for evaluation to one specific
# directory. It is done only to make sending files to evalution site
# easier.

dst_dir="filesToEvaluation"

files_to_cp=(
"main.cpp"

"board/include/Board.h"
"board/include/BoardParser.h"
"board/include/Cell.h"
"board/include/CellCoords.h"
"board/include/HexVerifier.h"
"board/include/DistancesKeeper.h"
"board/include/DoneVerifiers.h"
"board/include/NeighboursHelpers.h"
#"board/include/WinVerifiers.h"
"board/algorithms/include/Bfs.h"
"board/algorithms/include/Dfs.h"
"board/algorithms/include/MinMax.h"
"board/algorithms/include/WinVerificationAlgorithm.h"
"board/src/Board.cpp"
"board/src/BoardParser.cpp"
"board/src/Cell.cpp"
"board/src/DistancesKeeper.cpp"
"board/src/DoneVerifiers.cpp"
"board/src/HexVerifier.cpp"
"board/src/NeighboursHelpers.cpp"
"board/algorithms/src/Bfs.cpp"
"board/algorithms/src/Dfs.cpp"
"board/algorithms/src/MinMax.cpp"

"utils/include/InputReader.h"
"utils/include/InputReaderImpl.h"
"utils/include/String.h"
"utils/src/InputReaderImpl.cpp"
"utils/src/String.cpp"

"dataStructures/include/List.h"
"dataStructures/include/Queue.h"
"dataStructures/include/Stack.h"
"dataStructures/include/Vector.h"
)

for file in "${files_to_cp[@]}"; do
  cp "$file" "$dst_dir"
done
