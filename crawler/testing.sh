# #!/bin/bash

# # Test program with incorrect command-line arguments
# echo "Testing with incorrect command-line arguments..."
# ./crawler
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html 
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pageDirectory/ 2 extra_arg
# ./crawler not_a_url ./output 2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html not_a_directory 2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pageDirectory/ not_a_depth
# echo

# # Crawl a simple, closed set of cross-linked web pages
# echo "Crawling a simple, closed set of cross-linked web pages..."
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pageDirectory/ 0
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pageDirectory/ 1
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pageDirectory/ 2
# echo

# # Repeat with a different seed page
# echo "Crawling with a different seed page..."
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ./pageDirectory/ 2
# echo

# # Point the crawler at one of our bigger playgrounds
# echo "Crawling a bigger playground..."
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pageDirectory/ 0
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pageDirectory/ 1
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pageDirectory/ 2
# echo

# # Test with a greater depth
# echo "Crawling with a greater depth..."
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ./pageDirectory/ 3

#!/bin/bash

# Run the crawler program
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./pageDirectory/ 10

# Print the exit status of the crawler program
echo "Exit status: $?"

# Optionally, you can add commands to check the output files or clean up the output directory