#!/bin/bash

tests_failed=0
rm results.txt


test_start () {
    testname=$1
    failed_save=$test_failed
}

test_end () {
    if [ "$test_failed" == "$failed_save" ]; then
        echo "$testname: ok" >> results.txt
    else
        echo "$testname: failed" >> results.txt
    fi
}

test_fail () {
    test_failed=$((test_failed+1))
}

tests_finished() {
    exit $test_failed
}
