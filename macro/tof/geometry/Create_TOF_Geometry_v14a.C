// Macro creating a ToF geometry for test beam 
//
// Usage:
// - bNhVersion = kTRUE => only the X position of the SM is optimized, values stored in
//   DxColl[Outer_Module_Col]
// - bNhVersion = kFALSE => Y position is also optimized with an angular overlap from
//   target of kfAngOverlap
//   * Wall_Z_ReferenceSmPos = Wall_Z_Position => best Y position to get kfAngOverlap
//     at the Wall_Z_Position
//   * Wall_Z_ReferenceSmPos = XXX cm => best Y position to get kfAngOverlap
//     at XXX cm from target, allow to have a fixed geometry just shifted in Z
// in root all sizes are given in cm

#include "TSystem.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoPgon.h"
#include "TGeoMatrix.h"
#include "TGeoCompositeShape.h"
#include "TFile.h"
#include "TString.h"
#include "TList.h"
#include "TROOT.h"
#include "TMath.h"

#include <iostream>

// Name of geometry version and output file
const TString geoVersion = "tof_v14a";
const TString FileName = geoVersion + ".root";

// Names of the different used materials which are used to build the modules
// The materials are defined in the global media.geo file 
const TString KeepingVolumeMedium     = "air";
const TString BoxVolumeMedium         = "aluminium";
const TString NoActivGasMedium        = "RPCgas_noact";
const TString ActivGasMedium          = "RPCgas";
const TString GlasMedium              = "RPCglass";
const TString ElectronicsMedium       = "carbon";

// Distance to the center of the TOF wall [cm];
const Float_t Wall_Z_Position = 200; 
const Float_t Wall_ZPLA_Position = 190; 

// Form automatically Y shifted modules
Bool_t bNhVersion = kTRUE; // no Y shift
//Bool_t bNhVersion = kFALSE; // Y shift enabled
const Float_t kfAngOverlap = 0.1; // degrees
// Distance to the center of the TOF wall [cm] used for obtaining the SM position from overlap value in Y;
//const Float_t Wall_Z_ReferenceSmPos = Wall_Z_Position; // None => Y position optimized for each Z position
const Float_t Wall_Z_ReferenceSmPos = 200; // target distance of setup 

// Counters: 
// 0 HD small
// 1 HD big
// 2 HD ref
// 3 Buc ref
// 4 Pla 
// 5 Diamond 
// 6 Buc 2013
// 7 USTC
// 8 Buc 2012
// 9 THU Pad 
const Int_t NumberOfDifferentCounterTypes = 8;
const Float_t Glass_X[NumberOfDifferentCounterTypes] = {32.,  52., 15.04, 18.,  2.,   0.2, 26.816, 32.};
const Float_t Glass_Y[NumberOfDifferentCounterTypes] = {26.9, 53.,  4.,    4.6, 10.,   0.2, 20.0,   27.};
const Float_t Glass_Z[NumberOfDifferentCounterTypes] = {0.1,   0.1, 0.1,   0.1, 0.01, 0.01, 0.1,    0.1};

const Float_t GasGap_X[NumberOfDifferentCounterTypes] = {32., 52., 15.04, 18.,   2.,  0.2, 26.816, 32.};
const Float_t GasGap_Y[NumberOfDifferentCounterTypes] = {26.9,53.,  4.,    4.6,  10.,  0.2, 20.0,   27.};
const Float_t GasGap_Z[NumberOfDifferentCounterTypes] = {0.025,0.025,0.025,0.025,2.,  0.01, 0.02,   0.02};

const Int_t NumberOfGaps[NumberOfDifferentCounterTypes] = {8,8,6,10,1,1,8,8};
const Int_t NumberOfReadoutStrips[NumberOfDifferentCounterTypes] = {32,56,16,72,1,16,64,16};

const Float_t SingleStackStartPosition_Z[NumberOfDifferentCounterTypes] = {-0.6,-0.6,-0.6,-0.6,-1.,-0.1,-0.6,-0.6};

const Float_t Electronics_X[NumberOfDifferentCounterTypes] = {34.0,53.0,15.5, 18.5, 2.,  0.3, 27., 34.};
const Float_t Electronics_Y[NumberOfDifferentCounterTypes] = { 5.0, 5.0, 1.0,  1.,  2.,  0.1,  4.,  5.};
const Float_t Electronics_Z[NumberOfDifferentCounterTypes] = { 0.3, 0.3, 0.3,  0.3, 0.3, 0.1,  0.3, 0.3};

const Int_t NofModuleTypes = 8;
// Aluminum box for all supermodule types
// 0 HDbig
// 1 BucRef
// 2 Pla 
// 3 HD small 
// 4 HD ref
// 5 Diamond
// 6 Buc 2013
// 7 USTC
// 8 Buc 2012
// 9 THU
const Float_t Module_Size_X[NofModuleTypes] = {120.,35., 5.,100., 30., 5., 30., 60.};
const Float_t Module_Size_Y[NofModuleTypes] = { 74.,18.,20., 49., 16., 5., 21., 60.};
const Float_t Module_Over_Y[NofModuleTypes] = { 11.,11., 0.,  4.,  0., 0.,  0.,  0.};
const Float_t Module_Size_Z[NofModuleTypes] = { 10.,10., 5., 10., 10., 1., 10., 10.};
const Float_t Module_Thick_Alu_X_left = 5.;
const Float_t Module_Thick_Alu_X_right = 1.;
const Float_t Module_Thick_Alu_Y = 1.;
const Float_t Module_Thick_Alu_Z = 0.1;

//Type of Counter for module 
const Int_t CounterTypeInModule[NofModuleTypes] = {1,3,4,0,2,5,6,7};
const Int_t NCounterInModule[NofModuleTypes]    = {1,1,1,1,1,1,1,1};

// Placement of the counter inside the module
const Float_t CounterXStartPosition[NofModuleTypes] = {-10.0,     0.0,   2.0,  0.0,  0.0,  0., 0., 0.};
const Float_t CounterXDistance[NofModuleTypes]      = {30.0,   30.6,   0.0, 30.0,  0.0,  0., 0., 0.};
const Float_t CounterZDistance[NofModuleTypes]      = {0.0,     0.0,   0.0,  2.5,  0.0,  0., 0., 0.};
const Float_t CounterRotationAngle[NofModuleTypes]  = {0.,      0.0,   0.0,   0.,  0.0,  0., 0., 0.};

// Pole (support structure)
const Int_t MaxNumberOfPoles=20;
Float_t Pole_ZPos[MaxNumberOfPoles];
Float_t Pole_Col[MaxNumberOfPoles];
Int_t NumberOfPoles=0;

const Float_t Pole_Size_X = 20.;
const Float_t Pole_Size_Y = 1000.;
const Float_t Pole_Size_Z = 10.;
const Float_t Pole_Thick_X = 5.;
const Float_t Pole_Thick_Y = 5.;
const Float_t Pole_Thick_Z = 5.;

//Float_t Pole_ZPos[MaxNumberOfPoles];


// Bars (support structure)
const Float_t Bar_Size_X = 20.;
const Float_t Bar_Size_Y = 20.;
Float_t Bar_Size_Z = 100.;

const Int_t MaxNumberOfBars=20;
Float_t Bar_ZPos[MaxNumberOfBars];
Float_t Bar_XPos[MaxNumberOfBars];
Int_t NumberOfBars=0;

const Float_t ChamberOverlap=40;
//const Float_t DxColl=153.0; //158.  //Module_Size_X-ChamberOverlap;
//const Float_t Pole_Offset=Module_Size_X/2.+Pole_Size_X/2.;
const Float_t Pole_Offset=90.0+Pole_Size_X/2.;

// Position for module placement
const Float_t Inner_Module_First_Y_Position=20.;
const Float_t Inner_Module_Last_Y_Position=40.;
const Float_t Inner_Module_X_Offset=2.;
const Int_t Inner_Module_NTypes = 3;
const Float_t Inner_Module_Types[Inner_Module_NTypes]  = {0.,5.,0.};
const Float_t Inner_Module_Number[Inner_Module_NTypes] = {1.,2.,0.}; //debugging, V14a

const Float_t HDbig_Z_Position=222.;
const Float_t HDbig_First_Y_Position=-86.;
const Float_t HDbig_X_Offset=-1.5; //3.95;
const Float_t HDbig_rotate_Z=-90.;
const Int_t HDbig_NTypes = 1;
const Float_t HDbig_Types[HDbig_NTypes]  = {0.};
const Float_t HDbig_Number[HDbig_NTypes] = {1.}; //debugging, V14a

const Float_t HDsmall_Z_Position=220.;
//const Float_t HDsmall_First_Y_Position=-68.;  // HD - P2
const Float_t HDsmall_First_Y_Position=-65.;  // TSU
const Float_t HDsmall_X_Offset=0.;
const Float_t HDsmall_rotate_Z=90.;
const Int_t HDsmall_NTypes = 1;
const Float_t HDsmall_Types[HDsmall_NTypes]  = {3.};
const Float_t HDsmall_Number[HDsmall_NTypes] = {1.}; //debugging, V14a

const Float_t BucRef_Z_Position=234.;
const Float_t BucRef_First_Y_Position=0.;
const Float_t BucRef_X_Offset=30.5;
const Float_t BucRef_rotate_Z=0.;
const Int_t BucRef_NTypes = 1;
const Float_t BucRef_Types[BucRef_NTypes]  = {1.};
const Float_t BucRef_Number[BucRef_NTypes] = {1.}; //debugging, V14a

const Float_t HDRef_Z_Position=246.;
const Float_t HDRef_First_Y_Position=-76.;
const Float_t HDRef_X_Offset=-2.;
const Float_t HDRef_rotate_Z=180.;
const Int_t HDRef_NTypes = 1;
const Float_t HDRef_Types[HDRef_NTypes]  = {4.}; // this is the SmType!
const Float_t HDRef_Number[HDRef_NTypes] = {1.}; // evtl. double for split signals 

const Float_t Dia_Z_Position=-2.;
const Float_t Dia_First_Y_Position=0.;
const Float_t Dia_X_Offset=0.;
const Float_t Dia_rotate_Z=0.;
const Int_t Dia_NTypes = 3;
const Float_t Dia_Types[Dia_NTypes]  = {5.,5.,5.};
const Float_t Dia_Number[Dia_NTypes] = {1.,1.,1.};

const Float_t Buc2013_Z_Position=203.3;
const Float_t Buc2013_First_Y_Position=0.;
const Float_t Buc2013_X_Offset=26.;
const Float_t Buc2013_rotate_Z=0.;
const Int_t Buc2013_NTypes = 1;
const Float_t Buc2013_Types[Buc2013_NTypes]  = {6.};
const Float_t Buc2013_Number[Buc2013_NTypes] = {1.};

const Float_t USTC_Z_Position=200.;
const Float_t USTC_First_Y_Position=0.;
const Float_t USTC_X_Offset=80.;
const Float_t USTC_rotate_Z=90.;
const Int_t USTC_NTypes = 1;
const Float_t USTC_Types[USTC_NTypes]  = {7.};
const Float_t USTC_Number[USTC_NTypes] = {1.};

const Int_t Pla_NTypes = 4;
const Float_t Pla_X_Position[Pla_NTypes]={0.,0.,25.,33.7};
const Float_t Pla_Y_Position[Pla_NTypes]={-60.,-80.,0.,0.};
const Float_t Pla_Z_Position[Pla_NTypes]={191.,263.,191.8,258.5};
const Float_t Pla_rotate_Z[Pla_NTypes]={90.,90.,90.,90.};
const Float_t Pla_Types[Pla_NTypes]  = {2.,2.,2.,2.};
const Float_t Pla_Number[Pla_NTypes] = {1.,1.,1.,1.};   //V14a

const Float_t InnerSide_Module_X_Offset=51.;
const Int_t InnerSide_Module_NTypes = 1;
const Float_t InnerSide_Module_Types[Inner_Module_NTypes]  = {5.};
//const Float_t InnerSide_Module_Number[Inner_Module_NTypes] = {2.};  //v13_3a
const Float_t InnerSide_Module_Number[Inner_Module_NTypes] = {0.};  //debug, V14a

const Float_t Outer_Module_First_Y_Position=0.;
const Float_t Outer_Module_Last_Y_Position=480.;
const Float_t Outer_Module_X_Offset=2.;  //3.
const Int_t Outer_Module_Col = 4;
const Int_t Outer_Module_NTypes = 2;
const Float_t Outer_Module_Types [Outer_Module_NTypes][Outer_Module_Col] = {1.,1.,1.,1.,  2.,2.,2.,2.};
//const Float_t Outer_Module_Number[Outer_Module_NTypes][Outer_Module_Col] = {9.,9.,2.,0.,  0.,0.,3.,4.};//V13_3a
const Float_t Outer_Module_Number[Outer_Module_NTypes][Outer_Module_Col] = {1.,1.,0.,0.,  0.,0.,0.,0.};//V14a
const Float_t DxColl[Outer_Module_Col] = { 153, 147, 126, 93 }; //158.

// some global variables
TGeoManager* gGeoMan = NULL;  // Pointer to TGeoManager instance
TGeoVolume* gModules[NofModuleTypes]; // Global storage for module types
TGeoVolume* gCounter[NumberOfDifferentCounterTypes];
TGeoVolume* gPole;
TGeoVolume* gBar[MaxNumberOfBars];

Float_t Last_Size_Y=0.;
Float_t Last_Over_Y=0.;

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
void position_inner_tof_modules(Int_t); // HDbig
void position_pla_modules(Int_t); 
void position_HDbig(Int_t);
void position_HDsmall(Int_t);
void position_BucRef(Int_t);
void position_HDRef(Int_t);
void position_Dia(Int_t);
void position_Buc2013(Int_t);
void position_USTC(Int_t);
void position_side_tof_modules(Int_t);
void position_outer_tof_modules(Int_t);

// Form automatically Y shifted modules
const Double_t dRefOffsetZ = Wall_Z_ReferenceSmPos - Wall_Z_Position;
Float_t fPrevModPosY = 0;
Int_t   iPrevModType = 0;
Float_t fPrevModPosZ = 0;
const Float_t Module_Dead_Y[NofModuleTypes] = {
      Module_Size_Y[0] - GasGap_Y[ CounterTypeInModule[0] ] ,
      Module_Size_Y[1] - GasGap_Y[ CounterTypeInModule[1] ],
      Module_Size_Y[2] - GasGap_Y[ CounterTypeInModule[2] ],
      Module_Size_Y[3] - GasGap_Y[ CounterTypeInModule[3] ],
      Module_Size_Y[4] - GasGap_Y[ CounterTypeInModule[4] ],
      Module_Size_Y[5] - GasGap_Y[ CounterTypeInModule[5] ]};
Float_t GetShiftedY( Double_t, Int_t, Double_t,
                       Int_t, Double_t,
                       Double_t);

void Create_TOF_Geometry_v14a() {
  // Load the necessary FairRoot libraries 
//  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
//  basiclibs();
//  gSystem->Load("libGeoBase");
//  gSystem->Load("libParBase");
//  gSystem->Load("libBase");

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
 
  TGeoVolume* tof = new TGeoVolumeAssembly(geoVersion);
  top->AddNode(tof, 1);
  
  for(Int_t counterType = 0; counterType < NumberOfDifferentCounterTypes; counterType++) { 
    gCounter[counterType] = create_new_counter(counterType);
  }

  for(Int_t moduleType = 0; moduleType < NofModuleTypes; moduleType++) { 
    gModules[moduleType] = create_new_tof_module(moduleType);
    gModules[moduleType]->SetVisContainers(1); 
  }  

  //  gPole = create_tof_pole();

  //  position_HDbig(1);
  position_HDsmall(1);
  position_pla_modules(4);
  position_HDRef(1);
  position_Dia(3);
  position_USTC(1);
  position_BucRef(1);
  position_Buc2013(1);
  
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* outfile = new TFile(FileName,"RECREATE");
  top->Write();
  //gGeoMan->Write();
  outfile->Close();
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
  FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString geoFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(geoFile);
  geoFace->readMedia();

  // Read the required media and create them in the GeoManager
  FairGeoMedia* geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* air              = geoMedia->getMedium("air");
  FairGeoMedium* aluminium        = geoMedia->getMedium("aluminium");
  FairGeoMedium* RPCgas           = geoMedia->getMedium("RPCgas");
  FairGeoMedium* RPCgas_noact     = geoMedia->getMedium("RPCgas_noact");
  FairGeoMedium* RPCglass         = geoMedia->getMedium("RPCglass");
  FairGeoMedium* carbon           = geoMedia->getMedium("carbon");

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
  Int_t l=0;

  //glass
  Float_t gdx=Glass_X[modType]; 
  Float_t gdy=Glass_Y[modType];
  Float_t gdz=Glass_Z[modType];

  //gas gap
  Int_t  nstrips=NumberOfReadoutStrips[modType];
  Int_t  ngaps=NumberOfGaps[modType];


  Float_t ggdx=GasGap_X[modType];  
  Float_t ggdy=GasGap_Y[modType];
  Float_t ggdz=GasGap_Z[modType];
  Float_t gsdx=ggdx/float(nstrips);

  //single stack
  Float_t dzpos=gdz+ggdz;
  Float_t startzpos=SingleStackStartPosition_Z[modType];

  // electronics
  //pcb dimensions 
  Float_t dxe=Electronics_X[modType]; 
  Float_t dye=Electronics_Y[modType];
  Float_t dze=Electronics_Z[modType];
  Float_t yele=(gdy+0.1)/2.+dye/2.;
 
  // needed materials
  TGeoMedium* glassPlateVolMed   = gGeoMan->GetMedium(GlasMedium);
  TGeoMedium* noActiveGasVolMed  = gGeoMan->GetMedium(NoActivGasMedium);
  TGeoMedium* activeGasVolMed    = gGeoMan->GetMedium(ActivGasMedium);
  TGeoMedium* electronicsVolMed  = gGeoMan->GetMedium(ElectronicsMedium);

  // Single glass plate
  TGeoBBox* glass_plate = new TGeoBBox("", gdx/2., gdy/2., gdz/2.);
  TGeoVolume* glass_plate_vol = 
    new TGeoVolume("tof_glass", glass_plate, glassPlateVolMed);
  glass_plate_vol->SetLineColor(kMagenta); // set line color for the glass plate
  glass_plate_vol->SetTransparency(20); // set transparency for the TOF
  TGeoTranslation* glass_plate_trans 
    = new TGeoTranslation("", 0., 0., 0.);

  // Single gas gap
  TGeoBBox* gas_gap = new TGeoBBox("", ggdx/2., ggdy/2., ggdz/2.);
  //TGeoVolume* gas_gap_vol = 
  //new TGeoVolume("tof_gas_gap", gas_gap, noActiveGasVolMed);
  TGeoVolume* gas_gap_vol = 
    new TGeoVolume("tof_gas_active", gas_gap, activeGasVolMed);
  gas_gap_vol->Divide("Strip",1,nstrips,-ggdx/2.,0);

  gas_gap_vol->SetLineColor(kRed); // set line color for the gas gap
  gas_gap_vol->SetTransparency(70); // set transparency for the TOF
  TGeoTranslation* gas_gap_trans 
    = new TGeoTranslation("", 0., 0., (gdz+ggdz)/2.);

 
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

  // Add 8 single stacks + one glass plate at the end to a multi stack
  TGeoVolume* multi_stack = new TGeoVolumeAssembly("multi_stack");
  for (l=0; l<ngaps; l++){
    TGeoTranslation* single_stack_trans 
      = new TGeoTranslation("", 0., 0., startzpos + l*dzpos);
    multi_stack->AddNode(single_stack, l, single_stack_trans);
  }
  TGeoTranslation* single_glass_back_trans 
    = new TGeoTranslation("", 0., 0., startzpos + ngaps*dzpos);
  multi_stack->AddNode(glass_plate_vol, l, single_glass_back_trans);
  
  // Add electronics above and below the glass stack to build a complete counter
  TGeoVolume* counter = new TGeoVolumeAssembly("counter");
  TGeoTranslation* multi_stack_trans 
      = new TGeoTranslation("", 0., 0., 0.);
  counter->AddNode(multi_stack, l, multi_stack_trans);

  TGeoBBox* pcb = new TGeoBBox("", dxe/2., dye/2., dze/2.);
  TGeoVolume* pcb_vol = 
    new TGeoVolume("pcb", pcb, electronicsVolMed);
  pcb_vol->SetLineColor(kCyan); // set line color for the gas gap
  pcb_vol->SetTransparency(10); // set transparency for the TOF
  for (Int_t l=0; l<2; l++){
    yele *= -1.;
    TGeoTranslation* pcb_trans 
      = new TGeoTranslation("", 0., yele, 0.);
    counter->AddNode(pcb_vol, l, pcb_trans);
  }

  return counter;

}

TGeoVolume* create_new_counter(Int_t modType)
{

  //glass
  Float_t gdx=Glass_X[modType]; 
  Float_t gdy=Glass_Y[modType];
  Float_t gdz=Glass_Z[modType];

  //gas gap
  Int_t  nstrips=NumberOfReadoutStrips[modType];
  Int_t  ngaps=NumberOfGaps[modType];


  Float_t ggdx=GasGap_X[modType];  
  Float_t ggdy=GasGap_Y[modType];
  Float_t ggdz=GasGap_Z[modType];
  Float_t gsdx=ggdx/(Float_t)(nstrips);

  // electronics
  //pcb dimensions 
  Float_t dxe=Electronics_X[modType]; 
  Float_t dye=Electronics_Y[modType];
  Float_t dze=Electronics_Z[modType];
  Float_t yele=gdy/2.+dye/2.;
 
  // counter size (calculate from glas, gap and electronics sizes)
  Float_t cdx = TMath::Max(gdx, ggdx);
  cdx = TMath::Max(cdx, dxe)+ 0.2;
  Float_t cdy = TMath::Max(gdy, ggdy) + 2*dye + 0.2;
  Float_t cdz = ngaps * (gdz+ggdz) + gdz + 0.2;

  //calculate thickness and first position in coonter of single stack
  Float_t dzpos=gdz+ggdz;
  Float_t startzposglas=(-cdz+gdz)/2.;
  Float_t startzposgas=-cdz/2.+ gdz + ggdz/2.;


  // needed materials
  TGeoMedium* glassPlateVolMed   = gGeoMan->GetMedium(GlasMedium);
  TGeoMedium* noActiveGasVolMed  = gGeoMan->GetMedium(NoActivGasMedium);
  TGeoMedium* activeGasVolMed    = gGeoMan->GetMedium(ActivGasMedium);
  TGeoMedium* electronicsVolMed  = gGeoMan->GetMedium(ElectronicsMedium);


  // define counter volume
  TGeoBBox* counter_box = new TGeoBBox("", cdx/2., cdy/2., cdz/2.);
  TGeoVolume* counter = 
    new TGeoVolume("counter", counter_box, noActiveGasVolMed);
  counter->SetLineColor(kCyan); // set line color for the counter
  counter->SetTransparency(70); // set transparency for the TOF

  // define single glass plate volume
  TGeoBBox* glass_plate = new TGeoBBox("", gdx/2., gdy/2., gdz/2.);
  TGeoVolume* glass_plate_vol = 
    new TGeoVolume("tof_glass", glass_plate, glassPlateVolMed);
  glass_plate_vol->SetLineColor(kMagenta); // set line color for the glass plate
  glass_plate_vol->SetTransparency(20); // set transparency for the TOF
  // define single gas gap volume
  TGeoBBox* gas_gap = new TGeoBBox("", ggdx/2., ggdy/2., ggdz/2.);
  TGeoVolume* gas_gap_vol = 
    new TGeoVolume("Gap", gas_gap, activeGasVolMed);
  gas_gap_vol->Divide("Cell",1,nstrips,-ggdx/2.,0);
  gas_gap_vol->SetLineColor(kRed); // set line color for the gas gap
  gas_gap_vol->SetTransparency(99); // set transparency for the TOF
 
  // place 8 gas gaps and 9 glas plates in the counter
  for( Int_t igap = 0; igap < ngaps; igap++) {

    Float_t zpos_glas = startzposglas + igap*dzpos;
    Float_t zpos_gas = startzposgas + igap*dzpos;

    //    cout <<"Zpos(Glas): "<< zpos_glas << endl;
    //    cout <<"Zpos(Gas): "<< zpos_gas << endl;

    TGeoTranslation* glass_plate_trans 
      = new TGeoTranslation("", 0., 0., zpos_glas);
    TGeoTranslation* gas_gap_trans 
      = new TGeoTranslation("", 0., 0., zpos_gas);
    counter->AddNode(glass_plate_vol, igap, glass_plate_trans);
    counter->AddNode(gas_gap_vol, igap, gas_gap_trans);

  }

  Float_t zpos_glas = startzposglas + (ngaps+1)*dzpos;
  TGeoTranslation* glass_plate_trans 
    = new TGeoTranslation("", 0., 0., zpos_glas);
  counter->AddNode(glass_plate_vol, ngaps, glass_plate_trans);
  
  // create and place the electronics above and below the glas stack
  TGeoBBox* pcb = new TGeoBBox("", dxe/2., dye/2., dze/2.);
  TGeoVolume* pcb_vol = 
    new TGeoVolume("pcb", pcb, electronicsVolMed);
  pcb_vol->SetLineColor(kCyan); // set line color for electronics
  pcb_vol->SetTransparency(10); // set transparency for the TOF
  for (Int_t l=0; l<2; l++){
    yele *= -1.;
    TGeoTranslation* pcb_trans 
      = new TGeoTranslation("", 0., yele, 0.);
    counter->AddNode(pcb_vol, l, pcb_trans);
  }
 

  return counter;

}

TGeoVolume* create_tof_module(Int_t modType)
{
  Int_t cType = CounterTypeInModule[modType];
  Float_t dx=Module_Size_X[modType];
  Float_t dy=Module_Size_Y[modType];
  Float_t dz=Module_Size_Z[modType];
  Float_t width_aluxl=Module_Thick_Alu_X_left;
  Float_t width_aluxr=Module_Thick_Alu_X_right;
  Float_t width_aluy=Module_Thick_Alu_Y;
  Float_t width_aluz=Module_Thick_Alu_Z;

  Float_t shift_gas_box = (Module_Thick_Alu_X_right - Module_Thick_Alu_X_left)/2;

  Float_t dxpos=CounterXDistance[modType];
  Float_t startxpos=CounterXStartPosition[modType];
  Float_t dzoff=CounterZDistance[modType];
  Float_t rotangle=CounterRotationAngle[modType];

  TGeoMedium* boxVolMed          = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* noActiveGasVolMed  = gGeoMan->GetMedium(NoActivGasMedium);

  TString moduleName = Form("module_%d", modType);
  TGeoVolume* module = new TGeoVolumeAssembly(moduleName);

  TGeoBBox* alu_box = new TGeoBBox("", dx/2., dy/2., dz/2.);
  TGeoVolume* alu_box_vol = 
    new TGeoVolume("alu_box", alu_box, boxVolMed);
  alu_box_vol->SetLineColor(kGreen); // set line color for the alu box
  alu_box_vol->SetTransparency(20); // set transparency for the TOF
  TGeoTranslation* alu_box_trans 
    = new TGeoTranslation("", 0., 0., 0.);
  module->AddNode(alu_box_vol, 0, alu_box_trans);

  TGeoBBox* gas_box = new TGeoBBox("", (dx-(width_aluxl+width_aluxr))/2., (dy-2*width_aluy)/2., (dz-2*width_aluz)/2.);
  TGeoVolume* gas_box_vol = 
    new TGeoVolume("gas_box", gas_box, noActiveGasVolMed);
  gas_box_vol->SetLineColor(kYellow); // set line color for the gas box
  gas_box_vol->SetTransparency(70); // set transparency for the TOF
  TGeoTranslation* gas_box_trans 
    = new TGeoTranslation("", shift_gas_box, 0., 0.);
  alu_box_vol->AddNode(gas_box_vol, 0, gas_box_trans);
  
  for (Int_t j=0; j<5; j++){ //loop over counters (modules)
    Float_t zpos;
    if (0 == modType) {
      zpos = dzoff *=-1;
    } else {
      zpos = 0.;
    }
    TGeoTranslation* counter_trans 
      = new TGeoTranslation("", startxpos+ j*dxpos , 0.0 , zpos);

    TGeoRotation* counter_rot = new TGeoRotation();
    counter_rot->RotateY(rotangle);
    TGeoCombiTrans* counter_combi_trans = new TGeoCombiTrans(*counter_trans, *counter_rot);
    gas_box_vol->AddNode(gCounter[cType], j, counter_combi_trans);
  }

  return module;
}

TGeoVolume* create_new_tof_module(Int_t modType)
{
  Int_t cType = CounterTypeInModule[modType];
  Float_t dx=Module_Size_X[modType];
  Float_t dy=Module_Size_Y[modType];
  Float_t dz=Module_Size_Z[modType];
  Float_t width_aluxl=Module_Thick_Alu_X_left;
  Float_t width_aluxr=Module_Thick_Alu_X_right;
  Float_t width_aluy=Module_Thick_Alu_Y;
  Float_t width_aluz=Module_Thick_Alu_Z;

  Float_t shift_gas_box = (Module_Thick_Alu_X_right - Module_Thick_Alu_X_left)/2;

  Float_t dxpos=CounterXDistance[modType];
  Float_t startxpos=CounterXStartPosition[modType];
  Float_t dzoff=CounterZDistance[modType];
  Float_t rotangle=CounterRotationAngle[modType];

  TGeoMedium* boxVolMed          = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* noActiveGasVolMed  = gGeoMan->GetMedium(NoActivGasMedium);

  TString moduleName = Form("module_%d", modType);

  TGeoBBox* module_box = new TGeoBBox("", dx/2., dy/2., dz/2.);
  TGeoVolume* module = 
  new TGeoVolume(moduleName, module_box, boxVolMed);
  module->SetLineColor(kGreen); // set line color for the alu box
  module->SetTransparency(20); // set transparency for the TOF

  TGeoBBox* gas_box = new TGeoBBox("", (dx-(width_aluxl+width_aluxr))/2., (dy-2*width_aluy)/2., (dz-2*width_aluz)/2.);
  TGeoVolume* gas_box_vol = 
    new TGeoVolume("gas_box", gas_box, noActiveGasVolMed);
  gas_box_vol->SetLineColor(kBlue); // set line color for the alu box
  gas_box_vol->SetTransparency(50); // set transparency for the TOF
  TGeoTranslation* gas_box_trans 
    = new TGeoTranslation("", shift_gas_box, 0., 0.);
  module->AddNode(gas_box_vol, 0, gas_box_trans);
  
  for (Int_t j=0; j< NCounterInModule[modType]; j++){ //loop over counters (modules)
    Float_t zpos;
    if (0 == modType || 3 == modType || 4 == modType || 5 == modType) {
      zpos = dzoff *=-1;
    } else {
      zpos = 0.;
    }
    TGeoTranslation* counter_trans 
      = new TGeoTranslation("", startxpos+ j*dxpos , 0.0 , zpos);

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
  TGeoMedium* boxVolMed   = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* airVolMed   = gGeoMan->GetMedium(KeepingVolumeMedium);
   
  Float_t dx=Pole_Size_X;
  Float_t dy=Pole_Size_Y;
  Float_t dz=Pole_Size_Z;
  Float_t width_alux=Pole_Thick_X;
  Float_t width_aluy=Pole_Thick_Y;
  Float_t width_aluz=Pole_Thick_Z;
  
  TGeoVolume* pole = new TGeoVolumeAssembly("Pole");
  TGeoBBox*   pole_alu_box = new TGeoBBox("", dx/2., dy/2., dz/2.);
  TGeoVolume* pole_alu_vol = 
    new TGeoVolume("pole_alu", pole_alu_box, boxVolMed);
  pole_alu_vol->SetLineColor(kGreen); // set line color for the alu box
  pole_alu_vol->SetTransparency(20); // set transparency for the TOF
  TGeoTranslation* pole_alu_trans 
    = new TGeoTranslation("", 0., 0., 0.);
  pole->AddNode(pole_alu_vol, 0, pole_alu_trans);

  TGeoBBox* pole_air_box = new TGeoBBox("", dx/2.-width_alux, dy/2.-width_aluy, dz/2.-width_aluz);
  TGeoVolume* pole_air_vol = 
    new TGeoVolume("pole_air", pole_air_box, airVolMed);
  pole_air_vol->SetLineColor(kYellow); // set line color for the alu box
  pole_air_vol->SetTransparency(70); // set transparency for the TOF
  TGeoTranslation* pole_air_trans 
    = new TGeoTranslation("", 0., 0., 0.);
  pole_alu_vol->AddNode(pole_air_vol, 0, pole_air_trans);

  return pole;
}

TGeoVolume* create_tof_bar(Float_t dx, Float_t dy, Float_t dz)
{
  // needed materials
  TGeoMedium* boxVolMed = gGeoMan->GetMedium(BoxVolumeMedium);
  TGeoMedium* airVolMed = gGeoMan->GetMedium(KeepingVolumeMedium);
   
  Float_t width_alux=Pole_Thick_X;
  Float_t width_aluy=Pole_Thick_Y;
  Float_t width_aluz=Pole_Thick_Z;
  
  TGeoVolume* bar = new TGeoVolumeAssembly("Bar");
  TGeoBBox*   bar_alu_box = new TGeoBBox("", dx/2., dy/2., dz/2.);
  TGeoVolume* bar_alu_vol = 
    new TGeoVolume("bar_alu", bar_alu_box, boxVolMed);
  bar_alu_vol->SetLineColor(kGreen); // set line color for the alu box
  bar_alu_vol->SetTransparency(20); // set transparency for the TOF
  TGeoTranslation* bar_alu_trans 
    = new TGeoTranslation("", 0., 0., 0.);
  bar->AddNode(bar_alu_vol, 0, bar_alu_trans);

  TGeoBBox* bar_air_box = new TGeoBBox("", dx/2.-width_alux, dy/2.-width_aluy, dz/2.-width_aluz);
  TGeoVolume* bar_air_vol = 
    new TGeoVolume("bar_air", bar_air_box, airVolMed);
  bar_air_vol->SetLineColor(kYellow); // set line color for the alu box
  bar_air_vol->SetTransparency(70);   // set transparency for the TOF
  TGeoTranslation* bar_air_trans 
    = new TGeoTranslation("", 0., 0., 0.);
  bar_alu_vol->AddNode(bar_air_vol, 0, bar_air_trans);

  return bar;
}

void position_tof_poles(Int_t modType)
{

  TGeoTranslation* pole_trans=NULL;

  Int_t numPoles=0;
  for (Int_t i=0; i<NumberOfPoles; i++){
    if(i<2) {
     pole_trans 
      = new TGeoTranslation("", -Pole_Offset+2.0, 0., Pole_ZPos[i]);
     gGeoMan->GetVolume(geoVersion)->AddNode(gPole, numPoles, pole_trans);
     numPoles++;
    }else{
//     Float_t xPos=Pole_Offset+Pole_Size_X/2.+Pole_Col[i]*DxColl;
     Float_t xPos=Pole_Offset+Pole_Size_X/2.+ DxColl[ (Int_t)Pole_Col[i]-1 ]; // add x offset of this module column
     for(Int_t jj=0; jj<Pole_Col[i]-1; jj++) // add X offset of previous modules columns
        xPos += DxColl[jj];
     Float_t zPos=Pole_ZPos[i];
     pole_trans 
      = new TGeoTranslation("", xPos, 0., zPos);
     gGeoMan->GetVolume(geoVersion)->AddNode(gPole, numPoles, pole_trans);
     numPoles++;

     pole_trans 
      = new TGeoTranslation("", -xPos, 0., zPos);
     gGeoMan->GetVolume(geoVersion)->AddNode(gPole, numPoles, pole_trans);
     numPoles++;
    }
    cout << " Position Pole "<< numPoles<<" at z="<< Pole_ZPos[i] << endl;
  }
}

void position_tof_bars(Int_t modType)
{

  TGeoTranslation* bar_trans=NULL;

  Float_t xPos=0;
  Float_t yPos=0;
  Float_t zPos=0;

  Int_t numBars=0;
  Int_t i=0;
  for (i=0; i<NumberOfBars; i++){

     xPos=Bar_XPos[i];
     zPos=Bar_ZPos[i];
     yPos=Pole_Size_Y/2.+Bar_Size_Y/2.;

     bar_trans = new TGeoTranslation("", xPos, yPos, zPos);
     gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
     numBars++;

     bar_trans = new TGeoTranslation("", xPos,-yPos, zPos);
     gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
     numBars++;

     bar_trans = new TGeoTranslation("", -xPos, yPos, zPos);
     gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
     numBars++;

     bar_trans = new TGeoTranslation("", -xPos, -yPos, zPos);
     gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
     numBars++;

   }
   cout << " Position Bar "<< numBars<<" at z="<< Bar_ZPos[i] << endl;

   // horizontal frame bars 
   i = NumberOfBars;
   NumberOfBars++;
   gBar[i]=create_tof_bar(2.*xPos+Pole_Size_X,Bar_Size_Y,Bar_Size_Y);

   zPos = Pole_ZPos[0]+Pole_Size_Z/2.;
   bar_trans = new TGeoTranslation("", 0., yPos, zPos);
   gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
   numBars++;

   bar_trans = new TGeoTranslation("", 0., -yPos, zPos);
   gGeoMan->GetVolume(geoVersion)->AddNode(gBar[i], numBars, bar_trans);
   numBars++;

}

void position_inner_tof_modules(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=Inner_Module_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = Inner_Module_X_Offset;
 Float_t zPos  = Wall_Z_Position;

 Pole_ZPos[NumberOfPoles] = zPos;
 Pole_Col[NumberOfPoles] = 0;
 NumberOfPoles++;

 Float_t DzPos =0.;
 for (Int_t j=0; j<modNType; j++){
   if (Module_Size_Z[j]>DzPos){
       DzPos = Module_Size_Z[j];
   }
 }
 Pole_ZPos[NumberOfPoles]=zPos+DzPos;
 Pole_Col[NumberOfPoles] = 0;
 NumberOfPoles++;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Inner_Module_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<Inner_Module_Number[j]; i++) { 
    ii++; 

    if( kTRUE )
    {
    cout << "Inner ii "<<ii<<" Last "<<Last_Size_Y<<","<<Last_Over_Y<<endl;
    Float_t DeltaY=Module_Size_Y[modType]+Last_Size_Y-2.*(Module_Over_Y[modType]+Last_Over_Y);
    yPos += DeltaY;
    Last_Size_Y=Module_Size_Y[modType];
    Last_Over_Y=Module_Over_Y[modType];
    cout <<"Position Inner Module "<<i<<" of "<<Inner_Module_Number[j]<<" Type "<<modType
         <<" at Y = "<<yPos<<" Ysize = "<<Module_Size_Y[modType]
	 <<" DeltaY = "<<DeltaY<<endl;

    // Front staggered module (Top if pair), top
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;
    /*
    // Front staggered module (Bottom if pair), bottom
    module_trans 
      = new TGeoTranslation("", xPos, -yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;
    if (ii>0) {
       // Back staggered module (Top if pair), top
      module_trans 
	= new TGeoTranslation("", xPos, yPos-DeltaY/2, zPos+Module_Size_Z[modType]);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;
       // Back staggered module (Bottom if pair), bottom
      module_trans 
	= new TGeoTranslation("", xPos, -(yPos-DeltaY/2), zPos+Module_Size_Z[modType]);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;
    }
    */
    } // if( kTRUE == bNhVersion )
    else
    {
       // Attempt to make automatic angular overlap in Y
       // (xPos, yPos, zPos) is the position of module center
       // xPos and zPos are kept as in NH version
       // yPos is computed from position of last module and Z position of new module

       if (ii>0)
       {
          // Back staggered module = Bottom if pair
          yPos = GetShiftedY( fPrevModPosY, iPrevModType, fPrevModPosZ,
                              modType, zPos+Module_Size_Z[modType],
                               kfAngOverlap);

          cout <<"Position Inner Module "<<i<<" of "<<Inner_Module_Number[j]<<" Type "<<modType
               <<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos+Module_Size_Z[modType]
               <<" Ysize = "<<Module_Size_Y[modType]<<" Back, Diff to last "<<yPos-fPrevModPosY
               <<" Ov. "<<( (Module_Size_Y[iPrevModType]+Module_Size_Y[modType])/2.0 - (yPos-fPrevModPosY))<<endl;

          // Back staggered module, top
         module_trans
            = new TGeoTranslation("", xPos, yPos, zPos+Module_Size_Z[modType]);
         gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum + 2, module_trans);
//         modNum++;
          // Back staggered module, bottom
         module_trans
            = new TGeoTranslation("", xPos, -yPos, zPos+Module_Size_Z[modType]);
         gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum + 3, module_trans);
//         modNum++;
         fPrevModPosY  = yPos;
         iPrevModType  = modType;
         fPrevModPosZ  = zPos+Module_Size_Z[modType];

         // Front staggered module = Top if pair
         yPos = GetShiftedY( fPrevModPosY, iPrevModType, fPrevModPosZ,
                             modType, zPos,
                              kfAngOverlap);
         cout <<"Position Inner Module "<<i<<" of "<<Inner_Module_Number[j]<<" Type "<<modType
              <<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos
              <<" Ysize = "<<Module_Size_Y[modType]<<" Front, Diff to last "<<yPos-fPrevModPosY
              <<" Ov. "<<( (Module_Size_Y[iPrevModType]+Module_Size_Y[modType])/2.0 - (yPos-fPrevModPosY))<<endl;
         // Front staggered module, top
         module_trans
           = new TGeoTranslation("", xPos, yPos, zPos);
         gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
  //       modNum++;
         // Front staggered module, bottom
         module_trans
           = new TGeoTranslation("", xPos, -yPos, zPos);
         gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum + 1, module_trans);
  //       modNum++;
         fPrevModPosY  = yPos;
         iPrevModType  = modType;
         fPrevModPosZ  = zPos;

         // To keep same numbering as NH version
         modNum += 4;
       } // if (ii>0)
          else
          {
             // First layer start in front, not part of a pair
             yPos = GetShiftedY( fPrevModPosY, iPrevModType, fPrevModPosZ,
                                 modType, zPos,
                                  kfAngOverlap);
             cout <<"Position Inner Module "<<i<<" of "<<Inner_Module_Number[j]<<" Type "<<modType
                  <<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos
                  <<" Ysize = "<<Module_Size_Y[modType]<<", Diff to last "<<yPos-fPrevModPosY
                  <<" Ov. "<<( (Module_Size_Y[iPrevModType]+Module_Size_Y[modType])/2.0 - (yPos-fPrevModPosY))<<endl;

             // Front staggered module, top
             module_trans
               = new TGeoTranslation("", xPos, yPos, zPos);
             gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
             modNum++;
             // Front staggered module, bottom
             module_trans
               = new TGeoTranslation("", xPos, -yPos, zPos);
             gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
             modNum++;

             fPrevModPosY  = yPos;
             iPrevModType  = modType;
             fPrevModPosZ  = zPos;
          } // else of if (ii>0)

    } // else of  if( kTRUE == bNhVersion )
  }
 }
}

void position_HDbig(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(HDbig_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=HDbig_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = HDbig_X_Offset;
 Float_t zPos  = HDbig_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= HDbig_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<HDbig_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_HDsmall(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(HDsmall_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=HDsmall_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = HDsmall_X_Offset;
 Float_t zPos  = HDsmall_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= HDsmall_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<HDsmall_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_BucRef(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(BucRef_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=BucRef_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = BucRef_X_Offset;
 Float_t zPos  = BucRef_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= BucRef_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<BucRef_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_HDRef(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(HDRef_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=HDRef_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = HDRef_X_Offset;
 Float_t zPos  = HDRef_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= HDRef_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<HDRef_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_Dia(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(Dia_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=Dia_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = Dia_X_Offset;
 Float_t zPos  = Dia_Z_Position;

 Int_t modNum = 0;
 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Dia_Types[j];
  for(Int_t i=0; i<Dia_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_Buc2013(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(Buc2013_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=Buc2013_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = Buc2013_X_Offset;
 Float_t zPos  = Buc2013_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Buc2013_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<Buc2013_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_USTC(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(USTC_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=USTC_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = USTC_X_Offset;
 Float_t zPos  = USTC_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= USTC_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<USTC_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_pla_modules(Int_t modNType)
{

 Int_t ii=0; 
 Int_t modNum = 0;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Pla_Types[j];
  Float_t xPos  = Pla_X_Position[j];
  Float_t yPos  = Pla_Y_Position[j];
  Float_t zPos  = Pla_Z_Position[j];
  TGeoTranslation* module_trans=NULL;
  TGeoRotation* module_rot = new TGeoRotation();
  module_rot->RotateZ(Pla_rotate_Z[j]);
  TGeoCombiTrans* module_combi_trans = NULL;

  for(Int_t i=0; i<Pla_Number[j]; i++) { 
    ii++; 

    cout <<"Position Pla Module "<<i<<" of "<<Pla_Number[j]<<" Type "<<modType
         <<" at X = "<<xPos
         <<", Y = "<<yPos
         <<", Z = "<<zPos
	 <<endl;
    // Front staggered module (Top if pair), top
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_side_tof_modules(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(180.);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t xPos=0.;
 Float_t yPos=0.; //Inner_Module_First_Y_Position;
 Float_t zPos=0.;

 Int_t ii=0; 
 for (Int_t j=0; j<modNType; j++){
  Int_t modType= InnerSide_Module_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<InnerSide_Module_Number[j]; i++) { 
    ii++; 

    if( kTRUE == bNhVersion )
    {
    cout << "InnerSide ii "<<ii<<" Last "<<Last_Size_Y<<","<<Last_Over_Y<<endl;
    Float_t DeltaY=Module_Size_Y[modType]+Last_Size_Y-2.*(Module_Over_Y[modType]+Last_Over_Y);
    if (ii>1){yPos += DeltaY;}
    Last_Size_Y=Module_Size_Y[modType];
    Last_Over_Y=Module_Over_Y[modType];
    xPos  = InnerSide_Module_X_Offset;
    zPos  = Wall_Z_Position;
    cout <<"Position InnerSide Module "<<i<<" of "<<InnerSide_Module_Number[j]<<" Type "<<modType
         <<" at Y = "<<yPos<<" Ysize = "<<Module_Size_Y[modType]
	 <<" DeltaY = "<<DeltaY<<endl;

    // Front staggered module, center or top if pair, right
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;

    // Front staggered module, center or top if pair, left
    module_trans 
      = new TGeoTranslation("", -xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;

    if (ii>1) {
    // Front staggered module top in pair, bottom right
    module_trans 
      = new TGeoTranslation("", xPos, -yPos, zPos);
     gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;

    // Front staggered module top in pair, bottom left
    module_trans 
      = new TGeoTranslation("", -xPos, -yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;

    // Back staggered module bottom in pair, top right
    module_trans 
      = new TGeoTranslation("", xPos, yPos-DeltaY/2, zPos+Module_Size_Z[modType]);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;

    // Back staggered module bottom in pair, top left
    module_trans 
      = new TGeoTranslation("", -xPos, yPos-DeltaY/2, zPos+Module_Size_Z[modType]);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;

    // Back staggered module bottom in pair, bottom right
    module_trans 
      = new TGeoTranslation("", xPos, -(yPos-DeltaY/2), zPos+Module_Size_Z[modType]);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
    modNum++;

    // Back staggered module bottom in pair, bottom left
    module_trans 
     = new TGeoTranslation("", -xPos,-(yPos-DeltaY/2), zPos+Module_Size_Z[modType]);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;

    }
    } // if( kTRUE == bNhVersion )
    else
    {
       // Attempt to make automatic angular overlap in Y
       // (xPos, yPos, zPos) is the position of module center
       // xPos and zPos are kept as in NH version
       // yPos is computed from position of last module and Z position of new module

       if (ii>1)
       {
          // Back staggered module bottom in pair, top right
          yPos = GetShiftedY( fPrevModPosY, iPrevModType, fPrevModPosZ,
                              modType, zPos+Module_Size_Z[modType],
                               kfAngOverlap);
          cout <<"Position InnerSide Module "<<i<<" of "<<InnerSide_Module_Number[j]<<" Type "<<modType
               <<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos+Module_Size_Z[modType]
               <<" Ysize = "<<Module_Size_Y[modType]<<" Back, Diff to last "<<yPos-fPrevModPosY
               <<" Ov. "<<( (Module_Size_Y[iPrevModType]+Module_Size_Y[modType])/2.0 - (yPos-fPrevModPosY))<<endl;
          module_trans
            = new TGeoTranslation("", xPos, yPos, zPos+Module_Size_Z[modType]);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum+4, module_trans);
//          modNum++;

          // Back staggered module bottom in pair, top left
          module_trans
            = new TGeoTranslation("", -xPos, yPos, zPos+Module_Size_Z[modType]);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum+5, module_combi_trans);
//          modNum++;

          // Back staggered module bottom in pair, bottom right
          module_trans
            = new TGeoTranslation("", xPos, -yPos, zPos+Module_Size_Z[modType]);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum+6, module_trans);
//          modNum++;

          // Back staggered module bottom in pair, bottom left
          module_trans
           = new TGeoTranslation("", -xPos,-yPos, zPos+Module_Size_Z[modType]);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum+7, module_combi_trans);
//          modNum++;
          fPrevModPosY  = yPos;
          iPrevModType  = modType;
          fPrevModPosZ  = zPos+Module_Size_Z[modType];

          // Front staggered module, top in pair, top right
          yPos = GetShiftedY( fPrevModPosY, iPrevModType, fPrevModPosZ,
                              modType, zPos,
                               kfAngOverlap);
          cout <<"Position InnerSide Module "<<i<<" of "<<InnerSide_Module_Number[j]<<" Type "<<modType
               <<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos
               <<" Ysize = "<<Module_Size_Y[modType]<<" Front, Diff to last "<<yPos-fPrevModPosY
               <<" Ov. "<<( (Module_Size_Y[iPrevModType]+Module_Size_Y[modType])/2.0 - (yPos-fPrevModPosY))<<endl;
          module_trans
            = new TGeoTranslation("", xPos, yPos, zPos);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
//          modNum++;

          // Front staggered module, top in pair, top left
          module_trans
            = new TGeoTranslation("", -xPos, yPos, zPos);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum+1, module_combi_trans);
//          modNum++;

          // Front staggered module top in pair, bottom right
          module_trans
            = new TGeoTranslation("", xPos, -yPos, zPos);
           gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum+2, module_trans);
//          modNum++;

          // Front staggered module top in pair, bottom left
          module_trans
            = new TGeoTranslation("", -xPos, -yPos, zPos);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum+3, module_combi_trans);
//          modNum++;
          fPrevModPosY  = yPos;
          iPrevModType  = modType;
          fPrevModPosZ  = zPos;

          modNum += 8;
       } // if (ii>1)
       else
       {
          // First layer start in front, not part of a pair
          // This one get the same shift as in NH version
          cout << "InnerSide ii "<<ii<<" Last "<<Last_Size_Y<<","<<Last_Over_Y<<endl;
          Float_t DeltaY=Module_Size_Y[modType]+Last_Size_Y-2.*(Module_Over_Y[modType]+Last_Over_Y);

          Last_Size_Y=Module_Size_Y[modType];
          Last_Over_Y=Module_Over_Y[modType];
          Float_t xPos  = InnerSide_Module_X_Offset;
          Float_t zPos  = Wall_Z_Position;
          cout <<"Position InnerSide Module "<<i<<" of "<<InnerSide_Module_Number[j]<<" Type "<<modType
               <<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos<<" Ysize = "<<Module_Size_Y[modType]
          <<" DeltaY = "<<DeltaY<<endl;

          // Front staggered module, center, right
          module_trans
            = new TGeoTranslation("", xPos, yPos, zPos);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_trans);
          modNum++;

          // Front staggered module, center, left
          module_trans
            = new TGeoTranslation("", -xPos, yPos, zPos);
          module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
          gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
          modNum++;

          fPrevModPosY  = yPos;
          iPrevModType  = modType;
          fPrevModPosZ  = zPos;
       } // else of if (ii>1)
    } // else of  if( kTRUE == bNhVersion )
  }
 }
}

void position_outer_tof_modules(Int_t nCol) //modType, Int_t col1, Int_t col2)
{
  TGeoTranslation* module_trans=NULL;
  TGeoRotation* module_rot = new TGeoRotation();
  module_rot->RotateZ(180.);
  TGeoCombiTrans* module_combi_trans = NULL;

  //  Int_t numModules=(Int_t)( (Outer_Module_Last_Y_Position-Outer_Module_First_Y_Position)/Module_Size_Y[modType])+1;
  
  Int_t modNum[NofModuleTypes];
  for (Int_t k=0; k<NofModuleTypes; k++){
     modNum[k]=0;
  }

  Float_t zPos = Wall_Z_Position;
  for(Int_t j=0; j<nCol; j++){ 
//   Float_t xPos = Outer_Module_X_Offset + ((j+1)*DxColl);
   Float_t xPos = Outer_Module_X_Offset + DxColl[j]; // // add x offset of this module column
   for(Int_t jj=0; jj<j; jj++) // add X offset of previous modules columns
      xPos += DxColl[jj];
   Last_Size_Y=0.;
   Last_Over_Y=0.;
   Float_t yPos = 0.;
   Int_t   ii=0; 
   Float_t DzPos =0.;
   for(Int_t k=0; k<Outer_Module_NTypes; k++){
    Int_t modType= Outer_Module_Types[k][j];
    if(Module_Size_Z[modType]>DzPos){
      if(Outer_Module_Number[k][j]>0){
       DzPos = Module_Size_Z[modType];
      }
     }
   }   

   zPos -= 2.*DzPos; //((j+1)*2*Module_Size_Z[modType]);
   /*
   Pole_ZPos[NumberOfPoles] = zPos;
   Pole_Col[NumberOfPoles] = j+1;
   NumberOfPoles++;
   Pole_ZPos[NumberOfPoles] = zPos+DzPos;
   Pole_Col[NumberOfPoles] = j+1;
   NumberOfPoles++;
   //if (j+1==nCol) {
   if (1) {
    Pole_ZPos[NumberOfPoles] = Pole_ZPos[0];
    Pole_Col[NumberOfPoles] = j+1;
    NumberOfPoles++;

    Bar_Size_Z = Pole_ZPos[0] - zPos;
    gBar[NumberOfBars] = create_tof_bar(Bar_Size_X, Bar_Size_Y, Bar_Size_Z);
    Bar_ZPos[NumberOfBars] = zPos+Bar_Size_Z/2.-Pole_Size_Z/2.;
    Bar_XPos[NumberOfBars] = xPos + Pole_Offset;
    NumberOfBars++;
   }
   */
   for (Int_t k=0; k<Outer_Module_NTypes; k++) {
    Int_t modType    = Outer_Module_Types[k][j];
    Int_t numModules = Outer_Module_Number[k][j];

    cout <<" Outer: position "<<numModules<<" of type "<<modType<<" in col "<<j
         <<" at z = "<<zPos<<", DzPos = "<<DzPos<<endl;    
    for(Int_t i=0; i<numModules; i++) {  
      ii++; 
      if( kTRUE )
      {
      cout << "Outer ii "<<ii<<" Last "<<Last_Size_Y<<","<<Last_Over_Y<<endl;
      Float_t DeltaY=Module_Size_Y[modType]+Last_Size_Y-2.*(Module_Over_Y[modType]+Last_Over_Y);      
      if (ii>1){yPos += DeltaY;}
      Last_Size_Y=Module_Size_Y[modType];
      Last_Over_Y=Module_Over_Y[modType];
      cout <<"Position Outer Module "<<i<<" of "<<Outer_Module_Number[k][j]<<" Type "<<modType
           <<"(#"<<modNum[modType]<<") "<<" at Y = "<<yPos<<" Ysize = "<<Module_Size_Y[modType]
  	   <<" DeltaY = "<<DeltaY<<endl;

      // Central module, right or Front staggered module, top right
      module_trans = new TGeoTranslation("", xPos, yPos, zPos);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_trans);
      modNum[modType]++;

      // Central module, left or Front staggered module, top left
      module_trans = new TGeoTranslation("", -xPos, yPos, zPos);
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
      modNum[modType]++;

      if (ii>1) {
         // Front staggered module, bottom right
	module_trans 
	  = new TGeoTranslation("", xPos, -yPos, zPos);
	gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_trans);
	modNum[modType]++;
         // Front staggered module, bottom left
	module_trans 
	  = new TGeoTranslation("", -xPos, -yPos, zPos);
	module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
	gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
	modNum[modType]++;

	// second layer
      // Back staggered module, top right
	module_trans 
	  = new TGeoTranslation("", xPos, yPos-DeltaY/2., zPos+DzPos);
	gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_trans);
	modNum[modType]++;
      // Back staggered module, top left
	module_trans 
	  = new TGeoTranslation("", -xPos, yPos-DeltaY/2., zPos+DzPos);
	module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
	gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
	modNum[modType]++;

      // Back staggered module, bottom right
	module_trans 
	  = new TGeoTranslation("", xPos, -(yPos-DeltaY/2.), zPos+DzPos);
	gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_trans);
	modNum[modType]++;
      // Back staggered module, bottom left
	module_trans 
	  = new TGeoTranslation("", -xPos, -(yPos-DeltaY/2.), zPos+DzPos);
	module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
	gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
	modNum[modType]++;

      }
      } // if( kTRUE == bNhVersion )
      else
      {
         // Attempt to make automatic angular overlap in Y
         // (xPos, yPos, zPos) is the position of module center
         // xPos and zPos are kept as in NH version
         // yPos is computed from position of last module and Z position of new module
         if (ii>1)
         {
            // Back layer
            yPos = GetShiftedY( fPrevModPosY, iPrevModType, fPrevModPosZ,
                                modType, zPos+DzPos,
                                 kfAngOverlap);
            cout <<"Position Outer Module Module "<<i<<" of "<<Outer_Module_Number[k][j]<<" Type "<<modType
                  <<"(#"<<modNum[modType]<<") "<<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos+DzPos
                 <<" Ysize = "<<Module_Size_Y[modType]<<" Back, Diff to last "<<yPos-fPrevModPosY
                 <<" Ov. "<<( (Module_Size_Y[iPrevModType]+Module_Size_Y[modType])/2.0 - (yPos-fPrevModPosY))<<endl;
               // Back staggered module, top right, bottom in staggering pair
            module_trans
              = new TGeoTranslation("", xPos, yPos, zPos+DzPos);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType]+4, module_trans);
//            modNum[modType]++;

               // Back staggered module, top left, bottom in staggering pair
            module_trans
              = new TGeoTranslation("", -xPos, yPos, zPos+DzPos);
            module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType]+5, module_combi_trans);
//            modNum[modType]++;

               // Back staggered module, bottom right, bottom in staggering pair
            module_trans
              = new TGeoTranslation("", xPos, -yPos, zPos+DzPos);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType]+6, module_trans);
//            modNum[modType]++;

               // Back staggered module, bottom left, bottom in staggering pair
            module_trans
              = new TGeoTranslation("", -xPos, -yPos, zPos+DzPos);
            module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType]+7, module_combi_trans);
//            modNum[modType]++;
            fPrevModPosY  = yPos;
            iPrevModType  = modType;
            fPrevModPosZ  = zPos+DzPos;

            // Front layer
            yPos = GetShiftedY( fPrevModPosY, iPrevModType, fPrevModPosZ,
                                modType, zPos,
                                kfAngOverlap);
            cout <<"Position Outer Module Module "<<i<<" of "<<Outer_Module_Number[k][j]<<" Type "<<modType
                  <<"(#"<<modNum[modType]<<") "<<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos
                 <<" Ysize = "<<Module_Size_Y[modType]<<" Front, Diff to last "<<yPos-fPrevModPosY
                 <<" Ov. "<<( (Module_Size_Y[iPrevModType]+Module_Size_Y[modType])/2.0 - (yPos-fPrevModPosY))<<endl;
            // Front staggered module, top right, Top in staggering pair
            module_trans = new TGeoTranslation("", xPos, yPos, zPos);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_trans);
//            modNum[modType]++;

            // Front staggered module, top left, Top in staggering pair
            module_trans = new TGeoTranslation("", -xPos, yPos, zPos);
            module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType]+1, module_combi_trans);
//            modNum[modType]++;

            // Front staggered module, bottom right, Top in staggering pair
            module_trans
              = new TGeoTranslation("", xPos, -yPos, zPos);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType]+2, module_trans);
//            modNum[modType]++;

            // Front staggered module, bottom left, Top in staggering pair
            module_trans
              = new TGeoTranslation("", -xPos, -yPos, zPos);
            module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType]+3, module_combi_trans);
//            modNum[modType]++;
            fPrevModPosY  = yPos;
            iPrevModType  = modType;
            fPrevModPosZ  = zPos+DzPos;

            modNum[modType] += 8;
         } // if (ii>1)
         else
         {
            // First layer start in front, not part of a pair
            // This one get the same shift as in NH version
            cout << "Outer ii "<<ii<<" Last "<<Last_Size_Y<<","<<Last_Over_Y<<endl;
            Float_t DeltaY=Module_Size_Y[modType]+Last_Size_Y-2.*(Module_Over_Y[modType]+Last_Over_Y);

            Last_Size_Y=Module_Size_Y[modType];
            Last_Over_Y=Module_Over_Y[modType];
            cout <<"Position Outer Module "<<i<<" of "<<Outer_Module_Number[k][j]<<" Type "<<modType
                 <<"(#"<<modNum[modType]<<") "<<" at X = "<<xPos<<" Y = "<<yPos<<" Z = "<<zPos
                 <<" Ysize = "<<Module_Size_Y[modType]
                 <<" DeltaY = "<<DeltaY<<endl;

            // Central module, right or Front staggered module, top right
            module_trans = new TGeoTranslation("", xPos, yPos, zPos);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_trans);
            modNum[modType]++;

            // Central module, left or Front staggered module, top left
            module_trans = new TGeoTranslation("", -xPos, yPos, zPos);
            module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
            gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum[modType], module_combi_trans);
            modNum[modType]++;

            fPrevModPosY  = yPos;
            iPrevModType = modType;
            fPrevModPosZ  = zPos;
         } // else of if (ii>1)
      } // else of  if( kTRUE == bNhVersion )
    }
   }
  }
}

// Return Y position of new module to have an angular overlap
// of fAngOverlap degrees, assuming we are in the top part of the
// wall and going upward
Float_t GetShiftedY( Double_t dPrevModPosY, Int_t iPrevModType, Double_t dPrevModPosZ,
                       Int_t iModType, Double_t dModPosZ,
                       Double_t dAngOverlap)
{


   Double_t dPrevModSizeY = Module_Size_Y[iPrevModType];
   Double_t dPrevModDeadY = Module_Dead_Y[iPrevModType];
   Double_t dModSizeY     = Module_Size_Y[iModType];
   Double_t dModDeadY     = Module_Dead_Y[iModType];
   // Eq: Y = Z*Tan( Atan( PrevY/PrevZ ) - ThetaOverlap )
   // Compute angular position of the previous module
   // Add only half of module size as the Y position is the one
   // of the module center
   Double_t dModThetaY = TMath::ATan2( (dPrevModPosY + dPrevModSizeY/2.0 - dPrevModDeadY/2.0),
                                       (dPrevModPosZ + dRefOffsetZ) );
   // Now get angular position of the module by applying the offset
   dModThetaY -= TMath::Pi()*dAngOverlap/180.0;

   // Convert to cartesian coordinate and return the value
   Double_t dModPosY = (dModPosZ + dRefOffsetZ)* TMath::Tan( dModThetaY );
   // Need to add half the size of the module as the positionning is done
   // at module center
   dModPosY += dModSizeY/2.0 - dModDeadY/2.0;
   return dModPosY;
}
