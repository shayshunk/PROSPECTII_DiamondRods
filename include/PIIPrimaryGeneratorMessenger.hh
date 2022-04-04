/// \file PIIPrimaryGeneratorMessenger
/// \brief Definition of the PIIPrimaryGeneratorMessenger class

#ifndef PIIPrimaryGeneratorMessenger_h
#define PIIPrimaryGeneratorMessenger_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

class PIIPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class PIIPrimaryGeneratorMessenger: public G4UImessenger
{
  public:

    PIIPrimaryGeneratorMessenger(PIIPrimaryGeneratorAction*);
    virtual ~PIIPrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);


  private:

    PIIPrimaryGeneratorAction*    fPrimaryGenerator;
    G4UIdirectory*                fGeneratorDir;
    G4UIcmdWith3VectorAndUnit*    fPositionCmd;
    G4UIcmdWithAnInteger*         fDivisionsCmd;
    G4UIcmdWithABool*             fIsotropicCmd;
    G4UIcmdWithAnInteger*         fDistributionCmd;
    G4UIcmdWithABool*             fRandomXYCmd;
    G4UIcommand*                  fDefaultsCmd;

};

#endif
