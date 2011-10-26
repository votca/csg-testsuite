#!/bin/bash

source ../functions.sh

test_start "running grompp"
# running grompp to generate tpr
grompp || test_fail
test_end

test_start "mapping tpr"
csg_map --top topol.tpr --trj conf.gro --cg water.xml --out "tpr.gro" || test_fail 
#diff tpr.gro tpr_ref.gro || test_fail
csg_call configuration compare tpr.gro tpr_ref.gro || test_fail
test_end 

test_start "map pdb file"
csg_map --top topol_pdb.xml --trj conf.gro --cg water_pdb.xml --out "pdb.gro" || test_fail 
#diff pdb.gro pdb_ref.gro || test_fail 
csg_call configuration compare pdb.gro pdb_ref.gro || test_fail
test_end 

test_start "map xml topology, 2 molecule types"
csg_map --top topol_rename.xml --trj conf.gro --cg "water1.xml;water2.xml" --out "xml.gro" || test_fail
#diff xml.gro xml_ref.gro || test_fail 
csg_call configuration compare xml.gro xml_ref.gro || test_fail
test_end 


tests_finished
