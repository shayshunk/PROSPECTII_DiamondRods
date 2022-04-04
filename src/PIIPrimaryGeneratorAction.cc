/// \file PIIPrimaryGeneratorAction.cc
/// \brief Implementation of the PIIPrimaryGeneratorAction class

#include "PIIPrimaryGeneratorAction.hh"
#include "PIIPrimaryGeneratorMessenger.hh"
#include "PIIEventAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include <sstream>

#include "CLHEP/Units/SystemOfUnits.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIPrimaryGeneratorAction::PIIPrimaryGeneratorAction(PIIEventAction* eventAction)
 : G4VUserPrimaryGeneratorAction(), fEventAction(eventAction)
{

  fGeneratorMessenger = new PIIPrimaryGeneratorMessenger(this);

  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic

  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");

  G4cout << particleDefinition << G4endl;

  SetDefaults();

  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleEnergy(2.92*eV);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIPrimaryGeneratorAction::~PIIPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  G4int eventID = anEvent->GetEventID();
  G4int fRowNum = fEventAction->GetNoRows();
  G4int fColNum = fEventAction->GetNoCols();

  if(eventID == 1){
    G4cout << "Test 1 => Rows: " << fRowNum << G4endl;
    G4cout << "Test 1 => Cols: " << fColNum << G4endl;
  }

  // Get values from commands

  G4ThreeVector pos = GetPosition();
  G4ThreeVector dir;
  G4bool iso = GetIsotropic();
  G4int distrb = GetDistribution();
  G4bool randoms = GetRandomXY();

  // Set up values

  G4double reflectorHeight = 14.478*cm; // tank height from cross section
  G4double chamberLength = 121.92*cm; // length of tank

  G4double randDistx = 1. - 2*G4UniformRand();
  G4double randDisty = 1. - 2*G4UniformRand();
  G4double randDistz = 1. - 2*G4UniformRand();

  G4double randPositionx = randDistx * reflectorHeight * 0.499;
  G4double randPositiony = randDisty * reflectorHeight * 0.499;
  G4double randPositionz = randDistz * chamberLength * 0.499;

  G4double randMomentumx = 1. - 2*G4UniformRand();
  G4double randMomentumy = 1. - 2*G4UniformRand();
  G4double randMomentumz = 1. - 2*G4UniformRand();

  G4double cosTheta = 1. - 2.*G4UniformRand();
  G4double sinTheta = sqrt(1. - cosTheta*cosTheta);
  G4double phi = twopi * G4UniformRand();
  dir = G4ThreeVector( (sinTheta * cos(phi)), (sinTheta * sin(phi)), cosTheta);

  G4ThreeVector polar = G4ThreeVector(randMomentumx, randMomentumy, randMomentumz);

  if (distrb == 1) {

    if (pos == G4ThreeVector(0, 0, 0)) {
      if (randoms == true) {
        pos = G4ThreeVector(randPositionx, randPositiony, 0);
      }
      else{
        pos = G4ThreeVector(0, 0, 0);
      }
    }
    dir = G4ThreeVector( (sinTheta * cos(phi)), (sinTheta * sin(phi)), cosTheta);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(dir);

    fLastPos = pos;

  }

  else if (distrb == 2) {

    pos = G4ThreeVector(randPositionx, randPositiony, randPositionz);
    dir = G4ThreeVector( (sinTheta * cos(phi)), (sinTheta * sin(phi)), cosTheta);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(dir);

    fLastPos = pos;
  }

  else if (distrb == 3) {

    G4int bombFlag;
    G4double zloc;

    if (eventID % 10000 == 0){
      bombFlag = 1;
      zloc = chamberLength*G4UniformRand();
    }
    else{
      bombFlag = 0;
    }

    if (bombFlag == 1){
      pos = G4ThreeVector(6.239*cm, 6.239*cm, (-0.5*chamberLength + zloc));
    }
    else {
      pos = fLastPos;
    }

    dir = G4ThreeVector( (sinTheta * cos(phi)), (sinTheta * sin(phi)), cosTheta);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(dir);

    fLastPos = pos;
  }

  else {

    if (pos == G4ThreeVector(0, 0, 0)) {
      if (randoms == true) {
        pos = G4ThreeVector(randPositionx, randPositiony, 0);
      }
      else{
        pos = G4ThreeVector(0, 0, 0);
      }
    }

    fParticleGun->SetParticlePosition(pos);

    if (iso == true) {
      fParticleGun->SetParticleMomentumDirection(dir);
    }
    else {
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector((sinTheta * cos(phi)), (sinTheta * sin(phi)), -1));
    }
  }

  fParticleGun->SetParticlePolarization(polar);
  fParticleGun->GeneratePrimaryVertex(anEvent);

  fEventAction->SetPos(pos);
  fEventAction->SetDirection(dir);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIPrimaryGeneratorAction::SetPosition(G4ThreeVector pos){
  fPos = pos;
}

void PIIPrimaryGeneratorAction::SetDivisions(G4int div){
  fDivs = div;
}

void PIIPrimaryGeneratorAction::SetDistribution(G4int distr){
  fDistrb = distr;
}

void PIIPrimaryGeneratorAction::SetIsotropic(G4bool isot){
  fIsotropic = isot;
}

void PIIPrimaryGeneratorAction::SetRandomXY(G4bool rands){
  fRandomXY = rands;
}

void PIIPrimaryGeneratorAction::SetDefaults(){

  fPos = G4ThreeVector(0, 0, 0);
  fDivs = 1;
  fDistrb = 2;
  fIsotropic = true;
  fRandomXY = false;

}

G4ThreeVector PIIPrimaryGeneratorAction::GetPosition(){
  return fPos;
}

G4int PIIPrimaryGeneratorAction::GetDivisions(){
  return fDivs;
}

G4int PIIPrimaryGeneratorAction::GetDistribution(){
  return fDistrb;
}

G4bool PIIPrimaryGeneratorAction::GetIsotropic(){
  return fIsotropic;
}

G4bool PIIPrimaryGeneratorAction::GetRandomXY(){
  return fRandomXY;
}
