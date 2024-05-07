### CS50 TSE Indexer
## Implementation Spec
In this document, we reference the Design Specification and focus on the implementation-specific decisions.

Here we focus on the core subset:

- Data structures
- Control flow: pseudo code for overall flow, and for each of the functions
- Detailed function prototypes and their parameters
- Error handling and recovery
- Testing plan

## Data structures
The key data structure is the index, which is a hashtable. Each key in the hashtable is a word and each value is a counterset that maps document IDs to counts.

### Control flow
The Indexer is implemented in one file indexer.c, with main functions.

## main
The main function parses the command-line arguments and initializes other modules. It then calls indexBuild with the pageDirectory as an argument.

## indexBuild
This function creates a new 'index' object and loops over document ID numbers, counting from 1. It loads a webpage from the document file 'pageDirectory/id'. If successful, it passes the webpage and docID to indexPage.

## indexPage
This function steps through each word of the webpage, skips trivial words (less than length 3), normalizes the word (converts to lower case), looks up the word in the index, adding the word to the index if needed, and increments the count of occurrences of this word in this docID.

### Function prototypes
int main(const int argc, char* argv[]);
void indexBuild(char* pageDirectory);
void indexPage(webpage_t *page, const int docID, index_t *index);

### Error handling and recovery
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.

### Testing plan
Here is an implementation-specific testing plan.

## Unit testing
A program indextest will serve as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file.

## Integration testing
The indexer, as a complete program, will be tested by building an index from a pageDirectory, and then the resulting index will be validated by running it through the indextest to ensure it can be loaded.

1. Test indexer with various invalid arguments.
2. no arguments
3. one argument
4. three or more arguments
5. invalid pageDirectory (non-existent path)
6. invalid pageDirectory (not a crawler directory)
7. invalid indexFile (non-existent path)
8. invalid indexFile (read-only directory)
9. invalid indexFile (existing, read-only file)
10. Run indexer on a variety of pageDirectories and use indextest as one means of validating the resulting index.
11. Run valgrind on both indexer and indextest to ensure no memory leaks or errors.