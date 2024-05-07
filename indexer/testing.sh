# test for no arguments
./indexer

# test for one argument
./indexer arg1

# test for three arguments
./indexer arg1 arg2 arg3

# invalid file directory
./indexer lydia testing

# valid file directory (but no preproduced crawler)
mkdir testingdir
./indexer testingdir test
rmdir testingdir

# invalid pageDirectory (not a crawler directory)
mkdir not_a_crawler_dir
touch not_a_crawler_dir/non_webpage_file
./indexer not_a_crawler_dir test
rm -r not_a_crawler_dir

# unable to write to file
echo file > file_test
chmod ugo=rx file_test
./indexer ../crawler/pageDirectory file_test
chmod ugo=rwx file_test
more file_test
rm file_test -f

# testing writable file
echo '' > file_test
chmod ugo=rx file_test
./indextest testoutput file_test
chmod ugo=rwx file_test
more file_test
rm file_test -f

# valid test
./indexer ../crawler/pageDirectory test

# run valgrind on indexer and indextest
valgrind --leak-check=full ./indexer ../crawler/pageDirectory test
valgrind --leak-check=full ./indextest test testoutput