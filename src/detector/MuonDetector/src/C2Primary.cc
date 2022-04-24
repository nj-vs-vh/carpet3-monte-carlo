#include "C2Primary.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#define PrintFlag 0

C2Primary::C2Primary()
{
    hb= (struct Header*) (malloc(sizeof(struct Header)));
    pb= (struct Particle*) (malloc(sizeof(struct Particle)));
    particleGun = new G4ParticleGun(1);
    particleTable = G4ParticleTable::GetParticleTable();
}

C2Primary::~C2Primary()
{
    delete particleGun;
}

void C2Primary::GeneratePrimaries(G4Event* anEvent)
{
    int n;
    double Mass,E2,Ek;
    double mx,my,mz,mr;
//
    n= anEvent->GetEventID();
    if (PrintFlag>0)
	printf("n= %8d\n",n);
    if (IDf[n]==1)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
	Mass= 0.0;
    }
    else if (IDf[n]==2)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("e+"));
	Mass= 5.109989461e-4; //[GeV]
    }
    else if (IDf[n]==3)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
	Mass= 5.109989461e-4; //[GeV]
    }
    else if (IDf[n]==5)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("mu+"));
	Mass= 0.1056583745; //[GeV]
    }
    else if (IDf[n]==6)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("mu-"));
	Mass= 0.1056583745; //[GeV]
    }
    else if (IDf[n]==8)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("pi+"));
	Mass= 0.13957039; //[GeV]
    }
    else if (IDf[n]==9)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("pi-"));
	Mass= 0.13957039; //[GeV]
    }
    else if (IDf[n]==10)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("kaon0L"));
	Mass= 0.497614; //[GeV]
    }
    else if (IDf[n]==11)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("kaon+"));
	Mass= 0.493677; //[GeV]
    }
    else if (IDf[n]==12)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("kaon-"));
	Mass= 0.493677; //[GeV]
    }
    else if (IDf[n]==13)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
	Mass= 0.9395654133;
    }
    else if (IDf[n]==14)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("proton"));
	Mass= 0.9382720813;
    }
    else if (IDf[n]==15)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("anti_proton"));
	Mass= 0.9382720813;
    }
    else if (IDf[n]==16)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("kaon0S"));
	Mass= 0.497614; //[GeV]
    }
    else if (IDf[n]==25)
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("anti_neutron"));
	Mass= 0.9395654133;
    }
    else
    {
	particleGun->SetParticleDefinition(particleTable->FindParticle("geantino"));
	Mass= 0.0;
    }
//
    particleGun->SetParticlePosition(G4ThreeVector(xf[n]*cm,yf[n]*cm,1.0*m));
    mr= sqrt(pxf[n]*pxf[n]+pyf[n]*pyf[n]+pzf[n]*pzf[n]);
    mx= pxf[n]/mr;
    my= pyf[n]/mr;
    mz= pzf[n]/mr;
    particleGun->SetParticleMomentumDirection(G4ThreeVector(mx,my,-mz));
    E2= mr*mr+Mass*Mass;
    Ek= sqrt(E2)-Mass;
    particleGun->SetParticleEnergy(Ek*GeV);
    particleGun->GeneratePrimaryVertex(anEvent);
}

int C2Primary::ReadShower()
{
    int i,f,ri;
    double rd;
//text mode
    /*f= fscanf(fpinp,"%d",&ri);  //
    f= fscanf(fpinp,"%d",&ri);  //
    f= fscanf(fpinp,"%lf",&rd); //
    f= fscanf(fpinp,"%lf",&rd); //
    f= fscanf(fpinp,"%lf",&rd); //
    f= fscanf(fpinp,"%d",&NParticle);*/
//binary mode
    fread(hb,sizeof(struct Header),1,fpinp);
    /*hb->EventNumber= EventNumber;*/
    PrimaryID= hb->PrimaryID;
    E0= hb->E0;
    Theta0= hb->Theta0;
    /*hb->Phi0= Phi0;
    hb->NParticle= NParticle;*/
//
    NParticle= hb->NParticle;
    if (PrintFlag>0)
    {
        printf("Header= %5d %5d %13.6e %13.6e %13.6e %8d\n",
	    hb->EventNumber,hb->PrimaryID,hb->E0,hb->Theta0,hb->Phi0,hb->NParticle);
    }
    for (i=0; i<NParticle; i++)
	ReadParticle(i);
    return(0);
}

int C2Primary::ReadParticle(int n)
{
    int ri,f;
    double rd;
//text mode
    /*f= fscanf(fpinp,"%d",&ri); ID[n] = ri;
    f= fscanf(fpinp,"%lf",&rd); px[n]= rd;
    f= fscanf(fpinp,"%lf",&rd); py[n]= rd;
    f= fscanf(fpinp,"%lf",&rd); pz[n]= rd;
    f= fscanf(fpinp,"%lf",&rd); x[n] = rd;
    f= fscanf(fpinp,"%lf",&rd); y[n] = rd;
//    f= fscanf(fpinp,"%lf",&rd); E[n] = rd;
    f= fscanf(fpinp,"%lf",&rd); t[n] = rd;*/
//binary mode
    fread(pb,sizeof(struct Particle),1,fpinp);
    ID[n]= pb->ParticleID;
    px[n]= pb->m[0];
    py[n]= pb->m[1];
    pz[n]= pb->m[2];
    x[n] = pb->x;
    y[n] = pb->y;
    t[n] = pb->t;
//
    if (PrintFlag>0)
    {
	printf("Particle ID= %3d\n",ID[n]);
	if (ID[n]==4) printf("Particle ID= 4\n");
	if (ID[n]==7) printf("pi0\n");
	if (ID[n]>14) printf("Unknown particle ID= %3d\n",ID[n]);
    }
//
    if (PrintFlag>0)
    {
	printf("%3d  %13.6e %13.6e %13.6e  %13.6e %13.6e %13.6e\n",
	    ID[n],px[n],py[n],pz[n],x[n],y[n],t[n]);
    }
    return(0);
}

int C2Primary::FilterParticles(double xa,double ya)
{
    int i,nf;
    double xs,ys;
    double dx,dy,dr;
//
    nf= 0.0;
    dr= 5.0e2; //delta-distance [cm]= 5 m
    dx= 2.75e2+dr;
    dy= 2.10e3+dr;
    for (i=0; i<NParticle; i++)
    { //filter particle
	xs= x[i]+xa;
	ys= y[i]+ya;
	if (PrintFlag>0)
	    printf("%13.6e %13.6e\n",xs,ys);
	if ((xs>=-dx)&&(xs<=dx)&&(ys>=-dy)&&(ys<=dy)) //inject this particle
	{ //energy threshold 500 MeV, in reality close to 1 GeV
	    IDf[nf]= ID[i];
	    xf[nf] = xs;
	    yf[nf] = ys;
	    pxf[nf]= px[i];
	    pyf[nf]= py[i];
	    pzf[nf]= pz[i];
	    Ef[nf] = E[i];
	    tf[nf] = t[i];
	    nf++;
	    if (PrintFlag>0)
	    {
		printf("%3d %13.6e %13.6e  %13.6e %13.6e %13.6e  %13.6e %13.6e\n",
		    ID[i],xs,ys,px[i],py[i],pz[i],E[i],t[i]);
	    }
	}
    }
    if (PrintFlag>0)
	printf("nf= %8d\n",nf);
    return(nf);
}
