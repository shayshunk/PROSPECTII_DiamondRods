/// \file PIIDetectorMessenger.hh
/// \brief Definition of the PIIDetectorMessenger class

#ifndef PIIDetectorMessenger_h
#define PIIDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PIIDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for PIIDetectorConstruction.
///
/// It implements commands:
/// - /PII/det/setTargetMaterial name
/// - /PII/det/setChamberMaterial name
/// - /PII/det/stepMax value unit

class PIIDetectorMessenger: public G4UImessenger
{
  public:
    PIIDetectorMessenger(PIIDetectorConstruction* );
    virtual ~PIIDetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    PIIDetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*           fPIIDirectory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
    G4UIcmdWithADoubleAndUnit* fHousingThicknessCmd;
    G4UIcmdWithADoubleAndUnit* fWindowThicknessCmd;
    G4UIcmdWithAnInteger*      fRowNumberCmd;
    G4UIcmdWithAnInteger*      fColNumberCmd;
    G4UIcommand*               fDefaultsCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
