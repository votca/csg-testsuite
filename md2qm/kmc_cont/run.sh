#!/bin/bash

source ../functions.sh

datadir="../data"

test_start "copying QM_files"
cp -R ${datadir}/QM_files . || test_fail
test_end

test_start "running easyj"
ctp_cont_kmc --crg ${datadir}/list_charges.xml --opt ${datadir}/main.xml --nframes 1 || test_fail
test_end

test_start "checking kmc_cont results"
diff kmc_cont.res kmc_cont.ref || test_fail
test_end

tests_finished


