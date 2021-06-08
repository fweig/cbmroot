/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig [committer] */

/******************************************************************************
 ** Creation of STS geometry in ROOT format (TGeo).
 **
 ** @file create_stsgeo_v19g.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 15 June 2012
 ** @date 09.05.2014
 ** @author Tomas Balog <T.Balog@gsi.de>
 **
 ** 2019-08-12 - DE - v19n: mSTS as built in March 2019 - the mSTS FEBs are in the bottom
 ** 2019-03-15 - DE - v18n: mSTS as built in March 2019 - downstream ladder of station 0 at position of station 1
 ** 2018-01-18 - DE - v18g: set overlaps in X and Y according to mSTS CAD model
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


#include "TGeoCompositeShape.h"
#include "TGeoCone.h"
#include "TGeoManager.h"
#include "TGeoPara.h"
#include "TGeoTube.h"

#include <iomanip>
#include <iostream>


// -------------   Steering variables       -----------------------------------

// ---> Thickness of sensors [cm]
const Double_t gkSensorThickness = 0.03;

// ---> Vertical gap between chained sensors [cm]
const Double_t gkChainGapY = 0.00;

// ---> Thickness of cables [cm]
const Double_t gkCableThickness = 0.02;

// ---> Vertical overlap of neighbouring sectors in a ladder [cm]
const Double_t gkSectorOverlapY = 0.46;  // 0.27;

// ---> Gap in z between neighbouring sectors in a ladder [cm]
const Double_t gkSectorGapZ = 0.02;

// ---> Horizontal overlap of neighbouring ladders [cm]
const Double_t gkLadderOverlapX = 0.25;  // 0.30;

// ---> Gap in z between neighbouring ladders [cm]
const Double_t gkLadderGapZ = 1.00;  // 0.00;

// ---> Gap in z between lowest sector to carbon support structure [cm]
const Double_t gkSectorGapZFrame = 0.10;

// ---> Switch to construct / not to construct readout cables
const Bool_t gkConstructCables = kTRUE;

// ---> Switch to construct / not to construct frames
const Bool_t gkConstructCones       = kFALSE;  // kFALSE;  // switch this false for v15a
const Bool_t gkConstructFrames      = kTRUE;   // kFALSE;  // switch this false for v15a
const Bool_t gkConstructSmallFrames = kTRUE;   // kFALSE;
const Bool_t gkCylindricalFrames    = kTRUE;   // kFALSE;

// ---> Size of the frame
const Double_t gkFrameThickness     = 0.2;
const Double_t gkThinFrameThickness = 0.05;
const Double_t gkFrameStep          = 4.0;  // size of frame cell along y direction

const Double_t gkCylinderDiaInner =
  0.07;  // properties of cylindrical carbon supports, see CBM-STS Integration Meeting (10 Jul 2015)
const Double_t gkCylinderDiaOuter =
  0.15;  // properties of cylindrical carbon supports, see CBM-STS Integration Meeting (10 Jul 2015)

// ----------------------------------------------------------------------------


// --------------   Parameters of beam pipe in the STS region    --------------
// ---> Needed to compute stations and STS such as to avoid overlaps
const Double_t gkPipeZ1 = 22.0;
const Double_t gkPipeR1 = 1.8;
const Double_t gkPipeZ2 = 50.0;
const Double_t gkPipeR2 = 1.8;
const Double_t gkPipeZ3 = 125.0;
const Double_t gkPipeR3 = 5.5;

//DE const Double_t gkPipeZ1 =  27.0;
//DE const Double_t gkPipeR1 =   1.05;
//DE const Double_t gkPipeZ2 = 160.0;
//DE const Double_t gkPipeR2 =   3.25;
// ----------------------------------------------------------------------------


// -------------   Other global variables   -----------------------------------
// ---> STS medium (for every volume except silicon)
TGeoMedium* gStsMedium = NULL;  // will be set later
// ---> TGeoManager (too lazy to write out 'Manager' all the time
TGeoManager* gGeoMan = NULL;  // will be set later
// ----------------------------------------------------------------------------

Int_t CreateSensors();
Int_t CreateSectors();
Int_t CreateLadders();
void CheckVolume(TGeoVolume* volume);
void CheckVolume(TGeoVolume* volume, fstream& file);
TGeoVolume* ConstructFrameElement(const TString& name, TGeoVolume* frameBoxVol, Double_t x);
TGeoVolume* ConstructHalfLadder(Int_t ladderid, const TString& name, Int_t nSectors, Int_t* sectorTypes, char align);
TGeoVolume* ConstructLadder(Int_t LadderIndex, TGeoVolume* halfLadderU, TGeoVolume* halfLadderD, Double_t shiftZ);
TGeoVolume* ConstructLadderWithGap(Int_t LadderIndex, TGeoVolume* halfLadderU, TGeoVolume* halfLadderD, Double_t gapY);
TGeoVolume* ConstructStation(Int_t iStation, Int_t nLadders, Int_t* ladderTypes, Double_t rHole);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_stsgeo_v19g(const char* geoTag = "v19g_mcbm")
{

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = "sts_";
  geoFileName         = geoFileName + geoTag + ".geo.root";
  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = geoFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "STS geometry created with create_stsgeo_v19g.C" << endl << endl;
  infoFile << "The geometry is the same as sts_v19b_mcbm but in the new "
              "geometry file format."
           << endl;
  infoFile << "The geo file contains the exported sts keeping volume plus" << endl;
  infoFile << "the corresponding transformation matrix to place it in the experiment." << endl << endl;
  infoFile << "Global variables: " << endl;
  infoFile << "Sensor thickness = " << gkSensorThickness << " cm" << endl;
  infoFile << "Vertical gap in sensor chain = " << gkChainGapY << " cm" << endl;
  infoFile << "Vertical overlap of sensors = " << gkSectorOverlapY << " cm" << endl;
  infoFile << "Gap in z between neighbour sensors = " << gkSectorGapZ << " cm" << endl;
  infoFile << "Horizontal overlap of sensors = " << gkLadderOverlapX << " cm" << endl;
  infoFile << "Gap in z between neighbour ladders = " << gkLadderGapZ << " cm" << endl;
  if (gkConstructCables) infoFile << "Cable thickness = " << gkCableThickness << " cm" << endl;
  else
    infoFile << "No cables" << endl;
  infoFile << endl;
  infoFile << "Beam pipe: R1 = " << gkPipeR1 << " cm at z = " << gkPipeZ1 << " cm" << endl;
  infoFile << "Beam pipe: R2 = " << gkPipeR2 << " cm at z = " << gkPipeZ2 << " cm" << endl;
  infoFile << "Beam pipe: R3 = " << gkPipeR3 << " cm at z = " << gkPipeZ3 << " cm" << endl;
  // --------------------------------------------------------------------------


  // -------   Load media from media file   -----------------------------------
  FairGeoLoader* geoLoad    = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath           = gSystem->Getenv("VMCWORKDIR");
  TString medFile           = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------


  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia   = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // ---> air
  FairGeoMedium* mAir = geoMedia->getMedium("air");
  if (!mAir) Fatal("Main", "FairMedium air not found");
  geoBuild->createMedium(mAir);
  TGeoMedium* air = gGeoMan->GetMedium("air");
  if (!air) Fatal("Main", "Medium air not found");

  // ---> silicon
  FairGeoMedium* mSilicon = geoMedia->getMedium("silicon");
  if (!mSilicon) Fatal("Main", "FairMedium silicon not found");
  geoBuild->createMedium(mSilicon);
  TGeoMedium* silicon = gGeoMan->GetMedium("silicon");
  if (!silicon) Fatal("Main", "Medium silicon not found");

  // ---> carbon
  FairGeoMedium* mCarbon = geoMedia->getMedium("carbon");
  if (!mCarbon) Fatal("Main", "FairMedium carbon not found");
  geoBuild->createMedium(mCarbon);
  TGeoMedium* carbon = gGeoMan->GetMedium("carbon");
  if (!carbon) Fatal("Main", "Medium carbon not found");

  // ---> STScable
  FairGeoMedium* mSTScable = geoMedia->getMedium("STScable");
  if (!mSTScable) Fatal("Main", "FairMedium STScable not found");
  geoBuild->createMedium(mSTScable);
  TGeoMedium* STScable = gGeoMan->GetMedium("STScable");
  if (!STScable) Fatal("Main", "Medium STScable not found");

  // ---
  gStsMedium = air;
  // --------------------------------------------------------------------------


  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("STSgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

  // ---------------   Create sensor   ----------------------------------------
  cout << endl << endl;
  cout << "===> Creating sensors...." << endl << endl;
  infoFile << endl << "Sensors: " << endl;
  Int_t nSensors = CreateSensors();

  Int_t iSensor      = 4;
  TString name       = Form("Sensor%02d", iSensor);
  TGeoVolume* sensor = gGeoMan->GetVolume(name);
  sensor->SetLineColor(kBlue);
  CheckVolume(sensor);
  CheckVolume(sensor, infoFile);
  // --------------------------------------------------------------------------

  // ----------------   Create sector   ---------------------------------------
  cout << endl << endl;
  cout << "===> Creating sectors...." << endl;
  infoFile << endl << "Sectors: " << endl;
  Int_t nSectors = CreateSectors();
  Int_t iSector  = 4;
  cout << endl;
  name               = Form("Sector%02d", iSector);
  TGeoVolume* sector = gGeoMan->GetVolume(name);
  CheckVolume(sector);
  CheckVolume(sector, infoFile);
  // --------------------------------------------------------------------------

  // ----------------   Create ladder   ---------------------------------------
  //TString name = "";
  cout << endl << endl;
  cout << "===> Creating ladders...." << endl;
  infoFile << endl << "Ladders:" << endl;
  Int_t nLadders = CreateLadders();
  Int_t iLadder  = 9;
  cout << endl;
  name               = Form("Ladder%02d", iLadder);
  TGeoVolume* ladder = gGeoMan->GetVolume(name);
  CheckVolume(ladder);
  CheckVolume(ladder, infoFile);
  CheckVolume(ladder->GetNode(0)->GetVolume(), infoFile);
  // --------------------------------------------------------------------------

  // ----------------   Create stations   -------------------------------------
  Float_t statPos[8] = {28., 42., 50., 60., 70., 80., 90., 100.};

  cout << endl << endl;
  cout << "===> Creating stations...." << endl;
  infoFile << endl << "Stations: ";
  nLadders = 0;
  Int_t ladderTypes[20];
  Double_t statZ             = 0.;
  Double_t rHole             = 0.;
  TGeoBBox* statShape        = NULL;
  TGeoTranslation* statTrans = NULL;


  // --- Station 01: 8 ladders, type 3 2 2 1 1 2 2 3
  cout << endl;
  statZ    = 30.;
  rHole    = 2.0;
  nLadders = 2;

  ladderTypes[0] = 9;
  ladderTypes[1] = 9;

  TGeoVolume* station01 = ConstructStation(0, nLadders, ladderTypes, rHole);
  CheckVolume(station01);
  CheckVolume(station01, infoFile);
  infoFile << "Position z = " << statPos[0] << endl;

  // ---------------   Create STS volume   ------------------------------------
  cout << endl << endl;
  cout << "===> Creating STS...." << endl;

  TString stsName = "sts_";
  stsName += geoTag;

  Double_t stsPosZ = 0.5 * (statPos[1] + statPos[0]);

  TGeoVolumeAssembly* sts = new TGeoVolumeAssembly(stsName.Data());  // do not produce keeping volumes

  // --- Place station in the STS
  Int_t iStation         = 1;
  TString statName       = Form("Station%02d", iStation);
  TGeoVolume* station    = gGeoMan->GetVolume(statName);
  Double_t posZ          = statPos[iStation - 1] - stsPosZ;
  TGeoTranslation* trans = new TGeoTranslation(0., 0., posZ);  // standard
  sts->AddNode(station, iStation, trans);
  sts->GetShape()->ComputeBBox();
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

  sts->Export(geoFileName);
  TFile* geoFile = new TFile(geoFileName, "UPDATE");
  stsTrans->Write();
  geoFile->Close();

  TString geoFileName_ = "sts_";
  geoFileName_         = geoFileName_ + geoTag + "_geo.root";

  geoFile = new TFile(geoFileName_, "RECREATE");
  gGeoMan->Write();  // use this is you want GeoManager format in the output
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
 ** Create sensors
 **
 ** Sensors are created as volumes with box shape and active silicon as medium.
 ** Four kinds of sensors: 3.2x2.2, 6.2x2.2, 6.2x4.2, 6.2x6.2
 **/
Int_t CreateSensors()
{

  Int_t nSensors = 0;

  Double_t xSize      = 0.;
  Double_t ySize      = 0.;
  Double_t zSize      = gkSensorThickness;
  TGeoMedium* silicon = gGeoMan->GetMedium("silicon");

  // ---  Sensor type 04: Big sensor (6.2 cm x 6.2 cm)
  xSize                    = 6.2092;
  ySize                    = 6.2;
  TGeoBBox* shape_sensor04 = new TGeoBBox("sensor04", xSize / 2., ySize / 2., zSize / 2.);
  new TGeoVolume("Sensor04", shape_sensor04, silicon);
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
Int_t CreateSectors()
{

  Int_t nSectors = 0;

  TGeoVolume* sensor04 = gGeoMan->GetVolume("Sensor04");
  TGeoBBox* box4       = (TGeoBBox*) sensor04->GetShape();

  // --- Sector type 4: single sensor of type 4
  TGeoVolumeAssembly* sector04 = new TGeoVolumeAssembly("Sector04");
  sector04->AddNode(sensor04, 1);
  sector04->GetShape()->ComputeBBox();
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
Int_t CreateLadders()
{

  Int_t nLadders = 0;

  // --- Some variables
  Int_t nSectors = 0;
  Int_t sectorTypes[10];
  TGeoBBox* shape = NULL;
  TString s0name;
  TGeoVolume* s0vol       = NULL;
  TGeoVolume* halfLadderU = NULL;
  TGeoVolume* halfLadderD = NULL;
  Double_t shiftZ         = 0.;
  Double_t ladderY        = 0.;
  Double_t gapY           = 0.;


  // --- Ladder 09: 2 sectors, type 4 4 - mCBM station 1
  nSectors       = 2;
  sectorTypes[0] = 4;
  sectorTypes[1] = 4;
  s0name         = Form("Sector%02d", sectorTypes[0]);
  s0vol          = gGeoMan->GetVolume(s0name);
  shape          = (TGeoBBox*) s0vol->GetShape();
  shiftZ         = 2. * shape->GetDZ() + gkSectorGapZ;
  halfLadderU    = ConstructHalfLadder(9, "HalfLadder09u", 0, sectorTypes, 'l');
  halfLadderD    = ConstructHalfLadder(9, "HalfLadder09d", nSectors, sectorTypes, 'r');
  //
  //  ConstructLadder(9, halfLadderU, halfLadderD, shiftZ);
  ConstructLadder(9, nullptr, halfLadderD, shiftZ);
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
TGeoVolume* ConstructModule(const char* name, TGeoVolume* sector, Double_t cableLength)
{

  // --- Check sector volume
  if (!sector) Fatal("CreateModule", "Sector volume not found!");

  // --- Get size of sector
  TGeoBBox* box    = (TGeoBBox*) sector->GetShape();
  Double_t sectorX = 2. * box->GetDX();
  Double_t sectorY = 2. * box->GetDY();
  Double_t sectorZ = 2. * box->GetDZ();

  // --- Get size of cable
  Double_t cableX = sectorX;
  Double_t cableY = cableLength;
  Double_t cableZ = gkCableThickness;

  // --- Create module volume
  Double_t moduleX   = TMath::Max(sectorX, cableX);
  Double_t moduleY   = sectorY + cableLength;
  Double_t moduleZ   = TMath::Max(sectorZ, cableZ);
  TGeoVolume* module = gGeoManager->MakeBox(name, gStsMedium, moduleX / 2., moduleY / 2., moduleZ / 2.);

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
    Double_t sensorXpos        = 0.;
    Double_t sensorYpos        = sectorYpos + xSensTrans[1];
    Double_t sensorZpos        = 0.;
    TGeoTranslation* sensTrans = new TGeoTranslation("sensTrans", sensorXpos, sensorYpos, sensorZpos);

    // --- Add sensor volume to module
    TGeoVolume* sensVol = sensor->GetVolume();
    module->AddNode(sensor->GetVolume(), iSensor + 1, sensTrans);
    module->GetShape()->ComputeBBox();
  }


  // --- Create cable volume, if necessary, and place it in module
  // --- Cable is centred in x and z and aligned to the top
  if (gkConstructCables && cableLength > 0.0001) {
    TString cableName       = TString(name) + "_cable";
    TGeoMedium* cableMedium = gGeoMan->GetMedium("STScable");
    if (!cableMedium) Fatal("CreateModule", "Medium STScable not found!");
    TGeoVolume* cable = gGeoManager->MakeBox(cableName.Data(), cableMedium, cableX / 2., cableY / 2., cableZ / 2.);
    // add color to cables
    cable->SetLineColor(kOrange);
    cable->SetTransparency(60);
    Double_t cableXpos          = 0.;
    Double_t cableYpos          = sectorY + 0.5 * cableY - 0.5 * moduleY;
    Double_t cableZpos          = 0.;
    TGeoTranslation* cableTrans = new TGeoTranslation("cableTrans", cableXpos, cableYpos, cableZpos);
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
TGeoVolume* ConstructHalfLadder(Int_t ladderid, const TString& name, Int_t nSectors, Int_t* sectorTypes, char align)
{

  // --- Create half ladder volume assembly
  TGeoVolumeAssembly* halfLadder = new TGeoVolumeAssembly(name);

  // --- Determine size of ladder
  Double_t ladderX = 0.;
  Double_t ladderY = 0.;
  Double_t ladderZ = 0.;
  for (Int_t iSector = 0; iSector < nSectors; iSector++) {
    TString sectorName = Form("Sector%02d", sectorTypes[iSector]);
    TGeoVolume* sector = gGeoMan->GetVolume(sectorName);
    if (!sector) Fatal("ConstructHalfLadder", (char*) Form("Volume %s not found", sectorName.Data()));
    TGeoBBox* box = (TGeoBBox*) sector->GetShape();
    // --- Ladder x size equals largest sector x size
    ladderX = TMath::Max(ladderX, 2. * box->GetDX());
    // --- Ladder y size is sum of sector ysizes
    ladderY += 2. * box->GetDY();
    // --- Ladder z size is sum of sector z sizes
    ladderZ += 2. * box->GetDZ();
  }
  // --- Subtract overlaps in y
  ladderY -= Double_t(nSectors - 1) * gkSectorOverlapY;
  // --- Add gaps in z direction
  ladderZ += Double_t(nSectors - 1) * gkSectorGapZ;


  // --- Create and place modules
  Double_t yPosSect = -0.5 * ladderY;
  Double_t zPosMod  = -0.5 * ladderZ;
  for (Int_t iSector = 0; iSector < nSectors; iSector++) {
    TString sectorName = Form("Sector%02d", sectorTypes[iSector]);
    TGeoVolume* sector = gGeoMan->GetVolume(sectorName);
    TGeoBBox* box      = (TGeoBBox*) sector->GetShape();
    Double_t sectorX   = 2. * box->GetDX();
    Double_t sectorY   = 2. * box->GetDY();
    Double_t sectorZ   = 2. * box->GetDZ();
    yPosSect += 0.5 * sectorY;  // Position of sector in ladder
    Double_t cableLength = 0.5 * ladderY - yPosSect - 0.5 * sectorY;
    TString moduleName   = name + "_" + Form("Module%02d", sectorTypes[iSector]);
    TGeoVolume* module   = ConstructModule(moduleName.Data(), sector, cableLength);

    TGeoBBox* shapeMod = (TGeoBBox*) module->GetShape();
    Double_t moduleX   = 2. * shapeMod->GetDX();
    Double_t moduleY   = 2. * shapeMod->GetDY();
    Double_t moduleZ   = 2. * shapeMod->GetDZ();
    Double_t xPosMod   = 0.;
    if (align == 'l') xPosMod = 0.5 * (moduleX - ladderX);  // left aligned
    else if (align == 'r')
      xPosMod = 0.5 * (ladderX - moduleX);  // right aligned
    else
      xPosMod = 0.;                                // centred in x
    Double_t yPosMod = 0.5 * (ladderY - moduleY);  // top aligned
    zPosMod += 0.5 * moduleZ;
    TGeoTranslation* trans = new TGeoTranslation("t", xPosMod, yPosMod, zPosMod);
    // DEDE
    // drop 2nd module on this halfladder for mSTS Nov 2019
    //    halfLadder->AddNode(module, iSector+1, trans);
    if (ladderid == 9) cout << "DE333 " << iSector << endl;
    if (ladderid == 9)
      if (iSector == 0) halfLadder->AddNode(module, iSector + 1, trans);
    halfLadder->GetShape()->ComputeBBox();
    yPosSect += 0.5 * sectorY - gkSectorOverlapY;
    zPosMod += 0.5 * moduleZ + gkSectorGapZ;
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
void AddCarbonLadder(Int_t LadderIndex, TGeoVolume* ladder, Double_t xu, Double_t ladderY, Double_t ladderZ)
{

  // --- Some variables
  TString name = Form("Ladder%02d", LadderIndex);
  Int_t i;
  Double_t j;

  Int_t YnumOfFrameBoxes = (Int_t)(ladderY / gkFrameStep) + 1;  // calculate number of elements
  if (LadderIndex == 1 || LadderIndex == 2)  // set even number of ladder elements for these ladders in station 1 and 2
    YnumOfFrameBoxes--;
  //  if (LadderIndex == 3 || LadderIndex == 4)  // set even number of ladder elements for these ladders in station 3 and 4
  //	YnumOfFrameBoxes++;
  YnumOfFrameBoxes += YnumOfFrameBoxes % 2;  // use even number of frame elements for all ladders

  //      cout << "DE: lad " << LadderIndex << " inum " << YnumOfFrameBoxes << endl;

  // DEDE
  TGeoBBox* fullFrameShp = new TGeoBBox(name + "_FullFrameBox_shp", xu / 2., gkFrameStep / 2.,
                                        (xu / 2. + sqrt(2.) * gkFrameThickness / 2.) / 2.);
  //  TGeoBBox* fullFrameShp = new TGeoBBox (name+"_FullFrameBox_shp", xu/2., gkFrameStep/2., (gkSectorGapZFrame+xu/2.+sqrt(2.)*gkFrameThickness/2.)/2.);
  TGeoVolume* fullFrameBoxVol = new TGeoVolume(name + "_FullFrameBox", fullFrameShp, gStsMedium);

  //  cout << "DE: frame Z size " << (xu/2.+sqrt(2.)*gkFrameThickness/2.) << " cm" << endl;

  ConstructFrameElement("FrameBox", fullFrameBoxVol, xu / 2.);
  TGeoRotation* fullFrameRot = new TGeoRotation;
  fullFrameRot->RotateY(180);

  Int_t inum = YnumOfFrameBoxes;  // 6; // 9;
  for (i = 1; i <= inum; i++) {
    j = -(inum - 1) / 2. + (i - 1);
    //        cout << "DE: i " << i << " j " << j << endl;

    if (LadderIndex <= 2)  // central ladders in stations 1 to 8
    {
      if ((j >= -1) && (j <= 1))  // keep the inner 4 elements free for the cone
        continue;
    }
    else if (LadderIndex <= 8)  // central ladders in stations 1 to 8
    {
      if ((j >= -2) && (j <= 2))  // keep the inner 4 elements free for the cone
        continue;
    }

    // DEDE
    ladder->AddNode(fullFrameBoxVol, i,
                    new TGeoCombiTrans(name + "_FullFrameBox_posrot", 0., j * gkFrameStep,
                                       -ladderZ / 2. - (xu / 2. + sqrt(2.) * gkFrameThickness / 2.) / 2.,
                                       fullFrameRot));
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

TGeoVolume* ConstructLadder(Int_t LadderIndex, TGeoVolume* halfLadderU, TGeoVolume* halfLadderD, Double_t shiftZ)
{

  // --- Some variables
  TGeoBBox* shape = NULL;
  Double_t xu     = 0.;
  Double_t yu     = 0.;
  Double_t zu     = 0.;

  // --- Dimensions of half ladders
  if (halfLadderU) {
    shape       = (TGeoBBox*) halfLadderU->GetShape();
    Double_t xu = 2. * shape->GetDX();
    Double_t yu = 2. * shape->GetDY();
    Double_t zu = 2. * shape->GetDZ();
  }

  shape       = (TGeoBBox*) halfLadderD->GetShape();
  Double_t xd = 2. * shape->GetDX();
  Double_t yd = 2. * shape->GetDY();
  Double_t zd = 2. * shape->GetDZ();

  // --- Create ladder volume assembly
  TString name               = Form("Ladder%02d", LadderIndex);
  TGeoVolumeAssembly* ladder = new TGeoVolumeAssembly(name);
  Double_t ladderX           = TMath::Max(xu, xd);
  //  Double_t ladderY = yu + yd - gkSectorOverlapY;
  Double_t ladderY = TMath::Max(yu, yd);
  Double_t ladderZ = TMath::Max(zu, zd + shiftZ);

  // --- Place half ladders
  if (halfLadderU) {
    Double_t xPosU      = 0.;                    // centred in x
    Double_t yPosU      = 0.5 * (ladderY - yu);  // top aligned
    Double_t zPosU      = 0.5 * (ladderZ - zu);  // front aligned
    TGeoTranslation* tu = new TGeoTranslation("tu", xPosU, yPosU, zPosU);
    ladder->AddNode(halfLadderU, 1, tu);
  }

  Double_t xPosD = 0.;                    // centred in x
  Double_t yPosD = 0.5 * (yd - ladderY);  // bottom aligned
  Double_t zPosD = 0.5 * (zd - ladderZ);  // back aligned
  //  Double_t zPosD = 0.75 * ( zd - ladderZ );  // back aligned

  cout << "ZPOS=" << zPosD << endl;
  /*
  if (yu == 0)  // if no top (= only bottom) half ladder
  {
    yPosD = 0.5 * ( ladderY - yd );  // top aligned
    zPosD = 0.5 * ( ladderZ - zd );  // back aligned
  }
*/
  TGeoRotation* rd = new TGeoRotation();
  rd->RotateZ(180.);
  TGeoCombiTrans* cd = new TGeoCombiTrans(xPosD, yPosD, zPosD + 0.025, rd);
  ladder->AddNode(halfLadderD, 2, cd);
  ladder->GetShape()->ComputeBBox();

  // ----------------   Create and place frame boxes   ------------------------

  if (gkConstructFrames) {
    AddCarbonLadder(LadderIndex, ladder, ladderX, ladderY, ladderZ);  // take width of any HL
  }
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

TGeoVolume* ConstructLadderWithGap(Int_t LadderIndex, TGeoVolume* halfLadderU, TGeoVolume* halfLadderD, Double_t gapY)
{

  // --- Some variables
  TGeoBBox* shape = NULL;
  Int_t i;
  Double_t j;

  // --- Dimensions of half ladders
  shape       = (TGeoBBox*) halfLadderU->GetShape();
  Double_t xu = 2. * shape->GetDX();
  Double_t yu = 2. * shape->GetDY();
  Double_t zu = 2. * shape->GetDZ();

  shape       = (TGeoBBox*) halfLadderD->GetShape();
  Double_t xd = 2. * shape->GetDX();
  Double_t yd = 2. * shape->GetDY();
  Double_t zd = 2. * shape->GetDZ();

  // --- Create ladder volume assembly
  TString name               = Form("Ladder%02d", LadderIndex);
  TGeoVolumeAssembly* ladder = new TGeoVolumeAssembly(name);
  Double_t ladderX           = TMath::Max(xu, xd);
  Double_t ladderY           = yu + yd + gapY;
  Double_t ladderZ           = TMath::Max(zu, zd);

  // --- Place half ladders
  Double_t xPosU      = 0.;                    // centred in x
  Double_t yPosU      = 0.5 * (ladderY - yu);  // top aligned
  Double_t zPosU      = 0.5 * (ladderZ - zu);  // front aligned
  TGeoTranslation* tu = new TGeoTranslation("tu", xPosU, yPosU, zPosU);
  ladder->AddNode(halfLadderU, 1, tu);

  Double_t xPosD   = 0.;                    // centred in x
  Double_t yPosD   = 0.5 * (yd - ladderY);  // bottom aligned
  Double_t zPosD   = 0.5 * (zd - ladderZ);  // back aligned
  TGeoRotation* rd = new TGeoRotation();
  rd->RotateZ(180.);
  TGeoCombiTrans* cd = new TGeoCombiTrans(xPosD, yPosD, zPosD, rd);
  ladder->AddNode(halfLadderD, 2, cd);
  ladder->GetShape()->ComputeBBox();

  // ----------------   Create and place frame boxes   ------------------------

  if (gkConstructFrames) AddCarbonLadder(LadderIndex, ladder, xu, ladderY, ladderZ);

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


TGeoVolume* ConstructStation(Int_t iStation, Int_t nLadders, Int_t* ladderTypes, Double_t rHole)
{

  TString name;
  name = Form("Station%02d", iStation + 1);  // 1,2,3,4,5,6,7,8

  // --- Some local variables
  TGeoShape* statShape  = NULL;
  TGeoBBox* ladderShape = NULL;
  TGeoBBox* shape       = NULL;
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
    ladderName       = Form("Ladder%02d", ladderType);
    ladder           = gGeoManager->GetVolume(ladderName);
    if (!ladder) Fatal("ConstructStation", Form("Volume %s not found", ladderName.Data()));
    shape = (TGeoBBox*) ladder->GetShape();
    statX += 2. * shape->GetDX();
    statY = TMath::Max(statY, 2. * shape->GetDY());
    if (iLadder % 2) statZeven = TMath::Max(statZeven, 2. * shape->GetDZ());
    else
      statZodd = TMath::Max(statZodd, 2. * shape->GetDZ());
  }
  statX -= Double_t(nLadders - 1) * gkLadderOverlapX;
  statZ = statZeven + gkLadderGapZ + statZodd;

  // --- Create station volume
  TString boxName(name);
  boxName += "_box";

  cout << "before   statZ/2.: " << statZ / 2. << endl;
  statZ = 2 * 4.5;  // changed Z size of the station for cone and gkLadderGapZ
  cout << "fixed to statZ/2.: " << statZ / 2. << endl;
  TGeoBBox* statBox = new TGeoBBox(boxName, statX / 2., statY / 2., statZ / 2.);

  TGeoVolumeAssembly* station = new TGeoVolumeAssembly(name);  // do not produce keeping volumes

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
    ladderName       = Form("Ladder%02d", ladderType);
    ladder           = gGeoManager->GetVolume(ladderName);
    shape            = (TGeoBBox*) ladder->GetShape();
    if (maxdz < shape->GetDZ()) maxdz = shape->GetDZ();
  }

  for (Int_t iLadder = 0; iLadder < nLadders; iLadder++) {
    Int_t ladderType = ladderTypes[iLadder];
    ladderName       = Form("Ladder%02d", ladderType);
    ladder           = gGeoManager->GetVolume(ladderName);
    shape            = (TGeoBBox*) ladder->GetShape();
    xPos += shape->GetDX();
    cout << "xPos2: " << xPos << endl;
    yPos              = 0.;  // vertically centred
    TGeoRotation* rot = new TGeoRotation();

    if (gkConstructFrames)
      // DEDE
      subtractedVal = sqrt(2.) * gkFrameThickness / 2. + shape->GetDX();
    //      subtractedVal = 2*gkSectorGapZFrame + sqrt(2.)*gkFrameThickness/2. + shape->GetDX();
    else
      subtractedVal = 0.;

    //    zPos = 0.5 * gkLadderGapZ + (shape->GetDZ()-subtractedVal/2.);  // non z-aligned ladders
    zPos = 0.5 * gkLadderGapZ + (2 * maxdz - shape->GetDZ() - subtractedVal / 2.);  // z-aligned ladders

    cout << "DE ladder" << ladderTypes[iLadder] << "  dx: " << shape->GetDX() << "  dy: " << shape->GetDY()
         << "  dz: " << shape->GetDZ() << "  max dz: " << maxdz << endl;

    cout << "DE ladder" << ladderTypes[iLadder] << "  fra: " << gkFrameThickness / 2. << "  sub: " << subtractedVal
         << "  zpo: " << zPos << endl
         << endl;

    //    if (iStation % 2 == 0) // flip ladders for even stations to reproduce CAD layout
    //    // even station 0,2,4,6
    if (iStation % 2 == 1)  // flip ladders for odd stations to reproduce CAD layout
    // odd station 1,3,5,7
    {
      // --- Unrotated ladders --- downstream
      if ((nLadders / 2 + iLadder) % 2) {
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
      if ((nLadders / 2 + iLadder) % 2) {
        //        zPos = -0.5 * gkLadderGapZ - (shape->GetDZ()-subtractedVal/2.);
        zPos = -zPos;
      }
      // --- Rotated ladders --- downstream
      else {
        //        zPos = 0.5 * gkLadderGapZ + (shape->GetDZ()-subtractedVal/2.);
        rot->RotateY(180.);
        //        zPos += 14.;  // move STS ladder from position of C-frame #1 to C-frame #3 - March 2019 version
      }
    }

    TGeoCombiTrans* trans = new TGeoCombiTrans(xPos, yPos, zPos, rot);
    // drop upstream ladder for mSTS Nov 2019
    //    station->AddNode(ladder, iLadder+1, trans);
    cout << "DE222 " << iLadder << endl;
    if (iLadder == 1) station->AddNode(ladder, iLadder + 1, trans);
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
void CheckVolume(TGeoVolume* volume)
{

  TGeoBBox* shape = (TGeoBBox*) volume->GetShape();
  cout << volume->GetName() << ": size " << fixed << setprecision(4) << setw(7) << 2. * shape->GetDX() << " x "
       << setw(7) << 2. * shape->GetDY() << " x " << setw(7) << 2. * shape->GetDZ();
  if (volume->IsAssembly()) cout << ", assembly";
  else {
    if (volume->GetMedium()) cout << ", medium " << volume->GetMedium()->GetName();
    else
      cout << ", "
           << "\033[31m"
           << " no medium"
           << "\033[0m";
  }
  cout << endl;
  if (volume->GetNdaughters()) {
    cout << "Daughters: " << endl;
    for (Int_t iNode = 0; iNode < volume->GetNdaughters(); iNode++) {
      TGeoNode* node  = volume->GetNode(iNode);
      TGeoBBox* shape = (TGeoBBox*) node->GetVolume()->GetShape();
      cout << setw(15) << node->GetName() << ", size " << fixed << setprecision(3) << setw(6) << 2. * shape->GetDX()
           << " x " << setw(6) << 2. * shape->GetDY() << " x " << setw(6) << 2. * shape->GetDZ() << ", position ( ";
      TGeoMatrix* matrix  = node->GetMatrix();
      const Double_t* pos = matrix->GetTranslation();
      cout << setfill(' ');
      cout << fixed << setw(8) << pos[0] << ", " << setw(8) << pos[1] << ", " << setw(8) << pos[2] << " )" << endl;
    }
  }
}
/** ======================================================================= **/


/** ===========================================================================
 ** Volume information for output to file
 **/
void CheckVolume(TGeoVolume* volume, fstream& file)
{

  if (!file) return;

  TGeoBBox* shape = (TGeoBBox*) volume->GetShape();
  file << volume->GetName() << ": size " << fixed << setprecision(4) << setw(7) << 2. * shape->GetDX() << " x "
       << setw(7) << 2. * shape->GetDY() << " x " << setw(7) << 2. * shape->GetDZ();
  if (volume->IsAssembly()) file << ", assembly";
  else {
    if (volume->GetMedium()) file << ", medium " << volume->GetMedium()->GetName();
    else
      file << ", "
           << "\033[31m"
           << " no medium"
           << "\033[0m";
  }
  file << endl;
  if (volume->GetNdaughters()) {
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
Double_t BeamPipeRadius(Double_t z)
{
  if (z < gkPipeZ2) return gkPipeR1;
  Double_t slope = (gkPipeR3 - gkPipeR2) / (gkPipeZ3 - gkPipeZ2);
  return gkPipeR2 + slope * (z - gkPipeZ2);
}
/** ======================================================================= **/


/** ======================================================================= **/
TGeoVolume* ConstructFrameElement(const TString& name, TGeoVolume* frameBoxVol, Double_t x)
{
  // --- Material of the frames
  TGeoMedium* framesMaterial = gGeoMan->GetMedium("carbon");

  Double_t t = gkFrameThickness / 2.;

  // --- Main vertical pillars
  //    	TGeoBBox* frameVertPillarShp = new TGeoBBox(name + "_vertpillar_shape", t, gkFrameStep/2., t);  // square crossection, along y
  //	TGeoVolume* frameVertPillarVol = new TGeoVolume(name + "_vertpillar", frameVertPillarShp, framesMaterial);
  //	frameVertPillarVol->SetLineColor(kGreen);
  //	frameBoxVol->AddNode(frameVertPillarVol, 1, new TGeoTranslation(name + "_vertpillar_pos_1", x-t, 0., -(x+sqrt(2.)*t-2.*t)/2.));
  //	frameBoxVol->AddNode(frameVertPillarVol, 2, new TGeoTranslation(name + "_vertpillar_pos_2", -(x-t), 0., -(x+sqrt(2.)*t-2.*t)/2.));

  TGeoBBox* frameVertPillarShp;
  if (gkCylindricalFrames)
    //          TGeoBBox* frameVertPillarShp = new TGeoTube(name + "_vertpillar_shape", 0, t, gkFrameStep/2.);  // circle crossection, along z
    frameVertPillarShp = new TGeoTube(name + "_vertpillar_shape", gkCylinderDiaInner / 2., gkCylinderDiaOuter / 2.,
                                      gkFrameStep / 2.);  // circle crossection, along z
  else
    frameVertPillarShp = new TGeoBBox(name + "_vertpillar_shape", t, t,
                                      gkFrameStep / 2.);  // square crossection, along z
  TGeoVolume* frameVertPillarVol = new TGeoVolume(name + "_vertpillar", frameVertPillarShp, framesMaterial);
  frameVertPillarVol->SetLineColor(kGreen);

  TGeoRotation* xRot90 = new TGeoRotation;
  xRot90->RotateX(90.);
  frameBoxVol->AddNode(
    frameVertPillarVol, 1,
    new TGeoCombiTrans(name + "_vertpillar_pos_1", x - t, 0., -(x + sqrt(2.) * t - 2. * t) / 2., xRot90));
  frameBoxVol->AddNode(
    frameVertPillarVol, 2,
    new TGeoCombiTrans(name + "_vertpillar_pos_2", -(x - t), 0., -(x + sqrt(2.) * t - 2. * t) / 2., xRot90));

  //	TGeoRotation* vertRot = new TGeoRotation(name + "_vertpillar_rot_1", 90., 45., -90.);
  TGeoRotation* vertRot = new TGeoRotation;
  vertRot->RotateX(90.);
  vertRot->RotateY(45.);
  frameBoxVol->AddNode(frameVertPillarVol, 3,
                       new TGeoCombiTrans(name + "_vertpillar_pos_3", 0., 0., (x - sqrt(2.) * t) / 2., vertRot));

  // --- Small horizontal pillar
  TGeoBBox* frameHorPillarShp =
    new TGeoBBox(name + "_horpillar_shape", x - 2. * t, gkThinFrameThickness / 2., gkThinFrameThickness / 2.);
  TGeoVolume* frameHorPillarVol = new TGeoVolume(name + "_horpillar", frameHorPillarShp, framesMaterial);
  frameHorPillarVol->SetLineColor(kCyan);
  frameBoxVol->AddNode(frameHorPillarVol, 1,
                       new TGeoTranslation(name + "_horpillar_pos_1", 0., -gkFrameStep / 2. + gkThinFrameThickness / 2.,
                                           -(x + sqrt(2.) * t - 2. * t) / 2.));

  if (gkConstructSmallFrames) {

    // --- Small sloping pillar
    TGeoPara* frameSlopePillarShp =
      new TGeoPara(name + "_slopepillar_shape", (x - 2. * t) / TMath::Cos(31.4 / 180. * TMath::Pi()),
                   gkThinFrameThickness / 2., gkThinFrameThickness / 2., 31.4, 0., 90.);
    TGeoVolume* frameSlopePillarVol = new TGeoVolume(name + "_slopepillar", frameSlopePillarShp, framesMaterial);
    frameSlopePillarVol->SetLineColor(kCyan);
    TGeoRotation* slopeRot = new TGeoRotation(name + "_slopepillar_rot_1", 0., 0., 31.4);
    TGeoCombiTrans* slopeTrRot =
      new TGeoCombiTrans(name + "_slopepillar_posrot_1", 0., 0., -(x + sqrt(2.) * t - 2. * t) / 2., slopeRot);

    frameBoxVol->AddNode(frameSlopePillarVol, 1, slopeTrRot);

    Double_t angl = 23.;
    // --- Small sub pillar
    TGeoPara* frameSubPillarShp =
      new TGeoPara(name + "_subpillar_shape", (sqrt(2) * (x / 2. - t) - t / 2.) / TMath::Cos(angl / 180. * TMath::Pi()),
                   gkThinFrameThickness / 2., gkThinFrameThickness / 2., angl, 0., 90.);
    TGeoVolume* frameSubPillarVol = new TGeoVolume(name + "_subpillar", frameSubPillarShp, framesMaterial);
    frameSubPillarVol->SetLineColor(kMagenta);

    Double_t posZ = t * (1. - 3. / (2. * sqrt(2.)));

    // one side of X direction
    TGeoRotation* subRot1 = new TGeoRotation(name + "_subpillar_rot_1", 90., 45., -90. + angl);
    TGeoCombiTrans* subTrRot1 =
      new TGeoCombiTrans(name + "_subpillar_posrot_1", -(-x / 2. + t - t / (2. * sqrt(2.))), 1., posZ, subRot1);

    TGeoRotation* subRot2 = new TGeoRotation(name + "_subpillar_rot_2", 90., -90. - 45., -90. + angl);
    TGeoCombiTrans* subTrRot2 =
      new TGeoCombiTrans(name + "_subpillar_posrot_2", -(-x / 2. + t - t / (2. * sqrt(2.))), -1., posZ, subRot2);

    // other side of X direction
    TGeoRotation* subRot3 = new TGeoRotation(name + "_subpillar_rot_3", 90., 90. + 45., -90. + angl);
    TGeoCombiTrans* subTrRot3 =
      new TGeoCombiTrans(name + "_subpillar_posrot_3", -x / 2. + t - t / (2. * sqrt(2.)), 1., posZ, subRot3);

    TGeoRotation* subRot4 = new TGeoRotation(name + "_subpillar_rot_4", 90., -45., -90. + angl);
    TGeoCombiTrans* subTrRot4 =
      new TGeoCombiTrans(name + "_subpillar_posrot_4", -x / 2. + t - t / (2. * sqrt(2.)), -1., posZ, subRot4);

    frameBoxVol->AddNode(frameSubPillarVol, 1, subTrRot1);
    frameBoxVol->AddNode(frameSubPillarVol, 2, subTrRot2);
    frameBoxVol->AddNode(frameSubPillarVol, 3, subTrRot3);
    frameBoxVol->AddNode(frameSubPillarVol, 4, subTrRot4);
    //                frameBoxVol->GetShape()->ComputeBBox();
  }

  return frameBoxVol;
}
/** ======================================================================= **/
