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

#include "C2MuonDetector.hh"

C2MuonDetector::C2MuonDetector()
{
}

C2MuonDetector::~C2MuonDetector()
{
    DestroyMaterials();
}

G4VPhysicalVolume *C2MuonDetector::Construct()
/* The main method for constructing the world volume  */
{
    ConstructMaterials();
    // World volume: 220 x 220 x 220 m
    // Coordinate system is positioned at it's center
    WorldSolid = new G4Box("WorldSolid", 1.1e2 * m, 1.1e2 * m, 1.1e2 * m);
    WorldLogical = new G4LogicalVolume(WorldSolid, Vacuum, "WorldLogical", nullptr, nullptr, nullptr);
    WorldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), WorldLogical, "WorldPhysical", 0, 0, false);
    // Rock: 200 x 200 x 100 m
    RockSolid = new G4Box("RockSolid", 1.0e2 * m, 1.0e2 * m, 50.0 * m);
    RockLogical = new G4LogicalVolume(RockSolid, Basalt, "RockLogical", nullptr, nullptr, nullptr);
    RockPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0.0 * m, 0.0 * m, -50.0 * m),
                                     RockLogical, "RockPhysical", WorldLogical, false, 0);
    // Horizontal concrete plate: 100 x 100 x 0.3 m
    ConcreteSolid = new G4Box("ConcreteSolid", 50.0 * m, 50.0 * m, 0.15 * m);
    ConcreteLogical = new G4LogicalVolume(ConcreteSolid, Concrete, "ConcreteLogical", nullptr, nullptr, nullptr);
    ConcretePhysical = new G4PVPlacement(nullptr, G4ThreeVector(0.0 * m, 0.0 * m, (50.0 - 1.85) * m),
                                         ConcreteLogical, "ConcretePhysical", RockLogical, false, 0);
    // Tunnel: 42 x 5.5 x 2.6 m
    TunnelSolid = new G4Box("TunnelSolid", 2.75 * m, 21.0 * m, 1.3 * m);
    TunnelLogical = new G4LogicalVolume(TunnelSolid, Vacuum, "TunnelLogical", nullptr, nullptr, nullptr);
    TunnelPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0.0 * m, 0.0 * m, (50.0 - 2.0 - 1.3) * m),
                                       TunnelLogical, "TunnelPhysical", RockLogical, false, 0);
    // Scintillator
    int scintillatorCopyNo = 0;
    for (int i = 0; i < 5; i++) // TODO: parametrize detector configuration
    {
        for (int j = 0; j < 35; j++)
        {
            PlasticSolid = new G4Box("PlasticSolid", 0.5 * m, 0.5 * m, 0.025 * m); // 1.0 x 1.0 x 0.05 m
            PlasticLogical = new G4LogicalVolume(PlasticSolid, Plastic, "PlasticLogical", nullptr, nullptr, nullptr);
            PlasticPhysical = new G4PVPlacement(
                nullptr,
                G4ThreeVector((i - 2.0) * 1.01 * m, (j - 17.0) * 1.01 * m, (1.2 - 0.025) * m),
                PlasticLogical,
                "PlasticPhysical",
                TunnelLogical,
                false,
                scintillatorCopyNo);
            scintillatorCopyNo++;
        }
    }

    G4cout << G4endl << "The geometrical tree defined is: " << G4endl << G4endl;
    DumpGeometricalTree(WorldPhysical);
    return WorldPhysical;
}

void C2MuonDetector::ConstructMaterials()
{
    G4int NComponents;
    G4double Z;
    G4double A;
    G4double Density;
    G4double FractionMass;
    G4String Name;
    G4String Symbol;
    // Vacuum
    Name = "Vacuum";
    Z = 1.0;
    A = 1.0 * g / mole;
    Density = 1.0e-10 * g / cm3;
    Vacuum = new G4Material(Name, Z, A, Density);
    // 1.
    Z = 1.0;
    A = 1.00795 * g / mole;
    G4Element *elH = new G4Element(Name = "Hydrogen", Symbol = "H", Z, A);
    // 2.
    Z = 6.0;
    A = 12.0108 * g / mole;
    G4Element *elC = new G4Element(Name = "Carbon", Symbol = "C", Z, A);
    // 3.
    Z = 8.0;
    A = 15.9994 * g / mole;
    G4Element *elO = new G4Element(Name = "Oxygen", Symbol = "O", Z, A);
    // 4.
    Z = 11.0;
    A = 22.9898 * g / mole;
    G4Element *elNa = new G4Element(Name = "Natrium", Symbol = "Na", Z, A);
    // 5.
    Z = 12.0;
    A = 24.305 * g / mole;
    G4Element *elMg = new G4Element(Name = "Magnium", Symbol = "Mg", Z, A);
    // 6.
    Z = 13.0;
    A = 26.98154 * g / mole;
    G4Element *elAl = new G4Element(Name = "Aluminium", Symbol = "Al", Z, A);
    // 7.
    Z = 14.0;
    A = 28.086 * g / mole;
    G4Element *elSi = new G4Element(Name = "Silicium", Symbol = "Si", Z, A);
    // 8.
    Z = 20.0;
    A = 40.08 * g / mole;
    G4Element *elCa = new G4Element(Name = "Calcium", Symbol = "Ca", Z, A);
    // 9.
    Z = 26.0;
    A = 55.847 * g / mole;
    G4Element *elFe = new G4Element(Name = "Ferrum", Symbol = "Fe", Z, A);
    // Basalt
    NComponents = 7;
    Density = 2.5 * g / cm3;
    Basalt = new G4Material(Name = "Basalt", Density, NComponents);
    Basalt->AddElement(elO, FractionMass = 0.600);
    Basalt->AddElement(elNa, FractionMass = 0.020);
    Basalt->AddElement(elMg, FractionMass = 0.025);
    Basalt->AddElement(elAl, FractionMass = 0.060);
    Basalt->AddElement(elSi, FractionMass = 0.175);
    Basalt->AddElement(elCa, FractionMass = 0.050);
    Basalt->AddElement(elFe, FractionMass = 0.070);
    // Concrete
    NComponents = 6;
    Density = 2.5 * g / cm3;
    Concrete = new G4Material(Name = "Concrete", Density, NComponents);
    Concrete->AddElement(elO, FractionMass = 0.540);
    Concrete->AddElement(elMg, FractionMass = 0.025);
    Concrete->AddElement(elAl, FractionMass = 0.020);
    Concrete->AddElement(elSi, FractionMass = 0.065);
    Concrete->AddElement(elCa, FractionMass = 0.325);
    Concrete->AddElement(elFe, FractionMass = 0.025);
    // Plastic
    NComponents = 2;
    Density = 1.0 * g / cm3;
    Plastic = new G4Material(Name = "Plastic", Density, NComponents);
    Plastic->AddElement(elH, FractionMass = 0.500); // 1.
    Plastic->AddElement(elC, FractionMass = 0.500); // 2.
    //
    const G4int nE = 3;
    G4double Energy[nE] = {7.00 * eV, 7.07 * eV, 7.14 * eV};
    G4double Eff[nE] = {0.1, 0.1, 0.1};
    G4double Rindex[nE] = {1.58, 1.58, 1.58};
    G4double AbsLength[nE] = {200.0 * cm, 200.0 * cm, 200.0 * cm};
    //
    mt = new G4MaterialPropertiesTable(); // Material Properties: create the object
    Plastic->SetMaterialPropertiesTable(mt);
    mt->AddProperty("FASTCOMPONENT", Energy, Eff, nE);
    mt->AddProperty("SLOWCOMPONENT", Energy, Eff, nE);
    mt->AddProperty("RINDEX", Energy, Rindex, nE);
    mt->AddProperty("ABSLENGTH", Energy, AbsLength, nE);
    mt->AddConstProperty("SCINTILLATIONYIELD", 100.0 / MeV);
    mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mt->AddConstProperty("FASTTIMECONSTANT", 2.0 * ns);
    mt->AddConstProperty("SLOWTIMECONSTANT", 5.0 * ns);
    mt->AddConstProperty("YIELDRATIO", 1.0);
    mt->DumpTable();
    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void C2MuonDetector::DestroyMaterials()
{
    size_t i;
    G4MaterialTable *matTable = (G4MaterialTable *)G4Material::GetMaterialTable();
    for (i = 0; i < matTable->size(); i++)
        delete (*(matTable))[i];
    matTable->clear();
    G4ElementTable *elemTable = (G4ElementTable *)G4Element::GetElementTable();
    for (i = 0; i < elemTable->size(); i++)
        delete (*(elemTable))[i];
    elemTable->clear();
}

void C2MuonDetector::DumpGeometricalTree(G4VPhysicalVolume *aVolume, G4int depth)
{
    for (int isp = 0; isp < depth; isp++)
        G4cout << "  ";
    G4cout << aVolume->GetName() << "[" << aVolume->GetCopyNo() << "] "
           << aVolume->GetLogicalVolume()->GetName() << " "
           << aVolume->GetLogicalVolume()->GetNoDaughters() << " "
           << aVolume->GetLogicalVolume()->GetMaterial()->GetName();
    G4cout << G4endl;
    for (size_t i = 0; i < aVolume->GetLogicalVolume()->GetNoDaughters(); i++)
        DumpGeometricalTree(aVolume->GetLogicalVolume()->GetDaughter(i), depth + 1);
}
