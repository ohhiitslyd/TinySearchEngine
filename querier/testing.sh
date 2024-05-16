#!/bin/bash

# Directory and file paths
PAGE_DIR="../crawler/pageDirectory"
INDEX_FILE="../indexer/testoutput"

# Check if necessary files exist
if [ ! -d "$PAGE_DIR" ]; then
    echo "Error: $PAGE_DIR does not exist."
    exit 1
fi

if [ ! -f "$INDEX_FILE" ]; then
    echo "Error: $INDEX_FILE does not exist."
    exit 1
fi

# Function to run the querier with given query and display results
run_querier() {
    echo -e "$1" | valgrind --leak-check=full --show-leak-kinds=all ./querier "$PAGE_DIR" "$INDEX_FILE"
}

# Test cases for command-line argument validation
echo "Test: No arguments"
./querier 2> /dev/null
if [ $? -ne 1 ]; then
    echo "Failed: No arguments"
else
    echo "Passed: No arguments"
fi

echo "Test: One argument"
./querier "$PAGE_DIR" 2> /dev/null
if [ $? -ne 1 ]; then
    echo "Failed: One argument"
else
    echo "Passed: One argument"
fi

echo "Test: Invalid page directory"
./querier "invalid_directory" "$INDEX_FILE" 2> /dev/null
if [ $? -ne 2 ]; then
    echo "Failed: Invalid page directory"
else
    echo "Passed: Invalid page directory"
fi

echo "Test: Invalid index file"
./querier "$PAGE_DIR" "invalid_indexfile" 2> /dev/null
if [ $? -ne 2 ]; then
    echo "Failed: Invalid index file"
else
    echo "Passed: Invalid index file"
fi

# Test cases for query validation
echo "Test: Empty query"
run_querier ""
echo "Passed: Empty query"

echo "Test: Query with invalid characters"
run_querier "hello world!"
echo "Expected: Error: bad character '!' in query."

echo "Test: Query starting with 'and'"
run_querier "and Dartmouth"
echo "Expected: Error: 'and' cannot be first"

echo "Test: Query starting with 'or'"
run_querier "or Dartmouth"
echo "Expected: Error: 'or' cannot be first"

echo "Test: Query ending with 'and'"
run_querier "Dartmouth and"
echo "Expected: Error: 'and' cannot be last"

echo "Test: Query ending with 'or'"
run_querier "Dartmouth or"
echo "Expected: Error: 'or' cannot be last"

echo "Test: Adjacent 'and' and 'or'"
run_querier "Dartmouth and or computer science"
echo "Expected: Error: 'and' and 'or' cannot be adjacent"

echo "Test: Adjacent 'or' and 'and'"
run_querier "Dartmouth or and computer science"
echo "Expected: Error: 'or' and 'and' cannot be adjacent"

echo "Test: Adjacent 'and' and 'and'"
run_querier "Dartmouth and and computer science"
echo "Expected: Error: 'and' and 'and' cannot be adjacent"

# Test cases for valid queries
echo "Test: Single word query"
run_querier "home"
echo "Expected: Query: home"

echo "Test: Spaces"
run_querier "computational           or world"
echo "Expected: hello world"

echo "Test: Two word query with implicit 'and'"
run_querier "home computational"
echo "Expected: Query: home computational"

echo "Test: Two word query with explicit 'and'"
run_querier "home and computational"
echo "Expected: Query: home and computational"

echo "Test: Query with 'or'"
run_querier "computational or home"
echo "Expected: Query: computational or home"

echo "Test: Complex query with 'and' and 'or'"
run_querier "home and computational or college and for"
echo "Expected: Query: home and computational or college and for"

# Cleanup
echo "All tests completed."