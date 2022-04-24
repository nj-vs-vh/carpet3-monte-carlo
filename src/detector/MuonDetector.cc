#include <iostream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT.hh"
#include "G4OpticalPhysics.hh"

#include "C2Primary.hh"
#include "C2Event.hh"
#include "C2Step.hh"

#include "C2MuonDetector.hh"

int main(int argc, char **argv)
{
    // TODO: separate agruments parsing into a shared function
    if (argc <= 1)
    {
        std::cout << "GEANT4 model for Carpet-3 muon detector" << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "\tMuonDetector [N] [path]" << std::endl;
        std::cout << "\tMuonDetector [N]" << std::endl;
        std::cout << "\t\tN - number of particles to read" << std::endl;
        std::cout << "\t\tpath - path to particle batch file; omit to read particles from stdin" << std::endl;
        exit(1);
    }
    int nInputParticles = atoi(argv[1]);
    std::istream *input;
    if (argc > 2)
    {
        std::string filename = argv[2];
        input = new std::ifstream(filename, std::ios_base::binary);
    }
    else
        input = &std::cin;

    G4RunManager *runManager = new G4RunManager;

    // Run manager initialization with our detector and physics
    runManager->SetUserInitialization(new C2MuonDetector);
    G4VModularPhysicsList *physics = new QGSP_BERT();
    G4OpticalPhysics *optical = new G4OpticalPhysics();
    physics->RegisterPhysics(optical);
    runManager->SetUserInitialization(physics);
    runManager->Initialize();

    C2Primary *primary = new C2Primary(input);
    runManager->SetUserAction(primary);
    runManager->SetUserAction(new C2Event);
    C2Step *step = new C2Step;
    runManager->SetUserAction(step);

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");

    step->fp = fopen("example-output", "w");
    runManager->BeamOn(nInputParticles);
    fclose(step->fp);
    delete runManager;
    return (0);
}
