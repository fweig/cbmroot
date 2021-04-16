//
// \file create_MUCH_geometry_v21a
// @Author: Omveer Singh , Partha Pratim Bhaduri & Ekata Nandy

//Updated Aug 27, 2020 (Osingh): Passive material implemented in GEM modules. Ar:CO2 (70:30) is used as active gas.
// \brief Generates MUCH geometry in Root format.
//#########################################################################################################################################
// The first absorber is 28 cm low density carbon (density 1.78 g/cm^3) + 30 cm concrete.
// The shield inside the first absorber is composed of 28 cm of Al + 30 cm of lead. Rest are made of Iron.
// For first absorber, the opening angle of the hole in the shield is decreased from 2.9 to 2.5 degrees for first absorber.
// The Al shields in the absorbers 2-5 have same thickness as the absorbers. The beam pipe passes through the cylindrical hole inside the shields.
// First two stations (1,2) made up of GEM and last two stations (3,4) made up of RPC.
// 12 mm thick Aluminium plates is used for support and cooling in the GEM modules.
// Drift and read-out PCBs (copper coated G10 plates) have been inserted for realistic material budget for both GEM and RPC modules.
//###########################################################################################################################################

// in root all sizes are given in cm



#include "TSystem.h"
#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoPgon.h"
#include "TGeoMatrix.h"
#include "TGeoCompositeShape.h"
#include "TGeoXtru.h"
#include "TGeoCone.h"
#include "TGeoBBox.h"
#include "TGeoTube.h"
#include "TFile.h"
#include "TString.h"
#include "TList.h"
#include "TRandom3.h"
#include "TDatime.h"
#include "TClonesArray.h"

#include "TObjArray.h"
#include "TFile.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>



// Name of output file with geometry
const TString tagVersion   = "_v21a";
const TString subVersion   = "_sis100_1m_lmvm";
const TString geoVersion   = "much";// + tagVersion + subVersion;
const TString FileNameSim  = geoVersion +  tagVersion + subVersion+".geo.root";
const TString FileNameGeo  = geoVersion +  tagVersion + subVersion+"_geo.root";
//const TString FileNameInfo = geoVersion +  tagVersion + subVersion+".geo.info";

// Names of the different used materials which are used to build the modules
// The materials are defined in the global media.geo file
const TString KeepingVolumeMedium     = "air";
const TString L= "MUCHlead";
const TString I= "MUCHiron";
const TString activemedium="MUCHGEMmixture";
const TString spacermedium="MUCHnoryl";
const TString LDcarbon = "MUCHcarbonLD"; //Low density Carbon for Ist Abs
const TString Concrete = "MUCHconcrete"; //Concrete for Ist Abs
const TString Al = "MUCHaluminium"; //Al for cooling & support purpose
const TString copper = "MUCHcopper";
const TString g10= "MUCHG10";
const TString RPCm= "MUCHRPCgas";
const TString RPCg= "MUCHRPCglass";
const TString Kapton = "MUCHkapton";

// Universal input parameters

Double_t fMuchZ1 =125.0; // MuchCave Zin position [cm]
Double_t fAcceptanceTanMin = 0.1; // Acceptance tangent min
Double_t fAcceptanceTanMax = 0.466; // Acceptance tangent max

//************************************************************


    // Input parameters for absorbers
//***********************************************************
const Int_t fAbs = 4;
const Int_t fNabs = 6; // Number of absorber pieces
TString AbsMaterial[6]={"LD Graphite","LD Graphite","Concrete","Iron","Iron","Iron"};
// Absorber Zin position [cm] in the cave reference frame
Double_t fAbsorberZ1[6]={0, 16, 28, 90, 140, 190};
// Absorber thickness [cm]
Double_t fAbsorberLz[6]= {16, 12, 30, 20, 20, 30};
Double_t safetyrad[6]={0.0, 0.0, 0.0,30.0,30.0,30.0};



// Input parameters for MUCH stations
//********************************************

const Int_t fNst = 4; // Number of stations
// Sector-type module parameters
// Number of sectors per layer (should be even for symmetry)
// Needs to be fixed with actual numbers
Int_t fNSectorsPerLayer[4] = {16, 20, 18, 20};
//Double_t fRpcGlassDz[4] = {0.0,0.0,0.2,0.2}; //Rpc Glass thickness [cm]
Double_t fSpacerR = 2.0;         // Spacer width in R [cm]
Double_t fSpacerPhi = 2.0;       // Spacer width in Phi [cm]
Double_t fOverlapR = 2.0;        // Overlap in R direction [cm]

// Station Zceneter [cm] in  the cave reference frame

Double_t fStationZ0[4]={75,125,175,235};
Int_t fNlayers[4]={3,3,3,3}; // Number of layers
Int_t fDetType[4]={3,3,4,4}; // Detector type
Double_t fLayersDz[4]={10,10,10,10};






// Input parameters for beam pipe shielding
// spans from 2.9 degree to 5.1 degree
//Inner radius is tan(2.9) + 2 cm, extra 20 mm for clamp connection

const Int_t fNshs=5;
TString ShMaterial[5]={"Al","Pb","Al","Al","Al"};
Double_t fShieldZin[5]={125,153.0, 215.0, 265.0, 315.0};
Double_t fShieldLz[5]={28, 30, 20, 20, 30};
Double_t fShield_AcceptanceTanMin[5] = {0.043,0.043, 0.051,0.051,0.051}; // Acceptance tangent min for shield
Double_t fShield_AcceptanceTanMax[5] = {0.1,0.1,0.1,0.1,0.1}; // Acceptance tangent max for shield


//***********************************************************

// some global variables
TGeoManager* gGeoMan = NULL;  // Pointer to TGeoManager instance
TGeoVolume*  gModules[fNabs]; // Global storage for module types
TGeoVolume*  gModules_shield[fNshs]; // Global storage for module types
TGeoVolume*  gModules_station[fNst]; // Global storage for module types

// Forward declarations
void create_materials_from_media_file();
TGeoVolume* CreateAbsorbers(int iabs);
TGeoVolume* CreateShields(int ishield);
TGeoVolume* CreateStations(int ist);
TGeoVolume* CreateGemLayers(int istn, int ily);
TGeoVolume* CreateRpcLayers(int istn, int ily);


fstream infoFile;
void create_MUCH_geometry_v21a_lmvm() {
  
  
  // -------   Open info file   -----------------------------------------------
  TString infoFileName = FileNameSim;
  infoFileName.ReplaceAll("root", "info");
  infoFile.open(infoFileName.Data(), fstream::out);
  infoFile << "MUCH geometry created with create_MUCH_geometry_v21a_lmvm.C" << endl << endl;
  infoFile<<"Global Variables: "<<endl;
  infoFile<<"MuchCave Zin position = "<<fMuchZ1<<" cm "<<endl;
  infoFile<<"Acceptance tangent min = "<<fAcceptanceTanMin<<endl;
  infoFile<<"Acceptance tangent max = "<<fAcceptanceTanMax<<endl;

  // Load needed material definition from media.geo file
  create_materials_from_media_file();
  
  // Get the GeoManager for later usage
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetVisLevel(10);
  
  // Create the top volume
  TGeoBBox* topbox= new TGeoBBox("", 1000., 1000., 2000.);
  TGeoVolume* top = new TGeoVolume("top", topbox, gGeoMan->GetMedium("air"));
  gGeoMan->SetTopVolume(top);

  TString topName=geoVersion+tagVersion+subVersion;

  TGeoVolume* much = new TGeoVolumeAssembly(topName);
  top->AddNode(much, 1);

  TGeoVolume *absr = new TGeoVolumeAssembly("absorber");
  much->AddNode(absr,1);

  TGeoVolume *shld = new TGeoVolumeAssembly("shield");
  much->AddNode(shld,1);

  TGeoVolume *sttn = new TGeoVolumeAssembly("station");
  much->AddNode(sttn,1);

  infoFile<<endl;
  infoFile<<"                     Absorbers "<<endl;
  infoFile<<"                    -----------"<<endl;
  infoFile<<"Total No. of Absorbers: "<<fAbs<<endl;
  infoFile<<"First abosrber is divided into two halves."<<endl;
  infoFile<<"First half inserted inside the Dipole Magnet."<<endl;
  infoFile<<"Second half is made of Low Density Graphite + Concrete."<<endl;
  infoFile<<"Total No. of Pieces: "<<fNabs<<endl;
  infoFile<<endl;
  infoFile<<"AbsPieces   Position[cm]   Thickness[cm]        Material"<<endl;
  infoFile<<"--------------------------------------------------------------"<<endl;

  for (Int_t iabs = 0; iabs <fNabs ; iabs++) {
    
    gModules[iabs] = CreateAbsorbers(iabs);
    
    absr->AddNode(gModules[iabs],iabs);
  }
  

  infoFile<<endl;
  infoFile<<"                     Shielding "<<endl;
  infoFile<<"                    -----------"<<endl;
  infoFile<<"No. of Shields: "<<fNshs<<endl;
  infoFile<<"Inside the Abs I, Shielding divided into two parts."<<endl;
  infoFile<<endl;
  infoFile<<"Shield No.   Z_In[cm]  Z_Out[cm]  R_In[cm]  R_Out[cm]   Material"<<endl;
  infoFile<<"--------------------------------------------------------------"<<endl;
  for (Int_t ishi = 0; ishi <fNshs ; ishi++) {
    
    gModules_shield[ishi] = CreateShields(ishi);
    
    shld->AddNode(gModules_shield[ishi],ishi);
    
  }
  
  infoFile<<endl;
  infoFile<<"                     Stations "<<endl;
  infoFile<<"                    ----------"<<endl;
  infoFile<<"No. of Stations: "<<fNst<<endl;
  infoFile<<"First two stations (1,2) are made up of GEM and last two stations (3,4) are made up of RPC."<<endl;
  infoFile<<"Passive material implemented in GEM modules. Ar:CO2 (70:30) is used as active gas. "<<endl;
  infoFile<<"12 mm thick Al plates are used for support and cooling in the GEM modules."<<endl;
  infoFile<<"2 mm thick Aluminum plates are used for support in the RPC modules behind the active area. 10 mm thick Aluminium at the boundaries as the frame."<<endl;
  infoFile<<"Drift and read-out PCBs (copper coated G10 plates) inserted for realistic material budget for both GEM and RPC modules."<<endl;
  infoFile<<"#Station   #Layers     Z[cm] #Sectors ActiveLz[cm]"<<endl;
  infoFile<<"--------------------------------------------------------------"<<endl;
  for (Int_t istn = 0; istn < 4; istn++) { // 4 Stations
    
    
    gModules_station[istn] = CreateStations(istn);
    
    sttn->AddNode(gModules_station[istn],istn);
  }
  
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.0000001);
  gGeoMan->PrintOverlaps();

  gGeoMan->CheckOverlaps(0.0001, "s");
  gGeoMan->PrintOverlaps();
  
  
  much->Export(FileNameSim);   // an alternative way of writing the much
  
  TFile* outfile = new TFile(FileNameSim, "UPDATE");
  TGeoTranslation* much_placement = new TGeoTranslation("much_trans", 0., 0., 0.);
  much_placement->Write();
  outfile->Close();
  
  outfile = new TFile(FileNameGeo,"RECREATE");
  gGeoMan->Write();  // use this if you want GeoManager format in the output
  outfile->Close();
  
  top->Draw("ogl");
  infoFile.close();
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

  FairGeoMedium* air = geoMedia->getMedium(KeepingVolumeMedium);
  geoBuild->createMedium(air);

  FairGeoMedium* MUCHiron = geoMedia->getMedium(I);
  geoBuild->createMedium(MUCHiron);

  FairGeoMedium* MUCHlead = geoMedia->getMedium(L);
  geoBuild->createMedium(MUCHlead);

  FairGeoMedium* MUCHargon = geoMedia->getMedium(activemedium);
  geoBuild->createMedium(MUCHargon);

  FairGeoMedium* MUCHnoryl = geoMedia->getMedium(spacermedium);
  geoBuild->createMedium(MUCHnoryl);

  FairGeoMedium* MUCHsupport = geoMedia->getMedium(Al);
  geoBuild->createMedium(MUCHsupport);

  FairGeoMedium* MUCHcarbonLD = geoMedia->getMedium(LDcarbon);
  geoBuild->createMedium(MUCHcarbonLD);

  FairGeoMedium* MUCHconcrete = geoMedia->getMedium(Concrete);
  geoBuild->createMedium(MUCHconcrete);

  FairGeoMedium* copperplate  = geoMedia->getMedium(copper);
  geoBuild->createMedium(copperplate);

  FairGeoMedium* g10plate  = geoMedia->getMedium(g10); //G10
  geoBuild->createMedium(g10plate);

  FairGeoMedium* RPCmedium = geoMedia->getMedium(RPCm);
  geoBuild->createMedium(RPCmedium);

  FairGeoMedium* RPCmaterial = geoMedia->getMedium(RPCg);
  geoBuild->createMedium(RPCmaterial);

  FairGeoMedium* kapton = geoMedia->getMedium(Kapton);
  geoBuild->createMedium(kapton);
}



TGeoVolume* CreateShields(int ish) {

  
  TGeoMedium* iron = gGeoMan->GetMedium(I);
  TGeoMedium* lead = gGeoMan->GetMedium(L);
  TGeoMedium* Aluminium= gGeoMan->GetMedium(Al);
  
  TString name = Form("shieldblock%d", ish);
  TGeoVolumeAssembly* shieldblock = new TGeoVolumeAssembly(name);
  
  TString conename_sh =  Form("conesh_%d",ish);
  

  Double_t dz = fShieldLz[ish]/2.0 ;
  Double_t globalZ1 = fShieldZin[ish] ;
  Double_t globalZ2 = fShieldZin[ish] + 2 * dz ;
  
  
  
  Double_t rmin1 = globalZ1 * fShield_AcceptanceTanMin[ish]+2.0;
  Double_t rmax1 = globalZ1 * fShield_AcceptanceTanMax[ish]-0.0001;
  Double_t rmin2 = globalZ2 * fShield_AcceptanceTanMin[ish]+2.0;
  Double_t rmax2 = globalZ2 * fShield_AcceptanceTanMax[ish]-0.0001;
  
  infoFile<<"   "<<ish<<"\t\t"<<globalZ1<<"\t  "<<globalZ2<<"\t"<<rmin1<<"\t   "<<rmax1<<"\t  "<<ShMaterial[ish]<<endl;

  if(ish==0){
    
    TGeoCone * sh =new TGeoCone(conename_sh,dz, rmin1, rmax1, rmin2, rmax2);
    TGeoVolume* shield = new TGeoVolume("shield", sh, Aluminium);

    shield->SetLineColor(7);
    shield->SetTransparency(2);
    TGeoTranslation *sh_trans = new TGeoTranslation("", 0., 0., globalZ1+dz);
    shieldblock->AddNode(shield,ish, sh_trans);
    

  }
  
  if(ish==1){
    TGeoCone * sh =new TGeoCone(conename_sh,dz, rmin1, rmax1, rmin2, rmax2);
    TGeoVolume* shield = new TGeoVolume("shield", sh, lead);
    
    shield->SetLineColor(kMagenta);
    shield->SetTransparency(2);
    TGeoTranslation *sh_trans = new TGeoTranslation("", 0., 0., globalZ1+dz);
    shieldblock->AddNode(shield,ish, sh_trans);
  }

  if(ish>1)
    {

      
      TGeoCone * sh =new TGeoCone(conename_sh,dz, rmin1, rmax1, rmin2, rmax2);
      TGeoVolume* shield = new TGeoVolume("shield", sh, Aluminium);
      
      shield->SetLineColor(kBlack);
      shield->SetTransparency(2);
      TGeoTranslation *sh_trans = new TGeoTranslation("", 0., 0., globalZ1+dz);
      shieldblock->AddNode(shield,ish, sh_trans);
    }
  

  return shieldblock;
  
}


TGeoVolume* CreateAbsorbers(int i) {
  
  TGeoMedium* graphite = gGeoMan->GetMedium(LDcarbon);
  TGeoMedium* iron = gGeoMan->GetMedium(I);
  TGeoMedium* concrete = gGeoMan->GetMedium(Concrete);
  TGeoMedium* Aluminium= gGeoMan->GetMedium(Al);

  TString name = Form("absblock%d", i);
  TGeoVolumeAssembly* absblock = new TGeoVolumeAssembly(name);

  TString pipename =  Form("beampipe_%d",i);
  TString conename =  Form("cone_%d",i);
  TString BoxName   = Form("Box_%d",i);
  TString supportShapeName = Form("Support_%d",i);
  TString TrapName =  Form("Trap_%d",i);
  
  Double_t dz = fAbsorberLz[i]/2.0 ;
  Double_t globalZ1 = fAbsorberZ1[i] + fMuchZ1;
  Double_t globalPos = globalZ1 + dz;
  Double_t globalZ2 = fAbsorberZ1[i] + 2 * dz + fMuchZ1;

  Double_t rmin1 = globalZ1 * fAcceptanceTanMin;
  Double_t rmin2 = globalZ2 * fAcceptanceTanMin;
  Double_t rmax1 = globalZ1 * fAcceptanceTanMax + safetyrad[i];
  Double_t rmax2 = globalZ2 * fAcceptanceTanMax + safetyrad[i];//

  infoFile<<"   "<<i+1<<"\t\t"<<globalPos<<"\t\t"<<2*dz<<"\t\t"<<AbsMaterial[i]<<endl;

  // 1st part of 1st absorber LD Carbon
  //dimensions are hardcoded
  if(i==0)
    {
     // printf("absorber %d \n",i);
      
      TGeoTrd2 * trap = new TGeoTrd2(TrapName,70.0,70.0,46.0,71.0,dz);
      TGeoCone * tube = new TGeoCone(pipename,dz+0.001,0.,rmin1,0.,rmin2);
      TString expression = TrapName +"-"+pipename;
      TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName,expression);
      TGeoVolume* abs0 = new TGeoVolume("absorber", shSupport, graphite);
      abs0->SetLineColor(kRed);
      abs0->SetTransparency(0);
      TGeoTranslation *abs0_trans = new TGeoTranslation("", 0., 0., globalZ1+dz);
      absblock->AddNode(abs0, i, abs0_trans);
    }



  // 2rd part of 1st absorber box (LD Carbon)
  if(i==1)
    {
     // printf("absorber %d \n",i);
      TGeoBBox * box = new TGeoBBox(BoxName,130.0,125.0,dz);
      TGeoCone * tube = new TGeoCone(pipename,dz+0.001,0.,rmin1,0.,rmin2);
      TString expression = BoxName +"-"+pipename;
      TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName,expression);
      
      TGeoVolume* abs1 = new TGeoVolume("absorber", shSupport, graphite);
      abs1->SetLineColor(kRed);
      abs1->SetTransparency(0);
      TGeoTranslation *abs1_trans = new TGeoTranslation("", 0., 0., globalZ1+dz);
      absblock->AddNode(abs1, i, abs1_trans);


    }

  // 3th part of 1st absorber box (Conc)
  if(i==2)
    {
     // printf("absorber %d \n",i);
      TGeoBBox * box = new TGeoBBox(BoxName,130.0,125.0,dz);
      TGeoCone * tube = new TGeoCone(pipename,dz+0.001,0.,rmin1,0.,rmin2);
      TString expression = BoxName +"-"+pipename;
      TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName,expression);

      TGeoVolume* abs1 = new TGeoVolume("absorber", shSupport, concrete);
      abs1->SetLineColor(kViolet);
      abs1->SetTransparency(0);
      TGeoTranslation *abs1_trans = new TGeoTranslation("", 0., 0., globalZ1+dz);
      absblock->AddNode(abs1, i, abs1_trans);
    }



  //rest of the absorbers
  if (i>2)
    {
      TGeoBBox * box = new TGeoBBox(BoxName,rmax2,rmax2,dz);
      TGeoCone * tube = new TGeoCone(pipename,dz+0.001,0.,rmin1,0.,rmin2);
      TString expression = BoxName +"-"+pipename;
      TGeoCompositeShape* shSupport = new TGeoCompositeShape(supportShapeName,expression);

      TGeoVolume* abs2 = new TGeoVolume("absorber", shSupport, iron);

      abs2->SetLineColor(kBlue);
      abs2->SetTransparency(2);


      TGeoTranslation *abs_trans = new TGeoTranslation("", 0., 0., globalZ1+dz);
      absblock->AddNode(abs2,i, abs_trans);


    }

  return absblock;

}

TGeoVolume * CreateStations(int ist){
  
  TString stationName = Form("muchstation%02i",ist+1);
  
  TGeoVolumeAssembly* station = new TGeoVolumeAssembly(stationName);//, shStation, air);
  
  TGeoVolume*  gLayer[4];

  for (int ii=0;ii<3;ii++){  // 3 Layers
    
    switch(ist){
    case 0:
    case 1:    gLayer[ii] = CreateGemLayers(ist, ii); break;
    case 2:
    case 3:    gLayer[ii] = CreateRpcLayers(ist, ii); break;}
    
    station->AddNode(gLayer[ii],ii);
    
  }
  
  return station;
}


TGeoVolume * CreateGemLayers(int istn, int ily){
  
  TString layerName = Form("muchstation%02ilayer%i",istn+1,ily+1);
  TGeoVolumeAssembly* volayer = new TGeoVolumeAssembly(layerName);

  //GEM Parametrs
  Double_t fSupportDz = 1.2; //1.2 cm Al (cooling + support)
  Double_t fCopperDz  = 0.0065; // 65 micron copper
  Double_t fCopperSliceDz = 0.0005; //5 micron copper slices
  Double_t fReadoutPlateDz = 0.3; //3mm G10
  Double_t fDriftPlateDz = 0.3; //3 mm G10
  Double_t fPassiveVolumeDz = 0.2; // 2 mm of Argon
  Double_t fActiveVolumeDz = 0.3; //3mm argon
  Double_t fKaptonDz = 0.005; //50 micron Kapton
  Double_t fFrameDz = 0.2; // 2mm frame
  
  
  Double_t stGlobalZ0 = fStationZ0[istn] + fMuchZ1; //z position of station center (midplane) [cm]
  

  
  Double_t layerZ0 = (ily - (fNlayers[istn] - 1) / 2.) * fLayersDz[istn];
  Double_t layerGlobalZ0 = layerZ0 + stGlobalZ0;
  

  //Active Gas distance from layer position
  Double_t ModuleZ = fSupportDz/2. + 2*fCopperDz + fReadoutPlateDz + 3*fPassiveVolumeDz + 6*fCopperSliceDz + 3*fKaptonDz + fActiveVolumeDz/2.;
  
  //ReadOut Plate distance from layer position
  Double_t ReadOutZ = fSupportDz/2. + fCopperDz + fReadoutPlateDz/2.;
  
  //Copper distance from layer position
  Double_t CooperIZ   = fSupportDz/2. + fCopperDz/2.;
  Double_t CooperIIZ  = ReadOutZ + fReadoutPlateDz/2. + fCopperDz/2.;
  Double_t CooperIIIZ = ModuleZ + fActiveVolumeDz/2. + fCopperDz/2.;
  Double_t CooperIVZ  = CooperIIIZ + fCopperDz + fDriftPlateDz;
  
  //Drift Plate distance from layer position
  Double_t DriftZ = ModuleZ + fActiveVolumeDz/2. + fCopperDz + fDriftPlateDz/2.;
  
  //Passive Gas distance from layer position
  Double_t PassivePassiveDz = fPassiveVolumeDz + 2*fCopperSliceDz + fKaptonDz;
  Double_t PassiveGasIZ = ReadOutZ + fReadoutPlateDz/2. + fCopperDz + fPassiveVolumeDz/2.;
  Double_t PassiveGasIIZ = PassiveGasIZ + PassivePassiveDz;
  Double_t PassiveGasIIIZ = PassiveGasIIZ + PassivePassiveDz;

  //Kapton distance from layer position
  Double_t KaptonPassiveDz = fPassiveVolumeDz/2. + fCopperSliceDz + fKaptonDz/2.;
  Double_t KaptonIZ = PassiveGasIZ + KaptonPassiveDz;
  Double_t KaptonIIZ = PassiveGasIIZ + KaptonPassiveDz;
  Double_t KaptonIIIZ = PassiveGasIIIZ + KaptonPassiveDz;
  
  //Copper Slice distance from layer position
  Double_t PassiveCopperDz = fPassiveVolumeDz/2. + fCopperSliceDz/2.;
  Double_t KaptonCopperDz = fKaptonDz + fCopperSliceDz;
  Double_t CopperSilceIZ = PassiveGasIZ + PassiveCopperDz;
  Double_t CopperSilceIIZ = CopperSilceIZ + KaptonCopperDz;
  Double_t CopperSilceIIIZ = PassiveGasIIZ + PassiveCopperDz;
  Double_t CopperSilceIVZ = CopperSilceIIIZ + KaptonCopperDz;
  Double_t CopperSilceVZ = PassiveGasIIIZ + PassiveCopperDz;
  Double_t CopperSilceVIZ = CopperSilceVZ + KaptonCopperDz;

 //Set Rmin & Rmax
  Double_t stDz = ((fNlayers[istn] - 1) * fLayersDz[istn])/2. + CooperIVZ + fCopperDz/2.;
  
  Double_t stGlobalZ2 = stGlobalZ0 + stDz;
  Double_t stGlobalZ1 = stGlobalZ0 - stDz;
  
  Double_t rmin = stGlobalZ1 * fAcceptanceTanMin;
  Double_t rmax = stGlobalZ2 * fAcceptanceTanMax;


  // Module dimention calculation
  Double_t phi0 = TMath::Pi()/fNSectorsPerLayer[istn]; // azimuthal half widh of each module
  Double_t ymin = rmin+fSpacerR;
  Double_t ymax = rmax;

  //define the dimensions of the trapezoidal module
  Double_t dy  = (ymax-ymin)/2.; //y (length)
  Double_t dx1 = ymin*TMath::Tan(phi0)+fOverlapR/TMath::Cos(phi0);  // large x
  Double_t dx2 = ymax*TMath::Tan(phi0)+fOverlapR/TMath::Cos(phi0); // small x

  //define the spacer dimensions
  Double_t tg = (dx2-dx1)/2/dy;
  Double_t dd1 = fSpacerPhi*tg;
  Double_t dd2 = fSpacerPhi*sqrt(1+tg*tg);
  Double_t sdx1 = dx1+dd2-dd1-0.1; // 0.1 cm to avoid overlaps
  Double_t sdx2 = dx2+dd2+dd1;
  Double_t sdy  = dy+fSpacerR;
  
  
  
  infoFile<<"   "<<istn+1<<"\t      "<<ily+1<<"\t\t"<<layerGlobalZ0<<"\t"<<fNSectorsPerLayer[istn]<<"\t"<<fActiveVolumeDz<<endl;
  
  
  // Aluminum Plate (Cooling + Support)
  TString supportAlName  = Form("shStation%02iSupportAl",istn+1);
  TGeoTube* shSupportAl = new TGeoTube(supportAlName,rmin,rmax,fSupportDz/2.);
  
  TString  supportName1  = Form("muchstation%02ilayer%isupportAl",istn+1,ily+1);
  TGeoMedium* coolMat = gGeoMan->GetMedium(Al);
  
  TGeoVolume* voSupport1 = new TGeoVolume(supportName1,shSupportAl,coolMat);
  voSupport1->SetLineColor(kCyan);
  
  TGeoTranslation *support_trans1 = new TGeoTranslation("supportName1", 0,0,layerGlobalZ0);
  volayer->AddNode(voSupport1,0,support_trans1);
  
  
  
  // Now start adding the GEM modules
  for (Int_t iModule=0; iModule<fNSectorsPerLayer[istn]; iModule++){
    
    Double_t phi  = 2 * phi0 * (iModule + 0.5);  // add 0.5 to not overlap with y-axis for left-right layer separation
    Bool_t isBack = iModule%2;
    Char_t cside  = (isBack==1) ? 'b' : 'f';
    Int_t iMod = iModule/2;

    // correct the x, y positions
    Double_t pos[21];
    pos[0] = -(ymin+dy)*sin(phi);
    pos[1] =  (ymin+dy)*cos(phi);
    // different z positions for odd/even modules
    pos[2] = (isBack ? 1 : -1)*ModuleZ + layerGlobalZ0; //Active volume & Frame
    pos[3] = (isBack ? 1 : -1)*ReadOutZ + layerGlobalZ0;//Readout G10
    pos[4] = (isBack ? 1 : -1)*CooperIZ + layerGlobalZ0; //Copper I
    pos[5] = (isBack ? 1 : -1)*CooperIIZ + layerGlobalZ0; //Copper II
    pos[6] = (isBack ? 1 : -1)*CooperIIIZ + layerGlobalZ0; //Copper III
    pos[7] = (isBack ? 1 : -1)*CooperIVZ + layerGlobalZ0; //Copper IV
    pos[8] = (isBack ? 1 : -1)*DriftZ + layerGlobalZ0; //Drift G10 PassiveGasIZ
    pos[9] = (isBack ? 1 : -1)*PassiveGasIZ + layerGlobalZ0; // Passive Gas I
    pos[10] = (isBack ? 1 : -1)*PassiveGasIIZ + layerGlobalZ0; // Passive Gas II
    pos[11] = (isBack ? 1 : -1)*PassiveGasIIIZ + layerGlobalZ0; // Passive Gas III
    pos[12] = (isBack ? 1 : -1)*KaptonIZ + layerGlobalZ0; // Kapton I
    pos[13] = (isBack ? 1 : -1)*KaptonIIZ + layerGlobalZ0; // Kapton II
    pos[14] = (isBack ? 1 : -1)*KaptonIIIZ + layerGlobalZ0; // Kapton III
    pos[15] = (isBack ? 1 : -1)*CopperSilceIZ + layerGlobalZ0; // Copper Slice I
    pos[16] = (isBack ? 1 : -1)*CopperSilceIIZ + layerGlobalZ0; // Copper Slice II
    pos[17] = (isBack ? 1 : -1)*CopperSilceIIIZ + layerGlobalZ0; // Copper Slice III
    pos[18] = (isBack ? 1 : -1)*CopperSilceIVZ + layerGlobalZ0; // Copper Slice IV
    pos[19] = (isBack ? 1 : -1)*CopperSilceVZ + layerGlobalZ0; // Copper Slice V
    pos[20] = (isBack ? 1 : -1)*CopperSilceVIZ + layerGlobalZ0; // Copper Slice VI

    //define media
    TGeoMedium* argon = gGeoMan->GetMedium(activemedium); // active medium
    TGeoMedium* noryl = gGeoMan->GetMedium(spacermedium); // frame medium
    TGeoMedium* g10plate = gGeoMan->GetMedium(g10); // G10 medium
    TGeoMedium* copperplate = gGeoMan->GetMedium(copper); // copper
    TGeoMedium* kapton = gGeoMan->GetMedium(Kapton); // Kapton
    
    

    //Active Volume
    TGeoTrap* shapeActive = new TGeoTrap(fActiveVolumeDz/2.,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
    shapeActive->SetName(Form("shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule));

    TString activeName = Form("muchstation%02ilayer%i%cactive%03igasArgon",istn+1,ily+1,cside,iMod+1);
    TGeoVolume* voActive = new TGeoVolume(activeName,shapeActive,argon);
    voActive->SetLineColor(kGreen);
    
    //Frame
    TGeoTrap* shapeFrame = new TGeoTrap(fFrameDz/2.,0,0,sdy,sdx1,sdx2,0,sdy,sdx1,sdx2,0);
    shapeFrame->SetName(Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole", istn, ily, cside, iModule));
    
    TString expression = Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole-shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule, istn, ily, cside, iModule);
    
    TGeoCompositeShape* shFrame = new TGeoCompositeShape(Form("shStation%02iLayer%i%cModule%03iFinalFrameNoHole", istn, ily, cside, iModule), expression);
    
    TString frameName = Form("muchstation%02ilayer%i%cframe%03i",istn+1,ily+1,cside,iModule+1);
    
    TGeoVolume* voFrame = new TGeoVolume(frameName,shFrame,noryl);  // add a name to the frame
    voFrame->SetLineColor(kMagenta);

    //Readout Plate
    TGeoTrap* shapeReadOut = new TGeoTrap(fReadoutPlateDz/2.0,0,0,sdy,sdx1,sdx2,0,sdy,sdx1,sdx2,0);
    shapeReadOut->SetName(Form("shStation%02iLayer%i%cModule%03iReadOut", istn, ily, cside, iModule));
    
    TString ReadOutName = Form("muchstation%02ilayer%i%cReadOut%03i",istn+1,ily+1,cside,iModule+1);
    
    TGeoVolume* voReadOut = new TGeoVolume(ReadOutName,shapeReadOut,g10plate);
    voReadOut->SetLineColor(2);
    
    
    //4 Copper  65 micron
    TGeoTrap* shapeCopper[4];
    TString  CopperName[4];
    TGeoVolume* voCopper[4];
    for(Int_t iCop =0; iCop <4; iCop++){
      shapeCopper[iCop] = new TGeoTrap(fCopperDz/2.0,0,0,sdy,sdx1,sdx2,0,sdy,sdx1,sdx2,0);
      shapeCopper[iCop]->SetName(Form("shStation%02iLayer%i%cModule%03iCopper", istn, ily, cside, iModule));
      
      CopperName[iCop] = Form("muchstation%02ilayer%i%cCopper%iModule%03i",istn+1,ily+1,cside,iCop+1,iModule+1);
      
      voCopper[iCop] = new TGeoVolume(CopperName[iCop],shapeCopper[iCop],copperplate);
      voCopper[iCop]->SetLineColor(3);
    }
    
    //Drift Plate
    TGeoTrap* shapeDrift = new TGeoTrap(fDriftPlateDz/2.0,0,0,sdy,sdx1,sdx2,0,sdy,sdx1,sdx2,0);
    shapeDrift->SetName(Form("shStation%02iLayer%i%cModule%03iDrift", istn, ily, cside, iModule));
    
    TString DriftName = Form("muchstation%02ilayer%i%cDrift%03i",istn+1,ily+1,cside,iModule+1);
    
    TGeoVolume* voDrift = new TGeoVolume(DriftName,shapeDrift,g10plate);
    voDrift->SetLineColor(2);
    
    //Passive Gas (2mm Ar)
    TGeoTrap* shapePassiveGas[3];
    TString PassiveGasName[3];
    TGeoVolume* voPassiveGas[3];
    for(Int_t iPGas =0; iPGas<3; iPGas++){
      shapePassiveGas[iPGas] = new TGeoTrap(fPassiveVolumeDz/2.,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
      shapePassiveGas[iPGas]->SetName(Form("shStation%02iLayer%i%cModule%03iPassiveGasNoHole", istn, ily, cside, iModule));
      
      PassiveGasName[iPGas] = Form("muchstation%02ilayer%i%cPassiveGas%iModule%03i",istn+1,ily+1,cside,iPGas,iModule+1);
      voPassiveGas[iPGas] = new TGeoVolume(PassiveGasName[iPGas],shapePassiveGas[iPGas],argon);
      voPassiveGas[iPGas]->SetLineColor(kGreen);
    }
       
    //kapton (50 micron)
    TGeoTrap* shapeKapton[3];
    TString  KaptonName[3];
    TGeoVolume* voKapton[3];
    for(Int_t iCop =0; iCop <3; iCop++){
      shapeKapton[iCop] = new TGeoTrap(fKaptonDz/2.0,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
      shapeKapton[iCop]->SetName(Form("shStation%02iLayer%i%cModule%03iKapton", istn, ily, cside, iModule));
      
      KaptonName[iCop] = Form("muchstation%02ilayer%i%cKapton%iModule%03i",istn+1,ily+1,cside,iCop+1,iModule+1);
      
      voKapton[iCop] = new TGeoVolume(KaptonName[iCop],shapeKapton[iCop],kapton);
      voKapton[iCop]->SetLineColor(4);
    }
    
    //6 Copper Slice (5 micron) fCopperSliceDz
    TGeoTrap* shapeCopperSlice[6];
    TString  CopperSliceName[6];
    TGeoVolume* voCopperSlice[6];
    for(Int_t iCop =0; iCop <6; iCop++){
      shapeCopperSlice[iCop] = new TGeoTrap(fCopperSliceDz/2.0,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
      shapeCopperSlice[iCop]->SetName(Form("shStation%02iLayer%i%cModule%03iCopperSlice", istn, ily, cside, iModule));
      
      CopperSliceName[iCop] = Form("muchstation%02ilayer%i%cCopperSlice%iModule%03i",istn+1,ily+1,cside,iCop+1,iModule+1);
      
      voCopperSlice[iCop] = new TGeoVolume(CopperSliceName[iCop],shapeCopperSlice[iCop],copperplate);
      voCopperSlice[iCop]->SetLineColor(3);
    }
    
    
    
    // Calculate the phi angle of the sector where it has to be placed
      Double_t angle = 180. / TMath::Pi() * phi;  // convert angle phi from rad to deg
      
      
      TGeoRotation *r2 = new TGeoRotation("r2");
      //rotate in the vertical plane (per to z axis) with angle
      r2->RotateZ(angle);
      
      
      TGeoTranslation *trans[20];
      TGeoHMatrix *incline_mod[20];
      
      for(Int_t i=0; i<19; i++){
	trans[i] = new TGeoTranslation("",pos[0],pos[1],pos[i+2]);
        incline_mod[i] = new TGeoHMatrix("");
	(*incline_mod[i]) =  (*trans[i]) * (*r2);
      }
      volayer->AddNode(voActive, iModule, incline_mod[0]); // add active volume
    //  volayer->AddNode(voFrame, iModule, incline_mod[0]);  // add frame  // Frame removed from Active gas
      volayer->AddNode(voReadOut, iModule, incline_mod[1]);  // add Read Out
      for(int iNode =0; iNode<4; iNode++)volayer->AddNode(voCopper[iNode], iModule, incline_mod[iNode+2]); //add Copper
      volayer->AddNode(voDrift, iModule, incline_mod[6]);  // add Drift Out
      for(int iNode =0; iNode<3; iNode++)volayer->AddNode(voPassiveGas[iNode], iModule, incline_mod[iNode+7]); //add Passive Gas
      for(int iNode =0; iNode<3; iNode++)volayer->AddNode(voKapton[iNode], iModule, incline_mod[iNode+10]); //add Kapton
      for(int iNode =0; iNode<6; iNode++)volayer->AddNode(voCopperSlice[iNode], iModule, incline_mod[iNode+13]); //add CopperSlices
      
  }
  
  return volayer;
}



TGeoVolume * CreateRpcLayers(int istn, int ily){
  
  TString layerName = Form("muchstation%02ilayer%i",istn+1,ily+1);
  TGeoVolumeAssembly* volayer = new TGeoVolumeAssembly(layerName);
  
  //RPC Parametrs
  Double_t fActiveVolumeDz = 0.2; //2mm RPC
  Double_t fSupportDz = 0.2; //2 mm Al (cooling + support)
  Double_t fRpcGlassDz = 0.2; // 2mm glass
  Double_t fCopperDz  = 0.0035; // 35 micron copper
  Double_t fReadoutPlateDz = 0.3; //3mm G10
  Double_t fDriftPlateDz = 0.3; //3 mm G10
  Double_t fFrameDz = 1.0; // 1cm  Al frame
  
  
  Double_t stGlobalZ0 = fStationZ0[istn] + fMuchZ1; //z position of station center (midplane) [cm]
  
  Double_t layerZ0 = (ily - (fNlayers[istn] - 1) / 2.) * fLayersDz[istn];
  Double_t layerGlobalZ0 = layerZ0 + stGlobalZ0;
  
  
  //Active Gas distance from layer position
  Double_t ModuleZ = fSupportDz/2. + fCopperDz + fReadoutPlateDz +  fRpcGlassDz + fActiveVolumeDz/2.;
  
  //RPC Glass
  Double_t GlassDz = fRpcGlassDz/2. + fActiveVolumeDz/2.;
  Double_t GlassIZ = ModuleZ - GlassDz;
  Double_t GlassIIZ = ModuleZ + GlassDz;    
  
  //ReadOut Plate distance from layer position
  Double_t ReadOutZ = fSupportDz/2. + fCopperDz + fReadoutPlateDz/2.;

  //Drift Plate distance from layer position
  Double_t DriftZ = GlassIIZ + fRpcGlassDz/2. + fDriftPlateDz/2.;

  //Copper distance from layer position
  Double_t CooperIZ   = fSupportDz/2. + fCopperDz/2.;
  Double_t CooperIIZ  = DriftZ + fDriftPlateDz/2. + fCopperDz/2.;

  
  //Set Rmin & Rmax
  Double_t stDz = ((fNlayers[istn] - 1) * fLayersDz[istn])/2. + CooperIIZ + fCopperDz/2.;
  
  Double_t stGlobalZ2 = stGlobalZ0 + stDz;
  Double_t stGlobalZ1 = stGlobalZ0 - stDz;
  
  Double_t rmin = stGlobalZ1 * fAcceptanceTanMin;
  Double_t rmax = stGlobalZ2 * fAcceptanceTanMax;
 
  
  // Module dimention calculation
  Double_t phi0 = TMath::Pi()/fNSectorsPerLayer[istn]; // azimuthal half widh of each module
  Double_t ymin = rmin+fSpacerR;
  Double_t ymax = rmax;
  
  //define the dimensions of the trapezoidal module
  Double_t dy  = (ymax-ymin)/2.; //y (length)
  Double_t dx1 = ymin*TMath::Tan(phi0)+fOverlapR/TMath::Cos(phi0);  // large x
  Double_t dx2 = ymax*TMath::Tan(phi0)+fOverlapR/TMath::Cos(phi0); // small x
  
  //define the spacer dimensions
  Double_t tg = (dx2-dx1)/2/dy;
  Double_t dd1 = fSpacerPhi*tg;
  Double_t dd2 = fSpacerPhi*sqrt(1+tg*tg);
  Double_t sdx1 = dx1+dd2-dd1-0.1; // 0.1 cm to avoid overlaps
  Double_t sdx2 = dx2+dd2+dd1;
  Double_t sdy  = dy+fSpacerR;
  
  
  
  infoFile<<"   "<<istn+1<<"\t      "<<ily+1<<"\t\t"<<layerGlobalZ0<<"\t"<<fNSectorsPerLayer[istn]<<"\t"<<fActiveVolumeDz<<endl;
  
  
  // Aluminum Plate (Cooling + Support)
  TString supportAlName  = Form("shStation%02iSupportAl",istn+1);
  TGeoTube* shSupportAl = new TGeoTube(supportAlName,rmin,rmax,fSupportDz/2.);
  
  TString  supportName1  = Form("muchstation%02ilayer%isupportAl",istn+1,ily+1);
  TGeoMedium* coolMat = gGeoMan->GetMedium(Al);
  
  TGeoVolume* voSupport1 = new TGeoVolume(supportName1,shSupportAl,coolMat);
  voSupport1->SetLineColor(kCyan);
  
  TGeoTranslation *support_trans1 = new TGeoTranslation("supportName1", 0,0,layerGlobalZ0);
  volayer->AddNode(voSupport1,0,support_trans1);
  
// Now start adding the GEM modules
  for (Int_t iModule=0; iModule<fNSectorsPerLayer[istn]; iModule++){
    
    Double_t phi  = 2 * phi0 * (iModule + 0.5);  // add 0.5 to not overlap with y-axis for left-right layer separation
    Bool_t isBack = iModule%2;
    Char_t cside  = (isBack==1) ? 'b' : 'f';
    Int_t iMod = iModule/2;
    // correct the x, y positions
    Double_t pos[9];
    pos[0] = -(ymin+dy)*sin(phi);
    pos[1] =  (ymin+dy)*cos(phi);
    // different z positions for odd/even modules
    pos[2] = (isBack ? 1 : -1)*ModuleZ + layerGlobalZ0; //Active volume & Frame
    pos[3] = (isBack ? 1 : -1)*GlassIZ + layerGlobalZ0; //Glass I
    pos[4] = (isBack ? 1 : -1)*GlassIIZ + layerGlobalZ0; //Glass II
    pos[5] = (isBack ? 1 : -1)*ReadOutZ + layerGlobalZ0; //ReadOut Plate
    pos[6] = (isBack ? 1 : -1)*DriftZ + layerGlobalZ0; //Drift Plate
    pos[7] = (isBack ? 1 : -1)*CooperIZ + layerGlobalZ0; //Copper I Plate
    pos[8] = (isBack ? 1 : -1)*CooperIIZ + layerGlobalZ0; //Copper II Plate
    
    

    TGeoMedium*  RPCgasmedium = gGeoMan->GetMedium(RPCm); // RPC Gas
    TGeoMedium*  RPCglassmat = gGeoMan->GetMedium(RPCg); // RPC Glass
    TGeoMedium* g10plate = gGeoMan->GetMedium(g10); // G10 medium
    TGeoMedium* copperplate = gGeoMan->GetMedium(copper); // copper
    TGeoMedium* AlFrame = gGeoMan->GetMedium(Al); // spacer medium  

    
    //Active Volume
    TGeoTrap* shapeActive = new TGeoTrap(fActiveVolumeDz/2.,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
    shapeActive->SetName(Form("shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule));
    
    TString activeName = Form("muchstation%02ilayer%i%cactive%03irpcgas",istn+1,ily+1,cside,iMod+1);
    TGeoVolume* voActive = new TGeoVolume(activeName,shapeActive,RPCgasmedium);
    voActive->SetLineColor(kGreen);
    
    // RPC Glass
     TGeoTrap* shapeGlass[2];
     TString GlassName[2];
     TGeoVolume* voGlass[2];
     for(Int_t iGlass =0 ; iGlass<2; iGlass++){
     shapeGlass[iGlass] = new TGeoTrap(fRpcGlassDz/2.,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
     shapeGlass[iGlass]->SetName(Form("shStation%02iLayer%i%cModule%03iNoHoleGlass", istn, ily, cside, iModule));
     GlassName[iGlass] = Form("muchstation%02ilayer%i%crpcglass%imodule%03i",istn+1,ily+1,cside,iGlass+1,iModule+1);
     voGlass[iGlass] = new TGeoVolume(GlassName[iGlass],shapeGlass[iGlass],RPCglassmat);
     voGlass[iGlass]->SetLineColor(kRed);
     }
     
     //Readout Plate
     TGeoTrap* shapeReadOut = new TGeoTrap(fReadoutPlateDz/2.0,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
     shapeReadOut->SetName(Form("shStation%02iLayer%i%cModule%03iReadOut", istn, ily, cside, iModule));
    
     TString ReadOutName = Form("muchstation%02ilayer%i%cReadOut%03i",istn+1,ily+1,cside,iModule+1);
     
     TGeoVolume* voReadOut = new TGeoVolume(ReadOutName,shapeReadOut,g10plate);
     voReadOut->SetLineColor(2);
     
     //Drift Plate
     TGeoTrap* shapeDrift = new TGeoTrap(fDriftPlateDz/2.0,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
     shapeDrift->SetName(Form("shStation%02iLayer%i%cModule%03iDrift", istn, ily, cside, iModule));
     
     TString DriftName = Form("muchstation%02ilayer%i%cDrift%03i",istn+1,ily+1,cside,iModule+1);
     
     TGeoVolume* voDrift = new TGeoVolume(DriftName,shapeDrift,g10plate);
     voDrift->SetLineColor(2);
     
     
     //2 Copper  35 micron
     TGeoTrap* shapeCopper[2];
     TString  CopperName[2];
     TGeoVolume* voCopper[2];
     for(Int_t iCop =0; iCop <2; iCop++){
       shapeCopper[iCop] = new TGeoTrap(fCopperDz/2.0,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
       shapeCopper[iCop]->SetName(Form("shStation%02iLayer%i%cModule%03iCopper", istn, ily, cside, iModule));
       
       CopperName[iCop] = Form("muchstation%02ilayer%i%cCopper%iModule%03i",istn+1,ily+1,cside,iCop+1,iModule+1);
       
       voCopper[iCop] = new TGeoVolume(CopperName[iCop],shapeCopper[iCop],copperplate);
       voCopper[iCop]->SetLineColor(kRed);
     }
     
     
     //Frame (1 cm Al)
     
     TGeoTrap* shape = new TGeoTrap(fFrameDz/2.,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
     shape->SetName(Form("shStation%02iLayer%i%cModule%03iFrameNoHole", istn, ily, cside, iModule));
     
     TGeoTrap* shapeFrame = new TGeoTrap(fFrameDz/2.,0,0,sdy,sdx1,sdx2,0,sdy,sdx1,sdx2,0);
     shapeFrame->SetName(Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole", istn, ily, cside, iModule));
     
     TString expression = Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole-shStation%02iLayer%i%cModule%03iFrameNoHole", istn, ily, cside, iModule, istn, ily, cside, iModule);
     
     TGeoCompositeShape* shFrame = new TGeoCompositeShape(Form("shStation%02iLayer%i%cModule%03iFinalFrameNoHole", istn, ily, cside, iModule), expression);
    
     TString frameName = Form("muchstation%02ilayer%i%cframe%03i",istn+1,ily+1,cside,iModule+1);
     
     TGeoVolume* voFrame = new TGeoVolume(frameName,shFrame,AlFrame);  // add a name to the frame
     voFrame->SetLineColor(kMagenta);
     
     
     
     // Calculate the phi angle of the sector where it has to be placed 
     Double_t angle = 180. / TMath::Pi() * phi;  // convert angle phi from rad to deg
     
     
     TGeoRotation *r2 = new TGeoRotation("r2");
     //rotate in the vertical plane (per to z axis) with angle 
     r2->RotateZ(angle);
     
     
     TGeoTranslation *trans[8];
     TGeoHMatrix *incline_mod[8]; 
     
     for(int i=0; i<7; i++){
       trans[i] = new TGeoTranslation("",pos[0],pos[1],pos[i+2]);
       
       incline_mod[i] = new TGeoHMatrix("");
       (*incline_mod[i]) =  (*trans[i]) * (*r2);  
     }
     
     volayer->AddNode(voActive, iModule, incline_mod[0]); // add active volume 
    // volayer->AddNode(voFrame, iModule, incline_mod[0]); // add frame 
     for(int iNode =0; iNode<2; iNode++)volayer->AddNode(voGlass[iNode], iModule, incline_mod[iNode+1]); //add Glass 
     volayer->AddNode(voReadOut, iModule, incline_mod[3]); // add ReadOut Plate
     volayer->AddNode(voDrift, iModule, incline_mod[4]); // add Drift Plate
     for(int iNode =0; iNode<2; iNode++)volayer->AddNode(voCopper[iNode], iModule, incline_mod[iNode+5]); //add Copper
  }
  return volayer;
}
