// Macro creating a ToF geometry for test beam: v16b - Cern Nov 2016, initial version 
//
// Usage:
//

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
const TString DiamondMedium           = "silicon";  // to get active material

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
// 4 Plastic
// 5 Diamond 
// 6 Buc 2015a
// 7 Buc 2015b
// 8 Ceramic Pad 
// 9 THU strip 
//
const Int_t NumberOfDifferentCounterTypes = 11;
const Float_t Glass_X[NumberOfDifferentCounterTypes] = {32.,  52., 15.04, 18.,  2.,   0.2,  40.4, 28.8, 17.4, 25., 32.};
const Float_t Glass_Y[NumberOfDifferentCounterTypes] = {26.9, 53.,  4.,    4.6, 10.,  0.2,  9.6,   9.6,  2.0, 26.7, 27.};
const Float_t Glass_Z[NumberOfDifferentCounterTypes] = {0.1,   0.1, 0.1,   0.1, 0.01, 0.1,  0.05,  0.05, 0.07, 0.1, 0.028};

const Float_t GasGap_X[NumberOfDifferentCounterTypes] = {32., 52., 15.04, 18.,    2.,  0.2,  40.4, 28.8,  17.4, 25., 32.};
const Float_t GasGap_Y[NumberOfDifferentCounterTypes] = {26.9,53.,  4.,    4.6,  10.,  0.2, 9.6,  9.6,   2.0, 26.7, 27.};
const Float_t GasGap_Z[NumberOfDifferentCounterTypes] = {0.022,0.022,0.022,0.014, 2.,  0.1, 0.014,0.014, 0.025, 0.025, 0.014};

const Int_t NumberOfGaps[NumberOfDifferentCounterTypes] = {8,8,6,10,1,1,10,8,10,10,12};
const Int_t NumberOfReadoutStrips[NumberOfDifferentCounterTypes] = {32,56,16,72,1,16,40,40,8,24,32};

const Float_t SingleStackStartPosition_Z[NumberOfDifferentCounterTypes] = {-0.6,-0.6,-0.6,-0.6,-0.1,-0.1,-0.6,-0.6,-1.,-0.6, -0.6};

const Float_t Electronics_X[NumberOfDifferentCounterTypes] = {34.0,53.0,15.5, 18.5, 2.,  0.3, 30.5, 30.5,  0.2, 26., 34.};
const Float_t Electronics_Y[NumberOfDifferentCounterTypes] = { 5.0, 5.0, 1.0,  1.,  2.,  0.1, 0.5,  0.5,  0.2,  5., 5.0};
const Float_t Electronics_Z[NumberOfDifferentCounterTypes] = { 0.3, 0.3, 0.3,  0.3, 0.3, 0.1, 0.3,  0.3, 0.3,  0.3, 0.3};

const Int_t NofModuleTypes = 10;
// Aluminum box for all supermodule types
// 0 HDbig
// 1 BucRef
// 2 Plastic
// 3 HD small (P2) 
// 4 HD ref   (P5)
// 5 Diamond
// 6 Buc 2015
// 7 Buc 2012
// 8 Ceramic
// 9 Star 2-counter module 
const Float_t Module_Size_X[NofModuleTypes] = {120.,35., 5., 50., 30., 8., 50., 70., 29., 102.};
const Float_t Module_Size_Y[NofModuleTypes] = { 74.,18.,20., 49., 16., 5., 30., 35., 13.,  49.};
const Float_t Module_Over_Y[NofModuleTypes] = { 11.,11., 0.,  4.,  0., 0.,  0.,  0.,  0.,   0.};
const Float_t Module_Size_Z[NofModuleTypes] = { 10.,10., 5., 10., 10., 1., 15., 20., 10.,  11.};
const Float_t Module_Thick_Alu_X_left = 1.;
const Float_t Module_Thick_Alu_X_right = 0.1;
const Float_t Module_Thick_Alu_Y = 0.1;
const Float_t Module_Thick_Alu_Z = 0.1;

//Type of Counter for module 
const Int_t CounterTypeInModule[NofModuleTypes] = {1,3,4,0,2,5,6,7,8,10};
const Int_t NCounterInModule[NofModuleTypes]    = {1,1,1,1,1,1,2,4,1,2};

// Placement of the counter inside the module
const Float_t CounterXStartPosition[NofModuleTypes] = {-10.0,   0.0,   2.0,  0.0,  0.0,  0., 0., -15., 0.,  25.1};
const Float_t CounterXDistance[NofModuleTypes]      = {30.0,   30.6,   0.0, 30.0,  0.0,  0.,-6.4, 30., 0.,  0.};
const Float_t CounterYStartPosition[NofModuleTypes] = {0.0,     0.0,   0.0,  0.0,  0.0,  0., 0., -4.,  0.,  0.};
const Float_t CounterYDistance[NofModuleTypes]      = {0.0,     0.0,   0.0,  0.0,  0.0,  0., 0.,  8.,  0., 0.};
const Float_t CounterZStartPosition[NofModuleTypes] = {0.0,     0.0,   0.0,  2.5,  0.0,  0.,-2., -8.,  0., -2.};
const Float_t CounterZDistance[NofModuleTypes]      = {0.0,     0.0,   0.0,  2.5,  0.0,  0., 4.,  5.,  4.,  4.};
const Float_t CounterRotationAngle[NofModuleTypes]  = {0.,      0.0,   0.0,   0.,  0.0,  0., 0.,  0.,  0.,  0.};

const Int_t Module_NTypes = 1;
const Float_t Module_Types[Module_NTypes]  = {9.};  // Module Type specifier
const Float_t Module_Number[Module_NTypes] = {36.}; //debugging,
const Double_t RING_DelPhi = 30.;
const Double_t Deg2Rad = 0.017453293;


const Float_t ZDist2Target=353.;
const Float_t MeanTheta=-7.5;
Float_t TanTheta=TMath::Tan(TMath::DegToRad()*MeanTheta);

const Float_t HDbig_Z_Position=222.;
const Float_t HDbig_First_Y_Position=-86.;
const Float_t HDbig_X_Offset=-1.5; //3.95;
const Float_t HDbig_rotate_Z=-90.;
const Int_t HDbig_NTypes = 1;
const Float_t HDbig_Types[HDbig_NTypes]  = {0.};
const Float_t HDbig_Number[HDbig_NTypes] = {1.}; //debugging, V14c

const Float_t HDsmall_Z_Position=445.5;     // HD - P2
const Float_t HDsmall_First_Y_Position=51;  // HD - P2
const Float_t HDsmall_X_Offset=4.5;
const Float_t HDsmall_rotate_Z=-90.;
const Int_t HDsmall_NTypes = 1;
const Float_t HDsmall_Types[HDsmall_NTypes]  = {3.};
const Float_t HDsmall_Number[HDsmall_NTypes] = {1.}; //debugging, V14b

const Float_t HDRef_Z_Position=ZDist2Target+7;        // HD - P5
const Float_t HDRef_First_Y_Position=HDRef_Z_Position*TanTheta;
const Float_t HDRef_X_Offset=0.7;
const Float_t HDRef_rotate_Z=180.;
const Int_t HDRef_NTypes = 1;
const Float_t HDRef_Types[HDRef_NTypes]  = {4.}; // this is the SmType!
const Float_t HDRef_Number[HDRef_NTypes] = {1.}; // evtl. double for split signals 

const Float_t Dia_Z_Position=0.1;
const Float_t Dia_DeltaZ_Position=-4.;
const Float_t Dia_First_Y_Position=0.;
const Float_t Dia_X_Offset=2.;
const Float_t Dia_rotate_Z=0.;
const Int_t Dia_NTypes = 3;
const Float_t Dia_Types[Dia_NTypes]  = {5.,5.,5.};
const Float_t Dia_Number[Dia_NTypes] = {1.,1.,1.};

const Float_t BucRef_Z_Position=-100.;
const Float_t BucRef_First_Y_Position=-28.;
const Float_t BucRef_X_Offset=-5.;
const Float_t BucRef_rotate_Z=180.;
const Int_t BucRef_NTypes = 1;
const Float_t BucRef_Types[BucRef_NTypes]  = {1.};
const Float_t BucRef_Number[BucRef_NTypes] = {1.}; //debugging, V14b

const Float_t Buc2012_Z_Position=461.;
const Float_t Buc2012_First_Y_Position=-24.2;//-26.;
const Float_t Buc2012_X_Offset=4.2; 
const Float_t Buc2012_rotate_Z=0.;
const Int_t Buc2012_NTypes = 1;
const Float_t Buc2012_Types[Buc2012_NTypes]  = {7.};
const Float_t Buc2012_Number[Buc2012_NTypes] = {1.};

const Float_t Buc2015_Z_Position=ZDist2Target+125.;
const Float_t Buc2015_First_Y_Position=Buc2015_Z_Position*TanTheta - 3.;
const Float_t Buc2015_X_Offset=5.5; 
const Float_t Buc2015_rotate_Z=0.;
const Int_t Buc2015_NTypes = 1;
const Float_t Buc2015_Types[Buc2015_NTypes]  = {6.};
const Float_t Buc2015_Number[Buc2015_NTypes] = {1.};

const Float_t USTC_Z_Position=332.4;
const Float_t USTC_First_Y_Position=61.0;
const Float_t USTC_X_Offset=0.;
const Float_t USTC_rotate_Z=90.;
const Int_t USTC_NTypes = 1;
const Float_t USTC_Types[USTC_NTypes]  = {7.};
const Float_t USTC_Number[USTC_NTypes] = {1.};

const Float_t THU_Z_Position=360.;
const Float_t THU_First_Y_Position=-19.0;
const Float_t THU_X_Offset=0.;
const Float_t THU_rotate_Z=90.;
const Int_t THU_NTypes = 1;
const Float_t THU_Types[THU_NTypes]  = {8.};
const Float_t THU_Number[THU_NTypes] = {1.};


//const Float_t Star2_First_Z_Position=423.6;
//const Float_t Star2_Delta_Z_Position=-23.8;
const Float_t Module_X_Position = 156.7;
const Float_t Module_Y_Position = 0.;
const Float_t Module_Z_Position = 300;
//const Float_t Star2_First_Z_Position=ZDist2Target+33.;
//const Float_t Star2_Delta_Z_Position=29.0;
//const Float_t Star2_First_Y_Position=Star2_First_Z_Position*TanTheta;  // 
//const Float_t Star2_Delta_Y_Position=Star2_Delta_Z_Position*TanTheta;   // 
const Float_t Star2_rotate_Z=0.;
const Int_t Star2_NTypes = 3;
const Float_t Star2_Types[Star2_NTypes]  = {9.,9.,9.};
const Float_t Star2_Number[Star2_NTypes] = {1.,1.,1.}; //debugging, V16b
const Float_t Star2_X_Offset[Star2_NTypes]={2.,0.,0.};
const Float_t Ring_dX_Position[3] = {0., 6.5, 16.5};
const Float_t Ring_dY_Position[3] = {0., 27.5, -28.1};
const Float_t Ring_dZ_Position[3] = {0., 12.0, 24.0};
const Float_t Ring_dPhi_Position[3] = {0., 9.42, -9.167};


const Int_t Pla_NTypes = 2;
const Float_t Pla_Z_Position[Pla_NTypes]={(float)ZDist2Target,(float)(ZDist2Target+179.5)};
const Float_t Pla_X_Position[Pla_NTypes]={-3.0,-0.5};
const Float_t Pla_Y_Position[Pla_NTypes]={(Float_t)(ZDist2Target*TanTheta),(Float_t)((ZDist2Target+179.5)*TanTheta)};
const Float_t Pla_rotate_Z[Pla_NTypes]={90.,90.};
const Float_t Pla_Types[Pla_NTypes]  = {2.,2.};
const Float_t Pla_Number[Pla_NTypes] = {1.,1.};   //V16b

const Int_t Cer_NTypes = 3;
const Float_t Cer_Z_Position[Cer_NTypes]={static_cast<Float_t>(ZDist2Target+164.),static_cast<Float_t>(ZDist2Target+190.5),static_cast<Float_t>(ZDist2Target+210.5)};
const Float_t Cer_X_Position[Cer_NTypes]={0.,0.,0.};
const Float_t Cer_Y_Position[Cer_NTypes]={static_cast<Float_t>((ZDist2Target+164.)*TanTheta),static_cast<Float_t>((ZDist2Target+190.5)*TanTheta),static_cast<Float_t>((ZDist2Target+210.5)*TanTheta)};
const Float_t Cer_rotate_Z[Cer_NTypes]={0.,0.,0.};
const Float_t Cer_Types[Cer_NTypes]  = {8.,8.,8.};
const Float_t Cer_Number[Cer_NTypes] = {1.,1.,1.};   //V16b

// some global variables
TGeoManager* gGeoMan = NULL;  // Pointer to TGeoManager instance
TGeoVolume* gModules[NofModuleTypes]; // Global storage for module types
TGeoVolume* gCounter[NumberOfDifferentCounterTypes];


// Forward declarations
void create_materials_from_media_file();
TGeoVolume* create_new_counter(Int_t);
TGeoVolume* create_tof_module(Int_t);
TGeoVolume* create_new_tof_module(Int_t);

void position_pla_modules(Int_t); 
void position_cer_modules(Int_t); 
void position_HDbig(Int_t);
void position_HDsmall(Int_t);
void position_BucRef(Int_t);
void position_HDRef(Int_t);
void position_Dia(Int_t);
void position_Buc2012(Int_t);
void position_Buc2015(Int_t);
void position_USTC(Int_t);
void position_THU(Int_t);
void position_Star2(Int_t);

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

void Create_TOF_Geometry_vSTAR17a() {
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
    cout << "Create Module "<<moduleType<<endl;
    gModules[moduleType] = create_new_tof_module(moduleType);  // create modules 
    gModules[moduleType]->SetVisContainers(1); 
  }  

  
  position_Dia(1);
  //  position_HDsmall(1);
  //position_HDRef(1);
  position_Star2(1);
  //  position_BucRef(1);
  //position_Buc2015(1);
  //position_Buc2012(1);
  //position_THU(1);
  //position_pla_modules(2);
  //position_cer_modules(3);
  

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
  FairGeoMedium* silicon          = geoMedia->getMedium("silicon");

  // include check if all media are found

  geoBuild->createMedium(air);
  geoBuild->createMedium(aluminium);
  geoBuild->createMedium(RPCgas);
  geoBuild->createMedium(RPCgas_noact);
  geoBuild->createMedium(RPCglass);
  geoBuild->createMedium(carbon);
  geoBuild->createMedium(silicon);
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
  TGeoMedium* DiamondVolMed      = gGeoMan->GetMedium(DiamondMedium);

  if(NULL == DiamondVolMed) cout << "<E> DiamondMedium "<<DiamondMedium<<" not found " << endl;

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
  glass_plate_vol->SetLineColor(kYellow); // set line color for the glass plate
  glass_plate_vol->SetTransparency(98); // set transparency for the TOF
  // define single gas gap volume
  TGeoBBox* gas_gap = new TGeoBBox("", ggdx/2., ggdy/2., ggdz/2.);
  TGeoVolume* gas_gap_vol=NULL;
  if (modType == 5) {
    gas_gap_vol = new TGeoVolume("Gap", gas_gap, DiamondVolMed);
  }else{
    gas_gap_vol = new TGeoVolume("Gap", gas_gap, activeGasVolMed);
  }
  gas_gap_vol->Divide("Cell",1,nstrips,-ggdx/2.,0);
  gas_gap_vol->SetLineColor(kYellow); // set line color for the gas gap
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
  gas_box_vol->SetTransparency(99); // set transparency for the TOF
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
  Float_t dypos=CounterYDistance[modType];
  Float_t startypos=CounterYStartPosition[modType];
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
    Float_t xpos,ypos,zpos;
    if (0 == modType || 3 == modType || 4 == modType || 5 == modType) {
      zpos = dzoff *=-1;
    } else {
      zpos = CounterZStartPosition[modType]+j*dzoff;
    }
    if(8 == modType) { // ITEP Pad
      ;
    }
    xpos=startxpos + j*dxpos;
    ypos=startypos + j*dypos;
    if(6 == modType) {  //Buc2015a/b
      if(j==1) cType=7; //second guy is a
    }
    if(7 == modType) {  //Buc2012
      Int_t zsh[4]={0,2,1,3};
      xpos = startxpos+Float_t((Int_t)j/2)*dxpos;
      ypos = startypos+Float_t((Int_t)j%2)*dypos;
      zpos=CounterZStartPosition[modType]+zsh[j]*dzoff;
    }
    cout << "Add new counter "<<cType<<" at "<<startxpos+j*dxpos<<", "<<startypos+j*dypos<<", "<<zpos<<endl;

    TGeoTranslation* counter_trans 
      = new TGeoTranslation("", xpos, ypos, zpos);
    TGeoRotation* counter_rot = new TGeoRotation();
    counter_rot->RotateY(rotangle);
    TGeoCombiTrans* counter_combi_trans = new TGeoCombiTrans(*counter_trans, *counter_rot);
    gas_box_vol->AddNode(gCounter[cType], j, counter_combi_trans);
  }

  return module;
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

void position_Star2(Int_t modNType)
{
  /* //used @ CERN
TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation("Star2",Star2_rotate_Z,-MeanTheta,0.);
 // module_rot->RotateZ(Star2_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 Float_t yPos  = Star2_First_Y_Position;
 Float_t zPos  = Star2_First_Z_Position;
 Int_t ii=0; 

 Int_t modNum = 0;
 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Star2_Types[j];
  Float_t xPos  = Star2_X_Offset[j];
  for(Int_t i=0; i<Star2_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
    yPos += Star2_Delta_Y_Position;
    zPos += Star2_Delta_Z_Position;
  }

 }
  */
  // used @ STAR
TGeoTranslation* module_trans=NULL;

 Int_t ii=0; 
 Float_t xPos  = Module_X_Position;
 Float_t yPos  = Module_Y_Position;
 Float_t zPos  = Module_Z_Position;
 Double_t Radius = TMath::Sqrt(xPos*xPos + yPos*yPos);
 
 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Module_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<Module_Number[j]; i++) { 
    ii++;
    Int_t iRing = TMath::Floor(i/12);
    if(i==12 || i==24) {
      Float_t xPos  = Module_X_Position + Ring_dX_Position[iRing];
      Float_t yPos  = Module_Y_Position + Ring_dY_Position[iRing];
      Float_t zPos  = Module_Z_Position + Ring_dZ_Position[iRing];
      Double_t Radius = TMath::Sqrt(xPos*xPos + yPos*yPos);
    }
    Double_t Phi_Pos=RING_DelPhi*(i%12) + Ring_dPhi_Position[iRing];
    Double_t Phi=Phi_Pos*Deg2Rad;
    xPos = Radius*TMath::Cos(Phi);
    yPos = Radius*TMath::Sin(Phi);
    //zPos  = Module_Z_Position-RING_DelZ*iRing;
    if(i==10) {
      module_trans = new TGeoTranslation("", xPos, yPos, zPos);
      TGeoRotation* module_rot = new TGeoRotation();
      module_rot->RotateZ(RING_DelPhi*(i%12));
      TGeoCombiTrans* module_combi_trans = NULL;
      module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
      gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
       modNum++;
     }
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
 TGeoRotation* module_rot = new TGeoRotation("HDRef",HDRef_rotate_Z,MeanTheta,0.);
 //module_rot->RotateZ(HDRef_rotate_Z);
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

 Int_t modNum = 0;
 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Dia_Types[j];
  Float_t zPos  = Dia_Z_Position + j*Dia_DeltaZ_Position;

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

void position_Buc2012(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(Buc2012_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 Float_t yPos=Buc2012_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = Buc2012_X_Offset;
 Float_t zPos  = Buc2012_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Buc2012_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<Buc2012_Number[j]; i++) { 
    ii++; 
    module_trans 
      = new TGeoTranslation("", xPos, yPos, zPos);
    module_combi_trans = new TGeoCombiTrans(*module_trans, *module_rot);
    gGeoMan->GetVolume(geoVersion)->AddNode(gModules[modType], modNum, module_combi_trans);
    modNum++;
  }
 }
}

void position_Buc2015(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation("Buc2015",Buc2015_rotate_Z,-MeanTheta,0.);
 // module_rot->RotateZ(Buc2015_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=Buc2015_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = Buc2015_X_Offset;
 Float_t zPos  = Buc2015_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Buc2015_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<Buc2015_Number[j]; i++) { 
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

void position_THU(Int_t modNType)
{
 TGeoTranslation* module_trans=NULL;
 TGeoRotation* module_rot = new TGeoRotation();
 module_rot->RotateZ(THU_rotate_Z);
 TGeoCombiTrans* module_combi_trans = NULL;

 //  Int_t numModules=(Int_t)( (Inner_Module_Last_Y_Position-Inner_Module_First_Y_Position)/Module_Size_Y[modType])+1;
 Float_t yPos=THU_First_Y_Position;
 Int_t ii=0; 
 Float_t xPos  = THU_X_Offset;
 Float_t zPos  = THU_Z_Position;

 for (Int_t j=0; j<modNType; j++){
  Int_t modType= THU_Types[j];
  Int_t modNum = 0;
  for(Int_t i=0; i<THU_Number[j]; i++) { 
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
  //module_rot->RotateZ(Pla_rotate_Z[j]);
  TGeoCombiTrans* module_combi_trans = NULL;
  TGeoRotation* module_rot = new TGeoRotation(Form("Pla%d",j),90.+MeanTheta,90.,90.,0.,-MeanTheta,-90.);
  //TGeoRotation* module_rot = new TGeoRotation(Form("Pla%d",j),90.,0.,90.,90.,0.,0.);

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

void position_cer_modules(Int_t modNType)
{
 Int_t ii=0; 
 Int_t modNum = 0;
 for (Int_t j=0; j<modNType; j++){
  Int_t modType= Cer_Types[j];
  Float_t xPos  = Cer_X_Position[j];
  Float_t yPos  = Cer_Y_Position[j];
  Float_t zPos  = Cer_Z_Position[j];
  TGeoTranslation* module_trans=NULL;
  TGeoRotation* module_rot = new TGeoRotation(Form("Cer%d",j),Cer_rotate_Z[j],-MeanTheta,0.);
  // module_rot->RotateZ(Cer_rotate_Z[j]);
  TGeoCombiTrans* module_combi_trans = NULL;

  for(Int_t i=0; i<Cer_Number[j]; i++) { 
    ii++; 
    cout <<"Position Ceramic Module "<<i<<" of "<<Cer_Number[j]<<" Type "<<modType
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
