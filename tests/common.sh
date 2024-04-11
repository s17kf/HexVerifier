
export VALGRIND_OK=0
export VALGRIND_MEM_LEAKS=1
export VALGRIND_ERRORS=2

export NC="\033[0m"
export RED="\033[0;31m"
export GREEN="\033[0;32m"
export RED_BG="\033[41m"
export GREEN_BG="\033[42m"
export BLACK_BOLD="\033[1;30m"
export YELLOW_BOLD="\033[1;33m"
export WHITE_BOLD="\033[1;37m"
export WHITE_UNDER="\033[4;37m"

function verify_valgrind_output() {
    local output_file=$1
    if ! grep -q "All heap blocks were freed -- no leaks are possible" "$output_file"; then
      echo -e "${RED_BG}${WHITE_BOLD}VALGRIND: FAIL${NC}: Mem leaks found!"
      return $VALGRIND_MEM_LEAKS
    fi
    if ! grep -q "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" "$output_file"; then
      echo -e "${RED_BG}${WHITE_BOLD}VALGRIND: FAIL${NC}: Errors found!"
      return $VALGRIND_ERRORS
    fi
    echo -e "${GREEN_BG}${BLACK_BOLD}VALGRIND${NC}: OK"
    return $VALGRIND_OK
}

function show_valgrind_summary() {
    local output_file=$1
    grep -A 2 "HEAP SUMMARY" "$output_file"
}
