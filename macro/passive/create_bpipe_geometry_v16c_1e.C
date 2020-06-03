/******************************************************************************
 ** Creation of beam pipe geometry in ROOT format (TGeo).
 **
 ** @file create_bpipe_geometry_v16c_1e.C
 ** @author David Emschermann <d.emschermann@gsi.de>
 ** @author Andrey Chernogorov <a.chernogorov@gsi.de>
 ** @date 19.07.2016
 **
 ** SIS-100
 ** pipe v16c_1e - is a pipe for the STS up to the interface to RICH at z = 1700 mm
 **                with a (blue) flange at the downstream end of the STS box
 **
 ** The beam pipe is composed of carbon with a fixed wall thickness of 0.5 or 1.0 mm.
 ** It is placed directly into the cave as mother volume. The beam pipe consists of 
 ** few sections up to the RICH section (1700-3700mm), which is part of the RICH geometry. 
 ** Each section has a PCON shape (including windows).
 ** The STS section is composed of cylinder D(z=220-410mm)=34mm and cone (z=410-1183mm). 
 ** All sections of the beam pipe with conical shape have half opening angle 2.5deg.
 *****************************************************************************/




#include <iomanip>
#include <iostream>
#include "TGeoManager.h"


using namespace std;


// -------------   Steering variables       -----------------------------------
// ---> Beam pipe material name
TString pipeMediumName = "carbon"; // "beryllium" "aluminium"
// ----------------------------------------------------------------------------


// -------------   Other global variables   -----------------------------------
// ---> Macro name to info file
TString macroname = "create_bpipe_geometry_v16c_1e.C";
// ---> Geometry file name (output)
TString rootFileName = "pipe_v16c_1e.geo.root";
// ---> Geometry name
TString pipeName = "pipe_v16c_1e";
// ----------------------------------------------------------------------------

TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
                     Double_t* rout, TGeoMedium* medium, fstream* infoFile);

TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
                       Double_t* rout, TGeoMedium* medium, fstream* infoFile);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_bpipe_geometry_v16c_1e()
{
  // -----   Define beam pipe sections   --------------------------------------
  /** For v16c_1e:   **/
  TString pipe1name = "pipe1 - vacuum chamber";
  const Int_t nSects1 = 6;
  Double_t z1[nSects1]    = { -50.,  -5.,   -5.,  230.17, 230.17, 230.87 }; // mm
  Double_t rin1[nSects1]  = {  25.,  25.,  400.,  400.,   110.,   110.   };
  Double_t rout1[nSects1] = {  25.7, 25.7, 400.7, 400.7,  400.7,  130.7  };
  TString pipe2name = "pipe2 - first window @ 220mm, h=0.7mm, R=600mm";
  const Int_t nSects2 = 7;
  Double_t z2[nSects2]      = { 220., 220.7, 221.45, 223.71, 227.49, 230.17, 230.87 }; // mm
  Double_t rin2[nSects2]    = {  17.,  17.,   30.,    60.,    90.,   105.86, 110.   };
  Double_t rout2[nSects2]   = {  17.,  28.69, 39.3,   65.55,  94.14, 110.,   110.   };
  TString pipevac1name = "pipevac1";
  const Int_t nSects01 = 10;
  Double_t z01[nSects01]    = { -50., -5.,  -5., 220., 220., 220.7,  221.45, 223.71, 227.49, 230.17 }; // mm
  Double_t rin01[nSects01]  = {   0.,  0.,   0.,   0.,  17.,  28.69,  39.3,   65.55,  94.14, 110.   };
  Double_t rout01[nSects01] = {  25., 25., 400., 400., 400., 400.,   400.,   400.,   400.,   400.   };

  // tan (acos(-1)/180 * 2.5) *  30 cm = 1.310 cm
  // tan (acos(-1)/180 * 2.5) *  40 cm = 1.746 cm
  // tan (acos(-1)/180 * 2.5) *  50 cm = 2.183 cm
  // tan (acos(-1)/180 * 2.5) *  60 cm = 2.620 cm
  // tan (acos(-1)/180 * 2.5) *  70 cm = 3.056 cm
  // tan (acos(-1)/180 * 2.5) *  80 cm = 3.493 cm
  // tan (acos(-1)/180 * 2.5) *  90 cm = 3.929 cm
  // tan (acos(-1)/180 * 2.5) * 100 cm = 4.366 cm
  // tan (acos(-1)/180 * 2.5) * 120 cm = 5.239 cm

  //  const Double_t pipewallthickness = 0.5; // mm
  const Double_t pipewallthickness = 1.0; // mm
  
  TString pipe3name = "pipe3 - STS section";
//  const Int_t nSects3 = 3;
//  Double_t z3[nSects3]      = { 220.,  500.,  1250. }; // mm
//  Double_t rout3[nSects3]   = {  18.,   18.,    55. };
//  Double_t rin3[nSects3]; for(Int_t i=0; i<nSects3; i++) { rin3[i] = rout3[i] - rout3[i]/30.; }
  const Int_t nSects3 = 5;
  Double_t z3[nSects3]      = { 220.,  410.,  1183.,  1183.,  1188. }; // mm Wolfgang's drawing
  //  Double_t z3[nSects3]      = { 220.,  400.,  1183.,  1183.,  1188. }; // mm Wolfgang's drawing
  Double_t rout3[nSects3]   = {34/2., 34/2.,    55.+pipewallthickness, 
			                                85.,    85. };
  Double_t rin3[nSects3]    = { 999.,  999.,    55.,    55.,    55. };
  for(Int_t i=0; i<2; i++) { rin3[i] = rout3[i] - pipewallthickness; }
  TString pipevac3name = "pipevac3";
  const Int_t nSects03 = nSects3;
  Double_t z03[nSects03]    = { 220.,  410.,  1183.,  1183.,  1188. }; // mm Wolfgang's drawing
  //  Double_t z03[nSects03]    = { 220.,  400.,  1183.,  1183.,  1188. }; // mm Wolfgang's drawing
  Double_t rin03[nSects03]  = {   0.,    0.,     0.,     0.,     0. };
  Double_t rout03[nSects03]; for(Int_t i=0; i<nSects03; i++) { rout03[i]=rin3[i]; }

  TString pipe4name = "pipe4 - STS flange";
  const Int_t nSects4 = 6;
  Double_t z4[nSects4]      = { 1188.,  1198.,  1198.,  1215.,  1215.,  1225. }; // mm
  Double_t rout4[nSects4]   = {   85.,    85.,    55.+pipewallthickness,    
                                                          55.+pipewallthickness,
                                                                  85.,    85. };
  Double_t rin4[nSects4]    = {   55.,    55.,    55.,    55.,    55.,    55. };
  //  Double_t rin4[nSects4]; for(Int_t i=0; i<nSects4; i++) { rin4[i] = rout4[i] - pipewallthickness; }
  TString pipevac4name = "pipevac4";
  const Int_t nSects04 = 2; // nSects4;
  Double_t z04[nSects04]    = { 1188.,  1225. }; // mm
  Double_t rin04[nSects04]  = {    0.,     0. };
  Double_t rout04[nSects04]; for(Int_t i=0; i<nSects04; i++) { rout04[i]=rin4[i]; }

  TString pipe5name = "pipe5 - STS to RICH junction";
  const Int_t nSects5 = 4;
  Double_t z5[nSects5]      = { 1225.,  1230.,  1230.,  1700.  }; // mm
  Double_t rout5[nSects5]   = {   85.,    85.,    55.+pipewallthickness,
				                         74.2 };
  Double_t rin5[nSects5]    = {   55.,    55.,    55.,     74.2-pipewallthickness}; 
  TString pipevac5name = "pipevac5";
  const Int_t nSects05 = nSects5;
  Double_t z05[nSects05]    = { 1225.,  1230.,  1230.,  1700. }; // mm
  Double_t rin05[nSects05]  = {    0.,     0.,     0.,     0. };
  Double_t rout05[nSects05]; for(Int_t i=0; i<nSects05; i++) { rout05[i]=rin5[i]; }

  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = rootFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  fstream infoFileEmpty;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "SIS-100. Beam pipe geometry created with " + macroname << endl << endl;
  infoFile << "  pipe v16c_1e - is a pipe for the STS up to the interface to RICH at z = 1700 mm" << endl << endl;
  infoFile << "                 with a (blue) flange at the downstream end of the STS box" << endl << endl;
  infoFile << "The beam pipe is composed of carbon with a fixed wall thickness of 0.5 or 1.0 mm." << endl;
  infoFile << "It is placed directly into the cave as mother volume. The beam pipe consists of" << endl;
  infoFile << "few sections up to the RICH section (1700-3700mm), which is part of the RICH geometry." << endl;
  infoFile << "Each section has a PCON shape (including windows)." << endl;
  infoFile << "The STS section is composed of cylinder D(z=220-410mm)=34mm and cone (z=410-1183mm)." << endl;
  infoFile << "All sections of the beam pipe with conical shape have half opening angle 2.5deg." << endl << endl;

  infoFile << "Material:  " << pipeMediumName << endl;
  infoFile << "Wall Thickness: " << pipewallthickness << " mm" << endl << endl;
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

//  // ---> iron
//  FairGeoMedium* mIron = geoMedia->getMedium("iron");
//  if ( ! mIron ) Fatal("Main", "FairMedium iron not found");
//  geoBuild->createMedium(mIron);
//  TGeoMedium* iron = gGeoMan->GetMedium("iron");
//  if ( ! iron ) Fatal("Main", "Medium iron not found");

//  // ---> lead
//  FairGeoMedium* mLead = geoMedia->getMedium("lead");
//  if ( ! mLead ) Fatal("Main", "FairMedium lead not found");
//  geoBuild->createMedium(mLead);
//  TGeoMedium* lead = gGeoMan->GetMedium("lead");
//  if ( ! lead ) Fatal("Main", "Medium lead not found");

//  // ---> carbon
//  FairGeoMedium* mCarbon = geoMedia->getMedium("carbon");
//  if ( ! mCarbon ) Fatal("Main", "FairMedium carbon not found");
//  geoBuild->createMedium(mCarbon);
//  TGeoMedium* carbon = gGeoMan->GetMedium("carbon");
//  if ( ! carbon ) Fatal("Main", "Medium carbon not found");

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
  pipe1->SetLineColor(kYellow);
  //  pipe1->SetLineColor(kGray);
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
  TGeoVolume* pipevac3 = MakeVacuum(3, nSects03, z03, rin03, rout03, vacuum,     &infoFile); 
  pipevac3->SetLineColor(kCyan);
  pipe->AddNode(pipevac3, 0);

  infoFile << endl << "Beam pipe section: " << pipe4name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
  TGeoVolume* pipe4    = MakePipe  (4, nSects4,  z4,  rin4,  rout4,  pipeMedium, &infoFile); 
  pipe4->SetLineColor(kBlue);
  pipe->AddNode(pipe4, 0);
  TGeoVolume* pipevac4 = MakeVacuum(4, nSects04, z04, rin04, rout04, vacuum,     &infoFile); 
  pipevac4->SetLineColor(kCyan);
  pipe->AddNode(pipevac4, 0);

  infoFile << endl << "Beam pipe section: " << pipe5name << endl;
  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
  TGeoVolume* pipe5    = MakePipe  (5, nSects5,  z5,  rin5,  rout5,  pipeMedium, &infoFile); 
  pipe5->SetLineColor(kGreen);
  pipe->AddNode(pipe5, 0);
  TGeoVolume* pipevac5 = MakeVacuum(5, nSects05, z05, rin05, rout05, vacuum,     &infoFile); 
  pipevac5->SetLineColor(kCyan);
  pipe->AddNode(pipevac5, 0);

  // -----   End   --------------------------------------------------

  // ---------------   Finish   -----------------------------------------------
  top->AddNode(pipe, 1);
  cout << endl << endl;
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();
  
  // visualize it with ray tracing, OGL/X3D viewer
  //top->Raytrace();
  top->Draw("ogl");
  //top->Draw("x3d");

  TFile* rootFile = new TFile(rootFileName, "RECREATE");
  top->Write();
  cout << endl;
  cout << "Geometry " << top->GetName() << " written to " 
       << rootFileName << endl;
  rootFile->Close();
  infoFile.close();


}
// ============================================================================
// ======                   End of main function                          =====
// ============================================================================


//// =====  Make the beam pipe volume   =========================================
//TGeoPcon* MakeShape(Int_t nSects, char* name, Double_t* z, Double_t* rin, 
//                    Double_t* rout, fstream* infoFile) {
//
//  // ---> Shape
//  TGeoPcon* shape = new TGeoPcon(name, 0., 360., nSects);
//  for (Int_t iSect = 0; iSect < nSects; iSect++) {
//    shape->DefineSection(iSect, z[iSect]/10., rin[iSect]/10., rout[iSect]/10.); // mm->cm
//    *infoFile << setw(2)  << iSect+1
//              << setw(10) << fixed << setprecision(2) << z[iSect]
//              << setw(10) << fixed << setprecision(2) << rin[iSect]
//              << setw(10) << fixed << setprecision(2) << rout[iSect]
//              << setw(10) << fixed << setprecision(2) << rout[iSect]-rin[iSect] << endl;
//  }
//
//  return shape;
//
//}
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
