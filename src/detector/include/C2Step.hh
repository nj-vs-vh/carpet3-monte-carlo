#include "G4UserSteppingAction.hh"
#include "globals.hh"

class C2Step : public G4UserSteppingAction
{
public:
    C2Step();
    ~C2Step();
    void UserSteppingAction(const G4Step *aStep);
    //
    FILE *fp;

private:
};
