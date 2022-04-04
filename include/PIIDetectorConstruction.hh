/// \file PIIDetectorConstruction.hh
/// \brief Definition of the PIIDetectorConstruction class

#ifndef PIIDetectorConstruction_h
#define PIIDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4OpticalSurface.hh"
#include "tls.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;

class PIIDetectorMessenger;

/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.

class PIIDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    PIIDetectorConstruction();
    virtual ~PIIDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    virtual G4int GetNoPMT();
    virtual G4int GetNoRows();
    virtual G4int GetNoCols();

    // Set methods
    void SetMaxStep(G4double);
    void SetCheckOverlaps(G4bool);
    void SetRowNumb(G4int);
    void SetColNumb(G4int);
    void SetWindowThickness(G4double);
    void SetHousingThickness(G4double);
    void SetDefaults();

  private:
    // methods
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // data members
    G4int fRowNum;
    G4int fColNum;
    G4double fWindowThickness;
    G4double fHousingThickness;
    G4int fNbOfPMTs;
    G4int fNbOfReflectors;

    G4Material* air;
    G4Material* nylon;
    G4Material* glass;
    G4Material* minOil;
    G4Material* carbon_plane;
    G4Material* acrylic;
    G4Material* refMat;
    G4Material* scintMat;
    G4Material* tabMat;

    G4OpticalSurface* surfOpt;
    G4OpticalSurface* surfLightG;
    G4OpticalSurface* tabMatSurf;

    G4LogicalVolume**   fLogicReflector; // pointer to the logical Reflector array

    G4UserLimits* fStepLimit;            // pointer to user step limits

    PIIDetectorMessenger*  fDetMessenger;   // messenger

    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;
                                         // magnetic field messenger

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};

// inline functions

inline G4int PIIDetectorConstruction::GetNoPMT() {
  return fNbOfPMTs;
}

inline G4int PIIDetectorConstruction::GetNoRows() {
  return fRowNum;
}

inline G4int PIIDetectorConstruction::GetNoCols() {
  return fColNum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
