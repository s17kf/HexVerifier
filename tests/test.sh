#!/bin/bash

test_file_base_names=(
"canWinWithNaive"
"isBoardPossible"
"isGameOver"
)

cd "$(dirname "$0")"

exit_code=0

for base_name in "${test_file_base_names[@]}"; do
  input_file="${base_name}.in"
  output_file="${base_name}.out"
  expected_file="${base_name}.expected"
  echo "info: verify $base_name"
  ../cmake-build-debug/Hex < "$input_file" > "$output_file"
  if ! diff -q "$output_file" "$expected_file" > /dev/null; then
    echo "FAILED: $base_name"
    exit_code=1
  else
    echo "OK: $base_name"
  fi
done

exit $exit_code