/// \file PIIRunAction.cc
/// \brief Implementation of the PIIRunAction class

#include "PIIRunAction.hh"
#include "PIIRunMessenger.hh"
#include "PIIEventAction.hh"
#include "PIISteppingAction.hh"
#include "PIIAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4Types.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIRunAction::PIIRunAction(PIISteppingAction* stepAction, PIIEventAction* eventAction)
 : G4UserRunAction(), fStepAction(stepAction), fEventAction(eventAction)
{

  fRunMessenger = new PIIRunMessenger(this);
  SetDefaults();

  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(100000);

  // Create analysis manager for recording hits
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  // Set up analysis manager
  man->SetVerboseLevel(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIRunAction::~PIIRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIRunAction::BeginOfRunAction(const G4Run* aRun)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4cout << "~~~~~ Run Number " << fRunNum << " Initiated ~~~~~" << G4endl;
  //G4int seeder = G4UniformRand() * 1000;
  //G4Random::setTheSeed(fRunNum*seeder + 1); // set unique random seed for run --- can't be 0

  // Get analysis manager and open output file
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->OpenFile("PII");

  man->CreateNtuple("Geometry" + filename, "Geometry Info");
  man->CreateNtupleIColumn("Number of PMTs");
  man->CreateNtupleIColumn("Number of Rows");
  man->CreateNtupleIColumn("Number of Columns");
  man->FinishNtuple();

  if (fOutputs == 1){
    man->CreateNtuple("PII_photons_" + filename + fRunid, "Photon Tracking");
    man->CreateNtupleIColumn("PMT Hit");
    man->CreateNtupleIColumn("Event Number");
    man->CreateNtupleDColumn("X position");
    man->CreateNtupleDColumn("Y position");
    man->CreateNtupleDColumn("Z position");
    man->CreateNtupleDColumn("X direction");
    man->CreateNtupleDColumn("Y direction");
    man->CreateNtupleDColumn("Z direction");
    man->CreateNtupleDColumn("Time");
    man->FinishNtuple();
  }

  else if (fOutputs == 2){
    man->CreateNtuple("PII_bombs_" + filename + fRunid, "Bomb Tracking");
    man->CreateNtupleIColumn("Event Number");
    man->CreateNtupleIColumn("Left PMT");
    man->CreateNtupleIColumn("Right PMT");
    man->CreateNtupleDColumn("X position");
    man->CreateNtupleDColumn("Y position");
    man->CreateNtupleDColumn("Z position");
    man->FinishNtuple();
  }

  else if (fOutputs == 3){
    man->CreateNtuple("PII_photons_" + filename + fRunid, "Photon Tracking");
    man->CreateNtupleIColumn("PMT Hit");
    man->CreateNtupleIColumn("Event Number");
    man->CreateNtupleDColumn("X position");
    man->CreateNtupleDColumn("Y position");
    man->CreateNtupleDColumn("Z position");
    man->CreateNtupleDColumn("X direction");
    man->CreateNtupleDColumn("Y direction");
    man->CreateNtupleDColumn("Z direction");
    man->CreateNtupleDColumn("Time");
    man->FinishNtuple();

    man->CreateNtuple("PII_bombs_" + filename + fRunid, "Bomb Tracking");
    man->CreateNtupleIColumn("Event Number");
    man->CreateNtupleIColumn("Left PMT");
    man->CreateNtupleIColumn("Right PMT");
    man->CreateNtupleDColumn("X position");
    man->CreateNtupleDColumn("Y position");
    man->CreateNtupleDColumn("Z position");
    man->FinishNtuple();
  }

  G4int nEvents = aRun->GetNumberOfEventToBeProcessed();
  fEventAction->SetNoEvents(nEvents);
  G4cout << "Number of events set with: " << nEvents << G4endl;
  fEventAction->SetOutputFiles(fOutputs);
  G4cout << "Output files set with " << fOutputs << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIRunAction::EndOfRunAction(const G4Run*)
{

  // Save data
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIRunAction::SetFilename(G4String name)
{
  filename = name;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIRunAction::SetRunid(G4String runid, G4int rnum)
{
  fRunNum = rnum;
  fRunid = runid;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIRunAction::SetDefaults()
{
  filename = "";
  fOutputs = 3;
  fRunid = "";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIRunAction::SetOutputFiles(G4int outputs)
{
  fOutputs = outputs;
}
