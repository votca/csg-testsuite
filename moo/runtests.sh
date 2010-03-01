#!/bin/sh

summary="results.txt"
data="data_files"

rm -f $summary      

date > $summary
# transfer integral in a face-to-face geomery as a function of separation
cd $data
out="out"
test="translate.test"
moo_overlap  > $out
echo "difference in test and output:" ../$summary
diff $out $test

exit 0

