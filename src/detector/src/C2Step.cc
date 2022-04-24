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

C2Step::C2Step()
{
}

C2Step::~C2Step()
{
}

void C2Step::UserSteppingAction(const G4Step *aStep)
{
	int trackId, ParticleType;
	double Ek1, Ek2, t;
	double x1[3], x2[3], m[3];
	G4String preStepVolumeName;
	//
	G4Track *aTrack = aStep->GetTrack();
	trackId = aTrack->GetTrackID();
	const G4DynamicParticle *MyDynamicParticle = aTrack->GetDynamicParticle();
	G4StepPoint *MyPreStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *MyPostStepPoint = aStep->GetPostStepPoint();
	G4VPhysicalVolume *MyPreStepVolume = MyPreStepPoint->GetPhysicalVolume();
	G4int VolumeNumber = MyPreStepVolume->GetCopyNo();
	if (MyPreStepVolume != NULL)
		preStepVolumeName = MyPreStepVolume->GetName();
	Ek1 = MyPreStepPoint->GetKineticEnergy();
	Ek2 = MyPostStepPoint->GetKineticEnergy();
	for (int i = 0; i < 3; i++)
		x1[i] = MyPreStepPoint->GetPosition()[i];
	for (int i = 0; i < 3; i++)
		x2[i] = MyPostStepPoint->GetPosition()[i];
	for (int i = 0; i < 3; i++)
		m[i] = MyPreStepPoint->GetMomentumDirection()[i];
	t = MyPostStepPoint->GetGlobalTime();
	if (MyDynamicParticle != NULL)
	{
		const G4ParticleDefinition *MyParticleDefinition = MyDynamicParticle->GetDefinition();
		if (MyParticleDefinition != NULL)
		{
			const G4String MyParticleName = MyParticleDefinition->GetParticleName();
			if (MyParticleName == "gamma")
				ParticleType = 1;
			else if (MyParticleName == "e+")
				ParticleType = 2;
			else if (MyParticleName == "e-")
				ParticleType = 3;
			else if (MyParticleName == "mu+")
				ParticleType = 5;
			else if (MyParticleName == "mu-")
				ParticleType = 6;
			else if (MyParticleName == "pi+")
				ParticleType = 8;
			else if (MyParticleName == "pi-")
				ParticleType = 9;
			else if (MyParticleName == "kaon0L")
				ParticleType = 10;
			else if (MyParticleName == "kaon+")
				ParticleType = 11;
			else if (MyParticleName == "kaon-")
				ParticleType = 12;
			else if (MyParticleName == "neutron")
				ParticleType = 13;
			else if (MyParticleName == "proton")
				ParticleType = 14;
			else if (MyParticleName == "anti_proton")
				ParticleType = 15;
			else if (MyParticleName == "kaon0S")
				ParticleType = 16;
			else if (MyParticleName == "anti_neutron")
				ParticleType = 25;
			else if (MyParticleName == "nu_e")
				ParticleType = 26;
			else if (MyParticleName == "nu_mu")
				ParticleType = 27;
			else if (MyParticleName == "nu_tau")
				ParticleType = 28;
			else if (MyParticleName == "anti_nu_e")
				ParticleType = 29;
			else if (MyParticleName == "anti_nu_mu")
				ParticleType = 30;
			else if (MyParticleName == "anti_nu_tau")
				ParticleType = 31;
			else if (MyParticleName == "opticalphoton")
				ParticleType = 32;
			else
			{
				ParticleType = 33;
				if ((x1[2] < -2.01e3) && (x1[2] > -2.151e3) && (fabs(x1[0]) < 2.75e3) && (fabs(x1[1]) < 2.1e4))
				{
					printf("Unknown particle\n");
					G4cout << MyParticleName << G4endl;
				}
			}
			//
			//	    if ((x1[2]<-2.01e3)&&(x1[2]>-2.151e3)&&(fabs(x1[0])<2.75e3)&&(fabs(x1[1])<2.1e4))
			if (preStepVolumeName == "PlasticPhysical")
			{
				fprintf(fp, "%10d %2d %3d  %13.6e %13.6e %13.6e  %13.6e %13.6e %13.6e  %13.6e %13.6e %13.6e  %13.6e %13.6e %13.6e\n",
						trackId, ParticleType, VolumeNumber, x1[0], x1[1], x1[2], x2[0], x2[1], x2[2], m[0], m[1], m[2], Ek1, Ek2, t);
			}
		}
	}
}
