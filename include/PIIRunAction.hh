/// \file PIIRunAction.hh
/// \brief Definition of the PIIRunAction class

#ifndef PIIRunAction_h
#define PIIRunAction_h 1

#include "G4UserRunAction.hh"
#include "PIIRunMessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class PIISteppingAction;
class PIIEventAction;

/// Run action class

class PIIRunAction : public G4UserRunAction
{
  public:
    PIIRunAction(PIISteppingAction* stepAction, PIIEventAction* eventAction);
    virtual ~PIIRunAction();

    virtual void   BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);
    virtual void   SetDefaults();
    virtual void   SetFilename(G4String);
    virtual void   SetRunid(G4String, G4int);
    virtual void   SetOutputFiles(G4int);

    PIISteppingAction* fStepAction;
    PIIEventAction*    fEventAction;

    G4String filename;
    G4String fRunid;
    G4int    fRunNum;
    G4int    fOutputs;

  private:
    PIIRunMessenger* fRunMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
