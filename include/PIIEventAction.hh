/// \file PIIEventAction.hh
/// \brief Definition of the PIIEventAction class

#ifndef PIIEventAction_h
#define PIIEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include "globals.hh"

class PIIDetectorConstruction;

/// Event action class

class PIIEventAction : public G4UserEventAction
{
  public:
    PIIEventAction(PIIDetectorConstruction* detectorConstruction);
    virtual ~PIIEventAction();

    virtual void           BeginOfEventAction(const G4Event*);
    virtual void           EndOfEventAction(const G4Event*);
    virtual void           AddPhotonHit(G4int PMTno);
    virtual G4int          GetPhotonHit(G4int PMTno);
    virtual G4int          GetPhotonHit2(G4int PMTno);
    virtual void           ResetPhotonHits(G4int PMTno);
    virtual void           SetNoEvents(G4int nEvents);
    virtual G4int          GetNoEvents();
    virtual void           SetEventTime(G4double time);
    virtual G4double       GetEventTime();
    virtual void           SetPos(G4ThreeVector pos);
    virtual G4ThreeVector  GetPos();
    virtual void           SetDirection(G4ThreeVector dir);
    virtual G4ThreeVector  GetDirection();
    virtual void           SetNoRows(G4int rowNum);
    virtual G4int          GetNoRows();
    virtual void           SetNoCols(G4int colNum);
    virtual G4int          GetNoCols();
    virtual void           SetCurrentPMTHit(G4int pmtHit);
    virtual G4int          GetCurrentPMTHit();
    virtual void           SetPhotonFlag(G4int flag);
    virtual G4int          GetPhotonFlag();
    virtual void           SetOutputFiles(G4int outputs);
    virtual G4int          GetOutputFiles();

    G4int eventID;
    G4int* PMTHits;
    G4int* PMTHits2;
    G4int nEvent;
    G4double nTime;
    G4ThreeVector nPos;
    G4ThreeVector nDir;
    G4int fRowNum;
    G4int fColNum;
    G4int currentPMT;
    G4int photonFlag;
    G4int outputFlag;

  private:
    PIIDetectorConstruction* fDetConstruction;
};

// inline functions

inline void PIIEventAction::AddPhotonHit(G4int PMTno) {
  PMTHits[PMTno] = PMTHits[PMTno] + 1;
  PMTHits2[PMTno] = PMTHits2[PMTno] + 1;
}

inline G4int PIIEventAction::GetPhotonHit(G4int PMTno) {
  return PMTHits[PMTno];
}

inline G4int PIIEventAction::GetPhotonHit2(G4int PMTno) {
  return PMTHits2[PMTno];
}

inline void PIIEventAction::ResetPhotonHits(G4int PMTno) {
  PMTHits[PMTno] = 0;
}

inline void PIIEventAction::SetNoEvents(G4int nEvents) {
  nEvent = nEvents;
}

inline G4int PIIEventAction::GetNoEvents() {
  return nEvent;
}

inline void PIIEventAction::SetEventTime(G4double time) {
  nTime = time;
}

inline G4double PIIEventAction::GetEventTime() {
  return nTime;
}

inline void PIIEventAction::SetPos(G4ThreeVector pos) {
  nPos = pos;
}

inline G4ThreeVector PIIEventAction::GetPos() {
  return nPos;
}

inline void PIIEventAction::SetDirection(G4ThreeVector dir) {
  nDir = dir;
}

inline G4ThreeVector PIIEventAction::GetDirection() {
  return nDir;
}

inline void PIIEventAction::SetNoRows(G4int rowNum) {
  fRowNum = rowNum;
}

inline G4int PIIEventAction::GetNoRows() {
  return fRowNum;
}

inline void PIIEventAction::SetNoCols(G4int colNum) {
  fColNum = colNum;
}

inline G4int PIIEventAction::GetNoCols() {
  return fColNum;
}

inline void PIIEventAction::SetCurrentPMTHit(G4int copyNo) {
  currentPMT = copyNo;
}

inline G4int PIIEventAction::GetCurrentPMTHit() {
  return currentPMT;
}

inline void PIIEventAction::SetPhotonFlag(G4int flag) {
  photonFlag = flag;
}

inline G4int PIIEventAction::GetPhotonFlag() {
  return photonFlag;
}

inline void PIIEventAction::SetOutputFiles(G4int outputs) {
  outputFlag = outputs;
}

inline G4int PIIEventAction::GetOutputFiles() {
  return outputFlag;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
