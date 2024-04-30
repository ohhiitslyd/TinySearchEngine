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

void processWebpage(hashtable_t *processedPages, bag_t *toCrawl, char *pageDirectory, webpage_t *page);

void printHash(FILE *fp, const char *key, void *item) {
    fprintf(fp, "%s : %p", key, item);
}

void printBag(FILE *fp, void *item) {
    webpage_t *page = item;
    fprintf(fp, "%s", webpage_getURL(page));
}

void delete_hashtable_item(void* arg, const char* key, void* item) {
    // Delete the webpage
    free(item);

    // Free the key if it was dynamically allocated
    free((char*)key);
}


inline static void logr(const char *action, char *url, int depth){
    fprintf(stdout, "%2d %9s: %s \n", depth, action, url);
}

void processWebpage(hashtable_t *processedPages, bag_t *toCrawl, char *pageDirectory, webpage_t *page) {
    static int id = 0; 

    printf("%2d Fetched: %s\n", webpage_getDepth(page), webpage_getURL(page));
    printf("%2d Scanning: %s\n", webpage_getDepth(page), webpage_getURL(page));

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
                printf("%2d Found: %s\n", webpage_getDepth(page), normalizedURL);
                if (hashtable_find(processedPages, normalizedURL) == NULL) {
                    if (isInternalURL(normalizedURL)) {
                        if (hashtable_find(processedPages, normalizedURL) != NULL) {
                            printf("%2d IgnDupl: %s\n", webpage_getDepth(page), normalizedURL);
                        } else if (webpage_getDepth(page) < MAX_DEPTH) {
                            webpage_t *newPage = webpage_new(strdup(normalizedURL), webpage_getDepth(page) + 1, NULL);
                            if (newPage != NULL) {
                                bag_insert(toCrawl, newPage);
                                hashtable_insert(processedPages, normalizedURL, "");
                                printf("%2d Added: %s\n", webpage_getDepth(page), normalizedURL);
                            } else {
                                fprintf(stderr, "Failed to create new page\n");
                                free(normalizedURL);
                                free(result);
                                continue;
                            }
                        }
                    } else {
                        printf("%2d IgnExtrn: %s\n", webpage_getDepth(page), normalizedURL);
                    }
                } else {
                    printf("%2d IgnDupl: %s\n", webpage_getDepth(page), normalizedURL);
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

    } 
    else {
        fprintf(stderr, "Failed to fetch page: %s\n", webpage_getURL(page));
    }
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
    hashtable_t *processedPages = hashtable_new(50);
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
    hashtable_insert(processedPages, seedURL, "");
    

    // Keep crawling until the bag is empty
    webpage_t *page;
    while ((page = bag_extract(toCrawl)) != NULL) {
        processWebpage(processedPages, toCrawl, pageDirectory, page);
        printf("HTML: %s\n", webpage_getHTML(page));
        printf("URL: %s\n", webpage_getURL(page));

        if(page != seedPage) {
            webpage_delete(page);
        }
    }
    free(webpage_getHTML(seedPage));
    // free(webpage_getURL(seedPage));
    free(seedPage);


    //hashtable_iterate(processedPages, NULL, delete_hashtable_item);
    //bag_delete(toCrawl, webpage_delete);
    bag_delete(toCrawl, NULL);
    //printf("Hash before being deleted:\n");
    //hashtable_print(processedPages, stdout, printHash);
    //printf("\n");
    hashtable_delete(processedPages, NULL);
    //free(processedPages);
    //hashtable_delete(processedPages, NULL);
    //free(seedURL);
    //free(seedPage);

    return 0;
}