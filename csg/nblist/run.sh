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

test_start "nblist grid 1x1x1"
csg_stat --top topol.tpr --trj conf.gro --cg water_cg.xml --options settings_1x1x1.xml || test_fail
diff CG-CG.dist.new CG-CG_1x1x1.ref || test_fail
test_end

test_start "nblist grid 2x2x2"
csg_stat --top topol.tpr --trj conf.gro --cg water_cg.xml --options settings_2x2x2.xml || test_fail
diff CG-CG.dist.new CG-CG_2x2x2.ref || test_fail
test_end

test_start "nblist grid 3x3x3"
csg_stat --top topol.tpr --trj conf.gro --cg water_cg.xml --options settings_3x3x3.xml || test_fail
diff CG-CG.dist.new CG-CG_3x3x3.ref || test_fail
test_end

test_start "nblist grid 4x4x4"
csg_stat --top topol.tpr --trj conf.gro --cg water_cg.xml --options settings_4x4x4.xml || test_fail
diff CG-CG.dist.new CG-CG_4x4x4.ref || test_fail
test_end

tests_finished

