#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdbool.h>
#include "../libcs50/webpage.h"

// Save a webpage to a file
bool pagedir_save(const char *pageDirectory, const webpage_t *page, int id);

// Load a webpage from a file
webpage_t *pagedir_load(const char *filename);

#endif // __PAGEDIR_H