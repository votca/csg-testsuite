#!/bin/bash

source ../functions.sh

datadir="../data"

test_start "copying QM_files"
cp -R ${datadir}/QM_files . || test_fail
test_end

test_start "running ctp_map"
ctp_map --top ${datadir}/topol.tpr --cg ${datadir}/norma_ch3.xml --trj ${datadir}/traj.xtc -l ${datadir}/list_charges.xml  --nframes 3 || test_fail
test_end

test_start "checking nblist"
diff nbl_votca_0.res nbl_map.ref || test_fail
test_end

tests_finished


