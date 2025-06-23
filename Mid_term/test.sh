#!/bin/bash

# Set project variables
PROJECT_NAME="Tutorial04"
SRC_DIR="src"          # Directory containing C++ source files
BIN_DIR="bin"          # Directory for compiled binary
TEST_DIR="tests"       # Directory containing test input/output files
COMPILER="g++"         # The compiler to use (e.g., g++, clang++)
FLAGS="-Wno-c++11-extensions"  # Compilation flags -Wall -Werror 
EXECUTABLE="${BIN_DIR}/$PROJECT_NAME"

# Step 1: Clean old build files
echo "Cleaning previous build files..."
rm -rf ${BIN_DIR}/*
mkdir -p $BIN_DIR

# Step 2: Compile the project
echo "Compiling the project..."
$COMPILER $FLAGS -o $EXECUTABLE $SRC_DIR/*.cpp

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
else
    echo "Compilation successful."
fi

# Step 3: Run tests
echo "Running tests..."
TEST_FILES=$(ls $TEST_DIR/*.in)
PASS_COUNT=0
FAIL_COUNT=0

for TEST_FILE in $TEST_FILES; do
    # Extract test name
    TEST_NAME=$(basename $TEST_FILE .in)

    # Run the compiled executable with test input
    ./$EXECUTABLE < "$TEST_FILE" > "${TEST_DIR}/${TEST_NAME}.out"

    # Compare output with the expected output
    DIFF_RESULT=$(diff "${TEST_DIR}/${TEST_NAME}.out" "${TEST_DIR}/${TEST_NAME}.expected")

    if [ "$DIFF_RESULT" == "" ]; then
        echo "Test $TEST_NAME passed."
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo "Test $TEST_NAME failed."
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
done

# Step 4: Report results
echo ""
echo "Test Summary:"
echo "Passed: $PASS_COUNT"
echo "Failed: $FAIL_COUNT"

# Step 5: Exit with an appropriate status code
if [ $FAIL_COUNT -ne 0 ]; then
    exit 1
else
    exit 0
fi
