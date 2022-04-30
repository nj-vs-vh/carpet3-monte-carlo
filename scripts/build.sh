source scripts/activate.sh --verbose

echo "
Building GEANT4 detector model"
cd src/detector
make
cd ../..
