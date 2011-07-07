#!/bin/bash

source ../functions.sh

test_start "testing csg_property"
tmp=$(csg_property --file test.xml --path "root" --print "node1") || test_fail
if [ ! "$tmp" = "node1 = hello" ]; then
  test_fail
fi

test_end

test_start "csg_resample: resampling (akima)"
csg_resample --in resample.in --out resampled --grid 0:0.1:3 || test_fail
sed -i '/^#/d' resampled resampled.ref
diff resampled resampled.ref || test_fail
test_end

test_start "csg_resample: fitting"
csg_resample --in resampled --type cubic --out fitted --grid 0:0.1:3 --fitgrid 0:1:3 || test_fail
sed -i '/^#/d' fitted fitted.ref
diff fitted fitted.ref || test_fail
test_end

tests_finished
