#include <iostream>
#include <cmath>

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "C2Primary.hh"

#define PrintFlag 0

C2Primary::C2Primary(std::istream *input) : inputStream(input)
{
    currentParticle = Particle();
    particleGun = new G4ParticleGun(1); // TODO: check if batching is available with >1 particles simultaneously
    particleTable = G4ParticleTable::GetParticleTable();
}

C2Primary::~C2Primary()
{
    delete particleGun;
}

void C2Primary::GeneratePrimaries(G4Event *anEvent)
{
    int particleNumber = anEvent->GetEventID();

    (*inputStream).read((char *)&currentParticle, sizeof(Particle));

    if (PrintFlag > 0)
    {
        printf(
            "Particle %i: ID=%3d px=%+.0e py=%+.0e pz=%+.0e x=%+.0e y=%+.0e t=%+.0e\n",
            particleNumber,
            currentParticle.id,
            currentParticle.p[0], currentParticle.p[1], currentParticle.p[2],
            currentParticle.x, currentParticle.y,
            currentParticle.t);
    }

    double Mass;
    if (currentParticle.id == 1)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
        Mass = 0.0;
    }
    else if (currentParticle.id == 2)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("e+"));
        Mass = 5.109989461e-4; //[GeV]
    }
    else if (currentParticle.id == 3)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
        Mass = 5.109989461e-4; //[GeV]
    }
    else if (currentParticle.id == 5)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("mu+"));
        Mass = 0.1056583745; //[GeV]
    }
    else if (currentParticle.id == 6)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("mu-"));
        Mass = 0.1056583745; //[GeV]
    }
    else if (currentParticle.id == 8)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("pi+"));
        Mass = 0.13957039; //[GeV]
    }
    else if (currentParticle.id == 9)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("pi-"));
        Mass = 0.13957039; //[GeV]
    }
    else if (currentParticle.id == 10)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("kaon0L"));
        Mass = 0.497614; //[GeV]
    }
    else if (currentParticle.id == 11)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("kaon+"));
        Mass = 0.493677; //[GeV]
    }
    else if (currentParticle.id == 12)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("kaon-"));
        Mass = 0.493677; //[GeV]
    }
    else if (currentParticle.id == 13)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
        Mass = 0.9395654133;
    }
    else if (currentParticle.id == 14)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("proton"));
        Mass = 0.9382720813;
    }
    else if (currentParticle.id == 15)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("anti_proton"));
        Mass = 0.9382720813;
    }
    else if (currentParticle.id == 16)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("kaon0S"));
        Mass = 0.497614; //[GeV]
    }
    else if (currentParticle.id == 25)
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("anti_neutron"));
        Mass = 0.9395654133;
    }
    else
    {
        particleGun->SetParticleDefinition(particleTable->FindParticle("geantino"));
        Mass = 0.0;
    }
    
    // TODO: don't we need to set the starting point of the particle according to it's timing inside the shower?
    particleGun->SetParticlePosition(
        G4ThreeVector(currentParticle.x * cm, currentParticle.y * cm, 1.0 * m) // spawning the particle 1m above the (0, 0) plane
    );
    double p = sqrt(sqr(currentParticle.p[0]) + sqr(currentParticle.p[1]) + sqr(currentParticle.p[2]));
    double pxNorm = currentParticle.p[0] / p;
    double pyNorm = currentParticle.p[1] / p;
    double pzNorm = currentParticle.p[2] / p;
    particleGun->SetParticleMomentumDirection(G4ThreeVector(pxNorm, pyNorm, -pzNorm)); // conversion between CORSIKA and GEANT4 z-axes
    double eSquared = sqr(p) + sqr(Mass);
    double eKin = sqrt(eSquared) - Mass;
    particleGun->SetParticleEnergy(eKin * GeV);
    particleGun->GeneratePrimaryVertex(anEvent);
}
