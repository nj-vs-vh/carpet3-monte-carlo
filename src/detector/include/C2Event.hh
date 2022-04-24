#include "G4UserEventAction.hh"
#include "globals.hh"

class C2Event:public G4UserEventAction
{
    public:
    C2Event() ;
    virtual ~C2Event();
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    void SetDrawFlag   (G4String val)  {drawFlag = val;};
    void SetPrintModulo(G4int    val)  {printModulo = val;};
    private:
    G4String drawFlag;
    G4int    printModulo;
};
