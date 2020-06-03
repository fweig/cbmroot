/******************************************************************************
 ** Creation of STS geometry in ROOT format (TGeo).
 **
 ** @file create_stsgeo_v18f.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 15 June 2012
 ** @date 09.05.2014
 ** @author Tomas Balog <T.Balog@gsi.de>
 **
 ** v18f: flip orientation of carbon ladders for primary beam left of mSTS, change z-positions to 30 and 45 cm
 ** v18e: 2 stations, derived from v15b, 1st 2x2, 2nd of 3x3 sensor array, sensor size 6x6 cm2 with carbon ladder supports
 ** v18d: 2 stations of 3x3 sensor array, sensor size 6x6 cm2 with carbon ladder supports
 ** v18c: (causes segfault due to divide) 2 stations of 3x3 sensor array, sensor size 6x6 cm2 with carbon ladder supports
 ** v18b: 2 stations of 4x4 sensor array, sensor size 6x6 cm2
 ** v18a: 2 stations of 3x3 sensor array, sensor size 6x6 cm2
 **
 ** v15b: introduce modified carbon ladders from v13z
 ** v15a: with flipped ladder orientation for stations 0,2,4,6 to match CAD design
 **
 ** TODO:
 **
 ** DONE:
 ** v15b - use carbon macaroni as ladder support
 ** v15b - introduce a small gap between lowest sensor and carbon ladder
 ** v15b - build small cones for the first 2 stations
 ** v15b - within a station the ladders of adjacent units should not touch eachother - set gkLadderGapZ to 10 mm
 ** v15b - for all ladders set an even number of ladder elements 
 ** v15b - z offset of cones to ladders should not be 0.3 by default, but 0.26
 ** v15b - within a station the ladders should be aligned in z, defined either by the unit or the ladder with most sensors
 ** v15b - get rid of cone overlap in stations 7 and 8 - done by adapting rHole size
 **
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


// Differences to v12:
// gkChainGap reduced from 1 mm to 0
// gkCableThickness increased from 100 mum to 200 mum (2 cables per module)
// gkSectorOverlapY reduced from 3 mm to 2.4 mm
// New sensor types 05 and 06
// New sector types 07 and 08
// Re-definiton of ladders (17 types instead of 8)
// Re-definiton of station from new ladders


#include <iomanip>
#include <iostream>
#include "TGeoManager.h"

#include "TGeoCompositeShape.h"
#include "TGeoPara.h"
#include "TGeoTube.h"
#include "TGeoCone.h"


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
const Bool_t   gkConstructCones       = kFALSE;  // kFALSE;  // switch this false for v15a
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

//DE const Double_t gkPipeZ1 =  27.0;
//DE const Double_t gkPipeR1 =   1.05;
//DE const Double_t gkPipeZ2 = 160.0;
//DE const Double_t gkPipeR2 =   3.25;
// ----------------------------------------------------------------------------



// -------------   Other global variables   -----------------------------------
// ---> STS medium (for every volume except silicon)
TGeoMedium*    gStsMedium        = NULL;  // will be set later
// ---> TGeoManager (too lazy to write out 'Manager' all the time
TGeoManager*   gGeoMan           = NULL;  // will be set later
// ----------------------------------------------------------------------------

Int_t CreateSensors();
Int_t CreateSectors();
Int_t CreateLadders();
void CheckVolume(TGeoVolume* volume);
void CheckVolume(TGeoVolume* volume, fstream& file);
TGeoVolume* ConstructFrameElement(const TString& name, TGeoVolume* frameBoxVol, Double_t x);
TGeoVolume* ConstructSmallCone(Double_t coneDz);
TGeoVolume* ConstructBigCone(Double_t coneDz);
TGeoVolume* ConstructHalfLadder(const TString& name,
				Int_t nSectors,
				Int_t* sectorTypes,
				char align);
TGeoVolume* ConstructLadder(Int_t LadderIndex,
			    TGeoVolume* halfLadderU,
			    TGeoVolume* halfLadderD,
			    Double_t shiftZ);
TGeoVolume* ConstructLadderWithGap(Int_t LadderIndex,
                                   TGeoVolume* halfLadderU,
				   TGeoVolume* halfLadderD,
				   Double_t gapY);
TGeoVolume* ConstructStation(Int_t iStation, 
                             Int_t nLadders,
			     Int_t* ladderTypes, 
                             Double_t rHole);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_stsgeo_v18f(const char* geoTag="v18f_mcbm")
{

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = "sts_";
  geoFileName = geoFileName + geoTag + ".geo.root";
  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = geoFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "STS geometry created with create_stsgeo_v18f.C" << endl << endl;
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




  // --------------   Create media   ------------------------------------------
  /*
  cout << endl;
  cout << "===> Creating media....";
  cout << CreateMedia();
  cout << " media created" << endl;
  TList* media = gGeoMan->GetListOfMedia();
  for (Int_t iMedium = 0; iMedium < media->GetSize(); iMedium++ ) {
    cout << "Medium " << iMedium << ": " 
	 << ((TGeoMedium*) media->At(iMedium))->GetName() << endl;
  }
  gStsMedium = gGeoMan->GetMedium("air");
  if ( ! gStsMedium ) Fatal("Main", "medium sts_air not found");
  */
  // --------------------------------------------------------------------------

  


  // ---------------   Create sensors   ---------------------------------------
  cout << endl << endl;
  cout << "===> Creating sensors...." << endl << endl;
  infoFile << endl << "Sensors: " << endl;
  Int_t nSensors = CreateSensors();
  for (Int_t iSensor = 1; iSensor <= nSensors; iSensor++) {
    TString name = Form("Sensor%02d",iSensor);
    TGeoVolume* sensor = gGeoMan->GetVolume(name);

    // add color to sensors
    if (iSensor == 1)
      sensor->SetLineColor(kYellow);
    if (iSensor == 2)
      sensor->SetLineColor(kRed);
    if (iSensor == 3)
      sensor->SetLineColor(kGreen);
    if (iSensor == 4)
      sensor->SetLineColor(kBlue);
    if (iSensor == 5)
      sensor->SetLineColor(kYellow);
    if (iSensor == 6)
      sensor->SetLineColor(kYellow);

    CheckVolume(sensor);
    CheckVolume(sensor, infoFile);
  }
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




  // ----------------   Create ladders   --------------------------------------
  TString name = "";
  cout << endl << endl;
  cout << "===> Creating ladders...." << endl;
  infoFile << endl << "Ladders:" << endl;
  Int_t nLadders = CreateLadders();
  for (Int_t iLadder = 1; iLadder <= nLadders; iLadder++) {
    cout << endl;
    name = Form("Ladder%02d", iLadder);
    TGeoVolume* ladder = gGeoMan->GetVolume(name);
    CheckVolume(ladder);
    CheckVolume(ladder, infoFile);
    CheckVolume(ladder->GetNode(0)->GetVolume(), infoFile);
  }
  // --------------------------------------------------------------------------


  // ----------------   Create cone   -----------------------------------------
  Double_t coneDz = 1.64;
  TGeoVolume* coneSmallVolum = ConstructSmallCone(coneDz);
  if (!coneSmallVolum) Fatal("ConstructSmallCone", "Volume Cone not found");
  TGeoVolume* coneBigVolum = ConstructBigCone(coneDz);
  if (!coneBigVolum) Fatal("ConstructBigCone", "Volume Cone not found");
  // --------------------------------------------------------------------------


  // ----------------   Create stations   -------------------------------------
  //  Float_t statPos[8] = {30., 40., 50., 60., 70., 80., 90., 100.};
  Float_t statPos[8] = {28., 42., 50., 60., 70., 80., 90., 100.};
  //  Float_t statPos[8] = {30., 45., 50., 60., 70., 80., 90., 100.};
  
  cout << endl << endl;
  cout << "===> Creating stations...." << endl;
  infoFile << endl << "Stations: ";
  nLadders = 0;
  Int_t ladderTypes[20];
  Double_t statZ = 0.;
  Double_t rHole = 0.;
  TGeoBBox*        statShape = NULL;
  TGeoTranslation* statTrans = NULL;


  // --- Station 01: 8 ladders, type 3 2 2 1 1 2 2 3
  cout << endl;
  statZ = 30.;
  rHole = 2.0;
  nLadders = 2;
  ladderTypes[0] =  9;
  ladderTypes[1] =  9;
//  ladderTypes[2] =  9;  // 12;  // 2;
//  ladderTypes[3] =  1;  // 11;  // 21;   // 1;
//  ladderTypes[4] =  1;  // 11;  // 21;   // 1;
//  ladderTypes[5] =  9;  // 12;  // 2;
//  ladderTypes[6] =  9;  // 12;  // 2;
//  ladderTypes[7] = 10;  // 13;  // 3;
  TGeoVolume* station01 = ConstructStation(0, nLadders, ladderTypes, rHole);

  if (gkConstructCones) {
    // upstream
    TGeoRotation* coneRot11 = new TGeoRotation;
    coneRot11->RotateZ(90);
    coneRot11->RotateY(180);
    //    TGeoCombiTrans* conePosRot11 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot11);
    TGeoCombiTrans* conePosRot11 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.305-gkLadderGapZ/2., coneRot11);
    station01->AddNode(coneSmallVolum, 1, conePosRot11);

    // downstream
    TGeoRotation* coneRot12 = new TGeoRotation;
    coneRot12->RotateZ(90);
    //    TGeoCombiTrans* conePosRot12 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot12);
    TGeoCombiTrans* conePosRot12 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.305+gkLadderGapZ/2., coneRot12);
    station01->AddNode(coneSmallVolum, 2, conePosRot12);

    station01->GetShape()->ComputeBBox();
  }
  
  CheckVolume(station01);
  CheckVolume(station01, infoFile);
  infoFile << "Position z = " << statPos[0] << endl;
  

  // --- Station 02: 12 ladders, type 4 3 3 2 2 1 1 2 2 3 3 4
  cout << endl;
  statZ = 40.;
  rHole = 2.0;
  nLadders = 3;
  ladderTypes[0]  = 10;
  ladderTypes[1]  = 10;
  ladderTypes[2]  = 10;
//  ladderTypes[3]  =  9;  // 12;  // 2;
//  ladderTypes[4]  =  9;  // 12;  // 2;
//  ladderTypes[5]  =  2;  // 21;  // 1;
//  ladderTypes[6]  =  2;  // 21;  // 1;
//  ladderTypes[7]  =  9;  // 12;  // 2;
//  ladderTypes[8]  =  9;  // 12;  // 2;
//  ladderTypes[9]  = 10;  // 13;  // 3;
//  ladderTypes[10] = 10;  // 13;  // 3;
//  ladderTypes[11] = 11;  // 22;  // 4;
  TGeoVolume* station02 = ConstructStation(1, nLadders, ladderTypes, rHole);

  if (gkConstructCones) {
    // upstream
    TGeoRotation* coneRot21 = new TGeoRotation;
    coneRot21->RotateZ(-90);
    coneRot21->RotateY(180);
    //    TGeoCombiTrans* conePosRot21 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot21);
    TGeoCombiTrans* conePosRot21 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.305-gkLadderGapZ/2., coneRot21);
    station02->AddNode(coneSmallVolum, 1, conePosRot21);

    // downstream
    TGeoRotation* coneRot22 = new TGeoRotation;
    coneRot22->RotateZ(-90);
    //    TGeoCombiTrans* conePosRot22 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot22);
    TGeoCombiTrans* conePosRot22 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.305+gkLadderGapZ/2., coneRot22);
    station02->AddNode(coneSmallVolum, 2, conePosRot22);

    station02->GetShape()->ComputeBBox();
  }
  
  CheckVolume(station02);
  CheckVolume(station02, infoFile);
  infoFile << "Position z = " << statPos[1] << endl;


//  // --- Station 03: 12 ladders, type 8 7 6 6 6 5 5 6 6 6 7 8
//  cout << endl;
//  statZ = 50.;
//  rHole = 2.9;
//  nLadders = 12;
//  ladderTypes[0]  = 14;  // 34;  // 8;
//  ladderTypes[1]  = 13;  // 33;  // 7;
//  ladderTypes[2]  = 12;  // 32;  // 6;
//  ladderTypes[3]  = 12;  // 32;  // 6;
//  ladderTypes[4]  = 12;  // 32;  // 6;
//  ladderTypes[5]  =  3;  // 31;  // 22;   // 5;
//  ladderTypes[6]  =  3;  // 31;  // 22;   // 5;
//  ladderTypes[7]  = 12;  // 32;  // 6;
//  ladderTypes[8]  = 12;  // 32;  // 6;
//  ladderTypes[9]  = 12;  // 32;  // 6;
//  ladderTypes[10] = 13;  // 33;  // 7;
//  ladderTypes[11] = 14;  // 34;  // 8;
//  TGeoVolume* station03 = ConstructStation(2, nLadders, ladderTypes, rHole);
//
//  if (gkConstructCones) {
//    // upstream
//    TGeoRotation* coneRot31 = new TGeoRotation;
//    coneRot31->RotateZ(90);
//    coneRot31->RotateY(180);
//    //    TGeoCombiTrans* conePosRot31 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot31);
//    TGeoCombiTrans* conePosRot31 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.285-gkLadderGapZ/2., coneRot31);
//    station03->AddNode(coneBigVolum, 1, conePosRot31);
//
//    // downstream
//    TGeoRotation* coneRot32 = new TGeoRotation;
//    coneRot32->RotateZ(90);
//    //    TGeoCombiTrans* conePosRot32 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot32);
//    TGeoCombiTrans* conePosRot32 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.285+gkLadderGapZ/2., coneRot32);
//    station03->AddNode(coneBigVolum, 2, conePosRot32);
//
//    station03->GetShape()->ComputeBBox();
//  }
//  
//  CheckVolume(station03);
//  CheckVolume(station03, infoFile);
//  infoFile << "Position z = " << statPos[2] << endl;
//
//
//  // --- Station 04: 14 ladders, type 9 8 7 6 6 6 5 5 6 6 7 8 9
//  cout << endl;
//  statZ = 60.;
//  rHole = 2.9;
//  nLadders = 14;
//  ladderTypes[0]  = 15;  // 42;  // 9;
//  ladderTypes[1]  = 14;  // 34;  // 8;
//  ladderTypes[2]  = 13;  // 33;  // 7;
//  ladderTypes[3]  = 12;  // 32;  // 6;
//  ladderTypes[4]  = 12;  // 32;  // 6;
//  ladderTypes[5]  = 12;  // 32;  // 6;
//  ladderTypes[6]  =  4;  // 41;  // 5;
//  ladderTypes[7]  =  4;  // 41;  // 5;
//  ladderTypes[8]  = 12;  // 32;  // 6;
//  ladderTypes[9]  = 12;  // 32;  // 6;
//  ladderTypes[10] = 12;  // 32;  // 6;
//  ladderTypes[11] = 13;  // 33;  // 7;
//  ladderTypes[12] = 14;  // 34;  // 8;
//  ladderTypes[13] = 15;  // 42;  // 9;
//  TGeoVolume* station04 = ConstructStation(3, nLadders, ladderTypes, rHole);
//
//  if (gkConstructCones) {
//    // upstream
//    TGeoRotation* coneRot41 = new TGeoRotation;
//    coneRot41->RotateZ(-90);
//    coneRot41->RotateY(180);
//    //    TGeoCombiTrans* conePosRot41 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot41);
//    TGeoCombiTrans* conePosRot41 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.285-gkLadderGapZ/2., coneRot41);
//    station04->AddNode(coneBigVolum, 1, conePosRot41);
//
//    // downstream
//    TGeoRotation* coneRot42 = new TGeoRotation;
//    coneRot42->RotateZ(-90);
//    //    TGeoCombiTrans* conePosRot42 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot42);
//    TGeoCombiTrans* conePosRot42 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.285+gkLadderGapZ/2., coneRot42);
//    station04->AddNode(coneBigVolum, 2, conePosRot42);
//
//    station04->GetShape()->ComputeBBox();
//  }
//  
//  CheckVolume(station04);
//  CheckVolume(station04, infoFile);
//  infoFile << "Position z = " << statPos[3] << endl;
//
//
//  // --- Station 05: 14 ladders, type 14 13 12 12 11 11 10 10 11 11 12 12 13 14
//  cout << endl;
//  statZ = 70.;
//  rHole = 3.7;
//  nLadders = 14;
//  ladderTypes[0]  = 19;  //  55;  // 14;
//  ladderTypes[1]  = 18;  //  54;  // 13;
//  ladderTypes[2]  = 17;  //  53;  // 12;
//  ladderTypes[3]  = 17;  //  53;  // 12;
//  ladderTypes[4]  = 16;  //  52;  // 11;
//  ladderTypes[5]  = 16;  //  52;  // 11;
//  ladderTypes[6]  =  5;  //  51;  // 23;   // 10;
//  ladderTypes[7]  =  5;  //  51;  // 23;   // 10;
//  ladderTypes[8]  = 16;  //  52;  // 11;
//  ladderTypes[9]  = 16;  //  52;  // 11;
//  ladderTypes[10] = 17;  //  53;  // 12;
//  ladderTypes[11] = 17;  //  53;  // 12;
//  ladderTypes[12] = 18;  //  54;  // 13;
//  ladderTypes[13] = 19;  //  55;  // 14;
//  TGeoVolume* station05 = ConstructStation(4, nLadders, ladderTypes, rHole);
//
//  if (gkConstructCones) {
//    // upstream
//    TGeoRotation* coneRot51 = new TGeoRotation;
//    coneRot51->RotateZ(90);
//    coneRot51->RotateY(180);
//    //    TGeoCombiTrans* conePosRot51 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot51);
//    TGeoCombiTrans* conePosRot51 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.285-gkLadderGapZ/2., coneRot51);
//    station05->AddNode(coneBigVolum, 1, conePosRot51);
//
//    // downstream
//    TGeoRotation* coneRot52 = new TGeoRotation;
//    coneRot52->RotateZ(90);
//    //    TGeoCombiTrans* conePosRot52 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot52);
//    TGeoCombiTrans* conePosRot52 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.285+gkLadderGapZ/2., coneRot52);
//    station05->AddNode(coneBigVolum, 2, conePosRot52);
//
//    station05->GetShape()->ComputeBBox();
//  }
//  
//  CheckVolume(station05);
//  CheckVolume(station05, infoFile);
//  infoFile << "Position z = " << statPos[4] << endl;
//
//
//  // --- Station 06: 14 ladders, type 14 13 12 12 11 11 10 10 11 11 12 12 13 14
//  cout << endl;
//  statZ = 80.;
//  rHole = 3.7;
//  nLadders = 14;
//  ladderTypes[0]  = 19;  // 55;  // 14;
//  ladderTypes[1]  = 18;  // 54;  // 13;
//  ladderTypes[2]  = 17;  // 53;  // 12;
//  ladderTypes[3]  = 17;  // 53;  // 12;
//  ladderTypes[4]  = 16;  // 52;  // 11;
//  ladderTypes[5]  = 16;  // 52;  // 11;
//  ladderTypes[6]  =  6;  // 61;  // 10;
//  ladderTypes[7]  =  6;  // 61;  // 10;
//  ladderTypes[8]  = 16;  // 52;  // 11;
//  ladderTypes[9]  = 16;  // 52;  // 11;
//  ladderTypes[10] = 17;  // 53;  // 12;
//  ladderTypes[11] = 17;  // 53;  // 12;
//  ladderTypes[12] = 18;  // 54;  // 13;
//  ladderTypes[13] = 19;  // 55;  // 14;
//  TGeoVolume* station06 = ConstructStation(5, nLadders, ladderTypes, rHole);
//
//  if (gkConstructCones) {
//    // upstream
//    TGeoRotation* coneRot61 = new TGeoRotation;
//    coneRot61->RotateZ(-90);
//    coneRot61->RotateY(180);
//    //    TGeoCombiTrans* conePosRot61 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot61);
//    TGeoCombiTrans* conePosRot61 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.285-gkLadderGapZ/2., coneRot61);
//    station06->AddNode(coneBigVolum, 1, conePosRot61);
//
//    // downstream
//    TGeoRotation* coneRot62 = new TGeoRotation;
//    coneRot62->RotateZ(-90);
//    //    TGeoCombiTrans* conePosRot62 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot62);
//    TGeoCombiTrans* conePosRot62 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.285+gkLadderGapZ/2., coneRot62);
//    station06->AddNode(coneBigVolum, 2, conePosRot62);
//
//    station06->GetShape()->ComputeBBox();
//  }
//  
//  CheckVolume(station06);
//  CheckVolume(station06, infoFile);
//  infoFile << "Position z = " << statPos[5] << endl;
//
//
//  // --- Station 07: 16 ladders, type 14 13 17 17 16 16 16 15 15 16 16 16 17 17 13 14
//  cout << endl;
//  statZ = 90.;
//  rHole = 4.2;
//  nLadders = 16;
//  ladderTypes[0]  = 21;  // 73;  // 17;
//  ladderTypes[1]  = 19;  // 55;  // 14;
//  ladderTypes[2]  = 18;  // 54;  // 13;
//  ladderTypes[3]  = 20;  // 72;  // 16;
//  ladderTypes[4]  = 20;  // 72;  // 16;
//  ladderTypes[5]  = 20;  // 72;  // 16;
//  ladderTypes[6]  = 20;  // 72;  // 16;
//  ladderTypes[7]  =  7;  // 71;  // 15;
//  ladderTypes[8]  =  7;  // 71;  // 15;
//  ladderTypes[9]  = 20;  // 72;  // 16;
//  ladderTypes[10] = 20;  // 72;  // 16;
//  ladderTypes[11] = 20;  // 72;  // 16;
//  ladderTypes[12] = 20;  // 72;  // 16;
//  ladderTypes[13] = 18;  // 54;  // 13;
//  ladderTypes[14] = 19;  // 55;  // 14;
//  ladderTypes[15] = 21;  // 73;  // 17;
//  TGeoVolume* station07 = ConstructStation(6, nLadders, ladderTypes, rHole);
//
//  if (gkConstructCones) {
//    // upstream
//    TGeoRotation* coneRot71 = new TGeoRotation;
//    coneRot71->RotateZ(90);
//    coneRot71->RotateY(180);
//    //    TGeoCombiTrans* conePosRot71 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot71);
//    TGeoCombiTrans* conePosRot71 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.285-gkLadderGapZ/2., coneRot71);
//    station07->AddNode(coneBigVolum, 1, conePosRot71);
//
//    // downstream
//    TGeoRotation* coneRot72 = new TGeoRotation;
//    coneRot72->RotateZ(90);
//    //    TGeoCombiTrans* conePosRot72 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot72);
//    TGeoCombiTrans* conePosRot72 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.285+gkLadderGapZ/2., coneRot72);
//    station07->AddNode(coneBigVolum, 2, conePosRot72);
//
//    station07->GetShape()->ComputeBBox();
//  }
//  
//  CheckVolume(station07);
//  CheckVolume(station07, infoFile);
//  infoFile << "Position z = " << statPos[6] << endl;
//
//
//  // --- Station 08: 16 ladders, type 14 13 17 17 16 16 16 15 15 16 16 16 17 17 13 14
//  cout << endl;
//  statZ = 100.;
//  rHole = 4.2;
//  nLadders = 16;
//  ladderTypes[0]  = 19;  // 55;  // 14;
//  ladderTypes[1]  = 17;  // 53;  // 12;
//  ladderTypes[2]  = 23;  // 83;  // 20;
//  ladderTypes[3]  = 22;  // 82;  // 19;
//  ladderTypes[4]  = 22;  // 82;  // 19;
//  ladderTypes[5]  = 22;  // 82;  // 19;
//  ladderTypes[6]  = 22;  // 82;  // 19;
//  ladderTypes[7]  =  8;  // 81;  // 18;
//  ladderTypes[8]  =  8;  // 81;  // 18;
//  ladderTypes[9]  = 22;  // 82;  // 19;
//  ladderTypes[10] = 22;  // 82;  // 19;
//  ladderTypes[11] = 22;  // 82;  // 19;
//  ladderTypes[12] = 22;  // 82;  // 19;
//  ladderTypes[13] = 23;  // 83;  // 20;
//  ladderTypes[14] = 17;  // 53;  // 12;
//  ladderTypes[15] = 19;  // 55;  // 14;
//  TGeoVolume* station08 = ConstructStation(7, nLadders, ladderTypes, rHole);
//
//  if (gkConstructCones) {
//    // upstream
//    TGeoRotation* coneRot81 = new TGeoRotation;
//    coneRot81->RotateZ(-90);
//    coneRot81->RotateY(180);
//    //    TGeoCombiTrans* conePosRot81 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.3-gkLadderGapZ/2., coneRot81);
//    TGeoCombiTrans* conePosRot81 = new TGeoCombiTrans(name+"conePosRot2", 0., 0., -coneDz-0.285-gkLadderGapZ/2., coneRot81);
//    station08->AddNode(coneBigVolum, 1, conePosRot81);
//
//    // downstream
//    TGeoRotation* coneRot82 = new TGeoRotation;
//    coneRot82->RotateZ(-90);
//    //    TGeoCombiTrans* conePosRot82 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.3+gkLadderGapZ/2., coneRot82);
//    TGeoCombiTrans* conePosRot82 = new TGeoCombiTrans(name+"conePosRot1", 0., 0., coneDz+0.285+gkLadderGapZ/2., coneRot82);
//    station08->AddNode(coneBigVolum, 2, conePosRot82);
//
//    station08->GetShape()->ComputeBBox();
//  }
//  
//  CheckVolume(station08);
//  CheckVolume(station08, infoFile);
//  infoFile << "Position z = " << statPos[7] << endl;
  // --------------------------------------------------------------------------




  // ---------------   Create STS volume   ------------------------------------
  cout << endl << endl;
  cout << "===> Creating STS...." << endl;

  TString stsName = "sts_";  
  stsName += geoTag;

  // --- Determine size of STS box
  Double_t stsX = 0.;
  Double_t stsY = 0.;
  Double_t stsZ = 0.;
  Double_t stsBorder = 2*5.;  // 5 cm space for carbon ladders on each side
  //  for (Int_t iStation = 1; iStation<=8; iStation++) {
  for (Int_t iStation = 1; iStation<=2; iStation++) {
    TString statName = Form("Station%02d", iStation);
    TGeoVolume* station = gGeoMan->GetVolume(statName);
    TGeoBBox* shape = (TGeoBBox*) station->GetShape();
    stsX = TMath::Max(stsX, 2.* shape->GetDX() );
    stsY = TMath::Max(stsY, 2.* shape->GetDY() );
    cout << "Station " << iStation << ":  Y " << stsY << endl;
  }
  // --- Some border around the stations
  stsX += stsBorder;  
  stsY += stsBorder; 
  stsZ = ( statPos[1] - statPos[0] ) + stsBorder;
  Double_t stsPosZ = 0.5 * ( statPos[1] + statPos[0] );

  // --- Create box  around the stations
  TGeoBBox* stsBox = new TGeoBBox("stsBox", stsX/2., stsY/2., stsZ/2.);
  cout << "size of STS box: x " <<  stsX << " - y " << stsY << " - z " << stsZ << endl;

//  // --- Create cone hosting the beam pipe
//  // --- One straight section with constant radius followed by a cone
//  Double_t z1 = statPos[0] - 0.5 * stsBorder;  // start of STS box
//  Double_t z2 = gkPipeZ2;
//  Double_t z3 = statPos[1] + 0.5 * stsBorder;  // end of STS box
//  Double_t r1 = BeamPipeRadius(z1);
//  Double_t r2 = BeamPipeRadius(z2);
//  Double_t r3 = BeamPipeRadius(z3);
//  r1 += 0.01;    // safety margin
//  r2 += 0.01;    // safety margin
//  r3 += 0.01;    // safety margin
//
//  cout << endl;
//  cout << z1 << "  " << r1 << endl;
//  cout << z2 << "  " << r2 << endl;
//  cout << z3 << "  " << r3 << endl;
//
//  cout << endl;
//  cout << "station1 :  " << BeamPipeRadius(statPos[0]) << endl;
//  cout << "station2 :  " << BeamPipeRadius(statPos[1]) << endl;
//  cout << "station3 :  " << BeamPipeRadius(statPos[2]) << endl;
//  cout << "station4 :  " << BeamPipeRadius(statPos[3]) << endl;
//  cout << "station5 :  " << BeamPipeRadius(statPos[4]) << endl;
//  cout << "station6 :  " << BeamPipeRadius(statPos[5]) << endl;
//  cout << "station7 :  " << BeamPipeRadius(statPos[6]) << endl;
//  cout << "station8 :  " << BeamPipeRadius(statPos[7]) << endl;
//
//  //  TGeoPcon* cutout = new TGeoPcon("stsCone", 0., 360., 3); // 2.*TMath::Pi(), 3);
//  //  cutout->DefineSection(0, z1, 0., r1);
//  //  cutout->DefineSection(1, z2, 0., r2);
//  //  cutout->DefineSection(2, z3, 0., r3);
//  new TGeoTrd2("stsCone1", r1, r2, r1, r2, (z2-z1)/2.+.1);  // add .1 in z length for a clean cutout
//  TGeoTranslation *trans1 = new TGeoTranslation("trans1", 0., 0., -(z3-z1)/2.+(z2-z1)/2.);
//  trans1->RegisterYourself();
//  new TGeoTrd2("stsCone2", r2, r3, r2, r3, (z3-z2)/2.+.1);  // add .1 in z length for a clean cutout
//  TGeoTranslation *trans2 = new TGeoTranslation("trans2", 0., 0., +(z3-z1)/2.-(z3-z2)/2.);
//  trans2->RegisterYourself();
  
//DE   Double_t z1 = statPos[0] - 0.5 * stsBorder;  // start of STS box
//DE   Double_t z2 = statPos[7] + 0.5 * stsBorder;  // end of STS box
//DE   Double_t slope = (gkPipeR2 - gkPipeR1) / (gkPipeZ2 - gkPipeZ1);
//DE   Double_t r1 = gkPipeR1 + slope * (z1 - gkPipeZ1); // at start of STS
//DE   Double_t r2 = gkPipeR1 + slope * (z2 - gkPipeZ1); // at end of STS
//DE   r1 += 0.1;    // safety margin
//DE   r2 += 0.1;    // safety margin
//DE   //  new TGeoCone("stsCone", stsZ/2., 0., r1, 0., r2);
//DE   new TGeoTrd2("stsCone", r1, r2, r1, r2, stsZ/2.);

  // --- Create STS volume
//  TGeoShape* stsShape = new TGeoCompositeShape("stsShape", 
//                                               "stsBox-stsCone1:trans1-stsCone2:trans2");
//  TGeoVolume* sts = new TGeoVolume(stsName.Data(), stsShape, gStsMedium);
//  TGeoVolume* sts = new TGeoVolume(stsName.Data(), stsBox, gStsMedium);
  TGeoVolumeAssembly* sts = new TGeoVolumeAssembly(stsName.Data());   // do not produce keeping volumes

  // --- Place stations in the STS
  //  for (Int_t iStation = 1; iStation <=8; iStation++) {
  for (Int_t iStation = 1; iStation <=2; iStation++) {
    TString statName = Form("Station%02d", iStation);
    TGeoVolume* station = gGeoMan->GetVolume(statName);
    Double_t posZ = statPos[iStation-1] - stsPosZ;
    //    TGeoTranslation* trans = new TGeoTranslation(0., 0., posZ);  // standard
    TGeoTranslation* trans = new TGeoTranslation(0., gkSectorOverlapY/2., posZ);  // mcbm
    sts->AddNode(station, iStation, trans);
    sts->GetShape()->ComputeBBox();
  }
  cout << endl;
  CheckVolume(sts);
  // --------------------------------------------------------------------------




  // ---------------   Finish   -----------------------------------------------
  TGeoTranslation* stsTrans = new TGeoTranslation(0., 0., stsPosZ);
  top->AddNode(sts, 1, stsTrans);
  top->GetShape()->ComputeBBox();
  cout << endl << endl;
  CheckVolume(top);
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

  TString geoFileName_ = "sts_";
  geoFileName_ = geoFileName_ + geoTag + "_geo.root";

  geoFile = new TFile(geoFileName_, "RECREATE");
  gGeoMan->Write();  // use this is you want GeoManager format in the output
  geoFile->Close();

  TString geoFileName__ = "sts_";
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
  density = 1.205e-3;  // [g/cm^3]
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


  // --- Sensor Type 01: Half small sensor (4 cm x 2.5 cm)
  xSize = 4.0;
  ySize = 2.5;
  TGeoBBox* shape_sensor01 = new TGeoBBox("sensor01", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("Sensor01", shape_sensor01, silicon);
  nSensors++;


  // --- Sensor type 02: Small sensor (6.2 cm x 2.2 cm)
  xSize = 6.2092;
  ySize = 2.2;
  TGeoBBox* shape_sensor02 = new TGeoBBox("sensor02", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("Sensor02", shape_sensor02, silicon);
  nSensors++;


  // --- Sensor type 03: Medium sensor (6.2 cm x 4.2 cm)
  xSize = 6.2092;
  ySize = 4.2;
  TGeoBBox* shape_sensor03 = new TGeoBBox("sensor03", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("Sensor03", shape_sensor03, silicon);
  nSensors++;


  // ---  Sensor type 04: Big sensor (6.2 cm x 6.2 cm)
  xSize = 6.2092;
  ySize = 6.2;
  TGeoBBox* shape_sensor04 = new TGeoBBox("sensor04", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("Sensor04", shape_sensor04, silicon);
  nSensors++;

  // ---  Sensor type 05: Additional "in hole" sensor (3.1 cm x 4.2 cm)
  xSize = 3.1;
  ySize = 4.2;
  TGeoBBox* shape_sensor05 = new TGeoBBox("sensor05", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("Sensor05", shape_sensor05, silicon);
  nSensors++;
  
  // ---  Sensor type 06: Mini Medium sensor (1.5 cm x 4.2 cm)
  xSize = 1.5;
  ySize = 4.2;
  TGeoBBox* shape_sensor06 = new TGeoBBox("sensor06", 
					  xSize/2., ySize/2., zSize/2.);
  new TGeoVolume("Sensor06", shape_sensor06, silicon);
  nSensors++;
  

  return nSensors;
}
/** ======================================================================= **/




/** ===========================================================================
 ** Create sectors
 **
 ** A sector is either a single sensor or several chained sensors.
 ** It is implemented as TGeoVolumeAssembly.
 ** Currently available:
 ** - single sensors of type 1 - 4
 ** - two chained sensors of type 4
 ** - three chained sensors of type 4
 **/
Int_t CreateSectors() {

  Int_t nSectors = 0;

  TGeoVolume* sensor01 = gGeoMan->GetVolume("Sensor01");
  TGeoVolume* sensor02 = gGeoMan->GetVolume("Sensor02");
  TGeoVolume* sensor03 = gGeoMan->GetVolume("Sensor03");
  TGeoVolume* sensor04 = gGeoMan->GetVolume("Sensor04");
  TGeoVolume* sensor05 = gGeoMan->GetVolume("Sensor05");
  TGeoVolume* sensor06 = gGeoMan->GetVolume("Sensor06");
  TGeoBBox*   box4     = (TGeoBBox*) sensor04->GetShape();

  // --- Sector type 1: single sensor of type 1
  TGeoVolumeAssembly* sector01 = new TGeoVolumeAssembly("Sector01");
  sector01->AddNode(sensor01, 1);
  sector01->GetShape()->ComputeBBox();
  nSectors++;

  // --- Sector type 2: single sensor of type 2
  TGeoVolumeAssembly* sector02 = new TGeoVolumeAssembly("Sector02");
  sector02->AddNode(sensor02, 1);
  sector02->GetShape()->ComputeBBox();
  nSectors++;

  // --- Sector type 3: single sensor of type 3
  TGeoVolumeAssembly* sector03 = new TGeoVolumeAssembly("Sector03");
  sector03->AddNode(sensor03, 1);
  sector03->GetShape()->ComputeBBox();
  nSectors++;

  // --- Sector type 4: single sensor of type 4
  TGeoVolumeAssembly* sector04 = new TGeoVolumeAssembly("Sector04");
  sector04->AddNode(sensor04, 1);
  sector04->GetShape()->ComputeBBox();
  nSectors++;

  // --- Sector type 5: two sensors of type 4
  TGeoVolumeAssembly* sector05 = new TGeoVolumeAssembly("Sector05");
  Double_t shift5 = 0.5 * gkChainGapY + box4->GetDY();
  TGeoTranslation* transD5 = 
    new TGeoTranslation("td", 0., -1. * shift5, 0.);
  TGeoTranslation* transU5 = 
    new TGeoTranslation("tu", 0., shift5, 0.);
  sector05->AddNode(sensor04, 1, transD5);
  sector05->AddNode(sensor04, 2, transU5);
  sector05->GetShape()->ComputeBBox();
  nSectors++;

  // --- Sector type 6: single sensor of type 5
  TGeoVolumeAssembly* sector06 = new TGeoVolumeAssembly("Sector06");
  sector06->AddNode(sensor05, 1);
  sector06->GetShape()->ComputeBBox();
  nSectors++;

  // --- Sector type 7: single sensor of type 6
  TGeoVolumeAssembly* sector07 = new TGeoVolumeAssembly("Sector07");
  sector07->AddNode(sensor06, 1);
  sector07->GetShape()->ComputeBBox();
  nSectors++;

  return nSectors;
}
/** ======================================================================= **/


  
    
/** ===========================================================================
 ** Create ladders
 **
 ** Ladders are the building blocks of the stations. They contain 
 ** several modules placed one after the other along the z axis
 ** such that the sectors are arranged vertically (with overlap).
 ** 
 ** A ladder is constructed out of two half ladders, the second of which
 ** is rotated in the x-y plane by 180 degrees and displaced
 ** in z direction.
 **/
Int_t CreateLadders() {

  Int_t nLadders = 0;

  // --- Some variables
  Int_t nSectors = 0;
  Int_t sectorTypes[10];
  TGeoBBox*   shape = NULL;
  TString     s0name;
  TGeoVolume* s0vol = NULL;
  TGeoVolume* halfLadderU = NULL;
  TGeoVolume* halfLadderD = NULL;
  Double_t shiftZ  = 0.;
  Double_t ladderY = 0.;
  Double_t gapY    = 0.;


  // --- Ladder 01 x-mirror of 02: 10 sectors, type 4 4 3 2 1 1 2 3 4 4
  nSectors       = 5;
  sectorTypes[0] = 1;
  sectorTypes[1] = 2;
  sectorTypes[2] = 3;
  sectorTypes[3] = 4;
  sectorTypes[4] = 4;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder01u", nSectors, sectorTypes, 'r');  // mirrored
  halfLadderD = ConstructHalfLadder("HalfLadder01d", nSectors, sectorTypes, 'l');  // mirrored
  ConstructLadder(1, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 02: 10 sectors, type 4 4 3 2 1 1 2 3 4 4
  nSectors       = 5;
  sectorTypes[0] = 1;
  sectorTypes[1] = 2;
  sectorTypes[2] = 3;
  sectorTypes[3] = 4;
  sectorTypes[4] = 4;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder02u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder02d", nSectors, sectorTypes, 'r');
  ConstructLadder(2, halfLadderU, halfLadderD, shiftZ);
  nLadders++;

  
  // --- Ladder 09: 2 sectors, type 4 4 - mCBM station 1
  nSectors       = 2;
  sectorTypes[0] = 4;
  sectorTypes[1] = 4;
//  sectorTypes[2] = 3;
//  sectorTypes[3] = 4;
//  sectorTypes[4] = 4;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder09u", nSectors, sectorTypes, 'l');
  //  halfLadderD = ConstructHalfLadder("HalfLadder09d", nSectors, sectorTypes, 'r');
  halfLadderD = ConstructHalfLadder("HalfLadder09d", 0, sectorTypes, 'r');
  ConstructLadder(9, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 10: 3 sectors, type 4 4 4 - mCBM station 2
  nSectors       = 3;
  sectorTypes[0] = 4;
  sectorTypes[1] = 4;
  sectorTypes[2] = 4;
//  sectorTypes[3] = 3;
//  sectorTypes[4] = 4;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder10u", nSectors, sectorTypes, 'l');
  //  halfLadderD = ConstructHalfLadder("HalfLadder10d", nSectors, sectorTypes, 'r');
  halfLadderD = ConstructHalfLadder("HalfLadder10d", 0, sectorTypes, 'r');
  ConstructLadder(10, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 11: 4 sectors, type 3 3 3 3
  nSectors       = 2;
  sectorTypes[0] = 3;
  sectorTypes[1] = 3;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder11u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder11d", nSectors, sectorTypes, 'r');
  ConstructLadder(11, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 03 x-mirror of 04: 10 sectors, type 5 4 3 3 6 6 3 3 4 5
  nSectors       = 5;
  sectorTypes[0] = 6;
  sectorTypes[1] = 3;
  sectorTypes[2] = 3;
  sectorTypes[3] = 4;
  sectorTypes[4] = 5;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder03u", nSectors, sectorTypes, 'r');  // mirrored
  halfLadderD = ConstructHalfLadder("HalfLadder03d", nSectors, sectorTypes, 'l');  // mirrored
  ConstructLadder(03, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 04: 10 sectors, type 5 4 3 3 6 6 3 3 4 5
  nSectors       = 5;
  sectorTypes[0] = 6;
  sectorTypes[1] = 3;
  sectorTypes[2] = 3;
  sectorTypes[3] = 4;
  sectorTypes[4] = 5;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder04u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder04d", nSectors, sectorTypes, 'r');
  ConstructLadder(4, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 12: 10 sectors, type 5 4 3 3 3 3 3 3 4 5
  nSectors       = 5;
  sectorTypes[0] = 3;
  sectorTypes[1] = 3;
  sectorTypes[2] = 3;
  sectorTypes[3] = 4;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder12u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder12d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(12, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 13: 8 sectors, type 5 4 3 3 3 3 4 5
  nSectors       = 4;
  sectorTypes[0] = 3;
  sectorTypes[1] = 3;
  sectorTypes[2] = 4;
  sectorTypes[3] = 5;
  s0name = Form("Sector%02d", sectorTypes[0]);
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  shiftZ = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU = ConstructHalfLadder("HalfLadder13u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder13d", nSectors, sectorTypes, 'r');
  ConstructLadder(13, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 14: 6 sensors, type 5 4 3 3 4 5
  nSectors       = 3;
  sectorTypes[0] = 3;
  sectorTypes[1] = 4;
  sectorTypes[2] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder14u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder14d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(14, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 15: 4 sectors, type 4 4 4 4
  nSectors       = 2;
  sectorTypes[0] = 4;
  sectorTypes[1] = 4;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder15u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder15d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(15, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 05 x-mirror of 06: 10 sectors, type 5 5 4 3 7 7 3 4 5 5
  nSectors       = 5;
  sectorTypes[0] = 7;
  sectorTypes[1] = 3;
  sectorTypes[2] = 4;
  sectorTypes[3] = 5;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder05u", nSectors, sectorTypes, 'r');  // mirrored
  halfLadderD = ConstructHalfLadder("HalfLadder05d", nSectors, sectorTypes, 'l');  // mirrored
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(5, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 06: 10 sectors, type 5 5 4 3 7 7 3 4 5 5 
  nSectors       = 5;
  sectorTypes[0] = 7;
  sectorTypes[1] = 3;
  sectorTypes[2] = 4;
  sectorTypes[3] = 5;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder06u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder06d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(6, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 16: 10 sectors, type 5 5 4 3 3 3 3 4 5 5
  nSectors       = 5;
  sectorTypes[0] = 3;
  sectorTypes[1] = 3;
  sectorTypes[2] = 4;
  sectorTypes[3] = 5;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder16u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder16d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(16, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 17: 8 sectors, type 5 5 4 3 3 4 5 5
  nSectors       = 4;
  sectorTypes[0] = 3;
  sectorTypes[1] = 4;
  sectorTypes[2] = 5;
  sectorTypes[3] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder17u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder17d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(17, halfLadderU, halfLadderD, shiftZ);
  nLadders++;
 

  // --- Ladder 18: 6 sectors, type 5 5 4 4 5 5
  nSectors       = 3;
  sectorTypes[0] = 4;
  sectorTypes[1] = 5;
  sectorTypes[2] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder18u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder18d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(18, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 19: 4 sectors, type 5 5 5 5
  nSectors       = 2;
  sectorTypes[0] = 5;
  sectorTypes[1] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder19u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder19d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(19, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 07: 10 sectors, type 5 5 4 3 3 gap 3 3 4 5 5, with gap
  nSectors       = 5;
  sectorTypes[0] = 3;
  sectorTypes[1] = 3;
  sectorTypes[2] = 4;
  sectorTypes[3] = 5;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder07u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder07d", nSectors, sectorTypes, 'r');
  shape =(TGeoBBox*) halfLadderU->GetShape();
  gapY = 4.4;
  ConstructLadderWithGap(7, halfLadderU, halfLadderD, 2*gapY);
  nLadders++;


  // --- Ladder 20: 10 sectors, type 5 5 5 3 2 2 3 5 5 5
  nSectors       = 5;
  sectorTypes[0] = 2;
  sectorTypes[1] = 3;
  sectorTypes[2] = 5;
  sectorTypes[3] = 5;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder20u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder20d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(20, halfLadderU, halfLadderD, shiftZ);
  nLadders++;
 

  // --- Ladder 21: 2 sectors, type 5 5
  nSectors       = 1;
  sectorTypes[0] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder21u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder21d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(21, halfLadderU, halfLadderD, shiftZ);
  nLadders++;


  // --- Ladder 08: 8 sectors, type 5 5 5 4 gap 4 5 5 5, with gap
  nSectors       = 4;
  sectorTypes[0] = 4;
  sectorTypes[1] = 5;
  sectorTypes[2] = 5;
  sectorTypes[3] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder08u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder08d", nSectors, sectorTypes, 'r');
  shape =(TGeoBBox*) halfLadderU->GetShape();
  gapY = 4.57;
  ConstructLadderWithGap(8, halfLadderU, halfLadderD, 2*gapY);
  nLadders++;


  // --- Ladder 22: 10 sectors, type 5 5 5 4 3 3 4 5 5 5 
  nSectors       = 5;
  sectorTypes[0] = 3;
  sectorTypes[1] = 4;
  sectorTypes[2] = 5;
  sectorTypes[3] = 5;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder22u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder22d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(22, halfLadderU, halfLadderD, shiftZ);
  nLadders++;
  

  // --- Ladder 23: 10 sectors, type 5 5 4 4 3 3 4 4 5 5 
  nSectors       = 5;
  sectorTypes[0] = 3;
  sectorTypes[1] = 4;
  sectorTypes[2] = 4;
  sectorTypes[3] = 5;
  sectorTypes[4] = 5;
  s0vol  = gGeoMan->GetVolume(s0name);
  shape  = (TGeoBBox*) s0vol->GetShape();
  ladderY = 2. * shape->GetDY();
  halfLadderU = ConstructHalfLadder("HalfLadder23u", nSectors, sectorTypes, 'l');
  halfLadderD = ConstructHalfLadder("HalfLadder23d", nSectors, sectorTypes, 'r');
  shape = (TGeoBBox*) halfLadderU->GetShape();
  ConstructLadder(23, halfLadderU, halfLadderD, shiftZ);
  nLadders++;

  return nLadders;
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
    const Double_t* xSensTrans = sensor->GetMatrix()->GetTranslation();
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
 ** Construct a half ladder
 **
 ** A half ladder is a virtual volume (TGeoVolumeAssembly) consisting
 ** of several modules arranged on top of each other. The modules
 ** have a given overlap in y and a displacement in z to allow for the
 ** overlap.
 **
 ** The typ of sectors / modules to be placed must be specified:
 **    1 = sensor01
 **    2 = sensor02
 **    3 = sensor03
 **    4 = sensor04
 **    5 = 2 x sensor04 (chained)
 **    6 = 3 x sensor04 (chained)
 ** The cable is added automatically from the top of each sensor to
 ** the top of the half ladder.
 ** The alignment can be left (l) or right (r), which matters in the
 ** case of different x sizes of sensors (e.g. SensorType01).
 **
 ** Arguments: 
 **            name             volume name
 **            nSectors         number of sectors
 **            sectorTypes      array with sector types
 **            align            horizontal alignment of sectors
 **/
TGeoVolume* ConstructHalfLadder(const TString& name,
				Int_t nSectors,
				Int_t* sectorTypes,
				char align) {

  // --- Create half ladder volume assembly
  TGeoVolumeAssembly* halfLadder = new TGeoVolumeAssembly(name);

  // --- Determine size of ladder
  Double_t ladderX = 0.;
  Double_t ladderY = 0.;
  Double_t ladderZ = 0.;
  for (Int_t iSector = 0; iSector < nSectors; iSector++) {
    TString sectorName = Form("Sector%02d", 
			      sectorTypes[iSector]);
    TGeoVolume* sector = gGeoMan->GetVolume(sectorName);
    if ( ! sector )
      Fatal("ConstructHalfLadder", (char *)Form("Volume %s not found", sectorName.Data()));
    TGeoBBox* box = (TGeoBBox*) sector->GetShape();
    // --- Ladder x size equals largest sector x size
    ladderX = TMath::Max(ladderX, 2. * box->GetDX());
    // --- Ladder y size is sum of sector ysizes
    ladderY += 2. * box->GetDY();
    // --- Ladder z size is sum of sector z sizes
    ladderZ += 2. * box->GetDZ();
  }
  // --- Subtract overlaps in y
  ladderY -= Double_t(nSectors-1) * gkSectorOverlapY;
  // --- Add gaps in z direction
  ladderZ += Double_t(nSectors-1) * gkSectorGapZ;


  // --- Create and place modules
  Double_t yPosSect = -0.5 * ladderY;
  Double_t zPosMod  = -0.5 * ladderZ;
  for (Int_t iSector = 0; iSector < nSectors; iSector++) {
    TString sectorName = Form("Sector%02d", 
			      sectorTypes[iSector]);
    TGeoVolume* sector = gGeoMan->GetVolume(sectorName);
    TGeoBBox* box = (TGeoBBox*) sector->GetShape();
    Double_t sectorX = 2. * box->GetDX();
    Double_t sectorY = 2. * box->GetDY();
    Double_t sectorZ = 2. * box->GetDZ();
    yPosSect += 0.5 * sectorY;   // Position of sector in ladder
    Double_t cableLength = 0.5 * ladderY - yPosSect - 0.5 * sectorY;
    TString moduleName = name + "_" + Form("Module%02d", 
					   sectorTypes[iSector]);
    TGeoVolume* module = ConstructModule(moduleName.Data(),
					 sector, cableLength);

    TGeoBBox* shapeMod = (TGeoBBox*) module->GetShape();
    Double_t moduleX = 2. * shapeMod->GetDX();
    Double_t moduleY = 2. * shapeMod->GetDY();
    Double_t moduleZ = 2. * shapeMod->GetDZ();
    Double_t xPosMod = 0.;
    if ( align == 'l' ) 
      xPosMod = 0.5 * (moduleX - ladderX);  // left aligned
    else if ( align == 'r' )
      xPosMod = 0.5 * (ladderX - moduleX);  // right aligned
    else 
      xPosMod = 0.;                         // centred in x
    Double_t yPosMod = 0.5 * (ladderY - moduleY);  // top aligned
    zPosMod += 0.5 * moduleZ;
    TGeoTranslation* trans = new TGeoTranslation("t", xPosMod, 
						 yPosMod, zPosMod);
    halfLadder->AddNode(module, iSector+1, trans);
    halfLadder->GetShape()->ComputeBBox();
    yPosSect += 0.5 * sectorY - gkSectorOverlapY;
    zPosMod  += 0.5 * moduleZ + gkSectorGapZ;
  }

  CheckVolume(halfLadder);
  cout << endl;

  return halfLadder;
}
/** ======================================================================= **/




/** ===========================================================================
 ** Add a carbon support to a ladder
 ** 
 ** Arguments: 
 **            LadderIndex      ladder number
 **            ladder           pointer to ladder
 **            xu               size of halfladder
 **            ladderY          height of ladder along y
 **            ladderZ          thickness of ladder along z
 **/
void AddCarbonLadder(Int_t LadderIndex,
                     TGeoVolume* ladder,
                     Double_t xu,
                     Double_t ladderY,
                     Double_t ladderZ) {

  // --- Some variables
  TString name = Form("Ladder%02d", LadderIndex);
  Int_t i;
  Double_t j;
    
  Int_t YnumOfFrameBoxes = (Int_t)(ladderY / gkFrameStep)+1;    // calculate number of elements
  if (LadderIndex == 1 || LadderIndex == 2)  // set even number of ladder elements for these ladders in station 1 and 2
	YnumOfFrameBoxes--;
//  if (LadderIndex == 3 || LadderIndex == 4)  // set even number of ladder elements for these ladders in station 3 and 4
//	YnumOfFrameBoxes++;
  YnumOfFrameBoxes += YnumOfFrameBoxes % 2;                     // use even number of frame elements for all ladders

  //      cout << "DE: lad " << LadderIndex << " inum " << YnumOfFrameBoxes << endl;

  // DEDE
  TGeoBBox* fullFrameShp = new TGeoBBox (name+"_FullFrameBox_shp", xu/2., gkFrameStep/2., (xu/2.+sqrt(2.)*gkFrameThickness/2.)/2.);
  //  TGeoBBox* fullFrameShp = new TGeoBBox (name+"_FullFrameBox_shp", xu/2., gkFrameStep/2., (gkSectorGapZFrame+xu/2.+sqrt(2.)*gkFrameThickness/2.)/2.);
  TGeoVolume* fullFrameBoxVol = new TGeoVolume(name+"_FullFrameBox", fullFrameShp, gStsMedium);

  //  cout << "DE: frame Z size " << (xu/2.+sqrt(2.)*gkFrameThickness/2.) << " cm" << endl;

  ConstructFrameElement("FrameBox", fullFrameBoxVol, xu/2.);
  TGeoRotation* fullFrameRot = new TGeoRotation;
  fullFrameRot->RotateY(180);

  Int_t inum = YnumOfFrameBoxes; // 6; // 9;
  for (i=1; i<=inum; i++)
  {
    j=-(inum-1)/2.+(i-1); 
	//        cout << "DE: i " << i << " j " << j << endl;

    if (LadderIndex <= 2)  // central ladders in stations 1 to 8
    {
      if ((j>=-1) && (j<=1))   // keep the inner 4 elements free for the cone
        continue;
    }
    else if (LadderIndex <= 8)  // central ladders in stations 1 to 8
    {
      if ((j>=-2) && (j<=2))   // keep the inner 4 elements free for the cone
        continue;
    }
    
    // DEDE
    ladder->AddNode(fullFrameBoxVol, i, new TGeoCombiTrans(name+"_FullFrameBox_posrot", 0., j*gkFrameStep, -ladderZ/2.-(xu/2.+sqrt(2.)*gkFrameThickness/2.)/2., fullFrameRot));
    //    ladder->AddNode(fullFrameBoxVol, i, new TGeoCombiTrans(name+"_FullFrameBox_posrot", 0., j*gkFrameStep, -ladderZ/2.-(gkSectorGapZFrame+xu/2.+sqrt(2.)*gkFrameThickness/2.)/2., fullFrameRot));
  }
  //      cout << endl;
  ladder->GetShape()->ComputeBBox();

}
/** ======================================================================= **/




/** ===========================================================================
 ** Construct a ladder out of two half ladders
 ** 
 ** The second half ladder will be rotated by 180 degrees 
 ** in the x-y plane. The two half ladders will be put on top of each
 ** other with a vertical overlap and displaced in z bz shiftZ.
 **
 ** Arguments: 
 **            name             volume name
 **            halfLadderU      pointer to upper half ladder
 **            halfLadderD      pointer to lower half ladder
 **            shiftZ           relative displacement along the z axis
 **/

TGeoVolume* ConstructLadder(Int_t LadderIndex,
			    TGeoVolume* halfLadderU,
			    TGeoVolume* halfLadderD,
			    Double_t shiftZ) {

  // --- Some variables
  TGeoBBox* shape = NULL;

  // --- Dimensions of half ladders
  shape = (TGeoBBox*) halfLadderU->GetShape();
  Double_t xu = 2. * shape->GetDX();
  Double_t yu = 2. * shape->GetDY();
  Double_t zu = 2. * shape->GetDZ();

  shape = (TGeoBBox*) halfLadderD->GetShape();
  Double_t xd = 2. * shape->GetDX();
  Double_t yd = 2. * shape->GetDY();
  Double_t zd = 2. * shape->GetDZ();

  // --- Create ladder volume assembly
  TString name = Form("Ladder%02d", LadderIndex);
  TGeoVolumeAssembly* ladder = new TGeoVolumeAssembly(name);
  Double_t ladderX = TMath::Max(xu, xd);
  Double_t ladderY = yu + yd - gkSectorOverlapY;
  Double_t ladderZ = TMath::Max(zu, zd + shiftZ);

  // --- Place half ladders
  Double_t xPosU = 0.;                      // centred in x
  Double_t yPosU = 0.5 * ( ladderY - yu );  // top aligned
  Double_t zPosU = 0.5 * ( ladderZ - zu );  // front aligned
  TGeoTranslation* tu = new TGeoTranslation("tu", xPosU, yPosU, zPosU);
  ladder->AddNode(halfLadderU, 1, tu);

  Double_t xPosD = 0.;                      // centred in x
  Double_t yPosD = 0.5 * ( yd - ladderY );  // bottom aligned
  Double_t zPosD = 0.5 * ( zd - ladderZ );  // back aligned
  TGeoRotation* rd = new TGeoRotation();
  rd->RotateZ(180.);
  TGeoCombiTrans* cd = new TGeoCombiTrans(xPosD, yPosD, zPosD, rd);
  ladder->AddNode(halfLadderD, 2, cd);
  ladder->GetShape()->ComputeBBox();

  // ----------------   Create and place frame boxes   ------------------------

  if (gkConstructFrames)
      AddCarbonLadder(LadderIndex, ladder, xu, ladderY, ladderZ);
  
  // --------------------------------------------------------------------------

  return ladder;
}
/** ======================================================================= **/




/** ===========================================================================
 ** Construct a ladder out of two half ladders with vertical gap
 ** 
 ** The second half ladder will be rotated by 180 degrees 
 ** in the x-y plane. The two half ladders will be put on top of each
 ** other with a vertical gap.
 **
 ** Arguments: 
 **            name             volume name
 **            halfLadderU      pointer to upper half ladder
 **            halfLadderD      pointer to lower half ladder
 **            gapY             vertical gap
 **/

TGeoVolume* ConstructLadderWithGap(Int_t LadderIndex,
                                   TGeoVolume* halfLadderU,
				   TGeoVolume* halfLadderD,
				   Double_t gapY) {

  // --- Some variables
  TGeoBBox* shape = NULL;
  Int_t i;
  Double_t j;

  // --- Dimensions of half ladders
  shape = (TGeoBBox*) halfLadderU->GetShape();
  Double_t xu = 2. * shape->GetDX();
  Double_t yu = 2. * shape->GetDY();
  Double_t zu = 2. * shape->GetDZ();

  shape = (TGeoBBox*) halfLadderD->GetShape();
  Double_t xd = 2. * shape->GetDX();
  Double_t yd = 2. * shape->GetDY();
  Double_t zd = 2. * shape->GetDZ();

  // --- Create ladder volume assembly
  TString name = Form("Ladder%02d", LadderIndex);
  TGeoVolumeAssembly* ladder = new TGeoVolumeAssembly(name);
  Double_t ladderX = TMath::Max(xu, xd);
  Double_t ladderY = yu + yd + gapY;
  Double_t ladderZ = TMath::Max(zu, zd);

  // --- Place half ladders
  Double_t xPosU = 0.;                      // centred in x
  Double_t yPosU = 0.5 * ( ladderY - yu );  // top aligned
  Double_t zPosU = 0.5 * ( ladderZ - zu );  // front aligned
  TGeoTranslation* tu = new TGeoTranslation("tu", xPosU, yPosU, zPosU);
  ladder->AddNode(halfLadderU, 1, tu);

  Double_t xPosD = 0.;                      // centred in x
  Double_t yPosD = 0.5 * ( yd - ladderY );  // bottom aligned
  Double_t zPosD = 0.5 * ( zd - ladderZ );  // back aligned
  TGeoRotation* rd = new TGeoRotation();
  rd->RotateZ(180.);
  TGeoCombiTrans* cd = new TGeoCombiTrans(xPosD, yPosD, zPosD, rd);
  ladder->AddNode(halfLadderD, 2, cd);
  ladder->GetShape()->ComputeBBox();

  // ----------------   Create and place frame boxes   ------------------------

  if (gkConstructFrames)
      AddCarbonLadder(LadderIndex, ladder, xu, ladderY, ladderZ);

  // --------------------------------------------------------------------------

  return ladder;
}
/** ======================================================================= **/




/** ===========================================================================
 ** Construct a station
 **
 ** The station volume is the minimal  box comprising all ladders
 ** minus a tube accomodating the beam pipe.
 **
 ** The ladders are arranged horizontally from left to right with
 ** a given overlap in x.
 ** Every second ladder is slightly displaced upstream from the centre
 ** z plane and facing downstream, the others are slightly displaced
 ** downstream and facing upstream (rotated around the y axis).
 **
 ** Arguments: 
 **            name             volume name
 **            nLadders         number of ladders
 **            ladderTypes      array of ladder types
 **            rHole            radius of inner hole
 **/

// TGeoVolume* ConstructStation(const char* name, 
//                              Int_t iStation, 

TGeoVolume* ConstructStation(Int_t iStation, 
                             Int_t nLadders,
			     Int_t* ladderTypes, 
                             Double_t rHole) {

  TString name;
  name = Form("Station%02d", iStation+1);  // 1,2,3,4,5,6,7,8
  //  name = Form("Station%02d", iStation);  // 0,1,2,3,4,5,6,7 - Station00 missing in output

  // --- Some local variables
  TGeoShape* statShape  = NULL;
  TGeoBBox* ladderShape = NULL;
  TGeoBBox* shape = NULL;
  TGeoVolume* ladder    = NULL;
  TString ladderName;


  // --- Determine size of station from ladders
  Double_t statX     = 0.;
  Double_t statY     = 0.;
  Double_t statZeven = 0.;
  Double_t statZodd  = 0.;
  Double_t statZ     = 0.;  
  for (Int_t iLadder = 0; iLadder < nLadders; iLadder++) {
    Int_t ladderType = ladderTypes[iLadder];
    ladderName = Form("Ladder%02d", ladderType);
    ladder = gGeoManager->GetVolume(ladderName);
    if ( ! ladder )
      Fatal("ConstructStation", Form("Volume %s not found", ladderName.Data()));
    shape = (TGeoBBox*) ladder->GetShape();
    statX += 2. * shape->GetDX();
    statY = TMath::Max(statY, 2. * shape->GetDY());
    if ( iLadder % 2 ) statZeven = TMath::Max(statZeven, 2. * shape->GetDZ() );
    else statZodd = TMath::Max(statZodd, 2. * shape->GetDZ() );
  }
  statX -= Double_t(nLadders-1) * gkLadderOverlapX;
  statZ = statZeven + gkLadderGapZ + statZodd;

  // --- Create station volume
  TString boxName(name);
  boxName += "_box";

  cout << "before   statZ/2.: " << statZ/2. << endl;
  statZ = 2 * 4.5;  // changed Z size of the station for cone and gkLadderGapZ
  cout << "fixed to statZ/2.: " << statZ/2. << endl;
  TGeoBBox* statBox = new TGeoBBox(boxName, statX/2., statY/2., statZ/2.);

//  TString tubName(name);
//  tubName += "_tub";
//  TString expression = boxName + "-" + tubName;
//  //  TGeoTube* statTub = new TGeoTube(tubName, 0., rHole, statZ/2.);
//  //  TGeoBBox* statTub = new TGeoBBox(tubName, rHole, rHole, statZ/2.);
//  TGeoBBox* statTub = new TGeoBBox(tubName, rHole, rHole, statZ/2.+.1);  // .1 opens the hole in z direction
//  
//  statShape = new TGeoCompositeShape(name, expression.Data());
//  TGeoVolume* station = new TGeoVolume(name, statShape, gStsMedium);
//  TGeoVolume* station = new TGeoVolume(name, statBox, gStsMedium);
  TGeoVolumeAssembly* station = new TGeoVolumeAssembly(name);   // do not produce keeping volumes

  Double_t subtractedVal;
  
  // --- Place ladders in station
  cout << "xPos0: " << statX << endl;
  Double_t xPos = -0.5 * statX;
  cout << "xPos1: " << xPos << endl;
  Double_t yPos = 0.;
  Double_t zPos = 0.;

  Double_t maxdz = 0.;
  for (Int_t iLadder = 0; iLadder < nLadders; iLadder++) {
    Int_t ladderType = ladderTypes[iLadder];
    ladderName = Form("Ladder%02d", ladderType);
    ladder = gGeoManager->GetVolume(ladderName);
    shape = (TGeoBBox*) ladder->GetShape();
    if (maxdz < shape->GetDZ())
      maxdz = shape->GetDZ();
  }
 
  for (Int_t iLadder = 0; iLadder < nLadders; iLadder++) {
    Int_t ladderType = ladderTypes[iLadder];
    ladderName = Form("Ladder%02d", ladderType);
    ladder = gGeoManager->GetVolume(ladderName);
    shape = (TGeoBBox*) ladder->GetShape();
    xPos += shape->GetDX();
    cout << "xPos2: " << xPos << endl;
    yPos = 0.;    // vertically centred  
    TGeoRotation* rot = new TGeoRotation();

    if (gkConstructFrames)
      // DEDE
      subtractedVal = sqrt(2.)*gkFrameThickness/2. + shape->GetDX();
    //      subtractedVal = 2*gkSectorGapZFrame + sqrt(2.)*gkFrameThickness/2. + shape->GetDX();
    else
      subtractedVal = 0.;

    //    zPos = 0.5 * gkLadderGapZ + (shape->GetDZ()-subtractedVal/2.);  // non z-aligned ladders
    zPos = 0.5 * gkLadderGapZ + (2*maxdz-shape->GetDZ()-subtractedVal/2.);  // z-aligned ladders
    
    cout << "DE ladder" << ladderTypes[iLadder]
	 << "  dx: " << shape->GetDX() 
	 << "  dy: " << shape->GetDY() 
	 << "  dz: " << shape->GetDZ() 
	 << "  max dz: " << maxdz << endl;

    cout << "DE ladder" << ladderTypes[iLadder]
	 << "  fra: " << gkFrameThickness/2.
	 << "  sub: " << subtractedVal
	 << "  zpo: " << zPos << endl << endl;

//    if (iStation % 2 == 0) // flip ladders for even stations to reproduce CAD layout
//    // even station 0,2,4,6
    if (iStation % 2 == 1) // flip ladders for odd stations to reproduce CAD layout
    // odd station 1,3,5,7
    {
      // --- Unrotated ladders --- downstream
      if ( (nLadders/2 + iLadder) % 2 ) {
	//        zPos = 0.5 * gkLadderGapZ + (shape->GetDZ()-subtractedVal/2.);
        rot->RotateY(180.);
      }
      // --- Rotated ladders --- upstream
      else {
	//        zPos = -0.5 * gkLadderGapZ - (shape->GetDZ()-subtractedVal/2.);
        zPos = -zPos;
      }
    }
    else
    // odd station 1,3,5,7
    {
      // --- Unrotated ladders --- upstream
      if ( (nLadders/2 + iLadder) % 2 ) {
	//        zPos = -0.5 * gkLadderGapZ - (shape->GetDZ()-subtractedVal/2.);
        zPos = -zPos;
      }
      // --- Rotated ladders --- downstream
      else {
	//        zPos = 0.5 * gkLadderGapZ + (shape->GetDZ()-subtractedVal/2.);
        rot->RotateY(180.);
      }
    }

    TGeoCombiTrans* trans = new TGeoCombiTrans(xPos, yPos, zPos, rot);
    station->AddNode(ladder, iLadder+1, trans);
    station->GetShape()->ComputeBBox();
    xPos += shape->GetDX() - gkLadderOverlapX;
    cout << "xPos3: " << xPos << endl;
  }

  return station;
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
      const Double_t* pos = matrix->GetTranslation();
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


/** ===========================================================================
 ** Calculate beam pipe outer radius for a given z
 **/
Double_t BeamPipeRadius(Double_t z) { 	
  if ( z < gkPipeZ2 ) return gkPipeR1;	
  Double_t slope = (gkPipeR3 - gkPipeR2 ) / (gkPipeZ3 - gkPipeZ2);
  return gkPipeR2 + slope * (z - gkPipeZ2);
}
/** ======================================================================= **/


   
/** ======================================================================= **/
TGeoVolume* ConstructFrameElement(const TString& name, TGeoVolume* frameBoxVol, Double_t x)
{
	// --- Material of the frames
	TGeoMedium* framesMaterial = gGeoMan->GetMedium("carbon");

	Double_t t = gkFrameThickness/2.;

	// --- Main vertical pillars
//    	TGeoBBox* frameVertPillarShp = new TGeoBBox(name + "_vertpillar_shape", t, gkFrameStep/2., t);  // square crossection, along y
//	TGeoVolume* frameVertPillarVol = new TGeoVolume(name + "_vertpillar", frameVertPillarShp, framesMaterial);
//	frameVertPillarVol->SetLineColor(kGreen);
//	frameBoxVol->AddNode(frameVertPillarVol, 1, new TGeoTranslation(name + "_vertpillar_pos_1", x-t, 0., -(x+sqrt(2.)*t-2.*t)/2.));
//	frameBoxVol->AddNode(frameVertPillarVol, 2, new TGeoTranslation(name + "_vertpillar_pos_2", -(x-t), 0., -(x+sqrt(2.)*t-2.*t)/2.));

        TGeoBBox* frameVertPillarShp;
        if (gkCylindricalFrames)
	  //          TGeoBBox* frameVertPillarShp = new TGeoTube(name + "_vertpillar_shape", 0, t, gkFrameStep/2.);  // circle crossection, along z
          frameVertPillarShp = new TGeoTube(name + "_vertpillar_shape", gkCylinderDiaInner/2., gkCylinderDiaOuter/2., gkFrameStep/2.);  // circle crossection, along z
        else
          frameVertPillarShp = new TGeoBBox(name + "_vertpillar_shape", t, t, gkFrameStep/2.);  // square crossection, along z
	TGeoVolume* frameVertPillarVol = new TGeoVolume(name + "_vertpillar", frameVertPillarShp, framesMaterial);
	frameVertPillarVol->SetLineColor(kGreen);

        TGeoRotation* xRot90 = new TGeoRotation;
        xRot90->RotateX(90.);
	frameBoxVol->AddNode(frameVertPillarVol, 1, new TGeoCombiTrans(name + "_vertpillar_pos_1", x-t,    0., -(x+sqrt(2.)*t-2.*t)/2., xRot90));
	frameBoxVol->AddNode(frameVertPillarVol, 2, new TGeoCombiTrans(name + "_vertpillar_pos_2", -(x-t), 0., -(x+sqrt(2.)*t-2.*t)/2., xRot90));

	//	TGeoRotation* vertRot = new TGeoRotation(name + "_vertpillar_rot_1", 90., 45., -90.);
	TGeoRotation* vertRot = new TGeoRotation;
        vertRot->RotateX(90.);
        vertRot->RotateY(45.);
	frameBoxVol->AddNode(frameVertPillarVol, 3, new TGeoCombiTrans(name + "_vertpillar_pos_3", 0., 0., (x-sqrt(2.)*t)/2., vertRot));

	// --- Small horizontal pillar
	TGeoBBox* frameHorPillarShp = new TGeoBBox(name + "_horpillar_shape", x-2.*t, gkThinFrameThickness/2., gkThinFrameThickness/2.);
	TGeoVolume* frameHorPillarVol = new TGeoVolume(name + "_horpillar", frameHorPillarShp, framesMaterial);
	frameHorPillarVol->SetLineColor(kCyan);
	frameBoxVol->AddNode(frameHorPillarVol, 1, new TGeoTranslation(name + "_horpillar_pos_1", 0., -gkFrameStep/2.+gkThinFrameThickness/2., -(x+sqrt(2.)*t-2.*t)/2.));

	if (gkConstructSmallFrames) {

		// --- Small sloping pillar
		TGeoPara* frameSlopePillarShp = new TGeoPara(name + "_slopepillar_shape",
			(x-2.*t)/TMath::Cos(31.4/180.*TMath::Pi()), gkThinFrameThickness/2., gkThinFrameThickness/2., 31.4, 0., 90.);
		TGeoVolume* frameSlopePillarVol = new TGeoVolume(name + "_slopepillar", frameSlopePillarShp, framesMaterial);
		frameSlopePillarVol->SetLineColor(kCyan);
		TGeoRotation* slopeRot = new TGeoRotation(name + "_slopepillar_rot_1", 0., 0., 31.4);
		TGeoCombiTrans* slopeTrRot = new TGeoCombiTrans(name + "_slopepillar_posrot_1", 0., 0., -(x+sqrt(2.)*t-2.*t)/2., slopeRot);

		frameBoxVol->AddNode(frameSlopePillarVol, 1, slopeTrRot);

		Double_t angl = 23.;
		// --- Small sub pillar
		TGeoPara* frameSubPillarShp = new TGeoPara(name + "_subpillar_shape",
			(sqrt(2)*(x/2.-t)-t/2.)/TMath::Cos(angl/180.*TMath::Pi()), gkThinFrameThickness/2., gkThinFrameThickness/2., angl, 0., 90.);
		TGeoVolume* frameSubPillarVol = new TGeoVolume(name + "_subpillar", frameSubPillarShp, framesMaterial);
		frameSubPillarVol->SetLineColor(kMagenta);

		Double_t posZ = t * (1. - 3. / ( 2.*sqrt(2.) ));

		// one side of X direction
		TGeoRotation* subRot1 = new TGeoRotation(name + "_subpillar_rot_1", 90., 45., -90.+angl);
		TGeoCombiTrans* subTrRot1 = new TGeoCombiTrans(name + "_subpillar_posrot_1", -(-x/2.+t-t/(2.*sqrt(2.))), 1., posZ, subRot1);

		TGeoRotation* subRot2 = new TGeoRotation(name + "_subpillar_rot_2", 90., -90.-45., -90.+angl);
		TGeoCombiTrans* subTrRot2 = new TGeoCombiTrans(name + "_subpillar_posrot_2", -(-x/2.+t-t/(2.*sqrt(2.))), -1., posZ, subRot2);

		// other side of X direction
		TGeoRotation* subRot3 = new TGeoRotation(name + "_subpillar_rot_3", 90., 90.+45., -90.+angl);
		TGeoCombiTrans* subTrRot3 = new TGeoCombiTrans(name + "_subpillar_posrot_3", -x/2.+t-t/(2.*sqrt(2.)), 1., posZ, subRot3);

		TGeoRotation* subRot4 = new TGeoRotation(name + "_subpillar_rot_4", 90., -45., -90.+angl);
		TGeoCombiTrans* subTrRot4 = new TGeoCombiTrans(name + "_subpillar_posrot_4", -x/2.+t-t/(2.*sqrt(2.)), -1., posZ, subRot4);

		frameBoxVol->AddNode(frameSubPillarVol, 1, subTrRot1);
		frameBoxVol->AddNode(frameSubPillarVol, 2, subTrRot2);
		frameBoxVol->AddNode(frameSubPillarVol, 3, subTrRot3);
		frameBoxVol->AddNode(frameSubPillarVol, 4, subTrRot4);
		//                frameBoxVol->GetShape()->ComputeBBox();
	}

	return frameBoxVol;
}
/** ======================================================================= **/

/** ======================================================================= **/
TGeoVolume* ConstructSmallCone(Double_t coneDz)
{
	// --- Material of the frames
	TGeoMedium* framesMaterial = gGeoMan->GetMedium("carbon");

	// --- Outer cone
//	TGeoConeSeg* A = new TGeoConeSeg ("A", coneDz, 6., 7.6, 6., 6.04, 0., 180.);
//	TGeoBBox* B = new TGeoBBox ("B", 8., 6., 10.);

	Double_t radius    = 3.0;
	Double_t thickness = 0.04; // 0.4 mm
//	TGeoConeSeg* A = new TGeoConeSeg ("A", coneDz, 3., 3.2, 3., 3.2, 0., 180.);
	TGeoConeSeg* A = new TGeoConeSeg ("A", coneDz, radius, radius+thickness, radius, radius+thickness, 0., 180.);
	TGeoBBox* B = new TGeoBBox ("B", 8., 6., 10.);

	TGeoCombiTrans* M = new TGeoCombiTrans ("M");
	M->RotateX (45.);
	M->SetDy (-5.575);
	M->SetDz (6.935);
	M->RegisterYourself();

	TGeoShape* coneShp = new TGeoCompositeShape ("Cone_shp", "A-B:M");
	TGeoVolume* coneVol = new TGeoVolume ("Cone", coneShp, framesMaterial);
	coneVol->SetLineColor(kGreen);
//	coneVol->RegisterYourself();
	
//	// --- Inner cone
//	Double_t thickness = 0.02;
//	Double_t thickness2 = 0.022;
//	//	TGeoConeSeg* A2 = new TGeoConeSeg ("A2", coneDz-thickness, 6.+thickness, 7.6-thickness2, 5.99+thickness, 6.05-thickness2, 0., 180.);
//	TGeoConeSeg* A2 = new TGeoConeSeg ("A2", coneDz-thickness, 3.+thickness, 4.6-thickness2, 2.99+thickness, 3.05-thickness2, 0., 180.);
//
//	TGeoCombiTrans* M2 = new TGeoCombiTrans ("M2");
//	M2->RotateX (45.);
//	M2->SetDy (-5.575+thickness*sqrt(2.));
//	M2->SetDz (6.935);
//	M2->RegisterYourself();
//
//	TGeoShape* coneShp2 = new TGeoCompositeShape ("Cone2_shp", "A2-B:M2");
//	TGeoVolume* coneVol2 = new TGeoVolume ("Cone2", coneShp2, gStsMedium);
//	coneVol2->SetLineColor(kGreen);
////	coneVol2->RegisterYourself();
//
//	coneVol->AddNode(coneVol2, 1);

	return coneVol;
}
/** ======================================================================= **/

/** ======================================================================= **/
TGeoVolume* ConstructBigCone(Double_t coneDz)
{
	// --- Material of the frames
	TGeoMedium* framesMaterial = gGeoMan->GetMedium("carbon");

	// --- Outer cone
	TGeoConeSeg* bA = new TGeoConeSeg ("bA", coneDz, 6., 7.6, 6., 6.04, 0., 180.);
	TGeoBBox* bB = new TGeoBBox ("bB", 8., 6., 10.);

	TGeoCombiTrans* bM = new TGeoCombiTrans ("bM");
	bM->RotateX (45.);
	bM->SetDy (-5.575);
	bM->SetDz (6.935);
	bM->RegisterYourself();

	TGeoShape* coneBigShp = new TGeoCompositeShape ("ConeBig_shp", "bA-bB:bM");
	TGeoVolume* coneBigVol = new TGeoVolume ("ConeBig", coneBigShp, framesMaterial);
	coneBigVol->SetLineColor(kGreen);
//	coneBigVol->RegisterYourself();

	// --- Inner cone
	Double_t thickness = 0.02;
	Double_t thickness2 = 0.022;
	TGeoConeSeg* bA2 = new TGeoConeSeg ("bA2", coneDz-thickness, 6.+thickness, 7.6-thickness2, 5.99+thickness, 6.05-thickness2, 0., 180.);

	TGeoCombiTrans* bM2 = new TGeoCombiTrans ("bM2");
	bM2->RotateX (45.);
	bM2->SetDy (-5.575+thickness*sqrt(2.));
	bM2->SetDz (6.935);
	bM2->RegisterYourself();

	TGeoShape* coneBigShp2 = new TGeoCompositeShape ("ConeBig2_shp", "bA2-bB:bM2");
	TGeoVolume* coneBigVol2 = new TGeoVolume ("ConeBig2", coneBigShp2, gStsMedium);
	coneBigVol2->SetLineColor(kGreen);
//	coneBigVol2->RegisterYourself();

	coneBigVol->AddNode(coneBigVol2, 1);

	return coneBigVol;
}
/** ======================================================================= **/
