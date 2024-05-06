/*
 * pagedir.h - CS50 'pagedir' module
 *
 * see pagedir.c for implementation.
 *
 * CS50
 * Author: Lydia Jin
 * Date: April 29, 2024
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/* Initialize a page directory. */
bool pagedir_init(const char* pageDirectory);

/* Save a webpage to a page directory. */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/* Validate a page directory. */
bool pagedir_validate(const char* pageDirectory);

/* Load a webpage from a page directory. */
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H