/// \file PIIActionInitialization.cc
/// \brief Implementation of the PIIActionInitialization class

#include "PIIActionInitialization.hh"
#include "PIIPrimaryGeneratorAction.hh"
#include "PIIPrimaryGeneratorMessenger.hh"
#include "PIIRunAction.hh"
#include "PIIRunMessenger.hh"
#include "PIIEventAction.hh"
#include "PIISteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIActionInitialization::PIIActionInitialization(PIIDetectorConstruction* detConstruction, PIIEventAction* eventAction, PIISteppingAction* stepAction)
 : G4VUserActionInitialization(), fDetConstruction(detConstruction), fEventAction(eventAction), fStepAction(stepAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIActionInitialization::~PIIActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIActionInitialization::BuildForMaster() const
{
  SetUserAction(new PIIRunAction(fStepAction, fEventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIActionInitialization::Build() const
{
  SetUserAction(new PIIPrimaryGeneratorAction(fEventAction));
  SetUserAction(new PIIRunAction(fStepAction, fEventAction));
  SetUserAction(fEventAction);
  SetUserAction(fStepAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
