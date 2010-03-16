#! /bin/bash

source ../functions.sh

datadir="../data"
test_start "linking the QM files"
for i in $datadir/*
do
    ln -s ${i}
done

test_start "generating files for proj"
../../../../md2qm/src/tools/proj --top topol.tpr --cg "PCBM.xml;P3HT.xml" --trj tr10.trr --begin 0 --first-frame 0 --nframes 1  --listcharges list_charges.xml --nnnames "A:B;B:A" --cutoff 0.6
md5sum frame0/* > results.txt

test_start "checking files for projection calculation"
diff results.txt results.txt.ref || test_fail
test_end

tests_finished
