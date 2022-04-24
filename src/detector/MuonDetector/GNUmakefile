name := MuonDetector
G4TARGET := $(name)
G4EXLIB := true

G4SYSTEM = Linux-g++

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
