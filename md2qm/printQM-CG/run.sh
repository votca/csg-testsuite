#!/bin/bash

source ../functions.sh

datadir="../data"

test_start "copying QM_files"
cp -R ${datadir}/QM_files . || test_fail
test_end

test_start "running print"
../../../md2qm/src/tools/printqmattop --crg ${datadir}/list_charges.xml --opt ${datadir}/main.xml || test_fail
test_end

test_start "checking pdb files"
diff QMtraj.ref QMtraj.res
diff QMtraj.ref QMtraj.res
test_end

tests_finished


