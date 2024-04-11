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

exit_code=0
for base_name in "${test_file_base_names[@]}"; do
  input_file="${base_name}.in"
  output_file="${base_name}.out"
  expected_file="${base_name}.expected"
  echo
  echo "info: verify $base_name"
  valgrind_output_file="tmp_valgrind.out"
  valgrind --time-stamp=yes ../cmake-build-debug/Hex < "$input_file" > "$output_file" 2> "$valgrind_output_file"
  if ! diff -q "$output_file" "$expected_file" > /dev/null; then
    echo -e "${RED_BG}${WHITE_BOLD}FAILED${NC}: $base_name"
    exit_code=1
  else
    echo -e "${GREEN_BG}${BLACK_BOLD}OK${NC}: $base_name"
  fi
  verify_valgrind_output "$valgrind_output_file"
  show_valgrind_summary "$valgrind_output_file"
  rm $valgrind_output_file
done

exit $exit_code
