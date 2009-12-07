#!/bin/sh

summary="results.txt"

rm -f $summary      

echo tools `date` > $summary     
# check the return value, 0 == ok, other failed!

./basic
echo basic: $? >> $summary   

./property_test
echo propert_test: $? >> $summary

./spline_test
echo spline_test: $? >> $summary

exit 0

