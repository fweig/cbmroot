/******************************************************************************
 ** Creation of beam pipe geometry in ROOT format (TGeo).
 **
 ** @file create_bpipe_geometry_v16_1m.C
 ** @author Andrey Chernogorov <a.chernogorov@gsi.de>
 ** @author P.-A Loizeau <p.-a.loizeau@gsi.de>
 ** @date 02.06.2016
 **
 ** SIS-100
 ** pipe_v16a_1m = pipe for the SIS100 muon setups
 **
 ** The beam pipe is composed of carbon with a thickness of 0.5 mm (0.4 mm is
 ** available from producer).
 ** It is placed directly into the cave as mother volume.
 ** Each section has a PCON shape (including windows).
 ** There are two windows:
 **   - first one @ 220mm with R600mm and 0.7mm thickness,
 **   - second one of iron the end of the pipe with R600mm and 0.2mm thickness.
 ** The STS section is composed of cylinder D(z=220-500mm)=36mm and cone
 ** (z=500-1700mm).
 ** All other sections of the beam pipe fit either the 2.5 degree standard
 ** limit or their respective detectors opening if less than 2.5 degree is
 ** available.
 ** The PSD section of the beam pipe is missing because it is planned that it
 ** will be part of PSD geometry.
 *****************************************************************************/




#include <iomanip>
#include <iostream>
#include "TGeoManager.h"


using namespace std;


// -------------   Steering variables       -----------------------------------
// ---> Beam pipe material name
TString  pipeMediumName = "carbon"; // "aluminium" "beryllium" "carbon"
Double_t dPipeThickness = 0.5; // mm
// ----------------------------------------------------------------------------


// -------------   Other global variables   -----------------------------------
TString sVersion = "v16a_1e";
// ---> Macros name to info file
TString macrosname = "create_bpipe_geometry_" + sVersion + ".C";
// ---> Geometry file name (output)
TString rootFileName = "pipe_" + sVersion + ".geo.root";
// ---> Geometry name
TString pipeName = "pipe_" + sVersion;
// ----------------------------------------------------------------------------


// ---------------- MVD and STS pipe sections variables -----------------------
#include "pipe_v16a_mvdsts100.h"
// ----------------------------------------------------------------------------

TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, 
		     Double_t* rout, TGeoMedium* medium, fstream* infoFile);

TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, 
		       Double_t* rout, TGeoMedium* medium, fstream* infoFile);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_bpipe_geometry_v16a_1e( Bool_t bMuch = kFALSE, Bool_t bTrd = kTRUE, 
                                    Bool_t bTof  = kTRUE, Bool_t bEnd = kTRUE, 
                                    Bool_t bWin  = kTRUE )
{
  // -----   Define beam pipe sections   --------------------------------------
  
  /**** MVD and STS sections in common header file for all sis100 pipes ****/
  for(Int_t i=0; i<nSects3; i++) { rin3[i] = rout3[i] - dPipeThickness; }
  for(Int_t i=0; i<nSects02; i++) { rout02[i]=rin3[i]; }
  
/*  
  TString pipe1name = "pipe1 - vacuum chamber";
  const Int_t nSects1 = 6;
  Double_t z1[nSects1]    = { -50.,  -5.,   -5.,  230.17, 230.17, 230.87 }; // mm
  Double_t rin1[nSects1]  = {  25.,  25.,  400.,  400.,   110.,   110.   };
  Double_t rout1[nSects1] = {  25.7, 25.7, 400.7, 400.7,  400.7,  130.7  };
  TString pipe2name = "pipe2 - first window @ 220mm, h=0.7mm, R=600mm";
  const Int_t nSects2 = 7;
  Double_t z2[nSects2]    = { 220., 220.7, 221.45, 223.71, 227.49, 230.17, 230.87 }; // mm
  Double_t rin2[nSects2]  = {  18.,  18.,   30.,    60.,    90.,   105.86, 110.   };
  Double_t rout2[nSects2] = {  18.,  28.69, 39.3,   65.55,  94.14, 110.,   110.   };
  TString pipevac1name = "pipevac1";
  const Int_t nSects01 = 10;
  Double_t z01[nSects01]    = { -50., -5.,  -5., 220., 220., 220.7,  221.45, 223.71, 227.49, 230.17 }; // mm
  Double_t rin01[nSects01]  = {   0.,  0.,   0.,   0.,  18.,  28.69,  39.3,   65.55,  94.14, 110.   };
  Double_t rout01[nSects01] = {  25., 25., 400., 400., 400., 400.,   400.,   400.,   400.,   400.   };

  TString pipe3name = "pipe3 - STS section";
  const Int_t nSects3 = 4;
  Double_t z3[nSects3]    = { 220.,  500.,  1250.,   1700.  }; // mm
  Double_t rout3[nSects3] = {  18.,   18.,    55.,     74.2 };
  Double_t rin3[nSects3]; for(Int_t i=0; i<nSects3; i++) { rin3[i] = rout3[i] - dPipeThickness; }
  TString pipevac2name = "pipevac2";
  const Int_t nSects02 = nSects3;
  Double_t z02[nSects02]    = { 220.,  500.,  1250.,   1700. }; // mm
  Double_t rin02[nSects02]  = {   0.,    0.,     0.,      0. };
  Double_t rout02[nSects02]; for(Int_t i=0; i<nSects02; i++) { rout02[i]=rin3[i]; }
*/  
/*************************************************************/  
  TString pipeNameMuch = "pipe4 - MUCH section"; // First 2.5 then 3.0 from 1850 mm up to TRD
  const Int_t nSectsMuch = 3;
  Double_t dZposMuch[nSectsMuch] = { 1700. ,  1850.,   3700.  }; // mm
  Double_t dRoutMuch[nSectsMuch] = {   74.2,    97.,    193.9 }; // mm
  Double_t dRinMuch[ nSectsMuch]; for(Int_t i=0; i<nSectsMuch; i++) { dRinMuch[i] = dRoutMuch[i] - dPipeThickness; }
  
  TString pipeVacNameMuch = "pipevac3";
  const Int_t nSectsVacMuch = nSectsMuch;
  Double_t dZposVacMuch[nSectsVacMuch]; for(Int_t i=0; i<nSectsVacMuch; i++) { dZposVacMuch[i] = dZposMuch[i]; }
  Double_t dRinVacMuch[ nSectsVacMuch]; for(Int_t i=0; i<nSectsVacMuch; i++) { dRinVacMuch[i]  = 0.; }
  Double_t dRoutVacMuch[nSectsVacMuch]; for(Int_t i=0; i<nSectsVacMuch; i++) { dRoutVacMuch[i] = dRinMuch[i]; }
/*************************************************************/
  
/*************************************************************/
  TString pipeNameTrd = "pipe5 - TRD section";
  const Int_t nSectsTrd = 2;
  Double_t dZposTrd[nSectsTrd] = { 3700.,   6700.   }; // mm
  Double_t dRoutTrd[nSectsTrd] = {  193.9,   288.00 }; // mm, 3.0 deg inner opening in, detector fit out
  Double_t dRinTrd[ nSectsTrd]; for(Int_t i=0; i<nSectsTrd; i++) { dRinTrd[i] = dRoutTrd[i] - dPipeThickness; }
  
  TString pipeVacNameTrd = "pipevac5";
  const Int_t nSectsVacTrd = nSectsTrd;
  Double_t dZposVacTrd[nSectsVacTrd]; for(Int_t i=0; i<nSectsVacTrd; i++) { dZposVacTrd[i] = dZposTrd[i]; }
  Double_t dRinVacTrd[ nSectsVacTrd]; for(Int_t i=0; i<nSectsVacTrd; i++) { dRinVacTrd[i]  = 0.; }
  Double_t dRoutVacTrd[nSectsVacTrd]; for(Int_t i=0; i<nSectsVacTrd; i++) { dRoutVacTrd[i] = dRinTrd[i]; }
/*************************************************************/

/*************************************************************/
  TString pipeNameTof = "pipe6 - TOF section";
  const Int_t nSectsTof = 3;
  Double_t dZposTof[nSectsTof] = { 6700.,   8250.,   8800. }; // mm
  Double_t dRoutTof[nSectsTof] = {  288.00,  212.,    212. }; // mm, 2.5 deg start, TOF hole fit exit 
  Double_t dRinTof[ nSectsTof]; for(Int_t i=0; i<nSectsTof; i++) { dRinTof[i] = dRoutTof[i] - dPipeThickness; }
  
  TString pipeVacNameTof = "pipevac6";
  const Int_t nSectsVacTof = nSectsTof;
  Double_t dZposVacTof[nSectsVacTof]; for(Int_t i=0; i<nSectsVacTof; i++) { dZposVacTof[i] = dZposTof[i]; }
  Double_t dRinVacTof[ nSectsVacTof]; for(Int_t i=0; i<nSectsVacTof; i++) { dRinVacTof[i]  = 0.; }
  Double_t dRoutVacTof[nSectsVacTof]; for(Int_t i=0; i<nSectsVacTof; i++) { dRoutVacTof[i] = dRinTof[i]; }
/*************************************************************/

/*************************************************************/
  TString pipeNameEnd = "pipe7 - downstream section";
  const Int_t nSectsEnd = 2;
  Double_t dZposEnd[nSectsEnd] = { dZposTof[nSectsTof - 1],   15000.   }; // mm
  Double_t dRoutEnd[nSectsEnd] = { dRoutTof[nSectsTof - 1],     655.31 }; // mm, TOF pipe fit at beg, 2.5 deg inner opening at end  
  Double_t dRinEnd[ nSectsEnd]; for(Int_t i=0; i<nSectsEnd; i++) { dRinEnd[i] = dRoutEnd[i] - dPipeThickness; }
  
  TString pipeVacNameEnd = "pipevac7";
  const Int_t nSectsVacEnd = nSectsEnd;
  Double_t dZposVacEnd[nSectsVacEnd]; for(Int_t i=0; i<nSectsVacEnd; i++) { dZposVacEnd[i] = dZposEnd[i]; }
  Double_t dRinVacEnd[ nSectsVacEnd]; for(Int_t i=0; i<nSectsVacEnd; i++) { dRinVacEnd[i]  = 0.; }
  Double_t dRoutVacEnd[nSectsVacEnd]; for(Int_t i=0; i<nSectsVacEnd; i++) { dRoutVacEnd[i] = dRinEnd[i]; }
/*************************************************************/

/*************************************************************/
// Straight window
  TString pipeNameWinEnd = "pipe8 - second window @ the end, h=0.2mm, R=600mm"; // iron !!!
  const Int_t nSectsWinEnd = 2;
  Double_t dZposWinEnd[nSectsWinEnd] = { dZposEnd[nSectsEnd - 1], dZposEnd[nSectsEnd - 1] + 0.2  }; // mm
  Double_t dRoutWinEnd[nSectsWinEnd] = { dRoutEnd[nSectsEnd - 1], dRoutEnd[nSectsEnd - 1] }; // mm
  Double_t dRinWinEnd[nSectsWinEnd]  = {                      0.,                      0. }; // mm
  if( kFALSE == bEnd )
  {
     // End section of beam pipe disabled, find which section is last and use its end radius for the window
     // If none is enabled, disable the end window
     if( kTRUE == bTof )
     {
        dZposWinEnd[0] = dZposTof[nSectsTof - 1];
        dZposWinEnd[1] = dZposTof[nSectsTof - 1] + 0.2; // mm
        dRoutWinEnd[0] = dRoutTof[nSectsTof - 1];
        dRoutWinEnd[1] = dRoutTof[nSectsTof - 1]; // mm
     } // if( kTRUE == bTof )
     else if( kTRUE == bTrd )
     {
        dZposWinEnd[0] = dZposTrd[nSectsTrd - 1];
        dZposWinEnd[1] = dZposTrd[nSectsTrd - 1] + 0.2; // mm
        dRoutWinEnd[0] = dRoutTrd[nSectsTrd - 1];
        dRoutWinEnd[1] = dRoutTrd[nSectsTrd - 1]; // mm
     } // else if( kTRUE == bTrd )
     else if( kTRUE == bMuch )
     {
        dZposWinEnd[0] = dZposMuch[nSectsMuch - 1];
        dZposWinEnd[1] = dZposMuch[nSectsMuch - 1] + 0.2; // mm
        dRoutWinEnd[0] = dRoutMuch[nSectsMuch - 1];
        dRoutWinEnd[1] = dRoutMuch[nSectsMuch - 1]; // mm
     } // else if( kTRUE == bMuch )
      else bWin = kFALSE;
  } // if( kFALSE == bEnd )
  
  TString pipeVacNameWinEnd = "pipevac8";
  const Int_t nSectsVacWinEnd = 0;
  
// Curved window
/*
  TString pipe6name = "pipe7 - second window @ 6000mm, h=0.2mm, R=600mm"; // iron !!!
  const Int_t nSects6 = 7;
  Double_t z6[nSects6]     = { 5943.24, 5943.44, 5947.34, 5959.8,  5981.19, 5999.8,  6000.  }; // mm
  Double_t rin6[nSects6]   = {    0.,      0.,     66.58,  138.88,  209.35,  252.28,  253.27 };
  Double_t rout6[nSects6]  = {    0.,      3.42,   70.,    140.,    210.,    253.23,  253.27 };
  
  TString pipeVacNameTrd = "pipevac8";
  const Int_t nSectsVacTrd = 7;
  Double_t dZposVacTrd[nSectsVacTrd]    = { 3700.,   5943.24, 5943.44, 5947.34, 5959.8,  5981.19, 5999.8  }; // mm
  Double_t dRinVacTrd[ nSectsVacTrd]  = {    0.,      0.,      3.42,   70.,    140.,    210.,    253.23 };
  //Double_t rout04[nSects04] = {  156.2,   250.87,  250.88,  251.05,  251.57,  252.48,  253.23 };
  Double_t dRoutVacTrd[nSectsVacTrd] = {  156.2,   250.84,  250.85,  251.01,  251.54,  252.44,  253.23 };
*/
/*************************************************************/
  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = rootFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  fstream infoFileEmpty;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "SIS-100. Beam pipe geometry created with " + macrosname << endl << endl;
   infoFile << " pipe_v16a_1m = pipe for the SIS100 muon setups" << endl << endl;
   infoFile << " The beam pipe is composed of carbon with a thickness of 0.5 mm (0.4 mm is" << endl;
   infoFile << " available from producer)." << endl;
   infoFile << " It is placed directly into the cave as mother volume." << endl;
   infoFile << " Each section has a PCON shape (including windows)." << endl;
   infoFile << " There are two windows: " << endl;
   infoFile << "   - first one @ 220mm with R600mm and 0.7mm thickness," << endl;
   infoFile << "   - second one of iron the end of the pipe with R600mm and 0.2mm thickness." << endl;
   infoFile << " The STS section is composed of cylinder D(z=220-500mm)=36mm and cone" << endl;
   infoFile << " (z=500-1700mm). " << endl;
   infoFile << " All other sections of the beam pipe fit either the 2.5 degree standard" << endl;
   infoFile << " limit or their respective detectors opening if less than 2.5 degree is" << endl;
   infoFile << " available." << endl;
   infoFile << " The PSD section of the beam pipe is missing because it is planned that it " << endl;
   infoFile << " will be part of PSD geometry." << endl << endl;

  
  infoFile << "Material:  " << pipeMediumName << endl;
  infoFile << "Thickness: D(z)mm/60" << endl << endl;
  // --------------------------------------------------------------------------


  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  TGeoManager* gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------



  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
  
  // ---> pipe medium
  FairGeoMedium* fPipeMedium = geoMedia->getMedium(pipeMediumName.Data());
  TString fairError = "FairMedium " + pipeMediumName + " not found";
  if ( ! fPipeMedium ) Fatal("Main", fairError.Data());
  geoBuild->createMedium(fPipeMedium);
  TGeoMedium* pipeMedium = gGeoMan->GetMedium(pipeMediumName.Data());
  TString geoError = "Medium " + pipeMediumName + " not found";
  if ( ! pipeMedium ) Fatal("Main", geoError.Data());
  // ---> iron
  FairGeoMedium* mIron = geoMedia->getMedium("iron");
  if ( ! mIron ) Fatal("Main", "FairMedium iron not found");
  geoBuild->createMedium(mIron);
  TGeoMedium* iron = gGeoMan->GetMedium("iron");
  if ( ! iron ) Fatal("Main", "Medium iron not found");
  // ---> vacuum
  FairGeoMedium* mVacuum = geoMedia->getMedium("vacuum");
  if ( ! mVacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mVacuum);
  TGeoMedium* vacuum = gGeoMan->GetMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "Medium vacuum not found");
  // --------------------------------------------------------------------------



  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("PIPEgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  TGeoVolume* pipe = new TGeoVolumeAssembly(pipeName.Data());
  // --------------------------------------------------------------------------


  // -----   Create sections  -------------------------------------------------
  infoFile << endl << "Beam pipe section: " << pipe1name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
  
  TGeoVolume* pipe1    = MakePipe  (1, nSects1,  z1,  rin1,  rout1,  pipeMedium, &infoFile); 
  pipe1->SetLineColor(kGray);
  pipe->AddNode(pipe1, 0);
  
  infoFile << endl << "Beam pipe section: " << pipe2name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
  TGeoVolume* pipe2    = MakePipe  (2, nSects2,  z2,  rin2,  rout2,  pipeMedium, &infoFile); 
  pipe2->SetLineColor(kBlue);
  pipe->AddNode(pipe2, 0);
  TGeoVolume* pipevac1 = MakeVacuum(1, nSects01, z01, rin01, rout01, vacuum,     &infoFile); 
  pipevac1->SetLineColor(kCyan);
  pipe->AddNode(pipevac1, 0);
  
  infoFile << endl << "Beam pipe section: " << pipe3name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
  TGeoVolume* pipe3    = MakePipe  (3, nSects3,  z3,  rin3,  rout3,  pipeMedium, &infoFile); 
  pipe3->SetLineColor(kGreen);
  pipe->AddNode(pipe3, 0);
  TGeoVolume* pipevac2 = MakeVacuum(2, nSects02, z02, rin02, rout02, vacuum,     &infoFile); 
  pipevac2->SetLineColor(kCyan);
  pipe->AddNode(pipevac2, 0);

   // Process MUCH pipe params
  if( 0 < nSectsMuch  && kTRUE == bMuch )
  {
     infoFile << endl << "Beam pipe section: " << pipeNameMuch << endl;
     infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
     TGeoVolume* pipeMuch    = MakePipe  (5, nSectsMuch,  dZposMuch,  dRinMuch,  dRoutMuch,  pipeMedium, &infoFile); 
     pipeMuch->SetLineColor(kGreen);
     pipe->AddNode(pipeMuch, 0);
     TGeoVolume* pipeVacMuch = MakeVacuum(5, nSectsVacMuch, dZposVacMuch, dRinVacMuch, dRoutVacMuch, vacuum,     &infoFile); 
     pipeVacMuch->SetLineColor(kCyan);
     pipe->AddNode(pipeVacMuch, 0);
  } // if( 0 < nSectsMuch  && kTRUE == bMuch )

   // Process TRD pipe params
  if( 0 < nSectsTrd  && kTRUE == bTrd )
  {
     infoFile << endl << "Beam pipe section: " << pipeNameTrd << endl;
     infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
     TGeoVolume* pipeTrd    = MakePipe  (5, nSectsTrd,  dZposTrd,  dRinTrd,  dRoutTrd,  pipeMedium, &infoFile); 
     pipeTrd->SetLineColor(kGreen);
     pipe->AddNode(pipeTrd, 0);
     TGeoVolume* pipeVacTrd = MakeVacuum(5, nSectsVacTrd, dZposVacTrd, dRinVacTrd, dRoutVacTrd, vacuum,     &infoFile); 
     pipeVacTrd->SetLineColor(kCyan);
     pipe->AddNode(pipeVacTrd, 0);
  } // if( 0 < nSectsTrd  && kTRUE == bTrd )

   // Process TOF pipe params
  if( 0 < nSectsTof  && kTRUE == bTof )
  {
     infoFile << endl << "Beam pipe section: " << pipeNameTof << endl;
     infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
     TGeoVolume* pipeTof    = MakePipe  (6, nSectsTof,  dZposTof,  dRinTof,  dRoutTof,  pipeMedium, &infoFile); 
     pipeTof->SetLineColor(kGreen);
     pipe->AddNode(pipeTof, 0);
     TGeoVolume* pipeVacTof = MakeVacuum(6, nSectsVacTof, dZposVacTof, dRinVacTof, dRoutVacTof, vacuum,     &infoFile); 
     pipeVacTof->SetLineColor(kCyan);
     pipe->AddNode(pipeVacTof, 0);
  } // if( 0 < nSectsTof  && kTRUE == bTof )
  
   // Process downstream pipe params
  if( 0 < nSectsEnd  && kTRUE == bEnd )
  {
     infoFile << endl << "Beam pipe section: " << pipeNameEnd << endl;
     infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
     TGeoVolume* pipeEnd    = MakePipe  (5, nSectsEnd,  dZposEnd,  dRinEnd,  dRoutEnd,  pipeMedium, &infoFile); 
     pipeEnd->SetLineColor(kGreen);
     pipe->AddNode(pipeEnd, 0);
     TGeoVolume* pipeVacEnd = MakeVacuum(5, nSectsVacEnd, dZposVacEnd, dRinVacEnd, dRoutVacEnd, vacuum,     &infoFile); 
     pipeVacEnd->SetLineColor(kCyan);
     pipe->AddNode(pipeVacEnd, 0);
  } // if( 0 < nSectsEnd  && kTRUE == bEnd )
  
  // Process final window params
  if( 0 < nSectsWinEnd  && kTRUE == bWin )
  {
     infoFile << endl << "Beam pipe section: " << pipeNameWinEnd << ", material: iron" << endl;
     infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
     TGeoVolume* pipeWinEnd = MakePipe(8, nSectsWinEnd,  dZposWinEnd,  dRinWinEnd,  dRoutWinEnd, iron, &infoFile);
     pipeWinEnd->SetLineColor(kBlue);
     pipe->AddNode(pipeWinEnd, 0);
     
//     if( 0 < nSectsVacWinEnd )
//     {
//        TGeoVolume* pipeVacWinEnd = MakeVacuum(8, nSectsVacWinEnd, dZposVacWinEnd, dRinVacWinEnd, dRoutVacWinEnd, vacuum,     &infoFile); 
//        pipeVacWinEnd->SetLineColor(kCyan);
//        pipe->AddNode(pipeVacWinEnd, 0);
//     } // if( 0 < nSectsVacWinEnd )
  } // if( 0 < nSectsWinEnd  && kTRUE == bWin )
  
  // -----   End   --------------------------------------------------

  // ---------------   Finish   -----------------------------------------------
  top->AddNode(pipe, 1);
  cout << endl << endl;
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();
  
  TFile* rootFile = new TFile(rootFileName, "RECREATE");
  top->Write();
  cout << endl;
  cout << "Geometry " << top->GetName() << " written to " 
       << rootFileName << endl;
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
TGeoPcon* MakeShape(Int_t nSects, char* name, Double_t* z, Double_t* rin, 
                    Double_t* rout, fstream* infoFile) {

  // ---> Shape
  TGeoPcon* shape = new TGeoPcon(name, 0., 360., nSects);
  for (Int_t iSect = 0; iSect < nSects; iSect++) {
    shape->DefineSection(iSect, z[iSect]/10., rin[iSect]/10., rout[iSect]/10.); // mm->cm
    *infoFile << setw(2)  << iSect+1
              << setw(10) << fixed << setprecision(2) << z[iSect]
              << setw(10) << fixed << setprecision(2) << rin[iSect]
              << setw(10) << fixed << setprecision(2) << rout[iSect]
              << setw(10) << fixed << setprecision(2) << rout[iSect]-rin[iSect] << endl;
  }

  return shape;

}
// ============================================================================




// =====  Make the beam pipe volume   =========================================
TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, 
	                 Double_t* rout, TGeoMedium* medium, fstream* infoFile) {

  // ---> Shape
  TString volName = Form("pipe%i", iPart);
  TGeoPcon* shape = new TGeoPcon(volName.Data(), 0., 360., nSects);
  for (Int_t iSect = 0; iSect < nSects; iSect++) {
    shape->DefineSection(iSect, z[iSect]/10., rin[iSect]/10., rout[iSect]/10.); // mm->cm
    *infoFile << setw(2)  << iSect+1
              << setw(10) << fixed << setprecision(2) << z[iSect]
              << setw(10) << fixed << setprecision(2) << rin[iSect]
              << setw(10) << fixed << setprecision(2) << rout[iSect]
              << setw(10) << fixed << setprecision(2) << rout[iSect]-rin[iSect] << endl;
  }

  // ---> Volume
  TGeoVolume* pipe = new TGeoVolume(volName.Data(), shape, medium);

  return pipe;

}
// ============================================================================



// =====   Make the volume for the vacuum inside the beam pipe   ==============
TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin, 
	                   Double_t* rout, TGeoMedium* medium, fstream* infoFile) {

  // ---> Shape
  TString volName = Form("pipevac%i", iPart);
  TGeoPcon* shape = new TGeoPcon(volName.Data(), 0., 360., nSects);
  for (Int_t iSect = 0; iSect < nSects; iSect++) {
    shape->DefineSection(iSect, z[iSect]/10., rin[iSect]/10., rout[iSect]/10.); // mm->cm
  }

  // ---> Volume
  TGeoVolume* pipevac = new TGeoVolume(volName.Data(), shape, medium);

  return pipevac;

}
// ============================================================================
