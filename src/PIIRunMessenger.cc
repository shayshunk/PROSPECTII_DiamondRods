/// \file PIIRunMessenger.cc
/// \brief Implementation of the PIIRunMessenger class

#include "PIIRunMessenger.hh"
#include "PIIRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIRunMessenger::PIIRunMessenger(PIIRunAction* runner)
 : fRunAction(runner)
{
  fRunDirectory = new G4UIdirectory("/PII/output/");
  fRunDirectory->SetGuidance("Run Action commands.");

  fFilenameCmd = new G4UIcmdWithAString("/PII/output/filename", this);
  fFilenameCmd->SetGuidance("Set filename.");
  fFilenameCmd->SetParameterName("filenames", true);
  fFilenameCmd->SetDefaultValue("");
  fFilenameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fFilenameCmd->SetToBeBroadcasted(false);

  fRunidCmd = new G4UIcmdWithAnInteger("/PII/output/runid", this);
  fRunidCmd->SetGuidance("Set run number to append to filename.");
  fRunidCmd->SetParameterName("runid", true);
  fRunidCmd->SetDefaultValue(0);
  fRunidCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fRunidCmd->SetToBeBroadcasted(false);

  fOutputCmd = new G4UIcmdWithAnInteger("/PII/output/files", this);
  fOutputCmd->SetGuidance("Set which output files to create.");
  fOutputCmd->SetGuidance("1 is for only photon-level data.");
  fOutputCmd->SetGuidance("2 is for only bomb-level data.");
  fOutputCmd->SetGuidance("3 is for both files.");

  fDefaultsCmd = new G4UIcommand("/output/defaults", this);
  fDefaultsCmd->SetGuidance("Sets filename to default");
  fDefaultsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fDefaultsCmd->SetToBeBroadcasted(false);
}

PIIRunMessenger::~PIIRunMessenger()
{
  delete fFilenameCmd;
}

void PIIRunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  if (command == fFilenameCmd) {
    fRunAction->SetFilename(newValue);
  }
  else if (command == fRunidCmd) {
    fRunAction->SetRunid(newValue, fRunidCmd->GetNewIntValue(newValue));
  }
  else if (command == fOutputCmd) {
    fRunAction->SetOutputFiles(fOutputCmd->GetNewIntValue(newValue));
  }
  else if (command == fDefaultsCmd) {
    fRunAction->SetDefaults();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
