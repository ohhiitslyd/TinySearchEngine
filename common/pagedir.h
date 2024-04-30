/*
 * pagedir.h - CS50 'pagedir' module
 *
 * This module is used for creating a page directory and saving webpages.
 *
 * CS50
 * Author: Lydia Jin
 * Date: April 29, 2024
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdbool.h>
#include "../libcs50/webpage.h"

/*
 * pagedir_init - Initialize a new page directory.
 * @pageDirectory: The directory to initialize.
 *
 * Return: true if successful, false otherwise.
 */
bool pagedir_init(const char* pageDirectory);

/*
 * pagedir_save - Save a webpage to a page directory.
 * @page: The webpage to save.
 * @pageDirectory: The directory to save the webpage in.
 * @docID: The document ID to assign to the saved webpage.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H