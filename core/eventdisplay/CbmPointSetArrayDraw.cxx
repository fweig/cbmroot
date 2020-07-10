// -------------------------------------------------------------------------
// -----               CbmPointSetArrayDraw source file                -----
// -----                Created 18/06/22  by J. Brandt                 -----
// -----               Following class FairPointSetDraw                -----
// -------------------------------------------------------------------------
#include "CbmPointSetArrayDraw.h"

#include "CbmPixelHit.h"       // for CbmPixelHit
#include "CbmPointSetArray.h"  // for CbmPointSetArray
#include "CbmTofHit.h"         // for CbmTofHit

#include <FairEventManager.h>  // for FairEventManager
#include <FairLogger.h>        // for LOG, Logger
#include <FairRootManager.h>   // for FairRootManager
#include <FairTask.h>          // for FairTask, InitStatus, kSUCCESS

#include <Rtypes.h>             // for kRed, ClassImp
#include <TClonesArray.h>       // for TClonesArray
#include <TEveManager.h>        // for TEveManager, gEve
#include <TEveTreeTools.h>      // for TEvePointSelectorConsumer, TEvePointS...
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TVector3.h>           // for TVector3

#include <iomanip>  // for operator<<, setprecision

// -----   Default constructor   -------------------------------------------
CbmPointSetArrayDraw::CbmPointSetArrayDraw()
  : FairTask("CbmPointSetArrayDraw", 0)
  , fVerbose(0)
  , fPointList(nullptr)
  , fEventManager(nullptr)
  , fl(nullptr)
  , fColor(0)
  , fStyle(0)
  , fTimeOffset(0)
  , fTimeMax(0)
  , fColorMode(1)
  , fMarkerMode(1)
  , fRender(kTRUE) {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmPointSetArrayDraw::CbmPointSetArrayDraw(const char* name,
                                           Int_t colorMode,
                                           Int_t markerMode,
                                           Int_t iVerbose,
                                           Bool_t render)
  : FairTask(name, iVerbose)
  , fVerbose(iVerbose)
  , fPointList(nullptr)
  , fEventManager(nullptr)
  , fl(nullptr)
  , fColor(kRed)
  , fStyle(4)
  , fTimeOffset(0)
  , fTimeMax(0)
  , fColorMode(colorMode)
  , fMarkerMode(markerMode)
  , fRender(render) {}
// -------------------------------------------------------------------------
InitStatus CbmPointSetArrayDraw::Init() {
  LOG(debug) << "CbmPointSetArrayDraw::Init()";
  FairRootManager* fManager = FairRootManager::Instance();
  fPointList = static_cast<TClonesArray*>(fManager->GetObject(GetName()));
  if (fPointList == 0) {
    LOG(warn) << "CbmPointSetArrayDraw::Init()  branch " << GetName()
              << " Not found! Task will be deactivated ";
    SetActive(kFALSE);
  }
  LOG(debug1) << "CbmPointSetArrayDraw::Init() get track list" << fPointList;
  fEventManager = FairEventManager::Instance();
  LOG(debug1)
    << "CbmPointSetArrayDraw::Init() get instance of FairEventManager ";
  fl = 0;

  return kSUCCESS;
}
// -------------------------------------------------------------------------
void CbmPointSetArrayDraw::Exec(Option_t* /*option*/) {
  if (IsActive()) {
    Int_t npoints = fPointList->GetEntriesFast();
    Reset();

    // initialize CbmPointSetArray to display set of hits
    CbmPointSetArray* l = new CbmPointSetArray("TofHitTime", "");
    l->SetColorMode(fColorMode);
    l->SetMarkerMode(fMarkerMode);
    l->SetSourceCS(TEvePointSelectorConsumer::kTVT_XYZ);
    l->SetMarkerColor(kRed);
    l->SetMarkerStyle(4);
    l->SetMarkerSize(2.0);

    DetermineTimeOffset();
    l->InitBins("Hits", npoints, 0.5, npoints + 0.5);
    l->InitValues(npoints);

    for (Int_t i = 1; i <= npoints; i++) {  //loop over all hits in event
      TObject* p = static_cast<TObject*>(fPointList->At(i - 1));
      if (p != 0) {
        TVector3 vec(GetVector(p));
        l->Fill(vec.X(), vec.Y(), vec.Z(), i);  // fill 3D position
        l->FillValues(
          GetPointId(p),
          GetTime(p),
          GetTot(p),
          GetClusterSize(p),
          i);  // fill physical information used for color and markersize
      }
    }

    l->ApplyColorMode();  // apply colorMode and calculate color of each bin
    l->ApplyMarkerMode();  // apply markerMode and calculate markersize of each bin
    l->ApplyTitles();      // set BBox-title of each bin and computeBBox

    l->SetRnrChildren(fRender);
    gEve->AddElement(l);
    gEve->Redraw3D(kFALSE);
    fl = l;
  }
}
// --------------------------------------------------------------------------------
// returns 3D-vector with position data of hit
TVector3 CbmPointSetArrayDraw::GetVector(TObject* obj) {
  CbmPixelHit* p = (CbmPixelHit*) obj;
  LOG(debug2) << "-I- CbmPointSetArrayDraw::GetVector(): " << p->GetX() << " "
              << p->GetY() << " " << p->GetZ() << " ";
  return TVector3(p->GetX(), p->GetY(), p->GetZ());
}
// --------------------------------------------------------------------------------
// returns hit-time against first hit
Double_t CbmPointSetArrayDraw::GetTime(TObject* obj) {
  CbmPixelHit* p = (CbmPixelHit*) obj;
  LOG(debug2) << "-I- CbmPointSetArrayDraw::GetTime(): "
              << p->GetTime() - fTimeOffset;
  return p->GetTime() - fTimeOffset;
}
// --------------------------------------------------------------------------------
// returns ClusterSize of Hit
Int_t CbmPointSetArrayDraw::GetClusterSize(TObject* obj) {
  //CluSize of TofHit is stored in Flag-Variable (set in Clusterizer)
  CbmTofHit* p     = (CbmTofHit*) obj;
  Double_t cluSize = p->GetFlag();
  //Flag= #digis = 2*cluSize  +100 if used for track
  if (cluSize > 100) {
    cluSize = (cluSize - 100) / 2;
  } else {
    cluSize /= 2;
  }
  LOG(debug3) << "-I- CbmPointSetArrayDraw::GetClusterSize(): " << cluSize;
  return cluSize;
}
// --------------------------------------------------------------------------------
// returns ToT of hit
Double_t CbmPointSetArrayDraw::GetTot(TObject* obj) {
  // ToT of TofHit is stored in Channel-Variable (set in Clusterizer)
  CbmTofHit* p = (CbmTofHit*) obj;
  Double_t tot = Double_t(p->GetCh()) / (20 * GetClusterSize(p));
  LOG(debug3) << "-I- CbmPointSetArrayDraw::GetTot(): " << tot;
  return tot;
}
// --------------------------------------------------------------------------------
// returns Id of hit
Int_t CbmPointSetArrayDraw::GetPointId(TObject* obj) {
  CbmPixelHit* p = (CbmPixelHit*) obj;
  return p->GetRefId();
}
// ---------------------------------------------------------------------------------
// Determine time of first hit in event to use as offset
void CbmPointSetArrayDraw::DetermineTimeOffset() {
  Int_t npoints = fPointList->GetEntriesFast();
  fTimeOffset   = 115200000000000;  //32hours in ns as maximum of clock
  fTimeMax      = 0;
  Double_t currtime;
  CbmPixelHit* hit;
  for (Int_t i = 0; i < npoints; i++) {  //loop over all hits in event
    hit      = static_cast<CbmPixelHit*>(fPointList->At(i));
    currtime = hit->GetTime();
    if (currtime < fTimeOffset) {
      fTimeOffset = currtime;
    } else if (currtime > fTimeMax) {
      fTimeMax = currtime;
    }
  }
  fTimeMax -= fTimeOffset;  //time of latest hit in event
  LOG(debug3) << std::setprecision(15)
              << "-I- CbmPointSetArrayDraw::DetermineTimeBins: fTimeOffset "
              << fTimeOffset;
}

// -----   Destructor   ----------------------------------------------------
CbmPointSetArrayDraw::~CbmPointSetArrayDraw() {}
// -------------------------------------------------------------------------
void CbmPointSetArrayDraw::SetParContainers() {}
// -------------------------------------------------------------------------
/** Action after each event**/
void CbmPointSetArrayDraw::Finish() {}
// -------------------------------------------------------------------------
void CbmPointSetArrayDraw::Reset() {
  if (fl != 0) {
    fl->RemoveElementsLocal();
    gEve->RemoveElement(fl, fEventManager);
  }
}


ClassImp(CbmPointSetArrayDraw);
