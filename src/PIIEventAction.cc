/// \file PIIEventAction.cc
/// \brief Implementation of the PIIEventAction class

#include "PIIEventAction.hh"
#include "PIIAnalysis.hh"
#include "PIIDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIEventAction::PIIEventAction(PIIDetectorConstruction* detectorConstruction)
: G4UserEventAction(),
  fDetConstruction(detectorConstruction)
{
  G4int fNbOfPMTs = fDetConstruction->GetNoPMT();
  PMTHits = new G4int[fNbOfPMTs];
  PMTHits2 = new G4int[fNbOfPMTs];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIEventAction::~PIIEventAction()
{
  delete PMTHits;
  delete PMTHits2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIEventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIEventAction::EndOfEventAction(const G4Event* event)
{

  G4int nbOfPMTs = fDetConstruction->GetNoPMT();
  G4int nbOfRows = fDetConstruction->GetNoRows();
  G4int nbOfCols = fDetConstruction->GetNoCols();
  G4int copyNo = GetCurrentPMTHit();

  if(photonFlag != 1){
    copyNo = photonFlag;
  }

  SetNoRows(nbOfRows);
  SetNoCols(nbOfCols);

  G4int* hits = nullptr;
  G4int* hits2 = nullptr;
  hits = new G4int[nbOfPMTs];
  hits2 = new G4int[nbOfPMTs];

  for(G4int c = 0; c < nbOfPMTs; c++){
    hits[c] = GetPhotonHit(c);
    hits2[c] = GetPhotonHit2(c);
  }
  // periodic printing

  G4int nEvents = GetNoEvents();
  G4ThreeVector pos = GetPos();
  G4ThreeVector dir = GetDirection();

  G4double x_pos = pos.x();
  G4double y_pos = pos.y();
  G4double z_pos = pos.z();

  G4double x_dir = dir.x();
  G4double y_dir = dir.y();
  G4double z_dir = dir.z();

  G4double time = GetEventTime();

  eventID = event->GetEventID();

  // Get analysis manager
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  if (eventID == (nEvents - 1)) {
    G4cout << ">>> Event: " << eventID + 1 << G4endl;

    for(G4int counter = 0; counter < nbOfPMTs; counter ++){
      G4cout << "    "
             << hits2[counter] << " hits stored in PMT " << (counter + 1) << G4endl;
    }

    G4cout << "Number of events: " << nEvents << G4endl;

    man->FillNtupleIColumn(0, 0, nbOfPMTs);
    man->FillNtupleIColumn(0, 1, nbOfRows);
    man->FillNtupleIColumn(0, 2, nbOfCols);
    man->AddNtupleRow(0);
  }

  // Fill ntuple
  if(outputFlag == 1){

    man->FillNtupleIColumn(1, 0, eventID+1);
    man->FillNtupleIColumn(1, 1, copyNo);
    man->FillNtupleDColumn(1, 2, x_pos);
    man->FillNtupleDColumn(1, 3, y_pos);
    man->FillNtupleDColumn(1, 4, z_pos);
    man->FillNtupleDColumn(1, 5, x_dir);
    man->FillNtupleDColumn(1, 6, y_dir);
    man->FillNtupleDColumn(1, 7, z_dir);
    man->FillNtupleDColumn(1, 8, time);
    man->AddNtupleRow(1);
  }

  else if(outputFlag == 2){

    if ((eventID + 1) % 10000 == 0){
      G4cout << "Printing to file." << G4endl;
      man->FillNtupleIColumn(1, 0, eventID+1);
      man->FillNtupleIColumn(1, 1, hits[4]);
      man->FillNtupleIColumn(1, 2, hits[13]);
      man->FillNtupleDColumn(1, 3, x_pos);
      man->FillNtupleDColumn(1, 4, y_pos);
      man->FillNtupleDColumn(1, 5, z_pos);
      man->AddNtupleRow(1);

      for(G4int c = 0; c < nbOfPMTs; c++){
        ResetPhotonHits(c);
      }
    }
  }
  else if(outputFlag == 3){

    man->FillNtupleIColumn(1, 0, eventID+1);
    man->FillNtupleIColumn(1, 1, copyNo);
    man->FillNtupleDColumn(1, 2, x_pos);
    man->FillNtupleDColumn(1, 3, y_pos);
    man->FillNtupleDColumn(1, 4, z_pos);
    man->FillNtupleDColumn(1, 5, x_dir);
    man->FillNtupleDColumn(1, 6, y_dir);
    man->FillNtupleDColumn(1, 7, z_dir);
    man->FillNtupleDColumn(1, 8, time);
    man->AddNtupleRow(1);

    if ((eventID + 1) % 10000 == 0){

      man->FillNtupleIColumn(2, 0, eventID+1);
      man->FillNtupleIColumn(2, 1, hits[4]);
      man->FillNtupleIColumn(2, 2, hits[13]);
      man->FillNtupleDColumn(2, 3, x_pos);
      man->FillNtupleDColumn(2, 4, y_pos);
      man->FillNtupleDColumn(2, 5, z_pos);
      man->AddNtupleRow(2);

      for(G4int c = 0; c < nbOfPMTs; c++){
        ResetPhotonHits(c);
      }
    }
  }

  // Freeing Memory
  delete[] hits;
  delete[] hits2;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
