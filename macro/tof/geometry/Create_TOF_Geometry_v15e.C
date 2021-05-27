// -----------------------------------------------------------------------------
// ----- Create_TOF_Geometry_v15e.C                                        -----
// -----                                                                   -----
// ----- created by C. Simon on 2018-04-27                                 -----
// -----                                                                   -----
// ----- geometrical description of the upper counter branch               -----
// -----   4-0-0, 9-0-0, 9-0-1, 9-1-0, 9-1-1, 9-2-0, 9-2-1, 3-0-0          -----
// ----- including two strip-diamond start counters                        -----
// -----   5-1-0, 5-2-0                                                    -----
// ----- of the setup used at CERN/SPS in November 2015 for simulations    -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "FairGeoBuilder.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoMedia.h"
#include "FairGeoMedium.h"

#include "TFile.h"
#include "TGeoCompositeShape.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGeoPgon.h"
#include "TGeoVolume.h"
#include "TList.h"
#include "TMath.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"

#include <iostream>

// geometry version and output file name
const TString gtGeoVersion = "tof_v14a_n";
const TString gtFileName   = "tof_v14a.root";  // for compatibility reasons

// names of materials used in this geometry (defined in media.geo)
const TString gtKeepingVolumeMedium = "air";
const TString gtBoxVolumeMedium     = "aluminium";
const TString gtNoActivGasMedium    = "RPCgas_noact";
const TString gtActivGasMedium      = "RPCgas";
const TString gtGlasMedium          = "RPCglass";
const TString gtElectronicsMedium   = "carbon";


// list of counter types used in this geometry
//  0 strip DD
//  1 HD P5      (low-resistive glass)
//  2 USTC SS/DS (thin float glass)
//  3 THU SS     (low-resistive glass)
//  4 THU DS     (low-resistive glass)
//  5 HD P2      (low-resistive glass)
const Int_t giNCounterTypes = 6;

const Double_t gdGlass_X[giNCounterTypes] = {2., 15.04, 32., 32., 32., 32.};
const Double_t gdGlass_Y[giNCounterTypes] = {2., 4., 26.9, 26.9, 26.9, 26.9};
const Double_t gdGlass_Z[giNCounterTypes] = {0.1, 0.1, 0.028, 0.07, 0.07, 0.07};

const Double_t gdGasGap_X[giNCounterTypes] = {2., 15.04, 32., 32., 32., 32.};
const Double_t gdGasGap_Y[giNCounterTypes] = {2., 4., 26.9, 26.9, 26.9, 26.9};
const Double_t gdGasGap_Z[giNCounterTypes] = {0.03, 0.022, 0.014, 0.022, 0.025, 0.022};

const Int_t giNGaps[giNCounterTypes]          = {1, 6, 12, 8, 8, 8};
const Int_t giNReadoutStrips[giNCounterTypes] = {16, 16, 32, 32, 32, 32};

const Double_t gdElectronics_X[giNCounterTypes] = {2., 15.5, 34., 34., 34., 34.};
const Double_t gdElectronics_Y[giNCounterTypes] = {0.1, 5., 5., 5., 5., 5.};
const Double_t gdElectronics_Z[giNCounterTypes] = {0.1, 0.3, 0.3, 0.3, 0.3, 0.3};

const Double_t gdBoundaryDistance = 0.1;


// list of module box types used in this geometry
//  0 strip DD
//  1 HD P5
//  2 STAR Box 1 (USTC SS, USTC SS)
//  3 STAR Box 2 (USTC DS, THU SS)
//  4 STAR Box 3 (THU DS, THU DS)
//  5 HD P2
const Int_t giNModuleTypes = 6;

const Double_t gdModule_X[giNModuleTypes] = {5., 54.5, 120., 120., 120., 72.5};
const Double_t gdModule_Y[giNModuleTypes] = {5., 29.5, 74., 74., 74., 51.5};
const Double_t gdModule_Z[giNModuleTypes] = {1., 6.5, 10., 10., 10., 10.5};

const Double_t gdModuleWallWidth_Xplus[giNModuleTypes]  = {0.1, 0.5, 0.5, 0.5, 0.5, 0.5};
const Double_t gdModuleWallWidth_Xminus[giNModuleTypes] = {0.1, 0.5, 0.1, 0.1, 0.1, 0.5};
const Double_t gdModuleWallWidth_Y[giNModuleTypes]      = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
const Double_t gdModuleWallWidth_Z[giNModuleTypes]      = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1};


// placement of counters inside the module boxes
const Int_t giMaxNCountersInModule                                     = 2;
const Int_t giNCountersInModule[giNModuleTypes]                        = {1, 1, 2, 2, 2, 1};
const Int_t giCountersInModule[giNModuleTypes][giMaxNCountersInModule] = {{0, -1}, {1, -1}, {2, 2},
                                                                          {2, 3},  {4, 4},  {5, 0}};

const Double_t gdCounterXPosition[giNModuleTypes][giMaxNCountersInModule] = {{0., 0.}, {0., 0.}, {0., 0.},
                                                                             {0., 0.}, {0., 0.}, {-2., 0.}};
const Double_t gdCounterYPosition[giNModuleTypes][giMaxNCountersInModule] = {{0., 0.}, {0., 0.}, {0., 0.},
                                                                             {0., 0.}, {0., 0.}, {0., 0.}};
const Double_t gdCounterZPosition[giNModuleTypes][giMaxNCountersInModule] = {{0., 0.},  {0., 0.},  {-2., 2.},
                                                                             {-2., 2.}, {-2., 2.}, {2.5, 0.}};

const Double_t gdCounterXRotation[giNModuleTypes][giMaxNCountersInModule] = {{0., 0.}, {0., 0.}, {0., 0.},
                                                                             {0., 0.}, {0., 0.}, {0., 0.}};
const Double_t gdCounterYRotation[giNModuleTypes][giMaxNCountersInModule] = {{0., 0.}, {0., 0.}, {0., 0.},
                                                                             {0., 0.}, {0., 0.}, {0., 0.}};
const Double_t gdCounterZRotation[giNModuleTypes][giMaxNCountersInModule] = {{0., 0.}, {0., 0.}, {0., 0.},
                                                                             {0., 0.}, {0., 0.}, {90., 0.}};


// placement of modules in the setup
const Int_t giMaxNModulesInSetup              = 2;
const Int_t giNModulesInSetup[giNModuleTypes] = {2, 1, 1, 1, 1, 1};

const Double_t gdModuleXPosition[giNModuleTypes][giMaxNModulesInSetup] = {{0., 0.},  {2.7, 0.}, {2.2, 0.},
                                                                          {2.2, 0.}, {2.2, 0.}, {6.5, 0.}};
const Double_t gdModuleYPosition[giNModuleTypes][giMaxNModulesInSetup] = {{0., 0.},  {39.7, 0.}, {41.5, 0.},
                                                                          {44., 0.}, {46.5, 0.}, {53., 0.}};
const Double_t gdModuleZPosition[giNModuleTypes][giMaxNModulesInSetup] = {{-3., -5.},  {359., 0.},  {376., 0.},
                                                                          {399.8, 0.}, {423.6, 0.}, {440.5, 0.}};

const Double_t gdModuleXRotation[giNModuleTypes][giMaxNModulesInSetup] = {{0., 0.},   {-6.3, 0.}, {-6.3, 0.},
                                                                          {-6.3, 0.}, {-6.3, 0.}, {-6.3, 0.}};
const Double_t gdModuleYRotation[giNModuleTypes][giMaxNModulesInSetup] = {{0., 0.}, {0., 0.}, {0., 0.},
                                                                          {0., 0.}, {0., 0.}, {0., 0.}};
const Double_t gdModuleZRotation[giNModuleTypes][giMaxNModulesInSetup] = {{0., 90.}, {0., 0.}, {0., 0.},
                                                                          {0., 0.},  {0., 0.}, {0., 0.}};

const Int_t giModuleTypeIdentifier[giNModuleTypes][giMaxNModulesInSetup]  = {{5, 5},  {4, -1}, {9, -1},
                                                                            {9, -1}, {9, -1}, {3, -1}};
const Int_t giModuleIndexIdentifier[giNModuleTypes][giMaxNModulesInSetup] = {{1, 2},  {0, -1}, {0, -1},
                                                                             {1, -1}, {2, -1}, {0, -1}};


TGeoVolume* gModules[giNModuleTypes];
TGeoVolume* gCounters[giNCounterTypes];
TGeoVolume* gtTofVolume;

void create_materials_from_media_file();
TGeoVolume* create_counter(Int_t);
TGeoVolume* create_module(Int_t);
void place_modules(Int_t);


void Create_TOF_Geometry_v15e()
{
  // load material definitions from media.geo
  create_materials_from_media_file();

  gROOT->FindObject("FAIRGeom");
  gGeoManager->SetVisLevel(5);
  gGeoManager->SetVisOption(0);

  TGeoVolume* tTopVolume = new TGeoVolumeAssembly("TOP");
  gGeoManager->SetTopVolume(tTopVolume);

  gtTofVolume = new TGeoVolumeAssembly(gtGeoVersion);
  tTopVolume->AddNode(gtTofVolume, 1);

  for (Int_t iCounterType = 0; iCounterType < giNCounterTypes; iCounterType++) {
    cout << "create counter " << iCounterType << endl;
    gCounters[iCounterType] = create_counter(iCounterType);
  }

  for (Int_t iModuleType = 0; iModuleType < giNModuleTypes; iModuleType++) {
    cout << "create module " << iModuleType << endl;
    gModules[iModuleType] = create_module(iModuleType);
    gModules[iModuleType]->SetVisContainers(1);

    place_modules(iModuleType);
  }


  gGeoManager->CloseGeometry();
  gGeoManager->CheckOverlaps(0.001);
  gGeoManager->PrintOverlaps();
  gGeoManager->Test();

  TFile* tOutputFile = new TFile(gtFileName, "RECREATE");
  tTopVolume->Write();
  //  gGeoManager->Write();
  tOutputFile->Close();
  tTopVolume->SetVisContainers(1);
  gGeoManager->SetVisLevel(5);

  if (!gROOT->IsBatch()) tTopVolume->Draw("ogl");
}


void create_materials_from_media_file()
{
  // use the FairRoot geometry interface to load the required media
  FairGeoLoader* tGeoLoader       = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* tGeoInterface = tGeoLoader->getGeoInterface();
  TString tGeoPath                = gSystem->Getenv("VMCWORKDIR");
  TString tGeoFile                = tGeoPath + "/geometry/media.geo";
  tGeoInterface->setMediaFile(tGeoFile);
  tGeoInterface->readMedia();

  // read the required media and create them in the TGeoManager
  FairGeoMedia* tGeoMedia     = tGeoInterface->getMedia();
  FairGeoBuilder* tGeoBuilder = tGeoLoader->getGeoBuilder();

  FairGeoMedium* air          = tGeoMedia->getMedium("air");
  FairGeoMedium* aluminium    = tGeoMedia->getMedium("aluminium");
  FairGeoMedium* RPCgas       = tGeoMedia->getMedium("RPCgas");
  FairGeoMedium* RPCgas_noact = tGeoMedia->getMedium("RPCgas_noact");
  FairGeoMedium* RPCglass     = tGeoMedia->getMedium("RPCglass");
  FairGeoMedium* carbon       = tGeoMedia->getMedium("carbon");

  tGeoBuilder->createMedium(air);
  tGeoBuilder->createMedium(aluminium);
  tGeoBuilder->createMedium(RPCgas);
  tGeoBuilder->createMedium(RPCgas_noact);
  tGeoBuilder->createMedium(RPCglass);
  tGeoBuilder->createMedium(carbon);
}


TGeoVolume* create_counter(Int_t iCounterType)
{
  Double_t dElectronicsPosition_Y = gdGlass_Y[iCounterType] / 2. + gdElectronics_Y[iCounterType] / 2.;

  // counter volume dimensions
  Double_t dCounter_X =
    TMath::Max(TMath::Max(gdGlass_X[iCounterType], gdGasGap_X[iCounterType]), gdElectronics_X[iCounterType])
    + 2. * gdBoundaryDistance;
  Double_t dCounter_Y = TMath::Max(gdGlass_Y[iCounterType], gdGasGap_Y[iCounterType])
                        + 2. * gdElectronics_Y[iCounterType] + 2. * gdBoundaryDistance;
  Double_t dCounter_Z = giNGaps[iCounterType] * (gdGlass_Z[iCounterType] + gdGasGap_Z[iCounterType])
                        + gdGlass_Z[iCounterType] + 2. * gdBoundaryDistance;

  Double_t dGapDistance         = gdGlass_Z[iCounterType] + gdGasGap_Z[iCounterType];
  Double_t dGlassZStartPosition = -dCounter_Z / 2. + gdBoundaryDistance + gdGlass_Z[iCounterType] / 2.;
  Double_t dGapZStartPosition   = dGlassZStartPosition + dGapDistance / 2.;

  // needed materials
  TGeoMedium* tGlassPlateVolMed  = gGeoManager->GetMedium(gtGlasMedium);
  TGeoMedium* tNoActiveGasVolMed = gGeoManager->GetMedium(gtNoActivGasMedium);
  TGeoMedium* tActiveGasVolMed   = gGeoManager->GetMedium(gtActivGasMedium);
  TGeoMedium* tElectronicsVolMed = gGeoManager->GetMedium(gtElectronicsMedium);

  // create counter volume
  TGeoBBox* tCounterShape    = new TGeoBBox("", dCounter_X / 2., dCounter_Y / 2., dCounter_Z / 2.);
  TGeoVolume* tCounterVolume = new TGeoVolume("counter", tCounterShape, tNoActiveGasVolMed);

  // create glass plate volume
  TGeoBBox* tGlassPlateShape =
    new TGeoBBox("", gdGlass_X[iCounterType] / 2., gdGlass_Y[iCounterType] / 2., gdGlass_Z[iCounterType] / 2.);
  TGeoVolume* tGlassPlateVolume = new TGeoVolume("tof_glass", tGlassPlateShape, tGlassPlateVolMed);

  // create gas gap volume
  TGeoBBox* tGasGapShape =
    new TGeoBBox("", gdGasGap_X[iCounterType] / 2., gdGasGap_Y[iCounterType] / 2., gdGasGap_Z[iCounterType] / 2.);
  TGeoVolume* tGasGapVolume = new TGeoVolume("Gap", tGasGapShape, tActiveGasVolMed);

  // divide the gas gap volume into readout strips ("cells")
  tGasGapVolume->Divide("Cell", 1, giNReadoutStrips[iCounterType], -gdGasGap_X[iCounterType] / 2., 0);

  // position all glass plates and gas gaps in the counter except for the final glass plate
  for (Int_t iGap = 0; iGap < giNGaps[iCounterType]; iGap++) {
    Double_t dZPositionGlass = dGlassZStartPosition + iGap * dGapDistance;
    Double_t dZPositionGap   = dGapZStartPosition + iGap * dGapDistance;

    tCounterVolume->AddNode(tGlassPlateVolume, iGap, new TGeoTranslation("", 0., 0., dZPositionGlass));
    tCounterVolume->AddNode(tGasGapVolume, iGap, new TGeoTranslation("", 0., 0., dZPositionGap));
  }

  // position the final glass plate in the counter
  Double_t dZPositionGlass = dGlassZStartPosition + giNGaps[iCounterType] * dGapDistance;
  tCounterVolume->AddNode(tGlassPlateVolume, giNGaps[iCounterType], new TGeoTranslation("", 0., 0., dZPositionGlass));

  // create electronics volume
  TGeoBBox* tElectronicsShape = new TGeoBBox("", gdElectronics_X[iCounterType] / 2., gdElectronics_Y[iCounterType] / 2.,
                                             gdElectronics_Z[iCounterType] / 2.);
  TGeoVolume* tElectronicsVolume = new TGeoVolume("pcb", tElectronicsShape, tElectronicsVolMed);

  // position both electronics PCBs in the counter
  tCounterVolume->AddNode(tElectronicsVolume, 0, new TGeoTranslation("", 0., -dElectronicsPosition_Y, 0.));
  tCounterVolume->AddNode(tElectronicsVolume, 1, new TGeoTranslation("", 0., dElectronicsPosition_Y, 0.));

  // drawing style
  tCounterVolume->SetLineColor(kCyan);
  tCounterVolume->SetTransparency(70);
  tGlassPlateVolume->SetLineColor(kYellow);
  tGlassPlateVolume->SetTransparency(0);
  tGasGapVolume->SetLineColor(kRed);
  tGasGapVolume->SetTransparency(50);
  tElectronicsVolume->SetLineColor(kGreen);
  tElectronicsVolume->SetTransparency(10);

  return tCounterVolume;
}


TGeoVolume* create_module(Int_t iModuleType)
{
  TGeoMedium* tBoxVolMed         = gGeoManager->GetMedium(gtBoxVolumeMedium);
  TGeoMedium* tNoActiveGasVolMed = gGeoManager->GetMedium(gtNoActivGasMedium);

  // create module volume
  TGeoBBox* tModuleShape =
    new TGeoBBox("", gdModule_X[iModuleType] / 2., gdModule_Y[iModuleType] / 2., gdModule_Z[iModuleType] / 2.);
  TGeoVolume* tModuleVolume = new TGeoVolume(Form("module_%d", iModuleType), tModuleShape, tBoxVolMed);

  // create gas box volume
  TGeoBBox* tGasBoxShape = new TGeoBBox(
    "", (gdModule_X[iModuleType] - (gdModuleWallWidth_Xplus[iModuleType] + gdModuleWallWidth_Xminus[iModuleType])) / 2.,
    (gdModule_Y[iModuleType] - 2. * gdModuleWallWidth_Y[iModuleType]) / 2.,
    (gdModule_Z[iModuleType] - 2. * gdModuleWallWidth_Z[iModuleType]) / 2.);
  TGeoVolume* tGasBoxVolume = new TGeoVolume("gas_box", tGasBoxShape, tNoActiveGasVolMed);

  // position gas box in the module
  tModuleVolume->AddNode(
    tGasBoxVolume, 0,
    new TGeoTranslation("", (gdModuleWallWidth_Xminus[iModuleType] - gdModuleWallWidth_Xplus[iModuleType]) / 2., 0.,
                        0.));

  // position all counters in the gas box
  for (Int_t iCounter = 0; iCounter < giNCountersInModule[iModuleType]; iCounter++) {
    TGeoTranslation* tTranslationMatrix = new TGeoTranslation();
    tTranslationMatrix->SetDx(gdCounterXPosition[iModuleType][iCounter]);
    tTranslationMatrix->SetDy(gdCounterYPosition[iModuleType][iCounter]);
    tTranslationMatrix->SetDz(gdCounterZPosition[iModuleType][iCounter]);

    TGeoRotation* tRotationMatrix = new TGeoRotation();
    tRotationMatrix->RotateX(gdCounterXRotation[iModuleType][iCounter]);
    tRotationMatrix->RotateY(gdCounterYRotation[iModuleType][iCounter]);
    tRotationMatrix->RotateZ(gdCounterZRotation[iModuleType][iCounter]);

    tGasBoxVolume->AddNode(gCounters[giCountersInModule[iModuleType][iCounter]], iCounter,
                           new TGeoCombiTrans(*tTranslationMatrix, *tRotationMatrix));
  }

  // drawing style
  tModuleVolume->SetLineColor(kGreen);
  tModuleVolume->SetTransparency(20);
  tGasBoxVolume->SetLineColor(kBlue);
  tGasBoxVolume->SetTransparency(50);

  return tModuleVolume;
}


void place_modules(Int_t iModuleType)
{
  for (Int_t iModule = 0; iModule < giNModulesInSetup[iModuleType]; iModule++) {
    TGeoTranslation* tTranslationMatrix = new TGeoTranslation();
    tTranslationMatrix->SetDx(gdModuleXPosition[iModuleType][iModule]);
    tTranslationMatrix->SetDy(gdModuleYPosition[iModuleType][iModule]);
    tTranslationMatrix->SetDz(gdModuleZPosition[iModuleType][iModule]);

    TGeoRotation* tRotationMatrix = new TGeoRotation();
    tRotationMatrix->RotateX(gdModuleXRotation[iModuleType][iModule]);
    tRotationMatrix->RotateY(gdModuleYRotation[iModuleType][iModule]);
    tRotationMatrix->RotateZ(gdModuleZRotation[iModuleType][iModule]);

    gtTofVolume->AddNode(gModules[iModuleType], iModule, new TGeoCombiTrans(*tTranslationMatrix, *tRotationMatrix));

    TGeoNode* tModuleNode = gtTofVolume->GetNode(gtTofVolume->GetNdaughters() - 1);
    tModuleNode->SetName(TString::Format("module_%d_%d", giModuleTypeIdentifier[iModuleType][iModule],
                                         giModuleIndexIdentifier[iModuleType][iModule]));
    tModuleNode->SetNumber(giModuleIndexIdentifier[iModuleType][iModule]);
  }
}
