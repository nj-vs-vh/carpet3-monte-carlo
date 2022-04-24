#include "G4ParticleTable.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#define ND   3
#define NMAX 10000000

class G4ParticleGun;

struct Header
{
    int EventNumber;
    int PrimaryID;
    float E0;
    float Theta0;
    float Phi0;
    int NParticle;
};

struct Particle
{ //response model shower
    int ParticleID;
    float m[3];
    float x;
    float y;
    float t;
};

class C2Primary:public G4VUserPrimaryGeneratorAction
{
    public:
    C2Primary();
    ~C2Primary();
    void GeneratePrimaries(G4Event* anEvent);
    int ReadShower();
    int FilterParticles(double xa,double ya);
    G4ParticleTable* particleTable;
    FILE *fpinp;
//
    int PrimaryID;
    double E0,Theta0;
    private:
    int NParticle;				//total number of particles in the shower (observation level)
//
    int ID[NMAX];
    double x[NMAX],y[NMAX];
    double px[NMAX],py[NMAX],pz[NMAX];
    double E[NMAX],t[NMAX];
//
    int IDf[NMAX];
    double xf[NMAX],yf[NMAX];
    double pxf[NMAX],pyf[NMAX],pzf[NMAX];
    double Ef[NMAX],tf[NMAX];
//
    struct Header* hb;
    struct Particle* pb;
//
    G4ParticleGun* particleGun;
    int ReadParticle(int n);
};
