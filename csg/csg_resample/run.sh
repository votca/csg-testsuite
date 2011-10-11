#!/bin/bash

source ../functions.sh

types="cubic akima linear"
bcs="natural periodic derivativezero"
#TODO: akima splines with derivativezero as BC not implemented yet...
for ttype in $types
do
  for bc in $bcs
  do
    [ $ttype = 'akima'  -a  $bc = 'derivativezero' ] || [ $ttype = 'cubic'  -a  $bc = 'derivativezero' ]  && continue
    test_start "csg_resample: resampling ($ttype, $bc)"
    csg_resample --type $ttype --boundaries $bc --in resample.in --out resampled_${ttype}_${bc} --grid 0:0.1:3 || test_fail
    sed -i '/^#/d' resampled_${ttype}_${bc} resampled_${ttype}_${bc}.ref
    diff resampled_${ttype}_${bc} resampled_${ttype}_${bc}.ref || test_fail
    test_end
  done

  #  test_start "csg_resample: fitting"
  #  csg_resample --in resampled --type cubic --out fitted --grid 0:0.1:3 --fitgrid 0:1:3 || test_fail
  #  sed -i '/^#/d' fitted fitted.ref
  #  diff fitted fitted.ref || test_fail
  #  test_end
done

tests_finished
