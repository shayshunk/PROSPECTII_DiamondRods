/// \file PIIPrimaryGeneratorAction.hh
/// \brief Definition of the PIIPrimaryGeneratorAction class

#ifndef PIIPrimaryGeneratorAction_h
#define PIIPrimaryGeneratorAction_h 1

#include "PIIPrimaryGeneratorMessenger.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class PIIEventAction;

/// The primary generator action class with particle gun.
///
/// It defines a single particle which hits the Tracker
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class
/// (see the macros provided with this example).

class PIIPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PIIPrimaryGeneratorAction(PIIEventAction* eventAction);
    virtual ~PIIPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* );

    G4ParticleGun* GetParticleGun() {return fParticleGun;}

    // Set command methods

    void SetPosition(G4ThreeVector);
    void SetDivisions(G4int);
    void SetIsotropic(G4bool);
    void SetDistribution(G4int);
    void SetRandomXY(G4bool);
    void SetDefaults();

    // Set return methods

    G4ThreeVector     GetPosition();
    G4int             GetDivisions();
    G4bool            GetIsotropic();
    G4int             GetDistribution();
    G4bool            GetRandomXY();

  private:
    G4ParticleGun*  fParticleGun; // G4 particle gun
    PIIEventAction* fEventAction;
    PIIPrimaryGeneratorMessenger* fGeneratorMessenger;
    G4ThreeVector   fPos;
    G4ThreeVector   fLastPos;
    G4int           fDivs;
    G4int           fDistrb;
    G4bool          fIsotropic;
    G4bool          fRandomXY;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
