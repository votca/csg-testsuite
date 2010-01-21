#!/bin/bash

source ../functions.sh

datadir="../data"

test_start "copying QM_files"
cp -R ${datadir}/QM_files . || test_fail
test_end

test_start "running easyj"
../../../md2qm/src/tools/easyj --top ${datadir}/topol.tpr --cg ${datadir}/norma_ch3.xml --trj ${datadir}/traj.xtc -l ${datadir}/list_charges.xml -o ${datadir}/main.xml --nframes 1 || test_fail
test_end

test_start "checking kmc_cont results"
diff kmc_cont.res kmc_cont.ref || test_fail
test_end

tests_finished

