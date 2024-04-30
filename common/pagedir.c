#include <stdio.h>
#include <stdlib.h>
#include "pagedir.h"

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