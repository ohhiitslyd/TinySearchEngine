/*
 * index.c - CS50 'common' directory
 *
 * The 'index' module provides the functionality to create, modify, save, and load an index that maps from words to document IDs and counts. This index is used by the 'indexer' and 'querier' modules to facilitate efficient searching of the crawled webpages.
 *
 * CS50
 * Author: Lydia Jin
 * Date: May 6, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "hashtable.h"
#include "counters.h"

typedef hashtable_t index_t;

// Create a new index
index_t* index_new(const int num_slots) {
    return hashtable_new(num_slots);
    
}

//add index
void index_add(index_t *index, const char *word, int docID) {
    counters_t *counters = hashtable_find(index, word);
    if (counters == NULL) {
        counters = counters_new();
        hashtable_insert(index, word, counters);
    }
    counters_add(counters, docID);
}

// Function to delete a counter
void counters_delete_fn(void *item) {
    counters_t *counters = item;
    counters_delete(counters);
    
}

// Delete the index
void index_delete(index_t* index) {
    hashtable_delete(index, counters_delete_fn);
}


// Save an item in the counters
void counters_item_save(void* arg, const int key, const int item) {
    FILE* fp = arg;
    fprintf(fp, "%d %d ", key, item);
}

// Save an item in the index
void index_item_save(void* arg, const char* key, void* item) {
    FILE* fp = arg;
    counters_t* counters = item;

    fprintf(fp, "%s ", key);
    counters_iterate(counters, fp, counters_item_save);
    fprintf(fp, "\n");
}


// Save the index to a file
int index_save(FILE* fp, index_t* index) {
    if (fp == NULL || index == NULL) {
        return 1; 
    }
    hashtable_iterate(index, fp, index_item_save);
    return 0;
    
}

// Load the index from a file
index_t* index_load(FILE* fp) {
  int size = file_numLines(fp);
  index_t* newIndex = index_new(size);
  char* word;
    // Extract data from index file into index struct
    while ((word = file_readWord(fp)) != NULL) {
    counters_t* ctr = counters_new();
    int docID, count;

    //Extract (docID, count) pairs
    while(fscanf(fp, "%d %d", &docID, &count) == 2) {
        counters_set(ctr, docID, count);
    }
    hashtable_insert(newIndex, word, ctr);
    free(word);
    }
    return newIndex;
}

