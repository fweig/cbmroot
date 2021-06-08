/* Copyright (C) 2016-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: David Emschermann, Florian Uhlig [committer] */

/******************************************************************************
 ** Creation of beam pipe geometry in ROOT format (TGeo).
 **
 ** @file create_bpipe_geometry_v19d.C
 ** @author David Emschermann <d.emschermann@gsi.de>
 ** @author Andrey Chernogorov <a.chernogorov@gsi.de>
 ** @date 19.07.2016
 **
 ** mCBM
 ** pipe v19d    - create two separate volumes, one with the targetbox the
 **                other one with the pipe
 ** pipe v19b    - build target box from CAD design
 ** pipe v19a    - adapt dimensions of beampipe to technical drawings
 ** pipe v18g    - rotate 2-diameter beampipe v18f to 25 degrees
 ** pipe v18f    - increase pipe length from 3.00 m to 4.00 m
 ** pipe v18f    - reduce diameter of first 50 cm of beampipe to avoid collision with mSTS
 ** pipe v18e    - rotate cylindrical pipe around the vertical (y) axis by 20 degrees
 ** pipe v18d    - rotate cylindrical pipe around the vertical (y) axis by 25 degrees
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

// dimensions from z = -38.0 cm to z = +59.8 cm
// dimensions of core [-29.9 cm .. +15.2 cm]
// dimensions of sections -38.0 cm | + 7.7 + 0.4 + 45.1 + 0.6 + 44.0 | +59.8 cm

// naming scheme
// main elements - flanges
// pipe10 - pipe11
// pipe20 - pipe21, 22, 23, 24, 25
// pipe30 - pipe31


#include "TGeoManager.h"
#include "TGeoPcon.h"
#include "TGeoTube.h"

#include <iomanip>
#include <iostream>

using namespace std;


// -------------   Steering variables       -----------------------------------
// ---> Beam pipe material name
TString pipeMediumName = "iron";  // "carbon"; // "beryllium"; // "aluminium";
// ----------------------------------------------------------------------------


// -------------   Other global variables   -----------------------------------
// ---> Macro name to info file
TString macroname = "create_bpipe_geometry_v19d.C";
// ---> Geometry file name (output)
TString targetBoxFileName = "pipe_v19d_tb_mcbm.geo.root";
TString pipeFileName      = "pipe_v19d_mcbm.geo.root";
// ---> Geometry name
TString targetBoxName = "targetbox_v19d";
TString pipeName      = "pipe_v19d";
// ----------------------------------------------------------------------------

TGeoVolume* MakeCutPipe(Int_t ipart, TGeoMedium* medium, Double_t rmin, Double_t rmax, Double_t dz, Double_t angle1,
                        Double_t angle2, Double_t nlo1, Double_t nlo2, Double_t nlo3, Double_t nhi1, Double_t nhi2,
                        Double_t nhi3);

//TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
//                     Double_t* rout, TGeoMedium* medium, fstream* infoFile);
//
//TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
//                       Double_t* rout, TGeoMedium* medium, fstream* infoFile);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_bpipe_geometry_v19d()
{
  // -----   Define beam pipe sections   --------------------------------------
  /** For v19d:   **/
  TString pipe1name = "pipe1 - straight miniCBM beampipe";

  // start and stop angles of beampipe

  //  Double_t angle1 = 180;  // lower half
  //  Double_t angle2 =   0;  // lower half
  //
  //  Double_t angle1 = 190;  // open cut @ -x
  //  Double_t angle2 = 170;  // open cut @ -x

  Double_t angle1 = 0;    // closed
  Double_t angle2 = 360;  // closed

  Double_t nlow[3];
  nlow[0] = 0;
  nlow[1] = 0;
  nlow[2] = -1;

  Double_t theta = 25. * TMath::Pi() / 180.;
  Double_t phi   = 180. * TMath::Pi() / 180.;

  Double_t nhi[3];
  nhi[0] = TMath::Sin(theta) * TMath::Cos(phi);
  nhi[1] = TMath::Sin(theta) * TMath::Sin(phi);
  nhi[2] = TMath::Cos(theta);

  Double_t pipe_angle = 25.;  // rotation angle around y-axis

  // tan (acos(-1)/180 * 2.5) *  30 cm = 1.310 cm

  cout << "1 - lx: " << nlow[0] << " ly: " << nlow[1] << " lz: " << nlow[2] << endl;
  cout << "2 - hx: " << nhi[0] << " hy: " << nhi[1] << " hz: " << nhi[2] << endl;

  // end of thin beampipe in reality: 610 mm downstream of target

  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = targetBoxFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  fstream infoFileEmpty;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "SIS-18 mCBM beam pipe geometry created with " + macroname << endl;
  infoFile << "Introducing the target chamber derived from CAD drawings." << endl << endl;
  //  infoFile << "It ends at z=610 mm downstream of the target." << endl << endl;
  infoFile << "The targetbox and the pipe after the targetbox are implemented" << endl;
  infoFile << "into two different TGeoVolumeAssemblies and exported into two" << endl;
  infoFile << "different output files which both needs to be loaded in the" << endl;
  infoFile << "simulation. This separation was done due to work around a" << endl;
  infoFile << "problem with the TGeoManager." << endl << endl;
  infoFile << "The beam pipe is composed of iron with a varying wall thickness." << endl << endl;
  infoFile << "Material:  " << pipeMediumName << endl;
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
  if (!fPipeMedium) Fatal("Main", "%s", fairError.Data());
  geoBuild->createMedium(fPipeMedium);
  TGeoMedium* pipeMedium = gGeoMan->GetMedium(pipeMediumName.Data());
  TString geoError       = "Medium " + pipeMediumName + " not found";
  if (!pipeMedium) Fatal("Main", "%s", geoError.Data());

  // ---> iron
  FairGeoMedium* mIron = geoMedia->getMedium("iron");
  if (!mIron) Fatal("Main", "FairMedium iron not found");
  geoBuild->createMedium(mIron);
  TGeoMedium* iron = gGeoMan->GetMedium("iron");
  if (!iron) Fatal("Main", "Medium iron not found");

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
  TGeoVolume* pipe      = new TGeoVolumeAssembly(pipeName.Data());
  TGeoVolume* targetbox = new TGeoVolumeAssembly(targetBoxName.Data());
  // --------------------------------------------------------------------------


  // -----   Create sections  -------------------------------------------------
  Int_t i = 0;

  // Aussendurchmesser 35.56 cm
  // Wanddicke           0.3 cm
  // Laenge             45.1 cm
  // Offset             29.9 cm

  Double_t rmax20   = 35.56 / 2.;
  Double_t rmin20   = rmax20 - 0.3;
  Double_t length20 = 45.1;
  Double_t offset20 = 29.9;

  TGeoVolume* vpipe20    = gGeoManager->MakeCtub("pipe20", pipeMedium, rmin20, rmax20, length20 / 2., angle1, angle2,
                                              nlow[0], nlow[1], nlow[2], nhi[0], nhi[1], nhi[2]);
  TGeoTranslation* tra20 = new TGeoTranslation("tra20", 0, 0, -offset20 + length20 / 2.);
  vpipe20->SetLineColor(kBlue);
  targetbox->AddNode(vpipe20, 1, tra20);

  TGeoVolume* vvacu20 = gGeoManager->MakeCtub("vacu20", vacuum, 0, rmin20, length20 / 2., angle1, angle2, nlow[0],
                                              nlow[1], nlow[2], nhi[0], nhi[1], nhi[2]);
  vvacu20->SetLineColor(kYellow);
  vvacu20->SetTransparency(50);
  targetbox->AddNode(vvacu20, 1, tra20);

  // upstream cover
  Double_t rmax21   = rmax20;
  Double_t rmin21   = 15.9 / 2.;
  Double_t length21 = 0.4;

  TGeoVolume* vwall21 =
    gGeoManager->MakeCtub("wall21", pipeMedium, rmin21, rmax21, length21 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  TGeoTranslation* tra21 = new TGeoTranslation("tra21", 0, 0, -offset20 - length21 / 2.);
  vwall21->SetLineColor(kBlue);
  targetbox->AddNode(vwall21, 1, tra21);

  //=======================================================================================

  // downstream cover with cutout
  Double_t rmax22   = rmax20;
  Double_t rmin22   = 5.4 / 2.;
  Double_t length22 = 0.6 / cos(25. * acos(-1.) / 180.);  // compensate for 25 degree rotation

  TGeoCtub* cdown =
    new TGeoCtub(rmin22, rmax22, length22 / 2., angle1, angle2, -nhi[0], -nhi[1], -nhi[2], nhi[0], nhi[1], nhi[2]);
  cdown->SetName("O");  // shapes need names too

  TGeoBBox* box22 = new TGeoBBox(11.5 / 2., 10.0 / 2., 2.0 / 2.);
  box22->SetName("A");  // shapes need names too

  TGeoTranslation* tcut1 = new TGeoTranslation("tcut1", -10.0, 0., 0.);
  tcut1->RegisterYourself();

  Double_t fthick22 = 0.6;
  TGeoBBox* frame22 = new TGeoBBox(13.9 / 2., 12.4 / 2., fthick22 / 2.);
  frame22->SetName("F");  // shapes need names too

  TGeoTranslation* tfra1 = new TGeoTranslation("tfra1", -10.0, 0., fthick22 - 0.0001);  // 0.0001 avoids optical error
  tfra1->RegisterYourself();

  // kapton foil frame dimensions - inner dimensions - frame width
  // x/y/z - 13.9/12.4/0.6 cm     - 11.5/10.0 cm     - 1.2 cm

  // corner of pipe at
  // x = -2.70
  // z = 14.54 = tan(25.*acos(-1.)/180.) * -5.4/2. + 15.8

  // corner of frame towards beampipe
  // x = -3.74
  // z = 14.06
  // distance = sqrt( 1.04 * 1.04 + 0.48 * 0.48 ) = 1.14 cm

  // rotate clockwise
  TGeoRotation* rot22 = new TGeoRotation();
  rot22->RotateY(-25.);
  TGeoCombiTrans* frot22 = new TGeoCombiTrans("frot22", 0, 0, 0, rot22);
  frot22->RegisterYourself();

  // rotate counter clockwise
  TGeoRotation* back22 = new TGeoRotation();
  back22->RotateY(+25.);
  TGeoCombiTrans* brot22 = new TGeoCombiTrans("brot22", 0, 0, 0, back22);
  brot22->RegisterYourself();

  //  TGeoCombiTrans*  tcut = new TGeoCombiTrans("tcut",
  //                          cos(25.*acos(-1.)/180.) * -10.0, 0., sin(25.*acos(-1.)/180.) * -10.0, rot22);
  //  tcut->RegisterYourself();

  // cutout inner border at:
  //  x : cos(25.*acos(-1.)/180.) *  -4.25              : x =  -3.852 cm
  //  z : sin(25.*acos(-1.)/180.) *  -4.25 + 15.2 + 0.3 : z =  13.704 cm

  // cutout outer border at:
  //  x : cos(25.*acos(-1.)/180.) * -15.75              : x = -14.274 cm
  //  z : sin(25.*acos(-1.)/180.) * -15.75 + 15.2 + 0.3 : z =   8.843 cm

  //  TGeoCompositeShape *compsha = new TGeoCompositeShape("compsha", "O - A:tcut");
  //  TGeoVolume *vpipe22 = new TGeoVolume("wall22", compsha, pipeMedium);
  //  TGeoTranslation* tra22 = new TGeoTranslation("tra22", 0, 0, -offset20 +length20 +length22/2.);

  TGeoCompositeShape* compsha = new TGeoCompositeShape("compsha", "O:brot22 + F:tfra1 - A:tcut1");
  TGeoVolume* vpipe22         = new TGeoVolume("wall22", compsha, pipeMedium);
  TGeoCombiTrans* tra22       = new TGeoCombiTrans("tra22", 0, 0, -offset20 + length20 + length22 / 2., rot22);
  vpipe22->SetLineColor(kBlue);
  targetbox->AddNode(vpipe22, 1, tra22);

  //=======================================================================================

  // vertical tubes
  Double_t height23   = 28.0 - 17.8;
  TGeoRotation* rot23 = new TGeoRotation();
  rot23->RotateX(90.);

  // target tube
  TGeoVolume* tube23    = gGeoManager->MakeTube("shaft23", pipeMedium, 10.4 / 2., 10.8 / 2., height23 / 2.);
  TGeoCombiTrans* tra23 = new TGeoCombiTrans("tra23", 0, 28.0 - height23 / 2., 0, rot23);
  tube23->SetLineColor(kBlue);
  targetbox->AddNode(tube23, 1, tra23);

  // diamond tube
  TGeoVolume* tube24    = gGeoManager->MakeTube("shaft24", pipeMedium, 10.4 / 2., 10.8 / 2., height23 / 2.);
  TGeoCombiTrans* tra24 = new TGeoCombiTrans("tra24", 0, 28.0 - height23 / 2., -20.0, rot23);
  tube24->SetLineColor(kBlue);
  targetbox->AddNode(tube24, 1, tra24);

  //=======================================================================================

  // upstream pipe
  Double_t rmax10   = 15.9 / 2.;
  Double_t rmin10   = rmax10 - 0.2;
  Double_t length10 = 7.7 + length21;

  TGeoVolume* vpipe10 =
    gGeoManager->MakeCtub("pipe10", pipeMedium, rmin10, rmax10, length10 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  TGeoTranslation* tra10 = new TGeoTranslation("tra10", 0, 0, -offset20 - length10 / 2.);
  vpipe10->SetLineColor(kBlue);
  targetbox->AddNode(vpipe10, 1, tra10);

  TGeoVolume* vvacu10 =
    gGeoManager->MakeCtub("vacu10", vacuum, 0, rmin10, length10 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  vvacu10->SetLineColor(kYellow);
  targetbox->AddNode(vvacu10, 1, tra10);

  // upstream flange
  // size of flange
  // Diameter  19.9 cm
  // Thickness  1.8 cm

  Double_t rmax11   = 19.9 / 2.;
  Double_t rmin11   = rmax10;
  Double_t length11 = 1.8;

  TGeoVolume* vfla11 =
    gGeoManager->MakeCtub("flange11", pipeMedium, rmin11, rmax11, length11 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  TGeoTranslation* tra11 = new TGeoTranslation("tra11", 0, 0, -offset20 - length10 + length11 / 2.);
  vfla11->SetLineColor(kBlue);
  targetbox->AddNode(vfla11, 1, tra11);

  //=======================================================================================

  // Laenge       44.0 cm
  // Durchmesser   5.4 cm
  // Wanddicke     0.2 cm

  // downstream pipe
  Double_t rmax30   = 5.4 / 2.;
  Double_t rmin30   = rmax30 - 0.2;
  Double_t length30 = 44.0 + 0.6;

  TGeoVolume* vpipe30 = gGeoManager->MakeCtub("pipe30", pipeMedium, rmin30, rmax30, length30 / 2., angle1, angle2,
                                              -nhi[0], -nhi[1], -nhi[2], -nlow[0], -nlow[1], -nlow[2]);
  vpipe30->SetLineColor(kBlue);
  pipe->AddNode(vpipe30, 1);

  TGeoVolume* vvacu30 = gGeoManager->MakeCtub("vacu30", vacuum, 0, rmin30, length30 / 2., angle1, angle2, -nhi[0],
                                              -nhi[1], -nhi[2], -nlow[0], -nlow[1], -nlow[2]);
  vvacu30->SetLineColor(kYellow);
  pipe->AddNode(vvacu30, 1);

  // size of flange
  // Thickness  1.8 cm
  // Diameter  11.4 cm

  Double_t rmax40   = 11.4 / 2.;
  Double_t rmin40   = rmax30;
  Double_t length40 = 1.8;

  // downstream flange
  TGeoVolume* vfla31 =
    gGeoManager->MakeCtub("flange31", pipeMedium, rmin40, rmax40, length40 / 2., angle1, angle2, 0, 0, -1, 0, 0, 1);
  TGeoTranslation* fla31 = new TGeoTranslation("fla31", 0., 0., +length30 / 2 - length40 / 2.);
  vfla31->SetLineColor(kBlue);
  pipe->AddNode(vfla31, 1, fla31);


  //=======================================================================================

  //  infoFile << endl << "Beam pipe section: " << pipe1name << endl;
  //  infoFile << setw(2) << "i" << setw(10) << "Z,mm" << setw(10) << "Rin,mm" << setw(10) << "Rout,mm" << setw(10) << "h,mm" << endl;
  //  TGeoVolume* pipe1    = MakePipe  (1, nSects1,  z1,  rin1,  rout1,  pipeMedium, &infoFile);
  //  pipe1->SetLineColor(kYellow);
  //  //  pipe1->SetLineColor(kGray);
  //  pipe->AddNode(pipe1, 0);
  //
  //  TGeoVolume* pipevac1 = MakeVacuum(1, nSects01, z01, rin01, rout01, vacuum,     &infoFile);
  //  pipevac1->SetLineColor(kCyan);
  //  pipe->AddNode(pipevac1, 0);

  // -----   End   --------------------------------------------------

  // ---------------   Finish   -----------------------------------------------
  top->AddNode(targetbox, 1);
  top->AddNode(pipe, 1);
  cout << endl << endl;
  gGeoMan->CloseGeometry();

  targetbox->Export(targetBoxFileName);

  TFile* targetBoxFile = new TFile(targetBoxFileName, "UPDATE");

  // rotate the PIPE around y
  TGeoRotation* pipe_rotation = new TGeoRotation();
  pipe_rotation->RotateY(pipe_angle);
  TGeoCombiTrans* targetbox_placement = new TGeoCombiTrans("pipe_rot", 0., 0., 0, pipe_rotation);
  targetbox_placement->Write();

  targetBoxFile->Close();

  cout << endl;
  cout << "Geometry " << top->GetName() << " written to " << targetBoxFileName << endl;

  pipe->Export(pipeFileName);

  TFile* pipeFile = new TFile(pipeFileName, "UPDATE");

  TGeoTranslation* tra30 = new TGeoTranslation("tra30", 0, 0, -offset20 + length20 + length30 / 2.);
  Double_t placement_x   = TMath::Cos(pipe_angle * TMath::DegToRad()) * rmax20;
  Double_t placement_z = TMath::Cos(pipe_angle * TMath::DegToRad()) * (-offset20 + length20 + length22 + length30 / 2.);
  TGeoCombiTrans* pipe_placement = new TGeoCombiTrans("pipe_rot", placement_x, 0., placement_z, pipe_rotation);
  pipe_placement->Write();

  pipeFile->Close();

  infoFile.close();

  // visualize it with ray tracing, OGL/X3D viewer
  //top->Raytrace();
  top->Draw("ogl");
  //top->Draw("x3d");
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
TGeoVolume* MakeCutPipe(Int_t ipart, TGeoMedium* medium, Double_t rmin, Double_t rmax, Double_t dz, Double_t angle1,
                        Double_t angle2, Double_t nlo1, Double_t nlo2, Double_t nlo3, Double_t nhi1, Double_t nhi2,
                        Double_t nhi3)
{

  // ---> Shape
  TString volName = Form("part%i", ipart);
  TGeoCtub* shape = new TGeoCtub(volName.Data(), rmin, rmax, dz, angle1, angle2, nlo1, nlo2, nlo3, nhi1, nhi2, nhi3);

  // ---> Volume
  TGeoVolume* pipe = new TGeoVolume(volName.Data(), shape, medium);

  return pipe;
}
// ============================================================================


// // =====  Make the beam pipe volume   =========================================
// TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
// 	                 Double_t* rout, TGeoMedium* medium, fstream* infoFile) {
//
//   // ---> Shape
//   TString volName = Form("pipe%i", iPart);
//   TGeoPcon* shape = new TGeoPcon(volName.Data(), 0., 360., nSects);
//   for (Int_t iSect = 0; iSect < nSects; iSect++) {
//     shape->DefineSection(iSect, z[iSect]/10., rin[iSect]/10., rout[iSect]/10.); // mm->cm
//     *infoFile << setw(2)  << iSect+1
//               << setw(10) << fixed << setprecision(2) << z[iSect]
//               << setw(10) << fixed << setprecision(2) << rin[iSect]
//               << setw(10) << fixed << setprecision(2) << rout[iSect]
//               << setw(10) << fixed << setprecision(2) << rout[iSect]-rin[iSect] << endl;
//   }
//
//   // ---> Volume
//   TGeoVolume* pipe = new TGeoVolume(volName.Data(), shape, medium);
//
//   return pipe;
//
// }
// // ============================================================================
//
//
//
// // =====   Make the volume for the vacuum inside the beam pipe   ==============
// TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
// 	                   Double_t* rout, TGeoMedium* medium, fstream* infoFile) {
//
//   // ---> Shape
//   TString volName = Form("pipevac%i", iPart);
//   TGeoPcon* shape = new TGeoPcon(volName.Data(), 0., 360., nSects);
//   for (Int_t iSect = 0; iSect < nSects; iSect++) {
//     shape->DefineSection(iSect, z[iSect]/10., rin[iSect]/10., rout[iSect]/10.); // mm->cm
//   }
//
//   // ---> Volume
//   TGeoVolume* pipevac = new TGeoVolume(volName.Data(), shape, medium);
//
//   return pipevac;
//
// }
// // ============================================================================
