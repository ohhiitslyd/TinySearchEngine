#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

typedef hashtable_t index_t;

/* Create a new index. */
index_t* index_new(const int num_slots);

/* Add a word with its document ID to the index. */
void index_add(index_t *index, const char *word, int docID);

/* Delete the index. */
void index_delete(index_t* index);

/* Save the index to a file. */
int index_save(FILE* fp, index_t* index);

/* Load the index from a file. */
index_t* index_load(FILE* fp);

#endif // __INDEX_H