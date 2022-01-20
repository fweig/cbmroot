/* Copyright (C) 2021 Physikalisches Institut, Universitaet Heidelberg, Germany
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ingo Deppner, Eoin Clerkin [committer] */
///
/// \file Create_TOF_Geometry_v13_4x.C
/// \brief Generates TOF geometry in Root format.
///

// Changelog
// 2021-11-15 - ID - update design of  inner wall (current design, 300 counters, 2 MRPC types)
// 2020-02-23 - ID - implementation of Bucharest wall (current design), possibility to choose between simple, single stack and double stack MRPCs
// 2017-10-18 - PAL- Fix the overlaps in the support structure => v17c
// 2016-07-18 - DE - patch double free or corruption with poleshort: same TGeoVolume name was used in pole
// 2015-11-09 - PAL- Change naming convention to follow the more meaningfull one used in trd: YYv_ss
//                   with YY = year, v = version (a, b, ...) and ss = setup (1h for SIS100 hadron, ...)
//                   => Prepare tof_v16a_1h to tof_v16a_3m
// 2015-11-09 - PAL- Modify to easily prepare tof_v14_0a to tof_v14_0e on model of 13_5a to 13_5e
// 2014-06-30 - NH - prepare tof_v14_0  geometry - SIS 300 hadron  : TOF_Z_Front =  880 cm //Bucharest
// 2014-06-27 - NH - prepare tof_v13_6b geometry - SIS 300 hadron  : TOF_Z_Front =  880 cm //external input
// 2013-10-16 - DE - prepare tof_v13_5a geometry - SIS 100 hadron  : TOF_Z_Front =  450 cm
// 2013-10-16 - DE - prepare tof_v13_5b geometry - SIS 100 electron: TOF_Z_Front =  600 cm
// 2013-10-16 - DE - prepare tof_v13_5c geometry - SIS 100 muon    : TOF_Z_Front =  650 cm
// 2013-10-16 - DE - prepare tof_v13_5d geometry - SIS 300 electron: TOF_Z_Front =  880 cm
// 2013-10-16 - DE - prepare tof_v13_5e geometry - SIS 300 muon    : TOF_Z_Front = 1020 cm
// 2013-10-16 - DE - patch pole_alu bug - skip 0 thickness air volume in pole
// 2013-09-04 - DE - prepare tof_v13_4a geometry - SIS 100 hadron  : TOF_Z_Front =  450 cm
// 2013-09-04 - DE - prepare tof_v13_4b geometry - SIS 100 electron: TOF_Z_Front =  600 cm
// 2013-09-04 - DE - prepare tof_v13_4c geometry - SIS 100 muon    : TOF_Z_Front =  650 cm
// 2013-09-04 - DE - prepare tof_v13_4d geometry - SIS 300 electron: TOF_Z_Front =  880 cm
// 2013-09-04 - DE - prepare tof_v13_4e geometry - SIS 300 muon    : TOF_Z_Front = 1020 cm
// 2013-09-04 - DE - dump z-positions to .geo.info file
// 2013-09-04 - DE - define front z-position of TOF wall (TOF_Z_Front)
// 2013-09-04 - DE - fix arrangement of glass plates in RPC cells

// in root all sizes are given in cm
// read positions of modules from dat - file

#include "TFile.h"
#include "TGeoCompositeShape.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGeoPgon.h"
#include "TGeoVolume.h"
#include "TList.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"

#include <iostream>
#include <sstream>

const Bool_t IncludeSupports = true;  // false;   // true, if support structure is included in geometry

// Name of geometry version and output file
//const TString geoVersion = "tof_v21at_1h";     // SIS 100 hadron, 4.5 m
//const TString geoVersion = "tof_v21at_1e";     // SIS 100 electron, 6 m
//const TString geoVersion = "tof_v21at_1m";     // SIS 100 muon, 6.8 m
const TString geoVersion = "tof_v21a";  // SIS 300 electron, 8.8 m
//const TString geoVersion = "tof_v21at_3m";     // SIS 300 muon, 10 m
const TString FileNameSim  = geoVersion + ".geo.root";
const TString FileNameGeo  = geoVersion + "_geo.root";
const TString FileNameInfo = geoVersion + ".geo.info";

// TOF_Z_Front corresponds to front cover of outer super module towers
const Float_t TOF_Z_Front = ("tof_v21at_1h" == geoVersion ? 450 :               // SIS 100 hadron
                               ("tof_v21at_1e" == geoVersion ? 600 :            // SIS 100 electron
                                  ("tof_v21at_1m" == geoVersion ? 680 :         // SIS 100 muon
                                     ("tof_v21at_3e" == geoVersion ? 880 :      // SIS 300 electron
                                        ("tof_v21at_3m" == geoVersion ? 1020 :  // SIS 300 muon
                                           600                                  // Set default to SIS 100 electron
                                         )))));

const TString GeometryType = "20b";

const TString KeepingVolumeMedium = "air";
const TString BoxVolumeMedium     = "aluminium";
const TString PoleVolumeMedium    = "tof_pole_aluminium";
const TString NoActivGasMedium    = "RPCgas_noact";
const TString ActivGasMedium      = "RPCgas";
const TString GlasMedium          = "RPCglass";
const TString ElectronicsMedium   = "carbon";

const Int_t NumberOfDifferentCounterTypes = 6;

const Float_t PCB_X[NumberOfDifferentCounterTypes] = {32., 32., 32., 30., 30., 30.};
const Float_t PCB_Y[NumberOfDifferentCounterTypes] = {
  52., 26.9, 26.9, 20., 10., 6,
};
const Float_t PCB_Z[NumberOfDifferentCounterTypes] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1};

const Float_t Glass_X[NumberOfDifferentCounterTypes] = {32., 32., 32., 30., 30., 30.};
const Float_t Glass_Y[NumberOfDifferentCounterTypes] = {
  52., 26.9, 26.9, 20., 10., 6,
};
const Float_t Glass_Z[NumberOfDifferentCounterTypes] = {0.028, 0.028, 0.07, 0.07, 0.07, 0.07};

const Float_t GasGap_X[NumberOfDifferentCounterTypes] = {32., 32., 32., 30., 30., 30.};
const Float_t GasGap_Y[NumberOfDifferentCounterTypes] = {
  52., 26.9, 26.9, 20., 10., 6,
};
const Float_t GasGap_Z[NumberOfDifferentCounterTypes] = {0.023, 0.023, 0.025, 0.02, 0.02, 0.02};

const Int_t NumberOfGaps[NumberOfDifferentCounterTypes]          = {10, 10, 8, 10, 10, 10};
const Int_t NumberOfReadoutStrips[NumberOfDifferentCounterTypes] = {32, 32, 32, 32, 32, 32};

const Float_t Electronics_X[NumberOfDifferentCounterTypes] = {34.0, 34.0, 34.0, 32.0, 32.0, 32.0};
const Float_t Electronics_Y[NumberOfDifferentCounterTypes] = {5.0, 5.0, 5.0, 0.5, 0.5, 0.5};
const Float_t Electronics_Z[NumberOfDifferentCounterTypes] = {0.3, 0.3, 0.3, 0.3, 0.3, 0.3};

const Int_t NofModuleTypes = 15;
const Int_t MaxNofCounters = 60;
const Int_t MaxNofModules  = 230;

const Int_t NCounterInModule[NofModuleTypes]     = {5, 5, 5, 5, 5, 30, 24, 27, 18, 24, 30, 24, 27, 18, 24};
const Int_t NModulesOfModuleType[NofModuleTypes] = {62, 32, 8, 96, 16, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1};
Int_t iMod[NofModuleTypes]                       = {0};
//Int_t ActNofModuleTypes    = 2;
//Int_t NModules[NofModuleTypes] = {0};

Float_t xPosCou[NofModuleTypes][MaxNofCounters];
Float_t yPosCou[NofModuleTypes][MaxNofCounters];
Float_t zPosCou[NofModuleTypes][MaxNofCounters];
Int_t CouType[NofModuleTypes][MaxNofCounters];

Float_t xPosMod[MaxNofModules];
Float_t yPosMod[MaxNofModules];
Float_t zPosMod[MaxNofModules];
Int_t ModType[MaxNofModules];
//Float_t FlipMod[NofModuleTypes][MaxNofModules];

const Float_t Module_Size_X[NofModuleTypes] = {180.2, 180.2, 180.2, 180.2, 180.2, 210.5, 124.4, 98.3,
                                               69.3,  124.4, 210.5, 124.4, 98.3,  69.3,  124.4};
const Float_t Module_Size_Y[NofModuleTypes] = {74.,   49.,   49.,  49.,   49.,   73.8,  130.0, 128.7,
                                               128.7, 130.0, 73.8, 130.0, 128.7, 128.7, 130.0};
const Float_t Module_Size_Z[NofModuleTypes] = {11.2, 11.2, 11.2, 11.2, 11.2, 19.6, 19.6, 19.6,
                                               19.6, 19.6, 19.6, 19.6, 19.6, 19.6, 19.6};

// Placement of the counter inside the module
const Float_t CounterRotationAngle[NofModuleTypes] = {10., 10., 0., 10., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};

const Float_t Module_Thick_Alu_X_left  = 1.;
const Float_t Module_Thick_Alu_X_right = 0.1;
const Float_t Module_Thick_Alu_Y       = 0.1;
const Float_t Module_Thick_Alu_Z_front = 0.1;
const Float_t Module_Thick_Alu_Z_back  = 1.;

const Float_t shift_gas_box_x = (Module_Thick_Alu_X_right - Module_Thick_Alu_X_left) / 2;
const Float_t shift_gas_box_z = (Module_Thick_Alu_Z_back - Module_Thick_Alu_Z_front) / 2;

const Float_t Wall_Z_Position =
  TOF_Z_Front + 1000. - 884.;  // if you want to know where this numbers come from ask Ingo

// Pole (support structure)
const Int_t MaxNumberOfPoles = 200;
Float_t Pole_ZPos[MaxNumberOfPoles];
Float_t Pole_XPos[MaxNumberOfPoles];
Float_t Pole_Col[MaxNumberOfPoles];
Int_t NumberOfPoles = 0;

const Float_t Pole_Size_X      = 8.;
const Float_t Pole_Size_Y      = 1000.;
const Float_t PoleShort_Size_Y = 370.;
const Float_t Pole_Size_Z      = 2.;
const Float_t Pole_Thick_X     = 0.4;
const Float_t Pole_Thick_Y     = 0.4;
const Float_t Pole_Thick_Z     = 0.4;
const Float_t XLimInner        = 180.;


// Bars & frame (support structure)
const Float_t Frame_Size_X = 20.;
const Float_t Frame_Size_Y = 20.;
Float_t Bar_Size_Z         = 176.;
const Float_t Frame_XLen   = 1400;
const Float_t Frame_YLen   = Pole_Size_Y + 2. * Frame_Size_Y;
Float_t Frame_Pos_Z        = TOF_Z_Front + 88.;
const Float_t Bar_Size_X   = 30;
const Float_t Bar_Size_Y   = 20.;
Float_t Bar_Pos_Z;

const Int_t MaxNumberOfBars = 200;
Float_t Bar_ZPos[MaxNumberOfBars];
Float_t Bar_XPos[MaxNumberOfBars];
Int_t NumberOfBars = 0;

const Float_t ChamberOverlap = 40;
const Float_t DxColl         = 158.0;  //Module_Size_X-ChamberOverlap;
//const Float_t Pole_Offset=Module_Size_X/2.+Pole_Size_X/2.;
const Float_t Pole_Offset = 90.0 + Pole_Size_X / 2.;

// some global variables
TGeoManager* gGeoMan = NULL;           // Pointer to TGeoManager instance
TGeoVolume* gModules[NofModuleTypes];  // Global storage for module types
TGeoVolume* gCounter[NumberOfDifferentCounterTypes];
TGeoVolume* gPole;
TGeoVolume* gPoleShort;
TGeoVolume* gBar[MaxNumberOfBars];

Float_t Last_Size_Y = 0.;
Float_t Last_Over_Y = 0.;

// Forward declarations

void create_materials_from_media_file();
TGeoVolume* create_counter_simple(Int_t);
TGeoVolume* create_counter_doublestack(Int_t);
TGeoVolume* create_counter_singlestack(Int_t);
TGeoVolume* create_tof_module(Int_t);
TGeoVolume* create_tof_module_m(Int_t);
TGeoVolume* create_new_tof_module(Int_t);
TGeoVolume* create_new_tof_module_m(Int_t);
TGeoVolume* create_tof_pole();
TGeoVolume* create_tof_poleshort();
TGeoVolume* create_tof_bar();
void position_tof_poles(Int_t);
void position_tof_bars(Int_t);
void position_inner_tof_modules(Int_t);
void position_side_tof_modules(Int_t);
void position_outer_tof_modules(Int_t);
void position_tof_modules();
void position_tof_modules_m(Int_t, Int_t);
void dump_info_file();
void read_module_positions();
void read_counter_positions();

void Create_TOF_Geometry_v21a_FullWall()
{
  // Load the necessary FairRoot libraries
  //  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  //  basiclibs();
  //  gSystem->Load("libGeoBase");
  //  gSystem->Load("libParBase");
  //  gSystem->Load("libBase");

  // Printout what we are generating
  std::cout << "Generating geometry " << geoVersion << " at " << TOF_Z_Front << " cm from target." << std::endl;

  // read input Data
  read_counter_positions();
  read_module_positions();
  // Load needed material definition from media.geo file
  create_materials_from_media_file();

  // Get the GeoManager for later usage
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetVisLevel(7);  // 2 = super modules
  gGeoMan->SetVisOption(1);

  // Create the top volume
  /*
  TGeoBBox* topbox= new TGeoBBox("", 1000., 1000., 1000.);
  TGeoVolume* top = new TGeoVolume("top", topbox, gGeoMan->GetMedium("air"));
  gGeoMan->SetTopVolume(top);
  */

  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);

  TGeoVolume* tof = new TGeoVolumeAssembly(geoVersion);
  top->AddNode(tof, 1);

  for (Int_t counterType = 0; counterType < NumberOfDifferentCounterTypes; counterType++) {
    //gCounter[counterType] = create_counter_simple(counterType);
    gCounter[counterType] = create_counter_doublestack(counterType);
    //gCounter[counterType] = create_counter_singlestack(counterType);
  }

  for (Int_t moduleType = 0; moduleType < NofModuleTypes; moduleType++) {
    gModules[moduleType] = create_new_tof_module(moduleType);
    gModules[moduleType]->SetVisContainers(1);
  }

  gPole      = create_tof_pole();
  gPoleShort = create_tof_poleshort();

  position_tof_modules();

  if (IncludeSupports) position_tof_poles(0);
  if (IncludeSupports) position_tof_bars(0);

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.00001);
  gGeoMan->CheckOverlaps(0.00001, "s");
  gGeoMan->PrintOverlaps();
  gGeoMan->GetListOfOverlaps()->Print();
  gGeoMan->Test();

  tof->Export(FileNameSim);  // an alternative way of writing the tof volume

  TFile* outfile                 = new TFile(FileNameSim, "UPDATE");
  TGeoTranslation* tof_placement = new TGeoTranslation("tof_trans", 0., 0., 0.);
  tof_placement->Write();
  outfile->Close();

  outfile = new TFile(FileNameGeo, "RECREATE");
  gGeoMan->Write();
  outfile->Close();

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

  // Printout what we are generating
  std::cout << "Done generating geometry " << geoVersion << " at " << TOF_Z_Front << " cm from target." << std::endl;
}

void read_counter_positions()
{
  //TFile * fPosInput = new TFile( "TOF_10M.dat", "READ");
  for (Int_t modtype = 0; modtype < NofModuleTypes; modtype++) {
    TString moduleTypeName = Form("ModuleType%d_v21a.dat", modtype);
    cout << "load parameters from file " << moduleTypeName << endl;
    ifstream inFile;
    inFile.open(moduleTypeName);
    if (!inFile.is_open()) {
      cout << "<E> cannot open input file " << endl;
      return;
    }

    cout << "------------------------------" << endl;
    cout << "Reading content of " << moduleTypeName << endl;
    std::string strdummy;
    std::getline(inFile, strdummy);
    //cout<<strdummy<<endl;
    Int_t iNum;
    Int_t ictype;
    Float_t iX;
    Float_t iY;
    Float_t iZ;
    Int_t iCou = 0;
    while (std::getline(inFile, strdummy)) {
      //  std::getline(inFile,strdummy);
      //cout<<strdummy<<endl;
      stringstream ss;
      ss << strdummy;
      ss >> iNum >> ictype >> iX >> iY >> iZ;
      ss << strdummy;
      //cout<<iCou<< "    "<<iNum<<"    "<<ictype<<"   "<<iX<<"    "<<iY<<"    "<<iZ<<endl;
      CouType[modtype][iCou] = ictype;
      xPosCou[modtype][iCou] = iX;
      yPosCou[modtype][iCou] = iY;
      zPosCou[modtype][iCou] = iZ;
      iCou++;
    }
  }
}


void read_module_positions()
{
  //TFile * fPosInput = new TFile( "TOF_10M.dat", "READ");
  ifstream inFile;
  inFile.open("ModulePosition_10m_v21a.dat");
  if (!inFile.is_open()) {
    cout << "<E> cannot open input file " << endl;
    return;
  }

  cout << "------------------------------" << endl;
  cout << "Reading content of ModulePosition_10m_v21a.dat" << endl;
  std::string strdummy;
  std::getline(inFile, strdummy);
  //cout<<strdummy<<endl;
  Int_t iNum;
  Int_t iModT;
  Float_t iX;
  Float_t iY;
  Float_t iZ;
  //Int_t iModType=0;
  Int_t iMod = 0;
  //while( !inFile.eof()  )
  //for(Int_t iL=0; iL<2; iL++)
  while (std::getline(inFile, strdummy)) {
    //  std::getline(inFile,strdummy);
    //cout<<strdummy<<endl;
    stringstream ss;
    ss << strdummy;
    ss >> iNum >> iModT >> iX >> iY >> iZ;
    ss << strdummy;
    //  ss>>iNum>>iX>>iY>>iZ>>cType[0]>>cType[1];
    cout << iNum << "   " << iModT << "   " << iX << "   " << iY << "   " << iZ << endl;

    //cout<<" ModType "<<iModType<<endl;
    //cout<<" ModType "<<iModType<<", # "<<iMod<<endl;
    ModType[iMod] = iModT;
    xPosMod[iMod] = iX;
    yPosMod[iMod] = iY;
    zPosMod[iMod] = iZ;
    iMod++;
    /*
  if(cPos=='l'){
    FlipMod[iModType][iMod]=1.;
  }else{
    FlipMod[iModType][iMod]=0.;
  }
  //  if (iModType==1 && iMod==1) return;
 
    cout<<" ModType "<<iModType<<", Mod "<<iMod<<", x "<<xPosMod[iModType][iMod]<<", y "
      <<yPosMod[iModType][iMod]<<", z "<<zPosMod[iModType][iMod]<<endl;
  */
  }
  cout << "Data reading finished for " << endl;
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

  FairGeoMedium* air                = geoMedia->getMedium("air");
  FairGeoMedium* aluminium          = geoMedia->getMedium("aluminium");
  FairGeoMedium* tof_pole_aluminium = geoMedia->getMedium("tof_pole_aluminium");
  FairGeoMedium* RPCgas             = geoMedia->getMedium("RPCgas");
  FairGeoMedium* RPCgas_noact       = geoMedia->getMedium("RPCgas_noact");
  FairGeoMedium* RPCglass           = geoMedia->getMedium("RPCglass");
  FairGeoMedium* carbon             = geoMedia->getMedium("carbon");

  // include check if all media are found

  geoBuild->createMedium(air);
  geoBuild->createMedium(aluminium);
  geoBuild->createMedium(tof_pole_aluminium);
  geoBuild->createMedium(RPCgas);
  geoBuild->createMedium(RPCgas_noact);
  geoBuild->createMedium(RPCglass);
  geoBuild->createMedium(carbon);
}


TGeoVolume* create_counter_simple(Int_t countType)
{
  TGeoMedium* activeGasVolMed   = gGeoMan->GetMedium(ActivGasMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);
  //TGeoMedium* glassPlateVolMed   = gGeoMan->GetMedium(GlasMedium);
  //gas gap
  Int_t nstrips = NumberOfReadoutStrips[countType];

  Float_t ggdx = GasGap_X[countType];
  Float_t ggdy = GasGap_Y[countType];
  //Float_t ggdz=GasGap_Z[countType];
  Float_t ggdz = 1.;
  Float_t gsdx = ggdx / float(nstrips);


  TGeoBBox* counter_box = new TGeoBBox("", (ggdx + 0.2) / 2., (ggdy + 0.2) / 2., (ggdz + 0.2) / 2.);
  TGeoVolume* counter   = new TGeoVolume("counter", counter_box, noActiveGasVolMed);
  counter->SetLineColor(kCyan);  // set line color for the counter
  counter->SetTransparency(70);  // set transparency for the TOF
  // Single gas gap
  TGeoBBox* gas_gap       = new TGeoBBox("", ggdx / 2., ggdy / 2., ggdz / 2.);
  TGeoVolume* gas_gap_vol = new TGeoVolume("Gap", gas_gap, activeGasVolMed);
  gas_gap_vol->Divide("Cell", 1, nstrips, -ggdx / 2., 0);
  gas_gap_vol->SetLineColor(kRed);   // set line color for the gas gap
  gas_gap_vol->SetTransparency(70);  // set transparency for the TOF
  TGeoTranslation* gas_gap_trans = new TGeoTranslation("", 0., 0., 0.);

  counter->AddNode(gas_gap_vol, 0, gas_gap_trans);

  return counter;
}

TGeoVolume* create_counter_doublestack(Int_t countType)
{
  //glass
  Float_t pdx = PCB_X[countType];
  Float_t pdy = PCB_Y[countType];
  Float_t pdz = PCB_Z[countType];

  //glass
  Float_t gdx = Glass_X[countType];
  Float_t gdy = Glass_Y[countType];
  Float_t gdz = Glass_Z[countType];

  //gas gap
  Int_t nstrips = NumberOfReadoutStrips[countType];
  Int_t ngaps   = NumberOfGaps[countType];


  Float_t ggdx = GasGap_X[countType];
  Float_t ggdy = GasGap_Y[countType];
  Float_t ggdz = GasGap_Z[countType];
  Float_t gsdx = ggdx / float(nstrips);

  //single stack
  //Float_t dzpos=gdz+ggdz;
  // Float_t startzpos=SingleStackStartPosition_Z[modType];

  // electronics
  //pcb dimensions
  Float_t dxe  = Electronics_X[countType];
  Float_t dye  = Electronics_Y[countType];
  Float_t dze  = Electronics_Z[countType];
  Float_t yele = (gdy + 0.1) / 2. + dye / 2.;

  // needed materials
  TGeoMedium* glassPlateVolMed  = gGeoMan->GetMedium(GlasMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);
  TGeoMedium* activeGasVolMed   = gGeoMan->GetMedium(ActivGasMedium);
  TGeoMedium* electronicsVolMed = gGeoMan->GetMedium(ElectronicsMedium);

  // Single PCB
  TGeoBBox* pcb       = new TGeoBBox("", pdx / 2., pdy / 2., pdz / 2.);
  TGeoVolume* pcb_vol = new TGeoVolume("tof_pcb", pcb, electronicsVolMed);
  pcb_vol->SetLineColor(kGreen);  // set line color for the pcb
  pcb_vol->SetTransparency(20);   // set transparency for the TOF
  TGeoTranslation* pcb_trans0 = new TGeoTranslation("", 0., 0., 0.);

  // Single glass plate
  TGeoBBox* glass_plate       = new TGeoBBox("", gdx / 2., gdy / 2., gdz / 2.);
  TGeoVolume* glass_plate_vol = new TGeoVolume("tof_glass", glass_plate, glassPlateVolMed);
  glass_plate_vol->SetLineColor(kMagenta);  // set line color for the glass plate
  glass_plate_vol->SetTransparency(20);     // set transparency for the TOF
  //TGeoTranslation* glass_plate_trans = new TGeoTranslation("", 0., 0., 0.);

  // Single gas gap
  TGeoBBox* gas_gap       = new TGeoBBox("", ggdx / 2., ggdy / 2., ggdz / 2.);
  TGeoVolume* gas_gap_vol = new TGeoVolume("Gap", gas_gap, activeGasVolMed);
  gas_gap_vol->Divide("Cell", 1, nstrips, -ggdx / 2., 0);
  gas_gap_vol->SetLineColor(kRed);   // set line color for the gas gap
  gas_gap_vol->SetTransparency(70);  // set transparency for the TOF
  //TGeoTranslation* gas_gap_trans = new TGeoTranslation("", 0., 0., (gdz+ggdz)/2.);

  TGeoVolume* counter = new TGeoVolumeAssembly("counter");
  counter->AddNode(pcb_vol, 0, pcb_trans0);
  Int_t l = 0;
  for (l = 0; l < ngaps + 1; l++) {
    if (l % 2 == 0) {
      if (l == 0) {
        TGeoTranslation* glass_plate_trans = new TGeoTranslation("", 0., 0., 0.5 * (pdz + gdz));
        counter->AddNode(glass_plate_vol, l, glass_plate_trans);
        TGeoTranslation* glass_plate_trans1 = new TGeoTranslation("", 0., 0., -0.5 * (pdz + gdz));
        counter->AddNode(glass_plate_vol, l + ngaps + 1, glass_plate_trans1);
      }
      else {
        TGeoTranslation* glass_plate_trans =
          new TGeoTranslation("", 0., 0., 0.5 * (pdz + gdz) + l * (0.5 * (ggdz + gdz)));
        counter->AddNode(glass_plate_vol, l, glass_plate_trans);
        TGeoTranslation* glass_plate_trans1 =
          new TGeoTranslation("", 0., 0., -0.5 * (pdz + gdz) - l * (0.5 * (ggdz + gdz)));
        counter->AddNode(glass_plate_vol, l + ngaps + 1, glass_plate_trans1);
      }
    }
    else {
      TGeoTranslation* gas_gap_trans = new TGeoTranslation("", 0., 0., 0.5 * (pdz + gdz) + l * (0.5 * (ggdz + gdz)));
      counter->AddNode(gas_gap_vol, l, gas_gap_trans);
      TGeoTranslation* gas_gap_trans1 = new TGeoTranslation("", 0., 0., -0.5 * (pdz + gdz) - l * (0.5 * (ggdz + gdz)));
      counter->AddNode(gas_gap_vol, l + ngaps + 1, gas_gap_trans1);
    }
  }
  TGeoTranslation* pcb_trans1 = new TGeoTranslation("", 0., 0., (pdz + gdz) + (l - 1) * (0.5 * (ggdz + gdz)));
  counter->AddNode(pcb_vol, l + ngaps + 1, pcb_trans1);
  TGeoTranslation* pcb_trans2 = new TGeoTranslation("", 0., 0., -(pdz + gdz) - (l - 1) * (0.5 * (ggdz + gdz)));
  counter->AddNode(pcb_vol, l + ngaps + 1, pcb_trans2);


  TGeoBBox* epcb       = new TGeoBBox("", dxe / 2., dye / 2., dze / 2.);
  TGeoVolume* epcb_vol = new TGeoVolume("epcb", epcb, electronicsVolMed);
  epcb_vol->SetLineColor(kCyan);  // set line color for the electronic
  epcb_vol->SetTransparency(10);  // set transparency for the TOF
  for (Int_t l = 0; l < 2; l++) {
    yele *= -1.;
    TGeoTranslation* epcb_trans = new TGeoTranslation("", 0., yele, 0.);
    counter->AddNode(epcb_vol, l, epcb_trans);
  }

  return counter;
}

TGeoVolume* create_counter_singlestack(Int_t modType)
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
  counter->SetLineColor(kCyan);  // set line color for the counter
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

TGeoVolume* create_new_tof_module(Int_t modType)
{

  Float_t dx          = Module_Size_X[modType];
  Float_t dy          = Module_Size_Y[modType];
  Float_t dz          = Module_Size_Z[modType];
  Float_t width_aluxl = Module_Thick_Alu_X_left;
  Float_t width_aluxr = Module_Thick_Alu_X_right;
  Float_t width_aluy  = Module_Thick_Alu_Y;
  Float_t width_aluzf = Module_Thick_Alu_Z_front;
  Float_t width_aluzb = Module_Thick_Alu_Z_back;
  Float_t rotangle    = CounterRotationAngle[modType];

  TGeoMedium* boxVolMed         = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* noActiveGasVolMed = gGeoMan->GetMedium(NoActivGasMedium);

  TString moduleName = Form("module_%d", modType);

  TGeoBBox* module_box = new TGeoBBox("", dx / 2., dy / 2., dz / 2.);
  TGeoVolume* module   = new TGeoVolume(moduleName, module_box, boxVolMed);
  module->SetLineColor(kGreen);  // set line color for the alu box
  module->SetTransparency(20);   // set transparency for the TOF

  if (modType < 5) {
    TGeoBBox* gas_box       = new TGeoBBox("", (dx - (width_aluxl + width_aluxr)) / 2., (dy - 2 * width_aluy) / 2.,
                                     (dz - 2 * width_aluzf) / 2.);
    TGeoVolume* gas_box_vol = new TGeoVolume("gas_box", gas_box, noActiveGasVolMed);
    gas_box_vol->SetLineColor(kBlue);  // set line color for the alu box
    gas_box_vol->SetTransparency(50);  // set transparency for the TOF
    TGeoTranslation* gas_box_trans = new TGeoTranslation("", shift_gas_box_x, 0., 0.);
    module->AddNode(gas_box_vol, 0, gas_box_trans);

    for (Int_t j = 0; j < NCounterInModule[modType]; j++) {  //loop over counters (modules)
      cout << j << "  " << modType << "  xPos " << xPosCou[modType][j] << "  yPos " << yPosCou[modType][j] << "  zPos "
           << zPosCou[modType][j] << endl;
      TGeoTranslation* counter_trans =
        new TGeoTranslation("", xPosCou[modType][j], yPosCou[modType][j], zPosCou[modType][j]);
      TGeoRotation* counter_rot = new TGeoRotation();
      counter_rot->RotateY(rotangle);
      TGeoCombiTrans* counter_combi_trans = new TGeoCombiTrans(*counter_trans, *counter_rot);
      gas_box_vol->AddNode(gCounter[CouType[modType][j]], j, counter_combi_trans);
    }
    cout << "-------------------------------" << endl;
  }
  else {
    TGeoBBox* gas_box       = new TGeoBBox("", (dx - 2 * width_aluxr) / 2., (dy - 2 * width_aluy) / 2.,
                                     (dz + (width_aluzf - width_aluzb)) / 2.);
    TGeoVolume* gas_box_vol = new TGeoVolume("gas_box", gas_box, noActiveGasVolMed);
    gas_box_vol->SetLineColor(kBlue);  // set line color for the alu box
    gas_box_vol->SetTransparency(50);  // set transparency for the TOF
    TGeoTranslation* gas_box_trans = new TGeoTranslation("", 0., 0., -shift_gas_box_z);
    module->AddNode(gas_box_vol, 0, gas_box_trans);

    for (Int_t j = 0; j < NCounterInModule[modType]; j++) {  //loop over counters (modules)
      cout << j << "  " << modType << "  xPos " << xPosCou[modType][j] << "  yPos " << yPosCou[modType][j] << "  zPos "
           << zPosCou[modType][j] << endl;
      TGeoTranslation* counter_trans =
        new TGeoTranslation("", xPosCou[modType][j], yPosCou[modType][j], zPosCou[modType][j]);
      TGeoRotation* counter_rot = new TGeoRotation();
      counter_rot->RotateY(rotangle);
      TGeoCombiTrans* counter_combi_trans = new TGeoCombiTrans(*counter_trans, *counter_rot);
      gas_box_vol->AddNode(gCounter[CouType[modType][j]], j, counter_combi_trans);
    }
    cout << "-------------------------------" << endl;
  }
  return module;
}

TGeoVolume* create_tof_pole()
{
  // needed materials
  TGeoMedium* boxVolMed = gGeoMan->GetMedium(PoleVolumeMedium);
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
  //  pole_alu_vol->SetTransparency(20); // set transparency for the TOF
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

TGeoVolume* create_tof_poleshort()
{
  // needed materials
  TGeoMedium* boxVolMed = gGeoMan->GetMedium(PoleVolumeMedium);
  TGeoMedium* airVolMed = gGeoMan->GetMedium(KeepingVolumeMedium);

  Float_t dx         = Pole_Size_X;
  Float_t dy         = PoleShort_Size_Y;
  Float_t dz         = Pole_Size_Z;
  Float_t width_alux = Pole_Thick_X;
  Float_t width_aluy = Pole_Thick_Y;
  Float_t width_aluz = Pole_Thick_Z;

  TGeoVolume* pole         = new TGeoVolumeAssembly("PoleShort");
  TGeoBBox* pole_alu_box   = new TGeoBBox("", dx / 2., dy / 2., dz / 2.);
  TGeoVolume* pole_alu_vol = new TGeoVolume("poleshort_alu", pole_alu_box, boxVolMed);
  pole_alu_vol->SetLineColor(kGreen);  // set line color for the alu box
  //  pole_alu_vol->SetTransparency(20); // set transparency for the TOF
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
    TGeoVolume* pole_air_vol = new TGeoVolume("poleshort_air", pole_air_box, airVolMed);
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
  TGeoMedium* boxVolMed = gGeoMan->GetMedium(PoleVolumeMedium);
  TGeoMedium* airVolMed = gGeoMan->GetMedium(KeepingVolumeMedium);

  Float_t width_alux = Pole_Thick_X;
  Float_t width_aluy = Pole_Thick_Y;
  Float_t width_aluz = Pole_Thick_Z;

  TGeoVolume* bar         = new TGeoVolumeAssembly("Bar");
  TGeoBBox* bar_alu_box   = new TGeoBBox("", dx / 2., dy / 2., dz / 2.);
  TGeoVolume* bar_alu_vol = new TGeoVolume("bar_alu", bar_alu_box, boxVolMed);
  bar_alu_vol->SetLineColor(kGreen);  // set line color for the alu box
  //  bar_alu_vol->SetTransparency(20); // set transparency for the TOF
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

  Int_t numPoles      = 0;
  Int_t numPolesShort = 0;
  for (Int_t i = 0; i < NumberOfPoles; i++) {

    Float_t xPos = Pole_XPos[i];
    Float_t zPos = Pole_ZPos[i];
    if (TMath::Abs(xPos) > XLimInner) {
      pole_trans = new TGeoTranslation("", xPos, 0., zPos);
      gGeoMan->GetVolume(geoVersion)->AddNode(gPole, numPoles, pole_trans);
      numPoles++;
    }
    else {  // position 2 short poles

      // upper short poles
      pole_trans = new TGeoTranslation("", xPos, Pole_Size_Y / 2. - PoleShort_Size_Y / 2., zPos);
      gGeoMan->GetVolume(geoVersion)->AddNode(gPoleShort, numPolesShort, pole_trans);
      numPolesShort++;

      // lower short poles
      pole_trans = new TGeoTranslation("", xPos, PoleShort_Size_Y / 2. - Pole_Size_Y / 2., zPos);
      gGeoMan->GetVolume(geoVersion)->AddNode(gPoleShort, numPolesShort, pole_trans);
      numPolesShort++;
    }
    //     cout << " Position Pole "<< numPoles<<" at z="<< Pole_ZPos[i] <<", x "<<Pole_XPos[i]<< endl;
  }
}

void position_tof_bars(Int_t modType)
{

  TGeoTranslation* bar_trans = NULL;

  Int_t numBars = 0;
  Int_t i, j;
  for (i = 0; i < NumberOfBars; i++) {

    Float_t xPos = Bar_XPos[i];
    Float_t zPos = Bar_ZPos[i];
    Float_t yPos = Pole_Size_Y / 2. + Bar_Size_Y / 2.;

    bar_trans = new TGeoTranslation("", xPos, yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    bar_trans = new TGeoTranslation("", xPos, -yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    bar_trans = new TGeoTranslation("", -xPos, yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    bar_trans = new TGeoTranslation("", -xPos, -yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;
  }
  cout << " Position Bar " << numBars << " at z=" << Bar_ZPos[i] << endl;

  // outer horizontal and vertical frame bars

  NumberOfBars++;
  i               = NumberOfBars;
  gBar[i]         = create_tof_bar(Frame_XLen, Frame_Size_Y, Frame_Size_Y);  // Outer frame big bar along X
  j               = i + 1;
  gBar[j]         = create_tof_bar(Frame_Size_X, Frame_YLen, Frame_Size_Y);  // Outer frame big bar along Y
  Float_t numBarY = 0;
  numBars         = 0;

  for (Float_t dZ = -1.; dZ < 2.; dZ += 2.) {
    Float_t zPos = Frame_Pos_Z - dZ * (Bar_Size_Z / 2. - 10.);
    Float_t yPos = Frame_YLen / 2. + Frame_Size_Y / 2;  // Make outer frame independent of the inner poles!!!!

    // Outer Frame Top bar
    bar_trans = new TGeoTranslation("", 0., yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    // Outer Frame Bottom bar
    bar_trans = new TGeoTranslation("", 0., -yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
    numBars++;

    // Outer Frame Right bar
    Float_t xPos = Frame_XLen / 2 - Frame_Size_Y / 2.;
    bar_trans    = new TGeoTranslation("", xPos, 0., zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[j], numBarY, bar_trans);
    numBarY++;

    // Outer Frame Left bar
    bar_trans = new TGeoTranslation("", -xPos, 0., zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gBar[j], numBarY, bar_trans);
    numBarY++;
  }
}

void position_tof_modules()
{
  TGeoTranslation* module_trans = NULL;
  TGeoRotation* module_rot0     = new TGeoRotation();
  module_rot0->RotateZ(0.);
  TGeoRotation* module_rot1 = new TGeoRotation();
  module_rot1->RotateZ(180.);
  TGeoCombiTrans* module_combi_trans = NULL;

  //  if(modType != 0) continue; // debugging
  for (Int_t i = 0; i < MaxNofModules; i++) {
    // for(Int_t i=0; i<5; i++) {
    //if(i != 0) continue; // debugging
    Float_t xPos = xPosMod[i];
    Float_t yPos = yPosMod[i];
    Float_t zPos = zPosMod[i] - 884.0 + TOF_Z_Front;
    //cout<<"Place Mod Type "<<j<<" at x "<<xPos<<", y "<<yPos<<", z "<<zPos<<", Flip "<<FlipMod[j][i]<<endl;

    module_trans = new TGeoTranslation("", xPos, yPos, zPos);
    if (ModType[i] < 5 && xPosMod[i] < 0.0) { module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot1); }
    else {
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot0);
    }
    cout << i << "   ModType  " << ModType[i] << "  iMod:  " << iMod[ModType[i]] << endl;
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[ModType[i]], iMod[ModType[i]], module_combi_trans);
    iMod[ModType[i]]++;
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

  const Float_t TOF_Z_Back = TOF_Z_Front + 176.5;  // back of TOF wall

  fprintf(ifile, "# envelope\n");
  // Show extension of TRD
  fprintf(ifile, "%7.2f cm   start of TOF (z)\n", TOF_Z_Front - 10.);
  fprintf(ifile, "%7.2f cm   end   of TOF (z)\n", TOF_Z_Back + 10.);
  fprintf(ifile, "\n");

  // Layer thickness
  fprintf(ifile, "# central tower position\n");
  fprintf(ifile, "%7.2f cm   center of staggered, front RPC cell at x=0\n", Wall_Z_Position);
  fprintf(ifile, "\n");

  fclose(ifile);
}
