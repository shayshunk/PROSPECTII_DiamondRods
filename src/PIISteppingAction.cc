/// \file PIISteppingAction.cc
/// \brief Implementation of the PIISteppingAction class

#include "PIISteppingAction.hh"
#include "PIIEventAction.hh"
#include "PIIDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIISteppingAction::PIISteppingAction(
                      const PIIDetectorConstruction* detectorConstruction,
                      PIIEventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction), fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIISteppingAction::~PIISteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIISteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step

  // get volume of the current step
  auto volume1 = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
  G4double hit_time = step->GetPreStepPoint()->GetGlobalTime();

  G4String name1 = volume1->GetName();

  //G4cout << "VOLUME IS: " << volume2 << G4endl;

  // energy deposit

  // getting Track
  G4Track* theTrack = step->GetTrack();

  if (name1 == "pmtCathode") {
    fEventAction->AddPhotonHit(copyNo);
    fEventAction->SetCurrentPMTHit(copyNo);
    fEventAction->SetPhotonFlag(1);
    theTrack->SetTrackStatus(fStopAndKill);
    fEventAction->SetEventTime(hit_time);
    //G4cout << "PMT " << copyNo << " got hit at time: " << hit_time << G4endl;
  }
  else if (name1 == "pmtHousing") {
    theTrack->SetTrackStatus(fStopAndKill);
    fEventAction->SetPhotonFlag(-1);
  }
  else if (name1 != "pmtCathode" && name1 != "pmtHousing"){
    fEventAction->SetPhotonFlag(-2);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
