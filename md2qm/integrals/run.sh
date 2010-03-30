#!/bin/bash

source ../functions.sh

datadir="../data"

test_start "copying QM_files"
cp -R ${datadir}/QM_files . || test_fail
test_end

test_start "running ctp_integrals"
../../../md2qm/src/tools/ctp_integrals --crg ${datadir}/list_charges.xml --opt ${datadir}/main.xml || test_fail
test_end

test_start "checking nblist and integrals"
diff nbl_0.res nbl_int_only.ref || test_fail
test_end

tests_finished


