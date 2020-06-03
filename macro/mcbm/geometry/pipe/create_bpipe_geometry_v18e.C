/******************************************************************************
 ** Creation of beam pipe geometry in ROOT format (TGeo).
 **
 ** @file create_bpipe_geometry_v16c_1e.C
 ** @author David Emschermann <d.emschermann@gsi.de>
 ** @author Andrey Chernogorov <a.chernogorov@gsi.de>
 ** @date 19.07.2016
 **
 ** mCBM
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




#include <iomanip>
#include <iostream>
#include "TGeoManager.h"

#include "TGeoPcon.h"

using namespace std;


// -------------   Steering variables       -----------------------------------
// ---> Beam pipe material name
TString pipeMediumName = "iron";
//TString pipeMediumName = "carbon"; // "beryllium" "aluminium"
// ----------------------------------------------------------------------------


// -------------   Other global variables   -----------------------------------
// ---> Macro name to info file
TString macroname = "create_bpipe_geometry_v18e.C";
// ---> Geometry file name (output)
TString rootFileName = "pipe_v18e_mcbm.geo.root";
// ---> Geometry name
TString pipeName = "pipe_v18e";
// ----------------------------------------------------------------------------

TGeoVolume* MakePipe(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
                     Double_t* rout, TGeoMedium* medium, fstream* infoFile);

TGeoVolume* MakeVacuum(Int_t iPart, Int_t nSects, Double_t* z, Double_t* rin,
                       Double_t* rout, TGeoMedium* medium, fstream* infoFile);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_bpipe_geometry_v18e()
{
  // -----   Define beam pipe sections   --------------------------------------
  /** For v18e:   **/
  TString pipe1name = "pipe1 - straight miniCBM beampipe";
  const Int_t nSects1 = 2;
  Double_t z1[nSects1]      = {    0., 3000. }; // mm
  Double_t rin1[nSects1]    = {   50.,   50. };
  Double_t rout1[nSects1]   = {   48.,   48. };

  const Int_t nSects01 = 2;
  Double_t z01[nSects01]    = {    0., 3000. }; // mm
  Double_t rin01[nSects01]  = {    0.,    0. };
  Double_t rout01[nSects01] = {   48.,   48. };

  //  Double_t pipe_angle = 25.;  // rotation angle around y-axis
  Double_t pipe_angle = 20.;  // rotation angle around y-axis

  // tan (acos(-1)/180 * 2.5) *  30 cm = 1.310 cm

  //  const Double_t pipewallthickness = 2.0; // mm
  
  // --------------------------------------------------------------------------


  // -------   Open info file   -----------------------------------------------
  TString infoFileName = rootFileName;
  infoFileName.ReplaceAll("root", "info");
  fstream infoFile;
  fstream infoFileEmpty;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "SIS-100. Beam pipe geometry created with " + macroname << endl << endl;
  infoFile << "  pipe v18e - is a pipe for the STS up to the interface to RICH at z = 1700 mm" << endl << endl;
  infoFile << "                 with a (blue) flange at the downstream end of the STS box" << endl << endl;
  infoFile << "The beam pipe is composed of carbon with a fixed wall thickness of 0.5 or 1.0 mm." << endl;
  infoFile << "It is placed directly into the cave as mother volume. The beam pipe consists of" << endl;
  infoFile << "few sections up to the RICH section (1700-3700mm), which is part of the RICH geometry." << endl;
  infoFile << "Each section has a PCON shape (including windows)." << endl;
  infoFile << "The STS section is composed of cylinder D(z=220-410mm)=34mm and cone (z=410-1183mm)." << endl;
  infoFile << "All sections of the beam pipe with conical shape have half opening angle 2.5deg." << endl << endl;

  infoFile << "Material:  " << pipeMediumName << endl;
  //  infoFile << "Wall Thickness: " << pipewallthickness << " mm" << endl << endl;
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
  
  TGeoVolume* pipevac1 = MakeVacuum(1, nSects01, z01, rin01, rout01, vacuum,     &infoFile); 
  pipevac1->SetLineColor(kCyan);
  pipe->AddNode(pipevac1, 0);

  // -----   End   --------------------------------------------------

  // ---------------   Finish   -----------------------------------------------
  top->AddNode(pipe, 1);
  cout << endl << endl;
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  pipe->Export(rootFileName);
  
  //  TFile* rootFile = new TFile(rootFileName, "RECREATE");
  //  top->Write();

  TFile* rootFile = new TFile(rootFileName, "UPDATE");

  // rotate the PIPE around y
  TGeoRotation* pipe_rotation = new TGeoRotation();
  pipe_rotation->RotateY( pipe_angle );
  //  TGeoCombiTrans* pipe_placement = new TGeoCombiTrans( sin( pipe_angle/180.*acos(-1) ) * z1[1]/2., 0., 0., pipe_rotation);
  TGeoCombiTrans* pipe_placement = new TGeoCombiTrans("pipe_rot", 0., 0., 0, pipe_rotation);
  pipe_placement->Write();

  rootFile->Close();

  cout << endl;
  cout << "Geometry " << top->GetName() << " written to " 
       << rootFileName << endl;

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
