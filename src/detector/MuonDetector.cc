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
    if (argc <= 2)
    {
        std::cout << "GEANT4 model for Carpet-3 muon detector" << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "\tMuonDetector [N] [inputFile] [outputFile]" << std::endl;
        std::cout << "\tMuonDetector [N] [outputFile]" << std::endl;
        std::cout << "\t\tN - number of particles to read" << std::endl;
        std::cout << "\t\tinputFile - path to particle batch file; omit to read particle data from stdin" << std::endl;
        std::cout << "\t\toutputFile - path to output file" << std::endl;
        exit(1);
    }
    int nInputParticles = atoi(argv[1]);
    std::istream *input;
    std::ofstream *output;
    if (argc > 3)  // 3 command line arguments
    {
        input = new std::ifstream(argv[2], std::ios_base::binary);
        output = new std::ofstream(argv[3]);
    }
    else
    {
        input = &std::cin;
        output = new std::ofstream(argv[2]);
    }
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
    C2Step *step = new C2Step(output);
    runManager->SetUserAction(step);

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");

    runManager->BeamOn(nInputParticles);
    delete runManager;
    return (0);
}
