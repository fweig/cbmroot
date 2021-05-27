// Macro creating a ToF geometry for test beam: v15c - Cern Nov 15, initial version
//
// Usage:
//

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

// Name of geometry version and output file
const TString geoVersion = "tof_v14a";
const TString FileName   = geoVersion + ".root";

// Names of the different used materials which are used to build the modules
// The materials are defined in the global media.geo file
const TString KeepingVolumeMedium = "air";
const TString BoxVolumeMedium     = "aluminium";
const TString NoActivGasMedium    = "RPCgas_noact";
const TString ActivGasMedium      = "RPCgas";
const TString GlasMedium          = "RPCglass";
const TString ElectronicsMedium   = "carbon";
const TString DiamondMedium       = "silicon";  // to get active material


// Counters:
//  0 HD small
//  1 HD big
//  2 HD ref
//  3 Buc ref
//  4 Ceramic
//  5 Diamond (16-strip pcCVD-DD)
//  6 Buc 2015a
//  7 Buc 2015b
//  8 THU Pad
//  9 THU strip
// 10 Diamond (4-segment mosaic pcCVD-DD)
const Int_t NumberOfDifferentCounterTypes            = 11;
const Float_t Glass_X[NumberOfDifferentCounterTypes] = {32., 52., 15.04, 18., 2., 2., 40.4, 28.8, 17.4, 25., 1.};
const Float_t Glass_Y[NumberOfDifferentCounterTypes] = {26.9, 53., 4., 4.6, 2., 2., 9.6, 9.6, 2.0, 26.7, 1.};
const Float_t Glass_Z[NumberOfDifferentCounterTypes] = {0.1, 0.1, 0.1, 0.1, 0.01, 0.1, 0.05, 0.05, 0.07, 0.1, 0.1};

const Float_t GasGap_X[NumberOfDifferentCounterTypes] = {32., 52., 15.04, 18., 2., 2., 40.4, 28.8, 17.4, 25., 1.};
const Float_t GasGap_Y[NumberOfDifferentCounterTypes] = {26.9, 53., 4., 4.6, 2., 2., 9.6, 9.6, 2.0, 26.7, 1.};
const Float_t GasGap_Z[NumberOfDifferentCounterTypes] = {0.022, 0.022, 0.022, 0.014, 0.02, 0.03,
                                                         0.014, 0.014, 0.025, 0.025, 0.018};

const Int_t NumberOfGaps[NumberOfDifferentCounterTypes]          = {8, 8, 6, 10, 6, 1, 10, 8, 10, 10, 1};
const Int_t NumberOfReadoutStrips[NumberOfDifferentCounterTypes] = {32, 56, 16, 72, 1, 16, 40, 40, 8, 24, 4};

const Float_t Electronics_X[NumberOfDifferentCounterTypes] = {34.0, 53.0, 15.5, 18.5, 2., 2., 30.5, 30.5, 0.2, 26., 1.};
const Float_t Electronics_Y[NumberOfDifferentCounterTypes] = {5.0, 5.0, 1.0, 1., 0.5, 0.1, 0.5, 0.5, 0.2, 5., 0.1};
const Float_t Electronics_Z[NumberOfDifferentCounterTypes] = {0.3, 0.3, 0.3, 0.3, 0.3, 0.1, 0.3, 0.3, 0.3, 0.3, 0.1};

const Int_t NofModuleTypes = 11;
// Aluminum box for all supermodule types
//  0 HDbig
//  1 BucRef
//  2 Ceramic
//  3 HD small (P2)
//  4 HD ref   (P5)
//  5 Diamond (16-strip pcCVD-DD)
//  6 Buc 2015
//  7 Buc 2012
//  8 THU Pad
//  9 Star 2-counter module
// 10 Diamond (4-segment mosaic pcCVD-DD)
const Float_t Module_Size_X[NofModuleTypes] = {120., 35., 9., 50., 30., 9., 50., 70., 29., 100., 8.};
const Float_t Module_Size_Y[NofModuleTypes] = {74., 18., 7., 49., 16., 5., 30., 35., 33., 49., 5.};
const Float_t Module_Size_Z[NofModuleTypes] = {10., 10., 2., 10., 10., 1., 15., 20., 10., 11., 1.};

const Float_t Module_Thick_Alu_X_left[NofModuleTypes]  = {0.1, 0.1, 0.1, 0.1, 1., 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
const Float_t Module_Thick_Alu_X_right[NofModuleTypes] = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};
const Float_t Module_Thick_Alu_Y[NofModuleTypes]       = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
const Float_t Module_Thick_Alu_Z[NofModuleTypes]       = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};

//Type of Counter for module
const Int_t CounterTypeInModule[NofModuleTypes] = {1, 3, 4, 0, 2, 5, 6, 7, 8, 0, 10};
const Int_t NCounterInModule[NofModuleTypes]    = {1, 1, 1, 1, 1, 1, 2, 4, 6, 2, 1};

// Placement of the counter inside the gas box
//const Float_t CounterXStartPosition[NofModuleTypes] = {-10.0,  0.0, 0.0,  0.0, 0.0, 0., 0., -15., 0.,  0., 0.};
const Float_t CounterXStartPosition[NofModuleTypes] = {-12.45, -2.45,  -2.45, -2.45, -2.,  -2.45,
                                                       -2.45,  -17.45, -2.45, -2.45, -2.45};
const Float_t CounterXDistance[NofModuleTypes]      = {30.0, 30.6, 0.0, 30.0, 0.0, 0., 3.4, 30., 0., 0., 0.};
const Float_t CounterYStartPosition[NofModuleTypes] = {0.0, 0.0, 0.0, 0.0, 0.0, 0., 0., -4., -7., 0., 0.};
const Float_t CounterYDistance[NofModuleTypes]      = {0.0, 0.0, 0.0, 0.0, 0.0, 0., 0., 8., 2.2, 0., 0.};
const Float_t CounterZStartPosition[NofModuleTypes] = {0.0, 0.0, 0.0, 2.5, 0.0, 0., -2., -8., 0., -2., 0.};
const Float_t CounterZDistance[NofModuleTypes]      = {0.0, 0.0, 0.0, 2.5, 0.0, 0., 4., 5., 4., 4., 0.};
const Float_t CounterRotationAngle[NofModuleTypes]  = {0., 0.0, 0.0, 0., 0.0, 0., 0., 0., 0., 0., 0.};


// Position for module placement

const Float_t HDsmall_Z_Position             = 445.5;
const Float_t HDsmall_First_Y_Position       = 51;  // HD - P2
const Float_t HDsmall_X_Offset               = 4.5;
const Float_t HDsmall_rotate_Z               = -90.;
const Int_t HDsmall_NTypes                   = 1;
const Float_t HDsmall_Types[HDsmall_NTypes]  = {3.};
const Float_t HDsmall_Number[HDsmall_NTypes] = {1.};  //debugging, V14b

const Float_t HDRef_Z_Position           = 359.;
const Float_t HDRef_First_Y_Position     = 39.7;
const Float_t HDRef_X_Offset             = 0.7;
const Float_t HDRef_rotate_Z             = 180.;
const Int_t HDRef_NTypes                 = 1;
const Float_t HDRef_Types[HDRef_NTypes]  = {4.};  // this is the SmType!
const Float_t HDRef_Number[HDRef_NTypes] = {1.};  // evtl. double for split signals

const Float_t Dia_Z_Position             = -1.;
const Float_t Dia_DeltaZ_Position        = -2.;
const Int_t Dia_NTypes                   = 3;
const Float_t Dia_X_Position[Dia_NTypes] = {2., 2., 0.};
const Float_t Dia_Y_Position[Dia_NTypes] = {0., 0., -2.};
const Float_t Dia_rotate_Z[Dia_NTypes]   = {0., 0., -90.};
const Float_t Dia_Types[Dia_NTypes]      = {5., 5., 5.};
const Float_t Dia_Number[Dia_NTypes]     = {1., 1., 1.};

const Float_t BucRef_Z_Position            = 492.;
const Float_t BucRef_First_Y_Position      = -28.;
const Float_t BucRef_X_Offset              = -5.;
const Float_t BucRef_rotate_Z              = 180.;
const Int_t BucRef_NTypes                  = 1;
const Float_t BucRef_Types[BucRef_NTypes]  = {1.};
const Float_t BucRef_Number[BucRef_NTypes] = {1.};  //debugging, V14b

const Float_t Buc2012_Z_Position             = 461.;
const Float_t Buc2012_First_Y_Position       = -24.2;  //-26.;
const Float_t Buc2012_X_Offset               = 4.2;
const Float_t Buc2012_rotate_Z               = 0.;
const Int_t Buc2012_NTypes                   = 1;
const Float_t Buc2012_Types[Buc2012_NTypes]  = {7.};
const Float_t Buc2012_Number[Buc2012_NTypes] = {1.};

const Float_t Buc2015_Z_Position             = 403.;
const Float_t Buc2015_First_Y_Position       = -23.3;
const Float_t Buc2015_X_Offset               = 0.;
const Float_t Buc2015_rotate_Z               = 0.;
const Int_t Buc2015_NTypes                   = 1;
const Float_t Buc2015_Types[Buc2015_NTypes]  = {6.};
const Float_t Buc2015_Number[Buc2015_NTypes] = {1.};

const Float_t USTC_Z_Position          = 332.4;
const Float_t USTC_First_Y_Position    = 61.0;
const Float_t USTC_X_Offset            = 0.;
const Float_t USTC_rotate_Z            = 90.;
const Int_t USTC_NTypes                = 1;
const Float_t USTC_Types[USTC_NTypes]  = {7.};
const Float_t USTC_Number[USTC_NTypes] = {1.};

const Float_t THU_Z_Position         = 360.;
const Float_t THU_First_Y_Position   = -19.0;
const Float_t THU_X_Offset           = 0.;
const Float_t THU_rotate_Z           = 90.;
const Int_t THU_NTypes               = 1;
const Float_t THU_Types[THU_NTypes]  = {8.};
const Float_t THU_Number[THU_NTypes] = {1.};

const Float_t Star2_First_Y_Position = 41.5;  //
const Float_t Star2_Delta_Y_Position = 2.5;   //
//const Float_t Star2_First_Z_Position=423.6;
//const Float_t Star2_Delta_Z_Position=-23.8;
const Float_t Star2_First_Z_Position     = 376.;
const Float_t Star2_Delta_Z_Position     = 23.8;
const Float_t Star2_X_Offset             = 0.;
const Float_t Star2_rotate_Z             = 180.;
const Int_t Star2_NTypes                 = 1;
const Float_t Star2_Types[Star2_NTypes]  = {9.};
const Float_t Star2_Number[Star2_NTypes] = {3.};  //debugging, V15c

const Int_t Cer_NTypes                   = 2;
const Float_t Cer_X_Position[Cer_NTypes] = {0., 0.};
const Float_t Cer_Y_Position[Cer_NTypes] = {-29., -29.};
const Float_t Cer_Z_Position[Cer_NTypes] = {521.3, 526.3};
const Float_t Cer_rotate_Z[Cer_NTypes]   = {0., 0.};
const Float_t Cer_Types[Cer_NTypes]      = {2., 2.};
const Float_t Cer_Number[Cer_NTypes]     = {1., 1.};  //V15c


// some global variables
TGeoManager* gGeoMan = NULL;           // Pointer to TGeoManager instance
TGeoVolume* gModules[NofModuleTypes];  // Global storage for module types
TGeoVolume* gCounter[NumberOfDifferentCounterTypes];

// Forward declarations
void create_materials_from_media_file();
TGeoVolume* create_new_counter(Int_t);
TGeoVolume* create_new_tof_module(Int_t);
void position_cer_modules(Int_t);
void position_HDsmall(Int_t);
void position_BucRef(Int_t);
void position_HDRef(Int_t);
void position_Dia(Int_t);
void position_Buc2012(Int_t);
void position_Buc2015(Int_t);
void position_USTC(Int_t);
void position_THU(Int_t);
void position_Star2(Int_t);


void Create_TOF_Geometry_v15c()
{
  // Load needed material definition from media.geo file
  create_materials_from_media_file();

  // Get the GeoManager for later usage
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetVisLevel(5);  // 2 = super modules
  gGeoMan->SetVisOption(0);

  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);

  TGeoVolume* tof = new TGeoVolumeAssembly(geoVersion);
  top->AddNode(tof, 1);

  for (Int_t counterType = 0; counterType < NumberOfDifferentCounterTypes; counterType++) {
    gCounter[counterType] = create_new_counter(counterType);
  }

  for (Int_t moduleType = 0; moduleType < NofModuleTypes; moduleType++) {
    cout << "Create Module " << moduleType << endl;
    gModules[moduleType] = create_new_tof_module(moduleType);  // create modules
    gModules[moduleType]->SetVisContainers(1);
  }

  position_Dia(3);
  position_HDsmall(1);
  position_HDRef(1);
  position_Star2(1);
  position_BucRef(1);
  position_Buc2015(1);
  position_Buc2012(1);
  position_THU(1);
  position_cer_modules(2);

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* outfile = new TFile(FileName, "RECREATE");
  top->Write();
  //  gGeoMan->Write();
  outfile->Close();
  top->SetVisContainers(1);
  gGeoMan->SetVisLevel(5);
  if (!gROOT->IsBatch()) top->Draw("ogl");
  //top->Draw();
  //gModules[0]->Draw("ogl");
  //  gModules[0]->Draw("");
  gModules[0]->SetVisContainers(1);
  //  gModules[1]->Draw("");
  gModules[1]->SetVisContainers(1);
  //gModules[5]->Draw("");
  //  top->Raytrace();
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

  FairGeoMedium* air          = geoMedia->getMedium("air");
  FairGeoMedium* aluminium    = geoMedia->getMedium("aluminium");
  FairGeoMedium* RPCgas       = geoMedia->getMedium("RPCgas");
  FairGeoMedium* RPCgas_noact = geoMedia->getMedium("RPCgas_noact");
  FairGeoMedium* RPCglass     = geoMedia->getMedium("RPCglass");
  FairGeoMedium* carbon       = geoMedia->getMedium("carbon");
  FairGeoMedium* silicon      = geoMedia->getMedium("silicon");

  // include check if all media are found

  geoBuild->createMedium(air);
  geoBuild->createMedium(aluminium);
  geoBuild->createMedium(RPCgas);
  geoBuild->createMedium(RPCgas_noact);
  geoBuild->createMedium(RPCglass);
  geoBuild->createMedium(carbon);
  geoBuild->createMedium(silicon);
}

TGeoVolume* create_new_counter(Int_t iCounterType)
{

  //glass
  Float_t gdx = Glass_X[iCounterType];
  Float_t gdy = Glass_Y[iCounterType];
  Float_t gdz = Glass_Z[iCounterType];

  //gas gap
  Int_t nstrips = NumberOfReadoutStrips[iCounterType];
  Int_t ngaps   = NumberOfGaps[iCounterType];


  Float_t ggdx = GasGap_X[iCounterType];
  Float_t ggdy = GasGap_Y[iCounterType];
  Float_t ggdz = GasGap_Z[iCounterType];

  // electronics
  //pcb dimensions
  Float_t dxe  = Electronics_X[iCounterType];
  Float_t dye  = Electronics_Y[iCounterType];
  Float_t dze  = Electronics_Z[iCounterType];
  Float_t yele = gdy / 2. + dye / 2.;

  // counter size (calculate from glas, gap and electronics sizes)
  Float_t cdx = TMath::Max(gdx, ggdx);
  cdx         = TMath::Max(cdx, dxe) + 0.2;
  Float_t cdy = TMath::Max(gdy, ggdy) + 2 * dye + 0.2;
  Float_t cdz = ngaps * (gdz + ggdz) + gdz + 0.2;

  //calculate thickness and first position in coonter of single stack
  Float_t dzpos = gdz + ggdz;
  //  Float_t startzposglas=(-cdz+gdz)/2.; // FIXME
  //  Float_t startzposgas=-cdz/2.+ gdz + ggdz/2.;
  Float_t startzposglas = -cdz / 2. + 0.1 + gdz / 2.;
  Float_t startzposgas  = startzposglas + dzpos / 2.;


  // needed materials
  TGeoMedium* glassPlateVolMed  = gGeoMan->GetMedium(GlasMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);
  TGeoMedium* activeGasVolMed   = gGeoMan->GetMedium(ActivGasMedium);
  TGeoMedium* electronicsVolMed = gGeoMan->GetMedium(ElectronicsMedium);
  TGeoMedium* DiamondVolMed     = gGeoMan->GetMedium(DiamondMedium);

  // define counter volume
  TGeoBBox* counter_box = new TGeoBBox("", cdx / 2., cdy / 2., cdz / 2.);
  TGeoVolume* counter   = new TGeoVolume("counter", counter_box, noActiveGasVolMed);
  counter->SetLineColor(kCyan);  // set line color for the counter
  counter->SetTransparency(70);  // set transparency for the TOF

  // define single glass plate volume
  TGeoBBox* glass_plate       = new TGeoBBox("", gdx / 2., gdy / 2., gdz / 2.);
  TGeoVolume* glass_plate_vol = new TGeoVolume("tof_glass", glass_plate, glassPlateVolMed);
  glass_plate_vol->SetLineColor(kYellow);  // set line color for the glass plate
  glass_plate_vol->SetTransparency(0);     // set transparency for the TOF
  // define single gas gap volume
  TGeoBBox* gas_gap       = new TGeoBBox("", ggdx / 2., ggdy / 2., ggdz / 2.);
  TGeoVolume* gas_gap_vol = NULL;
  if (5 == iCounterType || 10 == iCounterType) { gas_gap_vol = new TGeoVolume("Gap", gas_gap, DiamondVolMed); }
  else {
    gas_gap_vol = new TGeoVolume("Gap", gas_gap, activeGasVolMed);
  }

  if (10 == iCounterType) {
    // place 4 pad daughter volumes in the gap volume
    Float_t fPadX = ggdx / (Float_t)(nstrips);
    Float_t fPadY = ggdy / (Float_t)(nstrips);

    TGeoBBox* tPadShape    = new TGeoBBox("", fPadX, fPadY, ggdz / 2.);
    TGeoVolume* tPadVolume = new TGeoVolume("Cell", tPadShape, DiamondVolMed);

    TGeoTranslation* tPadTrans1 = new TGeoTranslation("", fPadX, fPadY, 0.);
    TGeoTranslation* tPadTrans2 = new TGeoTranslation("", -fPadX, fPadY, 0.);
    TGeoTranslation* tPadTrans3 = new TGeoTranslation("", -fPadX, -fPadY, 0.);
    TGeoTranslation* tPadTrans4 = new TGeoTranslation("", fPadX, -fPadY, 0.);

    gas_gap_vol->AddNode(tPadVolume, 1, tPadTrans1);
    gas_gap_vol->AddNode(tPadVolume, 2, tPadTrans2);
    gas_gap_vol->AddNode(tPadVolume, 3, tPadTrans3);
    gas_gap_vol->AddNode(tPadVolume, 4, tPadTrans4);
  }
  else {
    gas_gap_vol->Divide("Cell", 1, nstrips, -ggdx / 2., 0);
  }

  gas_gap_vol->SetLineColor(kRed);   // set line color for the gas gap
  gas_gap_vol->SetTransparency(50);  // set transparency for the TOF

  // place 8 gas gaps and 9 glas plates in the counter
  for (Int_t igap = 0; igap < ngaps; igap++) {

    Float_t zpos_glas = startzposglas + igap * dzpos;
    Float_t zpos_gas  = startzposgas + igap * dzpos;

    //    cout <<"Zpos(Glas): "<< zpos_glas << endl;
    //    cout <<"Zpos(Gas): "<< zpos_gas << endl;

    TGeoTranslation* glass_plate_trans = new TGeoTranslation("", 0., 0., zpos_glas);
    TGeoTranslation* gas_gap_trans     = new TGeoTranslation("", 0., 0., zpos_gas);
    counter->AddNode(glass_plate_vol, igap, glass_plate_trans);
    counter->AddNode(gas_gap_vol, igap, gas_gap_trans);
  }

  //  Float_t zpos_glas = startzposglas + (ngaps+1)*dzpos; // FIXME
  Float_t zpos_glas                  = startzposglas + ngaps * dzpos;
  TGeoTranslation* glass_plate_trans = new TGeoTranslation("", 0., 0., zpos_glas);
  counter->AddNode(glass_plate_vol, ngaps, glass_plate_trans);

  // create and place the electronics above and below the glas stack
  TGeoBBox* pcb       = new TGeoBBox("", dxe / 2., dye / 2., dze / 2.);
  TGeoVolume* pcb_vol = new TGeoVolume("pcb", pcb, electronicsVolMed);
  pcb_vol->SetLineColor(kGreen);  // set line color for electronics
  pcb_vol->SetTransparency(10);   // set transparency for the TOF
  for (Int_t l = 0; l < 2; l++) {
    yele *= -1.;
    TGeoTranslation* pcb_trans = new TGeoTranslation("", 0., yele, 0.);
    counter->AddNode(pcb_vol, l, pcb_trans);
  }


  return counter;
}


TGeoVolume* create_new_tof_module(Int_t modType)
{
  Int_t cType         = CounterTypeInModule[modType];
  Float_t dx          = Module_Size_X[modType];
  Float_t dy          = Module_Size_Y[modType];
  Float_t dz          = Module_Size_Z[modType];
  Float_t width_aluxl = Module_Thick_Alu_X_left[modType];
  Float_t width_aluxr = Module_Thick_Alu_X_right[modType];
  Float_t width_aluy  = Module_Thick_Alu_Y[modType];
  Float_t width_aluz  = Module_Thick_Alu_Z[modType];

  Float_t shift_gas_box = (width_aluxr - width_aluxl) / 2;

  Float_t dxpos     = CounterXDistance[modType];
  Float_t startxpos = CounterXStartPosition[modType];
  Float_t dypos     = CounterYDistance[modType];
  Float_t startypos = CounterYStartPosition[modType];
  Float_t dzoff     = CounterZDistance[modType];
  Float_t rotangle  = CounterRotationAngle[modType];

  TGeoMedium* boxVolMed         = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);

  TString moduleName = Form("module_%d", modType);

  TGeoBBox* module_box = new TGeoBBox("", dx / 2., dy / 2., dz / 2.);
  TGeoVolume* module   = new TGeoVolume(moduleName, module_box, boxVolMed);
  module->SetLineColor(kGreen);  // set line color for the alu box
  module->SetTransparency(20);   // set transparency for the TOF

  TGeoBBox* gas_box =
    new TGeoBBox("", (dx - (width_aluxl + width_aluxr)) / 2., (dy - 2 * width_aluy) / 2., (dz - 2 * width_aluz) / 2.);
  TGeoVolume* gas_box_vol = new TGeoVolume("gas_box", gas_box, noActiveGasVolMed);
  gas_box_vol->SetLineColor(kBlue);  // set line color for the alu box
  gas_box_vol->SetTransparency(50);  // set transparency for the TOF
  TGeoTranslation* gas_box_trans = new TGeoTranslation("", shift_gas_box, 0., 0.);
  module->AddNode(gas_box_vol, 0, gas_box_trans);

  for (Int_t j = 0; j < NCounterInModule[modType]; j++) {  //loop over counters (modules)
    Float_t xpos, ypos, zpos;
    if (0 == modType || 3 == modType || 4 == modType || 5 == modType) { zpos = dzoff *= -1; }
    else {
      zpos = CounterZStartPosition[modType] + j * dzoff;
    }
    if (8 == modType) {  // ThuPad
      switch (j) {
        case 0:
        case 1:
        case 4:
        case 5: zpos = dzoff; break;
        case 2:
        case 3: zpos = -dzoff; break;
      }
    }
    xpos = startxpos + j * dxpos;
    ypos = startypos + j * dypos;
    if (6 == modType) {       //Buc2015a/b
      if (j == 1) cType = 7;  //second guy is a
    }
    if (7 == modType) {  //Buc2012
      Int_t zsh[4] = {0, 2, 1, 3};
      xpos         = startxpos + Float_t((Int_t) j / 2) * dxpos;
      ypos         = startypos + Float_t((Int_t) j % 2) * dypos;
      zpos         = CounterZStartPosition[modType] + zsh[j] * dzoff;
    }
    cout << "Add new counter " << cType << " at " << startxpos + j * dxpos << ", " << startypos + j * dypos << ", "
         << zpos << endl;

    TGeoTranslation* counter_trans = new TGeoTranslation("", xpos, ypos, zpos);
    TGeoRotation* counter_rot      = new TGeoRotation();
    counter_rot->RotateY(rotangle);
    TGeoCombiTrans* counter_combi_trans = new TGeoCombiTrans(*counter_trans, *counter_rot);
    gas_box_vol->AddNode(gCounter[cType], j, counter_combi_trans);
  }

  return module;
}


void position_HDsmall(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(HDsmall_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = HDsmall_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = HDsmall_X_Offset;
  Float_t zPos = HDsmall_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = HDsmall_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < HDsmall_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_Star2(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(Star2_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t xPos = Star2_X_Offset;
  Float_t yPos = Star2_First_Y_Position;
  Float_t zPos = Star2_First_Z_Position;
  Int_t ii     = 0;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = Star2_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < Star2_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
      yPos += Star2_Delta_Y_Position;
      zPos += Star2_Delta_Z_Position;
    }
  }
}

void position_BucRef(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(BucRef_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = BucRef_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = BucRef_X_Offset;
  Float_t zPos = BucRef_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = BucRef_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < BucRef_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_HDRef(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(HDRef_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = HDRef_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = HDRef_X_Offset;
  Float_t zPos = HDRef_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = HDRef_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < HDRef_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_Dia(Int_t modNType)
{
  TGeoTranslation* module_trans      = NULL;
  TGeoRotation* module_rot           = NULL;
  TGeoCombiTrans* module_combi_trans = NULL;

  Int_t ii = 0;

  Int_t modNum = 0;
  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = Dia_Types[j];
    Float_t xPos  = Dia_X_Position[j];
    Float_t yPos  = Dia_Y_Position[j];
    Float_t zPos  = Dia_Z_Position + j * Dia_DeltaZ_Position;

    for (Int_t i = 0; i < Dia_Number[j]; i++) {
      ii++;
      module_trans = new TGeoTranslation("", xPos, yPos, zPos);
      module_rot   = new TGeoRotation();
      module_rot->RotateZ(Dia_rotate_Z[j]);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      /*
    // use node name "module_5" for both pad and strip diamond detectors
    if( 10 == modType )
    {
      TGeoNode* tModule = gGeoMan->GetVolume(geoVersion)->GetNode(gGeoMan->GetVolume(geoVersion)->GetNdaughters() - 1);
      tModule->SetName(TString::Format("module_5_%d", modNum));
    }
*/
      modNum++;
    }
  }
}

void position_Buc2012(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(Buc2012_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = Buc2012_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = Buc2012_X_Offset;
  Float_t zPos = Buc2012_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = Buc2012_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < Buc2012_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_Buc2015(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(Buc2015_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = Buc2015_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = Buc2015_X_Offset;
  Float_t zPos = Buc2015_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = Buc2015_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < Buc2015_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_USTC(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(USTC_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = USTC_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = USTC_X_Offset;
  Float_t zPos = USTC_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = USTC_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < USTC_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_THU(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(THU_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = THU_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = THU_X_Offset;
  Float_t zPos = THU_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = THU_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < THU_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_cer_modules(Int_t modNType)
{
  Int_t ii     = 0;
  Int_t modNum = 0;
  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType                 = Cer_Types[j];
    Float_t xPos                  = Cer_X_Position[j];
    Float_t yPos                  = Cer_Y_Position[j];
    Float_t zPos                  = Cer_Z_Position[j];
    TGeoTranslation* module_trans = NULL;
    TGeoRotation* module_rot      = new TGeoRotation();
    module_rot->RotateZ(Cer_rotate_Z[j]);
    TGeoCombiTrans* module_combi_trans = NULL;

    for (Int_t i = 0; i < Cer_Number[j]; i++) {
      ii++;
      cout << "Position Ceramic Module " << i << " of " << Cer_Number[j] << " Type " << modType << " at X = " << xPos
           << ", Y = " << yPos << ", Z = " << zPos << endl;
      // Front staggered module (Top if pair), top
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}
