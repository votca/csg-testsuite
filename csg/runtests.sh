#!/bin/bash

tests=$(find . -maxdepth 2 -name run.sh -exec dirname {} \; )

rm failed_notify

for dir in $tests; do
  test=$(basename $dir)
  echo "------------------------------"
  echo -n "test group $test: "
  workdir=$PWD
  cd $dir

  ./run.sh > tests.log 2>&1
  result=$?
  cd $workdir
 
  if [ $result -ne 0 ]; then
    echo failed
    if [ -f $dir/NOTIFY ]; then
      cat $dir/NOTIFY >> failed_notify
    fi
  else
    echo passed
  fi
  echo "" 
  cd $workdir 
  if [ -f $dir/results.txt ]; then
    cat $dir/results.txt
  fi
done
echo "------------------------------"

#check failed_notify and send emails
