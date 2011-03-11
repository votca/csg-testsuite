#!/bin/bash -x

source ../functions.sh

trap 'true' 15

test_start "running ibm"
# running grompp to generate tpr
dir=$PWD
cd ibm || test_fail
rm -Rf done step_* inverse.log
nohup csg_inverse settings.xml &
wait $! || test_fail
cd $dir
test_end

test_start "ibm output"
diff ibm/step_002/CG-CG.pot.new ibm/ref/CG-CG.pot.new || test_fail
test_end

test_start "running imc"
# running grompp to generate tpr
dir=$PWD
cd imc || test_fail
rm -Rf done step_* inverse.log
nohup csg_inverse settings.xml &
wait $! || test_fail
cd $dir
test_end

test_start "imc output"
diff imc/step_001/CG-CG.pot.new imc/ref/CG-CG.pot.new || test_fail
test_end

tests_finished
