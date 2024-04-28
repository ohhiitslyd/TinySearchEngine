#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"

bool pagedir_save(const char *pageDirectory, const webpage_t *page, int id) {
    FILE *fp = fopen(pageDirectory, "a");
    if (fp == NULL) {
        return false;
    }

    fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

    fclose(fp);
    return true;
}

webpage_t *pagedir_load(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    char url[100];
    int depth;
    char *html = NULL;
    size_t len = 0;

    fscanf(fp, "%s", url);
    fscanf(fp, "%d", &depth);
    getline(&html, &len, fp);

    webpage_t *page = webpage_new(url, depth, html);

    free(html);
    fclose(fp);
    return page;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./pagedir pageDirectory filename\n");
        return 1;
    }

    const char *pageDirectory = argv[1];
    const char *filename = argv[2];

    // Create a dummy webpage for testing
    webpage_t *page = webpage_new("http://example.com", 0, "<html>Example</html>");

    if (!pagedir_save(pageDirectory, page, 0)) {
        fprintf(stderr, "Error: Could not save webpage to %s\n", pageDirectory);
        webpage_delete(page);
        return 1;
    }

    webpage_t *loaded_page = pagedir_load(filename);
    if (loaded_page == NULL) {
        fprintf(stderr, "Error: Could not load webpage from %s\n", filename);
        webpage_delete(page);
        return 1;
    }

    // Do something with loaded_page...

    webpage_delete(page);
    webpage_delete(loaded_page);

    return 0;
}