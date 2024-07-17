## Overview
The crawler.c module is part of the Tiny Search Engine project developed for Dartmouth's CS50 course. It is responsible for crawling webpages starting from a given seed URL. The module fetches webpages, scans them for embedded URLs, and repeats the process for all found links up to a specified maximum depth.

#### Components
Headers and Libraries
The module includes necessary standard libraries and custom libraries:

Standard Libraries: <stdio.h>, <stdlib.h>, <string.h>, <unistd.h>, <sys/types.h>, <sys/stat.h>
Custom Libraries: bag.h, hashtable.h, webpage.h, pagedir.h, file.h
MAX_DEPTH: Defines the maximum crawling depth (set to 10).

int main(int argc, char *argv[]) --
The entry point of the program. It performs the following tasks:

Argument Validation: Ensures the correct number of arguments are provided and validates them.
Directory Checks: Confirms the provided page directory is valid, writable, and can be used for saving crawled pages.
Initialize Data Structures: Creates a bag_t for pages to crawl and a hashtable_t for processed pages.
Create Seed Page: Initializes the seed webpage and starts the crawling process.
Memory Cleanup: Frees allocated memory and deletes data structures after crawling is completed.
void crawler(hashtable_t *processedPages, bag_t *toCrawl, char *pageDirectory, webpage_t *seedPage, int maxDepth)
The core crawling function. It performs the following tasks:

Initialization: Inserts the seed page into the bag and hashtable.
Crawling Loop: Continuously extracts pages from the bag, fetches their content, saves them to the specified directory, and scans for new URLs.
Page Fetching: Fetches the HTML content of each page and handles errors.
Page Scanning: Scans each fetched page for new URLs using the pagescanner function.
Memory Management: Deletes pages that are not the seed page to free memory.
void pagescanner(hashtable_t *processedPages, bag_t *toCrawl, webpage_t *page, int maxDepth)
Scans a webpage for embedded URLs and performs the following tasks:

URL Extraction: Extracts URLs from the page's HTML content.
Normalization and Validation: Normalizes each extracted URL and checks if it has already been processed or is an internal URL.
Duplication Check: Checks if the URL has already been processed to avoid duplication.
Depth Check: Ensures the depth of the URL does not exceed the maximum depth.
Insertion: Adds new valid URLs to the bag and hashtable for further crawling.
Logging: Logs actions such as found URLs, ignored duplicates, and external URLs.
inline static void logr(const char *action, char *url, int depth)
Logs actions performed during the crawling process. It takes the following parameters:

action: A string describing the action performed.
url: The URL involved in the action.
depth: The depth at which the action occurred.

## Usage
To build the project, run:
make

To run tests, use:
make test

To clean up build artifacts, use:
make clean

This implementation provides a robust framework for crawling websites, ensuring efficient processing and management of URLs, and saving fetched pages in a specified directory.
