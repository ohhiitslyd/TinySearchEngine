#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/file.h"
#include "../libcs50/hash.h" // for Jenkins hash

#define MAX_DEPTH 10

void processWebpage(void *arg, void *item);
void dummyDelete(void *item);

typedef struct crawlerData {
    hashtable_t *processedPages;
    bag_t *toCrawl;
    char *pageDirectory;
} crawlerData_t;

void dummyDelete(void *item) {
    // Do nothing
}

void printWebpageURL(FILE *fp, void *item) {
    webpage_t *page = item;
    fprintf(fp, "%s\n", webpage_getURL(page));
}

inline static void logr(const char *action, char *url, int depth){
    fprintf(stdout, "%2d %9s: %s \n", depth, action, url);
}

void processWebpage(void *arg, void *item) {
    static int id = 1; 
    crawlerData_t *crawlerData = (crawlerData_t *)arg;
    hashtable_t *processedPages = crawlerData->processedPages;
    bag_t *toCrawl = crawlerData->toCrawl;
    char *pageDirectory = crawlerData->pageDirectory;
    webpage_t *page = (webpage_t *)item;

    printf("%2d Webpage Fetched: %s\n", webpage_getDepth(page), webpage_getURL(page));
    printf("%2d Scanning Webpage: %s\n", webpage_getDepth(page), webpage_getURL(page));

    if (webpage_fetch(page)) {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/%d", pageDirectory, id++);
        FILE *fp = fopen(filename, "w");
        if (fp != NULL) {
            fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
            fclose(fp);
        } else {
            fprintf(stderr, "Failed to open file '%s'\n", filename);
        }

        int pos = 0;
        char *result;
        while ((result = webpage_getNextURL(page, &pos)) != NULL) {
            char *normalizedURL = normalizeURL(result);
            if (normalizedURL != NULL) {
                printf("%2d Webpage Seen: %s\n", webpage_getDepth(page) + 1, normalizedURL);
                if (isInternalURL(normalizedURL)) {
                    if (!hashtable_insert(processedPages, strdup(normalizedURL), "")) {
                        printf("%2d Duplicate: %s\n", webpage_getDepth(page) + 1, normalizedURL);
                    } else if (webpage_getDepth(page) < MAX_DEPTH) {
                        webpage_t *newPage = webpage_new(strdup(normalizedURL), webpage_getDepth(page) + 1, NULL);
                        if (newPage != NULL) {
                            bag_insert(toCrawl, newPage);
                            printf("%2d Page Inserted: %s\n", webpage_getDepth(page) + 1, normalizedURL);
                        } else {
                            fprintf(stderr, "Failed to create new page\n");
                            free(normalizedURL);
                            free(result);
                            continue;
                        }
                    }
                } else {
                    printf("%2d Unnecessary: %s\n", webpage_getDepth(page) + 1, normalizedURL);
                }
                free(normalizedURL);
                free(result);
            }
            else {
                fprintf(stderr, "Failed to normalize URL\n");
                free(result);
                continue;
            }
        }
        
    } else {
        fprintf(stderr, "Failed to fetch page: %s\n", webpage_getURL(page));
    }

    hashtable_insert(processedPages, webpage_getURL(page), page);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        return 1;
    }

    char *seedURL = argv[1];
    char *pageDirectory = argv[2];
    int maxDepth = atoi(argv[3]);

    if (!isInternalURL(seedURL)) {
        fprintf(stderr, "Invalid seed URL\n");
        return 1;
    }

    struct stat s;
    if (stat(pageDirectory, &s) == -1) {
        fprintf(stderr, "Error: Cannot find the directory '%s'\n", pageDirectory);
        return 1;
    } else if (!S_ISDIR(s.st_mode)) {
        fprintf(stderr, "Error: '%s' is not a directory\n", pageDirectory);
        return 1;
    } else if (access(pageDirectory, W_OK) == -1) {
        fprintf(stderr, "Error: Directory '%s' is not writable\n", pageDirectory);
        return 1;
    }

    if (maxDepth < 0 || maxDepth > MAX_DEPTH) {
        fprintf(stderr, "Max depth must be between 0 and 10\n");
        return 1;
    }

    bag_t *toCrawl = bag_new();
    if (toCrawl == NULL) {
        fprintf(stderr, "Failed to create bag\n");
        return 1;
    }
    hashtable_t *processedPages = hashtable_new(100);
    if (processedPages == NULL) {
        fprintf(stderr, "Failed to create hashtable\n");
        bag_delete(toCrawl, NULL);
        return 1;
    }

    webpage_t *seedPage = webpage_new(seedURL, 0, NULL);
    if (seedPage == NULL || seedURL == NULL) {
        fprintf(stderr, "Failed to create seed page\n");
        bag_delete(toCrawl, NULL);
        hashtable_delete(processedPages, NULL);
        return 1;
    }
    bag_insert(toCrawl, seedPage);
    hashtable_insert(processedPages, strdup(seedURL), seedPage);

    crawlerData_t crawlerData;
    crawlerData.processedPages = processedPages;
    crawlerData.toCrawl = toCrawl;
    crawlerData.pageDirectory = pageDirectory;

    // Keep crawling until the bag is empty
    webpage_t *page;
    while ((page = bag_extract(toCrawl)) != NULL) {
        processWebpage(&crawlerData, page);
    }

    bag_delete(toCrawl, dummyDelete);
    hashtable_delete(processedPages, dummyDelete);

    return 0;
}