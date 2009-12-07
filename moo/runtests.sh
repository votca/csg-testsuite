#!/bin/sh

summary="results.txt"
data="data_files"

rm -f $summary      

echo moo `date` > $summary     
# check the return value, 0 == ok, other failed!

# transfer integral for all orbitals up to LUMO at a fixed distance for a benzene dimer
out="orbitals.test"
test="$data/$out"
rm -f $out
./overlap_integral -o $data/benzene.orb -c $data/benzene.xyz -p $data/all.pairs -m $data/matrix.unitary -j $out 
diff $out $test
echo orbitals overlap vs energy level: $? >> $summary

# transfer integral in a face-to-face geomery as a function of separation
out="translate.test"
test="$data/$out"
rm -f $out

./overlap_integral -o $data/benzene.orb -c $data/benzene.xyz -p $data/homo_homo.pairs -m $data/matrix.translate -j $out
diff $out $test
echo orbitals overlap vs separation: $? >> $summary   

exit 0

