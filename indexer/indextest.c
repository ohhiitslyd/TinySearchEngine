#include <stdio.h>
#include <stdlib.h>
#include "../common/index.c"

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 3) {
        fprintf(stderr, "usage: %s oldIndexFilename newIndexFilename\n", argv[0]);
        return 1;
    }

    // Load the index from the old index file
    FILE *oldIndexFile = fopen(argv[1], "r");
    if (oldIndexFile == NULL) {
        perror("fopen");
        return 1;
    }
    index_t *index = index_load(oldIndexFile);
    fclose(oldIndexFile);

    // Save the index to the new index file
    FILE *newIndexFile = fopen(argv[2], "w");
    if (newIndexFile == NULL) {
        perror("fopen");
        return 1;
    }
    index_save(newIndexFile, index);
    fclose(newIndexFile);

    // Delete the index
    index_delete(index);

    return 0;
}