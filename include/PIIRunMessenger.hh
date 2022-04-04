/// \file PIIRunMessenger
/// \brief Definition of the PIIRunMessenger class

#ifndef PIIRunMessenger_h
#define PIIRunMessenger_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4UImessenger.hh"
#include "globals.hh"

class PIIRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class PIIRunMessenger: public G4UImessenger
{
  public:
    PIIRunMessenger(PIIRunAction*);
    virtual ~PIIRunMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    PIIRunAction*            fRunAction;

    G4UIdirectory*           fRunDirectory;
    G4UIcmdWithAString*      fFilenameCmd;
    G4UIcmdWithAnInteger*    fRunidCmd;
    G4UIcmdWithAnInteger*    fOutputCmd;
    G4UIcommand*             fDefaultsCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
