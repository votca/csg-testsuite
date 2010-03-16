#!/bin/bash

source ../functions.sh

test_start "atomistic run"
  ./run_atomistic.sh || test_fail
test_end

test_start "boltzmann inversion"
  cat boltzmann_cmds | csg_boltzmann --top topol.tpr --trj traj.trr --cg propane.xml || test_fail
  echo ""
  diff bond.dist.ib ref/bond.dist.ib || test_fail
  diff angle.dist.ib ref/angle.dist.ib || test_fail
  diff bond.pot.ib ref/bond.pot.ib || test_fail
  diff angle.pot.ib ref/angle.pot.ib || test_fail
test_end

test_start "force matching"
  csg_fmatch --top topol.tpr --trj traj.trr --cg propane.xml --options settings.xml || test_fail
  diff bond.force ref/bond.force || test_fail
  diff angle.force ref/angle.force || test_fail
test_end

test_start "force integration"
  csg_call table integrate angle.force angle.pot.fm || test_fail
  csg_call table integrate bond.force bond.pot.fm || test_fail
  diff bond.pot.fm ref/bond.pot.fm || test_fail
  diff angle.pot.fm ref/angle.pot.fm || test_fail
test_end 

test_start "force matching - constrained"
  csg_fmatch --top topol.tpr --trj traj.trr --cg propane.xml --options settings_constrained.xml || test_fail
  diff bond.force ref/bond.force.constr || test_fail
  diff angle.force ref/angle.force.constr || test_fail
test_end

tests_finished

