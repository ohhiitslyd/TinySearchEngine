# Querier Implementation Specification

The querier program is responsible for parsing user queries, validating them, searching for matching documents based on the queries, and displaying the results in a ranked order. The implementation can be broken down into the following components:

## Main Function

### main()
- **Description**: The entry point of the program. It handles argument parsing, initialization, and cleanup.
- **Responsibilities**:
  - Parse command-line arguments.
  - Initialize and load the index data structure containing word frequency information.
  - Call the function to process the query.
  - Clean up by deleting the index data structure.

## Argument Handling

### handleArguments()
- **Description**: Validates command-line arguments and initializes the necessary data structures.
- **Responsibilities**:
  - Ensure that exactly three arguments are provided.
  - Validate that the provided directory is a crawler directory.
  - Verify that the provided index file is readable.
  - Load the index data from the file.

## Query Parsing

### processQuery()
- **Description**: Processes queries read from stdin until EOF or an error occurs.
- **Responsibilities**:
  - Continuously read lines of input from stdin.
  - Display a prompt for query input.
  - Clean and split the input line into words.
  - Validate the query syntax.
  - Execute the query and display the results.
  - Clean up the allocated memory for the query words.

### splitQuery()
- **Description**: Splits a query string into individual words and normalizes them.
- **Responsibilities**:
  - Allocate memory for an array of words.
  - Parse the query string to extract words.
  - Normalize each word to lowercase.
  - Handle invalid characters by reporting an error.

### checkQuerySyntax()
- **Description**: Validates the syntax of the query.
- **Responsibilities**:
  - Ensure the query does not start or end with a boolean operator ("and" or "or").
  - Ensure that there are no consecutive boolean operators.
  - Print the validated query.

### isLogicalOperator()
- **Description**: Checks if a given word is a boolean operator.
- **Responsibilities**:
  - Return true if the word is "and" or "or".
  - Return false otherwise.

## Query Execution

### searchQuery()
- **Description**: Executes the query against the index data and finds matching documents.
- **Responsibilities**:
  - Initialize counters for the result and temporary storage.
  - Retrieve documents for each word in the query.
  - Handle boolean operators by merging or intersecting counters.
  - Return the final set of matched documents.

### retrieveDocs()
- **Description**: Retrieves documents that contain a specified word.
- **Responsibilities**:
  - Find the counters for the word in the index data.
  - Return the counters representing the documents.

### intersectCounters()
- **Description**: Intersects two sets of counters.
- **Responsibilities**:
  - Iterate through the second set of counters and update the first set to reflect the intersection.

### intersectionHelper()
- **Description**: Helper function for intersecting counters.
- **Responsibilities**:
  - Set the count in the first set of counters to the minimum count found in both sets.

### mergeCounters()
- **Description**: Merges two sets of counters.
- **Responsibilities**:
  - Iterate through the second set of counters and update the first set to reflect the union.

### mergeHelper()
- **Description**: Helper function for merging counters.
- **Responsibilities**:
  - Add counts from the second set of counters to the first set.

## Results Display

### displayResults()
- **Description**: Displays the matched documents in ranked order.
- **Responsibilities**:
  - Count the total number of matched documents.
  - Find and display the document with the highest score.
  - Print the score, document ID, and URL for each matched document.

### countSize()
- **Description**: Helper function to count the size of a counters set.
- **Responsibilities**:
  - Increment the provided counter for each document.

### findTopDoc()
- **Description**: Helper function to find the document with the highest score.
- **Responsibilities**:
  - Update the highest scoring document information.

## Summary

The querier program is organized into distinct functions, each responsible for a specific task. The main() function orchestrates the overall flow, from argument handling to query processing and result display. Helper functions are used extensively to break down complex operations into manageable steps, ensuring code readability and maintainability.