# Geant4 detector model

This uses an old Geant4Make toolchain and can be compiled with a simple `make`.

Additional detectors must be placed in their respective `AnotherDetector.cc`
files in this directory. They will most likely use the same primary, step and
other building blocks with their respective detector geometry definition.
