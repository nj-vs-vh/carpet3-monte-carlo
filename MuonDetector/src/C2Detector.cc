#include "C2Detector.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4TransportationManager.hh"

C2Detector::C2Detector()
{}

C2Detector::~C2Detector()
{
    DestroyMaterials();
//    delete WorldVisAtt;
//    delete DetectorVisAtt;
}

G4VPhysicalVolume* C2Detector::Construct()
{
    int i,j,k;
    ConstructMaterials();
//World volume
    WorldSolid= new G4Box("WorldSolid",1.1e2*m,1.1e2*m,1.1e2*m); //220X220X220 m
    WorldLogical= new G4LogicalVolume(WorldSolid,Vacuum,"WorldLogical",0,0,0);
    WorldPhysical= new G4PVPlacement(0,G4ThreeVector(),WorldLogical,"WorldPhysical",0,0,0);
//Rock
    RockSolid= new G4Box("RockSolid",1.0e2*m,1.0e2*m,50.0*m); //200X200X100 m
    RockLogical= new G4LogicalVolume(RockSolid,Basalt,"RockLogical",0,0,0);
    RockPhysical= new G4PVPlacement(0,G4ThreeVector(0.0*m,0.0*m,-50.0*m),
	    RockLogical,"RockPhysical",WorldLogical,false,0);
//Horizontal concrete plate
    ConcreteSolid= new G4Box("ConcreteSolid",50.0*m,50.0*m,0.15*m); //100X100X0.3 m
    ConcreteLogical= new G4LogicalVolume(ConcreteSolid,Concrete,"ConcreteLogical",0,0,0);
    ConcretePhysical= new G4PVPlacement(0,G4ThreeVector(0.0*m,0.0*m,(50.0-1.85)*m),
	    ConcreteLogical,"ConcretePhysical",RockLogical,false,0);
//Tunnel
    TunnelSolid= new G4Box("TunnelSolid",2.75*m,21.0*m,1.3*m); //42X5.5X2.6 m
    TunnelLogical= new G4LogicalVolume(TunnelSolid,Vacuum,"TunnelLogical",0,0,0);
    TunnelPhysical= new G4PVPlacement(0,G4ThreeVector(0.0*m,0.0*m,(50.0-2.0-1.3)*m),
	    TunnelLogical,"TunnelPhysical",RockLogical,false,0);
//Scintillator
    k= 0;
    for (i=0; i<5; i++)
    {
	for (j=0; j<35; j++)
	{
	    PlasticSolid= new G4Box("PlasticSolid",0.5*m,0.5*m,0.025*m); //1.0X1.0X0.05 m
	    PlasticLogical= new G4LogicalVolume(PlasticSolid,Plastic,"PlasticLogical",0,0,0);
	    PlasticPhysical= new G4PVPlacement(0,G4ThreeVector((i-2.0)*1.01*m,(j-17.0)*1.01*m,(1.2-0.025)*m),
		    PlasticLogical,"PlasticPhysical",TunnelLogical,false,k);
	    k++;
	}
    }
//

//
//Visualisation attributes
//World volume
    /*WorldVisAtt = new G4VisAttributes(G4Colour(0.0,0.0,0.0)); //Black
    WorldVisAtt->SetVisibility(false);
    WorldLogical->SetVisAttributes(WorldVisAtt);
//Detector
    RockVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //White
    RockVisAtt->SetVisibility(true);
    RockLogical->SetVisAttributes(DetectorVisAtt);*/
//
    G4cout << G4endl << "The geometrical tree defined is: " << G4endl << G4endl;
    DumpGeometricalTree(WorldPhysical);
    return WorldPhysical;
}

void C2Detector::ConstructMaterials()
{
    G4int NComponents;
    G4double Z;
    G4double A;
    G4double Density;
    G4double FractionMass;
    G4String Name;
    G4String Symbol;
//Vacuum
    Name   = "Vacuum";
    Z      = 1.0;
    A      = 1.0*g/mole;
    Density= 1.0e-10*g/cm3;
    Vacuum = new G4Material(Name,Z,A,Density);
//1.
    Z= 1.0;
    A= 1.00795*g/mole;
    G4Element* elH = new G4Element(Name="Hydrogen",Symbol="H",Z,A);
//2.
    Z= 6.0;
    A= 12.0108*g/mole;
    G4Element* elC = new G4Element(Name="Carbon",Symbol="C",Z,A);
//3.
    Z= 8.0;
    A= 15.9994*g/mole;
    G4Element* elO = new G4Element(Name="Oxygen",Symbol="O",Z,A);
//4.
    Z= 11.0;
    A= 22.9898*g/mole;
    G4Element* elNa= new G4Element(Name="Natrium",Symbol="Na",Z,A);
//5.
    Z= 12.0;
    A= 24.305*g/mole;
    G4Element* elMg= new G4Element(Name="Magnium",Symbol="Mg",Z,A);
//6.
    Z= 13.0;
    A= 26.98154*g/mole;
    G4Element* elAl= new G4Element(Name="Aluminium",Symbol="Al",Z,A);
//7.
    Z= 14.0;
    A= 28.086*g/mole;
    G4Element* elSi= new G4Element(Name="Silicium",Symbol="Si",Z,A);
//8.
    Z= 20.0;
    A= 40.08*g/mole;
    G4Element* elCa= new G4Element(Name="Calcium",Symbol="Ca",Z,A);
//9.
    Z= 26.0;
    A= 55.847*g/mole;
    G4Element* elFe= new G4Element(Name="Ferrum",Symbol="Fe",Z,A);
//
    NComponents= 7;
    Density= 2.5*g/cm3;
    Basalt= new G4Material(Name="Basalt",Density,NComponents);
    Basalt->AddElement(elO ,FractionMass= 0.600); //1.
    Basalt->AddElement(elNa,FractionMass= 0.020); //2.
    Basalt->AddElement(elMg,FractionMass= 0.025); //3.
    Basalt->AddElement(elAl,FractionMass= 0.060); //4.
    Basalt->AddElement(elSi,FractionMass= 0.175); //5.
    Basalt->AddElement(elCa,FractionMass= 0.050); //6.
    Basalt->AddElement(elFe,FractionMass= 0.070); //7.
//Concrete
    NComponents= 6;
    Density= 2.5*g/cm3;
    Concrete= new G4Material(Name="Concrete",Density,NComponents);
    Concrete->AddElement(elO ,FractionMass= 0.540); //1.
    Concrete->AddElement(elMg,FractionMass= 0.025); //2.
    Concrete->AddElement(elAl,FractionMass= 0.020); //3.
    Concrete->AddElement(elSi,FractionMass= 0.065); //4.
    Concrete->AddElement(elCa,FractionMass= 0.325); //5.
    Concrete->AddElement(elFe,FractionMass= 0.025); //6.
//
    NComponents= 2;
    Density= 1.0*g/cm3;
    Plastic= new G4Material(Name="Plastic",Density,NComponents);
    Plastic->AddElement(elH,FractionMass= 0.500); //1.
    Plastic->AddElement(elC,FractionMass= 0.500); //2.
//
    const G4int nE= 3;
    G4double Energy[nE]= {7.00*eV,7.07*eV,7.14*eV};
    G4double Eff[nE]= {0.1,0.1,0.1};
    G4double Rindex[nE]= {1.58,1.58,1.58};
    G4double AbsLength[nE]= {200.0*cm,200.0*cm,200.0*cm};
//
    mt = new G4MaterialPropertiesTable(); //Material Properties: create the object
    Plastic->SetMaterialPropertiesTable(mt);
    mt->AddProperty("FASTCOMPONENT",Energy,Eff,nE);
    mt->AddProperty("SLOWCOMPONENT",Energy,Eff,nE);
    mt->AddProperty("RINDEX",Energy,Rindex,nE);
    mt->AddProperty("ABSLENGTH",Energy,AbsLength,nE);
    mt->AddConstProperty("SCINTILLATIONYIELD",100.0/MeV);
    mt->AddConstProperty("RESOLUTIONSCALE",1.0);
    mt->AddConstProperty("FASTTIMECONSTANT",2.0*ns);
    mt->AddConstProperty("SLOWTIMECONSTANT",5.0*ns);
    mt->AddConstProperty("YIELDRATIO",1.0);
//    mt->AddConstProperty("RINDEX",1.58);
//    mt->AddConstProperty("ABSLENGHT",2.0*m);
    mt->DumpTable();
//Dump materials
    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void C2Detector::DestroyMaterials()
{
    size_t i;
    G4MaterialTable* matTable = (G4MaterialTable*)G4Material::GetMaterialTable();
    for(i=0;i<matTable->size();i++) delete (*(matTable))[i];
    matTable->clear();
    G4ElementTable* elemTable = (G4ElementTable*)G4Element::GetElementTable();
    for (i=0; i<elemTable->size(); i++) delete (*(elemTable))[i];
    elemTable->clear();
}

void C2Detector::DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth)
{
    for (int isp=0; isp<depth; isp++) G4cout << "  ";
    G4cout << aVolume->GetName() << "[" << aVolume->GetCopyNo() << "] "
	    << aVolume->GetLogicalVolume()->GetName() << " "
	    << aVolume->GetLogicalVolume()->GetNoDaughters() << " "
	    << aVolume->GetLogicalVolume()->GetMaterial()->GetName();
    G4cout << G4endl;
    for (int i=0; i<aVolume->GetLogicalVolume()->GetNoDaughters(); i++)
	DumpGeometricalTree(aVolume->GetLogicalVolume()->GetDaughter(i),depth+1);
}
