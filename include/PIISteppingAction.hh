/// \file PIISteppingAction.hh
/// \brief Definition of the PIISteppingAction class

#ifndef PIISteppingAction_h
#define PIISteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "tls.hh"

class PIIDetectorConstruction;
class PIIEventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in Absober and Gap layers and
/// updated in PIIEventAction.

class PIISteppingAction : public G4UserSteppingAction
{
public:
  PIISteppingAction(const PIIDetectorConstruction* detectorConstruction, PIIEventAction* fEventAction);
  virtual ~PIISteppingAction();

  virtual void UserSteppingAction(const G4Step* step);


private:
  const PIIDetectorConstruction* fDetConstruction;
  PIIEventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
