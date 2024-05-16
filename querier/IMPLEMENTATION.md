# Querier Implementation Specification

The querier program is responsible for parsing user queries, validating them, searching for matching documents based on the queries, and displaying the results in a ranked order. The implementation can be broken down into the following components:

## Main Function

### main()
- **Description**: The entry point of the program. It handles argument parsing, initialization, and cleanup.
- **Responsibilities**:
  - Parse command-line arguments using `parseArgs`.
  - Initialize and load the index data structure from the provided file.
  - Continuously read and process queries from stdin.
  - Clean up allocated memory and delete the index data structure before exiting.

## Argument Handling

### parseArgs()
- **Description**: Validates command-line arguments and initializes the necessary data structures.
- **Responsibilities**:
  - Ensure that exactly two arguments are provided.
  - Validate that the provided directory is a crawler directory.
  - Verify that the provided index file is readable.
  - Set the `pageDirectory` and `indexFilename` pointers to the appropriate command-line arguments.

## Query Processing

### display_prompt()
- **Description**: Displays a prompt for query input if the input is from a terminal.
- **Responsibilities**:
  - Check if stdin is a terminal using `isatty`.
  - Print "Query? " if the input is from a terminal.

### tokenize_query()
- **Description**: Splits a query string into individual words and normalizes them.
- **Responsibilities**:
  - Allocate memory for an array of words.
  - Parse the query string to extract words.
  - Normalize each word to lowercase using `normalizeWord`.
  - Handle invalid characters by reporting an error.

### validate_user_query()
- **Description**: Validates the syntax of the query.
- **Responsibilities**:
  - Ensure the query does not start or end with a boolean operator ("and" or "or").
  - Ensure that there are no consecutive boolean operators.
  - Return true if the query is valid, false otherwise.

## Query Execution

### perform_lookup()
- **Description**: Executes the query against the index data and finds matching documents.
- **Responsibilities**:
  - Initialize counters for the result and temporary storage.
  - Retrieve documents for each word in the query using `hashtable_find`.
  - Handle boolean operators by merging or intersecting counters.
  - Return the final set of matched documents.

### intersect_counters()
- **Description**: Intersects two sets of counters.
- **Responsibilities**:
  - Iterate through the first set of counters.
  - For each item, update the count in the first set to the minimum count found in both sets.
  - Use `intersect_counters_helper` to perform the intersection.

### intersect_counters_helper()
- **Description**: Helper function for intersecting counters.
- **Responsibilities**:
  - Set the count in the first set of counters to the minimum count found in both sets.

### merge_counters()
- **Description**: Merges two sets of counters.
- **Responsibilities**:
  - Iterate through the second set of counters.
  - For each item, add the count from the second set to the first set.
  - Use `merge_counters_helper` to perform the merge.

### merge_counters_helper()
- **Description**: Helper function for merging counters.
- **Responsibilities**:
  - Add counts from the second set of counters to the first set.

## Results Display

### rank_search_results()
- **Description**: Displays the matched documents in ranked order.
- **Responsibilities**:
  - Count the total number of matched documents.
  - Find and display the document with the highest score.
  - Print the score, document ID, and URL for each matched document.
  - Use `find_max_score` to determine the highest scoring document.

### count_items()
- **Description**: Helper function to count the size of a counters set.
- **Responsibilities**:
  - Increment the provided counter for each non-zero count in the counters set.

### find_max_score()
- **Description**: Helper function to find the document with the highest score.
- **Responsibilities**:
  - Update the highest scoring document information if the current score is higher than the previous maximum.

## Summary

The querier program is organized into distinct functions, each responsible for a specific task. The `main()` function orchestrates the overall flow, from argument handling to query processing and result display. Helper functions are used extensively to break down complex operations into manageable steps, ensuring code readability and maintainability. Each function is responsible for a well-defined part of the process, making the program modular and easier to understand.