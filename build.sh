#!/bin/bash

# build script for carpet 3 Monte Carlo pipeline

if [[ ! -f "./build.sh" ]] ; then
    echo "build.sh must be run from project root directory"
    exit 1;
fi

if [[ ! -f ".env" ]] ; then
    echo ".env file is missing, exiting"
    exit 1;
fi

echo "
Configuration read from .env file"
grep -v '^#' .env
export $(grep -v '^#' .env | xargs)
echo

# note that G4INSTALL is not geant4-vx.x.x-install directory from README!
# we're using an old Geant4Make build system which requires G4INSTALL to be a following
export G4INSTALL=$G4_CMAKE_PREFIX/share/Geant4-$G4_VERSION/geant4make
source $G4INSTALL/geant4make.sh

# forcing Geant4Make to install 
export G4WORKDIR=$(pwd)

echo "
Building Muon Detector GEANT4 model"
cd src/detector/MuonDetector
make
cd ../../..clear
