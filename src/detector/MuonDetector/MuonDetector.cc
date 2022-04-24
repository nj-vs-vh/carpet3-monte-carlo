#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "C2Detector.hh"
#include "C2Primary.hh"
#include "C2Event.hh"
#include "C2Step.hh"
#include "string.h"
#include "QGSP_BERT.hh"
#include "G4OpticalPhysics.hh"

#define LEN	200

    int NParticleF,nevent;
    double xa,ya;
    char InputName[LEN];			//input file name
    char OutputName[LEN];			//output file name
    C2Primary* MyPrimary;
    C2Step* MyStep;

    int SetInputName(int n);
    int SetOutputName(int n);

int main(int argc, char **argv)
{
    G4RunManager* RunMng= new G4RunManager;
    RunMng->SetUserInitialization(new C2Detector);
    G4VModularPhysicsList* physics= new QGSP_BERT();
    G4OpticalPhysics* optical= new G4OpticalPhysics();
    physics->RegisterPhysics(optical);
    RunMng->SetUserInitialization(physics);
    RunMng->Initialize();
//
    MyPrimary= new C2Primary;
    RunMng->SetUserAction(MyPrimary);
    RunMng->SetUserAction(new C2Event);
    MyStep= new C2Step;
    RunMng->SetUserAction(MyStep);
//
    nevent= atoi(argv[argc-3]);
    xa= 1.0e2*atoi(argv[argc-2]);		//[m]->[cm]
    ya= 1.0e2*atoi(argv[argc-1]);		//[m]->[cm]
    printf("nevent= %5d xa= %13.6e ya= %13.6e\n",nevent,xa,ya);
    SetInputName(nevent);			//input name
    MyPrimary->fpinp= fopen(InputName,"r");
    if (MyPrimary->fpinp==NULL)
    {
	printf("Input file Shower not found!\n");
	return(1);
    }
//axis coordinates [cm]
    MyPrimary->ReadShower();
    NParticleF= MyPrimary->FilterParticles(xa,ya);
    printf("NParticleF= %8d\n",NParticleF);
//
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/event/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0");
//
    SetOutputName(nevent);
    MyStep->fp= fopen(OutputName,"w");
    fprintf(MyStep->fp,"%1d %13.6e %13.6e\n",MyPrimary->PrimaryID,MyPrimary->E0,MyPrimary->Theta0);
    RunMng->BeamOn(NParticleF);
    fclose(MyStep->fp);
    fclose(MyPrimary->fpinp);
    delete RunMng;
    return(0);
}

int SetInputName(int n)
{
    char s0[128]= "/home/timur1606/CygnusCocoon/Data/Binary/Shower-";
    char s1[128]= "";				//(!!)
    char s00[128]= "000";
    char s01[128]= "00";
    char s02[128]= "0";
    char buff1[32];
//generate string name on nevent
    strcpy(InputName,s0); //prefix
    if (n<10)			 strcat(InputName,s00);
    else if ((n>=10)&&(n<100))	 strcat(InputName,s01);
    else if ((n>=100)&&(n<1000)) strcat(InputName,s02);
    sprintf(buff1,"%d",n);			//first numerical suffix
    strcat(s1,buff1);				//convert numerical suffix to string
    strcat(InputName,s1);
    printf("InputName= %s\n",InputName);
    return(0);
}

int SetOutputName(int n)
{
    char s0[128]= "Output/Result-";
    char s1[128]= "";				//(!!)
    char s00[128]= "000";
    char s01[128]= "00";
    char s02[128]= "0";
    char buff1[32];
//generate string name on nevent
    strcpy(OutputName,s0); //prefix
    if (n<10)			 strcat(OutputName,s00);
    else if ((n>=10)&&(n<100))	 strcat(OutputName,s01);
    else if ((n>=100)&&(n<1000)) strcat(OutputName,s02);
    sprintf(buff1,"%d",n);			//first numerical suffix
    strcat(s1,buff1);				//convert numerical suffix to string
    strcat(OutputName,s1);
    printf("OutputName= %s\n",OutputName);
    return(0);
}
