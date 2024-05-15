# Querier Design Specification

The querier program is designed to handle multiple tasks including parameter checking, query parsing and validation, searching for relevant documents, scoring and ranking them, and printing the results. This design specification outlines the key components and data structures used in the querier program.

## Tasks Overview

1. **Checking Parameters**: Validates command-line arguments and ensures necessary files and directories are accessible.
2. **Cleaning and Parsing User Queries**: Processes input queries, normalizes and validates them.
3. **Identifying Relevant Documents**: Searches for documents that match the query terms using an index data structure.
4. **Scoring and Ranking**: Scores the matched documents based on query relevance and ranks them in decreasing order.
5. **Printing Search Results**: Outputs the search results in a structured format.

## Data Structures

### `counters_t`
- **Description**: A structure used to count occurrences of words or scores of documents.
- **Usage**: Holds document IDs and their corresponding counts or scores.

### `index_t`
- **Description**: A structure that stores the word frequency data.
- **Usage**: Maps words to their corresponding `counters_t` structures, which hold document IDs and word counts.

### `two_counters`
- **Description**: A structure that bundles two `counters_t` pointers.
- **Usage**: Facilitates passing and manipulating two counters together, useful for union and intersection operations.
- **Members**:
  - `counters_t* ctrsA`: First counters structure.
  - `counters_t* ctrsB`: Second counters structure.

### `doc_t`
- **Description**: A structure that holds a document's key ID and an integer counter.
- **Usage**: Keeps track of the count of word occurrences or the score of a document.
- **Members**:
  - `int docID`: Document key ID.
  - `int count`: Count or score of the document.

### `all_docs`
- **Description**: A structure that contains an array of `doc_t` pointers and an integer for the number of elements.
- **Usage**: Facilitates easy iteration and sorting of documents.
- **Members**:
  - `doc_t** docs`: Array of document pointers.
  - `int numDocs`: Number of documents.

### `index`
- **Description**: A structure that contains all the "and" sequences of a particular query.
- **Usage**: Stores (word, counters) pairings, allowing comparison and accumulation of scores for documents across multiple terms.
- **Members**:
  - `char* key`: Arbitrary key.
  - `counters_t* counters`: Counters for each word.

## Functions Overview

### Parameter Checking
- **handleArguments()**: Validates command-line arguments and initializes necessary data structures.

### Query Parsing
- **processQuery()**: Processes queries read from stdin until EOF or an error occurs.
- **splitQuery()**: Splits a query string into individual words and normalizes them.
- **checkQuerySyntax()**: Validates the syntax of the query.
- **isLogicalOperator()**: Checks if a given word is a boolean operator.

### Query Execution
- **searchQuery()**: Executes the query against the index data and finds matching documents.
- **retrieveDocs()**: Retrieves documents that contain a specified word.
- **intersectCounters()**: Intersects two sets of counters.
- **intersectionHelper()**: Helper function for intersecting counters.
- **mergeCounters()**: Merges two sets of counters.
- **mergeHelper()**: Helper function for merging counters.

### Results Display
- **displayResults()**: Displays the matched documents in ranked order.
- **countSize()**: Helper function to count the size of a counters set.
- **findTopDoc()**: Helper function to find the document with the highest score.

## Summary

The querier program is structured to handle different tasks through distinct functions, ensuring efficient processing and maintainability. The key data structures such as `counters_t`, `index_t`, `two_counters`, `doc_t`, and `all_docs` are used to manage and process query-related data effectively. Each function in the querier program plays a specific role in the overall workflow, from parameter validation to query execution and result display. This modular design allows for easy updates and enhancements to the program.