#!/bin/bash

failed=0

# running grompp to generate tpr
grompp -n index.ndx || exit 1 
# generating normal nblist
csg_stat --top topol.tpr --trj conf.gro --cg water_cg.xml --options settings.xml || failed=$((failed+1)) 

diff CG-CG.dist.new CG-CG.ref || failed=$((failed+1))

exit $failed
