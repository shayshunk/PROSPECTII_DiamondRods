/// \file PIIPrimaryGeneratorAction.cc
/// \brief Implementation of the PIIPrimaryGeneratorAction class


#include "PIIPrimaryGeneratorMessenger.hh"
#include "PIIPrimaryGeneratorAction.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

PIIPrimaryGeneratorMessenger::PIIPrimaryGeneratorMessenger(PIIPrimaryGeneratorAction* generator)
  : fPrimaryGenerator(generator)
{

  fGeneratorDir = new G4UIdirectory("/PII/generator/");
  fGeneratorDir->SetGuidance("Particle Generation control");

  fPositionCmd = new G4UIcmdWith3VectorAndUnit("/PII/generator/position", this);
  fPositionCmd->SetGuidance("Set position of particle gun.");
  fPositionCmd->SetGuidance("Origin is center of world.");
  fPositionCmd->SetGuidance("Use units, cm recommended.");
  fPositionCmd->SetGuidance("Reflector height is 14.478 cm. Chamber length is 121.92 cm.");
  fPositionCmd->SetGuidance("Default position is (0, -7.239 cm, 0)");
  fPositionCmd->SetParameterName("positionx", "positiony", "positionz", true);
  fPositionCmd->SetDefaultValue(G4ThreeVector(0, -7.239, 0));
  fPositionCmd->SetDefaultUnit("cm");
  fPositionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fPositionCmd->SetToBeBroadcasted(false);

  fDivisionsCmd = new G4UIcmdWithAnInteger("/PII/generator/division", this);
  fDivisionsCmd->SetGuidance("Set number of divisions.");
  fDivisionsCmd->SetGuidance("No unit.");
  fDivisionsCmd->SetGuidance("Default value is 1. Value must be 1 or greater.");
  fDivisionsCmd->SetParameterName("divisions", true);
  fDivisionsCmd->SetDefaultValue(1);
  fDivisionsCmd->SetRange("divisions >= 1");
  fDivisionsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fDivisionsCmd->SetToBeBroadcasted(false);

  fIsotropicCmd = new G4UIcmdWithABool("/PII/generator/isotropic", this);
  fIsotropicCmd->SetGuidance("Set whether source is isotropic.");
  fIsotropicCmd->SetGuidance("If not isotropic, source will be 2pi, pointed at PMT 1.");
  fIsotropicCmd->SetGuidance("Default value is true.");
  fIsotropicCmd->SetParameterName("isotropic", true);
  fIsotropicCmd->SetDefaultValue(true);
  fIsotropicCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fIsotropicCmd->SetToBeBroadcasted(false);

  fDistributionCmd = new G4UIcmdWithAnInteger("/PII/generator/distribution", this);
  fDistributionCmd->SetGuidance("Set particle distribution:");
  fDistributionCmd->SetGuidance("1. Isotropic, one source, centered in optical segment.");
  fDistributionCmd->SetGuidance("2. Isotropic, source moves along z-axis through given divisions.");
  fDistributionCmd->SetGuidance("3. Isotropic, new random position per photon inside bottom optical segment.");
  fDistributionCmd->SetGuidance("Default value is 1.");
  fDistributionCmd->SetParameterName("distribution", true);
  fDistributionCmd->SetDefaultValue(1);
  fDistributionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fDistributionCmd->SetToBeBroadcasted(false);

  fRandomXYCmd = new G4UIcmdWithABool("/PII/generator/randomXY", this);
  fRandomXYCmd->SetGuidance("Set whether distribution is random in x and y, or centered on z-axis");
  fRandomXYCmd->SetGuidance("Default value is false.");
  fRandomXYCmd->SetParameterName("randomXY", true);
  fRandomXYCmd->SetDefaultValue(false);
  fRandomXYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fDefaultsCmd = new G4UIcommand("/PII/generator/defaults",this);
  fDefaultsCmd->SetGuidance("Set all generator values to defaults.");
  fDefaultsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fDefaultsCmd->SetToBeBroadcasted(false);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIPrimaryGeneratorMessenger::~PIIPrimaryGeneratorMessenger()
{

  delete fPositionCmd;
  delete fDivisionsCmd;
  delete fIsotropicCmd;
  delete fDistributionCmd;
  delete fDefaultsCmd;

}

void PIIPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  if (command == fPositionCmd) {
    fPrimaryGenerator->SetPosition(fPositionCmd->GetNew3VectorValue(newValue));
  }

  else if (command == fDivisionsCmd) {
    fPrimaryGenerator->SetDivisions(fDivisionsCmd->GetNewIntValue(newValue));
  }

  else if (command == fIsotropicCmd) {
    fPrimaryGenerator->SetIsotropic(fIsotropicCmd->GetNewBoolValue(newValue));
  }

  else if (command == fDistributionCmd) {
    fPrimaryGenerator->SetDistribution(fDistributionCmd->GetNewIntValue(newValue));
  }

  else if (command == fRandomXYCmd) {
    fPrimaryGenerator->SetRandomXY(fRandomXYCmd->GetNewBoolValue(newValue));
  }

  else if (command == fDefaultsCmd) {
    fPrimaryGenerator->SetDefaults();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
