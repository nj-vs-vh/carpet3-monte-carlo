source scripts/_common.sh

echo "
Configuration read from .env file"
grep -v '^#' .env
echo

echo "
Building Muon Detector GEANT4 model"
cd src/detector/MuonDetector
make
cd ../../..
