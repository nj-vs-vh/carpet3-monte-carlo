# main environment activation script

if [[ ! -f "./scripts/build.sh" ]] ; then
    echo "All scripts must be executed from project root directory, aborting"
    exit 1
fi

if [[ ! -f ".env" ]] ; then
    echo ".env file is missing, aborting"
    exit 1
fi

# parsing command line arguments

VERBOSE="false"
for flag in "$@"  # iterating over command line arguments
do
    if [[ $flag == "-v" ]] || [[ $flag == "--verbose" ]]; then
        VERBOSE="true"
    fi
done

# exporting .env file contents
export $(grep -v '^#' .env | xargs)
if [[ $VERBOSE == "true" ]]; then
    echo "
    .env file contents:"
    grep -v '^#' .env
fi

source $G4_CMAKE_PREFIX/bin/geant4.sh

# note that G4INSTALL is not geant4-vx.x.x-install directory from README!
# we're using an old Geant4Make build system which requires G4INSTALL to be a following
export G4INSTALL=$G4_CMAKE_PREFIX/share/Geant4-$G4_VERSION/geant4make
if [[ $VERBOSE == "true" ]]; then
    echo "
    Geant4Make installation directory (don't be confused by the name)"
    echo G4INSTALL=$G4INSTALL
fi

source $G4INSTALL/geant4make.sh

# confiruging Geant4Makr workdir and final binaries dir
export G4WORKDIR=$(pwd)
export G4SYSTEM="Linux-g++"
export CARPET3_BIN_DIR=$G4WORKDIR/bin/$G4SYSTEM
if [[ $VERBOSE == "true" ]]; then
    echo "
    Geant4Make build params"
    echo G4WORKDIR=$G4WORKDIR
    echo G4SYSTEM=$G4WORKDIR
    echo "
    All the compiled binaries will be placed in"
    echo CARPET3_BIN_DIR=$CARPET3_BIN_DIR
fi
