/// \file PIIDetectorConstruction.cc
/// \brief Implementation of the PIIDetectorConstruction class

#include "PIIDetectorConstruction.hh"
#include "PIIDetectorMessenger.hh"
#include "PIITrackerSD.hh"

#include "G4RunManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Polyhedra.hh"
#include "G4Cons.hh"
#include "G4ExtrudedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4MaterialPropertiesTable.hh"

#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RotationMatrix.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"
#include "G4TwoVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal
G4GlobalMagFieldMessenger* PIIDetectorConstruction::fMagFieldMessenger = 0;

PIIDetectorConstruction::PIIDetectorConstruction()
:G4VUserDetectorConstruction(),
 fLogicReflector(NULL),
 fStepLimit(NULL),
 fCheckOverlaps(true)
{
  fDetMessenger = new PIIDetectorMessenger(this);

  SetDefaults();

  fLogicReflector = new G4LogicalVolume*[2];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PIIDetectorConstruction::~PIIDetectorConstruction()
{
  delete [] fLogicReflector;
  delete fStepLimit;
  delete fDetMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PIIDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIDetectorConstruction::SetDefaults() {

  fRowNum = 3;
  fColNum = 3;
  fWindowThickness = 0.635*cm;
  fHousingThickness = 0.9525*cm;
  fNbOfPMTs = fRowNum*fColNum*2;
  fNbOfReflectors = fRowNum*(fColNum + 1) + fColNum*(fRowNum + 1);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIDetectorConstruction::DefineMaterials()
{
  // Pre-built G4 materials
  const G4int nE = 3;
  G4double photon_energies[nE] = {2.38*eV, 2.91*eV, 3.44*eV}; // range of scintillation energies used

  G4NistManager* nist = G4NistManager::Instance();

  nist->FindOrBuildMaterial("G4_AIR");
  nist->FindOrBuildMaterial("G4_NYLON-6-6");
  nist->FindOrBuildMaterial("G4_GLASS_PLATE");

  air  = G4Material::GetMaterial("G4_AIR");
  nylon  = G4Material::GetMaterial("G4_NYLON-6-6");
  glass = G4Material::GetMaterial("G4_GLASS_PLATE");

  // Glass Properties
  G4double RINDEX_Glass[nE] = {1.46, 1.46, 1.46};
  G4MaterialPropertiesTable* myGlass = new G4MaterialPropertiesTable();
  myGlass->AddProperty("RINDEX", photon_energies, RINDEX_Glass, nE);
  glass->SetMaterialPropertiesTable(myGlass);

  // Adding refractive index for air as a failsafe
  G4double RIndex_air[nE] = {1.0003, 1.0003, 1.0003};
  G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
  mptAir->AddProperty("RINDEX", photon_energies, RIndex_air, nE);
  air->SetMaterialPropertiesTable(mptAir);

  // Mineral oil defined manually
  G4double room_T = 293.15*kelvin;
  minOil = new G4Material("Mineral Oil CH1.1", 0.877*g/cm3, 2, kStateLiquid, room_T);
  G4Element* elC_Graphite = new G4Element("TS_C_of_Graphite", "C_GRAPHITE", 6.0, 12.0107*g/mole);
  G4Element* elH_Water = new G4Element("TS_H_of_Water", "H_WATER", 1.0, 1.0079*g/mole);
  minOil->AddElement(elC_Graphite, 91.53*perCent);
  minOil->AddElement(elH_Water, 8.47*perCent);

  // Mineral oil properties
  G4double RIndex3[nE];
  for(int i = 0; i < nE; i++)
    RIndex3[i] = 1.48;

  G4double AbsLength3[nE];
  for(int i = 0; i < nE; i++)
    AbsLength3[i] = 4.*m;

  G4MaterialPropertiesTable* mptOil = new G4MaterialPropertiesTable();
  mptOil->AddProperty("RINDEX", photon_energies, RIndex3, nE);
  mptOil->AddProperty("ABSLENGTH", photon_energies, AbsLength3, nE);
  minOil->SetMaterialPropertiesTable(mptOil);

  //  ------------- Elements -------------
  G4double a, z, density;
  G4String symbol;
  G4int nel;

  a = 0.01*g/mole;
  G4Element* elH  = new G4Element("Hydrogen", symbol="H", z=1., a);

  a = 12.01*g/mole;
  G4Element* elC  = new G4Element("Carbon",   symbol="C", z=6., a);

  a = 16.00*g/mole;
  G4Element* elO  = new G4Element("Oxygen",   symbol="O", z=8., a);

  // PMT made of Carbon
  density = 2.26*g/cm3;
  carbon_plane = new G4Material("Carbon plane", density, nel = 1);
  carbon_plane->AddElement(elC, 5);

  G4double rindex_pmt[nE] =  {1.92, 1.92, 1.92};
  G4double efficiency_pmt[nE] = {1.0, 1.0, 1.0};

  G4MaterialPropertiesTable* MPT_pmt = new G4MaterialPropertiesTable();
  MPT_pmt->AddProperty("RINDEX", photon_energies, rindex_pmt, nE);
  MPT_pmt->AddProperty("EFFICIENCY", photon_energies, efficiency_pmt, nE);

  carbon_plane->SetMaterialPropertiesTable(MPT_pmt);

  // PMMA C5H8O2 ( Acrylic )
  density = 1.19*g/cm3;
  acrylic = new G4Material("Acrylic", density, nel=3);
  acrylic->AddElement(elC, 5);
  acrylic->AddElement(elH, 8);
  acrylic->AddElement(elO, 2);

  G4double rindex_ac[nE] = {1.492, 1.492, 1.492};

  G4MaterialPropertiesTable* MPT_Acrylic = new G4MaterialPropertiesTable();
  MPT_Acrylic->AddProperty("RINDEX", photon_energies, rindex_ac, nE);

  acrylic->SetMaterialPropertiesTable(MPT_Acrylic);

  // Reflector Panels
  G4double Reflectivity3[nE]    = { 1, 1, 1 };
  G4double RefractiveIndex3[nE] = { 1.34, 1.34, 1.34 };
  G4double Efficiency3[nE]      = { 0.0, 0.0, 0.0 };
  G4double SpecularLobe3[nE]    = { 0.0, 0.0, 0.0 };
  G4double SpecularSpike3[nE]   = { 0.98, 0.98, 0.98 };
  G4double Backscatter3[nE]     = { 0.0, 0.0, 0.0 };

  G4MaterialPropertiesTable* mst = new G4MaterialPropertiesTable();
  mst->AddProperty("RINDEX", photon_energies, RefractiveIndex3, nE);
  mst->AddProperty("REFLECTIVITY", photon_energies, Reflectivity3, nE);
  mst->AddProperty("EFFICIENCY", photon_energies, Efficiency3, nE);
  mst->AddProperty("SPECULARLOBECONSTANT", photon_energies, SpecularLobe3, nE);
  mst->AddProperty("SPECULARSPIKECONSTANT", photon_energies, SpecularSpike3, nE);
  mst->AddProperty("BACKSCATTERCONSTANT", photon_energies, Backscatter3, nE);

  surfOpt = new G4OpticalSurface("reflectorOptSurface");
  surfOpt->SetType(dielectric_metal);
  surfOpt->SetModel(unified);
  surfOpt->SetFinish(polishedbackpainted);
  surfOpt->SetSigmaAlpha(30./180.*M_PI);
  surfOpt->SetMaterialPropertiesTable(mst);

  density = 1.008*g/cm3;
  refMat = new G4Material("RefMat", density, nel=1);
  refMat->AddElement(elC, 1);

  // Light Guides
  G4double LightGReflectivity3[nE]    = { 0.995, 0.995, 0.995 };
  G4double LightGEfficiency3[nE]      = { 0.0, 0.0, 0.0 };
  G4double LightGSpecularLobe3[nE]    = { 0.0, 0.0, 0.0 };
  G4double LightGSpecularSpike3[nE]   = { 0.99, 0.99, 0.99 };
  G4double LightGBackscatter3[nE]     = { 0.0, 0.0, 0.0 };

  G4MaterialPropertiesTable* mstLightG = new G4MaterialPropertiesTable();
  mstLightG->AddProperty("REFLECTIVITY", photon_energies, LightGReflectivity3, nE);
  mstLightG->AddProperty("EFFICIENCY", photon_energies, LightGEfficiency3, nE);
  mstLightG->AddProperty("SPECULARLOBECONSTANT", photon_energies, LightGSpecularLobe3, nE);
  mstLightG->AddProperty("SPECULARSPIKECONSTANT", photon_energies, LightGSpecularSpike3, nE);
  mstLightG->AddProperty("BACKSCATTERCONSTANT", photon_energies, LightGBackscatter3, nE);

  surfLightG = new G4OpticalSurface("lightGSurface");
  surfLightG->SetType(dielectric_metal);
  surfLightG->SetModel(unified);
  surfLightG->SetFinish(polished);
  surfLightG->SetSigmaAlpha(30/180 * M_PI);
  surfLightG->SetMaterialPropertiesTable(mstLightG);

  // Scintillator
  density = 1.008*g/cm3;
  scintMat = new G4Material("ScintMat", density, nel=1);
  scintMat->AddElement(elH, 1);

  G4double rindex_sc[nE] = {1.55, 1.55, 1.55};
  G4double absorption[nE] = {145*cm, 145*cm, 145*cm};

  G4MaterialPropertiesTable* ScintMPT = new G4MaterialPropertiesTable();

  ScintMPT->AddProperty("RINDEX", photon_energies, rindex_sc, nE);
  ScintMPT->AddProperty("ABSLENGTH", photon_energies, absorption, nE);

  scintMat->SetMaterialPropertiesTable(ScintMPT);

  // Corner Tabs
  tabMat = new G4Material("TabMat", density, nel = 1);
  tabMat->AddElement(elC, 1);

  G4double tabMatReflectivity3[nE]  = { 0.95, 0.95, 0.95 };
  G4double tabMatEfficiency3[nE]    = { 0.0, 0.0, 0.0};
  G4double tabMatSpecularLobe3[nE]  = { 0.0, 0.0, 0.0 };
  G4double tabMatSpecularSpike3[nE] = { 0.01, 0.01, 0.01 };
  G4double tabMatBackscatter3[nE]   = { 0.0, 0.0, 0.0 };

  G4MaterialPropertiesTable* mstTabMat = new G4MaterialPropertiesTable();
  mstTabMat->AddProperty("REFLECTIVITY", photon_energies, tabMatReflectivity3, nE);
  mstTabMat->AddProperty("EFFICIENCY", photon_energies, tabMatEfficiency3, nE);
  mstTabMat->AddProperty("SPECULARLOBECONSTANT", photon_energies, tabMatSpecularLobe3, nE);
  mstTabMat->AddProperty("SPECULARSPIKECONSTANT", photon_energies, tabMatSpecularSpike3, nE);
  mstTabMat->AddProperty("BACKSCATTERCONSTANT", photon_energies, tabMatBackscatter3, nE);

  tabMatSurf = new G4OpticalSurface("TabMatSurface");
  tabMatSurf->SetType(dielectric_metal);
  tabMatSurf->SetModel(unified);
  tabMatSurf->SetFinish(ground);
  tabMatSurf->SetSigmaAlpha(240/180 * M_PI);
  tabMatSurf->SetMaterialPropertiesTable(mstTabMat);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PIIDetectorConstruction::DefineVolumes()
{
  // Defining measurements;
  G4double chamberLength = 121.92*cm; // length of tank
  G4double chamberThickness = 1.27*cm; // thickness of tank walls

  G4double pmtRadius = 8.25*cm; // circular PMT radius
  G4double pmtInner = 8.0*cm; // a solid cylinder
  G4double pmtDistance = 6.15*cm; // circular PMT thickness
  G4double pmtSpacing = 1*cm; // distance from PMT face to window
  G4double pmtWindow = 1.962*cm; // distance from PMT face to photocathode
  G4double pmtMountWidth = 14.732*cm - fHousingThickness*2; // square PMT mounts
  G4double pmtMountLength = 7*cm; // length of mount
  G4double xLeftMisregistration = 0*cm;
  G4double yLeftMisregistration = 0*cm;
  G4double xRightMisregistration = 0*cm;
  G4double yRightMisregistration = 0*cm;
  G4double pmtOuterMountWidth = 14.732*cm;

  G4double reflectorLength = 121.92*cm; // reflector should run completely along inside of tank
  G4double reflectorThickness = 0.254*cm; // reflector thickness approx
  G4double reflectorHeight = 14.478*cm; // reflector should run completely along inside of tank
  G4double reflectorWidth = 14.732*cm; // reflector should match width of inside of tank

  G4double scintLength = 121.92*cm; // scintillator size defined to fully fit one optical segment
  G4double scintWidth = 14.478*cm;
  G4double scintHeight = 14.478*cm;

  G4double tabLength = 6.7733*cm;
  G4double tabSpacing = 1.27*cm;

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // Computing array size

  G4double chamberWidth = fColNum * reflectorWidth + 5*cm; // tank width from cross section
  G4double chamberHeight = fRowNum * reflectorHeight + 7*cm; // tank height from cross section

  G4double tankWidth = chamberWidth + 10*cm;
  G4double tankHeight = chamberHeight + 10*cm;
  G4double tankLength = chamberLength + 30*cm;

  G4double worldLength = tankLength*1.1; // appropriate larger boundaries
  G4double worldWidth = tankWidth*1.1;
  G4double worldHeight = tankHeight*1.1;

  G4double dRowNum = fRowNum;
  G4double dColNum = fColNum;
  G4double rowCenter = dRowNum/2.0;
  G4double colCenter = dColNum/2.0;

  G4cout << "Row Center = " << rowCenter << G4endl;
  G4cout << "Column Center = " << colCenter << G4endl;

  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
                worldWidth*0.5, worldHeight*0.5, worldLength*0.5); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 minOil,      //its material
                 "World"); //its name

  //  Must place the World Physical volume unrotated at (0,0,0).

  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps


  // Detector Volume

  G4Box* detectS
    = new G4Box("detectorVolume",
                 tankWidth*0.5, tankHeight*0.5, tankLength*0.5);

  G4LogicalVolume* detectLV
    = new G4LogicalVolume(
                    detectS,
                    minOil,
                    "DetectorVolume");

  new G4PVPlacement(0,               // no rotation
                    G4ThreeVector(),  // at (x,y,z)
                    detectLV,    // its logical volume
                    "DetectorVolume",        // its name
                    worldLV,         // its mother volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps


  // Tank

  G4Box* innerCell
    = new G4Box("innerCell",
                chamberWidth*0.5, chamberHeight*0.5, chamberLength*0.5);
  G4Box* outerCell
    = new G4Box("outerCell",
                (chamberWidth*0.5 + chamberThickness), (chamberHeight*0.5 + chamberThickness), (chamberLength*0.5 + fWindowThickness));
  G4SubtractionSolid* TankS = new G4SubtractionSolid("Tank", outerCell, innerCell); // Hollow acrylic box for tank

  G4LogicalVolume* TankLV
    = new G4LogicalVolume(TankS, acrylic, "Tank");
  new G4PVPlacement(0,               // no rotation
                    G4ThreeVector(0, 0, 0),  // at (x,y,z)
                    TankLV,    // its logical volume
                    "Tank",        // its name
                    detectLV,         // its mother volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps

  G4cout << "Tank is " << chamberLength/cm << " cm of "
         << acrylic->GetName() << G4endl;

  // Reflectors

  G4ThreeVector* positionReflector = nullptr;
  positionReflector = new G4ThreeVector[fNbOfReflectors];

  G4double rowHalf = floor(rowCenter);
  G4double colHalf = floor(colCenter);

  G4cout << "Col Half is: " << colHalf << G4endl;
  G4cout << "Row Half is: " << rowHalf << G4endl;
  G4int i = 0;

  while(i < fNbOfReflectors){
    if(i < (fColNum*(fRowNum + 1))) {
      for(G4int county = (-1*(rowHalf+1)); county <= (rowHalf+1); county++){
        for(G4int countx = (-1*colHalf); countx <= colHalf; countx++){
          if(county == 0){
            continue;
          }
          else{
            if(county < 0){
              positionReflector[i] = G4ThreeVector( (countx * reflectorWidth), (county * reflectorWidth + reflectorWidth*0.5), 0);
            }
            else{
              positionReflector[i] = G4ThreeVector( (countx * reflectorWidth), (county * reflectorWidth - reflectorWidth*0.5), 0);
            }
          }
          i++;
        }
      }
    }
    else{
      for(G4int countx = (-1*(colHalf+1)); countx <= (colHalf+1); countx++){
        for(G4int county = (-1*rowHalf); county <= rowHalf; county++){
          if(countx == 0){
            continue;
          }
          else{
            if(countx < 0){
              positionReflector[i] = G4ThreeVector( (countx * reflectorWidth + reflectorWidth*0.5), (county * reflectorWidth), 0);
            }
            else{
              positionReflector[i] = G4ThreeVector( (countx * reflectorWidth - reflectorWidth*0.5), (county * reflectorWidth), 0);
            }
          }
          i++;
        }
      }
    }
  }

  // Two different solids for reflectors but could have just rotated them

  G4Box* uprightReflector
    = new G4Box("uprightReflector",
                reflectorThickness*0.5, reflectorHeight*0.5, reflectorLength*0.5);

  G4Box* flatReflector
    = new G4Box("flatReflector",
                reflectorWidth*0.5, reflectorThickness*0.5, reflectorLength*0.5);

  fLogicReflector[0]
    = new G4LogicalVolume(flatReflector, refMat, "flatReflect");
  fLogicReflector[1]
    = new G4LogicalVolume(uprightReflector, refMat, "upReflect");

  for (i = 0; i < fNbOfReflectors; i++){
    if(i < (fColNum*(fRowNum + 1))){
      new G4PVPlacement(0,
                        positionReflector[i],
                        fLogicReflector[0],
                        "Reflector",
                        detectLV,
                        false,
                        i,
                        fCheckOverlaps);
                      }
    else{
      new G4PVPlacement(0,
                        positionReflector[i],
                        fLogicReflector[1],
                        "Reflector",
                        detectLV,
                        false,
                        i,
                        fCheckOverlaps);
                      }
  }

  // Scintillator

  G4Box* scintillS
    = new G4Box("Scintill",                                   //its name
                scintWidth*0.5, scintHeight*0.5, scintLength*0.5);  //its size
  G4LogicalVolume* scintLV
    = new G4LogicalVolume(
                 scintillS,       //its solid
                 scintMat,        //its material
                 "Scintill");     //its name

  G4int fNbOfScints = fRowNum*fColNum;
  G4ThreeVector* positionScint = nullptr;
  positionScint = new G4ThreeVector[fNbOfScints];

  for(i = 0; i < fNbOfScints; i++){
    positionScint[i] = positionReflector[i] + G4ThreeVector(0, (0.5*reflectorWidth), 0);
  }

  for(i = 0; i < fNbOfScints; i++){
    new G4PVPlacement(0,
                      positionScint[i],
                      scintLV,
                      "Scintillator",
                      detectLV,
                      false,
                      i,
                      fCheckOverlaps);
  }

  // Corner Tabs

  std::vector<G4TwoVector> poligon(3);
  poligon[0] = G4TwoVector(0, 7.747*mm);
  poligon[1] = G4TwoVector(8.814*mm, 0);
  poligon[2] = G4TwoVector(0, 0);

  G4TwoVector offA(0,0), offB(0,0);
  G4double scaleA = 1, scaleB = 1;

  G4double dz1 = tabLength * 0.5;

  G4ExtrudedSolid* cornerTabS = new G4ExtrudedSolid("Tab", poligon, dz1, offA, scaleA, offB, scaleB);
  G4LogicalVolume* tabLV = new G4LogicalVolume(cornerTabS, tabMat, "Tab");

  G4int fNbOfTabs = 60;
  G4ThreeVector* positionTabs = nullptr;
  positionTabs = new G4ThreeVector[fNbOfTabs];

  for(i = 0; i < fNbOfTabs; i++){

    G4int rotater = i % 4;
    G4int zloc = floor(i/4);

    if(rotater == 0){
      positionTabs[i] = G4ThreeVector( -(reflectorHeight*0.5), -(reflectorHeight*0.5), (-1*chamberLength*0.5 + ((zloc + 1) * tabSpacing) + (zloc * tabLength) + tabLength*0.5));
    }
    else if(rotater == 1){
      positionTabs[i] = G4ThreeVector( -(reflectorHeight*0.5), (reflectorHeight*0.5), (-1*chamberLength*0.5 + ((zloc + 1) * tabSpacing) + (zloc * tabLength) + tabLength*0.5));
    }
    else if(rotater == 2){
      positionTabs[i] = G4ThreeVector( (reflectorHeight*0.5), (reflectorHeight*0.5), (-1*chamberLength*0.5 + ((zloc + 1) * tabSpacing) + (zloc * tabLength) + tabLength*0.5));
    }
    else if(rotater == 3){
      positionTabs[i] = G4ThreeVector( (reflectorHeight*0.5), -(reflectorHeight*0.5), (-1*chamberLength*0.5 + ((zloc + 1) * tabSpacing) + (zloc * tabLength) + tabLength*0.5));
    }
  }

  G4RotationMatrix* rotationTab1 = new G4RotationMatrix();
  rotationTab1->rotateZ(90*deg);

  G4RotationMatrix* rotationTab2 = new G4RotationMatrix();
  rotationTab2->rotateZ(180*deg);

  G4RotationMatrix* rotationTab3 = new G4RotationMatrix();
  rotationTab3->rotateZ(270*deg);

  for(G4int copyNo = 0; copyNo < fNbOfTabs; copyNo++){

    G4int rotater = copyNo % 4;

    if(rotater == 0){
      new G4PVPlacement(0,
                        positionTabs[copyNo],
                        tabLV,
                        "Tab",
                        scintLV,
                        false,
                        copyNo,
                        fCheckOverlaps);
    }
    else if(rotater == 1){
      new G4PVPlacement(rotationTab1,
                        positionTabs[copyNo],
                        tabLV,
                        "Tab",
                        scintLV,
                        false,
                        copyNo,
                        fCheckOverlaps);
    }
    else if(rotater == 2){
      new G4PVPlacement(rotationTab2,
                        positionTabs[copyNo],
                        tabLV,
                        "Tab",
                        scintLV,
                        false,
                        copyNo,
                        fCheckOverlaps);
    }
    else if(rotater == 3){
      new G4PVPlacement(rotationTab3,
                        positionTabs[copyNo],
                        tabLV,
                        "Tab",
                        scintLV,
                        false,
                        copyNo,
                        fCheckOverlaps);
    }
  }

  // PMTs

  // PMT Housing
  G4Box* innerMount
    = new G4Box("innerMount",
                pmtMountWidth*0.5, pmtMountWidth*0.5, pmtMountLength*0.5);
  G4Box* outerMount
    = new G4Box("outerMount",
                pmtOuterMountWidth*0.5, pmtOuterMountWidth*0.5, pmtMountLength*0.5);

  G4SubtractionSolid* pmtMount = new G4SubtractionSolid("PmtMount", outerMount, innerMount);

  G4LogicalVolume* pmtHousing = new G4LogicalVolume(pmtMount, nylon, "PMThousing");

  // PMT Bulb

  G4Sphere* pmtSphere
    = new G4Sphere("PMTSphere", pmtInner, pmtRadius, 0.0*deg, 180.0*deg, 0.0*deg, 180.0*deg);

  G4Box* deletionBox = new G4Box("deletionBox", 10*cm, pmtDistance, 10*cm);

  G4SubtractionSolid* pmtSurface = new G4SubtractionSolid("PMTSurface", pmtSphere, deletionBox, 0, G4ThreeVector(0, 0, 0*cm));

  G4LogicalVolume* pmtBulb = new G4LogicalVolume(pmtSurface, glass, "PMTBulb");

  // Light Guides
  G4Cons* tryCone = new G4Cons("Cone", 0*cm, 1.414*pmtMountWidth*0.5, 0*cm, 5.6*cm, 1.5*cm, 0*deg, 360*deg);

  G4Box* tryBox = new G4Box("Box", pmtMountWidth*0.5 - 0.001*cm, pmtMountWidth*0.5 - 0.001*cm, 10*cm);

  G4IntersectionSolid* tryGuide = new G4IntersectionSolid("Guide", tryCone, tryBox, 0, G4ThreeVector(0, 0, 8.5*cm));

  G4Box* tryBox2 = new G4Box("Box2", pmtMountWidth*0.5, pmtMountWidth*0.5, 1.5*cm);
  G4SubtractionSolid* guideCone = new G4SubtractionSolid("Sigh", tryBox2, tryGuide, 0, G4ThreeVector(0, 0, 0*cm));
  G4LogicalVolume* lightG = new G4LogicalVolume(guideCone, refMat, "Sigh");

  G4ThreeVector* positionHousing = nullptr;
  positionHousing = new G4ThreeVector[fNbOfPMTs];

  for(i = 0; i < fNbOfScints; i++){
    positionHousing[i] = positionScint[i] + G4ThreeVector(xLeftMisregistration, yLeftMisregistration, -(chamberLength*0.5 + fWindowThickness + pmtMountLength*0.5));
    positionHousing[i+fNbOfScints] = positionScint[i] + G4ThreeVector(xRightMisregistration, yRightMisregistration, (chamberLength*0.5 + fWindowThickness + pmtMountLength*0.5));
  }

  G4ThreeVector* positionLightG = nullptr;
  positionLightG = new G4ThreeVector[fNbOfPMTs];

  G4ThreeVector* positionPMT = nullptr;
  positionPMT = new G4ThreeVector[fNbOfPMTs];

  for(i = 0; i < fNbOfPMTs; i++){

    if(i < fNbOfPMTs/2){
      positionPMT[i] = positionHousing[i] + G4ThreeVector(0, 0, (pmtMountLength*0.5 - pmtSpacing - pmtWindow - pmtDistance));
      positionLightG[i] = positionHousing[i] + G4ThreeVector(0, 0, (pmtMountLength*0.5 - 1.55*cm));
    }
    else{
      positionPMT[i] = positionHousing[i] - G4ThreeVector(0, 0, (pmtMountLength*0.5 - pmtSpacing - pmtWindow - pmtDistance));
      positionLightG[i] = positionHousing[i] - G4ThreeVector(0, 0, (pmtMountLength*0.5 - 1.55*cm));
    }
  }

  G4RotationMatrix* rotationLightG2 = new G4RotationMatrix();
  rotationLightG2->rotateZ(0*deg);

  G4RotationMatrix* rotationLightG1 = new G4RotationMatrix();
  rotationLightG1->rotateZ(0*deg);
  rotationLightG1->rotateX(180.0*deg);

  // PMTs on one side are just rotated 180 degrees from the other
  G4RotationMatrix* rotationPMT = new G4RotationMatrix();
  rotationPMT->rotateX(180.*deg);

  G4RotationMatrix* rotationPMT1 = new G4RotationMatrix();
  G4RotationMatrix* rotationPMT2 = new G4RotationMatrix();
  rotationPMT1->rotateX(270.*deg);
  rotationPMT2->rotateX(90.*deg);

  for(G4int copyNo = 0; copyNo < fNbOfPMTs; copyNo++){

    if(copyNo < fNbOfPMTs/2){
      new G4PVPlacement(0,
                        positionHousing[copyNo],
                        pmtHousing,
                        "pmtHouse",
                        detectLV,
                        false,
                        copyNo,
                        fCheckOverlaps);

      new G4PVPlacement(rotationPMT1,
                        positionPMT[copyNo],
                        pmtBulb,
                        "pmtCathode",
                        detectLV,
                        false,
                        copyNo,
                        fCheckOverlaps);

      new G4PVPlacement(rotationLightG1,
                        positionLightG[copyNo],
                        lightG,
                        "lightG",
                        detectLV,
                        false,
                        0,
                        fCheckOverlaps
                      );
    }
    else{
      new G4PVPlacement(rotationPMT,
                        positionHousing[copyNo],
                        pmtHousing,
                        "pmtHouse",
                        detectLV,
                        false,
                        copyNo,
                        fCheckOverlaps);

      new G4PVPlacement(rotationPMT2,
                        positionPMT[copyNo],
                        pmtBulb,
                        "pmtCathode",
                        detectLV,
                        false,
                        copyNo,
                        fCheckOverlaps);

      new G4PVPlacement(rotationLightG2,
                        positionLightG[copyNo],
                        lightG,
                        "lightG",
                        detectLV,
                        false,
                        0,
                        fCheckOverlaps
                      );
    }
  }


  // Optical Surfaces (Skin surfaces surround the volume in all directions)

  new G4LogicalSkinSurface("reflectorSkin", fLogicReflector[0] , surfOpt);
  new G4LogicalSkinSurface("reflectorSkin", fLogicReflector[1] , surfOpt);
  new G4LogicalSkinSurface("lightGuideSkin", lightG, surfLightG);
  new G4LogicalSkinSurface("tabMatSkin", tabLV, tabMatSurf);

  // Visualization attributes, can be tweaked as preferred

  G4VisAttributes* boxVisAtt1 = new G4VisAttributes(G4Colour(1.0, 1.0, 0.84, 1.0));
  G4VisAttributes* boxVisAtt2 = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
  G4VisAttributes* boxVisAtt3 = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.6));
  G4VisAttributes* planeVisAtt1 = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.3));
  G4VisAttributes* planeVisAtt2 = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.8));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
  G4VisAttributes* scintVisAtt = new G4VisAttributes(G4Colour(1, 0.1, 0.1, 0.6));

  worldLV->SetVisAttributes(boxVisAtt1);
  fLogicReflector[0]->SetVisAttributes(planeVisAtt2);
  fLogicReflector[1]->SetVisAttributes(planeVisAtt2);
  scintLV->SetVisAttributes(scintVisAtt);
  pmtHousing->SetVisAttributes(boxVisAtt2);
  tabLV->SetVisAttributes(boxVisAtt2);
  TankLV->SetVisAttributes(chamberVisAtt);
  detectLV->SetVisAttributes(boxVisAtt3);
  pmtBulb->SetVisAttributes(planeVisAtt1);
  lightG->SetVisAttributes(boxVisAtt1);

  // Example of User Limits
  //
  // Below is an example of how to set tracking constraints in a given
  // logical volume
  //
  // Sets a max step length in the tracker region, with G4StepLimiter

  G4double maxStep = 0.25*chamberWidth;
  fStepLimit = new G4UserLimits(maxStep);
  pmtHousing->SetUserLimits(fStepLimit);

  /// Set additional contraints on the track, with G4UserSpecialCuts
  ///
  /// G4double maxLength = 2*trackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
  /// trackerLV->SetUserLimits(new G4UserLimits(maxStep,
  ///                                           maxLength,
  ///                                           maxTime,
  ///                                           minEkin));

  // Always return the physical world

  return worldPV;
}

void PIIDetectorConstruction::ConstructSDandField()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PIIDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}

void PIIDetectorConstruction::SetRowNumb(G4int rowNumber)
{
  fRowNum = rowNumber;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PIIDetectorConstruction::SetColNumb(G4int colNumber)
{
  fColNum = colNumber;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PIIDetectorConstruction::SetWindowThickness(G4double windowThick)
{
  fWindowThickness = windowThick;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PIIDetectorConstruction::SetHousingThickness(G4double housingThick)
{
  fHousingThickness = housingThick;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
