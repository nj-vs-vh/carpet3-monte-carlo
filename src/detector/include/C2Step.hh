#include <ostream>
#include "G4UserSteppingAction.hh"
#include "globals.hh"

class C2Step : public G4UserSteppingAction
{
public:
    C2Step(std::ofstream* output) : outputStream(output) {};
    ~C2Step() {};
    void UserSteppingAction(const G4Step *aStep);

private:
    std::ofstream* outputStream;
};
