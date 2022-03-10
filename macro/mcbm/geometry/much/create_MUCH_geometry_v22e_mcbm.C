/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Omveer Singh, Shreya Roy, Florian Uhlig [committer] */

/*** @author Omveer Singh <omvir.ch@gmail.com>
 ** @date 17 May 2020
 ** For more details see info file*/

// clang-format off

// 2022-03-10 - DE - v22e - apply -2 degree rotation around the x-axis on the RPC module

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
const TString tagVersion = "_v22e";
//const TString subVersion   = "_sis100_1m_lmvm";
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
const TString Al                  = "aluminium";  //Al for cooling & support purpose
const TString copper              = "copper";
const TString g10                 = "G10";
const TString RPCm= "RPCgas";
const TString RPCg= "RPCglass";

// Input parameters for MUCH stations
//********************************************

const Int_t fNst = 3;  // Number of stations
Double_t TOF_Z_Front_Stand = 244.5;  // this is the TOF box front
//Double_t TOF_Z_Front_Stand = 247.;  // this is the TOF RPC front
Double_t RpcDistanceFromTof = 7.+1.9;  // add half thickness of RPC - gap is between backside of RPC and front of TOF
//Double_t RpcDistanceFromTof = 7.;
Double_t RpcPositionZ = TOF_Z_Front_Stand - RpcDistanceFromTof;
Int_t fNlayers         = 1;               // Number of layers
Double_t fLayersZ0[fNst]  = {75.7, 108.7, RpcPositionZ};   // Layers Positions
//Double_t fLayersZ0[fNst]  = {65.7, 88.7, RpcPositionZ};   // Layers Positions
Int_t fDetType[fNst]      = {3, 3, 4};          // Detector type
Double_t fSupportLz[fNst] = {1., 1., 0};        // (cooling + support)
Double_t fDriftDz      = 0.0035;          //35 micron copper Drift
Double_t fG10Dz        = 0.30;            // 3 mm G10

Double_t fActiveLzSector[fNst] = {0.3, 0.3, 0.2};  // Active volume thickness [cm]
Double_t fFrameLzSector[fNst]  = {.3, .3, .6};    // Frame thickness [cm]
Double_t fRpcGlassDz[fNst] = {0.0,0.0,0.2}; //Rpc Glass thickness [cm]
Double_t fSpacerPhi         = 2.0;         // Spacer width in Phi [cm]
Double_t fOverlapR          = 2.0;         // Overlap in R direction [cm]
Double_t fSpacerR1          = 2.8;         // Spacer width in R at low Z[cm]
Double_t fSpacerR2          = 3.5;         // Spacer width in R at high Z[cm]

//Size of Modules
//GEM
Double_t dy  = 40.0;
Double_t dx1 = 3.75;
Double_t dx2 = 20;


//RPC
Double_t dyR  = 116.13/2.; 
Double_t dx1R = 10.26/2.;
Double_t dx2R = 50.98/2.;

//box size
Double_t dxx  = 74.0/2. ; 
Double_t dyy = 123.0/2.;
Double_t dzz = 3.8/2.;


Double_t DetectorAxisHeight = 200.; //reference ground
Double_t BoxBaseHeight = 153.5 ;
Double_t BoxCentreHeight = BoxBaseHeight + dyy;
Double_t BoxAwayBeamAxisY = BoxCentreHeight - DetectorAxisHeight;


Double_t RpcBaseHeight = BoxBaseHeight + 4.5 ;
Double_t RpcCentreHeight = RpcBaseHeight + dyR;
Double_t RpcAwayBeamAxisY = RpcCentreHeight - DetectorAxisHeight;

//Position of Box
Double_t posBox[]={0,BoxAwayBeamAxisY}; //from the detector axis


Double_t fX[fNst]         = {7.2, 7.2, 0.};      //Placement of modules in X [cm]
Double_t fY[fNst]         = {24.53, 24.53, RpcAwayBeamAxisY};  //Placement of modules in Y [cm]

//***********************************************************

// some global variables
TGeoManager* gGeoMan = NULL;         // Pointer to TGeoManager instance
TGeoVolume* gModules_station[fNst];  // Global storage for module types

// Forward declarations
void create_materials_from_media_file();
TGeoVolume* CreateStations(int ist);
TGeoVolume* CreateLayersGem(int istn, int ily);
TGeoVolume* CreateLayersRpc(int istn, int ily);


fstream infoFile;
void create_MUCH_geometry_v22e_mcbm()
{


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = FileNameSim;
  infoFileName.ReplaceAll("root", "info");
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "MUCH geometry created with create_MUCH_geometry_v20a_mcbm.C" << endl << endl;
  infoFile << "Build a mMUCH setup for mCBM with 2 GEM." << endl;
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


  for (Int_t istn = 0; istn < fNst; istn++) {

    Double_t stGlobalZ0 = fLayersZ0[istn];
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

  FairGeoMedium* RPCmedium = geoMedia->getMedium(RPCm);
  geoBuild->createMedium(RPCmedium);

  FairGeoMedium* RPCmaterial = geoMedia->getMedium(RPCg);
  geoBuild->createMedium(RPCmaterial);

}

TGeoVolume* CreateStations(int ist)
{

  TString stationName         = Form("muchstation%02i", ist + 1);
  TGeoVolumeAssembly* station = new TGeoVolumeAssembly(stationName);  //, shStation, air);

  TGeoVolume* gLayer[fNlayers + 1];
  for (int ii = 0; ii < fNlayers; ii++) {  // 2 Layers

   // Double_t sideDz = fSupportLz[istn] / 2. + fActiveLzSector[istn] / 2.;  // distance between side's and layer's centers

    muchLy = new CbmMuchLayer(ist, ii, fLayersZ0[ist], 0.);
    muchSt->AddLayer(muchLy);
    muchLy->GetSideF()->SetZ(fLayersZ0[ist] );  


 if(ist<2)gLayer[ii] = CreateLayersGem(ist, ii);
   else gLayer[ii] = CreateLayersRpc(ist, ii);
    station->AddNode(gLayer[ii], ii);
  }

  return station;
}

TGeoVolume* CreateLayersGem(int istn, int ily)
{

  TString layerName = Form("muchstation%02ilayer%i", istn + 1, ily + 1);


  TGeoVolumeAssembly* volayer = new TGeoVolumeAssembly(layerName);

  Double_t SupportDz = fSupportLz[istn] / 2.;
  Double_t driftDz   = fActiveLzSector[istn] / 2 + fDriftDz / 2.;
  Double_t g10Dz     = driftDz + fDriftDz / 2. + fG10Dz / 2.;


  Double_t moduleZ   = fLayersZ0[istn];
  Double_t driftZIn  = moduleZ - driftDz;
  Double_t driftZOut = moduleZ + driftDz;
  Double_t g10ZIn    = moduleZ - g10Dz;
  Double_t g10ZOut   = moduleZ + g10Dz;
  Double_t cool_z    = g10ZIn - fG10Dz / 2. - SupportDz;

  Double_t dz = fActiveLzSector[istn] / 2.;  // Active Volume Thickness

  Int_t Nsector = 16.0;
  Double_t phi0 = TMath::Pi() / Nsector;  // azimuthal half widh of each module

  //define the spacer dimensions
  Double_t tg   = (dx2 - dx1) / 2 / dy;
  Double_t dd1  = fSpacerPhi * tg;
  Double_t dd2  = fSpacerPhi * sqrt(1 + tg * tg);
  Double_t sdx1 = dx1 + dd2 - dd1;
  Double_t sdx2 = dx2 + dd2 + dd1;
  Double_t sdy1 = dy + fSpacerR1;  // frame width added
  Double_t sdy2 = dy + fSpacerR2;  // frame width added
  Double_t sdz  = fFrameLzSector[istn] / 2.;

  //define Additional material as realistic GEM module
  Double_t dz_sD = fDriftDz / 2.;  //35 micron copper Drift
  Double_t dz_sG = fG10Dz / 2.;    // 3mm G10
  Double_t sdy   = sdy2;


  Double_t pos[10];

  Int_t iMod = 0;
  for (Int_t iSide = 0; iSide < 1; iSide++) {
    //muchLySd = muchLy->GetSide(iSide);
    // Now start adding the GEM modules
    for (Int_t iModule = 0; iModule < 1; iModule++) {

      Double_t phi  = 0;  // add 0.5 to not overlap with y-axis for left-right layer separation
      Bool_t isBack = iModule % 2;
      Char_t cside  = (isBack == 1) ? 'b' : 'f';

      // correct the x, y positions
      pos[0] = fX[istn];
      pos[1] = fY[istn];


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

      //------------------------------------------------------Al Cooling Plate--------------------------------------------------------------------
      TGeoVolume* voActive;
      TGeoTrap* coolGem;
      TGeoVolume* voCool;

      coolGem = new TGeoTrap(SupportDz, 0, phi, sdy2, sdx1, sdx2, 0, sdy2, sdx1, sdx2, 0);
      coolGem->SetName(Form("shStation%02iLayer%i%cModule%03icoolGem", istn, ily, cside, iModule));
      TString CoolName = Form("muchstation%02ilayer%i%ccoolGem%03iAluminum", istn + 1, ily + 1, cside, iModule + 1);
      voCool           = new TGeoVolume(CoolName, coolGem, coolMat);
      voCool->SetLineColor(kYellow);

      //------------------------------------------------------Active Volume-----------------------------------------------------
      //GEM
      TGeoTrap* shapeActive = new TGeoTrap(dz, 0, 0, dy, dx1, dx2, 0, dy, dx1, dx2, 0);
      shapeActive->SetName(Form("shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule));
      TString activeName = Form("muchstation%02ilayer%i%cactive%03igasArgon", istn + 1, ily + 1, cside, iMod + 1);
      voActive           = new TGeoVolume(activeName, shapeActive, argon);
      voActive->SetLineColor(kGreen);


      //---------------------------------------------------------Drift & PCB's---------------------------------------------------------------

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
      //rotate in the vertical plane (per to z axis) with angle
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
      volayer->AddNode(voDrift[0], iMod, incline_mod[1]);  //Drift In
      volayer->AddNode(voDrift[1], iMod, incline_mod[2]);  //Drift Out
      volayer->AddNode(voG10[0], iMod, incline_mod[3]);    //G10 In
      volayer->AddNode(voG10[1], iMod, incline_mod[4]);    //G10 Out
      volayer->AddNode(voCool, iMod, incline_mod[5]);      // Al Cooling Plate

      TVector3 Position;
      Position.SetXYZ(pos[0], pos[1], pos[2]);
      cout << pos[2] << "   " << pos[3] << "   " << pos[4] << "   " << pos[5] << "   " << pos[6] << "   " << pos[7]
           << endl;
     muchLySd->AddModule(new CbmMuchModuleGemRadial(fDetType[istn], istn, ily, iSide, iModule, Position, dx1,
                                                     dx2,  dy,  dz, muchSt->GetRmin()));
cout<<"stn: "<<istn<<"   module: "<<iModule<<endl;
    }
  }

  return volayer;
}

TGeoVolume* CreateLayersRpc(int istn, int ily){

 
 TString layerName;
 
   layerName = Form("muchstation%02ilayerRpc%i",istn+1,ily+1);
  

  TGeoVolumeAssembly* volayer = new TGeoVolumeAssembly(layerName);

  Double_t SupportDz = fSupportLz[istn]/2.;
  Double_t RpcGlassDz = fActiveLzSector[istn]/2. + fRpcGlassDz[istn]/2.;
  Double_t driftDz = RpcGlassDz + fRpcGlassDz[istn]/2 + fDriftDz/2.;
  Double_t g10Dz = driftDz + fDriftDz/2. + fG10Dz/2.;
  Double_t FrameRpcDz = fActiveLzSector[istn]/2. + fRpcGlassDz[istn] + fDriftDz + g10Dz + fG10Dz;
  
  Double_t moduleZ = fLayersZ0[istn]; 
  Double_t RpcGlassZIn = moduleZ - RpcGlassDz;
  Double_t RpcGlassZOut = moduleZ + RpcGlassDz;
  Double_t driftZIn = moduleZ - driftDz; 
  Double_t driftZOut = moduleZ + driftDz; 
  Double_t g10ZIn = moduleZ - g10Dz;
  Double_t g10ZOut = moduleZ + g10Dz;
 // Double_t cool_z = g10ZOut + fG10Dz/2. + SupportDz;
  Double_t cool_z = g10ZIn - fG10Dz / 2. - SupportDz;

  Double_t dz  = fActiveLzSector[istn]/2.; // Active Volume Thickness 
  
  Int_t Nsector=16.0;
  Double_t phi0 = TMath::Pi()/Nsector; // azimuthal half widh of each module

  dx1 = dx1R; dx2 = dx2R; dy = dyR;
  //define the spacer dimensions      
  Double_t tg = (dx2-dx1)/2/dy;
  Double_t dd1 = fSpacerPhi*tg;
  Double_t dd2 = fSpacerPhi*sqrt(1+tg*tg);
  Double_t sdx1 = dx1+dd2-dd1;
  Double_t sdx2 = dx2+dd2+dd1; 
  Double_t sdy1  = dy+fSpacerR1; // frame width added 
  Double_t sdy2  = dy+fSpacerR2; // frame width added
  Double_t sdz = fFrameLzSector[istn]/2.;
  
  //define Additional material as realistic GEM module
  Double_t dz_sD = fDriftDz/2.; //35 micron copper Drift 
  Double_t dz_sG = fG10Dz/2.; // 3mm G10
  Double_t sdy = sdy2;
  
  
  Double_t pos[10];
  
  Int_t iMod =0;
  for (Int_t iSide=0;iSide<1;iSide++){
     // muchLySd = muchLy->GetSide(iSide);
    // Now start adding the Rpc modules  
    for (Int_t iModule=0; iModule<1; iModule++){ 
      
      Double_t phi  = 0;  // add 0.5 to not overlap with y-axis for left-right layer separation
      Bool_t isBack = iModule%2; 
      Char_t cside  = (isBack==1) ? 'b' : 'f'; 
      
      // correct the x, y positions
      pos[0] = fX[istn];   
      pos[1] = fY[istn];
      
      
      
      // different z positions for odd/even modules
      pos[2] = moduleZ ;
      pos[3] = RpcGlassZIn;
      pos[4] = RpcGlassZOut;
      pos[5] = driftZIn;
      pos[6] = driftZOut;
      pos[7] = g10ZIn;
      pos[8] = g10ZOut;
      pos[9] = cool_z;
      
      if(iSide!=isBack)continue;
      if(iModule!=0)iMod =iModule/2;
      
      muchLySd = muchLy->GetSide(iSide);  
    

      TGeoMedium* SpacerRpc = gGeoMan->GetMedium(Al); // spacer medium
      TGeoMedium* copperplate = gGeoMan->GetMedium(copper); // copper Drift medium
      TGeoMedium* g10plate = gGeoMan->GetMedium(g10); // G10 medium
      TGeoMedium*  RPCglassmat = gGeoMan->GetMedium(RPCg); // RPC Glass
      TGeoMedium*  RPCgasmedium = gGeoMan->GetMedium(RPCm); // RPC Gas
      TGeoMedium* coolMat = gGeoMan->GetMedium(Al);
      

      TGeoTrap* shapeRpc = new TGeoTrap(sdz,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
     // TGeoBBox * shapeRpc = new TGeoBBox(dxx,dyy,FrameRpcDz);
      shapeRpc->SetName(Form("shStation%02iLayer%i%cModule%03iActiveRpcNoHole", istn, ily, cside, iModule));
      
      //------------------------------------------------------Al Cooling Plate--------------------------------------------------------------------
      TGeoVolume* voActive, *voRPCback, *voRPCFront;      
      TGeoTrap * coolRpc;
      TGeoVolume* voCool;
   
	coolRpc = new TGeoTrap(SupportDz,0,phi,sdy2,sdx1,sdx2,0,sdy2,sdx1,sdx2,0);
	coolRpc->SetName(Form("shStation%02iLayer%i%cModule%03icoolRpc", istn, ily, cside, iModule));
	TString CoolName = Form("muchstation%02ilayer%i%ccoolRpc%03iAluminum",istn+1,ily+1,cside,iModule+1);
	voCool = new TGeoVolume(CoolName,coolRpc,coolMat);
	voCool->SetLineColor(kYellow);
     
      
      
      //------------------------------------------------------Active Volume & Rpc Glass----------------------------------------------------- 
 
          //RPC Active
          TGeoTrap* shapeActive = new TGeoTrap(dz,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
          shapeActive->SetName(Form("shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule));
	  TString activeName = Form("muchstation%02ilayer%i%cactive%03irpcgas",istn+1,ily+1,cside,iMod+1);
	  voActive = new TGeoVolume(activeName,shapeActive,RPCgasmedium);
	  voActive->SetLineColor(kRed);
	  
	  //RPC Glass Front          
          TGeoTrap* shapeRPCfront = new TGeoTrap(dz,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
         // TGeoBBox * shapeRPCfront = new TGeoBBox(dxx,dyy,dz);
	  shapeRPCfront->SetName(Form("shStation%02iLayer%i%cModule%03iNoHoleFront", istn, ily, cside, iModule));
	  TString NameFront = Form("muchstation%02ilayer%i%c%03irpcglassFront",istn+1,ily+1,cside,iMod+1);
	  voRPCFront = new TGeoVolume(NameFront,shapeRPCfront,RPCglassmat);
	  voRPCFront->SetLineColor(kRed);
	  
	  //RPC Glass Back
          TGeoTrap* shapeRPCback = new TGeoTrap(dz,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
          //TGeoBBox * shapeRPCback = new TGeoBBox(dxx,dyy,dz);
	  shapeRPCback->SetName(Form("shStation%02iLayer%i%cModule%03iNoHoleback", istn, ily, cside, iModule));
	  TString Nameback = Form("muchstation%02ilayer%i%c%03irpcglassback",istn+1,ily+1,cside,iMod+1);
	  voRPCback = new TGeoVolume(Nameback,shapeRPCback,RPCglassmat);
	  voRPCback->SetLineColor(kRed);
      
//---------------------------------------------------------Drift & PCB's---------------------------------------------------------------     
      
      TString DriftName[2], G10Name[2], AlName;
      TGeoVolume *voDrift[2], *voG10[2];
      TGeoTrap *DriftRpc[2], *G10Rpc[2];
      
      for(int iPos =0; iPos<2; iPos++){
	Char_t cpos  = (iPos==0) ? 'i' : 'o'; 
	
	    DriftRpc[iPos] = new TGeoTrap(dz_sD,0,phi,dy+1.,dx1+1.,dx2+1.,0,dy+1.,dx1+1.,dx2+1.,0);
	    G10Rpc[iPos] = new TGeoTrap(dz_sG,0,phi,dy+1.,dx1+1.,dx2+1.,0,dy+1.,dx1+1.,dx2+1.,0);
	    DriftRpc[iPos]->SetName(Form("shStation%02iLayer%i%cModule%03i%cDrift", istn, ily, cside, iModule,cpos));
	    DriftName[iPos] = Form("muchstation%02ilayer%i%cside%03i%ccopperDrift",istn+1,ily+1,cside,iMod+1,cpos);
	    G10Rpc[iPos]->SetName(Form("shStation%02iLayer%i%cModule%03i%cG10", istn, ily, cside, iModule,cpos));
	    G10Name[iPos] = Form("muchstation%02ilayer%i%cside%03i%cG10",istn+1,ily+1,cside,iMod+1,cpos);
	    voDrift[iPos] = new TGeoVolume(DriftName[iPos],DriftRpc[iPos],copperplate);
	    voDrift[iPos]->SetLineColor(kRed);
	    voG10[iPos] = new TGeoVolume(G10Name[iPos],G10Rpc[iPos],g10plate);
	    voG10[iPos]->SetLineColor(3);
	  
      }

      //------------------------------------------------------------Frame-----------------------------------------------------------------
      // Define the trapezoidal Frame 
      TGeoTrap* shapeFrame = new TGeoTrap(sdz,0,phi,dy+1.,dx1+1.,dx2+1.,0,dy+1.,dx1+1.,dx2+1.,0);
      shapeFrame->SetName(Form("shStation%02iLayer%i%cModule%03iFullFrameGemNoHole", istn, ily, cside, iModule));

      TGeoTrap* shapeFrame1 = new TGeoTrap(sdz+.0001,0,phi,dy,dx1,dx2,0,dy,dx1,dx2,0);
      shapeFrame1->SetName(Form("shStation%02iLayer%i%cModule%03iActiveVol", istn, ily, cside, iModule));
      
         
     
      TString expression;
	  expression = Form("shStation%02iLayer%i%cModule%03iFullFrameGemNoHole-shStation%02iLayer%i%cModule%03iActiveVol", istn, ily, cside, iModule, istn, ily, cside, iModule);
        
      
      TGeoCompositeShape* shFrame = new TGeoCompositeShape(Form("shStation%02iLayer%i%cModule%03iFinalFrameNoHole", istn, ily, cside, iModule), expression);
      TString frameName = Form("muchstation%02ilayer%i%cframe%03i",istn+1,ily+1,cside,iMod+1);
      
      TGeoVolume* voFrame;
      
	  voFrame = new TGeoVolume(frameName,shFrame,SpacerRpc);  // Frame For RPC
          voFrame->SetLineColor(kRed);
      

//----------------------------------------------Box -------------------------------------------
       Double_t placeBox[3]={0.,0.,-0.5};
       TGeoBBox * RpcBox1 = new TGeoBBox(dxx,dyy,dzz);
       RpcBox1->SetName(Form("shStation%02iLayer%i%cModule%03iRpcBox1", istn, ily, cside, iModule));

       TGeoBBox * RpcBox2 = new TGeoBBox(dxx-.3,dyy-.3,dzz-.5/2.,placeBox);
       RpcBox2->SetName(Form("shStation%02iLayer%i%cModule%03iRpcBox2", istn, ily, cside, iModule));

       TString expressionBox;
       expressionBox = Form("shStation%02iLayer%i%cModule%03iRpcBox1-shStation%02iLayer%i%cModule%03iRpcBox2", istn, ily, cside, iModule, istn, ily, cside, iModule);


       TGeoCompositeShape* shBox = new TGeoCompositeShape(Form("shStation%02iLayer%i%cModule%03iFinalBox", istn, ily, cside, iModule), expressionBox);
      TString BoxName = Form("muchstation%02ilayer%i%cBox%03i",istn+1,ily+1,cside,iMod+1);
      
      TGeoVolume* voBox;
      voBox = new TGeoVolume(BoxName,shBox,SpacerRpc);  // Box For RPC
      voBox->SetLineColor(kMagenta);
      voBox->SetTransparency(2);
      //----------------------------------------------------Placement----------------------------------------------------------------------  
      // Calculate the phi angle of the sector where it has to be placed 
      Double_t angle = 180.0-(180. / TMath::Pi() * phi0);  // convert angle phi from rad to deg

      // displace RPC module along the TOF frame
      Double_t YOffset =  0.0;
      Double_t ZOffset = -0.5;
      
      Double_t displacement_along_angle = -8.5; // -50.0; // cm for Carbon-12 run
      Double_t module_rotation_angle    = -2.0; // degrees
      
      YOffset += displacement_along_angle *cos( module_rotation_angle * acos(-1)/180 );
      ZOffset += displacement_along_angle *sin( module_rotation_angle * acos(-1)/180 );

      // RPC backside at 0 degrees z = 237.5
      //    y = -46.5 cm ... 76.5 cm -> 123 cm size in y
      // -8.5 = -46.5 cm ... 76.5 cm displacement
      //      100 *cos( -20 * acos(-1)/180 )
      //      100 *sin( -20 * acos(-1)/180 )
      // end of displacement along the rails
      
      TGeoRotation *r2 = new TGeoRotation("r2");
      //rotate in the vertical plane (per to z axis) with angle 
      r2->RotateZ(0);
   //   cout<<RpcAwayBeamAxisY<<"     "<<BoxAwayBeamAxisY<<endl;
      TGeoTranslation *trans[10];
      TGeoHMatrix *incline_mod[10]; 
      
      TGeoTranslation *transBox = new TGeoTranslation("",posBox[0],posBox[1]+YOffset,pos[2]+ZOffset);
      //      TGeoTranslation *transBox = new TGeoTranslation("",posBox[0],posBox[1],pos[2]);
      TGeoHMatrix *incline_modBox = new TGeoHMatrix("");
	(*incline_modBox) =  (*transBox) * (*r2);
	// incline_modBox->RotateX(-2.);
	incline_modBox->RotateX(module_rotation_angle);//sroy
	
      for(int i=0; i<8; i++){
	trans[i] = new TGeoTranslation("",pos[0],pos[1]+YOffset,pos[i+2]+ZOffset);
	//	trans[i] = new TGeoTranslation("",pos[0],pos[1],pos[i+2]);
       
	incline_mod[i] = new TGeoHMatrix("");
	(*incline_mod[i]) =  (*trans[i]) * (*r2);  
	// incline_mod[i]->RotateX(-2.);
	incline_mod[i]->RotateX(module_rotation_angle);//sroy

      }
   
      volayer->AddNode(voBox, iMod, incline_modBox); // add box volume 
      volayer->AddNode(voActive, iMod, incline_mod[0]); // add active volume 
     // volayer->AddNode(voFrame, iMod, incline_mod[0]);  // add spacer
      volayer->AddNode(voRPCFront, iMod, incline_mod[1]); //Rpc Glass In
      volayer->AddNode(voRPCback, iMod, incline_mod[2]); //Rpc Glass Out 
     
      volayer->AddNode(voDrift[0], iMod, incline_mod[3]); //Drift In
      volayer->AddNode(voDrift[1], iMod, incline_mod[4]); //Drift Out
      volayer->AddNode(voG10[0], iMod, incline_mod[5]); //G10 In
      volayer->AddNode(voG10[1], iMod, incline_mod[6]); //G10 Out
   //   volayer->AddNode(voCool, iMod, incline_mod[7]); // Al Cooling Plate  

      TVector3 Position;
      Position.SetXYZ(pos[0], pos[1], pos[2]);
      cout << pos[2] << "   " << pos[3] << "   " << pos[4] << "   " << pos[5] << "   " << pos[6] << "   " << pos[7]
           << endl;
     muchLySd->AddModule(new CbmMuchModuleGemRadial(fDetType[istn], istn, ily, iSide, iModule, Position, dx1,
                                                     dx2,  dy,  dz, muchSt->GetRmin()));
cout<<"stn: "<<istn<<"   module: "<<iModule<<endl;
    }
  }  
  
  return volayer;




}
