/*
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * CS50
 * Author: Lydia Jin
 * Date: April 29, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "pagedir.h"
#include "../libcs50/file.h"

bool pagedir_init(const char* pageDirectory) {
    char crawlerFilePath[256];
    snprintf(crawlerFilePath, sizeof(crawlerFilePath), "%s/.crawler", pageDirectory);
    FILE *fp = fopen(crawlerFilePath, "w");
    if (fp != NULL) {
        fclose(fp);
        return true;
    } else {
        return false;
    }
}

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    char pageFilePath[256];
    snprintf(pageFilePath, sizeof(pageFilePath), "%s/%d", pageDirectory, docID);
    FILE *fp = fopen(pageFilePath, "w");
    if (fp != NULL) {
        fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
        fclose(fp);
    }
}

bool pagedir_validate(const char* pageDirectory) {
    char crawlerFilePath[256];
    snprintf(crawlerFilePath, sizeof(crawlerFilePath), "%s/.crawler", pageDirectory);
    FILE *fp = fopen(crawlerFilePath, "r");
    if (fp != NULL) {
        fclose(fp);
        return true;
    } else {
        return false;
    }
}

webpage_t* pagedir_load(const char* pageDirectory, const int docID) {
    char pageFilePath[256];
    snprintf(pageFilePath, sizeof(pageFilePath), "%s/%d", pageDirectory, docID);
    FILE *fp = fopen(pageFilePath, "r");
    if (fp != NULL) {
        char *url = malloc(256);

        if (url == NULL) {
            // If malloc failed, close the file and return NULL
            fclose(fp);
            free(url);
            return NULL;
        }
        

        webpage_t *page = webpage_new(url, 0, file_readFile(fp));
        fclose(fp);
        return page;
    } else {
        return NULL;
    }
}