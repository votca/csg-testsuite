#!/bin/bash

source ../functions.sh

datadir="../data"

test_start "copying QM_files & state"
cp -R ${datadir}/QM_files . || test_fail
test_end

test_start "running ctp_ratecalculator"
ctp_ratecalc --crg ${datadir}/list_charges.xml --opt ${datadir}/main.xml || test_fail
test_end

test_start "checking rates"
diff nbl_0.res nbl_rates.ref || test_fail
test_end

tests_finished


