#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../libcs50/bag.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/file.h"
#include "../libcs50/hash.h" // for Jenkins hash

#define MAX_DEPTH 10

typedef struct bagitem {
    void *item;
    struct bagitem *next;
} bagitem_t;

typedef struct bag {
    bagitem_t *head;
} bag_t;

// Hash the URL using Jenkins hash function
int hash(const char *url) {
    return hash_jenkins(url, strlen(url));
}

// Check if the bag is empty
bool bag_isempty(bag_t *bag) {
    return bag == NULL || bag->head == NULL;
}

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        return 1;
    }

    char *seedURL = argv[1];
    char *pageDirectory = argv[2];
    int maxDepth = atoi(argv[3]);

    // Validate arguments...
    // Check if seedURL is valid and internal
    if (!isInternalURL(seedURL)) {
        fprintf(stderr, "Invalid seed URL\n");
        return 1;
    }

    // Check if pageDirectory exists and is writable
    struct stat s;
    if (stat(pageDirectory, &s) == -1) {
        fprintf(stderr, "Error: Cannot find the file '%s'\n", pageDirectory);
        return 1;
    } else if (S_ISDIR(s.st_mode)) {
        fprintf(stderr, "Error: '%s' is a directory, not a file\n", pageDirectory);
        return 1;
    } else if (access(pageDirectory, W_OK) == -1) {
        fprintf(stderr, "Error: File '%s' is not writable\n", pageDirectory);
        return 1;
    }

    // Check if maxDepth is an integer in the range [0..10]
    if (maxDepth < 0 || maxDepth > MAX_DEPTH) {
        fprintf(stderr, "Max depth must be between 0 and 10\n");
        return 1;
    }

    // Create .crawler file in pageDirectory
    char crawlerFilePath[256];
    snprintf(crawlerFilePath, sizeof(crawlerFilePath), "%s/.crawler", pageDirectory);
    FILE *crawlerFile = fopen(crawlerFilePath, "w");
    if (crawlerFile == NULL) {
        fprintf(stderr, "Failed to create .crawler file\n");
        return 1;
    }
    fclose(crawlerFile);

    // Initialize queue and visited set
    bag_t *toCrawl = bag_new();
    counters_t *visited = counters_new();

    // Add seedURL to the queue with depth 0
    webpage_t *seedPage = webpage_new(seedURL, 0, NULL);
    bag_insert(toCrawl, seedPage);
    counters_add(visited, hash(seedURL));

    // Main crawl loop
    while (!bag_isempty(toCrawl)) {
        webpage_t *page = bag_extract(toCrawl);

        // Pause for one second
        sleep(1);

        // Fetch the page content of the URL
        if (!webpage_fetch(page)) {
            fprintf(stderr, "Failed to fetch page: %s\n", webpage_getURL(page));
            webpage_delete(page);
            continue;
        }

        // Write the page content to a file in pageDirectory with a unique ID, URL, and depth
        // Use file_writeFile function from file.h to write the content
        // If unable to create or write to a file, print error and return non-zero status

        // If the depth is less than maxDepth, parse the page content for URLs and enqueue each URL with depth+1
        if (webpage_getDepth(page) < maxDepth) {
            // Extract URLs from the page and add them to the queue
            int pos = 0;
            char *result;
            while ((result = webpage_getNextURL(page, &pos)) != NULL) {
                char *normalizedURL = normalizeURL(result);
                if (normalizedURL != NULL) {
                    if (isInternalURL(normalizedURL)) {
                        if (counters_get(visited, hash(normalizedURL)) == 0) {
                            counters_add(visited, hash(normalizedURL));
                            webpage_t *newPage = webpage_new(normalizedURL, webpage_getDepth(page) + 1, NULL);
                            bag_insert(toCrawl, newPage);
                        }
                    }
                    free(normalizedURL);
                }
                free(result);
            }
        }

        webpage_delete(page);
    }

    // Clean up
    bag_delete(toCrawl, webpage_delete);
    counters_delete(visited);

    return 0;
}