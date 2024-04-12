#!/bin/bash

cd "$(dirname "$0")"

source "common.sh"

test_file_base_names=(
"canWinWithNaive"
"canWinWithPerfect"
"isBoardPossible"
"isGameOver"
)

cmake --build ../cmake-build-debug --target Hex -j 6

if [[ $# -eq 1 ]] && [[ "$1" == "-v" ]] ; then
  valgrind="valgrind"
fi

exit_code=0
for base_name in "${test_file_base_names[@]}"; do
  input_file="${base_name}.in"
  output_file="${base_name}.out"
  expected_file="${base_name}.expected"
  echo "================================"
  echo -e "Verify ${WHITE_UNDER}$base_name${NC}:"
  valgrind_output_file="tmp_valgrind.out"
  time $valgrind ../cmake-build-debug/Hex < "$input_file" > "$output_file" 2> "$valgrind_output_file"
  if ! diff -q "$output_file" "$expected_file" > /dev/null; then
    echo -e "${RED_BG}${WHITE_BOLD}FAILED${NC}: $base_name"
    exit_code=1
  else
    echo -e "${GREEN_BG}${BLACK_BOLD}OK${NC}: $base_name"
  fi
  [[ -n $valgrind ]] && verify_valgrind_output "$valgrind_output_file"
  rm $valgrind_output_file
done
echo "================================"

exit $exit_code
