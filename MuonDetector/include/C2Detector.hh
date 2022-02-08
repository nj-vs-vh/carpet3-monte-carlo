#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4MaterialPropertiesTable.hh"

class G4Box;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;

class C2Detector:public G4VUserDetectorConstruction
{
    public:
    C2Detector();
    virtual ~C2Detector();
    virtual G4VPhysicalVolume* Construct();
//Volumes
//Solids
    G4VSolid* WorldSolid;
    G4VSolid* RockSolid;
    G4VSolid* ConcreteSolid;
    G4VSolid* TunnelSolid;
    G4VSolid* PlasticSolid;
//Logical
    G4LogicalVolume* WorldLogical;
    G4LogicalVolume* RockLogical;
    G4LogicalVolume* ConcreteLogical;
    G4LogicalVolume* TunnelLogical;
    G4LogicalVolume* PlasticLogical;
//Physical
    G4VPhysicalVolume* WorldPhysical;
    G4VPhysicalVolume* RockPhysical;
    G4VPhysicalVolume* ConcretePhysical;
    G4VPhysicalVolume* TunnelPhysical;
    G4VPhysicalVolume* PlasticPhysical;
//Materials
    G4Material* Vacuum;
    G4Material* Basalt;
    G4Material* Concrete;
    G4Material* Plastic;
    G4MaterialPropertiesTable* mt;
//Visualisation attributes
//    G4VisAttributes* WorldVisAtt;
//    G4VisAttributes* DetectorVisAtt;
    private:
    void ConstructMaterials();
    void DestroyMaterials();
    void DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth=0);
};
