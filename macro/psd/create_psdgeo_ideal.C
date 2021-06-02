/** @file create_psdgeo_ideal.C
  * @copyright Copyright (C) 2017-2018 National Research Nuclear University MEPhI (Moscow Engineering Physics Institute), Moscow
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese, Evgeny Kashirin [committer] **/

/** @file create_psdgeo_with_hole.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16 September 2017
 ** @version 1.0
 **
 ** This macro creates a PSD geometry in ROOT format to be used as input
 ** for the transport simulation. It allows to create module stacks of
 ** varying sizes; the numbers of rows and columns must both be uneven.
 ** The centre module as well as the edge modules are omitted.
 ** The module definition is implemented in the function ConstructModule.
 **
 ** The user can adjust the steering variables to the specific needs.
 **/

#include <iostream>


using std::cout;
using std::endl;
using std::fstream;
using std::right;
using std::setw;

// Forward declarations

TGeoVolume* ConstructIdealPsd(const char* name, Double_t sizeX, Double_t sizeY, Double_t sizeZ, Double_t modSize,
                              Double_t holesize, fstream* info);

// ============================================================================
// ======                         Main function                           =====
// ============================================================================

void create_psdgeo_ideal()
{
  // -----   Steering variables   ---------------------------------------------
  const Double_t psdX     = 9.65;     // x position of PSD in cave (front plane center)
  const Double_t psdY     = 0.;       // y position of PSD in cave (front plane center)
  const Double_t psdZ     = 800.;     // z position of PSD in cave (front plane center)
  const Double_t psdRotY  = 0.01321;  // Rotation of PSD around y axis [rad]
  const Double_t holeSize = 20.;      // diagonal size of the square shaped hole

  const Double_t bigModuleSize = 20.;  // Module size [cm]
  const Int_t nModulesX        = 8;    // Number of modules in a row (x direction)
  const Int_t nModulesY        = 6;    // Number of modules in a row (x direction)

  TString geoTag = Form("1mod_hole%icm_xshift%4.2fcm", (int) holeSize, psdX);  // Geometry tag

  // --------------------------------------------------------------------------
  cout << "Number of modules per row and column = " << nModulesX << " " << nModulesY << endl;

  // -------   Open info file   -----------------------------------------------
  TString infoFileName = "psd_";
  infoFileName         = infoFileName + geoTag + ".geo.info";
  fstream infoFile;
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "PSD geometry " << geoTag << " created with create_psdgeo_with_hole.C" << endl << endl;
  infoFile << "Ideal PSD geometry for performance studies - whole volume as a "
              "single module"
           << endl;
  infoFile << "Number of modules: " << nModulesX << " x " << nModulesY << endl;
  infoFile << "Big module size: " << bigModuleSize << " cm x " << bigModuleSize << " cm" << endl;
  infoFile << "PSD front plane center coordinates: (" << psdX << ", " << psdY << ", " << psdZ << ") cm" << endl;
  infoFile << "PSD rotation around y axis: " << psdRotY * TMath::RadToDeg() << " degrees" << endl;
  infoFile << "Diagonal size of the square shaped hole in PSD center: " << holeSize << " cm" << endl << endl;
  // --------------------------------------------------------------------------


  // -------   Load media from media file   -----------------------------------
  cout << endl << "==> Reading media..." << endl;
  FairGeoLoader* geoLoad    = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath           = gSystem->Getenv("VMCWORKDIR");
  TString medFile           = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  // --------------------------------------------------------------------------


  // ------   Create the required media from the media file    ----------------
  FairGeoMedia* geoMedia   = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // ---> Air
  FairGeoMedium* mAir = geoMedia->getMedium("air");
  if (!mAir) Fatal("Main", "FairMedium air not found");
  geoBuild->createMedium(mAir);
  TGeoMedium* air = gGeoManager->GetMedium("air");
  if (!air) Fatal("Main", "Medium air not found");
  cout << "Created medium: air" << endl;

  // ---> Vacuum
  FairGeoMedium* mVacuum = geoMedia->getMedium("vacuum");
  if (!mVacuum) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mVacuum);
  TGeoMedium* vacuum = gGeoManager->GetMedium("vacuum");
  if (!vacuum) Fatal("Main", "Medium vacuum not found");
  cout << "Created medium: vacuum" << endl;

  // ---> Medium for PSD volume
  TGeoMedium* psdMedium = vacuum;
  // --------------------------------------------------------------------------

  // --------------   Create geometry and top volume  -------------------------
  cout << endl << "==> Set top volume..." << endl;
  gGeoManager->SetName("PSDgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoManager->SetTopVolume(top);
  // --------------------------------------------------------------------------


  // ---------------   Create PSD volume   ------------------------------------
  cout << endl;
  cout << "===> Creating PSD...." << endl;

  // --- Determine size of PSD box (half-lengths)
  const Double_t psdSizeX = 0.5 * nModulesX * bigModuleSize;
  const Double_t psdSizeY = 0.5 * nModulesY * bigModuleSize;
  const Double_t psdSizeZ = 0.5 * 120.;

  TString psdName = "psd_";
  psdName += geoTag;
  TGeoVolume* psd = gGeoManager->MakeBox(psdName, psdMedium, psdSizeX, psdSizeY, psdSizeZ);
  cout << "Module array is " << nModulesX << " x " << nModulesY << endl;
  cout << "PSD size is " << 2. * psdSizeX << " cm x " << 2. * psdSizeY << " cm x " << 2. * psdSizeZ << " cm" << endl;
  infoFile << endl
           << "PSD size is " << 2. * psdSizeX << " cm x " << 2. * psdSizeY << " cm x " << 2. * psdSizeZ << " cm"
           << endl;
  infoFile << "PSD front plane center coordinates: (" << psdX << ", " << psdY << ", " << psdZ << ") cm" << endl;
  infoFile << "PSD rotation around y axis: " << psdRotY * TMath::RadToDeg() << " degrees" << endl;
  infoFile << "Diagonal size of the square shaped hole in PSD center: " << holeSize << " cm" << endl << endl;
  // --------------------------------------------------------------------------

  psd->AddNode(
    ConstructIdealPsd("ideal_psd", psdSizeX, psdSizeY, psdSizeZ, bigModuleSize / 2., holeSize / 2., &infoFile), 0);

  // --------------------------------------------------------------------------


  // -----   Place PSD in top node (cave) -------------------------------------
  Double_t psdHalfLength = psdSizeZ;
  Double_t psdVolCenterX = psdX + psdHalfLength * sin(psdRotY);
  Double_t psdVolCenterY = psdY;
  Double_t psdVolCenterZ = psdZ + psdHalfLength * cos(psdRotY);
  infoFile << "PSD volume center coordinates: (" << psdVolCenterX << ", " << psdVolCenterY << ", " << psdVolCenterZ
           << ") cm" << endl;
  TGeoRotation* psdRot = new TGeoRotation();
  psdRot->RotateY(psdRotY * TMath::RadToDeg());
  TGeoCombiTrans* psdTrans = new TGeoCombiTrans(psdVolCenterX, psdVolCenterY, psdVolCenterZ, psdRot);
  top->AddNode(psd, 0, psdTrans);
  cout << endl << "==> PSD position in cave: " << endl;
  psdTrans->Print();
  // --------------------------------------------------------------------------


  // -----   Close and check geometry   ---------------------------------------
  cout << endl << "==> Closing geometry..." << endl;
  gGeoManager->CloseGeometry();
  gGeoManager->CheckGeometryFull();
  cout << endl;
  gGeoManager->CheckOverlaps(0.0001, "s");
  // --------------------------------------------------------------------------


  // -----   Write PSD volume and placement matrix to geometry file   ---------
  cout << endl;
  TString geoFileName = "psd_";
  geoFileName         = geoFileName + geoTag + ".geo.root";
  psd->Export(geoFileName);
  TFile* geoFile = new TFile(geoFileName, "UPDATE");
  psdTrans->Write();
  cout << endl;
  cout << "==> Geometry " << psd->GetName() << " written to " << geoFileName << endl;
  cout << "==> Info written to " << infoFileName << endl;
  geoFile->Close();
  infoFile.close();
  // --------------------------------------------------------------------------


  // -----   Write entire TGeoManager to file   -------------------------------
  TString geoManFileName = "psd_";
  geoManFileName         = geoManFileName + geoTag + ".geoman.root";
  TFile* geoManFile      = new TFile(geoManFileName, "RECREATE");
  gGeoManager->Write();
  geoManFile->Close();
  cout << "==> TGeoManager " << gGeoManager->GetName() << " written to " << geoManFileName << endl << endl << endl;
  // --------------------------------------------------------------------------


  // ----   Display geometry   ------------------------------------------------
  //     gGeoManager->SetVisLevel(5);  // Scintillator level
  top->Draw("ogl");
  // --------------------------------------------------------------------------
  //     top->Raytrace();
}
//  End of main function
/** ======================================================================= **/

TGeoVolume* ConstructIdealPsd(const char* name, Double_t sizeX, Double_t sizeY, Double_t sizeZ, Double_t modSize,
                              Double_t holesize, fstream* info)
{

  TGeoMedium* medVacuum = gGeoManager->GetMedium("vacuum");  // PSD
  if (!medVacuum) Fatal("ConstructModule", "Medium vacuum not found");

  TGeoVolume* psd = gGeoManager->MakeBox(name, medVacuum, sizeX, sizeY, sizeZ);
  TGeoVolume* scint {nullptr};

  TGeoCombiTrans* rot45 = new TGeoCombiTrans("rot45", 0., 0., 0., new TGeoRotation("rot", 45., 0., 0.));
  rot45->RegisterYourself();

  TGeoBBox* moduleBox = new TGeoBBox("ideal_scint", sizeX, sizeY, sizeZ);

  TGeoBBox* Hole   = new TGeoBBox("hole", holesize, holesize, sizeZ);  //hole
  TGeoBBox* Module = new TGeoBBox("module", modSize, modSize, sizeZ);  //hole

  TGeoTranslation* trans1 = new TGeoTranslation("transl1", (sizeX - modSize), (sizeY - modSize), 0.);
  TGeoTranslation* trans2 = new TGeoTranslation("transl2", (sizeX - modSize), -(sizeY - modSize), 0.);
  TGeoTranslation* trans3 = new TGeoTranslation("transl3", -(sizeX - modSize), (sizeY - modSize), 0.);
  TGeoTranslation* trans4 = new TGeoTranslation("transl4", -(sizeX - modSize), -(sizeY - modSize), 0.);

  trans1->RegisterYourself();
  trans2->RegisterYourself();
  trans3->RegisterYourself();
  trans4->RegisterYourself();

  TString shape = "ideal_scint-(module:transl1)-(module:transl2)-(module:"
                  "transl3)-(module:transl4)";
  if (holesize > 0) shape += "-(hole:rot45)";

  TGeoCompositeShape* modShape = new TGeoCompositeShape("sens_area", shape);
  scint                        = new TGeoVolume("ideal_scint", modShape, medVacuum);

  psd->AddNode(scint, 0);

  return psd;
}
