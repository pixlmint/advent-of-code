#!/bin/bash

BASE_DIR="$(dirname "$0")"
SRC_DIR="$BASE_DIR/src"
LIB_DIR="$BASE_DIR/lib"
TEST_DIR="$BASE_DIR/tests"
INPUT_DIR="$BASE_DIR/input"
BINDIR="$BASE_DIR/bin"

create_day_files() {
    read -p "Enter day number: " day
    day=$(printf "%02d" $day) # Pad to two digits (e.g., 01, 02)
    
    # Ensure directories exist
    mkdir -p "$SRC_DIR" "$TEST_DIR" "$INPUT_DIR"

    day_src="$SRC_DIR/day${day}.c"
    day_hdr="$SRC_DIR/day${day}.h"
    test_file="$TEST_DIR/test_day${day}.c"
    input_file="$INPUT_DIR/day${day}.txt"

    # Create .h file
    if [[ ! -f $day_hdr ]]; then
        cat > "$day_hdr" <<EOF
#ifndef DAY${day}_H
#define DAY${day}_H

int solve_day${day}(const char *input);

#endif
EOF
        echo "Created: $day_hdr"
    else
        echo "File already exists: $day_hdr"
    fi

    # Create .c file
    if [[ ! -f $day_src ]]; then
        cat > "$day_src" <<EOF
#include <stdio.h>
#include "day${day}.h"

int solve_day${day}(const char *input) {
    // TODO: Implement solution for Day ${day}
    printf("Input for Day ${day}: %s\\n", input);
    return 0; // Example solution
}
EOF
        echo "Created: $day_src"
    else
        echo "File already exists: $day_src"
    fi

    # Create test file
    if [[ ! -f $test_file ]]; then
        cat > "$test_file" <<EOF
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "day${day}.h"

static void test_day${day}(void **state) {
    assert_true(0);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_day${day}),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
EOF
        echo "Created: $test_file"
    else
        echo "File already exists: $test_file"
    fi

    # Create input file
    if [[ ! -f $input_file ]]; then
        wget --load-cookies .cookies -O $input_file https://adventofcode.com/2024/day/$day/input
        echo "Created: $input_file"
    else
        echo "File already exists: $input_file"
    fi

    bear --append -- gcc -o "$BINDIR/day$day" "$day_src" "$test_file" "$TEST_DIR/framework.h" -I"$SRC_DIR" "$sources" -I"$LIB_DIR" -lcmocka -lm
}

run_tests() {
    mkdir -p "$BINDIR"
    for test_file in "$TEST_DIR"/*.c; do
        test_name=$(basename "$test_file" .c)
        day=$(echo "$test_name" | sed 's/test_day//') # Extract day number
        day_src="$SRC_DIR/day${day}.c"
        sources=$(find "$LIB_DIR" -name "*.c" -type f | tr '\n' ' ' | sed 's/[[:space:]]*$//')

        if [[ ! -f $day_src ]]; then
            echo "Source file for Day $day not found: $day_src"
            continue
        fi

        echo "Running test: $test_name"
        bear -- gcc -lpthread -DSUPPRESS_PRINTS -o "$BINDIR/$test_name" "$test_file" "$day_src" -I"$SRC_DIR" -I"$LIB_DIR" $LIB_DIR/*.c -lcmocka -lm && "$BINDIR/$test_name"
    done
}

debug_test() {
    read -p "Enter day number: " day
    day=$(printf "%02d" $day) # Pad to two digits

    mkdir -p "$BINDIR"
    
    test_file="$TEST_DIR/test_day${day}.c"
    day_src="$SRC_DIR/day${day}.c"
    sources=$(find "$LIB_DIR" -name "*.c" -type f | tr '\n' ' ' | sed 's/[[:space:]]*$//')
    output_bin="$BINDIR/test_day${day}_debug"

    if [[ ! -f $day_src ]]; then
        echo "Source file for Day $day not found: $day_src"
        return 1
    fi

    if [[ ! -f $test_file ]]; then
        echo "Test file for Day $day not found: $test_file"
        return 1
    fi

    echo "Compiling with debug symbols..."
    bear -- gcc -lpthread -g -O0 -DSUPPRESS_PRINTS -o "$output_bin" "$test_file" "$day_src" -I"$SRC_DIR" -I"$LIB_DIR" $LIB_DIR/*.c -lcmocka -lm

    if [[ $? -eq 0 ]]; then
        echo "Starting GDB..."
        gdb "$output_bin"
    else
        echo "Compilation failed."
        return 1
    fi
}

valgrind_test() {
    read -p "Enter day number: " day
    day=$(printf "%02d" $day)

    mkdir -p "$BINDIR"
    
    test_file="$TEST_DIR/test_day${day}.c"
    day_src="$SRC_DIR/day${day}.c"
    sources=$(find "$LIB_DIR" -name "*.c" -type f | tr '\n' ' ' | sed 's/[[:space:]]*$//')
    output_bin="$BINDIR/test_day${day}_valgrind"

    if [[ ! -f $day_src ]] || [[ ! -f $test_file ]]; then
        echo "Source or test file for Day $day not found"
        return 1
    fi

    echo "Compiling with debug symbols..."
    bear -- gcc -lpthread -g -O0 -DSUPPRESS_PRINTS -o "$output_bin" "$test_file" "$day_src" -I"$SRC_DIR" -I"$LIB_DIR" $LIB_DIR/*.c -lcmocka -lm

    if [[ $? -eq 0 ]]; then
        echo "Running Valgrind..."
        valgrind --leak-check=full \
                 --show-leak-kinds=all \
                 --track-origins=yes \
                 --log-file="$BINDIR/valgrind_day${day}.log" \
                 "$output_bin"
        echo "Valgrind log saved to: $BINDIR/valgrind_day${day}.log"
    else
        echo "Compilation failed."
        return 1
    fi
}

run_program_valgrind() {
    read -p "Enter day number: " day
    day=$(printf "%02d" $day) # Pad to two digits (e.g., 01, 02)

    mkdir -p "$BINDIR"

    day_src="$SRC_DIR/day${day}.c"
    day_hdr="day${day}.h"
    input_file="$INPUT_DIR/day${day}.txt"
    output_bin="$BINDIR/day${day}"
    temp_main="$BINDIR/main_day${day}.c"
    sources=$(find "$LIB_DIR" -name "*.c" -type f | tr '\n' ' ' | sed 's/[[:space:]]*$//')

    if [[ ! -f $day_src ]]; then
        echo "Source file for Day $day not found: $day_src"
        return
    fi

    if [[ ! -f "$SRC_DIR/$day_hdr" ]]; then
        echo "Header file for Day $day not found: $SRC_DIR/$day_hdr"
        return
    fi

    if [[ ! -f $input_file ]]; then
        echo "Input file for Day $day not found: $input_file"
        return
    fi

    # Create a temporary main file
    cat > "$temp_main" <<EOF
#include "${day_hdr}"

int main() {
    const char *input = "$input_file";
    return solve_day${day}(input);
}
EOF

    # Compile and run the program
    # cmd=
    # echo "Running compile command\n$cmd"
    bear -- gcc -lpthread -g -O0 -o "$output_bin" "$temp_main" "$day_src" -I"$SRC_DIR" -I"$LIB_DIR" -lm $sources
    if [[ $? -eq 0 ]]; then
        echo "Running Day $day..."
        valgrind --leak-check=full \
                 --show-leak-kinds=all \
                 --track-origins=yes \
                 --log-file="$BINDIR/valgrind_day${day}_run.log" \
                 "$output_bin"
        echo "Valgrind log saved to: $BINDIR/valgrind_day${day}_run.log"
    else
        echo "Compilation failed for Day $day."
    fi

    # Cleanup temporary main file
    rm -f "$temp_main"
}

run_program() {
    read -p "Enter day number: " day
    day=$(printf "%02d" $day) # Pad to two digits (e.g., 01, 02)

    mkdir -p "$BINDIR"

    day_src="$SRC_DIR/day${day}.c"
    day_hdr="day${day}.h"
    input_file="$INPUT_DIR/day${day}.txt"
    output_bin="$BINDIR/day${day}"
    temp_main="$BINDIR/main_day${day}.c"
    sources=$(find "$LIB_DIR" -name "*.c" -type f | tr '\n' ' ' | sed 's/[[:space:]]*$//')

    if [[ ! -f $day_src ]]; then
        echo "Source file for Day $day not found: $day_src"
        return
    fi

    if [[ ! -f "$SRC_DIR/$day_hdr" ]]; then
        echo "Header file for Day $day not found: $SRC_DIR/$day_hdr"
        return
    fi

    if [[ ! -f $input_file ]]; then
        echo "Input file for Day $day not found: $input_file"
        return
    fi

    # Create a temporary main file
    cat > "$temp_main" <<EOF
#include "${day_hdr}"

int main() {
    const char *input = "$input_file";
    return solve_day${day}(input);
}
EOF

    # Compile and run the program
    # cmd=
    # echo "Running compile command\n$cmd"
    bear -- gcc -lpthread -o "$output_bin" "$temp_main" "$day_src" -I"$SRC_DIR" -I"$LIB_DIR" -lm $sources
    if [[ $? -eq 0 ]]; then
        echo "Running Day $day..."
        "$output_bin"
    else
        echo "Compilation failed for Day $day."
    fi

    # Cleanup temporary main file
    rm -f "$temp_main"
}

case $1 in
    create)
        create_day_files
        ;;
    test)
        run_tests
        ;;
    debug_test)
        debug_test
        ;;
    run)
        run_program
        ;;
    valgrind_run)
        run_program_valgrind
        ;;
    valgrind)
        valgrind_test
        ;;
    *)
        echo "Usage: $0 {create|test|run|debug_test|valgrind}"
        exit 1
        ;;
esac
