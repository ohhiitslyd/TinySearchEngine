/*
 * word.c - CS50 'common' directory
 *
 * The 'word' module provides the functionality to normalize a word by converting all characters to lowercase.
 *
 * CS50
 * Author: Lydia Jin
 * Date: May 6, 2024
 */

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