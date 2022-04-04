/// \file PIITrackerHit.hh
/// \brief Definition of the PIITrackerHit class

#ifndef PIITrackerHit_h
#define PIITrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, PMTNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fPMTNb, fEdep, fPos

class PIITrackerHit : public G4VHit
{
  public:
    PIITrackerHit();
    PIITrackerHit(const PIITrackerHit&);
    virtual ~PIITrackerHit();

    // operators
    const PIITrackerHit& operator=(const PIITrackerHit&);
    G4bool operator==(const PIITrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetPMTNb    (G4int pmtn)       { fPMTNb = pmtn; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetPMTNb() const       { return fPMTNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

  private:

      G4int         fTrackID;
      G4int         fPMTNb;
      G4double      fEdep;
      G4ThreeVector fPos;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<PIITrackerHit> PIITrackerHitsCollection;

extern G4ThreadLocal G4Allocator<PIITrackerHit>* PIITrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* PIITrackerHit::operator new(size_t)
{
  if(!PIITrackerHitAllocator)
      PIITrackerHitAllocator = new G4Allocator<PIITrackerHit>;
  return (void *) PIITrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PIITrackerHit::operator delete(void *hit)
{
  PIITrackerHitAllocator->FreeSingle((PIITrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
