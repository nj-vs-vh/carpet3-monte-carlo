#include "C2Event.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"

C2Event::C2Event():drawFlag("all"), printModulo(1000)
{}

C2Event::~C2Event()
{}

void C2Event::BeginOfEventAction(const G4Event* evt)
{}

void C2Event::EndOfEventAction(const G4Event* evt)
{}
