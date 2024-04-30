#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdbool.h>
#include "../libcs50/webpage.h"

bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H