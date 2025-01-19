#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No color

# Array of test targets
tests=("test-statevec" "test-gate" "test-list")

# Function to run a test and check the result
run_test() {
    local test_name=$1

    echo "Running ${test_name}..."
    if make ${test_name} && ./run-${test_name}; then
        echo -e "${GREEN}${test_name} passed!${NC}"
    else
        echo -e "${RED}${test_name} failed.${NC}"
        exit 1 # Exit if any test fails
    fi
    echo
}

# Run all tests
for test in "${tests[@]}"; do
    run_test $test
done

echo -e "${GREEN}All tests passed successfully!${NC}"