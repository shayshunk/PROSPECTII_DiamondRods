/// \file examplePII.cc
/// \brief Main program of the PII example

#include "PIIDetectorConstruction.hh"
#include "PIIActionInitialization.hh"
#include "PIISteppingAction.hh"
#include "PIIEventAction.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4String.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4cout << "You have entered " << argc << " arguments:" << "\n";

  for (int i = 0; i < argc; ++i)
      G4cout << argv[i] << "\n";

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  // Construct the default run manager
  //
/* #ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  //G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 6);
  runManager->SetNumberOfThreads(1);
  G4cout << "===== PII is started with "
         <<  runManager->GetNumberOfThreads() << " threads =====" << G4endl;
*/
//#else
  G4RunManager* runManager = new G4RunManager;
//#endif

  // Set mandatory initialization classes
  // Detector construction
  PIIDetectorConstruction* detector = new PIIDetectorConstruction();
  runManager->SetUserInitialization(detector);

  // Physics List
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->SetVerboseLevel(0);
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);

  // Set user action classes
  auto eventAction = new PIIEventAction(detector);
  auto stepAction = new PIISteppingAction(detector, eventAction);
  runManager->SetUserInitialization(new PIIActionInitialization(detector, eventAction, stepAction));

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //

  G4String cmdlineEvents = "";
  G4String output = "";
  G4String runid = "";

  for(G4int i = 2; i < argc; ++i) {
          if(G4String(argv[i]) == "-n" && i+1 < argc)
            cmdlineEvents = G4String(argv[++i]);
          else if(G4String(argv[i]) == "-o" && i+1 < argc)
            output = G4String(argv[++i]);
          else if(G4String(argv[i]) == "-r" && i+1 < argc)
            runid = G4String(argv[++i]);
          }

  G4cout << "The number of events: " << cmdlineEvents << G4endl;
  G4cout << "The file output name: " << output << G4endl;
  G4cout << "The run id: " << runid << G4endl;

  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  UImanager->ApplyCommand("/PII/output/runid " + runid);
  UImanager->ApplyCommand("/PII/output/filename " + output);
  UImanager->ApplyCommand("/random/setSeeds " + runid + " " + cmdlineEvents);
  UImanager->ApplyCommand("/run/beamOn " + cmdlineEvents);

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !
  //
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
