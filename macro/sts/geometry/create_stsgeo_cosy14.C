/******************************************************************************
 ** Creation of STS geometry in ROOT format (TGeo).
 ** For the geometry of the beamtime COSY 2014
 ** (3 stations with one sensor each)
 **
 ** @file create_stsgeo_cosy14.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 5 October 2015
 ** @date 05.10.2015
 **
 *
 ** The geometry hierarachy is:
 **
 ** 1. Sensors  (see function CreateSensors)
 **    The sensors are the active volumes and the lowest geometry level.
 **    They are built as TGeoVolumes, shape box, material silicon.
 **    x size is determined by strip pitch 58 mu and 1024 strips 
 **    plus guard ring of 1.3 mm at each border -> 6.1992 cm.
 **    Sensor type 1 is half of that (3.0792 cm).
 **    y size is determined by strip length (2.2 / 4.2 / 6.3 cm) plus
 **    guard ring of 1.3 mm at top and bottom -> 2.46 / 4.46 / 6.46 cm.
 **    z size is a parameter, to be set by gkSensorThickness.
 **
 ** 2. Sectors  (see function CreateSectors)
 **    Sectors consist of several chained sensors. These are arranged
 **    vertically on top of each other with a gap to be set by
 **    gkChainGapY. Sectors are constructed as TGeoVolumeAssembly.
 **    The sectors are auxiliary volumes used for proper placement
 **    of the sensor(s) in the module. They do not show up in the
 **    final geometry.
 **
 ** 3. Modules (see function ConstructModule)
 **    A module is a readout unit, consisting of one sensor or
 **    a chain of sensors (see sector) and a cable.
 **    The cable extends from the top of the sector vertically to the
 **    top of the halfladder the module is placed in. The cable and module
 **    volume thus depend on the vertical position of the sector in 
 **    the halfladder. The cables consist of silicon with a thickness to be
 **    set by gkCableThickness.
 **    Modules are constructed as TGeoVolume, shape box, medium gStsMedium.
 **    The module construction can be switched off (gkConstructCables)
 **    to reproduce older geometries.
 **
 ** 4. Halfladders (see function ConstructHalfLadder)
 **    A halfladder is a vertical assembly of several modules. The modules
 **    are placed vertically such that their sectors overlap by 
 **    gkSectorOverlapY. They are displaced in z direction to allow for the 
 **    overlap in y by gkSectorGapZ.
 **    The horizontal placement of modules in the halfladder can be choosen
 **    to left aligned or right aligned, which only matters if sensors of
 **    different x size are involved.
 **    Halfladders are constructed as TGeoVolumeAssembly.
 **
 ** 5. Ladders (see function CreateLadders and ConstructLadder)
 **    A ladder is a vertical assembly of two halfladders, and is such the
 **    vertical building block of a station. The second (bottom) half ladder
 **    is rotated upside down. The vertical arrangement is such that the
 **    inner sectors of the two halfladders have the overlap gkSectorOverlapY
 **    (function CreateLadder) or that there is a vertical gap for the beam
 **    hole (function CreateLadderWithGap).
 **    Ladders are constructed as TGeoVolumeAssembly.
 **   
 ** 6. Stations (see function ConstructStation)
 **    A station represents one layer of the STS geometry: one measurement
 **    at (approximately) a given z position. It consist of several ladders
 **    arranged horizontally to cover the acceptance.
 **    The ladders are arranged such that there is a horizontal overlap
 **    between neighbouring ladders (gkLadderOverLapX) and a vertical gap
 **    to allow for this overlap (gkLadderGapZ). Each second ladder is
 **    rotated around its y axis to face away from or into the beam.
 **    Stations are constructed as TGeoVolumes, shape box minus tube (for
 **    the beam hole), material gStsMedium.
 **
 ** 7. STS
 **    The STS is a volume hosting the entire detectors system. It consists
 **    of several stations located at different z positions.
 **    The STS is constructed as TGeoVolume, shape box minus cone (for the
 **    beam pipe), material gStsMedium. The size of the box is computed to
 **    enclose all stations.
 *****************************************************************************/


// Remark: With the proper steering variables, this should exactly reproduce
// the geometry version v11b of A. Kotynia's described in the ASCII format.
// The only exception is a minimal difference in the z position of the 
// sectors/sensors. This is because of ladder types 2 and 4 containing the half
// sensors around the beam hole (stations 1,2 and 3). In v11b, the two ladders
// covering the beam hole cannot be transformed into each other by rotations,
// but only by a reflection. This means they are constructionally different.
// To avoid introducing another two ladder types, the difference in z position
// was accepted.





#include <iomanip>
#include <iostream>
#include "TGeoManager.h"


// -------------   Steering variables       -----------------------------------

// ---> Thickness of sensors [cm]
const Double_t gkSensorThickness = 0.03;

// ---> Vertical gap between chained sensors [cm]
const Double_t gkChainGapY       = 0.00;

// ---> Thickness of cables [cm]
const Double_t gkCableThickness  = 0.02;

// ---> Vertical overlap of neighbouring sectors in a ladder [cm]
const Double_t gkSectorOverlapY  = 0.27;

// ---> Gap in z between neighbouring sectors in a ladder [cm]
const Double_t gkSectorGapZ      = 0.02;

// ---> Horizontal overlap of neighbouring ladders [cm]
const Double_t gkLadderOverlapX  = 0.30;

// ---> Gap in z between neighbouring ladders [cm]
const Double_t gkLadderGapZ      = 1.00;  // 0.00;

// ---> Gap in z between lowest sector to carbon support structure [cm]
const Double_t gkSectorGapZFrame = 0.10;

// ---> Switch to construct / not to construct readout cables
const Bool_t   gkConstructCables = kTRUE;

// ---> Switch to construct / not to construct frames
const Bool_t   gkConstructCones       = kTRUE;  // kFALSE;  // switch this false for v15a
const Bool_t   gkConstructFrames      = kTRUE;  // kFALSE;  // switch this false for v15a
const Bool_t   gkConstructSmallFrames = kTRUE;  // kFALSE;
const Bool_t   gkCylindricalFrames    = kTRUE;  // kFALSE;

// ---> Size of the frame
const Double_t gkFrameThickness     = 0.2;
const Double_t gkThinFrameThickness = 0.05;
const Double_t gkFrameStep          = 4.0;  // size of frame cell along y direction

const Double_t gkCylinderDiaInner   = 0.07; // properties of cylindrical carbon supports, see CBM-STS Integration Meeting (10 Jul 2015)
const Double_t gkCylinderDiaOuter   = 0.15; // properties of cylindrical carbon supports, see CBM-STS Integration Meeting (10 Jul 2015)

// ----------------------------------------------------------------------------



// --------------   Parameters of beam pipe in the STS region    --------------
// ---> Needed to compute stations and STS such as to avoid overlaps
const Double_t gkPipeZ1 =  22.0;
const Double_t gkPipeR1 =   1.8;
const Double_t gkPipeZ2 =  50.0;
const Double_t gkPipeR2 =   1.8;
const Double_t gkPipeZ3 = 125.0;
const Double_t gkPipeR3 =   5.5;
// ----------------------------------------------------------------------------



// -------------   Other global variables   -----------------------------------
// ---> STS medium (for every volume except silicon)
TGeoMedium*    gStsMedium        = NULL;  // will be set later
// ---> TGeoManager (too lazy to write out 'Manager' all the time
TGeoManager*   gGeoMan           = NULL;  // will be set later
// ----------------------------------------------------------------------------




// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_stsgeo_cosy14(const char* geoTag="v15a")
{

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = "sts_cosy14_";
  geoFileName = geoFileName + geoTag + ".geo.root";
  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = geoFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "STS geometry created with create_stsgeo_cosy14.C" << endl << endl;
  infoFile << "Global variables: " << endl;
  infoFile << "Sensor thickness = " << gkSensorThickness << " cm" << endl;
  infoFile << "Vertical gap in sensor chain = " 
	   << gkChainGapY << " cm" << endl;
  infoFile << "Vertical overlap of sensors = " 
	   << gkSectorOverlapY << " cm" << endl;
  infoFile << "Gap in z between neighbour sensors = " 
	   << gkSectorGapZ << " cm" << endl;
  infoFile << "Horizontal overlap of sensors = " 
	   << gkLadderOverlapX << " cm" << endl;
  infoFile << "Gap in z between neighbour ladders = " 
	   << gkLadderGapZ << " cm" << endl;
  if ( gkConstructCables ) 
    infoFile << "Cable thickness = " << gkCableThickness << " cm" << endl;
  else
    infoFile << "No cables" << endl;
  infoFile << endl;
  infoFile << "Beam pipe: R1 = " << gkPipeR1 << " cm at z = " 
	   << gkPipeZ1 << " cm" << endl;
  infoFile << "Beam pipe: R2 = " << gkPipeR2 << " cm at z = " 
	   << gkPipeZ2 << " cm" << endl;
  infoFile << "Beam pipe: R3 = " << gkPipeR3 << " cm at z = " 
	   << gkPipeZ3 << " cm" << endl;
  // --------------------------------------------------------------------------


  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------


  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // ---> air
  FairGeoMedium* mAir      = geoMedia->getMedium("air");
  if ( ! mAir ) Fatal("Main", "FairMedium air not found");
  geoBuild->createMedium(mAir);
  TGeoMedium* air = gGeoMan->GetMedium("air");
  if ( ! air ) Fatal("Main", "Medium air not found");

  // ---> silicon
  FairGeoMedium* mSilicon  = geoMedia->getMedium("silicon");
  if ( ! mSilicon ) Fatal("Main", "FairMedium silicon not found");
  geoBuild->createMedium(mSilicon);
  TGeoMedium* silicon = gGeoMan->GetMedium("silicon");
  if ( ! silicon ) Fatal("Main", "Medium silicon not found");

  // ---> carbon
  FairGeoMedium* mCarbon  = geoMedia->getMedium("carbon");
  if ( ! mCarbon ) Fatal("Main", "FairMedium carbon not found");
  geoBuild->createMedium(mCarbon);
  TGeoMedium* carbon = gGeoMan->GetMedium("carbon");
  if ( ! carbon ) Fatal("Main", "Medium carbon not found");

  // ---> STScable
  FairGeoMedium* mSTScable  = geoMedia->getMedium("STScable");
  if ( ! mSTScable ) Fatal("Main", "FairMedium STScable not found");
  geoBuild->createMedium(mSTScable);
  TGeoMedium* STScable = gGeoMan->GetMedium("STScable");
  if ( ! STScable ) Fatal("Main", "Medium STScable not found");

  // ---
  gStsMedium = air;
  // --------------------------------------------------------------------------



  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("STSgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------




  // ---------------   Create sensors   ---------------------------------------
  cout << endl << endl;
  cout << "===> Creating sensors...." << endl << endl;
  infoFile << endl << "Sensors: " << endl;
  Int_t nSensors = CreateSensors();
  
  TGeoVolume* sensorBaby = gGeoMan->GetVolume("SensorBaby");
  sensorBaby->SetLineColor(kBlue);
  CheckVolume(sensorBaby);
  CheckVolume(sensorBaby, infoFile);
 
  TGeoVolume* sensorOut = gGeoMan->GetVolume("SensorOut");
  sensorOut->SetLineColor(kBlue);
  CheckVolume(sensorOut);
  CheckVolume(sensorOut, infoFile);
  // --------------------------------------------------------------------------




  // ----------------   Create sectors   --------------------------------------
  cout << endl << endl;
  cout << "===> Creating sectors...." << endl;
  infoFile << endl << "Sectors: " << endl;
  Int_t nSectors = CreateSectors();
  for (Int_t iSector = 1; iSector <= nSectors; iSector++) {
    cout << endl;
    TString name = Form("Sector%02d", iSector);
    TGeoVolume* sector = gGeoMan->GetVolume(name);
    CheckVolume(sector);
    CheckVolume(sector, infoFile);
  }
  // --------------------------------------------------------------------------
  
  
  
  // -----------------    Create modules   ------------------------------------
  cout << endl << endl;
  cout << "===> Creating modules...." << endl;
  
  TGeoVolume* sector01 = gGeoMan->GetVolume("Sector01");  
  TGeoVolume* module01 = ConstructModule("Module01", sector01, 0.);
  CheckVolume(module01);
  CheckVolume(module01, infoFile);
  
  TGeoVolume* sector02 = gGeoMan->GetVolume("Sector02");  
  TGeoVolume* module02 = ConstructModule("Module02", sector02, 0.);
  CheckVolume(module02);
  CheckVolume(module02, infoFile);
  // --------------------------------------------------------------------------
 
 
 
  // ------------------    Create half ladders   ------------------------------
  cout << endl << endl;
  cout << "===> Creating halfladders...." << endl;
  
  TGeoVolumeAssembly* halfLadder01 = new TGeoVolumeAssembly("HalfLadder01");
  halfLadder01->AddNode(module01, 1);
  halfLadder01->GetShape()->ComputeBBox();
  CheckVolume(halfLadder01);
  CheckVolume(halfLadder01, infoFile);
  
  TGeoVolumeAssembly* halfLadder02 = new TGeoVolumeAssembly("HalfLadder02");
  halfLadder02->AddNode(module02, 1);
  halfLadder02->GetShape()->ComputeBBox();
  CheckVolume(halfLadder02);
  CheckVolume(halfLadder02, infoFile);
  // --------------------------------------------------------------------------
  
  
  // ------------------    Create ladders   -----------------------------------
  cout << endl << endl;
  cout << "===> Creating ladders...." << endl;
  
  TGeoVolumeAssembly* ladder01 = new TGeoVolumeAssembly("Ladder01");
  ladder01->AddNode(halfLadder01, 1);
  ladder01->GetShape()->ComputeBBox();
  CheckVolume(ladder01);
  CheckVolume(ladder01, infoFile);
  
  TGeoVolumeAssembly* ladder02 = new TGeoVolumeAssembly("Ladder02");
  ladder02->AddNode(halfLadder02, 1);
  ladder02->GetShape()->ComputeBBox();
  CheckVolume(ladder02);
  CheckVolume(ladder02, infoFile);
  // --------------------------------------------------------------------------
 
 
 
  // ------------------    Create stations   ----------------------------------
  cout << endl << endl;
  cout << "===> Creating stations...." << endl;
  Double_t statPos[3];
  
  TGeoVolumeAssembly* station01 = new TGeoVolumeAssembly("Station01");
  station01->AddNode(ladder01, 1);
  station01->GetShape()->ComputeBBox();
  CheckVolume(station01);
  CheckVolume(station01, infoFile);
  statPos[0] = 25.3;
 
  TGeoVolumeAssembly* station02 = new TGeoVolumeAssembly("Station02");
  station02->AddNode(ladder02, 1);
  station02->GetShape()->ComputeBBox();
  CheckVolume(station02);
  CheckVolume(station02, infoFile);
  statPos[1] = 99.3;

  TGeoVolumeAssembly* station03 = new TGeoVolumeAssembly("Station03");
  station03->AddNode(ladder01, 1);
  station03->GetShape()->ComputeBBox();
  CheckVolume(station03);
  CheckVolume(station03, infoFile);
  statPos[2] = 140.7;
  // --------------------------------------------------------------------------



  // ---------------   Create STS volume   ------------------------------------
  cout << endl << endl;
  cout << "===> Creating STS...." << endl;

  // --- Determine size of STS box
  Double_t stsX = 0.;
  Double_t stsY = 0.;
  Double_t stsZ = 0.;
  Double_t stsBorder = 2*5.;  // 5 cm space for carbon ladders on each side
  for (Int_t iStation = 1; iStation<=3; iStation++) {
    TString statName = Form("Station%02d", iStation);
    TGeoVolume* station = gGeoMan->GetVolume(statName);
    TGeoBBox* shape = (TGeoBBox*) station->GetShape();
    stsX = TMath::Max(stsX, 2.* shape->GetDX() );
    stsY = TMath::Max(stsY, 2.* shape->GetDY() );
    cout << "Station " << iStation << ":  dX = " << stsX << ", dY " << stsY 
         << ", posZ = " << statPos[iStation-1] << endl;
    infoFile << "Station " << iStation << ":  dX = " << stsX << ", dY " << stsY 
             << ", posZ = " << statPos[iStation-1] << endl;
  }
  // --- Some border around the stations
  stsX += stsBorder;  
  stsY += stsBorder; 
  stsZ = ( statPos[2] - statPos[0] ) + stsBorder;
  Double_t stsPosZ = 0.5 * ( statPos[2] + statPos[0] );

  // --- Create box  around the stations
  TGeoShape* stsShape = new TGeoBBox("stsBox", stsX/2., stsY/2., stsZ/2.);
  cout << "size of STS box: x " <<  stsX << " - y " << stsY << " - z " << stsZ << endl;

  // --- Create STS volume
  TString stsName = "sts_cosy14_";  
  stsName += geoTag;
  TGeoVolume* sts = new TGeoVolume(stsName.Data(), stsShape, gStsMedium);

  // --- Place stations in the STS
  for (Int_t iStation = 1; iStation <=3; iStation++) {
    TString statName = Form("Station%02d", iStation);
    TGeoVolume* station = gGeoMan->GetVolume(statName);
    Double_t posZ = statPos[iStation-1] - stsPosZ;
    TGeoTranslation* trans = new TGeoTranslation(0., 0., posZ);
    sts->AddNode(station, iStation, trans);
    sts->GetShape()->ComputeBBox();
  }
  cout << endl;
  CheckVolume(sts);
  CheckVolume(sts, infoFile);
  // --------------------------------------------------------------------------




  // ---------------   Finish   -----------------------------------------------
  TGeoTranslation* stsTrans = new TGeoTranslation(0., 0., stsPosZ);
  top->AddNode(sts, 1, stsTrans);
  top->GetShape()->ComputeBBox();
  cout << endl << endl;
  CheckVolume(top);
  CheckVolume(top, infoFile);
  cout << endl << endl;
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  cout << endl;
  cout << "Geometry " << top->GetName() << " written to " 
       << geoFileName << endl;
  geoFile->Close();

  TString geoFileName__ = "sts_cosy14_";
  geoFileName_ = geoFileName__ + geoTag + "-geo.root";
  sts->Export(geoFileName_);

  geoFile = new TFile(geoFileName_, "UPDATE");
  stsTrans->Write();
  geoFile->Close();

  top->Draw("ogl");
  gGeoManager->SetVisLevel(6);
  
  infoFile.close();

}
// ============================================================================
// ======                   End of main function                          =====
// ============================================================================





// ****************************************************************************
// *****      Definition of media, sensors, sectors and ladders           *****
// *****                                                                  *****
// *****     Decoupled from main function for better readability          *****
// ****************************************************************************


/** ===========================================================================
 ** Create media
 **
 ** Currently created: air, active silicon, passive silion
 **
 ** Not used for the time being
 **/
Int_t CreateMedia() {

  Int_t nMedia     = 0;
  Double_t density = 0.;

  // --- Material air
  density = 1.205e-3.;  // [g/cm^3]
  TGeoMixture* matAir = new TGeoMixture("sts_air", 3, density);
  matAir->AddElement(14.0067, 7, 0.755);      // Nitrogen
  matAir->AddElement(15.999,  8, 0.231);      // Oxygen
  matAir->AddElement(39.948, 18, 0.014);      // Argon

  // --- Material silicon
  density    = 2.33;     // [g/cm^3]
  TGeoElement* elSi   = gGeoMan->GetElementTable()->GetElement(14);
  TGeoMaterial* matSi = new TGeoMaterial("matSi", elSi, density);


  // --- Air (passive)
  TGeoMedium* medAir = new TGeoMedium("air", nMedia++, matAir);
  medAir->SetParam(0, 0.);       // is passive
  medAir->SetParam(1, 1.);       // is in magnetic field
  medAir->SetParam(2, 20.);      // max. field [kG]
  medAir->SetParam(6, 0.001);    // boundary crossing precision [cm]
  
  
  // --- Active silicon for sensors
  TGeoMedium* medSiAct = new TGeoMedium("silicon", 
					nMedia++, matSi);
  medSiAct->SetParam(0, 1.);     // is active
  medSiAct->SetParam(1, 1.);     // is in magnetic field
  medSiAct->SetParam(2, 20.);    // max. field [kG]
  medSiAct->SetParam(6, 0.001);  // boundary crossing precisison [cm]

  // --- Passive silicon for cables
  TGeoMedium* medSiPas = new TGeoMedium("carbon", 
					nMedia++, matSi);
  medSiPas->SetParam(0, 0.);     // is passive
  medSiPas->SetParam(1, 1.);     // is in magnetic field
  medSiPas->SetParam(2, 20.);    // max. field [kG]
  medSiPas->SetParam(6, 0.001);  // boundary crossing precisison [cm]

  return nMedia;
}
/** ======================================================================= **/




/** ===========================================================================
 ** Create sensors
 **
 ** Sensors are created as volumes with box shape and active silicon as medium.
 ** Four kinds of sensors: 3.2x2.2, 6.2x2.2, 6.2x4.2, 6.2x6.2
 **/
Int_t CreateSensors() {

  Int_t nSensors = 0;

  Double_t xSize      = 0.;
  Double_t ySize      = 0.;
  Double_t zSize      = gkSensorThickness;
  TGeoMedium* silicon = gGeoMan->GetMedium("silicon");


  // --- Sensor Type Baby: baby sensor (1.62 cm x 1.62 cm)
  xSize = 1.62;
  ySize = 1.62;
  TGeoBBox* shape_sensorBaby = new TGeoBBox("sensorBaby", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("SensorBaby", shape_sensorBaby, silicon);
  nSensors++;
  
  // ---  Sensor type Out: Big sensor (6.2 cm x 6.2 cm)
  xSize = 6.2092;
  ySize = 6.2;
  TGeoBBox* shape_sensorOut = new TGeoBBox("sensorOut", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("SensorOut", shape_sensorOut, silicon);
  nSensors++;
  
  return nSensors;
}
/** ======================================================================= **/




/** ===========================================================================
 ** Create sectors
 **
 ** A sector is either a single sensor or several chained sensors.
 ** It is implemented as TGeoVolumeAssembly.
 ** Defined here:
 ** - single sensor of type baby
 ** - single sensor of type of type out
 **/
Int_t CreateSectors() {

  Int_t nSectors = 0;

  TGeoVolume* sensorBaby = gGeoMan->GetVolume("SensorBaby");
  TGeoVolume* sensorOut  = gGeoMan->GetVolume("SensorOut");

  // --- Sector type 1: single sensor of type baby
  TGeoVolumeAssembly* sector01 = new TGeoVolumeAssembly("Sector01");
  sector01->AddNode(sensorBaby, 1);
  sector01->GetShape()->ComputeBBox();
  nSectors++;

  // --- Sector type 2: single sensor of type Out
  TGeoVolumeAssembly* sector02 = new TGeoVolumeAssembly("Sector02");
  sector02->AddNode(sensorOut, 1);
  sector02->GetShape()->ComputeBBox();
  nSectors++;

  return nSectors;
}
/** ======================================================================= **/


  
 

// ****************************************************************************
// *****                                                                  *****
// *****    Generic functions  for the construction of STS elements       *****
// *****                                                                  *****
// *****  module:     volume (made of a sector and a cable)               *****
// *****  haf ladder: assembly (made of modules)                          *****
// *****  ladder:     assembly (made of two half ladders)                 *****
// *****  station:    volume (made of ladders)                            *****
// *****                                                                  *****
// ****************************************************************************


    
/** ===========================================================================
 ** Construct a module
 **
 ** A module is a sector plus the readout cable extending from the
 ** top of the sector. The cable is made from passive silicon.
 ** The cable has the same x size as the sector.
 ** Its thickness is given by the global variable gkCableThickness.
 ** The cable length is a parameter.
 ** The sensor(s) of the sector is/are placed directly in the module;
 ** the sector is just auxiliary for the proper placement.
 **
 ** Arguments: 
 **            name             volume name
 **            sector           pointer to sector volume
 **            cableLength      length of cable
 **/
TGeoVolume* ConstructModule(const char* name, 
			    TGeoVolume* sector,
			    Double_t cableLength) {

  // --- Check sector volume
  if ( ! sector ) Fatal("CreateModule", "Sector volume not found!");

  // --- Get size of sector
  TGeoBBox* box = (TGeoBBox*) sector->GetShape(); 
  Double_t sectorX = 2. * box->GetDX();
  Double_t sectorY = 2. * box->GetDY();
  Double_t sectorZ = 2. * box->GetDZ();

  // --- Get size of cable
  Double_t cableX   = sectorX;
  Double_t cableY   = cableLength;
  Double_t cableZ   = gkCableThickness;

  // --- Create module volume
  Double_t moduleX = TMath::Max(sectorX, cableX);
  Double_t moduleY = sectorY + cableLength;
  Double_t moduleZ = TMath::Max(sectorZ, cableZ);
  TGeoVolume* module = gGeoManager->MakeBox(name, gStsMedium,
					    moduleX/2.,
					    moduleY/2.,
					    moduleZ/2.);

  // --- Position of sector in module
  // --- Sector is centred in x and z and aligned to the bottom
  Double_t sectorXpos = 0.;
  Double_t sectorYpos = 0.5 * (sectorY - moduleY);
  Double_t sectorZpos = 0.;


  // --- Get sensor(s) from sector
  Int_t nSensors = sector->GetNdaughters();
  for (Int_t iSensor = 0; iSensor < nSensors; iSensor++) {
    TGeoNode* sensor = sector->GetNode(iSensor);

    // --- Calculate position of sensor in module
    Double_t* xSensTrans = sensor->GetMatrix()->GetTranslation();
    Double_t sensorXpos = 0.;
    Double_t sensorYpos = sectorYpos + xSensTrans[1];
    Double_t sensorZpos = 0.;
    TGeoTranslation* sensTrans = new TGeoTranslation("sensTrans",
						     sensorXpos,
						     sensorYpos,
						     sensorZpos);

    // --- Add sensor volume to module
    TGeoVolume* sensVol = sensor->GetVolume();
    module->AddNode(sensor->GetVolume(), iSensor+1, sensTrans);
    module->GetShape()->ComputeBBox();
  }


  // --- Create cable volume, if necessary, and place it in module
  // --- Cable is centred in x and z and aligned to the top
  if ( gkConstructCables && cableLength > 0.0001 ) {
    TString cableName = TString(name) + "_cable";
    TGeoMedium* cableMedium = gGeoMan->GetMedium("STScable");
    if ( ! cableMedium ) Fatal("CreateModule", "Medium STScable not found!");
    TGeoVolume* cable = gGeoManager->MakeBox(cableName.Data(),
					     cableMedium,
					     cableX / 2.,
					     cableY / 2.,
					     cableZ / 2.);
    // add color to cables
    cable->SetLineColor(kOrange);
    cable->SetTransparency(60);
    Double_t cableXpos = 0.;
    Double_t cableYpos = sectorY + 0.5 * cableY - 0.5 * moduleY;
    Double_t cableZpos = 0.;
    TGeoTranslation* cableTrans = new TGeoTranslation("cableTrans",
						      cableXpos,
						      cableYpos,
						      cableZpos);
    module->AddNode(cable, 1, cableTrans);
    module->GetShape()->ComputeBBox();
  }

  return module;
}
/** ======================================================================= **/
 



/** ===========================================================================
 ** Volume information for debugging
 **/
void CheckVolume(TGeoVolume* volume) {

  TGeoBBox* shape = (TGeoBBox*) volume->GetShape();
  cout << volume->GetName() << ": size " << fixed << setprecision(4)
       << setw(7) << 2. * shape->GetDX() << " x " << setw(7)
       << 2. * shape->GetDY() << " x " << setw(7)
       << 2. * shape->GetDZ();
  if ( volume->IsAssembly() ) cout << ", assembly";
  else {
    if ( volume->GetMedium() ) 
      cout << ", medium " << volume->GetMedium()->GetName();
    else cout << ", " << "\033[31m" << " no medium" << "\033[0m";
  }
  cout << endl;
  if ( volume->GetNdaughters() ) {
    cout << "Daughters: " << endl;
    for (Int_t iNode = 0; iNode < volume->GetNdaughters(); iNode++) {
      TGeoNode* node = volume->GetNode(iNode);
      TGeoBBox* shape = (TGeoBBox*) node->GetVolume()->GetShape();
      cout << setw(15) << node->GetName() << ", size " 
	   << fixed << setprecision(3)
	   << setw(6) << 2. * shape->GetDX() << " x " 
	   << setw(6) << 2. * shape->GetDY() << " x " 
	   << setw(6) << 2. * shape->GetDZ() << ", position ( ";
      TGeoMatrix* matrix = node->GetMatrix();
      Double_t* pos = matrix->GetTranslation();
      cout << setfill(' ');
      cout << fixed << setw(8) << pos[0] << ", " 
	   << setw(8) << pos[1] << ", "
	   << setw(8) << pos[2] << " )" << endl;
    }
  }

}
/** ======================================================================= **/

 
/** ===========================================================================
 ** Volume information for output to file
 **/
void CheckVolume(TGeoVolume* volume, fstream& file) {

  if ( ! file ) return;

  TGeoBBox* shape = (TGeoBBox*) volume->GetShape();
  file << volume->GetName() << ": size " << fixed << setprecision(4)
       << setw(7) << 2. * shape->GetDX() << " x " << setw(7)
       << 2. * shape->GetDY() << " x " << setw(7)
       << 2. * shape->GetDZ();
  if ( volume->IsAssembly() ) file << ", assembly";
  else {
    if ( volume->GetMedium() ) 
      file << ", medium " << volume->GetMedium()->GetName();
    else file << ", " << "\033[31m" << " no medium" << "\033[0m";
  }
  file << endl;
  if ( volume->GetNdaughters() ) {
    file << "Contains: ";
    for (Int_t iNode = 0; iNode < volume->GetNdaughters(); iNode++) 
      file << volume->GetNode(iNode)->GetVolume()->GetName() << " ";
    file << endl;
  }

}
/** ======================================================================= **/





