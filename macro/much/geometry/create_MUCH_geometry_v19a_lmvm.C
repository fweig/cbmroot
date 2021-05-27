/** @file create_MUCH_geometry_v19a_lmvm.C
  * @copyright Copyright (C) 2019-2020 Department of Physics, Aligarh Muslim University, Aligarh
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Omveer Singh [orginator] **/

//
/// \file create_MUCH_geometry_v19a.C
/// \brief Generates MUCH geometry in Root format.
///
// 2019-02-14 - OS PPB AS  - v19a - Copper, G10 (PCBs) and Aluminum plates implimented.
// 2017-05-16 - DE  - v17b - position the modules in a way to split layers left-right along y axis
// 2017-05-16 - DE  - v17b - attribute name to module frames
// 2017-05-16 - DE  - v17b - remove rim from support CompositeShape
// 2017-05-02 - PPB - v17a - Change the shape of the first absorber according to latest design
// 2017-04-27 - DE  - v17a - fix GEM module positions and angles
// 2017-04-22 - PPB - v17a - Define the absorber, shield and station shapes sizes ...
// 2016-04-19 - DE  - v17a - initial version derived from TRD

// in root all sizes are given in cm

#include "TClonesArray.h"
#include "TDatime.h"
#include "TFile.h"
#include "TGeoBBox.h"
#include "TGeoCompositeShape.h"
#include "TGeoCone.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGeoPgon.h"
#include "TGeoTube.h"
#include "TGeoVolume.h"
#include "TGeoXtru.h"
#include "TList.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom3.h"
#include "TString.h"
#include "TSystem.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>


// Name of output file with geometry
const TString tagVersion   = "_v19a";
const TString subVersion   = "_sis100_1m_lmvm";
const TString geoVersion   = "much";  // + tagVersion + subVersion;
const TString FileNameSim  = geoVersion + tagVersion + subVersion + ".geo.root";
const TString FileNameGeo  = geoVersion + tagVersion + subVersion + "_geo.root";
const TString FileNameInfo = geoVersion + tagVersion + subVersion + ".geo.info";

// Names of the different used materials which are used to build the modules
// The materials are defined in the global media.geo file
const TString KeepingVolumeMedium = "air";
const TString L                   = "MUCHlead";
const TString W                   = "MUCHwolfram";
const TString C                   = "carbon";
const TString I                   = "MUCHiron";
const TString activemedium        = "MUCHargon";
const TString spacermedium        = "MUCHnoryl";
const TString supportmedium       = "aluminium";
const TString copper              = "copper";  //Drift
const TString g10                 = "G10";     //G10 Plate


// Universal input parameters

Double_t fMuchZ1           = 125.0;  // MuchCave Zin position [cm]
Double_t fAcceptanceTanMin = 0.1;    // Acceptance tangent min
Double_t fAcceptanceTanMax = 0.466;  // Acceptance tangent max

//************************************************************


// Input parameters for absorbers
//***********************************************************

const Int_t fNabs = 5;  // Number of absorbers
// Absorber Zin position [cm] in the cave reference frame
Double_t fAbsorberZ1[5] = {0, 16, 90, 140, 190};
// Absorber thickness [cm]
Double_t fAbsorberLz[5] = {16, 44, 20, 20, 30};
Double_t safetyrad[5]   = {0.0, 30.0, 30.0, 30.0, 30.0};


// Input parameters for MUCH stations
//********************************************

const Int_t fNst = 4;  // Number of stations
// Sector-type module parameters
// Number of sectors per layer (should be even for symmetry)
// Needs to be fixed with actual numbers
Int_t fNSectorsPerLayer[4] = {16, 20, 24, 28};
Double_t fActiveLzSector   = 0.3;  // Active volume thickness [cm]
Double_t fSpacerR          = 2.0;  // Spacer width in R [cm]
Double_t fSpacerPhi        = 2.0;  // Spacer width in Phi [cm]
Double_t fOverlapR         = 2.0;  // Overlap in R direction [cm]

// Station Zceneter [cm] in  the cave reference frame

Double_t fStationZ0[4] = {75, 125, 175, 235};
Int_t fNlayers[4]      = {3, 3, 3, 3};  // Number of layers
Int_t fDetType[4]      = {3, 3, 3, 3};  // Detector type
Double_t fLayersDz[4]  = {10, 10, 10, 10};
Double_t fSupportLz[4] = {1.0, 1.0, 1.0, 1.0};
Double_t fDriftDz      = 0.0035;  //35 micron copper Drift
Double_t fG10Dz        = 0.3;     // 3 mm G10

/* 
   1 - detailed design (modules at two sides)
   * 0 - simple design (1 module per layer) 
   */

Int_t fModuleDesign[4] = {1, 1, 1, 1};


// Input parameters for beam pipe shielding
// spans from 2.9 degree to 5.1 degree
//Inner radius is tan(2.9) + 2 cm, extra 20 mm for clamp connection

const Int_t fNshs                 = 3;
Double_t fShieldZin[3]            = {215.0, 265.0, 315.0};
Double_t fShieldLz[3]             = {20, 20, 30};
Double_t fShield_AcceptanceTanMin = 0.051;  // Acceptance tangent min for shield
Double_t fShield_AcceptanceTanMax = 0.1;    // Acceptance tangent max for shield


//***********************************************************

// some global variables
TGeoManager* gGeoMan = NULL;         // Pointer to TGeoManager instance
TGeoVolume* gModules[fNabs];         // Global storage for module types
TGeoVolume* gModules_shield[fNshs];  // Global storage for module types
TGeoVolume* gModules_station[fNst];  // Global storage for module types

// Forward declarations
void create_materials_from_media_file();
TGeoVolume* CreateAbsorbers(int iabs);
TGeoVolume* CreateShields(int ishield);
TGeoVolume* CreateStations(int ist);
TGeoVolume* CreateLayers(int istn, int ily);


void create_MUCH_geometry_v19a_lmvm()
{

  // Load needed material definition from media.geo file
  create_materials_from_media_file();

  // Get the GeoManager for later usage
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetVisLevel(10);

  // Create the top volume
  TGeoBBox* topbox = new TGeoBBox("", 1000., 1000., 2000.);
  TGeoVolume* top  = new TGeoVolume("top", topbox, gGeoMan->GetMedium("air"));
  gGeoMan->SetTopVolume(top);

  TString topName = geoVersion + tagVersion + subVersion;

  TGeoVolume* much = new TGeoVolumeAssembly(topName);
  top->AddNode(much, 1);

  TGeoVolume* absr = new TGeoVolumeAssembly("absorber");
  much->AddNode(absr, 1);

  TGeoVolume* shld = new TGeoVolumeAssembly("shield");
  much->AddNode(shld, 1);

  TGeoVolume* sttn = new TGeoVolumeAssembly("station");
  much->AddNode(sttn, 1);


  for (Int_t iabs = 0; iabs < fNabs; iabs++) {  // 6 pieces of absorbers
    // first abosrber is divided into two halves
    // first half inserted inside the dipole magnet

    gModules[iabs] = CreateAbsorbers(iabs);

    absr->AddNode(gModules[iabs], iabs);
  }


  for (Int_t ishi = 0; ishi < fNshs; ishi++) {

    gModules_shield[ishi] = CreateShields(ishi);

    shld->AddNode(gModules_shield[ishi], ishi);
  }


  for (Int_t istn = 0; istn < fNst; istn++) {  // 4 Stations


    gModules_station[istn] = CreateStations(istn);

    sttn->AddNode(gModules_station[istn], istn);
  }

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0000001);
  gGeoMan->PrintOverlaps();

  gGeoMan->CheckOverlaps(0.000001, "s");
  gGeoMan->PrintOverlaps();
  //  gGeoMan->Test();


  much->Export(FileNameSim);  // an alternative way of writing the much

  TFile* outfile                  = new TFile(FileNameSim, "UPDATE");
  TGeoTranslation* much_placement = new TGeoTranslation("much_trans", 0., 0., 0.);
  much_placement->Write();
  outfile->Close();

  outfile = new TFile(FileNameGeo, "RECREATE");
  gGeoMan->Write();  // use this if you want GeoManager format in the output
  outfile->Close();

  top->Draw("ogl");
}

void create_materials_from_media_file()
{
  // Use the FairRoot geometry interface to load the media which are already defined
  FairGeoLoader* geoLoad    = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath           = gSystem->Getenv("VMCWORKDIR");
  TString geoFile           = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(geoFile);
  geoFace->readMedia();

  // Read the required media and create them in the GeoManager
  FairGeoMedia* geoMedia   = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* air = geoMedia->getMedium(KeepingVolumeMedium);
  geoBuild->createMedium(air);

  FairGeoMedium* MUCHiron = geoMedia->getMedium(I);
  geoBuild->createMedium(MUCHiron);

  FairGeoMedium* MUCHlead = geoMedia->getMedium(L);
  geoBuild->createMedium(MUCHlead);

  FairGeoMedium* MUCHwolfram = geoMedia->getMedium(W);
  geoBuild->createMedium(MUCHwolfram);

  FairGeoMedium* MUCHcarbon = geoMedia->getMedium(C);
  geoBuild->createMedium(MUCHcarbon);

  FairGeoMedium* MUCHargon = geoMedia->getMedium(activemedium);
  geoBuild->createMedium(MUCHargon);

  FairGeoMedium* MUCHnoryl = geoMedia->getMedium(spacermedium);
  geoBuild->createMedium(MUCHnoryl);

  FairGeoMedium* MUCHsupport = geoMedia->getMedium(supportmedium);
  geoBuild->createMedium(MUCHsupport);

  FairGeoMedium* copperplate = geoMedia->getMedium(copper);  //Copper for Drift
  geoBuild->createMedium(copperplate);
  FairGeoMedium* g10plate = geoMedia->getMedium(g10);  //G10
  geoBuild->createMedium(g10plate);
}


TGeoVolume* CreateShields(int ish)
{

  TGeoMedium* graphite = gGeoMan->GetMedium(C);
  TGeoMedium* iron     = gGeoMan->GetMedium(I);

  TString name                    = Form("shieldblock%d", ish);
  TGeoVolumeAssembly* shieldblock = new TGeoVolumeAssembly(name);

  TString conename_sh = Form("conesh_%d", ish);


  Double_t dz       = fShieldLz[ish] / 2.0;
  Double_t globalZ1 = fShieldZin[ish];
  Double_t globalZ2 = fShieldZin[ish] + 2 * dz;


  Double_t rmin1 = globalZ1 * fShield_AcceptanceTanMin + 2.0;
  Double_t rmax1 = globalZ1 * fShield_AcceptanceTanMax - 0.0001;  //remove overlap
  Double_t rmin2 = globalZ2 * fShield_AcceptanceTanMin + 2.0;
  Double_t rmax2 = globalZ2 * fShield_AcceptanceTanMax - 0.0001;
  //cout<<" Shields:  rmin1 "<<rmin1<<" rmax 1  "<<rmax1<<"  rmin2 "<<rmin2<<"  rmax2  "<<rmax2<<endl;


  TGeoCone* sh       = new TGeoCone(conename_sh, dz, rmin1, rmax1, rmin2, rmax2);
  TGeoVolume* shield = new TGeoVolume("shield", sh, iron);

  shield->SetLineColor(kBlack);
  shield->SetTransparency(2);
  TGeoTranslation* sh_trans = new TGeoTranslation("", 0., 0., globalZ1 + dz);
  shieldblock->AddNode(shield, ish, sh_trans);


  return shieldblock;
}


TGeoVolume* CreateAbsorbers(int i)
{

  TGeoMedium* graphite = gGeoMan->GetMedium(C);
  TGeoMedium* iron     = gGeoMan->GetMedium(I);

  TString name                 = Form("absblock%d", i);
  TGeoVolumeAssembly* absblock = new TGeoVolumeAssembly(name);

  TString pipename         = Form("beampipe_%d", i);
  TString conename         = Form("cone_%d", i);
  TString BoxName          = Form("Box_%d", i);
  TString supportShapeName = Form("Support_%d", i);
  TString TrapName         = Form("Trap_%d", i);

  Double_t dz       = fAbsorberLz[i] / 2.0;
  Double_t globalZ1 = fAbsorberZ1[i] + fMuchZ1;
  Double_t globalZ2 = fAbsorberZ1[i] + 2 * dz + fMuchZ1;

  Double_t rmin1 = globalZ1 * fAcceptanceTanMin;
  Double_t rmin2 = globalZ2 * fAcceptanceTanMin;
  Double_t rmax1 = globalZ1 * fAcceptanceTanMax + safetyrad[i];
  Double_t rmax2 = globalZ2 * fAcceptanceTanMax + safetyrad[i];  //

  // 1st part of 1st absorber trapezium
  //dimensions are hardcoded
  if (i == 0) {
    printf("absorber %d \n", i);

    TGeoTrd2* trap                = new TGeoTrd2(TrapName, 70.0, 70.0, 46.0, 71.0, dz);
    TGeoCone* tube                = new TGeoCone(pipename, dz + 0.001, 0., rmin1, 0., rmin2);
    TString expression            = TrapName + "-" + pipename;
    TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName, expression);
    TGeoVolume* abs0              = new TGeoVolume("absorber", shSupport, graphite);
    abs0->SetLineColor(kRed);
    abs0->SetTransparency(2);
    TGeoTranslation* abs0_trans = new TGeoTranslation("", 0., 0., globalZ1 + dz);
    absblock->AddNode(abs0, i, abs0_trans);
  }

  // 2nd part of 1st absorber box
  if (i == 1) {
    printf("absorber %d \n", i);
    TGeoBBox* box                 = new TGeoBBox(BoxName, 130.0, 125.0, dz);
    TGeoCone* tube                = new TGeoCone(pipename, dz + 0.001, 0., rmin1, 0., rmin2);
    TString expression            = BoxName + "-" + pipename;
    TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName, expression);

    TGeoVolume* abs1 = new TGeoVolume("absorber", shSupport, graphite);
    abs1->SetLineColor(kBlue);
    abs1->SetTransparency(2);
    TGeoTranslation* abs1_trans = new TGeoTranslation("", 0., 0., globalZ1 + dz);
    absblock->AddNode(abs1, i, abs1_trans);
  }


  //rest of the absorbers
  if (!(i == 0 || i == 1)) {
    TGeoBBox* box                 = new TGeoBBox(BoxName, rmax2, rmax2, dz);
    TGeoCone* tube                = new TGeoCone(pipename, dz + 0.001, 0., rmin1, 0., rmin2);
    TString expression            = BoxName + "-" + pipename;
    TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName, expression);

    TGeoVolume* abs2 = new TGeoVolume("absorber", shSupport, iron);

    abs2->SetLineColor(kBlue);
    abs2->SetTransparency(2);


    TGeoTranslation* abs_trans = new TGeoTranslation("", 0., 0., globalZ1 + dz);
    absblock->AddNode(abs2, i, abs_trans);
    //  cout<<" Abosrber # "<<i<<"  z  "<<globalZ1+dz<<endl;
  }

  return absblock;
}

TGeoVolume* CreateStations(int ist)
{

  TString stationName = Form("muchstation%02i", ist + 1);

  TGeoVolumeAssembly* station = new TGeoVolumeAssembly(stationName);  //, shStation, air);


  TGeoVolume* gLayer[4];

  for (int ii = 0; ii < 3; ii++) {  // 3 Layers


    gLayer[ii] = CreateLayers(ist, ii);
    station->AddNode(gLayer[ii], ii);
  }

  return station;
}


TGeoVolume* CreateLayers(int istn, int ily)
{

  TString layerName = Form("muchstation%02ilayer%i", istn + 1, ily + 1);
  //cout<<" check          "<<layerName<<endl;
  TGeoVolumeAssembly* volayer = new TGeoVolumeAssembly(layerName);


  Double_t stGlobalZ0 = fStationZ0[istn] + fMuchZ1;  //z position of station center (midplane) [cm]
  Double_t stDz       = ((fNlayers[istn] - 1) * fLayersDz[istn] + fSupportLz[istn] + 2 * fActiveLzSector) / 2.;
  Double_t stGlobalZ2 = stGlobalZ0 + stDz;
  Double_t stGlobalZ1 = stGlobalZ0 - stDz;

  Double_t rmin = stGlobalZ1 * fAcceptanceTanMin;
  Double_t rmax = stGlobalZ2 * fAcceptanceTanMax;


  Double_t layerZ0       = (ily - (fNlayers[istn] - 1) / 2.) * fLayersDz[istn];
  Double_t layerGlobalZ0 = layerZ0 + stGlobalZ0;

  Double_t driftDz   = fActiveLzSector / 2. + fDriftDz / 2.;
  Double_t g10Dz     = driftDz + fDriftDz / 2. + fG10Dz / 2.;
  Double_t moduleZ   = fSupportLz[istn] / 2. + fActiveLzSector / 2. + fDriftDz + fG10Dz;
  Double_t driftZIn  = moduleZ - driftDz;
  Double_t driftZOut = moduleZ + driftDz;
  Double_t g10ZIn    = moduleZ - g10Dz;
  Double_t g10ZOut   = moduleZ + g10Dz;

  Double_t phi0 = TMath::Pi() / fNSectorsPerLayer[istn];  // azimuthal half widh of each module
  Double_t ymin = rmin + fSpacerR;
  Double_t ymax = rmax;

  //define the dimensions of the trapezoidal module
  Double_t dy  = (ymax - ymin) / 2.;                                      //y (length)
  Double_t dx1 = ymin * TMath::Tan(phi0) + fOverlapR / TMath::Cos(phi0);  // large x
  Double_t dx2 = ymax * TMath::Tan(phi0) + fOverlapR / TMath::Cos(phi0);  // small x
  Double_t dz  = fActiveLzSector / 2.;                                    // thickness


  //define the spacer dimensions
  Double_t tg   = (dx2 - dx1) / 2 / dy;
  Double_t dd1  = fSpacerPhi * tg;
  Double_t dd2  = fSpacerPhi * sqrt(1 + tg * tg);
  Double_t sdx1 = dx1 + dd2 - dd1 - 0.1;  // 0.1 cm to avoid overlaps
  Double_t sdx2 = dx2 + dd2 + dd1;
  Double_t sdy  = dy + fSpacerR;
  Double_t sdz  = dz - 0.1;

  //define Additional material as realistic GEM module
  Double_t dz_sD = fDriftDz / 2.;  //35 micron copper Drift
  Double_t dz_sG = fG10Dz / 2.;    // 3mm G10

  const Int_t Nsector = fNSectorsPerLayer[istn];
  TGeoVolume* gsector[Nsector];

  Double_t pos[7];
  TVector3 size = TVector3(0.0, 0.0, fActiveLzSector);

  // Add the support structure
  // Create support

  Double_t supportDx = sqrt(rmax * rmax + dx2 * dx2);
  Double_t supportDy = sqrt(rmax * rmax + dx2 * dx2);
  Double_t supportDz = fSupportLz[istn] / 2.;


  TString supportBoxName   = Form("shStation%02iSupportBox", istn + 1);
  TString supportHoleName  = Form("shStation%02iSupportHole", istn + 1);
  TString translationName  = Form("trSt%02i", istn + 1);
  TString supportShapeName = Form("shSt%02iSupport", istn + 1);

  TGeoTube* shSupportHole = new TGeoTube(supportHoleName, 0., rmin, supportDz + 0.001);
  TGeoBBox* shSupportBox  = new TGeoBBox(supportBoxName, supportDx, supportDy, supportDz);

  TString expression            = supportBoxName + "-" + supportHoleName;
  TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName, expression);

  TString supportName1 = Form("muchstation%02ilayer%isupport", istn + 1, ily + 1);
  TGeoMedium* coolMat  = gGeoMan->GetMedium(supportmedium);

  TGeoVolume* voSupport1 = new TGeoVolume(supportName1, shSupport, coolMat);
  voSupport1->SetLineColor(kCyan);

  TGeoTranslation* support_trans1 = new TGeoTranslation("supportName1", 0, 0, layerGlobalZ0);
  volayer->AddNode(voSupport1, 0, support_trans1);


  Int_t iMod = 0;
  for (Int_t iSide = 0; iSide < 2; iSide++) {

    // Now start adding the GEM modules
    for (Int_t iModule = 0; iModule < fNSectorsPerLayer[istn]; iModule++) {

      Double_t phi  = 2 * phi0 * (iModule + 0.5);  // add 0.5 to not overlap with y-axis for left-right layer separation
      Bool_t isBack = iModule % 2;
      Char_t cside  = (isBack == 1) ? 'b' : 'f';

      // correct the x, y positions
      pos[0] = -(ymin + dy) * sin(phi);
      pos[1] = (ymin + dy) * cos(phi);

      // different z positions for odd/even modules
      pos[2] = (isBack ? 1 : -1) * moduleZ + layerGlobalZ0;
      pos[3] = (isBack ? 1 : -1) * driftZIn + layerGlobalZ0;
      pos[4] = (isBack ? 1 : -1) * driftZOut + layerGlobalZ0;
      pos[5] = (isBack ? 1 : -1) * g10ZIn + layerGlobalZ0;
      pos[6] = (isBack ? 1 : -1) * g10ZOut + layerGlobalZ0;

      //cout<<pos[2]<<"  "<<pos[3]<<"  "<<pos[4]<<"  "<<pos[5]<<"   "<<pos[6]<<endl;
      if (iSide != isBack) continue;
      if (iModule != 0) iMod = iModule / 2;


      TGeoMedium* argon       = gGeoMan->GetMedium(activemedium);  // active medium
      TGeoMedium* noryl       = gGeoMan->GetMedium(spacermedium);  // spacer medium
      TGeoMedium* copperplate = gGeoMan->GetMedium(copper);        // copper Drift medium
      TGeoMedium* g10plate    = gGeoMan->GetMedium(g10);           // G10 medium


      // Define and place the trapezoidal GEM module in X-Y plane
      TGeoTrap* shape = new TGeoTrap(dz, 0, 0, dy, dx1, dx2, 0, dy, dx1, dx2, 0);
      shape->SetName(Form("shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule));
      TString activeName   = Form("muchstation%02ilayer%i%cactive%03igasArgon", istn + 1, ily + 1, cside, iMod + 1);
      TGeoVolume* voActive = new TGeoVolume(activeName, shape, argon);
      voActive->SetLineColor(kGreen);

      TGeoTrap *Drift[2], *G10[2];
      TString DriftName[2], G10Name[2];
      TGeoVolume *voDrift[2], *voG10[2];

      for (int iPos = 0; iPos < 2; iPos++) {
        Char_t cpos = (iPos == 0) ? 'i' : 'o';
        // Define and place copper drift
        Drift[iPos] = new TGeoTrap(dz_sD, 0, phi, sdy, sdx1, sdx2, 0, sdy, sdx1, sdx2, 0);
        Drift[iPos]->SetName(Form("shStation%02iLayer%i%cModule%03i%cDrift", istn, ily, cside, iModule, cpos));
        DriftName[iPos] =
          Form("muchstation%02ilayer%i%cside%03i%ccopperDrift", istn + 1, ily + 1, cside, iMod + 1, cpos);
        voDrift[iPos] = new TGeoVolume(DriftName[iPos], Drift[iPos], copperplate);
        voDrift[iPos]->SetLineColor(kRed);

        // Define and place G10
        G10[iPos] = new TGeoTrap(dz_sG, 0, phi, sdy, sdx1, sdx2, 0, sdy, sdx1, sdx2, 0);
        G10[iPos]->SetName(Form("shStation%02iLayer%i%cModule%03i%cG10", istn, ily, cside, iModule, cpos));
        G10Name[iPos] = Form("muchstation%02ilayer%i%cside%03i%cG10", istn + 1, ily + 1, cside, iMod + 1, cpos);
        voG10[iPos]   = new TGeoVolume(G10Name[iPos], G10[iPos], g10plate);
        voG10[iPos]->SetLineColor(28);
      }

      // Define the trapezoidal spacers
      TGeoTrap* shapeFrame = new TGeoTrap(sdz, 0, 0, sdy, sdx1, sdx2, 0, sdy, sdx1, sdx2, 0);
      shapeFrame->SetName(Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole", istn, ily, cside, iModule));
      expression                  = Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole-"
                        "shStation%02iLayer%i%cModule%03iActiveNoHole",
                        istn, ily, cside, iModule, istn, ily, cside, iModule);
      TGeoCompositeShape* shFrame = new TGeoCompositeShape(
        Form("shStation%02iLayer%i%cModule%03iFrameNoHole", istn, ily, cside, iModule), expression);
      TString frameName   = Form("muchstation%02ilayer%i%cframe%03i", istn + 1, ily + 1, cside, iMod + 1);
      TGeoVolume* voFrame = new TGeoVolume(frameName, shFrame, noryl);  // add a name to the frame
      voFrame->SetLineColor(kMagenta);


      // Calculate the phi angle of the sector where it has to be placed
      Double_t angle = 180. / TMath::Pi() * phi;  // convert angle phi from rad to deg


      TGeoRotation* r2 = new TGeoRotation("r2");
      //rotate in the vertical plane (per to z axis) with angle
      r2->RotateZ(angle);


      TGeoTranslation* trans[5];
      TGeoHMatrix* incline_mod[5];

      for (int i = 0; i < 5; i++) {
        trans[i] = new TGeoTranslation("", pos[0], pos[1], pos[i + 2]);

        incline_mod[i]    = new TGeoHMatrix("");
        (*incline_mod[i]) = (*trans[i]) * (*r2);
      }

      volayer->AddNode(voActive, iMod, incline_mod[0]);    // add active volume
      volayer->AddNode(voFrame, iMod, incline_mod[0]);     // add spacer
      volayer->AddNode(voDrift[0], iMod, incline_mod[1]);  //Drift In
      volayer->AddNode(voDrift[1], iMod, incline_mod[2]);  //Drift Out
      volayer->AddNode(voG10[0], iMod, incline_mod[3]);    //G10 In
      volayer->AddNode(voG10[1], iMod, incline_mod[4]);    //G10 Out
    }
  }

  return volayer;
}
