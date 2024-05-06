#include <ctype.h>
#include <string.h>

void normalizeWord(char *word) {
    if (word == NULL) {
        return;
    }

    for (int i = 0; word[i]; i++) {
        word[i] = tolower((unsigned char) word[i]);
    }
}