//                                             
/// \file create_MUCH_geometry_v18g.C
/// \brief Generates MUCH geometry in Root format.
///    

// 2017-11-20 - DE  - v18g - shift back to z = 70, 80 and 90 cm to avoid mSTS box
// 2017-11-10 - PPB -      - correct the y position of the modules to generate much points             
// 2017-11-07 - PPB -      - change the shape of cooling plates from rectangular to sector                           
// 2017-11-06 - PPB, VS and AM - mcbm version with actual Mv2 dimesions of the module
// 2017-10-23 - DE  - mcbm - put mMUCH in 6 o'clock position on z axis, shift 15 cm up and to z = 60, 70 and 80 cm
// 2017-09-04 - PPB - mcbm - preliminary version of mini much geometry
// 2017-05-16 - DE  - v17b - position the modules in a way to split layers left-right along y axis
// 2017-05-16 - DE  - v17b - attribute name to module frames
// 2017-05-16 - DE  - v17b - remove rim from support CompositeShape
// 2017-05-02 - PPB - v17a - Change the shape of the first absorber according to latest design
// 2017-04-27 - DE  - v17a - fix GEM module positions and angles
// 2017-04-22 - PPB - v17a - Define the absorber, shield and station shapes sizes ...
// 2016-04-19 - DE  - v17a - initial version derived from TRD

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
const TString tagVersion   = "_v18g";
const TString geoVersion   = "much";
const TString FileNameSim  = geoVersion + tagVersion+"_mcbm.geo.root";
const TString FileNameGeo  = geoVersion + tagVersion+"_mcbm_geo.root";
const TString FileNameInfo = geoVersion + tagVersion+"_mcbm.geo.info";

// Names of the different used materials which are used to build the modules
// The materials are defined in the global media.geo file 
const TString KeepingVolumeMedium     = "air";
const TString L= "MUCHlead";
const TString W= "MUCHwolfram";  
const TString C= "MUCHcarbon";
const TString I= "MUCHiron";
const TString activemedium="MUCHargon";
const TString spacermedium="MUCHnoryl";
const TString coolmedium="aluminium"; //Al cooling plates


// Universal input parameters

// The inner angle is 11 degree (polar angle); We take z = 70 cm;
//Inner radius: R_in=z*tan(theta_min) cm
// Outer angle is decided from tan(theta_max)=R_out/z
// R_out=R_in+95 cm (transverse size of the M2 module) 


Double_t fMuchZ1 =0.0; // MuchCave Zin position [cm]
Double_t fAcceptanceTanMin = 0.19; // Acceptance tangent min (11 degree)

//************************************************************


 
// Input parameters for MUCH stations
//********************************************

const Int_t fNst = 1; // Number of stations
 // Sector-type module parameters
// Number of sectors per layer (should be even for symmetry)
// Needs to be fixed with actual numbers

Double_t fActiveLzSector =0.3;  // Active volume thickness [cm]
Double_t fSpacerR1 = 2.8;         // Spacer width in R at low Z[cm]
Double_t fSpacerR2 = 3.5;     // Spacer width in R at high Z[cm]

Double_t fSpacerPhi = 2.8;       // Spacer width in Phi [cm]
Double_t fOverlapR = 0.0;        // Overlap in R direction [cm]

// Station Zceneter [cm] in  the cave reference frame

Double_t fStationZ0=80;
//Double_t fStationZ0=70;   // DE - move 10 cm upstream
Int_t fNlayers=3; // Number of layers
Double_t fLayersDz=10; // distance between the layers
Double_t fCoolLz=1.0;  // thickness of the cooling plate also used as support

/* 
   1 - detailed design (modules at two sides)
   * 0 - simple design (1 module per layer) 
 */



//***********************************************************

// some global variables
TGeoManager* gGeoMan = NULL;  // Pointer to TGeoManager instance
TGeoVolume*  gModules_station[fNst]; // Global storage for module types

// Forward declarations
void create_materials_from_media_file();
TGeoVolume* CreateStations(int ist);
TGeoVolume* CreateLayers(int istn, int ily);


void create_MUCH_geometry_v18g() {

  // Load needed material definition from media.geo file
  create_materials_from_media_file();
  
  // Get the GeoManager for later usage
  gGeoMan = (TGeoManager*) gROOT->FindObject("FAIRGeom");
  gGeoMan->SetVisLevel(10);  
  
  // Create the top volume 
  TGeoBBox* topbox= new TGeoBBox("", 1000., 1000., 2000.);
  TGeoVolume* top = new TGeoVolume("top", topbox, gGeoMan->GetMedium("air"));
  gGeoMan->SetTopVolume(top);


  TGeoVolume* much = new TGeoVolumeAssembly(geoVersion);
  top->AddNode(much, 1);
  TGeoVolume *sttn = new TGeoVolumeAssembly("station"); //change name from Station ->station
   much->AddNode(sttn,1);

  for (Int_t istn = 0; istn < 1; istn++) { // 1 Station
    
    gModules_station[istn] = CreateStations(istn);
 
    sttn->AddNode(gModules_station[istn],istn);
    
  }

  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.000001);
  gGeoMan->PrintOverlaps();
  //  gGeoMan->Test();


  
  much->Export(FileNameSim);   // an alternative way of writing the much

  TFile* outfile = new TFile(FileNameSim, "UPDATE");
  TGeoTranslation* much_placement = new TGeoTranslation("much_trans", 0., 0., 0.);
  much_placement->Write();
  outfile->Close();

  outfile = new TFile(FileNameGeo,"RECREATE");
  gGeoMan->Write();  // use this if you want GeoManager format in the output
  outfile->Close();

  top->Draw("ogl");
 
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
 
  FairGeoMedium* MUCHwolfram = geoMedia->getMedium(W);
  geoBuild->createMedium(MUCHwolfram);

  FairGeoMedium* MUCHcarbon = geoMedia->getMedium(C);
  geoBuild->createMedium(MUCHcarbon);

  FairGeoMedium* MUCHargon = geoMedia->getMedium(activemedium);
  geoBuild->createMedium(MUCHargon);

  FairGeoMedium* MUCHnoryl = geoMedia->getMedium(spacermedium);
  geoBuild->createMedium(MUCHnoryl);

  FairGeoMedium*aluminium  = geoMedia->getMedium(coolmedium);
  // geoBuild->createMedium(MUCHcool);
  geoBuild->createMedium(aluminium);
}


TGeoVolume * CreateStations(int ist){
  
  TString stationName = Form("muchstation%02i",ist+1);

  TGeoVolumeAssembly* station = new TGeoVolumeAssembly(stationName);//, shStation, air);

  

  TGeoVolume*  gLayer[4];
  
  for (int ii=0;ii<3;ii++){  // 3 Layers
  

     gLayer[ii] = CreateLayers(ist, ii);
     station->AddNode(gLayer[ii],ii);

  }

  return station;
}


TGeoVolume * CreateLayers(int istn, int ily){

  TString layerName = Form("muchstation%02ilayer%i",istn+1,ily+1);
  TGeoVolumeAssembly* volayer = new TGeoVolumeAssembly(layerName);

  //Double_t DeltaR=80.0; // transverse dimension of the module
  
  Double_t stGlobalZ0 = fStationZ0 + fMuchZ1; //z position of station center (midplane) [cm]
  Double_t stDz = ((fNlayers - 1) * fLayersDz + fCoolLz+2*fActiveLzSector)/2.;
  Double_t stGlobalZ2 = stGlobalZ0 + stDz;
  Double_t stGlobalZ1 = stGlobalZ0 - stDz;

  //Double_t rmin = stGlobalZ1 * fAcceptanceTanMin;
  // Double_t rmax = rmin+ fSpacerR + DeltaR;

  Int_t Nsector=16.0;// need to be hard coded to match with station 1 of SIS100

  //cout<<" Nsector "<<Nsector<<endl;
  
  Double_t layerZ0 = (ily - (fNlayers - 1) / 2.) * fLayersDz;
  Double_t layerGlobalZ0 = layerZ0 + stGlobalZ0;
  Double_t sideDz = fCoolLz/2. + fActiveLzSector/2.; // distance between side's and layer's centers

 
  Double_t moduleZ = sideDz; // Z position of the module center in the layer cs
  
  Double_t phi0 = TMath::Pi()/Nsector; // azimuthal half widh of each module
  //Double_t ymin = rmin+fSpacerR;

  // Double_t ymax = rmax;    
  
  //define the dimensions of the trapezoidal module

  //Use hard coded values for mini-cbm 
  Double_t dy  = 40.0;//(ymax-ymin)/2.; //y (length)

  Double_t dx1 = 3.75;//ymin*TMath::Tan(phi0)+fOverlapR/TMath::Cos(phi0);  // large x
  Double_t dx2 = 20;//ymax*TMath::Tan(phi0)+fOverlapR/TMath::Cos(phi0); // small x
  Double_t dz  = fActiveLzSector/2.; // thickness
  
 

//define the spacer dimensions      
  Double_t tg = (dx2-dx1)/2/dy;
  Double_t dd1 = fSpacerPhi*tg;
  Double_t dd2 = fSpacerPhi*sqrt(1+tg*tg);
  Double_t sdx1 = dx1+dd2-dd1;
  Double_t sdx2 = dx2+dd2+dd1; 
  Double_t sdy1  = dy+fSpacerR1; // frame width added 
  Double_t sdy2  = dy+fSpacerR2; // frame width added
  Double_t sdz  = dz-0.1;

  
  // Define the (cooling plate) diemnsions (hardcoded for mcbm)
  Double_t dy_s= sdy2;//46.5;//dy+2.0;
  Double_t dx1_s  = sdx1;//27.0;  //dx1+2.0 ;// large x
  Double_t dx2_s= sdx2;//27.0;  //dx2+2.0;// x 
  Double_t dz_s=fCoolLz/2.;//


  

  TVector3 pos;
  TVector3 size = TVector3(0.0, 0.0, fActiveLzSector);


  
  // Now start adding the GEM modules  
  
  for (Int_t iModule=0; iModule<1; iModule++){ 
    // if (iModule!=0) continue;
    //    Double_t phi  = 2 * phi0 * (iModule + 0.5);  // add 0.5 to not overlap with y-axis for left-right layer separation

    // Position of the module will depend on Phi
    // Set Phi=180 degree 6 o'clock position
    // Set Phi=90 degree 3 o'clock position

    //    Double_t phi=TMath::Pi()/2.0;
    Double_t phi=0;  // do not blow up yMin in CbmMuchGeoScheme to see points

    Bool_t isBack = iModule%2; 
    Char_t cside  = (isBack==1) ? 'b' : 'f'; 
     
      // correct the x, y positions
//      pos[0] = -(ymin+dy)*sin(phi);
//      pos[1] =  (ymin+dy)*cos(phi);
      pos[0] =  0;  // DE - do not displace mMUCH in x
      pos[1] = 15;  // DE - move upwards in y [cm]

      // different z positions for odd/even modules
      //  pos[2] = (isBack ? 1 : -1)*moduleZ + layerGlobalZ0; 
      pos[2] = layerGlobalZ0; 

      TGeoMedium* argon = gGeoMan->GetMedium(activemedium); // active medium
      TGeoMedium* noryl = gGeoMan->GetMedium(spacermedium); // spacer medium
      TGeoMedium* aluminium = gGeoMan->GetMedium(coolmedium); // cool medium
      
      // Define and place the trapezoidal GEM module in X-Y plane
      TGeoTrap* shape = new TGeoTrap(dz,0,phi,dy,dx1,dx2,0,dy,dx1,dx2,0);

      // TGeoTrap* shape = new TGeoTrap(dz,0,0,dy,dx1,dx2,0,dy,dx1,dx2,0);
      
      
      shape->SetName(Form("shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule));
      TString activeName = Form("muchstation%02ilayer%i%cactive%03igasArgon",istn+1,ily+1,cside,iModule+1);
      TGeoVolume* voActive = new TGeoVolume(activeName,shape,argon);
      voActive->SetLineColor(kGreen);

      // Define the trapezoidal spacers 
      TGeoTrap* shapeFrame = new TGeoTrap(sdz,0,phi,sdy1,sdx1,sdx2,0,sdy2,sdx1,sdx2,0);

      //TGeoTrap* shapeFrame = new TGeoTrap(sdz,0,0,sdy,sdx1,sdx2,0,sdy,sdx1,sdx2,0);

      shapeFrame->SetName(Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole", istn, ily, cside, iModule));
      TString expression = Form("shStation%02iLayer%i%cModule%03iFullFrameNoHole-shStation%02iLayer%i%cModule%03iActiveNoHole", istn, ily, cside, iModule, istn, ily, cside, iModule);
      TGeoCompositeShape* shFrame = new TGeoCompositeShape(Form("shStation%02iLayer%i%cModule%03iFrameNoHole", istn, ily, cside, iModule), expression);
      TString frameName = Form("muchstation%02ilayer%i%csupport%03i",istn+1,ily+1,cside,iModule+1);
      TGeoVolume* voFrame = new TGeoVolume(frameName,shFrame,noryl);  // add a name to the frame
      voFrame->SetLineColor(kMagenta);


      // Define the trapezoidal  (cooling plates)

      // TGeoTrap* cool = new TGeoTrap(dz_s,0,phi,dy_s,dx1_s,dx2_s,0,dy_s,dx1_s,dx2_s,0);

      TGeoTrap* cool = new TGeoTrap(dz_s,0,phi,dy_s,dx1_s,dx2_s,0,dy_s,dx1_s,dx2_s,0);

      cool->SetName(Form("shStation%02iLayer%i%cModule%03icool", istn, ily, cside, iModule));

      TString CoolName = Form("muchstation%02ilayer%i%ccool%03iAluminum",istn+1,ily+1,cside,iModule+1);
      TGeoVolume* voCool = new TGeoVolume(CoolName,cool,aluminium);
      voCool->SetLineColor(kYellow);



      // Calculate the phi angle of the sector where it has to be placed 
      Double_t angle = 180. / TMath::Pi() * phi;  // convert angle phi from rad to deg
     
      TGeoTranslation*trans2=new TGeoTranslation("",pos[0],pos[1],pos[2]); //for module and frame

      TGeoTranslation*trans3=new TGeoTranslation("",pos[0],pos[1],pos[2]+0.65); //for module and frame


    
      TGeoRotation *r2 = new TGeoRotation("r2");
      //rotate in the vertical plane (per to z axis) with angle 
      //      r2->RotateZ(angle);

// DE       cout << "DE " << phi << endl;
// DE       cout << "DE " << angle << endl;
// DE       cout << "DE " << phi0 << endl;
// DE       cout << "DE " << 180. / TMath::Pi() * phi0 << endl;

      // r2->RotateZ(180.0);  // DE - 6 o'clock position
      r2->RotateZ(180.0-(180. / TMath::Pi() * phi0));  // DE - 6 o'clock position, left side vertical

      // r2->RotateZ(180.0-11.25);  // DE - 6 o'clock position, left side vertical
      
      // give rotation to set them in horizontal plane
      //r2->RotateZ(90.0);//TMath::Pi());

      TGeoHMatrix *incline_mod = new TGeoHMatrix("");

      (*incline_mod) =  (*trans2) * (*r2);  // OK
      
      volayer->AddNode(voFrame, iModule, incline_mod);  // add frame
      volayer->AddNode(voActive, iModule, incline_mod); // add active volume

      TGeoHMatrix *incline_mod1 = new TGeoHMatrix("");

      (*incline_mod1) =  (*trans3) * (*r2);  // for cooling

       volayer->AddNode(voCool, iModule, incline_mod1);  // cooling plate

      
  }	     
  
  return volayer;
}

