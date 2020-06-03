//----------------------------------------------------------------------------------------
//--------------                                  CbmMuch                      -----------
//---------------                   Modified 18/10/2017 by Omveer Singh        -----------
//----------------------------------------------------------------------------------------


#include "CbmMuch.h"

#include "CbmMuchPoint.h"
#include "CbmGeoMuchPar.h"
#include "CbmGeoMuch.h"
#include "CbmGeometryUtils.h"

#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "CbmStack.h"
#include "FairRuntimeDb.h"
#include "FairRun.h"
#include "FairVolume.h"

#include "TObjArray.h"
#include "TClonesArray.h"
#include "TGeoMCGeometry.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TKey.h"

#include "TGeoBBox.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoCompositeShape.h"
#include "TGeoTube.h"
#include "TGeoCone.h"
#include "FairGeoMedia.h"
#include "FairGeoMedium.h"
#include "TGeoBoolNode.h"

#include "CbmMuchAddress.h"
#include "CbmMuchStation.h"
#include "CbmMuchLayer.h"
#include "CbmMuchLayerSide.h"
#include "CbmMuchModule.h"
#include "CbmMuchGeoScheme.h"
#include "TGeoMatrix.h"
#include "CbmMuchModuleGemRadial.h"

#include <iostream>
#include <fstream>
#include <cassert>

#include "TGeoArb8.h"
#include <TFile.h>

using std::cout;
using std::endl;

ClassImp(CbmMuch)

// -----   Default constructor   -------------------------------------------
CbmMuch::CbmMuch() 
: FairDetector(),
  fTrackID(-1),
  fVolumeID(-1),
  fFlagID(0),
  fPosIn(),
  fPosOut(),
  fMomIn(),
  fMomOut(),
  fTime(0.),
  fLength(0.),
  fELoss(0.),
  fPosIndex(0),
  fMuchCollection(new TClonesArray("CbmMuchPoint")),
  kGeoSaved(kFALSE),
  flGeoPar(new TList()),
  fPar(NULL),
  fVolumeName(""),
  fCombiTrans()
{
  ResetParameters();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
CbmMuch::CbmMuch(const char* name, Bool_t active)
  : FairDetector(name, active),
    fTrackID(-1),
    fVolumeID(-1),
    fFlagID(0),
    fPosIn(),
    fPosOut(),
    fMomIn(),
    fMomOut(),
    fTime(0.),
    fLength(0.),
    fELoss(0.),
    fPosIndex(0),
    fMuchCollection(new TClonesArray("CbmMuchPoint")),
    kGeoSaved(kFALSE),
    flGeoPar(new TList()),    
    fPar(NULL),
    fVolumeName(""),
    fCombiTrans()
{
  
  ResetParameters();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}



// -----   Destructor   ----------------------------------------------------
CbmMuch::~CbmMuch() {
  
  if ( flGeoPar ) delete flGeoPar;
  if (fMuchCollection) {
    fMuchCollection->Delete();
    delete fMuchCollection;
  }
}


// -----   Public method ProcessHits  --------------------------------------


Bool_t CbmMuch::ProcessHits(FairVolume* vol) {
  // cout<<" called process Hit******************     "<<endl;
  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
  }
  
  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Set additional parameters at exit of active volume. Create CbmMuchPoint.
  if ( gMC->IsTrackExiting()    ||
       gMC->IsTrackStop()       ||
       gMC->IsTrackDisappeared()   ) {
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fVolumeID = vol->getMCid();
    Int_t fDetectorId = GetDetId(vol);
    //  cout<<" check det ID 2 "<<fDetectorId<<endl;
    if (fVerboseLevel>2){
      printf(" TrackId: %i",fTrackID);
      printf(" System: %i" , CbmMuchAddress::GetSystemIndex(fDetectorId));
      printf(" Station: %i", CbmMuchAddress::GetStationIndex(fDetectorId));
      printf(" Layer: %i"  , CbmMuchAddress::GetLayerIndex(fDetectorId));
      printf(" Side: %i"   , CbmMuchAddress::GetLayerSideIndex(fDetectorId));
      printf(" Module: %i" , CbmMuchAddress::GetModuleIndex(fDetectorId));
      printf(" Vol %i \n",fVolumeID);
    }
    Int_t iStation = CbmMuchAddress::GetStationIndex(fDetectorId);
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    
    assert(iStation >=0 && iStation < fPar->GetNStations());
    CbmMuchStation* station= (CbmMuchStation*) fPar->GetStations()->At(iStation);
    //cout<<" track # "<<fTrackID<<"   Rmin "<<station->GetRmin()<<"   Rmax  "<<station->GetRmax()<<" in perp "<<fPosIn.Perp()<<" out perp "<<fPosOut.Perp()<<"  eloss "<<fELoss<<endl;
    if (fPosIn.Perp() >station->GetRmax()) {return kFALSE; }
    if (fPosOut.Perp()>station->GetRmax()) {return kFALSE; }
    if (fPosIn.Perp() <station->GetRmin()) {return kFALSE; }
    if (fPosOut.Perp()<station->GetRmin()) {return kFALSE; }
  



    if (fELoss == 0. ) return kFALSE;
    AddHit(fTrackID, fDetectorId,
	   TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
	   TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
	   TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
	   TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
	   fTime, fLength, fELoss);
    
    //if (fPosOut.Z()>250) printf("%f\n",fPosOut.Z());
    
    // Increment number of MuchPoints for this track
    CbmStack* stack = (CbmStack*) gMC->GetStack();
    stack->AddPoint(ECbmModuleId::kMuch);
    
    ResetParameters();
  }
  return kTRUE;
}


//-------------------------------------------------------------------------

Int_t CbmMuch::GetDetId(FairVolume* vol) {
  TString name = vol->GetName();
  Char_t cStationNr[3]= {name[11],name[12],' '};
  Int_t  iStation     = atoi(cStationNr)-1;
  Int_t  iLayer       = TString(name[18]).Atoi()-1;
  Int_t  iSide        = (name[19]=='b') ? 1 : 0;
  Char_t cModuleNr[4] = {name[26],name[27],name[28],' '};
  Int_t  iModule      = atoi(cModuleNr)-1;
  if(iSide!=1 && iSide !=0) printf("side = %i", iSide);
  Int_t detectorId = CbmMuchAddress::GetAddress(iStation,iLayer,iSide,iModule);
  assert(CbmMuchAddress::GetStationIndex(detectorId) == iStation);
  assert(CbmMuchAddress::GetLayerIndex(detectorId) == iLayer);
  assert(CbmMuchAddress::GetLayerSideIndex(detectorId) == iSide);
  assert(CbmMuchAddress::GetModuleIndex(detectorId) == iModule);
  assert(detectorId > 0);
  
  return detectorId;

}

// -------------------------------------------------------------------------
void CbmMuch::BeginEvent() {
}



// -------------------------------------------------------------------------
void CbmMuch::EndOfEvent() {
  if (fVerboseLevel) Print("");
  fMuchCollection->Delete();
  ResetParameters();
}

// -------------------------------------------------------------------------
void CbmMuch::Register() {
  FairRootManager::Instance()->Register("MuchPoint", GetName(), fMuchCollection, kTRUE);
}

// -------------------------------------------------------------------------
TClonesArray* CbmMuch::GetCollection(Int_t iColl) const {
  if (iColl == 0) return fMuchCollection;
  else return NULL;
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
void CbmMuch::Print(Option_t*) const {
  Int_t nHits = fMuchCollection->GetEntriesFast();
  LOG(info) << fName << ": " << nHits << " points registered in this event.";
}

// -------------------------------------------------------------------------
void CbmMuch::Reset() {
  fMuchCollection->Delete();
  ResetParameters();
}

// -------------------------------------------------------------------------
void CbmMuch::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(info) << fName << ": " << nEntries << " entries to add.";
  TClonesArray& clref = *cl2;
  CbmMuchPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (CbmMuchPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) CbmMuchPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(info) << fName << ": " << cl2->GetEntriesFast() << " merged entries.";
}
// -------------------------------------------------------------------------



// -----   Private method AddHit   --------------------------------------------
CbmMuchPoint* CbmMuch::AddHit(Int_t trackID, Int_t detID, TVector3 posIn,
			      TVector3 posOut, TVector3 momIn,
			      TVector3 momOut, Double_t time,
			      Double_t length, Double_t eLoss) {
  
  TClonesArray& clref = *fMuchCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1)
    LOG(info) << fName << ": Adding Point at (" << posIn.X() << ", " << posIn.Y()
	      << ", " << posIn.Z() << ") cm,  detector " << detID << ", track "
	      << trackID << ", energy loss " << eLoss*1e06 << " keV";
  
  return new(clref[size]) CbmMuchPoint(trackID, detID, posIn, posOut,
				       momIn, momOut, time, length, eLoss);
}

// -----  ConstructGeometry  -----------------------------------------------
void CbmMuch::ConstructGeometry() {
  
  TString fileName = GetGeometryFileName();

  // --- Only ROOT geometries are supported
  if (  ! fileName.EndsWith(".root") ) {
    LOG(fatal) <<  GetName() << ": Geometry format of file "
	       << fileName.Data() << " not supported.";
  }
  
  if (fileName.Contains("mcbm")) {

    fFlagID=1;
    LOG(info) << "mcbm geometry found ";
  }
  ConstructRootGeometry();
}
// -------------------------------------------------------------------------


void CbmMuch::ConstructRootGeometry(TGeoMatrix*)
{   
  FairRun*        fRun = FairRun::Instance();
  if (!fRun) {Fatal("CreateGeometry","No FairRun found"); return;}
  FairRuntimeDb*  rtdb = FairRuntimeDb::instance();
  fPar  = (CbmGeoMuchPar*)(rtdb->getContainer("CbmGeoMuchPar"));
  TObjArray* stations = fPar->GetStations();

  //-------------------------------------------------------
  
  CbmMuchGeoScheme* fGeoScheme = CbmMuchGeoScheme::Instance();
  
  fGeoScheme->Init(stations,fFlagID);

  TGeoMatrix* tempMatrix{nullptr};
  if (Cbm::GeometryUtils::IsNewGeometryFile(fgeoName, fVolumeName, &tempMatrix)) {
    LOG(info) << "Importing MUCH geometry from ROOT file "
              << fgeoName.Data();
    Cbm::GeometryUtils::ImportRootGeometry(fgeoName, this);
  } else {
    LOG(info) << "Constructing MUCH geometry from ROOT file "
              << fgeoName.Data();
    FairModule::ConstructRootGeometry();
  } 
  

  TObjArray* fSensNodes = fPar->GetGeoSensitiveNodes();
  TObjArray* fPassNodes = fPar->GetGeoPassiveNodes();
  TGeoNode* ncave = gGeoManager->GetTopNode();
  
  fGeoScheme->ExtractGeoParameter(ncave,fVolumeName.Data());

  TString objName = fVolumeName+"_0";
  TGeoNode* nmuch =(TGeoNode*) ncave->GetNodes()->FindObject(objName);
  fPassNodes->Add(nmuch);
  
  
  TObjArray* nodes = nmuch->GetNodes();
  
  for (Int_t i=0;i<nodes->GetEntriesFast();i++){
    TGeoNode* node = (TGeoNode*) nodes->At(i);
    TString nodeName = node->GetName();
    
    
    TObjArray* nodes1= node->GetNodes();
    
    for(Int_t j=0; j<nodes1->GetEntriesFast();j++){ 
      TGeoNode* node1 = (TGeoNode*) nodes1->At(j);
      TString node1Name = node1->GetName();
      
      if (node1Name.Contains("absblock")) fPassNodes->Add(node);
      if (node1Name.Contains("muchstation")) { 
	
	TObjArray* layers = node1->GetNodes();
	for (Int_t l=0;l<layers->GetEntriesFast();l++){
	  TGeoNode* layer = (TGeoNode*) layers->At(l);
 
	  if (!TString(layer->GetName()).Contains("layer")) continue;
	  TObjArray* layerNodes = layer->GetNodes();
	  for (Int_t m=0;m<layerNodes->GetEntriesFast();m++){
	    TGeoNode* layerNode = (TGeoNode*)layerNodes->At(m);
	    TString layerNodeName = layerNode->GetName();

	    if (layerNodeName.Contains("active"))  fSensNodes->Add(layerNode);
	    
	    if (layerNodeName.Contains("support")) fPassNodes->Add(layerNode);
            
            if (layerNodeName.Contains("cool")) fPassNodes->Add(layerNode);
	    
	  }
	}
      }
    }
  }
  
  fPar->setChanged();
  fPar->setInputVersion(fRun->GetRunId(),1);
}
    

// -----   CheckIfSensitive   -------------------------------------------------
Bool_t CbmMuch::CheckIfSensitive(
				 std::string name)
{
  TString tsname = name;
  
  
  if (tsname.Contains("active")){
      LOG(debug1) << "CbmMuch::CheckIfSensitive: Register active volume: " << tsname;
      return kTRUE;
  }
  return kFALSE;
}
