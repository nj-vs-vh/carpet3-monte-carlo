#include "C2Step.hh"
#include "C2MuonDetector.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "math.h"
#include "G4SteppingManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

void C2Step::UserSteppingAction(const G4Step *aStep)
{
    G4Track *aTrack = aStep->GetTrack();
    int trackId = aTrack->GetTrackID();
    const G4DynamicParticle *dynamicParticle = aTrack->GetDynamicParticle();
    if (dynamicParticle == NULL)
        return;

    const G4ParticleDefinition *particleDefinition = dynamicParticle->GetDefinition();
    if (particleDefinition == NULL)
        return;

    const G4String particleName = particleDefinition->GetParticleName();
    int particleID; // accoding to CORSIKA
    if (particleName == "gamma")
        particleID = 1;
    else if (particleName == "e+")
        particleID = 2;
    else if (particleName == "e-")
        particleID = 3;
    else if (particleName == "mu+")
        particleID = 5;
    else if (particleName == "mu-")
        particleID = 6;
    else if (particleName == "pi+")
        particleID = 8;
    else if (particleName == "pi-")
        particleID = 9;
    else if (particleName == "kaon0L")
        particleID = 10;
    else if (particleName == "kaon+")
        particleID = 11;
    else if (particleName == "kaon-")
        particleID = 12;
    else if (particleName == "neutron")
        particleID = 13;
    else if (particleName == "proton")
        particleID = 14;
    else if (particleName == "anti_proton")
        particleID = 15;
    else if (particleName == "kaon0S")
        particleID = 16;
    else if (particleName == "anti_neutron")
        particleID = 25;
    else if (particleName == "nu_e")
        particleID = 66;
    else if (particleName == "nu_mu")
        particleID = 68;
    else if (particleName == "nu_tau")
        particleID = 133;
    else if (particleName == "anti_nu_e")
        particleID = 67;
    else if (particleName == "anti_nu_mu")
        particleID = 69;
    else if (particleName == "anti_nu_tau")
        particleID = 134;
    else if (particleName == "opticalphoton")
        particleID = 9900; // corsika uses this for Cherenkov photons, the closest we can get
    else
    {
        particleID = 0;
        G4cout << "Unknown particle: " << particleName << G4endl;
    }

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *preStepVolume = preStepPoint->GetPhysicalVolume();
    G4int volumeNumber = preStepVolume->GetCopyNo();
    if (preStepVolume == NULL)
        return;

    G4String preStepVolumeName = preStepVolume->GetName();
    if (preStepVolumeName != "PlasticPhysical")
        return;

    double eKinBefore = preStepPoint->GetKineticEnergy();
    double eKinAfter = postStepPoint->GetKineticEnergy();

    double x1[3], x2[3], m[3];
    for (int i = 0; i < 3; i++)

        x1[i] = preStepPoint->GetPosition()[i];
    for (int i = 0; i < 3; i++)
        x2[i] = postStepPoint->GetPosition()[i];
    for (int i = 0; i < 3; i++)
        m[i] = preStepPoint->GetMomentumDirection()[i];
    double t = postStepPoint->GetGlobalTime();

    // TODO: create a binary format for this data
    char outputLine[1024];
    sprintf(
        outputLine,
        "%10d %2d %3d  %13.6e %13.6e %13.6e  %13.6e %13.6e %13.6e  %13.6e %13.6e %13.6e  %13.6e %13.6e %13.6e\n",
        trackId, particleID, volumeNumber, x1[0], x1[1], x1[2], x2[0], x2[1], x2[2], m[0], m[1], m[2], eKinBefore, eKinAfter, t);
    (*outputStream) << outputLine;
}
