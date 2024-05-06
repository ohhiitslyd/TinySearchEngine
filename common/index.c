#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

typedef hashtable_t index_t;

// Create a new index
index_t* index_new(const int num_slots) {
    printf("Index created\n");
    return hashtable_new(num_slots);
    
}

void index_add(index_t *index, const char *word, int docID) {
    counters_t *counters = hashtable_find(index, word);
    printf("Index added for %s with docID %d\n", word, docID);
    if (counters == NULL) {
        counters = counters_new();
        hashtable_insert(index, word, counters);
    }
    counters_add(counters, docID);
    printf("Index added for %s\n", word);
}

// // Delete the index
// void index_delete(index_t* index) {
//     hashtable_delete(index, NULL);
//     printf("Index deleted\n");
// }

// Save an item in the counters
void counters_item_save(void* arg, const int key, const int item) {
    FILE* fp = arg;
    fprintf(fp, "%d %d ", key, item);
    printf("Counters item saved\n");
}

// Save an item in the index
void index_item_save(void* arg, const char* key, void* item) {
    FILE* fp = arg;
    counters_t* counters = item;

    fprintf(fp, "%s ", key);
    counters_iterate(counters, fp, counters_item_save);
    fprintf(fp, "\n");
    printf("Index item saved\n");
}


// Save the index to a file
int index_save(FILE* fp, index_t* index) {
    if (fp == NULL || index == NULL) {
        return 1; 
    }
    hashtable_iterate(index, fp, index_item_save);
    printf("Index saved\n");
    return 0;
    
}

// Load the index from a file
index_t* index_load(FILE* fp) {
    index_t* index = index_new(100); // Adjust size as needed

    char* word;
    while ((word = file_readWord(fp)) != NULL) {
        counters_t* counters = counters_new();
        int docID, count;
        while (fscanf(fp, "%d %d ", &docID, &count) == 2) {
            counters_set(counters, docID, count);
        }
        hashtable_insert(index, word, counters);
        printf("%s inserted in hashtable\n", word);
        free(word);
    }

    return index;
}

