/* Copyright (C) 2016-2021 Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Chernogorov, Mehul Shiroya [committer], Florian Uhlig*/

/******************************************************************************
 ** Creation of beam pipe geometry in ROOT format (TGeo).
 **
 ** @file create_bpipe_geometry_v16_1m.C
 ** @author Andrey Chernogorov <a.chernogorov@gsi.de>
 ** @author P.-A Loizeau <p.-a.loizeau@gsi.de>
 ** @date 02.06.2016
 ** @author Anna Senger <a.senger@gsi.de>
 ** SIS-100
 ** pipe_v18
 **
 * The file was copied from previous version and made some update for the STS beam pipe
 * Old beam pipe. Thickness is 1 mm.
 * 
 ** The beam pipe is composed of carbon with a thickness of 0.5 mm 
 ** It is placed directly into the cave as mother volume.
 ** Each section has a PCON shape (including window).
 ** There is one window of iron the end of the pipe with R95mm and 0.2mm thickness.
 ** The STS section is composed of cylinder D(z=220-500mm)=36mm and cone
 ** (z=500-1700mm).
 ** All other sections of the beam pipe fit either the 2.5 degree standard
 ** limit or their respective detectors opening if less than 2.5 degree is
 ** available.
 *****************************************************************************/


#include "TGeoManager.h"

#include <iomanip>
#include <iostream>


using namespace std;


// -------------   Steering variables       -----------------------------------
// ---> Beam pipe material name
TString pipeMediumName     = "carbon";  // "aluminium" "beryllium" "carbon"
Double_t dPipeThickness    = 0.5;       // mm
Double_t dSTSPipeThickness = 1.0;
Int_t energy               = 8;
TString Energy             = "8";
// ----------------------------------------------------------------------------


// -------------   Other global variables   -----------------------------------
//TString Version = "v19_v2_1m";
TString Version = "v21a";
//TString Variation = Version + ".AuAu" + Energy + "AGeV"; //sup
TString Variation = Version;  //sup
// ---> Macros name to info file
TString macrosname = "create_bpipe_geometry_" + Variation + ".C";
// ---> Geometry file name (output)
TString rootFileName = "pipe_" + Variation + ".geo.root";
// ---> Geometry name
TString pipeName = "pipe_" + Variation;
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, Double_t* rout, TGeoMedium* medium,
                     fstream* infoFile);

TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, Double_t* rout, TGeoMedium* medium,
                       fstream* infoFile);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_bpipe_geometry_v21a(Bool_t bMuch = kTRUE, Bool_t bTrd = kTRUE, Bool_t bTof = kTRUE, Bool_t bEnd = kTRUE,
                                Bool_t bWin = kTRUE)
{
  Double_t pipeRotationAngle[15];
  pipeRotationAngle[2]  = 1.8;
  pipeRotationAngle[4]  = 1.2;
  pipeRotationAngle[6]  = 0.8;
  pipeRotationAngle[8]  = 1.;
  pipeRotationAngle[10] = 0.7;
  pipeRotationAngle[12] = 0.6;

  Double_t pipeXshift1 = -370. * TMath::Tan(pipeRotationAngle[energy] * TMath::DegToRad());
  Double_t pipeXshift2 = pipeXshift1 + 5;
  Double_t pipeXshift3 = 5;

  // -----   Define beam pipe sections   --------------------------------------

  TString pipe1name   = "pipe1 - vacuum chamber";
  const Int_t nSects1 = 6;

  Double_t z1[nSects1]    = {-90., -45., -45., 190.17, 190.17, 190.87};  // mm
  Double_t rin1[nSects1]  = {25., 25., 400., 400., 110., 110.};
  Double_t rout1[nSects1] = {25.7, 25.7, 400.7, 400.7, 400.7, 130.7};

  /*
  Double_t z1[nSects1]    = { -50.,  -5.,   -5.,  230.17, 230.17, 230.87 }; // mm
  Double_t rin1[nSects1]  = {  25.,  25.,  400.,  400.,   110.,   110.   };
  Double_t rout1[nSects1] = {  30, 30, 405, 405,  405,  135  };
  */
  TString pipe2name       = "pipe2 - first window @ 220mm, h=0.7mm, R=600mm";
  const Int_t nSects2     = 7;
  Double_t z2[nSects2]    = {180., 180.7, 181.45, 183.71, 187.49, 190.17, 190.87};  // mm
  Double_t rin2[nSects2]  = {18., 18., 30., 60., 90., 105.86, 110.};
  Double_t rout2[nSects2] = {18., 28.69, 39.3, 65.55, 94.14, 110., 110.};

  TString pipevac1name      = "pipevac1";
  const Int_t nSects01      = 10;
  Double_t z01[nSects01]    = {-90., -45., -45., 180., 180., 180.7, 181.45, 183.71, 187.49, 190.17};  // mm
  Double_t rin01[nSects01]  = {0., 0., 0., 0., 18., 28.69, 39.3, 65.55, 94.14, 110.};
  Double_t rout01[nSects01] = {25., 25., 400., 400., 400., 400., 400., 400., 400., 400.};

  TString pipe3name       = "pipe3 - STS section";
  const Int_t nSects3     = 4;
  Double_t z3[nSects3]    = {180., 460., 1220., 1700.};  // mm
  Double_t rout3[nSects3] = {18., 18., 55., 74.2};
  Double_t rin3[nSects3];
  for (Int_t i = 0; i < nSects3; i++) {
    rin3[i] = rout3[i] - dPipeThickness;
  }
  TString pipevac2name     = "pipevac3";
  const Int_t nSects02     = nSects3;
  Double_t z02[nSects02]   = {180., 460., 1220., 1700.};  // mm
  Double_t rin02[nSects02] = {0., 0., 0., 0.};
  Double_t rout02[nSects02];
  for (Int_t i = 0; i < nSects02; i++) {
    rout02[i] = rin3[i];
  }

  /*
  TString pipeNameMuch           = "pipe4 - MUCH/RICH section";  // First 2.5 then 3.0 from 1850 mm up to TRD
  const Int_t nSectsMuch         = 2;
  Double_t dZposMuch[nSectsMuch] = {1700., 3700.};  // mm
  Double_t dRoutMuch[nSectsMuch] = {74.2, 161.1};   // mm
  Double_t dRinMuch[nSectsMuch];  //for(Int_t i=0; i<nSectsMuch; i++) { dRinMuch[i] = dRoutMuch[i] - dPipeThickness; }
  dRinMuch[0] = dRoutMuch[0] - 1.48;
  dRinMuch[1] = dRoutMuch[1] - 3.2;

  TString pipeVacNameMuch   = "pipevac4";
  const Int_t nSectsVacMuch = nSectsMuch;
  Double_t dZposVacMuch[nSectsVacMuch];
  for (Int_t i = 0; i < nSectsVacMuch; i++) {
    dZposVacMuch[i] = dZposMuch[i];
  }
  Double_t dRinVacMuch[nSectsVacMuch];
  for (Int_t i = 0; i < nSectsVacMuch; i++) {
    dRinVacMuch[i] = 0.;
  }
  Double_t dRoutVacMuch[nSectsVacMuch];
  for (Int_t i = 0; i < nSectsVacMuch; i++) {
    dRoutVacMuch[i] = dRinMuch[i];
  }

  TString pipeNameTrd   = "pipe5 - TRD-ToF section";
  const Int_t nSectsTrd = 2;
  //  Double_t dZposTrd[nSectsTrd] = { 3700.,  9000. }; // mm
  Double_t dZposTrd[nSectsTrd] = {3700. + 2, 9000.};  // mm
  Double_t dRoutTrd[nSectsTrd] = {95., 95.};          // mm
  Double_t dRinTrd[nSectsTrd];
  for (Int_t i = 0; i < nSectsTrd; i++) {
    dRinTrd[i] = dRoutTrd[i] - dPipeThickness;
  }

  TString pipeVacNameTrd   = "pipevac5";
  const Int_t nSectsVacTrd = nSectsTrd;
  Double_t dZposVacTrd[nSectsVacTrd];
  for (Int_t i = 0; i < nSectsVacTrd; i++) {
    dZposVacTrd[i] = dZposTrd[i];
  }
  Double_t dRinVacTrd[nSectsVacTrd];
  for (Int_t i = 0; i < nSectsVacTrd; i++) {
    dRinVacTrd[i] = 0.;
  }
  Double_t dRoutVacTrd[nSectsVacTrd];
  for (Int_t i = 0; i < nSectsVacTrd; i++) {
    dRoutVacTrd[i] = dRinTrd[i];
  }

  // Straight window
  TString pipeNameWin   = "pipe6 - second window @ the end, h=0.2mm, R=161.1mm";  // iron !!!
  const Int_t nSectsWin = 2;
  //  Double_t dZposWin[nSectsWin] = { 3700, 3700 + 0.2  }; // mm
  Double_t dZposWin[nSectsWin] = {3700 + 2, 3700 + 2.2};  // mm
  Double_t dRoutWin[nSectsWin] = {161.1, 161.1};          // mm
  Double_t dRinWin[nSectsWin]  = {0., 0.};                // mm

  TString pipeNameWinVac   = "pipevac6";  // iron !!!
  const Int_t nSectsWinVac = nSectsWin;
  //  Double_t dZposWinVac[nSectsWinVac] = { 3700, 3700 + 0.2  }; // mm
  Double_t dZposWinVac[nSectsWinVac] = {3700 + 2, 3700 + 2.2};  // mm
  Double_t dRoutWinVac[nSectsWinVac] = {95., 95.};              // mm
  Double_t dRinWinVac[nSectsWinVac]  = {0., 0.};                // mm


  TString pipeNamePsd          = "pipe7 - PSD section";
  const Int_t nSectsPsd        = 2;
  Double_t dZposPsd[nSectsPsd] = {9000 + 0.2, 19000.};  // mm
  Double_t dRoutPsd[nSectsPsd] = {95., 95.};            // mm
  Double_t dRinPsd[nSectsPsd];
  for (Int_t i = 0; i < nSectsPsd; i++) {
    dRinPsd[i] = dRoutPsd[i] - dPipeThickness;
  }

  TString pipeVacNamePsd   = "pipevac7";
  const Int_t nSectsVacPsd = nSectsPsd;
  Double_t dZposVacPsd[nSectsVacPsd];
  for (Int_t i = 0; i < nSectsVacPsd; i++) {
    dZposVacPsd[i] = dZposPsd[i];
  }
  Double_t dRinVacPsd[nSectsVacPsd];
  for (Int_t i = 0; i < nSectsVacPsd; i++) {
    dRinVacPsd[i] = 0.;
  }
  Double_t dRoutVacPsd[nSectsVacPsd];
  for (Int_t i = 0; i < nSectsVacPsd; i++) {
    dRoutVacPsd[i] = dRinPsd[i];
  }

  // Straight window
  TString pipeNameWinEnd             = "pipe8 - second window @ the end, h=0.2mm, R=95mm";  // iron !!!
  const Int_t nSectsWinEnd           = 2;
  Double_t dZposWinEnd[nSectsWinEnd] = {dZposPsd[nSectsPsd - 1], dZposPsd[nSectsPsd - 1] + 0.2};  // mm
  Double_t dRoutWinEnd[nSectsWinEnd] = {dRoutPsd[nSectsPsd - 1], dRoutPsd[nSectsPsd - 1]};        // mm
  Double_t dRinWinEnd[nSectsWinEnd]  = {0., 0.};                                                  // mm

  TString pipeVacNameWinEnd   = "pipevac8";
  const Int_t nSectsVacWinEnd = 0;

  //  /************************************************************
  //   */
  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = rootFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  fstream infoFileEmpty;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "SIS-100. Beam pipe geometry created with " + macrosname << endl << endl;
  infoFile << "pipe_" << Version << " = pipe for the SIS100" << endl << endl;
  infoFile << "Sections: MVD + STS + MUCH + TRD + TOF + PSD" << endl;
  infoFile << "Beam pipe behind MUCH with R=95mm" << endl;
  infoFile << "Beam pipe stops @ 19 m from target with 0.2 mm Fe" << endl;
  infoFile << "Material:  " << pipeMediumName << endl;
  infoFile << "Thickness: " << dPipeThickness << " mm" << endl << endl;
  // --------------------------------------------------------------------------


  // -------   Load media from media file   -----------------------------------
  FairGeoLoader* geoLoad    = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath           = gSystem->Getenv("VMCWORKDIR");
  TString medFile           = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  TGeoManager* gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------


  // -----------------   Get and create the required media    -----------------
  FairGeoMedia* geoMedia   = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // ---> pipe medium
  FairGeoMedium* fPipeMedium = geoMedia->getMedium(pipeMediumName.Data());
  TString fairError          = "FairMedium " + pipeMediumName + " not found";
  if (!fPipeMedium) Fatal("Main", "FairMedium for PipeMedium not found");
  geoBuild->createMedium(fPipeMedium);
  TGeoMedium* pipeMedium = gGeoMan->GetMedium(pipeMediumName.Data());
  TString geoError       = "Medium " + pipeMediumName + " not found";
  if (!pipeMedium) Fatal("Main", "Medium for PipeMedium not found");
  // ---> iron
  FairGeoMedium* mIron = geoMedia->getMedium("iron");
  if (!mIron) Fatal("Main", "FairMedium iron not found");
  geoBuild->createMedium(mIron);
  TGeoMedium* iron = gGeoMan->GetMedium("iron");
  if (!iron) Fatal("Main", "Medium iron not found");
  // ---> vacuum
  FairGeoMedium* mVacuum = geoMedia->getMedium("vacuum");
  if (!mVacuum) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mVacuum);
  TGeoMedium* vacuum = gGeoMan->GetMedium("vacuum");
  if (!vacuum) Fatal("Main", "Medium vacuum not found");
  // --------------------------------------------------------------------------


  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("PIPEgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* pipe = new TGeoVolumeAssembly(pipeName.Data());
  // --------------------------------------------------------------------------


  // -----   Create sections  -------------------------------------------------
  infoFile << endl << "Beam pipe section: " << pipe1name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;

  TGeoVolume* pipe1 = MakePipe(1, nSects1, z1, rin1, rout1, pipeMedium, &infoFile);
  pipe1->SetLineColor(kGray);
  pipe->AddNode(pipe1, 0);

  infoFile << endl << "Beam pipe section: " << pipe2name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;
  TGeoVolume* pipe2 = MakePipe(2, nSects2, z2, rin2, rout2, pipeMedium, &infoFile);
  pipe2->SetLineColor(kBlue);
  pipe->AddNode(pipe2, 0);
  TGeoVolume* pipevac1 = MakeVacuum(1, nSects01, z01, rin01, rout01, vacuum, &infoFile);
  pipevac1->SetLineColor(kCyan);
  pipe->AddNode(pipevac1, 0);

  infoFile << endl << "Beam pipe section: " << pipe3name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;
  TGeoVolume* pipe3 = MakePipe(3, nSects3, z3, rin3, rout3, pipeMedium, &infoFile);
  pipe3->SetLineColor(kGreen);
  pipe->AddNode(pipe3, 0);
  TGeoVolume* pipevac2 = MakeVacuum(3, nSects02, z02, rin02, rout02, vacuum, &infoFile);
  pipevac2->SetLineColor(kCyan);
  pipe->AddNode(pipevac2, 0);

  /*
  infoFile << endl << "Beam pipe section: " << pipeNameMuch << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;
  TGeoVolume* pipeMuch = MakePipe(4, nSectsMuch, dZposMuch, dRinMuch, dRoutMuch, pipeMedium, &infoFile);
  pipeMuch->SetLineColor(kGreen);
  pipe->AddNode(pipeMuch, 0);
  TGeoVolume* pipeVacMuch = MakeVacuum(4, nSectsVacMuch, dZposVacMuch, dRinVacMuch, dRoutVacMuch, vacuum, &infoFile);
  pipeVacMuch->SetLineColor(kCyan);

  TGeoRotation* pipe_rot = new TGeoRotation();
  pipe_rot->RotateY(pipeRotationAngle[energy]);

  TGeoTranslation* pipe_trans1 = new TGeoTranslation("pipe_trans1", pipeXshift1, 0., 0);
  TGeoCombiTrans* combi_trans1 = new TGeoCombiTrans(*pipe_trans1, *pipe_rot);

  TGeoTranslation* pipe_trans2 = new TGeoTranslation("pipe_trans2", pipeXshift2, 0., 0.);
  TGeoTranslation* pipe_trans3 = new TGeoTranslation("pipe_trans3", pipeXshift3, 0., 0.);
  TGeoCombiTrans* combi_trans2 = new TGeoCombiTrans(*pipe_trans2, *pipe_rot);


  infoFile << endl << "Beam pipe section: " << pipeNameTrd << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;
  TGeoVolume* pipeTrd = MakePipe(5, nSectsTrd, dZposTrd, dRinTrd, dRoutTrd, pipeMedium, &infoFile);
  pipeTrd->SetLineColor(kGreen);
  //pipe->AddNode(pipeTrd, 0, combi_trans2);

  TGeoVolume* pipeVacTrd = MakeVacuum(5, nSectsVacTrd, dZposVacTrd, dRinVacTrd, dRoutVacTrd, vacuum, &infoFile);
  pipeVacTrd->SetLineColor(kCyan);
  //pipe->AddNode(pipeVacTrd, 0, combi_trans2);

  infoFile << endl << "Beam pipe section: " << pipeNameWin << ", material: iron" << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;
  TGeoVolume* pipeWin = MakePipe(6, nSectsWin, dZposWin, dRinWin, dRoutWin, iron, &infoFile);
  pipeWin->SetLineColor(kBlue);
  //pipe->AddNode(pipeWin, 0, combi_trans1);

  TGeoVolume* pipeWinVac = MakeVacuum(6, nSectsWinVac, dZposWinVac, dRinWinVac, dRoutWinVac, vacuum, &infoFile);
  pipeWinVac->SetLineColor(kCyan);
  pipeWin->AddNode(pipeWinVac, 0, pipe_trans3);
  //pipe->AddNode(pipeWin, 0, combi_trans1);

  pipeVacMuch->AddNodeOverlap(pipeTrd, 0, combi_trans2);
  pipeVacMuch->AddNodeOverlap(pipeVacTrd, 0, combi_trans2);
  pipeVacMuch->AddNodeOverlap(pipeWinVac, 0, pipe_trans3);
  pipe->AddNode(pipeVacMuch, 0);

  infoFile << endl << "Beam pipe section: " << pipeNamePsd << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;
  TGeoVolume* pipePsd = MakePipe(7, nSectsPsd, dZposPsd, dRinPsd, dRoutPsd, pipeMedium, &infoFile);
  pipePsd->SetLineColor(kCyan);
  pipe->AddNode(pipePsd, 0, combi_trans2);

  TGeoVolume* pipeVacPsd = MakeVacuum(7, nSectsVacPsd, dZposVacPsd, dRinVacPsd, dRoutVacPsd, vacuum, &infoFile);
  pipeVacPsd->SetLineColor(kCyan);
  pipe->AddNode(pipeVacPsd, 0, combi_trans2);

  infoFile << endl << "Beam pipe section: " << pipeNameWinEnd << ", material: iron" << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10)
           << "h,mm" << endl;
  TGeoVolume* pipeWinEnd = MakePipe(8, nSectsWinEnd, dZposWinEnd, dRinWinEnd, dRoutWinEnd, iron, &infoFile);
  pipeWinEnd->SetLineColor(kBlue);
  pipe->AddNode(pipeWinEnd, 0, combi_trans2);

  //  */
  // -----   End   --------------------------------------------------

  // ---------------   Finish   -----------------------------------------------
  TGeoTranslation* pipeTrans = new TGeoTranslation(0., 0., 0.);
  top->AddNode(pipe, 1, pipeTrans);
  cout << endl << endl;
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0001);
  gGeoMan->PrintOverlaps();
  gGeoMan->CheckOverlaps(0.0001, "s");
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* rootFile = new TFile(rootFileName, "RECREATE");
  top->Write();
  cout << endl;
  cout << "Geometry " << top->GetName() << " written to " << rootFileName << endl;
  rootFile->Close();
  infoFile.close();

  // visualize it with ray tracing, OGL/X3D viewer
  //top->Raytrace();
  top->Draw("ogl");
  //top->Draw("x3d");
}
// ============================================================================
// ======                   End of main function                          =====
// ============================================================================


// =====  Make the beam pipe volume   =========================================
TGeoPcon* MakeShape(Int_t nSects, char* name, Double_t* z, Double_t* rin, Double_t* rout, fstream* infoFile)
{

  // ---> Shape
  TGeoPcon* shape = new TGeoPcon(name, 0., 360., nSects);
  for (Int_t iSect = 0; iSect < nSects; iSect++) {
    shape->DefineSection(iSect, z[iSect] / 10., rin[iSect] / 10., rout[iSect] / 10.);  // mm->cm
    *infoFile << setw(2) << iSect + 1 << setw(10) << fixed << setprecision(2) << z[iSect] << setw(10) << fixed
              << setprecision(2) << rin[iSect] << setw(10) << fixed << setprecision(2) << rout[iSect] << setw(10)
              << fixed << setprecision(2) << rout[iSect] - rin[iSect] << endl;
  }

  return shape;
}
// ============================================================================


// =====  Make the beam pipe volume   =========================================
TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, Double_t* rout, TGeoMedium* medium,
                     fstream* infoFile)
{

  // ---> Shape
  TGeoPcon* shape = new TGeoPcon(0., 360., nSects);
  for (Int_t iSect = 0; iSect < nSects; iSect++) {
    shape->DefineSection(iSect, z[iSect] / 10., rin[iSect] / 10., rout[iSect] / 10.);  // mm->cm
    *infoFile << setw(2) << iSect + 1 << setw(10) << fixed << setprecision(2) << z[iSect] << setw(10) << fixed
              << setprecision(2) << rin[iSect] << setw(10) << fixed << setprecision(2) << rout[iSect] << setw(10)
              << fixed << setprecision(2) << rout[iSect] - rin[iSect] << endl;
  }

  // ---> Volume
  TString volName  = Form("pipe%i", iPart);
  TGeoVolume* pipe = new TGeoVolume(volName.Data(), shape, medium);

  return pipe;
}
// ============================================================================


// =====   Make the volume for the vacuum inside the beam pipe   ==============
TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, Double_t* rout, TGeoMedium* medium,
                       fstream* infoFile)
{

  // ---> Shape
  TGeoPcon* shape = new TGeoPcon(0., 360., nSects);
  for (Int_t iSect = 0; iSect < nSects; iSect++) {
    shape->DefineSection(iSect, z[iSect] / 10., rin[iSect] / 10., rout[iSect] / 10.);  // mm->cm
  }

  // ---> Volume
  TString volName     = Form("pipevac%i", iPart);
  TGeoVolume* pipevac = new TGeoVolume(volName.Data(), shape, medium);

  return pipevac;
}
// ============================================================================
