source scripts/_common.sh

echo "
Configuration read from .env file"
grep -v '^#' .env
echo

echo "
Building GEANT4 detector model"
cd src/detector
make
cd ../..
