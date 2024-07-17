# Tiny Search Engine
## Lydia Jin (@ohhiitslyd)

This Tiny Search Engine project, developed for Dartmouth's CS50 course (Software Design & Implementation), is structured into three main components:

Crawler: This component traverses a website, retrieving webpages to a specified depth, and extracts embedded URLs.

Indexer: It constructs an inverted index from the crawler's directory, mapping words to their corresponding document IDs and counts.

Querier: Based on user queries, this component returns relevant webpages, supporting both 'and' and 'or' operators.

For detailed implementation specifications and additional information, please refer to the individual directories.

To build the project, use the command:
make

To run tests, use:
make test

To clean up, use:
make clean

The pageDirectory folder is included to store the .crawler and numbered files.
