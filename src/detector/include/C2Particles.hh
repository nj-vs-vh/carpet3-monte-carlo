#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class C2Particles : public G4VPhysicsConstructor
{
public:
    C2Particles(const G4String &name = "particles");
    virtual ~C2Particles();

public:
    virtual void ConstructParticle();
    virtual void ConstructProcess(){};
};
