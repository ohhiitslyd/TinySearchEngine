/*
 * indexer.c - CS50 'indexer' module
 *
 * Indexer reads the page files produced by the crawler, builds an index that maps from words to document IDs and counts, and write that index to a file.
 *
 * CS50
 * Author: Lydia Jin
 * Date: May 6, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "counters.h"
#include "webpage.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"

void indexPage(webpage_t *page, int docID, index_t *index);
bool pagedir_validate(const char *pageDirectory);
webpage_t *pagedir_load(const char *pageDirectory, int docID);
index_t *indexBuild(const char *pageDirectory);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
        return 1;
    }

    char *pageDirectory = argv[1];
    char *indexFilename = argv[2];

    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Invalid page directory\n");
        return 1;
    }

    index_t *index = indexBuild(pageDirectory);
    if (index == NULL) {
        fprintf(stderr, "Failed to build index\n");
        return 1;
    }

    FILE *fp = fopen(indexFilename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file for writing\n");
        index_delete(index);
        return 1;
    }

    if (index_save(fp, index)) {
        fprintf(stderr, "Failed to save index\n");
        index_delete(index);
        return 1;
    }

    fclose(fp);

    index_delete(index);
    return 0;
}

void indexPage(webpage_t *page, int docID, index_t *index) {
    int pos = 0;
    char *html = webpage_getHTML(page);
    if (html == NULL) {
        return;
    }

    // Skip the first two lines
    int newlineCount = 0;
    while (newlineCount < 2 && html[pos] != '\0') {
        if (html[pos] == '\n') {
            newlineCount++;
        }
        pos++;
    }

    char* word;

    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(word) >= 3) {
            normalizeWord(word);
            counters_t *counters = hashtable_find(index, word);
            if (counters == NULL) {
                counters = counters_new();
                if (counters != NULL) {
                    hashtable_insert(index, word, counters);
                }
            }
            if (counters != NULL) {
                counters_add(counters, docID);
            }
        }
        free(word);
    }
}

index_t *indexBuild(const char *pageDirectory) {
    index_t *index = index_new(100);
    if (index == NULL) {
        return NULL;
    }

    int docID = 1;
    webpage_t *page;
    while ((page = pagedir_load(pageDirectory, docID)) != NULL) {
        indexPage(page, docID, index);
        webpage_delete(page);
        docID++;
    }

    return index;
}