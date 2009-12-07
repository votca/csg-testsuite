#!/bin/sh

echo kmc testsuite `date` > results.txt
# check the return value, 0 == ok, other == failed!
rm -f test_kmc.dat
rm -f test.dat 
./test_kmc > test_kmc.dat
./comp.pl
echo comp.pl: $? >> results.txt

exit 0

