/** @file Create_TOF_Geometry_v20f_mcbm.C
  * @copyright Copyright (C) 2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

///
/// \file Create_TOF_Geometry_v20f_mcbm.C
/// \brief Generates TOF geometry in Root format.
///

// Changelog
//
// 2020-04-14 - v20b - NH - swapped double stack layer 2 with STAR2 moodule, buc kept as dummy
// 2020-04-01 - v20a - NH - move mTOF +20 cm in x direction for the Mar 2020 run
// 2019-11-28 - v19b - DE - move mTOF +12 cm in x direction for the Nov 2019 run
// 2019-07-31 - v19a - DE - this TOF March 2019 geometry is also known as v18m
// 2017-11-03 - v18i - DE - shift mTOF to z=298 cm for acceptance matching with mSTS
// 2017-10-06 - v18h - DE - put v18f into vertical position to fit into the mCBM cave
// 2017-07-15 - v18g - DE - swap the z-position of TOF modules: 2 in the front, 3 in the back
// 2017-07-14 - v18f - DE - reduce vertical gap between TOF modules to fix the gap between modules 1-2 and 4-5
// 2017-05-17 - v18e - DE - rotate electronics away from beam, shift 16 cm away from beam along x-axis
// 2017-05-17 - v18d - DE - change geometry name to v18d

// in root all sizes are given in cm

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
const TString geoVersion      = "tof_v20f_mcbm";  // do not change
const TString geoVersionStand = geoVersion + "Stand";
//
const TString fileTag      = "tof_v20f";
const TString FileNameSim  = fileTag + "_mcbm.geo.root";
const TString FileNameGeo  = fileTag + "_mcbm_geo.root";
const TString FileNameInfo = fileTag + "_mcbm.geo.info";

// TOF_Z_Front corresponds to front cover of outer super module towers
const Float_t TOF_Z_Front_Stand = 247.2;  // = z=298 mCBM@SIS18
const Float_t TOF_X_Front_Stand = 0.;     // = z=298 mCBM@SIS18
const Float_t TOF_Z_Front       = 0.;     // = z=298 mCBM@SIS18
//const Float_t TOF_Z_Front =  130;  // = z=225 mCBM@SIS18
//const Float_t TOF_Z_Front =  250;  // SIS 100 hadron
//const Float_t TOF_Z_Front =  450;  // SIS 100 hadron
//const Float_t TOF_Z_Front =  600;  // SIS 100 electron
//const Float_t TOF_Z_Front =  650;  // SIS 100 muon
//const Float_t TOF_Z_Front =  880;  // SIS 300 electron
//const Float_t TOF_Z_Front = 1020;  // SIS 300 muon
//
//const Float_t TOF_Z_Front = 951.5;   // Wall_Z_Position = 1050 cm


// Names of the different used materials which are used to build the modules
// The materials are defined in the global media.geo file
const TString KeepingVolumeMedium = "air";
const TString BoxVolumeMedium     = "aluminium";
const TString NoActivGasMedium    = "RPCgas_noact";
const TString ActivGasMedium      = "RPCgas";
const TString GlasMedium          = "RPCglass";
const TString ElectronicsMedium   = "carbon";

// Counters:
// 0 MRPC3a
// 1 MRPC3b
// 2 USTC
// 3
// 4 Diamond
//
// 6 Buc 2019
// 7 CERN 20gap
// 8 Ceramic Pad
const Int_t NumberOfDifferentCounterTypes            = 9;
const Float_t Glass_X[NumberOfDifferentCounterTypes] = {32., 52., 32., 32., 0.2, 32., 28.8, 20., 2.4};
const Float_t Glass_Y[NumberOfDifferentCounterTypes] = {27.0, 53., 26.8, 10., 0.2, 10., 6., 20., 2.4};
const Float_t Glass_Z[NumberOfDifferentCounterTypes] = {0.1, 0.1, 0.1, 0.1, 0.01, 0.1, 0.1, 0.1, 0.1};

const Float_t GasGap_X[NumberOfDifferentCounterTypes] = {32., 52., 32., 32., 0.2, 32., 28.8, 20., 2.4};
const Float_t GasGap_Y[NumberOfDifferentCounterTypes] = {27.0, 53., 26.8, 10., 0.2, 10., 6., 20., 2.4};
const Float_t GasGap_Z[NumberOfDifferentCounterTypes] = {0.025, 0.025, 0.025, 0.025, 0.01, 0.02, 0.02, 0.02, 0.025};

const Int_t NumberOfGaps[NumberOfDifferentCounterTypes] = {8, 8, 8, 8, 1, 8, 10, 20, 4};
//const Int_t NumberOfGaps[NumberOfDifferentCounterTypes] = {1,1,1,1}; //deb
const Int_t NumberOfReadoutStrips[NumberOfDifferentCounterTypes] = {32, 32, 32, 32, 8, 32, 32, 20, 1};
//const Int_t NumberOfReadoutStrips[NumberOfDifferentCounterTypes] = {1,1,1,1}; //deb

const Float_t SingleStackStartPosition_Z[NumberOfDifferentCounterTypes] = {-0.6, -0.6, -0.6, -0.6, -0.1,
                                                                           -0.6, -0.6, -0.6, -1.};

const Float_t Electronics_X[NumberOfDifferentCounterTypes] = {34.0, 53.0, 32.0, 32., 0.3, 0.1, 28.8, 20., 0.1};
const Float_t Electronics_Y[NumberOfDifferentCounterTypes] = {5.0, 5.0, 1.0, 1., 0.1, 0.1, 1.0, 1.0, 0.1};
const Float_t Electronics_Z[NumberOfDifferentCounterTypes] = {0.3, 0.3, 0.3, 0.3, 0.1, 0.1, 0.1, 0.1, 0.1};

const Int_t NofModuleTypes = 10;
// 5 Diamond
// 6 Buc
// 7 CERN 20 gap
// 8 Ceramic
// 9 Star2
// Aluminum box for all module types
const Float_t Module_Size_X[NofModuleTypes] = {180., 180., 180., 180., 180., 5., 40., 30., 22.5, 100.};
const Float_t Module_Size_Y[NofModuleTypes] = {49., 49., 74., 28., 18., 5., 12., 30., 11., 49.};
const Float_t Module_Over_Y[NofModuleTypes] = {11.5, 11.5, 11., 4.5, 4.5, 0., 0., 0., 0., 0.};
const Float_t Module_Size_Z[NofModuleTypes] = {11., 11., 13., 11., 11., 1., 12., 6., 6.2, 11.2};
const Float_t Module_Thick_Alu_X_left       = 0.1;
const Float_t Module_Thick_Alu_X_right      = 1.0;
const Float_t Module_Thick_Alu_Y            = 0.1;
const Float_t Module_Thick_Alu_Z            = 0.1;

// Distance to the center of the TOF wall [cm];
const Float_t Wall_Z_Position = 400.;
const Float_t MeanTheta       = 0.;

//Type of Counter for module
const Int_t CounterTypeInModule[NofModuleTypes] = {0, 0, 1, 2, 3, 4, 6, 7, 8, 2};
const Int_t NCounterInModule[NofModuleTypes]    = {5, 5, 3, 5, 5, 1, 2, 1, 8, 2};

// Placement of the counter inside the module
const Float_t CounterXStartPosition[NofModuleTypes] = {-60.1, -66.0, -56.0, -60.0, -60.0, 0.0, 0., 0., -7., 0.};
const Float_t CounterXDistance[NofModuleTypes]      = {29.3, 32.0, 51.0, 30.0, 30.0, 0.0, 0., 0., 2., -1.};
const Float_t CounterYStartPosition[NofModuleTypes] = {0.0, 0.0, 0.0, 0.0, 0.0, 0., 0., -4., -1.3, 0.};
const Float_t CounterYDistance[NofModuleTypes]      = {0.0, 0.0, 0.0, 0.0, 0.0, 0., 0., 8., 0., 1.};
const Float_t CounterZDistance[NofModuleTypes]      = {-2.5, 0.0, 0.0, 2.5, 2.5, 0., 6., 0., 0.1, 4.};
const Float_t CounterZStartPosition[NofModuleTypes] = {0.0, 0.0, 0.0, 0.0, 0.0, 0., -3., 0., 0.0, -2.};
const Float_t CounterRotationAngle[NofModuleTypes]  = {0., 8.7, 7.0, 0., 0., 0., 0., 0., 0., 0.};

// Pole (support structure)
const Int_t MaxNumberOfPoles = 20;
Float_t Pole_ZPos[MaxNumberOfPoles];
Float_t Pole_Col[MaxNumberOfPoles];
Int_t NumberOfPoles = 0;

const Float_t Pole_Size_X  = 20.;
const Float_t Pole_Size_Y  = 300.;
const Float_t Pole_Size_Z  = 10.;
const Float_t Pole_Thick_X = 5.;
const Float_t Pole_Thick_Y = 5.;
const Float_t Pole_Thick_Z = 5.;

// Bars (support structure)
const Float_t Bar_Size_X = 20.;
const Float_t Bar_Size_Y = 20.;
Float_t Bar_Size_Z       = 100.;

const Int_t MaxNumberOfBars = 20;
Float_t Bar_ZPos[MaxNumberOfBars];
Float_t Bar_XPos[MaxNumberOfBars];
Int_t NumberOfBars = 0;

const Float_t ChamberOverlap = 40;
const Float_t DxColl         = 158.0;  //Module_Size_X-ChamberOverlap;
//const Float_t Pole_Offset=Module_Size_X/2.+Pole_Size_X/2.;
const Float_t Pole_Offset = 90.0 + Pole_Size_X / 2.;

// Position for module placement
const Float_t Inner_Module_First_Y_Position = 16.;
const Float_t Inner_Module_Last_Y_Position  = 480.;
const Float_t Inner_Module_X_Offset         = 0.;  // centered position in x/y
//const Float_t Inner_Module_X_Offset=18; // shift by 16 cm in x
const Int_t Inner_Module_NTypes                       = 3;
const Float_t Inner_Module_Types[Inner_Module_NTypes] = {4., 3., 0.};
//const Float_t Inner_Module_Number[Inner_Module_NTypes] = {2.,2.,6.}; //V13_3a
const Float_t Inner_Module_Number[Inner_Module_NTypes] = {2., 2., 1.};  //V13_3a
//const Float_t Inner_Module_Number[Inner_Module_NTypes] = {0.,0.,0.}; //debugging

const Float_t InnerSide_Module_X_Offset                    = 51.;
const Float_t InnerSide_Module_NTypes                      = 1;
const Float_t InnerSide_Module_Types[Inner_Module_NTypes]  = {5.};
const Float_t InnerSide_Module_Number[Inner_Module_NTypes] = {2.};  //v13_3a
//const Float_t InnerSide_Module_Number[Inner_Module_NTypes] = {0.};  //debug

const Float_t Outer_Module_First_Y_Position                              = 0.;
const Float_t Outer_Module_Last_Y_Position                               = 480.;
const Float_t Outer_Module_X_Offset                                      = 3.;
const Int_t Outer_Module_Col                                             = 4;
const Int_t Outer_Module_NTypes                                          = 2;
const Float_t Outer_Module_Types[Outer_Module_NTypes][Outer_Module_Col]  = {1., 1., 1., 1., 2., 2., 2., 2.};
const Float_t Outer_Module_Number[Outer_Module_NTypes][Outer_Module_Col] = {9., 9., 2., 0., 0., 0., 3., 4.};  //V13_3a
//const Float_t Outer_Module_Number[Outer_Module_NTypes][Outer_Module_Col] = {1.,1.,0.,0.,  0.,0.,0.,0.};//debug

const Float_t Star2_First_Z_Position       = TOF_Z_Front + 16.5;
const Float_t Star2_Delta_Z_Position       = 0.;
const Float_t Star2_First_Y_Position       = 30.35;  //
const Float_t Star2_Delta_Y_Position       = 0.;     //
const Float_t Star2_rotate_Z               = -90.;
const Int_t Star2_NTypes                   = 1;
const Float_t Star2_Types[Star2_NTypes]    = {9.};
const Float_t Star2_Number[Star2_NTypes]   = {1.};   //debugging, V16b
const Float_t Star2_X_Offset[Star2_NTypes] = {49.};  //{51.};

const Float_t Buc_First_Z_Position     = TOF_Z_Front + 50.;
const Float_t Buc_Delta_Z_Position     = 0.;
const Float_t Buc_First_Y_Position     = -32.5;  //
const Float_t Buc_Delta_Y_Position     = 0.;     //
const Float_t Buc_rotate_Z             = 180.;
const Int_t Buc_NTypes                 = 1;
const Float_t Buc_Types[Buc_NTypes]    = {6.};
const Float_t Buc_Number[Buc_NTypes]   = {1.};  //debugging, V16b
const Float_t Buc_X_Offset[Buc_NTypes] = {53.5};

const Int_t Cer_NTypes                   = 3;
const Float_t Cer_Z_Position[Cer_NTypes] = {(float) (TOF_Z_Front + 13.2), (float) (TOF_Z_Front + 45.),
                                            (float) (TOF_Z_Front + 45.)};
const Float_t Cer_X_Position[Cer_NTypes] = {0., 49.8, 49.8};
const Float_t Cer_Y_Position[Cer_NTypes] = {-1., 5., 5.};
const Float_t Cer_rotate_Z[Cer_NTypes]   = {0., 0., 0.};
const Float_t Cer_Types[Cer_NTypes]      = {5., 8., 8.};
const Float_t Cer_Number[Cer_NTypes]     = {1., 1., 1.};  //V16b

const Float_t CERN_Z_Position          = TOF_Z_Front + 50;  // 20 gap
const Float_t CERN_First_Y_Position    = 36.;
const Float_t CERN_X_Offset            = 46.;  //65.5;
const Float_t CERN_rotate_Z            = 90.;
const Int_t CERN_NTypes                = 1;
const Float_t CERN_Types[CERN_NTypes]  = {7.};  // this is the SmType!
const Float_t CERN_Number[CERN_NTypes] = {1.};  // evtl. double for split signals

// some global variables
TGeoManager* gGeoMan = NULL;           // Pointer to TGeoManager instance
TGeoVolume* gModules[NofModuleTypes];  // Global storage for module types
TGeoVolume* gCounter[NumberOfDifferentCounterTypes];
TGeoVolume* gPole;
TGeoVolume* gBar[MaxNumberOfBars];

const Float_t Dia_Z_Position         = -0.2 - TOF_Z_Front_Stand;
const Float_t Dia_First_Y_Position   = 0.;
const Float_t Dia_X_Offset           = 0.;
const Float_t Dia_rotate_Z           = 0.;
const Int_t Dia_NTypes               = 1;
const Float_t Dia_Types[Dia_NTypes]  = {5.};
const Float_t Dia_Number[Dia_NTypes] = {1.};

Float_t Last_Size_Y = 0.;
Float_t Last_Over_Y = 0.;

// Forward declarations
void create_materials_from_media_file();
TGeoVolume* create_counter(Int_t);
TGeoVolume* create_new_counter(Int_t);
TGeoVolume* create_tof_module(Int_t);
TGeoVolume* create_new_tof_module(Int_t);
TGeoVolume* create_tof_pole();
TGeoVolume* create_tof_bar();
void position_tof_poles(Int_t);
void position_tof_bars(Int_t);
void position_inner_tof_modules(Int_t);
void position_side_tof_modules(Int_t);
void position_outer_tof_modules(Int_t);
void position_Dia(Int_t);
void position_Star2(Int_t);
void position_Buc(Int_t);
void position_cer_modules(Int_t);
void position_CERN(Int_t);
void dump_info_file();


void Create_TOF_Geometry_v20f_mcbm()
{

  // Load needed material definition from media.geo file
  create_materials_from_media_file();

  // Get the GeoManager for later usage
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetVisLevel(5);  // 2 = super modules
  gGeoMan->SetVisOption(0);

  // Create the top volume
  /*
  TGeoBBox* topbox= new TGeoBBox("", 1000., 1000., 1000.);
  TGeoVolume* top = new TGeoVolume("top", topbox, gGeoMan->GetMedium("air"));
  gGeoMan->SetTopVolume(top);
  */

  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);

  TGeoRotation* tof_rotation = new TGeoRotation();
  tof_rotation->RotateY(0.);  // angle with respect to beam axis
    //tof_rotation->RotateZ(   0 );   // electronics on  9 o'clock position = +x
  //  tof_rotation->RotateZ(   0 );   // electronics on  9 o'clock position = +x
  //  tof_rotation->RotateZ(  90 );   // electronics on 12 o'clock position (top)
  //  tof_rotation->RotateZ( 180 );   // electronics on  3 o'clock position = -x
  //  tof_rotation->RotateZ( 270 );   // electronics on  6 o'clock position (bottom)

  TGeoVolume* tof = new TGeoVolumeAssembly(geoVersion);
  //  top->AddNode(tof, 1, tof_rotation);
  top->AddNode(tof, 1);

  TGeoVolume* tofstand = new TGeoVolumeAssembly(geoVersionStand);
  // Mar 2020 run
  TGeoTranslation* stand_trans_local = new TGeoTranslation("", TOF_X_Front_Stand, 0., 0.);
  TGeoTranslation* stand_trans       = new TGeoTranslation("", 0., 0., TOF_Z_Front_Stand);
  TGeoCombiTrans* stand_combi_trans  = new TGeoCombiTrans(*stand_trans, *tof_rotation);

  // Nov 2019 run
  // TGeoTranslation*  stand_trans   = new TGeoTranslation("", 12., 0., TOF_Z_Front_Stand);
  // TGeoTranslation*  stand_trans   = new TGeoTranslation("",  0., 0., TOF_Z_Front_Stand);
  TGeoRotation* stand_rot = new TGeoRotation();
  stand_rot->RotateY(0.55);
  //stand_rot->RotateY(1.0);
  TGeoCombiTrans* stand_combi_trans_local = new TGeoCombiTrans(*stand_trans_local, *stand_rot);

  //tof->AddNode(tofstand, 1, stand_combi_trans);
  tof->AddNode(tofstand, 1, stand_combi_trans_local);
  //tof->AddNode(tofstand, 1);

  for (Int_t counterType = 0; counterType < NumberOfDifferentCounterTypes; counterType++) {
    gCounter[counterType] = create_new_counter(counterType);
  }

  for (Int_t moduleType = 0; moduleType < NofModuleTypes; moduleType++) {
    gModules[moduleType] = create_new_tof_module(moduleType);
    gModules[moduleType]->SetVisContainers(1);
  }

  // no pole
  //  gPole = create_tof_pole();

  //  position_side_tof_modules(1);  // keep order !!
  //  position_inner_tof_modules(2);
  position_inner_tof_modules(3);
  position_Dia(1);
  position_Star2(1);
  //  position_cer_modules(3);
  //  position_CERN(1);
  position_Buc(1);

  cout << "Outer Types " << Outer_Module_Types[0][0] << ", " << Outer_Module_Types[1][0]
       << ", col=1:  " << Outer_Module_Types[0][1] << ", " << Outer_Module_Types[1][1] << endl;
  cout << "Outer Number " << Outer_Module_Number[0][0] << ", " << Outer_Module_Number[1][0]
       << ", col=1:  " << Outer_Module_Number[0][1] << ", " << Outer_Module_Number[1][1] << endl;
  //  position_outer_tof_modules(4);
  // position_tof_poles(0);
  // position_tof_bars(0);

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->CheckOverlaps(0.001, "s");
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  tof->Export(FileNameSim);
  TFile* geoFile = new TFile(FileNameSim, "UPDATE");
  stand_combi_trans->Write();
  geoFile->Close();

  /*
  TFile* outfile1 = new TFile(FileNameSim,"RECREATE");
  top->Write();
  //gGeoMan->Write();
  outfile1->Close();
*/
  //tof->RemoveNode((TGeoNode*)tofstand);
  //top->AddNode(tof, 1, tof_rotation);
  //tof->ReplaceNode((TGeoNode*)tofstand, 0, stand_combi_trans);
  /*
  CbmTransport run;
  run.SetGeoFileName(FileNameGeo);
  run.LoadSetup("setup_mcbm_tof_2020");
  run.SetField(new CbmFieldConst());
*/
  //top->Export(FileNameGeo);

  TFile* outfile2 = new TFile(FileNameGeo, "RECREATE");
  gGeoMan->Write();
  outfile2->Close();

  dump_info_file();

  top->SetVisContainers(1);
  gGeoMan->SetVisLevel(5);
  top->Draw("ogl");
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

  // include check if all media are found

  geoBuild->createMedium(air);
  geoBuild->createMedium(aluminium);
  geoBuild->createMedium(RPCgas);
  geoBuild->createMedium(RPCgas_noact);
  geoBuild->createMedium(RPCglass);
  geoBuild->createMedium(carbon);
}

TGeoVolume* create_counter(Int_t modType)
{

  //glass
  Float_t gdx = Glass_X[modType];
  Float_t gdy = Glass_Y[modType];
  Float_t gdz = Glass_Z[modType];

  //gas gap
  Int_t nstrips = NumberOfReadoutStrips[modType];
  Int_t ngaps   = NumberOfGaps[modType];


  Float_t ggdx = GasGap_X[modType];
  Float_t ggdy = GasGap_Y[modType];
  Float_t ggdz = GasGap_Z[modType];
  Float_t gsdx = ggdx / float(nstrips);

  //single stack
  Float_t dzpos     = gdz + ggdz;
  Float_t startzpos = SingleStackStartPosition_Z[modType];

  // electronics
  //pcb dimensions
  Float_t dxe  = Electronics_X[modType];
  Float_t dye  = Electronics_Y[modType];
  Float_t dze  = Electronics_Z[modType];
  Float_t yele = (gdy + 0.1) / 2. + dye / 2.;

  // needed materials
  TGeoMedium* glassPlateVolMed  = gGeoMan->GetMedium(GlasMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);
  TGeoMedium* activeGasVolMed   = gGeoMan->GetMedium(ActivGasMedium);
  TGeoMedium* electronicsVolMed = gGeoMan->GetMedium(ElectronicsMedium);

  // Single glass plate
  TGeoBBox* glass_plate       = new TGeoBBox("", gdx / 2., gdy / 2., gdz / 2.);
  TGeoVolume* glass_plate_vol = new TGeoVolume("tof_glass", glass_plate, glassPlateVolMed);
  glass_plate_vol->SetLineColor(kMagenta);  // set line color for the glass plate
  glass_plate_vol->SetTransparency(20);     // set transparency for the TOF
  TGeoTranslation* glass_plate_trans = new TGeoTranslation("", 0., 0., 0.);

  // Single gas gap
  TGeoBBox* gas_gap = new TGeoBBox("", ggdx / 2., ggdy / 2., ggdz / 2.);
  //TGeoVolume* gas_gap_vol =
  //new TGeoVolume("tof_gas_gap", gas_gap, noActiveGasVolMed);
  TGeoVolume* gas_gap_vol = new TGeoVolume("tof_gas_active", gas_gap, activeGasVolMed);
  gas_gap_vol->Divide("Strip", 1, nstrips, -ggdx / 2., 0);

  gas_gap_vol->SetLineColor(kRed);   // set line color for the gas gap
  gas_gap_vol->SetTransparency(70);  // set transparency for the TOF
  TGeoTranslation* gas_gap_trans = new TGeoTranslation("", 0., 0., (gdz + ggdz) / 2.);


  // Single subdivided active gas gap
  /*
    TGeoBBox* gas_active = new TGeoBBox("", gsdx/2., ggdy/2., ggdz/2.);
    TGeoVolume* gas_active_vol = 
    new TGeoVolume("tof_gas_active", gas_active, activeGasVolMed);
  gas_active_vol->SetLineColor(kBlack); // set line color for the gas gap
  gas_active_vol->SetTransparency(70); // set transparency for the TOF
  */

  // Add glass plate, inactive gas gap and active gas gaps to a single stack
  TGeoVolume* single_stack = new TGeoVolumeAssembly("single_stack");
  single_stack->AddNode(glass_plate_vol, 0, glass_plate_trans);
  single_stack->AddNode(gas_gap_vol, 0, gas_gap_trans);

  /*
  for (Int_t l=0; l<nstrips; l++){
    TGeoTranslation* gas_active_trans 
      = new TGeoTranslation("", -ggdx/2+(l+0.5)*gsdx, 0., 0.);
    gas_gap_vol->AddNode(gas_active_vol, l, gas_active_trans);
    //    single_stack->AddNode(gas_active_vol, l, gas_active_trans);
  }
  */

  // Add 8 single stacks + one glass plate at the e09.750nd to a multi stack
  TGeoVolume* multi_stack = new TGeoVolumeAssembly("multi_stack");
  Int_t l;
  for (l = 0; l < ngaps; l++) {
    TGeoTranslation* single_stack_trans = new TGeoTranslation("", 0., 0., startzpos + l * dzpos);
    multi_stack->AddNode(single_stack, l, single_stack_trans);
  }
  TGeoTranslation* single_glass_back_trans = new TGeoTranslation("", 0., 0., startzpos + ngaps * dzpos);
  multi_stack->AddNode(glass_plate_vol, l, single_glass_back_trans);

  // Add electronics above and below the glass stack to build a complete counter
  TGeoVolume* counter                = new TGeoVolumeAssembly("counter");
  TGeoTranslation* multi_stack_trans = new TGeoTranslation("", 0., 0., 0.);
  counter->AddNode(multi_stack, l, multi_stack_trans);

  TGeoBBox* pcb       = new TGeoBBox("", dxe / 2., dye / 2., dze / 2.);
  TGeoVolume* pcb_vol = new TGeoVolume("pcb", pcb, electronicsVolMed);
  pcb_vol->SetLineColor(kCyan);  // set line color for the gas gap
  pcb_vol->SetTransparency(10);  // set transparency for the TOF
  for (Int_t l = 0; l < 2; l++) {
    yele *= -1.;
    TGeoTranslation* pcb_trans = new TGeoTranslation("", 0., yele, 0.);
    counter->AddNode(pcb_vol, l, pcb_trans);
  }

  return counter;
}

TGeoVolume* create_new_counter(Int_t modType)
{

  //glass
  Float_t gdx = Glass_X[modType];
  Float_t gdy = Glass_Y[modType];
  Float_t gdz = Glass_Z[modType];

  //gas gap
  Int_t nstrips = NumberOfReadoutStrips[modType];
  Int_t ngaps   = NumberOfGaps[modType];


  Float_t ggdx = GasGap_X[modType];
  Float_t ggdy = GasGap_Y[modType];
  Float_t ggdz = GasGap_Z[modType];
  Float_t gsdx = ggdx / (Float_t)(nstrips);

  // electronics
  //pcb dimensions
  Float_t dxe  = Electronics_X[modType];
  Float_t dye  = Electronics_Y[modType];
  Float_t dze  = Electronics_Z[modType];
  Float_t yele = gdy / 2. + dye / 2.;

  // counter size (calculate from glas, gap and electronics sizes)
  Float_t cdx = TMath::Max(gdx, ggdx);
  cdx         = TMath::Max(cdx, dxe) + 0.2;
  Float_t cdy = TMath::Max(gdy, ggdy) + 2 * dye + 0.2;
  Float_t cdz = ngaps * ggdz + (ngaps + 1) * gdz + 0.2;  // ngaps * (gdz+ggdz) + gdz + 0.2; // ok

  //calculate thickness and first position in counter of single stack
  Float_t dzpos         = gdz + ggdz;
  Float_t startzposglas = -ngaps * (gdz + ggdz) / 2.;  // -cdz/2.+0.1+gdz/2.; // ok  // (-cdz+gdz)/2.; // not ok
  Float_t startzposgas  = startzposglas + gdz / 2. + ggdz / 2.;  // -cdz/2.+0.1+gdz   +ggdz/2.;  // ok


  // needed materials
  TGeoMedium* glassPlateVolMed  = gGeoMan->GetMedium(GlasMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);
  TGeoMedium* activeGasVolMed   = gGeoMan->GetMedium(ActivGasMedium);
  TGeoMedium* electronicsVolMed = gGeoMan->GetMedium(ElectronicsMedium);


  // define counter volume
  TGeoBBox* counter_box = new TGeoBBox("", cdx / 2., cdy / 2., cdz / 2.);
  TGeoVolume* counter   = new TGeoVolume("counter", counter_box, noActiveGasVolMed);
  counter->SetLineColor(kRed);   // set line color for the counter
  counter->SetTransparency(70);  // set transparency for the TOF

  // define single glass plate volume
  TGeoBBox* glass_plate       = new TGeoBBox("", gdx / 2., gdy / 2., gdz / 2.);
  TGeoVolume* glass_plate_vol = new TGeoVolume("tof_glass", glass_plate, glassPlateVolMed);
  glass_plate_vol->SetLineColor(kMagenta);  // set line color for the glass plate
  glass_plate_vol->SetTransparency(20);     // set transparency for the TOF
  // define single gas gap volume
  TGeoBBox* gas_gap       = new TGeoBBox("", ggdx / 2., ggdy / 2., ggdz / 2.);
  TGeoVolume* gas_gap_vol = new TGeoVolume("Gap", gas_gap, activeGasVolMed);
  gas_gap_vol->Divide("Cell", 1, nstrips, -ggdx / 2., 0);
  gas_gap_vol->SetLineColor(kRed);   // set line color for the gas gap
  gas_gap_vol->SetTransparency(99);  // set transparency for the TOF

  // place 8 gas gaps and 9 glas plates in the counter
  for (Int_t igap = 0; igap <= ngaps; igap++) {
    // place (ngaps+1) glass plates
    Float_t zpos_glas                  = startzposglas + igap * dzpos;
    TGeoTranslation* glass_plate_trans = new TGeoTranslation("", 0., 0., zpos_glas);
    counter->AddNode(glass_plate_vol, igap, glass_plate_trans);
    // place ngaps gas gaps
    if (igap < ngaps) {
      Float_t zpos_gas               = startzposgas + igap * dzpos;
      TGeoTranslation* gas_gap_trans = new TGeoTranslation("", 0., 0., zpos_gas);
      counter->AddNode(gas_gap_vol, igap, gas_gap_trans);
    }
    //    cout <<"Zpos(Glas): "<< zpos_glas << endl;
    //    cout <<"Zpos(Gas): "<< zpos_gas << endl;
  }

  // create and place the electronics above and below the glas stack
  TGeoBBox* pcb       = new TGeoBBox("", dxe / 2., dye / 2., dze / 2.);
  TGeoVolume* pcb_vol = new TGeoVolume("pcb", pcb, electronicsVolMed);
  pcb_vol->SetLineColor(kYellow);  // kCyan); // set line color for electronics
  pcb_vol->SetTransparency(10);    // set transparency for the TOF
  for (Int_t l = 0; l < 2; l++) {
    yele *= -1.;
    TGeoTranslation* pcb_trans = new TGeoTranslation("", 0., yele, 0.);
    counter->AddNode(pcb_vol, l, pcb_trans);
  }


  return counter;
}

TGeoVolume* create_tof_module(Int_t modType)
{
  Int_t cType         = CounterTypeInModule[modType];
  Float_t dx          = Module_Size_X[modType];
  Float_t dy          = Module_Size_Y[modType];
  Float_t dz          = Module_Size_Z[modType];
  Float_t width_aluxl = Module_Thick_Alu_X_left;
  Float_t width_aluxr = Module_Thick_Alu_X_right;
  Float_t width_aluy  = Module_Thick_Alu_Y;
  Float_t width_aluz  = Module_Thick_Alu_Z;

  Float_t shift_gas_box = (Module_Thick_Alu_X_right - Module_Thick_Alu_X_left) / 2;

  Float_t dxpos     = CounterXDistance[modType];
  Float_t startxpos = CounterXStartPosition[modType];
  Float_t dzoff     = CounterZDistance[modType];
  Float_t rotangle  = CounterRotationAngle[modType];

  TGeoMedium* boxVolMed         = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);

  TString moduleName = Form("module_%d", modType);
  TGeoVolume* module = new TGeoVolumeAssembly(moduleName);

  TGeoBBox* alu_box       = new TGeoBBox("", dx / 2., dy / 2., dz / 2.);
  TGeoVolume* alu_box_vol = new TGeoVolume("alu_box", alu_box, boxVolMed);
  alu_box_vol->SetLineColor(kGreen);  // set line color for the alu box
  alu_box_vol->SetTransparency(20);   // set transparency for the TOF
  TGeoTranslation* alu_box_trans = new TGeoTranslation("", 0., 0., 0.);
  module->AddNode(alu_box_vol, 0, alu_box_trans);

  TGeoBBox* gas_box =
    new TGeoBBox("", (dx - (width_aluxl + width_aluxr)) / 2., (dy - 2 * width_aluy) / 2., (dz - 2 * width_aluz) / 2.);
  TGeoVolume* gas_box_vol = new TGeoVolume("gas_box", gas_box, noActiveGasVolMed);
  gas_box_vol->SetLineColor(kYellow);  // set line color for the gas box
  gas_box_vol->SetTransparency(70);    // set transparency for the TOF
  TGeoTranslation* gas_box_trans = new TGeoTranslation("", shift_gas_box, 0., 0.);
  alu_box_vol->AddNode(gas_box_vol, 0, gas_box_trans);

  for (Int_t j = 0; j < 5; j++) {  //loop over counters (modules)
    Float_t zpos;
    if (0 == modType) { zpos = dzoff *= -1; }
    else {
      zpos = 0.;
    }
    //cout << "counter z position " << zpos << endl;
    TGeoTranslation* counter_trans = new TGeoTranslation("", startxpos + j * dxpos, 0.0, zpos);

    TGeoRotation* counter_rot = new TGeoRotation();
    counter_rot->RotateY(rotangle);
    TGeoCombiTrans* counter_combi_trans = new TGeoCombiTrans(*counter_trans, *counter_rot);
    gas_box_vol->AddNode(gCounter[cType], j, counter_combi_trans);
  }

  return module;
}

TGeoVolume* create_new_tof_module(Int_t modType)
{
  Int_t cType         = CounterTypeInModule[modType];
  Float_t dx          = Module_Size_X[modType];
  Float_t dy          = Module_Size_Y[modType];
  Float_t dz          = Module_Size_Z[modType];
  Float_t width_aluxl = Module_Thick_Alu_X_left;
  Float_t width_aluxr = Module_Thick_Alu_X_right;
  Float_t width_aluy  = Module_Thick_Alu_Y;
  Float_t width_aluz  = Module_Thick_Alu_Z;

  Float_t shift_gas_box = (Module_Thick_Alu_X_right - Module_Thick_Alu_X_left) / 2;

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
                                                           //for (Int_t j=0; j< 1; j++){ //loop over counters (modules)
    Float_t xpos, ypos, zpos;
    if (0 == modType || 3 == modType || 4 == modType || 5 == modType) { zpos = dzoff *= -1; }
    else {
      zpos = CounterZStartPosition[modType] + j * dzoff;
    }
    //cout << "counter z position " << zpos << endl;
    xpos = startxpos + j * dxpos;
    ypos = startypos + j * dypos;

    TGeoTranslation* counter_trans = new TGeoTranslation("", xpos, ypos, zpos);

    TGeoRotation* counter_rot = new TGeoRotation();
    counter_rot->RotateY(rotangle);
    TGeoCombiTrans* counter_combi_trans = new TGeoCombiTrans(*counter_trans, *counter_rot);
    gas_box_vol->AddNode(gCounter[cType], j, counter_combi_trans);
  }

  return module;
}


TGeoVolume* create_tof_pole()
{
  // needed materials
  TGeoMedium* boxVolMed = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* airVolMed = gGeoMan->GetMedium(KeepingVolumeMedium);

  Float_t dx         = Pole_Size_X;
  Float_t dy         = Pole_Size_Y;
  Float_t dz         = Pole_Size_Z;
  Float_t width_alux = Pole_Thick_X;
  Float_t width_aluy = Pole_Thick_Y;
  Float_t width_aluz = Pole_Thick_Z;

  TGeoVolume* pole         = new TGeoVolumeAssembly("Pole");
  TGeoBBox* pole_alu_box   = new TGeoBBox("", dx / 2., dy / 2., dz / 2.);
  TGeoVolume* pole_alu_vol = new TGeoVolume("pole_alu", pole_alu_box, boxVolMed);
  pole_alu_vol->SetLineColor(kGreen);  // set line color for the alu box
  pole_alu_vol->SetTransparency(20);   // set transparency for the TOF
  TGeoTranslation* pole_alu_trans = new TGeoTranslation("", 0., 0., 0.);
  pole->AddNode(pole_alu_vol, 0, pole_alu_trans);

  Float_t air_dx = dx / 2. - width_alux;
  Float_t air_dy = dy / 2. - width_aluy;
  Float_t air_dz = dz / 2. - width_aluz;

  //  cout << "My pole." << endl;
  if (air_dx <= 0.) cout << "ERROR - No air volume in pole X, size: " << air_dx << endl;
  if (air_dy <= 0.) cout << "ERROR - No air volume in pole Y, size: " << air_dy << endl;
  if (air_dz <= 0.) cout << "ERROR - No air volume in pole Z, size: " << air_dz << endl;

  if ((air_dx > 0.) && (air_dy > 0.) && (air_dz > 0.))  // crate air volume only, if larger than zero
  {
    TGeoBBox* pole_air_box = new TGeoBBox("", air_dx, air_dy, air_dz);
    //  TGeoBBox* pole_air_box = new TGeoBBox("", dx/2.-width_alux, dy/2.-width_aluy, dz/2.-width_aluz);
    TGeoVolume* pole_air_vol = new TGeoVolume("pole_air", pole_air_box, airVolMed);
    pole_air_vol->SetLineColor(kYellow);  // set line color for the alu box
    pole_air_vol->SetTransparency(70);    // set transparency for the TOF
    TGeoTranslation* pole_air_trans = new TGeoTranslation("", 0., 0., 0.);
    pole_alu_vol->AddNode(pole_air_vol, 0, pole_air_trans);
  }
  else
    cout << "Skipping pole_air_vol, no thickness: " << air_dx << " " << air_dy << " " << air_dz << endl;

  return pole;
}

TGeoVolume* create_tof_bar(Float_t dx, Float_t dy, Float_t dz)
{
  // needed materials
  TGeoMedium* boxVolMed = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* airVolMed = gGeoMan->GetMedium(KeepingVolumeMedium);

  Float_t width_alux = Pole_Thick_X;
  Float_t width_aluy = Pole_Thick_Y;
  Float_t width_aluz = Pole_Thick_Z;

  TGeoVolume* bar         = new TGeoVolumeAssembly("Bar");
  TGeoBBox* bar_alu_box   = new TGeoBBox("", dx / 2., dy / 2., dz / 2.);
  TGeoVolume* bar_alu_vol = new TGeoVolume("bar_alu", bar_alu_box, boxVolMed);
  bar_alu_vol->SetLineColor(kGreen);  // set line color for the alu box
  bar_alu_vol->SetTransparency(20);   // set transparency for the TOF
  TGeoTranslation* bar_alu_trans = new TGeoTranslation("", 0., 0., 0.);
  bar->AddNode(bar_alu_vol, 0, bar_alu_trans);

  TGeoBBox* bar_air_box   = new TGeoBBox("", dx / 2. - width_alux, dy / 2. - width_aluy, dz / 2. - width_aluz);
  TGeoVolume* bar_air_vol = new TGeoVolume("bar_air", bar_air_box, airVolMed);
  bar_air_vol->SetLineColor(kYellow);  // set line color for the alu box
  bar_air_vol->SetTransparency(70);    // set transparency for the TOF
  TGeoTranslation* bar_air_trans = new TGeoTranslation("", 0., 0., 0.);
  bar_alu_vol->AddNode(bar_air_vol, 0, bar_air_trans);

  return bar;
}

void position_tof_poles(Int_t modType)
{

  TGeoTranslation* pole_trans = NULL;

  Int_t numPoles = 0;
  for (Int_t i = 0; i < NumberOfPoles; i++) {
    if (i < 2) {
      pole_trans = new TGeoTranslation("", -Pole_Offset + 2.0, 0., Pole_ZPos[i]);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gPole, numPoles, pole_trans);
      numPoles++;
    }
    else {
      Float_t xPos = Pole_Offset + Pole_Size_X / 2. + Pole_Col[i] * DxColl;
      Float_t zPos = Pole_ZPos[i];
      pole_trans   = new TGeoTranslation("", xPos, 0., zPos);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gPole, numPoles, pole_trans);
      numPoles++;

      pole_trans = new TGeoTranslation("", -xPos, 0., zPos);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gPole, numPoles, pole_trans);
      numPoles++;
    }
    cout << " Position Pole " << numPoles << " at z=" << Pole_ZPos[i] << endl;
  }
}

void position_tof_bars(Int_t modType)
{

  TGeoTranslation* bar_trans = NULL;

  Int_t numBars = 0;
  Int_t i;
  Float_t xPos;
  Float_t yPos;
  Float_t zPos;

  for (i = 0; i < NumberOfBars; i++) {

    xPos = Bar_XPos[i];
    zPos = Bar_ZPos[i];
    yPos = Pole_Size_Y / 2. + Bar_Size_Y / 2.;

    bar_trans = new TGeoTranslation("", xPos, yPos, zPos);
    gGeoMan->GetVolume(geoVersionStand)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    bar_trans = new TGeoTranslation("", xPos, -yPos, zPos);
    gGeoMan->GetVolume(geoVersionStand)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    bar_trans = new TGeoTranslation("", -xPos, yPos, zPos);
    gGeoMan->GetVolume(geoVersionStand)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    bar_trans = new TGeoTranslation("", -xPos, -yPos, zPos);
    gGeoMan->GetVolume(geoVersionStand)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;
  }
  cout << " Position Bar " << numBars << " at z=" << Bar_ZPos[i] << endl;

  // horizontal frame bars
  i = NumberOfBars;
  NumberOfBars++;
  // no bar
  //   gBar[i]=create_tof_bar(2.*xPos+Pole_Size_X,Bar_Size_Y,Bar_Size_Y);

  zPos      = Pole_ZPos[0] + Pole_Size_Z / 2.;
  bar_trans = new TGeoTranslation("", 0., yPos, zPos);
  gGeoMan->GetVolume(geoVersionStand)->AddNode(gBar[i], numBars, bar_trans);
  numBars++;

  bar_trans = new TGeoTranslation("", 0., -yPos, zPos);
  gGeoMan->GetVolume(geoVersionStand)->AddNode(gBar[i], numBars, bar_trans);
  numBars++;
}

void position_inner_tof_modules(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;

  //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
  Float_t yPos = Inner_Module_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = Inner_Module_X_Offset;
  Float_t zPos = Wall_Z_Position;

  Pole_ZPos[NumberOfPoles] = zPos;
  Pole_Col[NumberOfPoles]  = 0;
  NumberOfPoles++;

  Float_t DzPos = 0.;
  for (Int_t j = 0; j < modNType; j++) {
    if (Module_Size_Z[j] > DzPos) { DzPos = Module_Size_Z[j]; }
  }
  Pole_ZPos[NumberOfPoles] = zPos + DzPos;
  Pole_Col[NumberOfPoles]  = 0;
  NumberOfPoles++;

  // for (Int_t j=0; j<modNType; j++){
  // for (Int_t j=1; j<modNType; j++){
  Int_t modType;
  Int_t modNum;
  for (Int_t j = 2; j < modNType; j++) {  // place only M4 type modules (modNType == 2)
                                          //DEDE
    modType = Inner_Module_Types[j];
    modNum  = 0;
    //  for(Int_t i=0; i<Inner_Module_Number[j]; i++) {
    //  for(Int_t i=0; i<1; i++) { // place 1x2 modules in the top and same in the bottom
    for (Int_t i = 0; i < 2; i++) {  // place 2x2 modules in the top and same in the bottom
      ii++;
      cout << "Inner ii " << ii << " Last " << Last_Size_Y << ", " << Last_Over_Y << endl;
      Float_t DeltaY = Module_Size_Y[modType] + Last_Size_Y - 2. * (Module_Over_Y[modType] + Last_Over_Y);
      //    DeltaY = 1.5;
      cout << "DeltaY " << DeltaY << endl;
      yPos += DeltaY;
      Last_Size_Y = Module_Size_Y[modType];
      Last_Over_Y = Module_Over_Y[modType];
      cout << "Position Inner Module " << i << " of " << Inner_Module_Number[j] << " Type " << modType
           << " at Y = " << yPos << " Ysize = " << Module_Size_Y[modType] << " DeltaY = " << DeltaY << endl;

      ///    module_trans = new TGeoTranslation("", xPos, yPos, zPos);
      ///    gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
      ///    modNum++;
      ///    module_trans = new TGeoTranslation("", xPos, -yPos, zPos);
      ///    gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
      ///    modNum++;
      //    //    if (ii>0) {
      //    if (ii>1) {
      //      module_trans
      //	= new TGeoTranslation("", xPos, yPos-DeltaY/2, zPos+Module_Size_Z[modType]);
      //      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
      //    modNum++;
      //      module_trans
      //	= new TGeoTranslation("", xPos, -(yPos-DeltaY/2), zPos+Module_Size_Z[modType]);
      //      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
      //    modNum++;
      //    }
    }
  }
  // module_trans = new TGeoTranslation("", xPos, -49-3, zPos);

  // Mar2019 setup
  const Int_t NModules           = 5;
  xPos                           = 0.;
  yPos                           = 0.;
  zPos                           = TOF_Z_Front;
  const Double_t ModDx[NModules] = {0., 0., 0., 49.8, 49.8};
  //const Double_t ModDx[NModules]= { 1.5,    0., -1.5, 49.8, 55.8};
  const Double_t ModDy[NModules]     = {0., 0., 0., 0., 0.};
  const Double_t ModDz[NModules]     = {0., 16.5, 34., 0., 34.};
  const Double_t ModAng[NModules]    = {-90., -90., -90., -90., -90.0};
  TGeoRotation* module_rot           = NULL;
  TGeoCombiTrans* module_combi_trans = NULL;

  for (Int_t iMod = 0; iMod < NModules; iMod++) {
    module_trans = new TGeoTranslation("", xPos + ModDx[iMod], yPos + ModDy[iMod], zPos + ModDz[iMod]);
    module_rot   = new TGeoRotation();
    module_rot->RotateZ(ModAng[iMod]);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }


  /*
 module_trans = new TGeoTranslation("", xPos, 0, zPos+16.5);
 gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
 modNum++;
 
 // module_trans = new TGeoTranslation("", xPos, 49+3, zPos);
 module_trans = new TGeoTranslation("", xPos, 0, zPos+16.5+17.5);
 gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
 modNum++;

 // module_trans = new TGeoTranslation("", xPos,-26, zPos+Module_Size_Z[modType]);
 module_trans = new TGeoTranslation("", xPos, -49.8, zPos);
 gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
 modNum++;

 // module_trans = new TGeoTranslation("", xPos, 26, zPos+Module_Size_Z[modType]);
 module_trans = new TGeoTranslation("", xPos, -49.8, zPos+16.5);
 gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
 modNum++;
 */
}


void position_Dia(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(Dia_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
  Float_t yPos = Dia_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = Dia_X_Offset;
  Float_t zPos = Dia_Z_Position;

  Int_t modNum = 0;
  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = Dia_Types[j];
    for (Int_t i = 0; i < Dia_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
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

  Float_t yPos = Star2_First_Y_Position;
  Float_t zPos = Star2_First_Z_Position;
  Int_t ii     = 0;

  Int_t modNum = 0;
  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = Star2_Types[j];
    Float_t xPos  = Star2_X_Offset[j];
    for (Int_t i = 0; i < Star2_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
      yPos += Star2_Delta_Y_Position;
      zPos += Star2_Delta_Z_Position;
    }
  }
}

void position_Buc(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(Buc_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  Float_t yPos = Buc_First_Y_Position;
  Float_t zPos = Buc_First_Z_Position;
  Int_t ii     = 0;

  Int_t modNum = 0;
  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = Buc_Types[j];
    Float_t xPos  = Buc_X_Offset[j];
    for (Int_t i = 0; i < Buc_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
      yPos += Buc_Delta_Y_Position;
      zPos += Buc_Delta_Z_Position;
    }
  }
}

void position_cer_modules(Int_t modNType)
{
  Int_t ii     = 0;
  Int_t modNum = 0;
  for (Int_t j = 1; j < modNType; j++) {
    Int_t modType                 = Cer_Types[j];
    Float_t xPos                  = Cer_X_Position[j];
    Float_t yPos                  = Cer_Y_Position[j];
    Float_t zPos                  = Cer_Z_Position[j];
    TGeoTranslation* module_trans = NULL;
    TGeoRotation* module_rot      = new TGeoRotation(Form("Cer%d", j), Cer_rotate_Z[j], -MeanTheta, 0.);
    // module_rot->RotateZ(Cer_rotate_Z[j]);
    TGeoCombiTrans* module_combi_trans = NULL;

    for (Int_t i = 0; i < Cer_Number[j]; i++) {
      ii++;
      cout << "Position Ceramic Module " << i << " of " << Cer_Number[j] << " Type " << modType << " at X = " << xPos
           << ", Y = " << yPos << ", Z = " << zPos << endl;
      // Front staggered module (Top if pair), top
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
      //    modNum++;
    }
  }
}

void position_CERN(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(CERN_rotate_Z);
  TGeoCombiTrans* module_combi_trans = NULL;

  //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
  Float_t yPos = CERN_First_Y_Position;
  Int_t ii     = 0;
  Float_t xPos = CERN_X_Offset;
  Float_t zPos = CERN_Z_Position;

  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = CERN_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < CERN_Number[j]; i++) {
      ii++;
      module_trans       = new TGeoTranslation("", xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;
    }
  }
}

void position_side_tof_modules(Int_t modNType)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(180.);
  TGeoCombiTrans* module_combi_trans = NULL;

  //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
  Float_t yPos = 0.;  //Inner_Module_First_Y_Position;
  Int_t ii     = 0;
  for (Int_t j = 0; j < modNType; j++) {
    Int_t modType = InnerSide_Module_Types[j];
    Int_t modNum  = 0;
    for (Int_t i = 0; i < InnerSide_Module_Number[j]; i++) {
      ii++;
      cout << "InnerSide ii " << ii << " Last " << Last_Size_Y << "," << Last_Over_Y << endl;
      Float_t DeltaY = Module_Size_Y[modType] + Last_Size_Y - 2. * (Module_Over_Y[modType] + Last_Over_Y);
      if (ii > 1) { yPos += DeltaY; }
      Last_Size_Y  = Module_Size_Y[modType];
      Last_Over_Y  = Module_Over_Y[modType];
      Float_t xPos = InnerSide_Module_X_Offset;
      Float_t zPos = Wall_Z_Position;
      cout << "Position InnerSide Module " << i << " of " << InnerSide_Module_Number[j] << " Type " << modType
           << " at Y = " << yPos << " Ysize = " << Module_Size_Y[modType] << " DeltaY = " << DeltaY << endl;

      module_trans = new TGeoTranslation("", xPos, yPos, zPos);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
      modNum++;

      module_trans       = new TGeoTranslation("", -xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
      modNum++;

      if (ii > 1) {
        module_trans = new TGeoTranslation("", xPos, -yPos, zPos);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
        modNum++;

        module_trans       = new TGeoTranslation("", -xPos, -yPos, zPos);
        module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
        modNum++;

        module_trans = new TGeoTranslation("", xPos, yPos - DeltaY / 2, zPos + Module_Size_Z[modType]);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
        modNum++;

        module_trans       = new TGeoTranslation("", -xPos, yPos - DeltaY / 2, zPos + Module_Size_Z[modType]);
        module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
        modNum++;

        module_trans = new TGeoTranslation("", xPos, -(yPos - DeltaY / 2), zPos + Module_Size_Z[modType]);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_trans);
        modNum++;

        module_trans       = new TGeoTranslation("", -xPos, -(yPos - DeltaY / 2), zPos + Module_Size_Z[modType]);
        module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum, module_combi_trans);
        modNum++;
      }
    }
  }
}

void position_outer_tof_modules(Int_t nCol)  //modType, Int_t col1, Int_t col2)
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot      = new TGeoRotation();
  module_rot->RotateZ(180.);
  TGeoCombiTrans* module_combi_trans = NULL;

  //  Int_t numModules=(Int_t)( (Outer_Module_Last_Y_Position-Outer_Module_First_Y_Position)/Module_Size_Y[modType])+1;

  Int_t modNum[NofModuleTypes];
  for (Int_t k = 0; k < NofModuleTypes; k++) {
    modNum[k] = 0;
  }

  Float_t zPos = Wall_Z_Position;
  for (Int_t j = 0; j < nCol; j++) {
    Float_t xPos  = Outer_Module_X_Offset + ((j + 1) * DxColl);
    Last_Size_Y   = 0.;
    Last_Over_Y   = 0.;
    Float_t yPos  = 0.;
    Int_t ii      = 0;
    Float_t DzPos = 0.;
    for (Int_t k = 0; k < Outer_Module_NTypes; k++) {
      Int_t modType = Outer_Module_Types[k][j];
      if (Module_Size_Z[modType] > DzPos) {
        if (Outer_Module_Number[k][j] > 0) { DzPos = Module_Size_Z[modType]; }
      }
    }

    zPos -= 2. * DzPos;  //((j+1)*2*Module_Size_Z[modType]);

    Pole_ZPos[NumberOfPoles] = zPos;
    Pole_Col[NumberOfPoles]  = j + 1;
    NumberOfPoles++;
    Pole_ZPos[NumberOfPoles] = zPos + DzPos;
    Pole_Col[NumberOfPoles]  = j + 1;
    NumberOfPoles++;
    //if (j+1==nCol) {
    if (1) {
      Pole_ZPos[NumberOfPoles] = Pole_ZPos[0];
      Pole_Col[NumberOfPoles]  = j + 1;
      NumberOfPoles++;

      Bar_Size_Z             = Pole_ZPos[0] - zPos;
      gBar[NumberOfBars]     = create_tof_bar(Bar_Size_X, Bar_Size_Y, Bar_Size_Z);
      Bar_ZPos[NumberOfBars] = zPos + Bar_Size_Z / 2. - Pole_Size_Z / 2.;
      Bar_XPos[NumberOfBars] = xPos + Pole_Offset;
      NumberOfBars++;
    }

    for (Int_t k = 0; k < Outer_Module_NTypes; k++) {
      Int_t modType    = Outer_Module_Types[k][j];
      Int_t numModules = Outer_Module_Number[k][j];

      cout << " Outer: position " << numModules << " of type " << modType << " in col " << j << " at z = " << zPos
           << ", DzPos = " << DzPos << endl;
      for (Int_t i = 0; i < numModules; i++) {
        ii++;
        cout << "Outer ii " << ii << " Last " << Last_Size_Y << "," << Last_Over_Y << endl;
        Float_t DeltaY = Module_Size_Y[modType] + Last_Size_Y - 2. * (Module_Over_Y[modType] + Last_Over_Y);
        if (ii > 1) { yPos += DeltaY; }
        Last_Size_Y = Module_Size_Y[modType];
        Last_Over_Y = Module_Over_Y[modType];
        cout << "Position Outer Module " << i << " of " << Outer_Module_Number[k][j] << " Type " << modType << "(#"
             << modNum[modType] << ") "
             << " at Y = " << yPos << " Ysize = " << Module_Size_Y[modType] << " DeltaY = " << DeltaY << endl;

        module_trans = new TGeoTranslation("", xPos, yPos, zPos);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_trans);
        modNum[modType]++;

        module_trans       = new TGeoTranslation("", -xPos, yPos, zPos);
        module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
        gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
        modNum[modType]++;

        if (ii > 1) {
          module_trans = new TGeoTranslation("", xPos, -yPos, zPos);
          gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_trans);
          modNum[modType]++;
          module_trans       = new TGeoTranslation("", -xPos, -yPos, zPos);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
          modNum[modType]++;

          // second layer
          module_trans = new TGeoTranslation("", xPos, yPos - DeltaY / 2., zPos + DzPos);
          gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_trans);
          modNum[modType]++;
          module_trans       = new TGeoTranslation("", -xPos, yPos - DeltaY / 2., zPos + DzPos);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
          modNum[modType]++;

          module_trans = new TGeoTranslation("", xPos, -(yPos - DeltaY / 2.), zPos + DzPos);
          gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_trans);
          modNum[modType]++;
          module_trans       = new TGeoTranslation("", -xPos, -(yPos - DeltaY / 2.), zPos + DzPos);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersionStand)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
          modNum[modType]++;
        }
      }
    }
  }
}


void dump_info_file()
{
  TDatime datetime;  // used to get timestamp

  printf("writing info file: %s\n", FileNameInfo.Data());

  FILE* ifile;
  ifile = fopen(FileNameInfo.Data(), "w");

  if (ifile == NULL) {
    printf("error opening %s\n", FileNameInfo.Data());
    exit(1);
  }

  fprintf(ifile, "#\n##   %s information file\n#\n\n", geoVersion.Data());

  fprintf(ifile, "# created %d\n\n", datetime.GetDate());

  fprintf(ifile, "# TOF setup\n");
  if (TOF_Z_Front == 450) fprintf(ifile, "SIS 100 hadron setup\n");
  if (TOF_Z_Front == 600) fprintf(ifile, "SIS 100 electron\n");
  if (TOF_Z_Front == 650) fprintf(ifile, "SIS 100 muon\n");
  if (TOF_Z_Front == 880) fprintf(ifile, "SIS 300 electron\n");
  if (TOF_Z_Front == 1020) fprintf(ifile, "SIS 300 muon\n");
  fprintf(ifile, "\n");

  const Float_t TOF_Z_Back = Wall_Z_Position + 1.5 * Module_Size_Z[0];  // back of TOF wall

  fprintf(ifile, "# envelope\n");
  // Show extension of TRD
  fprintf(ifile, "%7.2f cm   start of TOF (z)\n", TOF_Z_Front);
  fprintf(ifile, "%7.2f cm   end   of TOF (z)\n", TOF_Z_Back);
  fprintf(ifile, "\n");

  // Layer thickness
  fprintf(ifile, "# central tower position\n");
  fprintf(ifile, "%7.2f cm   center of staggered, front RPC cell at x=0\n", Wall_Z_Position);
  fprintf(ifile, "\n");

  fclose(ifile);
}
