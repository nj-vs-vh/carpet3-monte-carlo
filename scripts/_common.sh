if [[ ! -f "./scripts/build.sh" ]] ; then
    echo "Scripts must be executed from project root directory, exiting"
    return
fi

if [[ ! -f ".env" ]] ; then
    echo ".env file is missing, exiting"
    return
fi

grep -v '^#' .env
export $(grep -v '^#' .env | xargs)


source $G4_CMAKE_PREFIX/bin/geant4.sh

# note that G4INSTALL is not geant4-vx.x.x-install directory from README!
# we're using an old Geant4Make build system which requires G4INSTALL to be a following
export G4INSTALL=$G4_CMAKE_PREFIX/share/Geant4-$G4_VERSION/geant4make
source $G4INSTALL/geant4make.sh

# forcing Geant4Make to work in this directory and place executables in ./bin/Linux-g++
export G4WORKDIR=$(pwd)
export G4SYSTEM="Linux-g++"
