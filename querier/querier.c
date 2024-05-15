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

