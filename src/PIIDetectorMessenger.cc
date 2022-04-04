/// \file PIIDetectorMessenger.cc
/// \brief Implementation of the PIIDetectorMessenger class

#include "PIIDetectorMessenger.hh"
#include "PIIDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIDetectorMessenger::PIIDetectorMessenger(PIIDetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  fPIIDirectory = new G4UIdirectory("/PII/");
  fPIIDirectory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/PII/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/PII/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);

  fRowNumberCmd = new G4UIcmdWithAnInteger("/PII/det/rowNumber", this);
  fRowNumberCmd->SetGuidance("Define the number of rows in the PMT array.");
  fRowNumberCmd->SetGuidance("Whole number must be greater than 0.");
  fRowNumberCmd->SetParameterName("rowNumber", true);
  fRowNumberCmd->SetDefaultValue(2);
  fRowNumberCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fRowNumberCmd->SetToBeBroadcasted(false);

  fColNumberCmd = new G4UIcmdWithAnInteger("/PII/det/colNumber", this);
  fColNumberCmd->SetGuidance("Define the number of columns in the PMT array.");
  fColNumberCmd->SetGuidance("Whole number must be greater than 0.");
  fColNumberCmd->SetParameterName("colNumber", true);
  fColNumberCmd->SetDefaultValue(1);
  fColNumberCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fColNumberCmd->SetToBeBroadcasted(false);

  fWindowThicknessCmd = new G4UIcmdWithADoubleAndUnit("/PII/det/windowThickness", this);
  fWindowThicknessCmd->SetGuidance("Define the acrylic window thickness.");
  fWindowThicknessCmd->SetGuidance("Unit is cm.");
  fWindowThicknessCmd->SetParameterName("windowThickness", true);
  fWindowThicknessCmd->SetDefaultValue(0.635*cm);
  fWindowThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWindowThicknessCmd->SetToBeBroadcasted(false);

  fHousingThicknessCmd = new G4UIcmdWithADoubleAndUnit("/PII/det/housingThickness", this);
  fHousingThicknessCmd->SetGuidance("Define the opaque housing thickness.");
  fHousingThicknessCmd->SetGuidance("Unit is cm.");
  fHousingThicknessCmd->SetParameterName("housingThickness", true);
  fHousingThicknessCmd->SetDefaultValue(0.635*cm);
  fHousingThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fHousingThicknessCmd->SetToBeBroadcasted(false);

  fDefaultsCmd = new G4UIcommand("/PII/det/defaults",this);
  fDefaultsCmd->SetGuidance("Set all generator values to defaults.");
  fDefaultsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fDefaultsCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIDetectorMessenger::~PIIDetectorMessenger()
{
  delete fStepMaxCmd;
  delete fPIIDirectory;
  delete fDetDirectory;
  delete fRowNumberCmd;
  delete fColNumberCmd;
  delete fHousingThicknessCmd;
  delete fWindowThicknessCmd;
  delete fDefaultsCmd;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

  if(command == fStepMaxCmd) {
    fDetectorConstruction->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  }

  if(command == fRowNumberCmd) {
    fDetectorConstruction->SetRowNumb(fRowNumberCmd->GetNewIntValue(newValue));
  }

  if(command == fColNumberCmd) {
    fDetectorConstruction->SetColNumb(fColNumberCmd->GetNewIntValue(newValue));
  }

  if(command == fWindowThicknessCmd) {
    fDetectorConstruction->SetWindowThickness(fWindowThicknessCmd->GetNewDoubleValue(newValue));
  }

  if(command == fHousingThicknessCmd) {
    fDetectorConstruction->SetHousingThickness(fHousingThicknessCmd->GetNewDoubleValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
