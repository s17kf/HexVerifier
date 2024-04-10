#!/bin/bash


cd "$(dirname "$0")"

source "common.sh"

targets=(
"UtilsTests"
"DataStructuresTests"
"BoardTests"
)

for target in "${targets[@]}"; do
  cmake --build ../cmake-build-debug --target "$target" -j 6
done

exit_code=0
for target in "${targets[@]}"; do
  echo
  echo "Run tests : ${target}"
  valgrind_output_file="tmp_valgrind.out"
  if valgrind --time-stamp=yes ./../cmake-build-debug/"$target" 2> "$valgrind_output_file" > /dev/null; then
    echo -e "${GREEN_BG}${BLACK_BOLD}OK${NC}: $target"
  else
    exit_code=1
    echo -e "${RED_BG}${WHITE_BOLD}FAILED${NC}: $target"
  fi
  verify_valgrind_output "$valgrind_output_file"
  show_valgrind_summary "$valgrind_output_file"
  rm $valgrind_output_file
done

exit $exit_code
