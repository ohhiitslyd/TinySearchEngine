#!/bin/bash

# Test program with incorrect command-line arguments
echo "Testing with incorrect command-line arguments..."
./crawler
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pagedir
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pagedir 2 extra_arg
./crawler not_a_url ./pagedir 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html not_a_directory 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pagedir not_a_depth
echo

# Crawl a simple, closed set of cross-linked web pages
echo "Crawling a simple, closed set of cross-linked web pages..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pagedir 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pagedir 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pagedir 2
echo

# Repeat with a different seed page
echo "Crawling with a different seed page..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ./pagedir 2
echo

# Point the crawler at one of our bigger playgrounds
echo "Crawling a bigger playground..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pagedir 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pagedir 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pagedir 2
echo

# Test with a greater depth
echo "Crawling with a greater depth..."
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pagedir 3