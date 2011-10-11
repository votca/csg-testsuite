#!/bin/bash

source ../functions.sh

test_start "testing csg_property"
tmp=$(csg_property --file test.xml --path "root" --print "node1") || test_fail
if [ ! "$tmp" = "node1 = hello" ]; then
  test_fail
fi

test_end

tests_finished
