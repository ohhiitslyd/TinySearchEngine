# test for no arguments
./indexer

# invalid file directory
./indexer lydia testing

# valid file directory (but no preproduced crawler)
mkdir crawlertesting
./indexer crawlertesting test
rmdir crawlertesting

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