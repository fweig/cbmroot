#include <iostream>
//#include "FairGeoMedium.h"
//#include "FairGeoBuilder.h"
//#include "FairGeoMedia.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"

using namespace std;

// Changelog
//
// 2019-11-28 - v19c - DE - move mRICH +12+7 cm in x direction (same as mTOF v19b) for the Nov 2019 run
// 2017-11-17 - v18d - DE - add aerogel as radiator to the mRICH module
// 2017-11-12 - v18c - DE - push mRICH downstream to z=355 cm for long setup
// 2017-07-19 - v18b - DE - add one level to the geometry hierarchy
// 2017-07-15 - v18b - DE - arrange 4x mRICH detectors in the setup and tilt towards target


void create_rich_v19c_mcbm() {
  gSystem->Load("libGeom");

  TString geoFileName = "rich_v19c_mcbm.geo.root";
  TString topNodeName = "rich_v19c_mcbm";

  FairGeoLoader* geoLoad    = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath           = gSystem->Getenv("VMCWORKDIR");
  TString medFile           = geoPath + "/geometry/media.geo";

  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  TGeoManager* gGeoMan = gGeoManager;

  FairGeoMedia* geoMedia   = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  //Media
  FairGeoMedium* mAluminium = geoMedia->getMedium("aluminium");
  if (mAluminium == NULL) Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAluminium);
  TGeoMedium* medAl = gGeoMan->GetMedium("aluminium");
  if (medAl == NULL) Fatal("Main", "Medium vacuum not found");

  FairGeoMedium* mKapton = geoMedia->getMedium("kapton");
  if (mKapton == NULL) Fatal("Main", "FairMedium kapton not found");
  geoBuild->createMedium(mKapton);
  TGeoMedium* medKapton = gGeoMan->GetMedium("kapton");
  if (medKapton == NULL) Fatal("Main", "Medium kapton not found");

  FairGeoMedium* mAir = geoMedia->getMedium("air");
  if (mAir == NULL) Fatal("Main", "FairMedium air not found");
  geoBuild->createMedium(mAir);
  TGeoMedium* medAir = gGeoMan->GetMedium("air");
  if (medAir == NULL) Fatal("Main", "Medium air not found");

  FairGeoMedium* mPmtGlas = geoMedia->getMedium("PMTglass");
  if (mPmtGlas == NULL) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mPmtGlas);
  TGeoMedium* medPmtGlas = gGeoMan->GetMedium("PMTglass");
  if (medPmtGlas == NULL) Fatal("Main", "Medium vacuum not found");

  FairGeoMedium* mNitrogen = geoMedia->getMedium("RICHgas_N2_dis");
  if (mNitrogen == NULL) Fatal("Main", "FairMedium RICHgas_N2_dis not found");
  geoBuild->createMedium(mNitrogen);
  TGeoMedium* medNitrogen = gGeoMan->GetMedium("RICHgas_N2_dis");
  if (medNitrogen == NULL) Fatal("Main", "Medium RICHgas_N2_dis not found");

  FairGeoMedium* mPMT = geoMedia->getMedium("CsI");
  if (mPMT == NULL) Fatal("Main", "FairMedium CsI not found");
  geoBuild->createMedium(mPMT);
  TGeoMedium* medCsI = gGeoMan->GetMedium("CsI");
  if (medCsI == NULL) Fatal("Main", "Medium CsI not found");

  FairGeoMedium* mElectronic = geoMedia->getMedium("air");
  if (mElectronic == NULL) Fatal("Main", "FairMedium air not found");
  geoBuild->createMedium(mElectronic);
  TGeoMedium* medElectronic = gGeoMan->GetMedium("air");
  if (medElectronic == NULL) Fatal("Main", "Medium air not found");

  FairGeoMedium* mAerogel = geoMedia->getMedium("aerogel");
  if (mAerogel == NULL) Fatal("Main", "FairMedium aerogel not found");
  geoBuild->createMedium(mAerogel);
  TGeoMedium* medAerogel = gGeoMan->GetMedium("aerogel");
  if (medAerogel == NULL) Fatal("Main", "Medium aerogel  not found");


  //Dimensions of the RICH prototype [cm]

  const Double_t ItemToKapton = 1.85;

  // Box
  const Double_t boxThickness      = 0.3;
  const Double_t boxBackThickness  = 1.;
  const Double_t boxFrontThickness = 0.3;
  const Double_t boxXLen           = 38.2;
  const Double_t boxYLen           = 59.7;
  const Double_t boxZLen           = 25. + boxBackThickness - ItemToKapton;
  const Bool_t isRotate90DegZ      = false;

  // PMT
  // PMT specs https://www.hamamatsu.com/resources/pdf/etd/H12700_TPMH1348E.pdf
  const Double_t pmtNofPixels      = 8;
  const Double_t pmtSize           = 5.2;
  const Double_t pmtSizeHalf       = pmtSize / 2.;
  const Double_t pmtSensitiveSize  = 4.85;
  const Double_t pmtPixelSize      = 0.6;  //pmtSensitiveSize / pmtNofPixels;
  const Double_t pmtPixelSizeHalf  = pmtPixelSize / 2.;
  const Double_t pmtOuterPixelSize = 0.625;  //pmtSensitiveSize / pmtNofPixels;
  const Double_t pmtOuterPixelSizeHalf = pmtOuterPixelSize / 2.;
  const Double_t pmtGap                = 0.1;
  const Double_t pmtGapHalf            = pmtGap / 2.;
  const Double_t pmtMatrixGap          = 0.1;
  const Double_t pmtPixelThickness     = 0.1;
  const Double_t pmtWindowThickness    = 0.15;
  Double_t pmtWindowPixelDistance      = 0.0;
  if (pmtWindowThickness > 0.0) pmtWindowPixelDistance = 0.0;
  const Double_t pmtThickness =
    pmtPixelThickness + pmtWindowThickness + pmtWindowPixelDistance;
  const Double_t pmtZPadding = 3.871;
  const Double_t pmtXShift   = 2.5;

  // Electronics
  const Double_t elecLength = 10.;
  const Double_t elecWidth  = 3. * pmtSize;
  const Double_t elecHeight = 2. * pmtSize;

  // Aerogel Box
  const Double_t aerogelXLen        = 20.;
  const Double_t aerogelYLen        = 20.;
  const Double_t aerogelZLen        = 3.;
  const Double_t aerogelPmtDistance = 10.;
  const Double_t aerogelGap         = 0.1;

  // Imaginary sensitive plane
  Bool_t isIncludeSensPlane           = false;
  const Double_t sensPlaneSize        = 200.;
  const Double_t sensPlaneBoxDistance = 1.;
  //Global positioning
  const Double_t boxFrontToTarget = 327. + ItemToKapton + boxZLen / 2;

  // Create the top volume
  TGeoVolume* top = new TGeoVolumeAssembly("top");
  gGeoMan->SetTopVolume(top);

  // Rich assembly
  TGeoVolume* rich = new TGeoVolumeAssembly(topNodeName);
  top->AddNode(rich, 1);

  // Al box
  TGeoRotation* rotBox  = new TGeoRotation("rotBox", 0., 0., 0.);
  TGeoCombiTrans* trBox = NULL;
  if (isRotate90DegZ) {
    rotBox->RotateZ(90.);
    rotBox->RotateY(0.955);  //Tilting of Rich
    trBox = new TGeoCombiTrans(
      0, -pmtXShift - (pmtSize + pmtGap + 1), boxFrontToTarget, rotBox);
  } else {
    rotBox->RotateX(0.955);  //Tilting of Rich
                             // Nov 2019 run
    trBox = new TGeoCombiTrans(-pmtXShift - (pmtSize + pmtGap + 1) + 12. + 7.,
                               0,
                               boxFrontToTarget,
                               rotBox);
    //	    trBox = new TGeoCombiTrans(-pmtXShift-(pmtSize + pmtGap + 1), 0, boxFrontToTarget, rotBox);
  }
  TGeoVolume* boxVol =
    gGeoMan->MakeBox("box", medAl, boxXLen / 2., boxYLen / 2., boxZLen / 2.);
  rich->AddNode(boxVol, 1, trBox);

  // Gas
  TGeoVolume* gasVol =
    gGeoMan->MakeBox("Gas",
                     medNitrogen,
                     boxXLen / 2. - boxThickness,
                     boxYLen / 2. - boxThickness,
                     ((boxZLen - boxBackThickness - boxFrontThickness) / 2.));
  TGeoTranslation* trGas =
    new TGeoTranslation(0., 0., (-(boxBackThickness - boxFrontThickness) / 2.));
  boxVol->AddNode(gasVol, 1, trGas);

  // Front plane from kapton
  TGeoVolume* kaptonVol = gGeoMan->MakeBox("kapton",
                                           medKapton,
                                           boxXLen / 2. - boxThickness,
                                           boxYLen / 2. - boxThickness,
                                           boxFrontThickness / 2.);
  TGeoTranslation* trKapton =
    new TGeoTranslation(0., 0., -(boxZLen - boxFrontThickness) / 2.);
  boxVol->AddNode(kaptonVol, 1, trKapton);


  // Aerogel
  TGeoVolume* aerogelVol = gGeoMan->MakeBox("aerogel",
                                            medAerogel,
                                            aerogelXLen / 2.,
                                            aerogelYLen / 2.,
                                            aerogelZLen / 2.);
  Double_t aerogelZ      = boxZLen / 2. - boxBackThickness - pmtZPadding
                      - aerogelPmtDistance - aerogelZLen / 2.;
  TGeoTranslation* trAerogel1 =
    new TGeoTranslation(pmtXShift, (aerogelYLen + aerogelGap) / 2., aerogelZ);
  TGeoTranslation* trAerogel2 =
    new TGeoTranslation(pmtXShift, -(aerogelYLen + aerogelGap) / 2., aerogelZ);
  gasVol->AddNode(aerogelVol, 1, trAerogel1);
  gasVol->AddNode(aerogelVol, 2, trAerogel2);

  // PMT
  TGeoTranslation* trPmt1 = new TGeoTranslation(
    pmtSizeHalf + pmtGapHalf, -pmtSize - pmtGap, 0.);  // Bottom Right
  TGeoTranslation* trPmt2 =
    new TGeoTranslation(pmtSizeHalf + pmtGapHalf, 0., 0.);  // Middle Right
  TGeoTranslation* trPmt3 = new TGeoTranslation(
    pmtSizeHalf + pmtGapHalf, pmtSize + pmtGap, 0.);  // Top Right
  TGeoTranslation* trPmt4 = new TGeoTranslation(
    -pmtSizeHalf - pmtGapHalf, -pmtSize - pmtGap, 0.);  // Bottom Left
  TGeoTranslation* trPmt5 =
    new TGeoTranslation(-pmtSizeHalf - pmtGapHalf, 0., 0.);  // Middle Left
  TGeoTranslation* trPmt6 = new TGeoTranslation(
    -pmtSizeHalf - pmtGapHalf, pmtSize + pmtGap, 0.);  // Top Left

  TGeoVolume* pmtContVol  = new TGeoVolumeAssembly("pmt_cont_vol");
  TGeoVolume* pmtVol      = new TGeoVolumeAssembly("pmt_vol_0");
  TGeoVolume* pmtVol_180  = new TGeoVolumeAssembly("pmt_vol_1");
  TGeoVolume* pmtPixelVol = gGeoMan->MakeBox("pmt_pixel",
                                             medCsI,
                                             pmtPixelSize / 2.,
                                             pmtPixelSize / 2.,
                                             pmtPixelThickness / 2.);
  TGeoVolume* pmtPixelVol_outer_outer =
    gGeoMan->MakeBox("pmt_pixel",
                     medCsI,
                     pmtOuterPixelSize / 2.,
                     pmtOuterPixelSize / 2.,
                     pmtPixelThickness / 2.);
  TGeoVolume* pmtPixelVol_inner_outer =
    gGeoMan->MakeBox("pmt_pixel",
                     medCsI,
                     pmtPixelSize / 2.,
                     pmtOuterPixelSize / 2.,
                     pmtPixelThickness / 2.);
  TGeoVolume* pmtPixelVol_outer_inner =
    gGeoMan->MakeBox("pmt_pixel",
                     medCsI,
                     pmtOuterPixelSize / 2.,
                     pmtPixelSize / 2.,
                     pmtPixelThickness / 2.);
  TGeoVolume* pmtWindow = gGeoMan->MakeBox("pmt_Window",
                                           medPmtGlas,
                                           pmtSizeHalf,
                                           pmtSizeHalf,
                                           pmtWindowThickness / 2.);

  pmtContVol->AddNode(
    pmtVol_180, 1, trPmt1);                // To Power Module    // Bottom Right
  pmtContVol->AddNode(pmtVol, 2, trPmt2);  // Middle             // Middle Right
  pmtContVol->AddNode(pmtVol, 3, trPmt3);  // To Combiner        // Top Right
  pmtContVol->AddNode(
    pmtVol_180, 4, trPmt4);                // To Power Module    // Bottom Left
  pmtContVol->AddNode(pmtVol, 5, trPmt5);  // Middle             // Middle Left
  pmtContVol->AddNode(pmtVol, 6, trPmt6);  // To Combiner        // Top Left

  Int_t halfPmtNofPixels = pmtNofPixels / 2;
  for (Int_t i = 0; i < pmtNofPixels; i++) {
    for (Int_t j = 0; j < pmtNofPixels; j++) {
      //check outer Pixels

      Double_t x   = 0.;
      Double_t y   = 0.;
      bool x_outer = false;
      bool y_outer = false;

      if (i == 0) {
        x = -(halfPmtNofPixels - 1) * pmtPixelSize - pmtOuterPixelSizeHalf;
        x_outer = true;
      } else if (i == (pmtNofPixels - 1)) {
        x       = (halfPmtNofPixels - 1) * pmtPixelSize + pmtOuterPixelSizeHalf;
        x_outer = true;
      } else if (i < halfPmtNofPixels) {
        x = -((halfPmtNofPixels - 1) - i) * pmtPixelSize - pmtPixelSizeHalf;
        x_outer = false;
      } else {
        x       = (i - halfPmtNofPixels) * pmtPixelSize + pmtPixelSizeHalf;
        x_outer = false;
      }


      if (j == 0) {
        y = -(halfPmtNofPixels - 1) * pmtPixelSize - pmtOuterPixelSizeHalf;
        y_outer = true;
      } else if (j == (pmtNofPixels - 1)) {
        y       = (halfPmtNofPixels - 1) * pmtPixelSize + pmtOuterPixelSizeHalf;
        y_outer = true;
      } else if (j < halfPmtNofPixels) {
        y = -((halfPmtNofPixels - 1) - j) * pmtPixelSize - pmtPixelSizeHalf;
        y_outer = false;
      } else {
        y       = (j - halfPmtNofPixels) * pmtPixelSize + pmtPixelSizeHalf;
        y_outer = false;
      }
      //Double_t x = (i - (halfPmtNofPixels - 1)) * pmtPixelSize - pmtPixelSizeHalf;

      //Double_t y = (j - (halfPmtNofPixels - 1)) * pmtPixelSize - pmtPixelSizeHalf;
      TGeoTranslation* trij =
        new TGeoTranslation(x, y, -pmtPixelThickness / 2. + pmtThickness / 2.);

      //normal rotated PMT (HV connector at lower side)
      uint uid =
        18 - ((j % 4) / 2) * 16 - (j % 2) + 2 * i + (1 - (j / 4)) * 100;

      //180?? rotated PMT
      uint uid_180 =
        15 + ((j % 4) / 2) * 16 + (j % 2) - 2 * i + (1 - (j / 4)) * 100;
      if (x_outer == true && y_outer == true) {
        pmtVol->AddNode(pmtPixelVol_outer_outer, uid, trij);
        pmtVol_180->AddNode(pmtPixelVol_outer_outer, uid_180, trij);
      } else if (x_outer == true && y_outer == false) {
        pmtVol->AddNode(pmtPixelVol_outer_inner, uid, trij);
        pmtVol_180->AddNode(pmtPixelVol_outer_inner, uid_180, trij);
      } else if (x_outer == false && y_outer == true) {
        pmtVol->AddNode(pmtPixelVol_inner_outer, uid, trij);
        pmtVol_180->AddNode(pmtPixelVol_inner_outer, uid_180, trij);
      } else {
        pmtVol->AddNode(pmtPixelVol, uid, trij);
        pmtVol_180->AddNode(pmtPixelVol, uid_180, trij);
      }
    }
  }

  TGeoTranslation* trWndw =
    new TGeoTranslation(0., 0., +pmtWindowThickness / 2. - pmtThickness / 2.);
  if (pmtWindowThickness > 0.0) {
    pmtVol->AddNode(pmtWindow, 1, trWndw);
    pmtVol_180->AddNode(pmtWindow, 1, trWndw);
  }

  //
  Double_t pmtContXLen = 2 * (pmtSize + pmtGap);
  Double_t pmtContYLen = 3 * (pmtSize + pmtGap);
  TGeoTranslation* trPmtCont1 =
    new TGeoTranslation(-pmtContXLen / 2., pmtContYLen, 0.);  //Top Left
  TGeoTranslation* trPmtCont2 =
    new TGeoTranslation(-pmtContXLen / 2., 0., 0.);  //Middle Left
  TGeoTranslation* trPmtCont3 =
    new TGeoTranslation(-pmtContXLen / 2., -pmtContYLen, 0.);  //Bottom Left
  TGeoTranslation* trPmtCont4 =
    new TGeoTranslation(pmtContXLen / 2., pmtContYLen, 0.);  //
  TGeoTranslation* trPmtCont5 =
    new TGeoTranslation(pmtContXLen / 2., 0., 0.);  //
  TGeoTranslation* trPmtCont6 =
    new TGeoTranslation(pmtContXLen / 2., -pmtContYLen, 0.);  //

  TGeoVolume* pmtPlaneVol = new TGeoVolumeAssembly("pmt_plane");
  pmtPlaneVol->AddNode(
    pmtContVol,
    00,
    trPmtCont1);  // Id gives the Address 0x7**0 of the top left DiRICH in a BP  (X|Y)
  pmtPlaneVol->AddNode(pmtContVol, 03, trPmtCont2);
  pmtPlaneVol->AddNode(pmtContVol, 06, trPmtCont3);
  pmtPlaneVol->AddNode(pmtContVol, 20, trPmtCont4);
  pmtPlaneVol->AddNode(pmtContVol, 23, trPmtCont5);
  pmtPlaneVol->AddNode(pmtContVol, 26, trPmtCont6);

  TGeoTranslation* trPmtPlane = new TGeoTranslation(
    pmtXShift,
    0.,
    boxZLen / 2. - boxBackThickness - pmtZPadding + (pmtThickness / 2.));
  gasVol->AddNode(pmtPlaneVol, 1, trPmtPlane);


  gGeoMan->CheckOverlaps();
  gGeoMan->PrintOverlaps();

  //Draw
  pmtPixelVol->SetLineColor(kYellow + 4);
  aerogelVol->SetLineColor(kCyan);
  aerogelVol->SetTransparency(70);
  gasVol->SetLineColor(kGreen);
  gasVol->SetTransparency(60);
  kaptonVol->SetLineColor(kBlue);
  kaptonVol->SetTransparency(70);


  //gGeoMan->SetTopVisible();
  //boxVol->SetVisibility(true);

  //	boxVol->Draw("ogl");
  //top->Draw("ogl");
  gGeoMan->SetVisLevel(7);  //7

  //	rich->Export(geoFileName);   // an alternative way of writing the trd volume
  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  cout << endl << "Geometry written to " << geoFileName << endl;
  geoFile->Close();
}
