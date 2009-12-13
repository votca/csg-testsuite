#!/bin/bash

tests=$(find . -maxdepth 2 -name run.sh -exec dirname {} \; )

for dir in $tests; do
  test=$(basename $dir)
  echo "------------------------------"
  echo -n "test group $test: "

  cd $dir

  ./run.sh > tests.log 2>&1
  result=$?
  echo $result 

  cd .. 
done
