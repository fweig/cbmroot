// -------------------------------------------------------------------------
// -----                  CbmMuchGeoScheme source file               -----
//------                  Modilfied 21/06/2019 by Ekata Nandy(Detector type for GEM & RPC included)         -----
//------                  Modilfied 18/10/2017 by Omveer Singh         -----
// -----                  Created 18/02/08  by E. Kryshen
// -------------------------------------------------------------------------
#include "CbmMuchGeoScheme.h"

#include "CbmMuchAddress.h"          // for CbmMuchAddress
#include "CbmMuchLayer.h"            // for CbmMuchLayer
#include "CbmMuchLayerSide.h"        // for CbmMuchLayerSide
#include "CbmMuchModule.h"           // for CbmMuchModule
#include "CbmMuchModuleGem.h"        // for CbmMuchModuleGem
#include "CbmMuchModuleGemRadial.h"  // for CbmMuchModuleGemRadial
#include "CbmMuchStation.h"          // for CbmMuchStation

#include <FairLogger.h>  // for LOG

#include <TArrayD.h>       // for TArrayD
#include <TArrayI.h>       // for TArrayI
#include <TClonesArray.h>  // for TClonesArray
#include <TFile.h>         // for TFile, gFile
#include <TGeoArb8.h>      // for TGeoTrap
#include <TGeoBBox.h>      // for TGeoBBox
#include <TGeoManager.h>   // for TGeoManager, gGeoManager
#include <TGeoNode.h>      // for TGeoNode
#include <TGeoVolume.h>    // for TGeoVolume
#include <TMath.h>         // for Cos, Sqrt
#include <TObjArray.h>     // for TObjArray
#include <TObject.h>       // for TObject
#include <TVector3.h>      // for TVector3


#include <cassert>    // for assert
#include <math.h>     // for sqrt
#include <stdexcept>  // for out_of_range
#include <utility>    // for pair
#include <vector>

using std::vector;

CbmMuchGeoScheme* CbmMuchGeoScheme::fInstance = nullptr;
Bool_t CbmMuchGeoScheme::fInitialized         = kFALSE;
Bool_t CbmMuchGeoScheme::fModulesInitialized  = kFALSE;

// -------------------------------------------------------------------------
CbmMuchGeoScheme::CbmMuchGeoScheme()
  : TObject()
  , fGeoPathHash(0)
  , fCurrentVolume(nullptr)
  , fVolumeBoxShape(nullptr)
  , fVolumeTrapShape(nullptr)
  , fGlobal()
  , fGlobalTrap()
  , fGlobalMatrix(nullptr)
  , fStation(0)
  , fLayer(0)
  , fActive(0)
  , fGeoID(0)
  ,
  //fIsSimulation(kFALSE),
  fModules()
  , fSides()
  , fMapSides()
  , fStations(nullptr)
  , fAbsorbers(new TObjArray())
  , fMuchCave(nullptr)
  , fMuchZ1(0.)
  , fAcceptanceTanMin(0.)
  , fAcceptanceTanMax(0.)
  , fNabs(0)
  , fNst(0)
  , fActiveLx(0.)
  , fActiveLy(0.)
  , fActiveLz(0.)
  , fSpacerLx(0.)
  , fSpacerLy(0.)
  , fOverlapY(0.)
  , fStrawLz(0.)
  , fNSectorsPerLayer(0)
  , fActiveLzSector(0.)
  , fSpacerR(0.)
  , fSpacerPhi(0)
  , fOverlapR(0.)
  , fAbsorberZ1(0)
  , fAbsorberLz(0)
  , fAbsorberMat(0)
  , fStationZ0(0)
  , fNlayers(0)
  , fDetType(0)
  , fLayersDz(0)
  , fSupportLz(0)
  , fModuleDesign(0)
  , muchSt(nullptr)
  , muchLy(nullptr)
  , muchLySd(nullptr)
  , Rmin(-1.)
  , Rmax(-1.)
  , Dx2(-1.) {
  LOG(debug) << "CbmMuchGeoScheme created";
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchGeoScheme::~CbmMuchGeoScheme() {

  if (fInstance != nullptr) delete fInstance;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
CbmMuchGeoScheme* CbmMuchGeoScheme::Instance() {

  if (!fInstance) fInstance = new CbmMuchGeoScheme();
  return fInstance;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchGeoScheme::Init(TObjArray* stations, Int_t flag) {

  if (!fInitialized) {
    fStations = stations;
    //LOG(info) <<" enter ini loop  check"<<fStations->GetEntriesFast()<<"    "<<stations->GetEntriesFast();
    fGeoID       = flag;
    fInitialized = kTRUE;
  }
  LOG(debug) << "CbmMuchGeoScheme init successful";
  InitModules();
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchGeoScheme::Init(TString digiFileName, Int_t flag) {

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* file         = new TFile(digiFileName);
  TObjArray* stations = (TObjArray*) file->Get("stations");
  file->Close();
  file->Delete();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  Init(stations, flag);
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchGeoScheme::InitModules() {

  if (!fModulesInitialized) {


    if (!fStations) Fatal("InitModules", "No input array of stations.");
    Int_t incSides = 0;
    fMapSides.clear();
    fSides.clear();
    fModules.clear();

    //LOG(info) <<"Total No. of Stations  "<<"  "<<GetNStations()<<" absorbers "<<GetNAbsorbers();
    for (Int_t iStation = 0; iStation < GetNStations(); iStation++) {

      const CbmMuchStation* station = GetStation(iStation);
      if (!station) continue;

      assert(iStation
             == CbmMuchAddress::GetStationIndex(station->GetDetectorId()));
      vector<CbmMuchLayerSide*> sides;
      vector<CbmMuchModule*> modules;

      for (Int_t iLayer = 0; iLayer < station->GetNLayers(); iLayer++) {
        CbmMuchLayer* layer = station->GetLayer(iLayer);
        if (!layer) continue;

        assert(iLayer == CbmMuchAddress::GetLayerIndex(layer->GetDetectorId()));
        for (Int_t iSide = 0; iSide < 2; iSide++) {
          CbmMuchLayerSide* side = (CbmMuchLayerSide*) layer->GetSide(iSide);
          if (!side) continue;
          assert(iSide
                 == CbmMuchAddress::GetLayerSideIndex(side->GetDetectorId()));
          if (side->GetNModules() != 0)
            fMapSides[side->GetDetectorId()] = incSides++;
          sides.push_back(side);

          for (Int_t iModule = 0; iModule < side->GetNModules(); iModule++) {
            CbmMuchModule* mod = side->GetModule(iModule);
            if (!mod) continue;

            assert(iModule
                   == CbmMuchAddress::GetModuleIndex(mod->GetDetectorId()));
            assert(iStation
                   == CbmMuchAddress::GetStationIndex(mod->GetDetectorId()));
            if (!mod->InitModule()) continue;
            modules.push_back(mod);
          }  // Modules
        }    // Sides
      }      // Layers
      fSides.push_back(sides);
      fModules.push_back(modules);
    }  // Stations

    fModulesInitialized = kTRUE;
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchStation* CbmMuchGeoScheme::GetStation(Int_t iStation) const {

  if (!fStations) return nullptr;
  Bool_t result = (iStation >= 0) || (iStation < fStations->GetEntriesFast());

  return result ? (CbmMuchStation*) fStations->At(iStation) : nullptr;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
CbmMuchLayer* CbmMuchGeoScheme::GetLayer(Int_t iStation, Int_t iLayer) const {

  CbmMuchStation* station = GetStation(iStation);
  return station ? station->GetLayer(iLayer) : nullptr;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchLayerSide* CbmMuchGeoScheme::GetLayerSide(Int_t iStation,
                                                 Int_t iLayer,
                                                 Bool_t iSide) const {

  CbmMuchLayer* layer = GetLayer(iStation, iLayer);
  return layer ? layer->GetSide(iSide) : nullptr;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchModule* CbmMuchGeoScheme::GetModule(Int_t iStation,
                                           Int_t iLayer,
                                           Bool_t iSide,
                                           Int_t iModule) const {

  CbmMuchLayerSide* side = GetLayerSide(iStation, iLayer, iSide);
  return side ? side->GetModule(iModule) : nullptr;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchStation* CbmMuchGeoScheme::GetStationByDetId(Int_t detId) const {

  Int_t iStation = CbmMuchAddress::GetStationIndex(detId);
  assert(iStation < GetNStations());
  return GetStation(iStation);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchLayer* CbmMuchGeoScheme::GetLayerByDetId(Int_t detId) const {

  CbmMuchStation* station = GetStationByDetId(detId);
  Int_t iLayer            = CbmMuchAddress::GetLayerIndex(detId);
  assert(iLayer < station->GetNLayers());
  return station ? station->GetLayer(iLayer) : nullptr;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchLayerSide* CbmMuchGeoScheme::GetLayerSideByDetId(Int_t detId) const {

  CbmMuchLayer* layer = GetLayerByDetId(detId);
  Int_t iSide         = CbmMuchAddress::GetLayerSideIndex(detId);
  assert(iSide < 2);
  return layer ? layer->GetSide(iSide) : nullptr;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchModule* CbmMuchGeoScheme::GetModuleByDetId(Int_t detId) const {

  CbmMuchLayerSide* side = GetLayerSideByDetId(detId);
  Int_t iModule          = CbmMuchAddress::GetModuleIndex(detId);
  assert(iModule < side->GetNModules());
  return side ? side->GetModule(iModule) : nullptr;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchGeoScheme::CreatePointArrays() {

  vector<CbmMuchModule*> modules = GetModules();
  for (vector<CbmMuchModule*>::iterator it = modules.begin();
       it != modules.end();
       it++) {
    CbmMuchModule* module = (*it);
    module->SetPoints(new TClonesArray("CbmVisPoint", 1));
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchGeoScheme::CreateHitArrays() {

  vector<CbmMuchModule*> modules = GetModules();
  for (vector<CbmMuchModule*>::iterator it = modules.begin();
       it != modules.end();
       it++) {
    CbmMuchModule* module = (*it);
    if (module->GetDetectorType() == 1) {
      CbmMuchModuleGem* mod = (CbmMuchModuleGem*) module;
      mod->SetHits(new TClonesArray("CbmVisPixelHit", 1));
      /*
    } else if (module->GetDetectorType()==2) {
      CbmMuchModuleStraws* mod = (CbmMuchModuleStraws*)module;
      mod->SetHits(new TClonesArray("CbmVisStripHit",1));
*/
    }
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchGeoScheme::CreateClusterArrays() {

  vector<CbmMuchModule*> modules = GetModules();
  for (vector<CbmMuchModule*>::iterator it = modules.begin();
       it != modules.end();
       it++) {
    CbmMuchModule* module = (*it);
    if (module->GetDetectorType() != 1) continue;
    CbmMuchModuleGem* mod = (CbmMuchModuleGem*) module;
    mod->SetClusters(new TClonesArray("CbmVisMuchCluster", 1));
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchGeoScheme::ClearPointArrays() {

  vector<CbmMuchModule*> modules = GetModules();
  for (vector<CbmMuchModule*>::iterator it = modules.begin();
       it != modules.end();
       it++) {
    (*it)->GetPoints()->Clear();
  }
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchGeoScheme::ClearHitArrays() {

  vector<CbmMuchModule*> modules = GetModules();
  for (vector<CbmMuchModule*>::iterator it = modules.begin();
       it != modules.end();
       it++) {
    (*it)->GetHits()->Clear();
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchGeoScheme::ClearClusterArrays() {

  vector<CbmMuchModule*> modules = GetModules();
  for (vector<CbmMuchModule*>::iterator it = modules.begin();
       it != modules.end();
       it++) {
    CbmMuchModule* module = (*it);
    if (module->GetDetectorType() != 1) continue;
    CbmMuchModuleGem* mod = (CbmMuchModuleGem*) module;
    mod->GetClusters()->Clear();
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
vector<CbmMuchLayerSide*>
CbmMuchGeoScheme::GetLayerSides(Int_t iStation) const {

  try {
    return fSides.at(iStation);
  } catch (const std::out_of_range& exc) {
    Fatal("GetLayerSides", "No input array of stations.");
  }
  return vector<CbmMuchLayerSide*>();
}

//Int_t CbmMuchGeoScheme::GetLayerSideNr(Int_t detId){
//  int i = 0;
//  Int_t sideId = GetLayerSideByDetId(detId)->GetDetectorId();
//  Int_t iStation = GetStationIndex(detId);
//  for(Int_t iSt=0; iSt<=iStation; iSt++){
//    vector<CbmMuchLayerSide*> sides = GetLayerSides(iSt);
//    for(Int_t iSide=0; iSide<sides.size(); iSide++){
//      i++;
//    }
//  }
//  printf("%i, %i\n",fMapSides[sideId] + 1, i);
//  assert(fMapSides[sideId] + 1 == i);
//  return i;
//}

Int_t CbmMuchGeoScheme::GetLayerSideNr(Int_t detId) const {
  Int_t sideId = GetLayerSideByDetId(detId)->GetDetectorId();
  if (fMapSides.find(sideId) == fMapSides.end())
    Fatal("GetLayerSideNr", "Wrong side id or no modules in the layer side");
  return fMapSides.find(sideId)->second + 1;
}

// -------------------------------------------------------------------------
void CbmMuchGeoScheme::NavigateTo(const TString& path) {

  gGeoManager->cd(path.Data());
  //    fGeoPathHash;
  fGeoPathHash      = path.Hash();
  fCurrentVolume    = gGeoManager->GetCurrentVolume();
  fVolumeBoxShape   = (TGeoBBox*) fCurrentVolume->GetShape();
  Double_t local[3] = {0., 0., 0.};
  gGeoManager->LocalToMaster(local, fGlobal);
}

//------------------------------------------------------
Double_t CbmMuchGeoScheme::GetSizeX(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fVolumeBoxShape->GetDX();
}


Double_t CbmMuchGeoScheme::GetSizeY(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fVolumeBoxShape->GetDY();
}


Double_t CbmMuchGeoScheme::GetSizeZ(const TString& path) {
  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fVolumeBoxShape->GetDZ();
}


Double_t CbmMuchGeoScheme::GetZ(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fGlobal[2];
}


Double_t CbmMuchGeoScheme::GetY(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fGlobal[1];
}


Double_t CbmMuchGeoScheme::GetX(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateTo(path); }
  return fGlobal[0];
}
//-----------------------------------------------------------------------------------------------------
void CbmMuchGeoScheme::ExtractGeoParameter(TGeoNode* ncave,
                                           const char* volumeName) {

  TObjArray* caveArr = ncave->GetNodes();

  for (int iSystem = 0; iSystem < caveArr->GetEntriesFast(); iSystem++) {
    TGeoNode* SystemNode = static_cast<TGeoNode*>(caveArr->At(iSystem));

    if (!TString(SystemNode->GetName()).Contains(volumeName)) continue;
    TObjArray* MuchObj = SystemNode->GetNodes();

    for (Int_t iMuchObj = 0; iMuchObj < MuchObj->GetEntriesFast(); iMuchObj++) {

      TGeoNode* MuchObjNode = static_cast<TGeoNode*>(MuchObj->At(iMuchObj));

      // if (!TString(MuchObjNode->GetName()).Contains("Station")) continue;
      if (!TString(MuchObjNode->GetName()).Contains("station")) continue;
      TString MuchObjPath = TString("/") + ncave->GetName() + "/"
                            + SystemNode->GetName() + "/"
                            + MuchObjNode->GetName();

      StationNode(MuchObjNode, MuchObjPath);  //StationNode
    }                                         //iMuchObj

  }  //iSystem
}  //SetParameters

//---------------------------------------------------------------------------------------------
void CbmMuchGeoScheme::StationNode(TGeoNode* fSNode, TString Path) {

  TObjArray* stations = fSNode->GetNodes();
  //fStations=stations; // by PPB 1/11/2017
  fNst = stations->GetEntriesFast();
  //  LOG(info) <<"check stations   "<<fNst;
  fStationZ0.Set(fNst);     // Station Zceneter [cm]
  fNlayers.Set(fNst);       // Number of layers
  fDetType.Set(fNst);       // Detector type
  fLayersDz.Set(fNst);      // Distance between layers [cm]
  fSupportLz.Set(fNst);     // Support thickness [cm]
  fLayersDz.Set(fNst);      //
  fModuleDesign.Set(fNst);  // Module design (0/1)
  fNSectorsPerLayer.Set(
    fNst);  // Number of sectors per layer in sector GEM geometry


  for (Int_t iStation = 0; iStation < fNst; iStation++) {
    TGeoNode* station   = static_cast<TGeoNode*>(stations->At(iStation));
    TString StationPath = Path + "/" + station->GetName();
    TObjArray* layers   = station->GetNodes();
    fNlayers[iStation]  = layers->GetEntriesFast();


    //   fDetType[iStation]=3;
    //      fLayersDz[iStation]=10.0;

    //-----------------------------Layer1-------------------------------------------------------
    TGeoNode* layer1 =
      static_cast<TGeoNode*>(layers->At(0));  //first layer position of station
    TString Layer1Path          = StationPath + "/" + layer1->GetName();
    TObjArray* Supportlayer1    = layer1->GetNodes();
    TGeoNode* Supportlayer1Node = static_cast<TGeoNode*>(Supportlayer1->At(0));

    TString Supportlayer1Path = Layer1Path + "/" + Supportlayer1Node->GetName();
    Double_t fLayer1Z0;
    //if(Supportlayer2Path.Contains("mcbm")) fLayer2Z0=GetModuleZ(Supportlayer2Path);
    if (fGeoID == 1)
      fLayer1Z0 = GetModuleZ(Supportlayer1Path);
    else
      fLayer1Z0 = GetZ(Supportlayer1Path);


    //-----------------------------Layer2-------------------------------------------------------
    TGeoNode* layer2 =
      static_cast<TGeoNode*>(layers->At(1));  //second layer position of station
    TString Layer2Path = StationPath + "/" + layer2->GetName();

    TObjArray* Supportlayer2    = layer2->GetNodes();
    TGeoNode* Supportlayer2Node = static_cast<TGeoNode*>(Supportlayer2->At(0));

    TString Supportlayer2Path = Layer2Path + "/" + Supportlayer2Node->GetName();

    Double_t fLayer2Z0;
    //if(Supportlayer2Path.Contains("mcbm")) fLayer2Z0=GetModuleZ(Supportlayer2Path);
    if (fGeoID == 1)
      fLayer2Z0 = GetModuleZ(Supportlayer2Path);
    else
      fLayer2Z0 = GetZ(Supportlayer2Path);
    //


    fLayersDz[iStation] = fLayer2Z0 - fLayer1Z0;

    //if(Supportlayer3Path.Contains("mcbm")) fSupportLz[iStation]=2.0*GetModuleDZ(Supportlayer3Path);
    if (fGeoID == 1)
      fSupportLz[iStation] = 2.0 * GetModuleDZ(Supportlayer1Path);
    else
      fSupportLz[iStation] = 2.0 * GetSizeZ(Supportlayer1Path);
    //LOG(info) <<fSupportLz[iStation]<<"  "<<fLayersDz[iStation];


    Double_t PosY = 0., Phi = 0., Dy = 0.;

    if (fNlayers[iStation] == 3) {
      //------------------------------Layer3-----------------------------------------------------------
      TGeoNode* layer3   = static_cast<TGeoNode*>(layers->At(2));
      TString Layer3Path = StationPath + "/" + layer3->GetName();

      TObjArray* Supportlayer3 = layer3->GetNodes();
      TGeoNode* Supportlayer3Node =
        static_cast<TGeoNode*>(Supportlayer3->At(0));
      TString Supportlayer3Path =
        Layer3Path + "/" + Supportlayer3Node->GetName();

      //    Double_t  fLayer3Z0;
      //if(Supportlayer3Path.Contains("mcbm"))  fLayer3Z0=GetModuleZ(Supportlayer3Path);
      //    if(fGeoID==1)  fLayer3Z0=GetModuleZ(Supportlayer3Path);
      //                else fLayer3Z0 = GetZ(Supportlayer3Path);
      TGeoNode* Activelayer3Node = static_cast<TGeoNode*>(Supportlayer3->At(1));
      TString Activelayer3Path = Layer3Path + "/" + Activelayer3Node->GetName();

      PosY = GetModuleY(Activelayer3Path);
      // Double_t PosX=GetModuleX(Activelayer3Path);
      Phi                  = GetModulePhi(Activelayer3Path);
      Dy                   = GetModuleH1(Activelayer3Path);
      fActiveLzSector      = 2.0 * GetModuleDZ(Activelayer3Path);
      fStationZ0[iStation] = fLayer2Z0;
    } else {
      TGeoNode* Activelayer2Node = static_cast<TGeoNode*>(Supportlayer2->At(1));
      TString Activelayer2Path = Layer2Path + "/" + Activelayer2Node->GetName();

      PosY = GetModuleY(Activelayer2Path);
      // Double_t PosX=GetModuleX(Activelayer3Path);
      Phi                  = GetModulePhi(Activelayer2Path);
      Dy                   = GetModuleH1(Activelayer2Path);
      fActiveLzSector      = 2.0 * GetModuleDZ(Activelayer2Path);
      fStationZ0[iStation] = (fLayer2Z0 - fLayer1Z0) / 2.;
    }


    //chaned by PPB on 16.11.2017
    //Double_t yMin = -(PosX/TMath::Sin(Phi))-Dy;
    Double_t yMin = (PosY / TMath::Cos(Phi)) - Dy;
    Double_t yMax = 2 * Dy + yMin;
    LOG(info) << " Geo Scheme "
              << " posY  " << PosY << " phi " << Phi << "  Dy " << Dy;
    Rmax = yMax;
    Rmin = yMin;  //-2.0; // Spacer width[cm] = 2.0


    muchSt = new CbmMuchStation(iStation, fStationZ0[iStation]);
    muchSt->SetRmin(Rmin);
    muchSt->SetRmax(Rmax);

    if (Supportlayer1->GetEntriesFast() > 0) fModuleDesign[iStation] = 1;
    muchSt->SetModuleDesign(fModuleDesign[iStation]);


    LayerNode(station, iStation, StationPath);


    muchLy         = muchSt->GetLayer(0);
    Double_t supDx = muchLy->GetSupportDx();
    Double_t supDy = muchLy->GetSupportDy();
    //    Double_t supDz  = muchLy->GetSupportDz();
    muchSt->SetTubeRmin(muchSt->GetRmin());
    muchSt->SetTubeRmax(TMath::Sqrt(supDx * supDx + supDy * supDy) + 10);
    //LOG(info) <<" fill fStations array "<<iStation<<" z cent "<<fStationZ0[iStation];
    fStations->Add(muchSt);

  }  //Station
}  //StationNode
//---------------------------------------------------------------------------------------------------------

void CbmMuchGeoScheme::LayerNode(TGeoNode* StNode,
                                 Int_t iStation,
                                 TString StPath) {

  TObjArray* layerArray = StNode->GetNodes();
  for (Int_t iLayer = 0; iLayer < layerArray->GetEntriesFast(); iLayer++) {
    TGeoNode* layerNode = static_cast<TGeoNode*>(layerArray->At(iLayer));
    TString layerPath   = StPath + "/" + layerNode->GetName();

    ModuleNode(layerNode, iStation, iLayer, layerPath);

    //Set support shape
    muchLy->SetSupportDx(sqrt(Rmax * Rmax + Dx2 * Dx2));
    muchLy->SetSupportDy(sqrt(Rmax * Rmax + Dx2 * Dx2));
    muchLy->SetSupportDz(fSupportLz[iStation] / 2.);

    muchSt->AddLayer(muchLy);

  }  //iLayer
}
//---------------------------------------------------------------------------------------------------------------------------------

void CbmMuchGeoScheme::ModuleNode(TGeoNode* layerNode,
                                  Int_t iStation,
                                  Int_t iLayer,
                                  TString layerPath) {


  TObjArray* moduleArray = layerNode->GetNodes();
  for (Int_t iModule = 0; iModule < moduleArray->GetEntriesFast(); iModule++) {
    TGeoNode* moduleNode = static_cast<TGeoNode*>(moduleArray->At(iModule));

    TString modulePath = layerPath + "/" + moduleNode->GetName();

    ActiveModuleNode(moduleNode, iStation, iLayer, iModule, modulePath);


  }  //iModule
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void CbmMuchGeoScheme::ActiveModuleNode(TGeoNode* moduleNode,
                                        Int_t iStation,
                                        Int_t iLayer,
                                        Int_t /*iModule*/,
                                        TString modulePath) {

  TString modName = moduleNode->GetName();


  if (modName.Contains("support")) {


    Double_t layerGlobalZ0;
    // if(modulePath.Contains("mcbm")) layerGlobalZ0=GetModuleZ(modulePath);
    if (fGeoID == 1)
      layerGlobalZ0 = GetModuleZ(modulePath);
    else
      layerGlobalZ0 = GetZ(modulePath);

    Double_t layerZ0 =
      (iLayer - (fNlayers[iStation] - 1) / 2.) * fLayersDz[iStation];
    Double_t sideDz = fSupportLz[iStation] / 2. + fActiveLzSector / 2.;

    muchLy = new CbmMuchLayer(
      iStation, iLayer, layerGlobalZ0, layerZ0);  //CbmMuchLayer Class
    muchLy->GetSideB()->SetZ(layerGlobalZ0 + sideDz);
    muchLy->GetSideF()->SetZ(layerGlobalZ0 - sideDz);
  }


  if (modName.Contains("active")) {

    gGeoManager->cd(modulePath.Data());
    //   TGeoNode* moduleNode = gGeoManager->GetMother(0);
    moduleNode = gGeoManager->GetMother(0);
    //    Int_t nModule=moduleNode->GetNumber();

    Bool_t iSide;
    if (modName.Contains("factive")) iSide = 0;
    if (modName.Contains("bactive")) iSide = 1;
    TVector3 pos;
    pos[0] = GetModuleX(modulePath);
    pos[1] = GetModuleY(modulePath);
    pos[2] = GetModuleZ(modulePath);
    //   LOG(info) <<" positions  "<<pos[0]<<"  "<<pos[1]<<"  "<<pos[2];
    Double_t Dz = GetModuleDZ(modulePath);
    //    Double_t Phi = GetModulePhi(modulePath);
    Double_t Dy  = GetModuleH1(modulePath);
    Double_t Dx1 = GetModuleBl1(modulePath);
    Dx2          = GetModuleTl1(modulePath);
    //LOG(info) <<" positions  "<<pos[0]<<"  "<<pos[1]<<"  "<<pos[2]<<" phi "<<Phi;

    // Is this formula correct? Check Omveer (PPB 16.11.2017)
    //    Double_t yMin = (pos[1]/TMath::Cos(Phi))-Dy;
    //    Double_t yMax = 2*Dy + yMin;
    // Double_t Rmin = yMin-2.0; // Spacer width[cm] = 2.0
    // Why is R0 required?
    //    Double_t R0 = yMax-Dy;
    // Double_t Rmax = yMax;


    Int_t DetType = -1;
    if (modName.Contains("gasArgon")) DetType = 3;  // Detector type 3 for GEM
    if (modName.Contains("rpcgas")) DetType = 4;    // Detector type 4 for RPC
    //    LOG(info) <<" detector type "<<DetType;

    muchLySd = muchLy->GetSide(iSide);
    muchLySd->AddModule(new CbmMuchModuleGemRadial(
      DetType,
      iStation,
      iLayer,
      iSide,
      muchLySd->GetNModules(),
      pos,
      Dx1,
      Dx2,
      Dy,
      Dz,
      Rmin));  // Detector type variable added in the class constructor on 03-07-2019


  }  //activeLayer


}  //ActiveModuleNode
//----------------------------------------------------------------------------------------------------------
void CbmMuchGeoScheme::NavigateModule(const TString& path) {
  gGeoManager->cd(path.Data());
  //    fGeoPathHash;
  fGeoPathHash      = path.Hash();
  fCurrentVolume    = gGeoManager->GetCurrentVolume();
  fVolumeTrapShape  = (TGeoTrap*) fCurrentVolume->GetShape();
  Double_t local[3] = {0., 0., 0.};
  gGeoManager->LocalToMaster(local, fGlobalTrap);

}  //NavigateModule

//----------------------------------------------------------------------------------------------------------

Double_t CbmMuchGeoScheme::GetModuleDZ(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fVolumeTrapShape->GetDZ();
}


Double_t CbmMuchGeoScheme::GetModuleZ(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fGlobalTrap[2];
}


Double_t CbmMuchGeoScheme::GetModuleY(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fGlobalTrap[1];
}


Double_t CbmMuchGeoScheme::GetModuleX(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fGlobalTrap[0];
}

Double_t CbmMuchGeoScheme::GetModulePhi(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fVolumeTrapShape->GetPhi();
}

Double_t CbmMuchGeoScheme::GetModuleH1(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fVolumeTrapShape->GetH1();
}

Double_t CbmMuchGeoScheme::GetModuleBl1(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fVolumeTrapShape->GetBl1();
}

Double_t CbmMuchGeoScheme::GetModuleTl1(const TString& path) {

  if (fGeoPathHash != path.Hash()) { NavigateModule(path); }
  return fVolumeTrapShape->GetTl1();
}

// -------------------------------------------------------------------------
Int_t CbmMuchGeoScheme::Intersect(Float_t x,
                                  Float_t y,
                                  Float_t dx,
                                  Float_t dy,
                                  Float_t r) {

  Float_t x1           = x - dx;
  Float_t x2           = x + dx;
  Float_t y1           = y - dy;
  Float_t y2           = y + dy;
  Int_t nCornersInside = 0;
  if (x1 * x1 + y1 * y1 < r * r) nCornersInside++;
  if (x2 * x2 + y1 * y1 < r * r) nCornersInside++;
  if (x1 * x1 + y2 * y2 < r * r) nCornersInside++;
  if (x2 * x2 + y2 * y2 < r * r) nCornersInside++;
  if (nCornersInside == 4) return 2;
  if (nCornersInside) return 1;
  if (!nCornersInside && x1 < r && y1 < 0 && y2 > 0) return 1;
  return 0;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
vector<CbmMuchModule*> CbmMuchGeoScheme::GetModules() const {

  vector<CbmMuchModule*> modules;
  for (Int_t iStation = 0; iStation < GetNStations(); ++iStation) {
    vector<CbmMuchModule*> stationModules = GetModules(iStation);
    for (vector<CbmMuchModule*>::iterator it = stationModules.begin();
         it != stationModules.end();
         it++) {
      CbmMuchModule* module = (*it);
      modules.push_back(module);
      assert(CbmMuchAddress::GetStationIndex(module->GetDetectorId())
             == iStation);
    }
  }
  return modules;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
vector<CbmMuchModuleGem*> CbmMuchGeoScheme::GetGemModules() const {

  vector<CbmMuchModuleGem*> modules;
  for (Int_t iStation = 0; iStation < GetNStations(); ++iStation) {
    vector<CbmMuchModule*> stationModules = GetModules(iStation);
    for (vector<CbmMuchModule*>::iterator it = stationModules.begin();
         it != stationModules.end();
         it++) {
      CbmMuchModule* module = (*it);
      if (module->GetDetectorType() != 3 && module->GetDetectorType() != 4)
        continue;  //Detector type 3:GEM, 4:RPC
      modules.push_back((CbmMuchModuleGem*) module);
      assert(CbmMuchAddress::GetStationIndex(module->GetDetectorId())
             == iStation);
    }
  }
  return modules;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
vector<CbmMuchModule*> CbmMuchGeoScheme::GetModules(Int_t iStation) const {

  try {
    return fModules.at(iStation);
  } catch (const std::out_of_range& exc) {
    Fatal("GetModules", "No input array of stations.");
  }
  return vector<CbmMuchModule*>();
}
// -------------------------------------------------------------------------

ClassImp(CbmMuchGeoScheme)
