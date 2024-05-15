#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include "mem.h"
#include "webpage.h"
#include "counters.h"
#include "index.h"
#include "pagedir.h"
#include "word.h"

int fileno(FILE *stream);

// Function prototypes
static void handleArguments(int argc, char *argv[], char **dir, char **indexFile, index_t** indexData);
static void processQuery(char ***queryWords, int* wordCount, index_t* indexData, counters_t** matchedDocs, char* dir);
static void displayPrompt(void);
static bool splitQuery(char *query, char ***queryWords, int *wordCount);
static bool checkQuerySyntax(char **queryWords, int wordCount);
static int isLogicalOperator(char *word);
static counters_t* searchQuery(char **queryWords, int wordCount, index_t* indexData);
static counters_t *retrieveDocs(char *current, char *next, hashtable_t *indexData);
static void intersectCounters(counters_t *mainCounters, counters_t *tempCounters);
static void intersectionHelper(void *arg, const int key, int count);
static void mergeCounters(counters_t *mainCounters, counters_t *tempCounters);
static void mergeHelper(void *arg, const int key, const int count);
static void displayResults(counters_t *matchedDocs, char* dir);
static void countSize(void *arg, const int docID, int count);
static void findTopDoc(void *arg, const int docID, int count);


int main(int argc, char *argv[])
{
    char *dir;  // Directory path for crawler files
    char *indexFile;  // Path to index file
    index_t *indexData;  // Index data structure
    char **queryWords;  // Array of query words
    int wordCount;  // Number of words in the query
    counters_t *matchedDocs;  // Matched documents

    handleArguments(argc, argv, &dir, &indexFile, &indexData);
    processQuery(&queryWords, &wordCount, indexData, &matchedDocs, dir);

    return 0;
}

// Handle command-line arguments and validate them
static void handleArguments(int argc, char *argv[], char **dir, char **indexFile, index_t** indexData)
{
    if (argc != 3)
    {
        fprintf(stderr, "Error: Incorrect number of arguments provided.\n");
        exit(1);
    }
    if (!dir || !indexFile || !indexData)
    {
        fprintf(stderr, "Error: One or more required pointers are null.\n");
        exit(1);
    }

    *dir = argv[1];
    if (!pagedir_validate(*dir))
    {
        fprintf(stderr, "Error: pageDirectory validation failed.\n");
        exit(1);
    }

    *indexFile = argv[2];
    FILE *fp = fopen(*indexFile, "r");
    if (!fp)
    {
        fprintf(stderr, "Error: Cannot open index file at the specified path.\n");
        exit(1);
    }
    *indexData = index_load(fp);
    if (!*indexData)
    {
        fprintf(stderr, "Error: Failed to load the index data from file.\n");
        exit(1);
    }
    fclose(fp);
}

// Process the user query from stdin
static void processQuery(char ***queryWords, int *wordCount, index_t* indexData, counters_t** matchedDocs, char* dir)
{
    if (!queryWords || !wordCount || !indexData || !matchedDocs) {
        fprintf(stderr, "Error: Invalid pointer(s) provided.\n");
        exit(EXIT_FAILURE);
    }

    char *line;
    while ((line = file_readLine(stdin)) != NULL) {  // Read each line of query input
        displayPrompt();

        if (splitQuery(line, queryWords, wordCount) && checkQuerySyntax(*queryWords, *wordCount)) {
            *matchedDocs = searchQuery(*queryWords, *wordCount, indexData);
            if (*matchedDocs != NULL) {
                displayResults(*matchedDocs, dir);
                free(*queryWords);  // Free memory allocated for queryWords
            }
        }

        free(line);  // Free memory allocated for the line
    }

    index_delete(indexData);  // Clean up the index data structure
}

// Display a query prompt if input is from a terminal
static void displayPrompt(void)
{
    if (isatty(fileno(stdin)))
    {
        printf("Query? ");
    }
}

// Split the query into words and tokenize
static bool splitQuery(char* query, char ***queryWords, int* wordCount)
{
    if (!query || !queryWords || !wordCount) {
        fprintf(stderr, "Error: Invalid pointer(s) provided.\n");
        return false;
    }

    int queryLen = strlen(query);
    if (queryLen <= 1) {
        fprintf(stderr, "Error: Invalid query.\n");
        return false;
    }

    int arrayLen = queryLen / 2;  // Estimate the number of words
    *queryWords = (char **) malloc(arrayLen * sizeof(char *));
    if (!*queryWords) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return false;
    }

    *wordCount = 0;
    bool onWord = false;
    char *wordStart = NULL;

    for (int i = 0; i < queryLen; i++, query++) {
        if (isspace(*query)) {
            if (onWord) {
                onWord = false;
                *query = '\0';  // Null-terminate the word
            }
        } else if (isalpha(*query)) {
            *query = tolower(*query);  // Convert to lowercase
            if (!onWord) {
                onWord = true;
                wordStart = query;
                (*queryWords)[(*wordCount)++] = wordStart;
            }
        } else {
            fprintf(stderr, "Error: Invalid character in query.\n");
            free(*queryWords);
            return false;
        }
    }

    return true;
}

// Check if the query syntax is correct
static bool checkQuerySyntax(char **queryWords, int wordCount)
{
    if (!queryWords || wordCount < 0) {
        fprintf(stderr, "Error: Invalid pointer(s) provided or negative word count.\n");
        return false;
    }

    printf("\nQuery:");
    for (int i = 0; i < wordCount; i++) {
        printf(" %s", queryWords[i]);
    }
    printf("\n");

    if (isLogicalOperator(queryWords[0]) > 0 || isLogicalOperator(queryWords[wordCount - 1]) > 0) {
        printf("Error: Query cannot start or end with a boolean operator.\n");
        free(queryWords);
        return false;
    }

    for (int i = 1; i < wordCount - 1; i++) {
        if (isLogicalOperator(queryWords[i]) > 0 && isLogicalOperator(queryWords[i + 1]) > 0) {
            printf("Error: Query cannot have consecutive boolean operators.\n");
            free(queryWords);
            return false;
        }
    }

    return true;
}

// Check if the word is a logical operator (and/or)
static int isLogicalOperator(char *word)
{
    if (!word) {
        fprintf(stderr, "Error: Null pointer passed for operator check.\n");
        return -1;
    }
    if (strcmp(word, "and") == 0) {
        return 1;
    }
    if (strcmp(word, "or") == 0) {
        return 2;
    }
    return 0;
}

// Search the query in the index and find matching documents
static counters_t* searchQuery(char **queryWords, int wordCount, index_t* indexData)
{
    if (!queryWords || !indexData) {
        fprintf(stderr, "Error: Invalid pointer(s) provided.\n");
        return NULL;
    }

    counters_t *result = counters_new();  // Initialize result counters
    counters_t *temp = counters_new();  // Temporary counters
    if (!result || !temp) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(queryWords);
        return NULL;
    }

    counters_t *wordDocs = retrieveDocs(queryWords[0], queryWords[0], indexData);
    if (!wordDocs) {
        counters_delete(result);
        counters_delete(temp);
        free(queryWords);
        return NULL;
    }

    mergeCounters(temp, wordDocs);  // Merge initial word documents

    for (int i = 1; i < wordCount; i++) {
        char *curr = queryWords[i];
        char *next = queryWords[i + 1];
        wordDocs = retrieveDocs(curr, next, indexData);
        if (!curr || ((i < wordCount - 1) && !next)) {
            fprintf(stderr, "Error: Invalid word(s) in query.\n");
            counters_delete(result);
            counters_delete(temp);
            free(queryWords);
            return NULL;
        }
        if (!wordDocs) {
            counters_delete(result);
            counters_delete(temp);
            free(queryWords);
            return NULL;
        }

        int operatorType = isLogicalOperator(curr);
        if (operatorType >= 2) {  // "or" operator
            mergeCounters(result, temp);
            counters_delete(temp);
            temp = counters_new();
            mergeCounters(temp, wordDocs);
            i++;
        } else if (operatorType == 1) {  // "and" operator
            intersectCounters(temp, wordDocs);
            i++;
        } else if (operatorType == 0) {  // Normal word
            intersectCounters(temp, wordDocs);
        }
    }

    mergeCounters(result, temp);  // Final merge of counters
    counters_delete(temp);

    return result;
}

// Retrieve documents containing the specified word
static counters_t *retrieveDocs(char *current, char *next, hashtable_t *indexData)
{
    if (!current || !indexData || (isLogicalOperator(current) >= 1 && !next)) {
        fprintf(stderr, "Error: Invalid pointer(s) provided.\n");
        return NULL;
    }

    char *word = (isLogicalOperator(current) >= 1) ? next : current;

    counters_t *wordDocs = hashtable_find(indexData, word);
    if (!wordDocs) {
        printf("Error: No documents match word '%s'.\n", word);
        return NULL;
    }

    return wordDocs;
}

// Intersect two counters objects
static void intersectCounters(counters_t *mainCounters, counters_t *tempCounters)
{
    counters_iterate(tempCounters, mainCounters, &intersectionHelper);
}

// Helper function for intersecting counters
static void intersectionHelper(void *arg, const int key, int count)
{
    counters_t *mainCounters = arg;
    if (!mainCounters) {
        fprintf(stderr, "Error: Invalid pointer provided.\n");
        return;
    }

    int countA = counters_get(mainCounters, key);
    int countB = count;

    if (countA != 0) {
        counters_set(mainCounters, key, (countA < countB) ? countA : countB);
    }
}

// Merge two counters objects
static void mergeCounters(counters_t *mainCounters, counters_t *tempCounters)
{
    counters_iterate(tempCounters, mainCounters, &mergeHelper);
}

// Helper function for merging counters
static void mergeHelper(void *arg, const int key, const int count)
{
    counters_t *mainCounters = arg;
    if (!mainCounters) {
        fprintf(stderr, "Error: Invalid argument for merge helper function.\n");
        return;
    }
    int countB = count;
    int countA = counters_get(mainCounters, key);

    if (countA != 0) {
        counters_set(mainCounters, key, countA + countB);
    } else {
        counters_set(mainCounters, key, countB);
    }
}

// Display the matched documents
static void displayResults(counters_t *matchedDocs, char* dir)
{
    if (!matchedDocs || !dir) {
        fprintf(stderr, "Error: Invalid parameters for printing matched documents.\n");
        return;
    }

    int ctrsSize = 0;
    int bestDoc[2] = {0, 0};
    counters_iterate(matchedDocs, &ctrsSize, &countSize);
    printf("Matches %d documents (ranked):\n", ctrsSize);

    for (int i = 0; i < ctrsSize; i++) {
        counters_iterate(matchedDocs, bestDoc, &findTopDoc);

        char *docFilename = malloc(strlen(dir) + (floor(log10(bestDoc[0])) + 1) + 2);
        if (sprintf(docFilename, "%s/%d", dir, bestDoc[0]) < 0) {
            fprintf(stderr, "Error: Failed to construct document file pathname.\n");
            return;
        }

        FILE *fp = fopen(docFilename, "r");
        if (!fp) {
            free(docFilename);
            fprintf(stderr, "Error: Failed to open document file.\n");
            return;
        }
        char *URL = file_readLine(fp);
        if (!URL) {
            free(docFilename);
            fprintf(stderr, "Error: Failed to read document file.\n");
            return;
        }

        printf("score %3d - doc %4d:  %s\n", bestDoc[1], bestDoc[0], URL);

        counters_set(matchedDocs, bestDoc[0], 0);

        free(docFilename);
        free(URL);
        fclose(fp);
        bestDoc[0] = 0;
        bestDoc[1] = 0;
    }

    counters_delete(matchedDocs);
}

// Helper function to count the size of the counters
static void countSize(void *arg, const int docID, int count)
{
    int *ctrsSize = arg;
    if (!ctrsSize) {
        fprintf(stderr, "Error: Failed to initialize counter for tracking size.\n");
    }
    (*ctrsSize)++;
}

// Helper function to find the top document by score
static void findTopDoc(void *arg, const int docID, int count)
{
    int* bestDoc = arg;
    if (!bestDoc) {
        fprintf(stderr, "Error: Failed to access array for storing best document info.\n");
        return;
    }
    if (count > bestDoc[1]) {
        bestDoc[0] = docID;
        bestDoc[1] = count;
    }
}