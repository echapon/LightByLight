#!/bin/bash

macro=$1
arg=$2
dir=/afs/cern.ch/user/e/echapon/workspace/private/LightByLight/EmiliensCode/QEDnorm

pwd
uname -a

# root setup
source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.04.02/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

# run the macro
cp $dir/*C $dir/*h .
pwd
root -l -b -q ${macro}+'('${arg}')'
cp output*root $dir
