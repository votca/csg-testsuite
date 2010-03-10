#!/bin/bash

source ../functions.sh

datadir="../data"

test_start "copying QM_files"
cp -R ${datadir}/QM_files . || test_fail
test_end

test_start "running easyj"
../../../md2qm/src/tools/easyj --top ${datadir}/topol.tpr --cg ${datadir}/norma_ch3.xml --trj ${datadir}/traj.xtc -l ${datadir}/list_charges.xml -o ${datadir}/main.xml --nframes 1 || test_fail
test_end

test_start "checking nblist, integrals, rates"
diff nbl_votca.res nbl_votca.ref || test_fail
test_end

test_start "running the printing of the pdb file"
../../../md2qm/src/tools/printqmattop --top ${datadir}/topol.tpr --cg ${datadir}/norma_ch3.xml --trj ${datadir}/traj.xtc -l ${datadir}/list_charges.xml  --nframes 3 || test_fail
test_end

test_start "checking pdb files"
diff CGtraj.pdb.ref  CGtraj.pdb || test_fail
test_end


tests_finished


