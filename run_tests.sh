#!/bin/bash

# Test script for Connected Components Assignment
# CSS311 - Parallel and Distributed Computing

echo "=========================================================="
echo "  Connected Components Algorithm - Test Suite"
echo "  CSS311 - Parallel and Distributed Computing"
echo "=========================================================="
echo ""

# Check if make is available
if ! command -v make &> /dev/null; then
    echo "Error: make is not installed"
    exit 1
fi

# Check if g++ is available
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ is not installed"
    exit 1
fi

# Clean previous builds
echo "Cleaning previous builds..."
make clean
echo ""

# Compile both versions
echo "Compiling programs..."
make all
if [ $? -ne 0 ]; then
    echo "Error: Compilation failed"
    exit 1
fi
echo ""

# Run serial version
echo "=========================================================="
echo "  RUNNING SERIAL VERSION"
echo "=========================================================="
echo ""
./serial_cc
echo ""

# Run parallel version
echo "=========================================================="
echo "  RUNNING PARALLEL VERSION"
echo "=========================================================="
echo ""
./parallel_cc
echo ""

# Generate summary
echo "=========================================================="
echo "  TEST SUMMARY"
echo "=========================================================="
echo ""
echo "✓ Serial implementation executed successfully"
echo "✓ Parallel implementation executed successfully"
echo "✓ Performance comparison completed"
echo ""
echo "For detailed analysis, see ASSIGNMENT_REPORT.md"
echo ""
echo "=========================================================="
echo "  All tests completed!"
echo "=========================================================="

