#!/bin/bash

source ../functions.sh

test_start "running grompp"
# running grompp to generate tpr
grompp -n index.ndx || test_fail
test_end

test_start "nblist simple"
csg_stat --top topol.tpr --trj conf.gro --cg water_cg.xml --options settings.xml || test_fail 
diff CG-CG.dist.new CG-CG.ref || test_fail
test_end 

tests_finished
