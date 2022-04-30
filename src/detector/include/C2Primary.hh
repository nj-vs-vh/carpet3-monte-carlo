#include <istream>

#include "G4ParticleTable.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#define ND 3
#define NMAX 10000000

class G4ParticleGun;


struct Particle
{
    int id;
    float p[3];
    float x;
    float y;
    float t;
};

class C2Primary : public G4VUserPrimaryGeneratorAction
{
public:
    C2Primary(std::istream *input, bool verbose);
    ~C2Primary();
    void GeneratePrimaries(G4Event *anEvent);
    G4ParticleTable *particleTable;

private:
    std::istream *inputStream;
    G4ParticleGun *particleGun;
    struct Particle currentParticle;
    bool printInputParticles;
};
