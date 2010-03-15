#!/bin/bash

source ../functions.sh

test_start "testing csg_property"
tmp=$(csg_property --file test.xml --path "root" --print "node1") || test_fail
if [ ! "$tmp" = "node1 = hello" ]; then
  test_fail
fi

test_end

test_start "csg_resample: resampling"
csg_resample --in resample.in --out resampled --grid 0:0.1:3 || test_fail
diff resampled resampled.ref || test_fail
test_end

test_start "csg_resample: fitting"
csg_resample --in resampled --out fitted --grid 0:0.1:3 --spfit 0:1:3 || test_fail
diff fitted fitted.ref || test_fail
test_end

tests_finished
