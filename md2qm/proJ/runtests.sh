#!/bin/bash

tests=$(find . -maxdepth 2 -name run.sh -exec dirname {} \; )

if [ -f NOTIFY ]; then
  cp NOTIFY failed_notify
else
  if [ -f failed_notify ]; then
    rm -f failed_notify 
  fi
fi

some_failed=0

if [ -f summary.txt ]; then
  rm -f summary.txt
fi

for dir in $tests; do
  test=$(basename $dir)
  echo "------------------------------" | tee -a summary.txt
  echo -n "test group $test: " | tee -a summary.txt
  workdir=$PWD
  cd $dir

  ./run.sh > tests.log 2>&1
  result=$?
  cd $workdir
 
  if [ $result -ne 0 ]; then
    echo failed | tee -a summary.txt
    some_failed=1
    if [ -f $dir/NOTIFY ]; then
      cat $dir/NOTIFY >> failed_notify
    fi
  else
    echo passed | tee -a summary.txt
  fi
  echo "" 
  cd $workdir 
  if [ -f $dir/results.txt ]; then
    cat $dir/results.txt | tee -a summary.txt
  fi
done
echo "------------------------------" | tee -a summary.txt

if [ $some_failed -ne 0 ]; then
  if [ ! -f failed_notify ]; then
    echo "some tests have failed, but noone to notify, exiting"
    exit 1
  fi
  echo "some tests have failed, TODO, send summary.txt to:"
  notify=$(sort -u failed_notify )
  echo $notify
fi
