/** @file CbmEvDisTracks.cxx
  * @copyright Copyright (C) 2007-2021 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors M. Al-Turany, Norbert Herrmann [committer], Florian Uhlig **/

#// -------------------------------------------------------------------------
// -----                  following FairMCTracks source file           -----
// -----                  Created 10/12/07  by M. Al-Turany            -----
// -------------------------------------------------------------------------
#define TOFDisplay 1  // =1 means active, other: without Label and not relying on TEvePointSet
#define TOFTtErr 1    // =1 means active, other: not relying on VelocityError of CbmTofTracklet

#include "CbmEvDisTracks.h"

#include "CbmTofHit.h"       // for CbmTofHit
#include "CbmTofTracklet.h"  // for CbmTofTracklet

#include <FairEventManager.h>  // for FairEventManager
#include <FairRootManager.h>   // for FairRootManager
#include <FairTask.h>          // for FairTask, InitStatus, kERROR, kSUCCESS
#include <Logger.h>            // for LOG, Logger

#include <Rtypes.h>               // for ClassImp
#include <TClonesArray.h>         // for TClonesArray
#include <TEveElement.h>          // for TEveElementList
#include <TEveManager.h>          // for TEveManager, gEve
#include <TEvePathMark.h>         // for TEvePathMark
#include <TEvePointSet.h>         // for TEvePointSetArray, TEvePointSet
#include <TEveTrack.h>            // for TEveTrack, TEveTrackList
#include <TEveTrackPropagator.h>  // for TEveTrackPropagator
#include <TEveVSDStructs.h>       // for TEveRecTrack
#include <TEveVector.h>           // for TEveVector, TEveVectorT
#include <TGLAnnotation.h>
#include <TGLViewer.h>
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TObjArray.h>          // for TObjArray
#include <TParticle.h>          // for TParticle
#include <TString.h>            // for Form, TString

#include <string.h>  // for strcmp

ClassImp(CbmEvDisTracks);
CbmEvDisTracks* CbmEvDisTracks::fInstance = 0;

static TGLAnnotation* anne;
static TGLAnnotation* annt;

// -----   Default constructor   -------------------------------------------
CbmEvDisTracks::CbmEvDisTracks()
  : FairTask("CbmEvDisTracks", 0)
  , fTrackList(nullptr)
  , fTrPr(nullptr)
  , fEventManager(nullptr)
  , fEveTrList(nullptr)
  , fEvent("")
  , fTrList(nullptr)
  , fEvePSList(nullptr)
  , fPSList(nullptr)
  , fRenderP(kFALSE)
  , fRenderT(kTRUE)
  , MinEnergyLimit(-1.)
  , MaxEnergyLimit(-1.)
  , PEnergy(-1.)
{
  if (!fInstance) fInstance = this;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmEvDisTracks::CbmEvDisTracks(const char* name, Int_t iVerbose, Bool_t renderP, Bool_t renderT)
  : FairTask(name, iVerbose)
  , fTrackList(nullptr)
  , fTrPr(nullptr)
  , fEventManager(nullptr)
  , fEveTrList(new TObjArray(16))
  , fEvent("")
  , fTrList(nullptr)
  , fEvePSList(new TObjArray(8))
  , fPSList(nullptr)
  , fRenderP(renderP)
  , fRenderT(renderT)
  , MinEnergyLimit(-1.)
  , MaxEnergyLimit(-1.)
  , PEnergy(-1.)
{
  if (!fInstance) fInstance = this;
}
// -------------------------------------------------------------------------
InitStatus CbmEvDisTracks::Init()
{
  LOG(debug) << "CbmEvDisTracks::Init()";
  FairRootManager* fManager = FairRootManager::Instance();
  fTrackList                = (TClonesArray*) fManager->GetObject("TofTracks");
  if (fTrackList == 0) {
    LOG(warn) << "CbmEvDisTracks::Init() branch " << GetName() << " Not found! Task will be deactivated ";
    SetActive(kFALSE);
  }
  LOG(debug1) << "CbmEvDisTracks::Init() get track list" << fTrackList;
  LOG(debug1) << "CbmEvDisTracks::Init() create propagator";
  fEventManager = FairEventManager::Instance();
  LOG(debug1) << "CbmEvDisTracks::Init() get instance of FairEventManager ";
  fEvent         = "Current Event";
  MinEnergyLimit = fEventManager->GetEvtMinEnergy();
  MaxEnergyLimit = fEventManager->GetEvtMaxEnergy();
  PEnergy        = 0;
  if (IsActive()) { return kSUCCESS; }
  else {
    return kERROR;
  }
}
// -------------------------------------------------------------------------
void CbmEvDisTracks::Exec(Option_t* option)
{

  if (IsActive()) {

    LOG(debug2) << " CbmEvDisTracks::Exec starting with verbosity " << fVerbose << " and option " << option;
    CbmTofTracklet* tr;
    const Double_t* point;
    CbmTofHit* hit;

    Reset();

    LOG(debug4) << " CbmEvDisTracks:: NTrks " << fTrackList->GetEntriesFast();
    Int_t TMul[10] = {10 * 0};  //FIXME - don't use constants in code

    for (Int_t iOpt = 0; iOpt < 2; iOpt++)
      for (Int_t i = 0; i < fTrackList->GetEntriesFast(); i++) {
        LOG(debug4) << "CbmEvDisTracks::Exec " << i;
        tr = (CbmTofTracklet*) fTrackList->At(i);
        if (nullptr == tr) continue;
        Int_t Np = tr->GetNofHits();

#if TOFDisplay == 1  //List for TEvePointSets
        if (iOpt == 0) TMul[Np]++;
        fPSList = GetPSGroup(Np, iOpt);
#endif

        fTrList          = GetTrGroup(tr->GetNofHits(), iOpt);
        TParticle* P     = new TParticle();
        TEveTrack* track = new TEveTrack(P, tr->GetPidHypo(), fTrPr);
        Int_t iCol       = Np;
        if (iCol > 4) iCol++;
        track->SetAttLineAttMarker(fTrList);  //set defaults
        track->SetLineColor(iCol);
        track->SetMarkerColor(iCol);
        track->SetMarkerSize(2.);
        //track->SetMarkerDraw(kTRUE);

        track->SetPoint(0, tr->GetFitX(0.), tr->GetFitY(0.), 0.);  //insert starting point
        TEveVector pos0     = TEveVector(tr->GetFitX(0.), tr->GetFitY(0.), 0.);
        TEvePathMark* path0 = new TEvePathMark();
        path0->fV           = pos0;
        track->AddPathMark(*path0);

        Double_t pbuf[3], vbuf[3];
        TEveRecTrack rt;
        rt.fIndex = i;
        pbuf[0]   = 0.;
        pbuf[1]   = 0.;
        pbuf[2]   = 1. / tr->GetTt();  // velocity
        rt.fP.Set(pbuf);
        vbuf[0] = tr->GetFitX(0.);
        vbuf[1] = tr->GetFitY(0.);
        vbuf[2] = 0.;
        rt.fV.Set(vbuf);
        track->SetName(Form("TEveTrack %d", rt.fIndex));

        //track->SetStdTitle();
        //      Double_t beta, beta_err, res_x, res_y, res_z, res_t;
        Double_t beta, beta_err, res_x, res_y, res_t;
        switch (iOpt) {
          case 0: track->SetStdTitle(); break;
          case 1:
#if TOFDisplay == 1  //setting content of label depending on available information
            beta = (1 / tr->GetTt()) / 29.98;
#if TOFTtErr == 1
            beta_err = beta * (tr->GetTtErr() / tr->GetTt());
            track->SetTitle(
              Form("%s\nChiSqDoF = %2.2f\nbeta = %1.3f +/- %1.3f", track->GetName(), tr->GetChiSq(), beta, beta_err));
#else
            track->SetTitle(Form("%s\nChiSqDoF = %2.2f\nbeta = %1.3f", track->GetName(), tr->GetChiSq(), beta));
#endif
#else
            track->SetStdTitle();
#endif
            break;
        }

#if TOFDisplay == 1
        // initialize TEvePointSet to show Datapoints belonging to track
        TEvePointSetArray* psa = new TEvePointSetArray(Form("TEveTrack Points %d", i), "");
        psa->SetMarkerColor(iCol);
        psa->SetMarkerSize(2.0);
        psa->SetMarkerStyle(4);
        psa->InitBins("Hits", Np, 0.5, Np + 0.5);
#endif

        for (Int_t n = 0; n < Np; n++) {
          switch (iOpt) {
            case 0: point = tr->GetPoint(n);  //pointer to member variable so GetFitPoint() would also change GetPoint()
#if TOFDisplay == 1
              // following belongs to filling and labeling of PointSetArray
              psa->Fill(point[0], point[1], point[2], n + 1);
              hit   = tr->GetTofHitPointer(n);
              res_x = (point[0] - tr->GetFitX(point[2])) / hit->GetDx();
              res_y = (point[1] - tr->GetFitY(point[2])) / hit->GetDy();
              res_t = (point[3] - tr->GetFitT(point[2])) / hit->GetTimeError();
              //          res_z=0;
              psa->GetBin(n + 1)->SetTitle(Form("%s\nPointId = %d\nResiduals:\nX = %1.3f\nY = %1.3f\nT = %1.3f",
                                                track->GetName(), tr->GetHitIndex(n), res_x, res_y, res_t));
#endif
              break;
            case 1: point = tr->GetFitPoint(n); break;
          }
          track->SetPoint(n + 1, point[0], point[1], point[2]);
          LOG(debug4) << Form("   CbmEvDisTracks::Exec SetPoint %d, %6.2f, %6.2f, %6.2f, %6.2f ", n, point[0], point[1],
                              point[2], point[3]);

          TEveVector pos     = TEveVector(point[0], point[1], point[2]);
          TEvePathMark* path = new TEvePathMark();
          path->fV           = pos;
          path->fTime        = point[3];
          if (n == 0) {
            TEveVector Mom = TEveVector(P->Px(), P->Py(), P->Pz());
            path->fP       = Mom;
          }
          LOG(debug4) << "Path marker added " << path;

          track->AddPathMark(*path);

          LOG(debug4) << "Path marker added " << path;
        }
#if TOFDisplay == 1
        if (iOpt == 0) { fPSList->AddElement(psa); }
#endif
        track->SortPathMarksByTime();
        fTrList->AddElement(track);
        LOG(debug3) << i << ". track added: " << track->GetName();
      }
    for (Int_t i = 0; i < fEveTrList->GetEntriesFast(); i++) {
      // TEveTrackList *TrListIn=( TEveTrackList *) fEveTrList->At(i);
      //TrListIn->FindMomentumLimits(TrListIn, kFALSE);
    }
    fEventManager->SetEvtMaxEnergy(MaxEnergyLimit);
    fEventManager->SetEvtMinEnergy(MinEnergyLimit);

    TString cEventInfo = Form("ev# %d ", fEventManager->GetCurrentEvent());
    TString cTrackInfo = "trkl mul: ";  // to be completed while building the display
    for (Int_t i = 9; i > 0; i--)
      if (TMul[i] > 0) cTrackInfo += Form("M%d %d/", i, TMul[i]);

    TGLViewer* v = gEve->GetDefaultGLViewer();

    if (NULL != anne) anne->SetText(cEventInfo);
    else
      anne = new TGLAnnotation(v, cEventInfo, 0.01, 0.95);
    if (NULL != annt) annt->SetText(cTrackInfo);
    else
      annt = new TGLAnnotation(v, cTrackInfo, 0.01, 0.92);
    anne->SetTextSize(0.03);  // % of window diagonal
    annt->SetTextSize(0.03);  // % of window diagonal
    anne->SetTextColor(4);
    annt->SetTextColor(4);

    gEve->Redraw3D(kFALSE);
    //gEve->DoRedraw3D();
    //gEve->Redraw3D(kTRUE);
  }
}
// -----   Destructor   ----------------------------------------------------
CbmEvDisTracks::~CbmEvDisTracks() {}
// -------------------------------------------------------------------------
void CbmEvDisTracks::SetParContainers() {}

// -------------------------------------------------------------------------
void CbmEvDisTracks::Finish() {}
// -------------------------------------------------------------------------
void CbmEvDisTracks::Reset()
{
  for (Int_t i = 0; i < fEveTrList->GetEntriesFast(); i++) {
    TEveTrackList* ele = (TEveTrackList*) fEveTrList->At(i);
    gEve->RemoveElement(ele, fEventManager);
  }
  fEveTrList->Clear();
#if TOFDisplay == 1
  for (Int_t i = 0; i < fEvePSList->GetEntriesFast(); i++) {
    TEveElementList* ele = (TEveElementList*) fEvePSList->At(i);
    gEve->RemoveElement(ele, fEventManager);
  }
  fEvePSList->Clear();
#endif
}

Char_t* gr;
TEveTrackList* CbmEvDisTracks::GetTrGroup(Int_t ihmul, Int_t iOpt)
{
  switch (iOpt) {
    case 0: gr = Form("Trkl_hmul%d", ihmul); break;
    case 1: gr = Form("FTrkl_hmul%d", ihmul); break;
  }
  fTrList = 0;
  for (Int_t i = 0; i < fEveTrList->GetEntriesFast(); i++) {
    TEveTrackList* TrListIn = (TEveTrackList*) fEveTrList->At(i);
    if (strcmp(TrListIn->GetName(), gr) == 0) {
      fTrList = TrListIn;
      break;
    }
  }
  if (fTrList == 0) {
    fTrPr      = new TEveTrackPropagator();
    fTrList    = new TEveTrackList(gr, fTrPr);
    Int_t iCol = ihmul;
    if (iCol > 4) iCol++;
    fTrList->SetMainColor(iCol);
    fEveTrList->Add(fTrList);
#if TOFDisplay == 1
    if (iOpt == 1) {  // delete if-condition to return to old code
      gEve->AddElement(fTrList, fEventManager);
    }
#else
    gEve->AddElement(fTrList, fEventManager);
#endif
    fTrList->SetRecurse(kTRUE);
    switch (iOpt) {
      case 0:  //  display points
        fTrList->SetRnrPoints(kTRUE);
        fTrList->SetRnrLine(kFALSE);
        fTrList->SetMarkerSize(2.);
        fTrList->SetRnrChildren(fRenderP);  // default not shown
        break;
      case 1:  //display fit line
        fTrList->SetRnrLine(kTRUE);
        fTrList->SetLineWidth(2.);
        fTrList->SetRnrChildren(fRenderT);  // default not shown
        break;
      default:;
    }
  }
  return fTrList;
}
#if TOFDisplay == 1
TEveElementList* CbmEvDisTracks::GetPSGroup(Int_t ihmul, Int_t /*iOpt*/)
{
  gr      = Form("PTrkl_hmul%d", ihmul);
  fPSList = 0;
  for (Int_t i = 0; i < fEvePSList->GetEntriesFast(); i++) {
    TEveElementList* l = (TEveElementList*) fEvePSList->At(i);
    if (strcmp(l->GetName(), gr) == 0) {
      fPSList = l;
      break;
    }
  }
  if (fPSList == 0) {
    fPSList    = new TEveElementList(gr);
    Int_t iCol = ihmul;
    if (iCol > 4) iCol++;
    fPSList->SetMainColor(iCol);
    fEvePSList->Add(fPSList);
    gEve->AddElement(fPSList, fEventManager);
    fPSList->SetRnrChildren(fRenderP);
  }
  return fPSList;
}
#endif

ClassImp(CbmEvDisTracks)
