#!/bin/bash

source ../functions.sh

test_start "running ibm"
# running grompp to generate tpr
dir=$PWD
cd ibm || test_fail
rm -Rf done step_*
bash -e "csg_inverse settings.xml" || test_fail
echo $tmp
cd $dir
test_end

test_start "ibm output"
diff ibm/CG-CG.pot.new ibm/ref/CG-CG.pot.new || test_fail
test_end

test_start "running imc"
# running grompp to generate tpr
dir=$PWD
cd imc || test_fail
rm -Rf done step_*
bash -e "csg_inverse settings.xml" || test_fail
cd $dir
test_end

test_start "imc output"
diff imc/CG-CG.pot.new imc/ref/CG-CG.pot.new || test_fail
test_end

tests_finished
