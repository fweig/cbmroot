/* Copyright (C) 2020-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Omveer Singh, Florian Uhlig [committer] */

/*** @author Omveer Singh <omvir.ch@gmail.com>
 ** @date 17 May 2020
 ** For more details see info file*/

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

TObjArray* fStations = new TObjArray();
CbmMuchStation* muchSt;
CbmMuchLayer* muchLy;
CbmMuchLayerSide* muchLySd;

// Name of output file with geometry
const TString tagVersion = "_v22d";
// const TString subVersion   = "_sis100_1m_lmvm";
const TString geoVersion   = "much";  // + tagVersion + subVersion;
const TString FileNameSim  = geoVersion + tagVersion + "_mcbm.geo.root";
const TString FileNameGeo  = geoVersion + tagVersion + "_mcbm_geo.root";
const TString FileNameInfo = geoVersion + tagVersion + "_mcbm.geo.info";
const TString FileNamePar  = geoVersion + tagVersion + "_mcbm.par.root";

// Names of the different used materials which are used to build the modules
// The materials are defined in the global media.geo file
const TString KeepingVolumeMedium = "air";
const TString activemedium        = "MUCHargon";
const TString spacermedium        = "MUCHnoryl";
const TString Al                  = "aluminium";  // Al for cooling & support purpose
const TString copper              = "copper";
const TString g10                 = "G10";

// Input parameters for MUCH stations
//********************************************

const Int_t fNst = 1;  // Number of stations

Int_t fNlayers        = 2;              // Number of layers
Double_t fLayersZ0[2] = {65.7, 118.7};  // Layers Positions
// Double_t fLayersZ0[2]  = {85.7, 118.7};   // Layers Positions
Int_t fDetType[2]      = {3, 3};          // Detector type
Double_t fX[2]         = {7.2, 7.2};      // Placement of modules in X [cm]
Double_t fY[2]         = {24.53, 24.53};  // Placement of modules in Y [cm]
Double_t fSupportLz[2] = {1., 1.};        // (cooling + support)
Double_t fDriftDz      = 0.0035;          // 35 micron copper Drift
Double_t fG10Dz        = 0.30;            // 3 mm G10

Double_t fActiveLzSector[2] = {0.3, 0.3};  // Active volume thickness [cm]
Double_t fFrameLzSector[2]  = {.3, .3};    // Frame thickness [cm]
Double_t fSpacerPhi         = 2.0;         // Spacer width in Phi [cm]
Double_t fOverlapR          = 2.0;         // Overlap in R direction [cm]
Double_t fSpacerR1          = 2.8;         // Spacer width in R at low Z[cm]
Double_t fSpacerR2          = 3.5;         // Spacer width in R at high Z[cm]

// Size of Modules
// GEM
Double_t dy  = 40.0;
Double_t dx1 = 3.75;
Double_t dx2 = 20;

//***********************************************************

// some global variables
TGeoManager* gGeoMan = NULL;         // Pointer to TGeoManager instance
TGeoVolume* gModules_station[fNst];  // Global storage for module types

// Forward declarations
void create_materials_from_media_file();
TGeoVolume* CreateStations(int ist);
TGeoVolume* CreateLayers(int istn, int ily);

fstream infoFile;
void create_MUCH_geometry_v22d_mcbm()
{

  // -------   Open info file   -----------------------------------------------
  TString infoFileName = FileNameSim;
  infoFileName.ReplaceAll("root", "info");
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "MUCH geometry created with create_MUCH_geometry_v22d_mcbm.C" << endl << endl;
  infoFile << "Build a mMUCH setup for mCBM with 1 GEM." << endl;
  infoFile << "10 mm thick Al plates are used for support and cooling in the "
              "GEM modules."
           << endl;
  infoFile << "Drift and read-out PCBs (copper coated G10 plates) inserted for "
              "realistic material budget for both GEM and RPC modules."
           << endl
           << endl;
  infoFile << "No of Modules: " << fNlayers << "      ( GEM )" << endl;
  infoFile << "Position of Modules Z [cm]: ";
  for (int i = 0; i < fNlayers; i++)
    infoFile << fLayersZ0[i] << "    ";

  infoFile << endl << endl << "Placement of Modules:" << endl;
  infoFile << "Module"
           << "\t"
           << "X [cm]"
           << "\t"
           << "Y [cm]"
           << "\t" << endl;
  infoFile << "----------------------" << endl;
  for (int i = 0; i < fNlayers; i++)
    infoFile << " " << i + 1 << "\t" << fX[i] << "\t" << fY[i] << endl;
  infoFile << "----------------------" << endl;
  infoFile << endl << "Al Cooling Plate Thickness [cm]: ";
  for (int i = 0; i < fNlayers; i++)
    infoFile << fSupportLz[i] << "    ";

  infoFile << endl << "Active Volume Thickness [cm]: ";
  for (int i = 0; i < fNlayers; i++)
    infoFile << fActiveLzSector[i] << "    ";

  infoFile << endl << endl << endl << " GEM Module:" << endl;
  infoFile << "                " << 2 * dx1 << " cm" << endl;
  infoFile << "               .........                 |" << endl;
  infoFile << "              ...........                |" << endl;
  infoFile << "             .............               |" << endl;
  infoFile << "            ...............              |" << endl;
  infoFile << "           .................             |" << endl;
  infoFile << "          ...................            |" << endl;
  infoFile << "         .....................           |" << endl;
  infoFile << "        .......................         " << 2 * dy << " cm" << endl;
  infoFile << "       .........................         |" << endl;
  infoFile << "      ...........................        |" << endl;
  infoFile << "     .............................       |" << endl;
  infoFile << "    ...............................      |" << endl;
  infoFile << "   .................................     |" << endl;
  infoFile << "  ...................................    |" << endl;
  infoFile << " .....................................   |" << endl;
  infoFile << ".......................................  |" << endl;
  infoFile << "                " << 2 * dx2 << " cm" << endl;
  infoFile << endl;

  // Load needed material definition from media.geo file
  create_materials_from_media_file();

  // Get the GeoManager for later usage
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetVisLevel(10);

  // Create the top volume
  TGeoBBox* topbox = new TGeoBBox("", 1000., 1000., 2000.);
  TGeoVolume* top  = new TGeoVolume("top", topbox, gGeoMan->GetMedium("air"));
  gGeoMan->SetTopVolume(top);

  TString topName = geoVersion + tagVersion + "_mcbm";

  TGeoVolume* much = new TGeoVolumeAssembly(topName);
  top->AddNode(much, 1);

  TGeoVolume* sttn = new TGeoVolumeAssembly("station");
  much->AddNode(sttn, 1);

  for (Int_t istn = 0; istn < 1; istn++) {

    Double_t stGlobalZ0 = (fLayersZ0[0] + fLayersZ0[1]) / 2.;
    muchSt              = new CbmMuchStation(istn, stGlobalZ0);
    muchSt->SetRmin(0.);
    muchSt->SetRmax(0.);
    fStations->Add(muchSt);

    gModules_station[istn] = CreateStations(istn);
    sttn->AddNode(gModules_station[istn], istn);
  }

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0001, "s");
  gGeoMan->PrintOverlaps();

  much->Export(FileNameSim);  // an alternative way of writing the much

  TFile* outfile                  = new TFile(FileNameSim, "UPDATE");
  TGeoTranslation* much_placement = new TGeoTranslation("much_trans", -6., 0., 0.);
  much_placement->Write();
  outfile->Close();

  outfile = new TFile(FileNameGeo, "RECREATE");
  gGeoMan->Write();  // use this if you want GeoManager format in the output
  outfile->Close();

  top->Draw("ogl");
  infoFile.close();

  TFile* parfile = new TFile(FileNamePar, "RECREATE");
  fStations->Write("stations", 1);  // for geometry parameters
  parfile->Close();
}

void create_materials_from_media_file()
{
  // Use the FairRoot geometry interface to load the media which are already
  // defined
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

  FairGeoMedium* MUCHargon = geoMedia->getMedium(activemedium);
  geoBuild->createMedium(MUCHargon);

  FairGeoMedium* MUCHnoryl = geoMedia->getMedium(spacermedium);
  geoBuild->createMedium(MUCHnoryl);

  FairGeoMedium* MUCHsupport = geoMedia->getMedium(Al);
  geoBuild->createMedium(MUCHsupport);

  FairGeoMedium* copperplate = geoMedia->getMedium(copper);
  geoBuild->createMedium(copperplate);

  FairGeoMedium* g10plate = geoMedia->getMedium(g10);
  geoBuild->createMedium(g10plate);
}

TGeoVolume* CreateStations(int ist)
{

  TString stationName         = Form("muchstation%02i", ist + 1);
  TGeoVolumeAssembly* station = new TGeoVolumeAssembly(stationName);  //, shStation, air);

  TGeoVolume* gLayer[fNlayers + 1];
  for (int ii = 0; ii < 1; ii++) {  // 1 Layers

    Double_t sideDz = fSupportLz[ii] / 2. + fActiveLzSector[ii] / 2.;  // distance between side's and layer's centers

    muchLy = new CbmMuchLayer(ist, ii, fLayersZ0[ii], 0.);
    muchSt->AddLayer(muchLy);
    muchLy->GetSideF()->SetZ(fLayersZ0[ii] - sideDz);

    gLayer[ii] = CreateLayers(ist, ii);
    station->AddNode(gLayer[ii], ii);
  }

  return station;
}

TGeoVolume* CreateLayers(int istn, int ily)
{

  TString layerName = Form("muchstation%02ilayer%i", istn + 1, ily + 1);

  TGeoVolumeAssembly* volayer = new TGeoVolumeAssembly(layerName);

  Double_t SupportDz = fSupportLz[ily] / 2.;
  Double_t driftDz   = fActiveLzSector[ily] / 2 + fDriftDz / 2.;
  Double_t g10Dz     = driftDz + fDriftDz / 2. + fG10Dz / 2.;

  Double_t moduleZ   = fLayersZ0[ily];
  Double_t driftZIn  = moduleZ - driftDz;
  Double_t driftZOut = moduleZ + driftDz;
  Double_t g10ZIn    = moduleZ - g10Dz;
  Double_t g10ZOut   = moduleZ + g10Dz;
  Double_t cool_z    = g10ZIn - fG10Dz / 2. - SupportDz;

  Double_t dz = fActiveLzSector[ily] / 2.;  // Active Volume Thickness

  Int_t Nsector = 16.0;
  Double_t phi0 = TMath::Pi() / Nsector;  // azimuthal half widh of each module

  // define the spacer dimensions
  Double_t tg   = (dx2 - dx1) / 2 / dy;
  Double_t dd1  = fSpacerPhi * tg;
  Double_t dd2  = fSpacerPhi * sqrt(1 + tg * tg);
  Double_t sdx1 = dx1 + dd2 - dd1;
  Double_t sdx2 = dx2 + dd2 + dd1;
  Double_t sdy1 = dy + fSpacerR1;  // frame width added
  Double_t sdy2 = dy + fSpacerR2;  // frame width added
  Double_t sdz  = fFrameLzSector[ily] / 2.;

  // define Additional material as realistic GEM module
  Double_t dz_sD = fDriftDz / 2.;  // 35 micron copper Drift
  Double_t dz_sG = fG10Dz / 2.;    // 3mm G10
  Double_t sdy   = sdy2;

  Double_t pos[10];

  Int_t iMod = 0;
  for (Int_t iSide = 0; iSide < 1; iSide++) {
    muchLySd = muchLy->GetSide(iSide);
    // Now start adding the GEM modules
    for (Int_t iModule = 0; iModule < 1; iModule++) {

      Double_t phi = 0;  // add 0.5 to not overlap with y-axis for left-right
                         // layer separation
      Bool_t isBack = iModule % 2;
      Char_t cside  = (isBack == 1) ? 'b' : 'f';

      // correct the x, y positions
      pos[0] = fX[ily];
      pos[1] = fY[ily];

      // different z positions for odd/even modules
      pos[2] = moduleZ;
      pos[3] = driftZIn;
      pos[4] = driftZOut;
      pos[5] = g10ZIn;
      pos[6] = g10ZOut;
      pos[7] = cool_z;

      if (iSide != isBack) continue;
      if (iModule != 0) iMod = iModule / 2;

      muchLySd = muchLy->GetSide(iSide);

      TGeoMedium* argon       = gGeoMan->GetMedium(activemedium);  // active medium
      TGeoMedium* noryl       = gGeoMan->GetMedium(spacermedium);  // spacer medium
      TGeoMedium* copperplate = gGeoMan->GetMedium(copper);        // copper Drift medium
      TGeoMedium* g10plate    = gGeoMan->GetMedium(g10);           // G10 medium
      TGeoMedium* coolMat     = gGeoMan->GetMedium(Al);

      // Define and place the shape of the modules
      TGeoTrap* shapeGem = new TGeoTrap(dz, 0, 0, dy, dx1, dx2, 0, dy, dx1, dx2, 0);
      shapeGem->SetName(Form("shStation%02iLayer%i%cModule%03iActiveGemNoHole", istn, ily, cside, iModule));

      //------------------------------------------------------Al Cooling
      // Plate--------------------------------------------------------------------
      TGeoVolume* voActive;
      TGeoTrap* coolGem;
      TGeoVolume* voCool;

      coolGem = new TGeoTrap(SupportDz, 0, phi, sdy2, sdx1, sdx2, 0, sdy2, sdx1, sdx2, 0);
      coolGem->SetName(Form("shStation%02iLayer%i%cModule%03icoolGem", istn, ily, cside, iModule));
      TString CoolName = Form("muchstation%02ilayer%i%ccoolGem%03iAluminum", istn + 1, ily + 1, cside, iModule + 1);
      voCool           = new TGeoVolume(CoolName, coolGem, coolMat);
      voCool->SetLineColor(kYellow);

      //------------------------------------------------------Active
      // Volume-----------------------------------------------------
      // GEM
      TGeoTrap* shapeActive = new TGeoTrap(dz, 0, 0, dy, dx1, dx2, 0, dy, dx1, dx2, 0);
      shapeActive->SetName(Form("shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule));
      TString activeName = Form("muchstation%02ilayer%i%cactive%03igasArgon", istn + 1, ily + 1, cside, iMod + 1);
      voActive           = new TGeoVolume(activeName, shapeActive, argon);
      voActive->SetLineColor(kGreen);

      //---------------------------------------------------------Drift &
      // PCB's---------------------------------------------------------------

      TString DriftName[2], G10Name[2], AlName;
      TGeoVolume *voDrift[2], *voG10[2];
      TGeoTrap *DriftGem[2], *G10Gem[2];

      for (int iPos = 0; iPos < 2; iPos++) {
        Char_t cpos    = (iPos == 0) ? 'i' : 'o';
        DriftGem[iPos] = new TGeoTrap(dz_sD, 0, phi, sdy, sdx1, sdx2, 0, sdy, sdx1, sdx2, 0);
        G10Gem[iPos]   = new TGeoTrap(dz_sG, 0, phi, sdy, sdx1, sdx2, 0, sdy, sdx1, sdx2, 0);
        DriftGem[iPos]->SetName(Form("shStation%02iLayer%i%cModule%03i%cDrift", istn, ily, cside, iModule, cpos));
        DriftName[iPos] =
          Form("muchstation%02ilayer%i%cside%03i%ccopperDrift", istn + 1, ily + 1, cside, iMod + 1, cpos);
        G10Gem[iPos]->SetName(Form("shStation%02iLayer%i%cModule%03i%cG10", istn, ily, cside, iModule, cpos));
        G10Name[iPos] = Form("muchstation%02ilayer%i%cside%03i%cG10", istn + 1, ily + 1, cside, iMod + 1, cpos);
        voDrift[iPos] = new TGeoVolume(DriftName[iPos], DriftGem[iPos], copperplate);
        voDrift[iPos]->SetLineColor(kRed);
        voG10[iPos] = new TGeoVolume(G10Name[iPos], G10Gem[iPos], g10plate);
        voG10[iPos]->SetLineColor(28);
      }
      //------------------------------------------------------------Frame-----------------------------------------------------------------
      // Define the trapezoidal Frame
      TGeoTrap* shapeFrame = new TGeoTrap(sdz, 0, 0, sdy, sdx1, sdx2, 0, sdy, sdx1, sdx2, 0);
      shapeFrame->SetName(Form("shStation%02iLayer%i%cModule%03iFullFrameGemNoHole", istn, ily, cside, iModule));

      TString expression = Form("shStation%02iLayer%i%cModule%03iFullFrameGemNoHole-shStation%"
                                "02iLayer%i%cModule%03iActiveGemNoHole",
                                istn, ily, cside, iModule, istn, ily, cside, iModule);

      TGeoCompositeShape* shFrame = new TGeoCompositeShape(
        Form("shStation%02iLayer%i%cModule%03iFinalFrameNoHole", istn, ily, cside, iModule), expression);
      TString frameName = Form("muchstation%02ilayer%i%csupport%03i", istn + 1, ily + 1, cside, iMod + 1);

      TGeoVolume* voFrame = new TGeoVolume(frameName, shFrame, noryl);  // Frame for GEM
      voFrame->SetLineColor(kMagenta);

      //----------------------------------------------------Placement----------------------------------------------------------------------
      // Calculate the phi angle of the sector where it has to be placed
      Double_t angle = 180.0 - (180. / TMath::Pi() * phi0);  // convert angle phi from rad to deg

      TGeoRotation* r2 = new TGeoRotation("r2");
      // rotate in the vertical plane (per to z axis) with angle
      r2->RotateZ(angle);

      TGeoTranslation* trans[10];
      TGeoHMatrix* incline_mod[10];

      for (int i = 0; i < 6; i++) {
        trans[i] = new TGeoTranslation("", pos[0], pos[1], pos[i + 2]);

        incline_mod[i]    = new TGeoHMatrix("");
        (*incline_mod[i]) = (*trans[i]) * (*r2);
      }

      volayer->AddNode(voFrame, iMod, incline_mod[0]);     // add spacer
      volayer->AddNode(voActive, iMod, incline_mod[0]);    // add active volume
      volayer->AddNode(voDrift[0], iMod, incline_mod[1]);  // Drift In
      volayer->AddNode(voDrift[1], iMod, incline_mod[2]);  // Drift Out
      volayer->AddNode(voG10[0], iMod, incline_mod[3]);    // G10 In
      volayer->AddNode(voG10[1], iMod, incline_mod[4]);    // G10 Out
      volayer->AddNode(voCool, iMod, incline_mod[5]);      // Al Cooling Plate

      TVector3 Position;
      Position.SetXYZ(pos[0], pos[1], pos[2]);
      cout << pos[2] << "   " << pos[3] << "   " << pos[4] << "   " << pos[5] << "   " << pos[6] << "   " << pos[7]
           << endl;
      muchLySd->AddModule(new CbmMuchModuleGemRadial(fDetType[ily], istn, ily, iSide, iModule, Position, 2 * dx1,
                                                     2 * dx2, 2 * dy, 2 * dz, muchSt->GetRmin()));
    }
  }

  return volayer;
}
