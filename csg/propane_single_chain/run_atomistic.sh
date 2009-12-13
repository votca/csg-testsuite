#! /bin/bash

grompp -v || exit 1

mdrun -append -cpi state.cpt &> log_mdrun  || exit 1

