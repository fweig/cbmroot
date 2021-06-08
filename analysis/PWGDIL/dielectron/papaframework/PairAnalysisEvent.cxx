/* Copyright (C) 2015-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Julian Book [committer], Florian Uhlig */
/*
  Event that holds all information needed for the of analysis (including fast simulations). 
  Init() the PairAnalysisTrack array fTracks and provides easy access to:

    hits          GetHits(      ECbmModuleId det)
    hit matches   GetHitMatches(ECbmModuleId det)
    points        GetPoints(    ECbmModuleId det)

    clusters      GetCluster(   ECbmModuleId det)

  for each sub-detector.

*/

#include "PairAnalysisEvent.h"

#include "CbmGlobalTrack.h"
#include "CbmKFVertex.h"
#include "CbmMCTrack.h"
#include "CbmMuchTrack.h"
#include "CbmRichRing.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrackMatchNew.h"
#include "CbmTrdTrack.h"
#include "CbmVertex.h"

#include "FairMCEventHeader.h"
#include "FairMCPoint.h"
#include "FairRootManager.h"
#include "FairTrackParam.h"

#include <Rtypes.h>
#include <TArrayS.h>
#include <TMatrixFSym.h>
#include <TObjArray.h>
#include <TParticle.h>

#include "PairAnalysisTrack.h"

ClassImp(PairAnalysisEvent)

  PairAnalysisEvent::PairAnalysisEvent()
  : TNamed()
  , fTracks(new TObjArray(1))  // array of papa tracks
{
  //
  // Default Constructor
  //
  fTracks->SetOwner(kTRUE);
}

//______________________________________________
PairAnalysisEvent::PairAnalysisEvent(const char* name, const char* title)
  : TNamed(name, title)
  , fTracks(new TObjArray(1))  // array of papa tracks
{
  //
  // Named Constructor
  //
  fTracks->SetOwner(kTRUE);
}

//______________________________________________
PairAnalysisEvent::~PairAnalysisEvent()
{
  //
  // Default Destructor
  //
  delete fPrimVertex;

  //  fTracks->Clear("C");
  fGlobalTracks->Delete();  //global tracks
  fMCHeader->Delete();      //mc tracks
  fMCTracks->Delete();      //mc tracks

  fTrdTracks->Delete();   //TRD tracks
  fStsTracks->Delete();   //STS tracks
  fMuchTracks->Delete();  //MUCH tracks
  fRichRings->Delete();   //RICH rings

  fStsMatches->Delete();   //STS matches
  fMuchMatches->Delete();  //MUCH matches
  fTrdMatches->Delete();   //TRD matches
  fRichMatches->Delete();  //RICH matches

  fMvdPoints->Delete();   //MVD hits
  fStsPoints->Delete();   //STS hits
  fMuchPoints->Delete();  //MUCH hits
  fRichPoints->Delete();  //RICH hits
  fTrdPoints->Delete();   //TRD hits
  fTofPoints->Delete();   //TOF matches

  fMvdHits->Delete();        //MVD hits
  fStsHits->Delete();        //STS hits
  fMuchHits->Delete();       //MUCH hits
  fMuchHitsStraw->Delete();  //MUCH hits
  fTrdHits->Delete();        //TRD hits
  fRichHits->Delete();       //RICH hits
  fTofHits->Delete();        //TOF hits

  fTrdCluster->Delete();  //TRD cluster

  fRichProjection->Delete();
  fMvdHitMatches->Delete();   //MVD hits
  fStsHitMatches->Delete();   //STS hits
  fMuchHitMatches->Delete();  //MUCH hits
  fRichHitMatches->Delete();  //RICH hits
  fTrdHitMatches->Delete();   //TRD hits
  fTofHitMatches->Delete();   //TOF hits

  fFastTracks->Delete();
}

//______________________________________________
void PairAnalysisEvent::SetInput(FairRootManager* man)
{
  //
  // setup the track/hit branches
  //
  fGlobalTracks = (TClonesArray*) man->GetObject("GlobalTrack");
  fTrdTracks    = (TClonesArray*) man->GetObject("TrdTrack");
  fStsTracks    = (TClonesArray*) man->GetObject("StsTrack");
  fMuchTracks   = (TClonesArray*) man->GetObject("MuchTrack");
  fRichRings    = (TClonesArray*) man->GetObject("RichRing");
  //  fPrimVertex   = (CbmVertex*)    man->GetObject("PrimaryVertex");    // Get pointer to PrimaryVertex object from IOManager if it exists
  // Get pointer to PrimaryVertex object from IOManager if it exists
  // The old name for the object is "PrimaryVertex" the new one
  // "PrimaryVertex." Check first for the new name
  fPrimVertex = dynamic_cast<CbmVertex*>(man->GetObject("PrimaryVertex."));
  if (nullptr == fPrimVertex) { fPrimVertex = dynamic_cast<CbmVertex*>(man->GetObject("PrimaryVertex")); }
  // MC matches and tracks
  fMCHeader    = (FairMCEventHeader*) man->GetObject("MCEventHeader.");
  fMCTracks    = (TClonesArray*) man->GetObject("MCTrack");
  fStsMatches  = (TClonesArray*) man->GetObject("StsTrackMatch");
  fMuchMatches = (TClonesArray*) man->GetObject("MuchTrackMatch");
  fTrdMatches  = (TClonesArray*) man->GetObject("TrdTrackMatch");
  fRichMatches = (TClonesArray*) man->GetObject("RichRingMatch");
  // hits
  fMvdHits       = (TClonesArray*) man->GetObject("MvdHit");
  fStsHits       = (TClonesArray*) man->GetObject("StsHit");
  fMuchHits      = (TClonesArray*) man->GetObject("MuchPixelHit");
  fMuchHitsStraw = (TClonesArray*) man->GetObject("MuchStrawHit");
  fTrdHits       = (TClonesArray*) man->GetObject("TrdHit");
  fRichHits      = (TClonesArray*) man->GetObject("RichHit");
  fTofHits       = (TClonesArray*) man->GetObject("TofHit");
  // hit matches (matches are accessed directly via CbmHit::GetMatch)
  fMvdHitMatches  = (TClonesArray*) man->GetObject("MvdHitMatch");  //needed for mvd matching
  fStsHitMatches  = (TClonesArray*) man->GetObject("StsHitMatch");
  fRichHitMatches = (TClonesArray*) man->GetObject("RichHitMatch");
  fTrdHitMatches  = (TClonesArray*) man->GetObject("TrdHitMatch");
  fTofHitMatches  = (TClonesArray*) man->GetObject("TofHitMatch");
  fMuchHitMatches = (TClonesArray*) man->GetObject("MuchPixelHitMatch");
  //  fMuchHitStrawMatches = (TClonesArray*) man->GetObject("MuchStrawHitMatch");
  // mc points
  fMvdPoints  = (TClonesArray*) man->GetObject("MvdPoint");
  fStsPoints  = (TClonesArray*) man->GetObject("StsPoint");
  fRichPoints = (TClonesArray*) man->GetObject("RichPoint");
  fMuchPoints = (TClonesArray*) man->GetObject("MuchPoint");
  fTrdPoints  = (TClonesArray*) man->GetObject("TrdPoint");
  fTofPoints  = (TClonesArray*) man->GetObject("TofPoint");
  // cluster
  fTrdCluster = (TClonesArray*) man->GetObject("TrdCluster");

  fRichProjection = (TClonesArray*) man->GetObject("RichProjection");
  // fast track
  fFastTracks = (TClonesArray*) man->GetObject("FastTrack");

  //  if(fMCTracks)   printf("PairAnalysisEvent::SetInput: size of mc array: %04d \n",fMCTracks->GetSize());
}

//______________________________________________
void PairAnalysisEvent::Init()
{
  //
  // initialization of track arrays
  //
  fTracks->Clear("C");
  if (!fGlobalTracks && !fFastTracks) return;

  // DEBUG stuff
  if (0) {
    fprintf(stderr, "check %s: has %d points in %p \n", "MVD", GetNumberOfPoints(ECbmModuleId::kMvd),
            GetPoints(ECbmModuleId::kMvd));
    fprintf(stderr, "check %s: has %d points in %p \n", "STS", GetNumberOfPoints(ECbmModuleId::kSts),
            GetPoints(ECbmModuleId::kSts));
    fprintf(stderr, "check %s: has %d points in %p \n", "RICH", GetNumberOfPoints(ECbmModuleId::kRich),
            GetPoints(ECbmModuleId::kRich));
    fprintf(stderr, "check %s: has %d points in %p \n", "TRD", GetNumberOfPoints(ECbmModuleId::kTrd),
            GetPoints(ECbmModuleId::kTrd));
    fprintf(stderr, "check %s: has %d points in %p \n", "TOF", GetNumberOfPoints(ECbmModuleId::kTof),
            GetPoints(ECbmModuleId::kTof));

    fprintf(stderr, "check %s: has %d hitMatches in %p \n", "MVD", GetNumberOfHitMatches(ECbmModuleId::kMvd),
            GetHitMatches(ECbmModuleId::kMvd));
    fprintf(stderr, "check %s: has %d hitMatches in %p \n", "STS", GetNumberOfHitMatches(ECbmModuleId::kSts),
            GetHitMatches(ECbmModuleId::kSts));
    fprintf(stderr, "check %s: has %d hitMatches in %p \n", "RICH", GetNumberOfHitMatches(ECbmModuleId::kRich),
            GetHitMatches(ECbmModuleId::kRich));
    fprintf(stderr, "check %s: has %d hitMatches in %p \n", "TRD", GetNumberOfHitMatches(ECbmModuleId::kTrd),
            GetHitMatches(ECbmModuleId::kTrd));
    fprintf(stderr, "check %s: has %d hitMatches in %p \n", "TOF", GetNumberOfHitMatches(ECbmModuleId::kTof),
            GetHitMatches(ECbmModuleId::kTof));

    fprintf(stderr, "check %s: has %d hits in %p \n", "MVD", GetNumberOfHits(ECbmModuleId::kMvd),
            GetHits(ECbmModuleId::kMvd));
    fprintf(stderr, "check %s: has %d hits in %p \n", "STS", GetNumberOfHits(ECbmModuleId::kSts),
            GetHits(ECbmModuleId::kSts));
    fprintf(stderr, "check %s: has %d hits in %p \n", "RICH", GetNumberOfHits(ECbmModuleId::kRich),
            GetHits(ECbmModuleId::kRich));
    fprintf(stderr, "check %s: has %d hits in %p \n", "TRD", GetNumberOfHits(ECbmModuleId::kTrd),
            GetHits(ECbmModuleId::kTrd));
    fprintf(stderr, "check %s: has %d hits in %p \n", "TOF", GetNumberOfHits(ECbmModuleId::kTof),
            GetHits(ECbmModuleId::kTof));
  }

  // get primary kf vertex or create one from mc header
  CbmKFVertex* vtx = 0x0;
  if (!fPrimVertex && fMCHeader) {
    TMatrixFSym cov(3);
    fPrimVertex = new CbmVertex("mcvtx", "mc vtx", fMCHeader->GetX(), fMCHeader->GetY(), fMCHeader->GetZ(), 1.0, 1,
                                fMCHeader->GetNPrim(), cov);
  }
  else if (!fPrimVertex && !fMCHeader) {
    TMatrixFSym cov(3);
    fPrimVertex = new CbmVertex("defaultvtx", "default vtx", 0., 0., 0., 1.0, 1,
                                TMath::Max(fGlobalTracks->GetEntriesFast(), fFastTracks->GetEntriesFast()), cov);
  }
  if (fPrimVertex) vtx = new CbmKFVertex(*fPrimVertex);

  TArrayS matches;
  if (fMCTracks) matches.Set(fMCTracks->GetEntriesFast());

  /// loop over all glbl tracks
  for (Int_t i = 0; i < (fGlobalTracks ? fGlobalTracks->GetEntriesFast() : 0); i++) {
    // global track
    CbmGlobalTrack* gtrk = static_cast<CbmGlobalTrack*>(fGlobalTracks->UncheckedAt(i));
    if (!gtrk) continue;

    Int_t itrd  = gtrk->GetTrdTrackIndex();
    Int_t ists  = gtrk->GetStsTrackIndex();
    Int_t irich = gtrk->GetRichRingIndex();
    Int_t itof  = gtrk->GetTofHitIndex();
    Int_t imuch = gtrk->GetMuchTrackIndex();

    // reconstructed tracks
    CbmTrdTrack* trdTrack = 0x0;
    if (fTrdTracks && itrd >= 0) trdTrack = static_cast<CbmTrdTrack*>(fTrdTracks->At(itrd));
    CbmStsTrack* stsTrack = 0x0;
    if (fStsTracks && ists >= 0) stsTrack = static_cast<CbmStsTrack*>(fStsTracks->At(ists));
    CbmRichRing* richRing = 0x0;
    if (fRichRings && irich >= 0) richRing = static_cast<CbmRichRing*>(fRichRings->At(irich));
    CbmTofHit* tofHit = 0x0;
    if (fTofHits && itof >= 0) tofHit = static_cast<CbmTofHit*>(fTofHits->At(itof));
    CbmMuchTrack* muchTrack = 0x0;
    if (fMuchTracks && imuch >= 0) muchTrack = static_cast<CbmMuchTrack*>(fMuchTracks->At(imuch));

    // track and TOFhit matches
    CbmTrackMatchNew* stsMatch = 0x0;
    if (fStsMatches && stsTrack) stsMatch = static_cast<CbmTrackMatchNew*>(fStsMatches->At(ists));
    Int_t istsMC                = (stsMatch && stsMatch->GetNofHits() > 0 ? stsMatch->GetMatchedLink().GetIndex() : -1);
    CbmTrackMatchNew* muchMatch = 0x0;
    if (fMuchMatches && muchTrack) muchMatch = static_cast<CbmTrackMatchNew*>(fMuchMatches->At(imuch));
    Int_t imuchMC = (muchMatch && muchMatch->GetNofHits() > 0 ? muchMatch->GetMatchedLink().GetIndex() : -1);
    CbmTrackMatchNew* trdMatch = 0x0;
    if (fTrdMatches && trdTrack) trdMatch = static_cast<CbmTrackMatchNew*>(fTrdMatches->At(itrd));
    Int_t itrdMC                = (trdMatch ? trdMatch->GetMatchedLink().GetIndex() : -1);
    CbmTrackMatchNew* richMatch = 0x0;
    if (fRichMatches && richRing) richMatch = static_cast<CbmTrackMatchNew*>(fRichMatches->At(irich));
    Int_t irichMC      = (richMatch && richMatch->GetNofHits() > 0 ? richMatch->GetMatchedLink().GetIndex() : -1);
    CbmMatch* tofMatch = 0x0;
    if (fTofHitMatches && tofHit)
      tofMatch = static_cast<CbmMatch*>(fTofHitMatches->At(itof));  //tofMatch = tofHit->GetMatch();
    FairMCPoint* tofPoint = 0x0;
    if (tofMatch && tofMatch->GetNofLinks() > 0)
      tofPoint = static_cast<FairMCPoint*>(fTofPoints->At(tofMatch->GetMatchedLink().GetIndex()));
    Int_t itofMC = (tofPoint ? tofPoint->GetTrackID() : -1);

    Int_t imvdMC = GetMvdMatchingIndex(stsTrack);

    // rich projection
    FairTrackParam* richProj = 0x0;
    if (fRichProjection) richProj = static_cast<FairTrackParam*>(fRichProjection->At(i));

    // monte carlo track based on the STS match!!!
    Int_t iMC           = istsMC;
    CbmMCTrack* mcTrack = 0x0;
    if (fMCTracks && iMC >= 0) mcTrack = static_cast<CbmMCTrack*>(fMCTracks->At(iMC));
    // increment position in matching array
    if (mcTrack && fMCTracks) matches[iMC]++;

    // build papa track
    fTracks->AddAtAndExpand(new PairAnalysisTrack(vtx, gtrk, stsTrack, muchTrack, trdTrack, richRing, tofHit, mcTrack,
                                                  stsMatch, muchMatch, trdMatch, richMatch, richProj, i),
                            i);

    // set MC label and matching bits
    PairAnalysisTrack* tr = static_cast<PairAnalysisTrack*>(fTracks->UncheckedAt(i));
    if (iMC < 0) iMC = -999;  // STS tracks w/o MC matching
    tr->SetLabel(iMC);
    // NOTE: sts track matching might include mvd points
    tr->SetBit(BIT(14 + ToIntegralType(ECbmModuleId::kMvd)), (iMC == imvdMC));
    tr->SetBit(BIT(14 + ToIntegralType(ECbmModuleId::kSts)), (iMC == istsMC));
    tr->SetBit(BIT(14 + ToIntegralType(ECbmModuleId::kRich)), (iMC == irichMC));
    tr->SetBit(BIT(14 + ToIntegralType(ECbmModuleId::kTrd)), (iMC == itrdMC));
    tr->SetBit(BIT(14 + ToIntegralType(ECbmModuleId::kTof)), (iMC == itofMC));
    tr->SetBit(BIT(14 + ToIntegralType(ECbmModuleId::kMuch)), (iMC == imuchMC));
  }

  // loop over all fast tracks and add them
  // NOTE: only when there are no global tracks
  if (!fGlobalTracks || !fGlobalTracks->GetEntriesFast()) {

    // loop over all fast tracks
    for (Int_t i = 0; i < (fFastTracks ? fFastTracks->GetEntriesFast() : 0); i++) {
      // fast(sim) track
      TParticle* ftrk = static_cast<TParticle*>(fFastTracks->UncheckedAt(i));
      if (!ftrk) continue;

      // monte carlo track
      Int_t iMC           = ftrk->GetFirstMother();
      CbmMCTrack* mcTrack = 0x0;
      if (fMCTracks && iMC >= 0) mcTrack = static_cast<CbmMCTrack*>(fMCTracks->At(iMC));
      // increment position in matching array
      if (mcTrack && fMCTracks) matches[iMC]++;
      // build papa track
      fTracks->AddAtAndExpand(new PairAnalysisTrack(ftrk, mcTrack), i);
    }
  }

  // number of multiple matched tracks
  fMultiMatch = (Int_t) matches.GetSum();
}


//______________________________________________
PairAnalysisTrack* PairAnalysisEvent::GetTrack(UInt_t pos)
{
  //
  // intialize the papa track and return it
  //

  // check intitialisation
  if (fTracks->GetSize() < 0 || UInt_t(fTracks->GetSize()) <= pos || !fTracks->UncheckedAt(pos))
    Fatal("PairAnalysisEvent::GetTrack", "Event initialisation failed somehow !!!");

  return static_cast<PairAnalysisTrack*>(fTracks->UncheckedAt(pos));
}

//______________________________________________
Int_t PairAnalysisEvent::GetNumberOfMatches(ECbmModuleId det) const
{
  //
  // number of track matches
  //
  switch (det) {
    case ECbmModuleId::kSts: return (fStsMatches ? fStsMatches->GetEntriesFast() : 0);
    case ECbmModuleId::kMuch: return (fMuchMatches ? fMuchMatches->GetEntriesFast() : 0);
    case ECbmModuleId::kTrd: return (fTrdMatches ? fTrdMatches->GetEntriesFast() : 0);
    case ECbmModuleId::kRich: return (fRichMatches ? fRichMatches->GetEntriesFast() : 0);
    default: return 0;
  }
}

//______________________________________________
Int_t PairAnalysisEvent::GetNumberOfHitMatches(ECbmModuleId det) const
{
  //
  // number of hit matches
  //
  if (!GetHitMatches(det)) { return 0; }
  else {
    return (GetHitMatches(det)->GetEntriesFast());
  }
}

//______________________________________________
Int_t PairAnalysisEvent::GetNumberOfHits(ECbmModuleId det) const
{
  //
  // number of reconstructed hits
  //
  if (!GetHits(det)) { return 0; }
  else {
    return (GetHits(det)->GetEntriesFast());
  }
}

//______________________________________________
Int_t PairAnalysisEvent::GetNumberOfPoints(ECbmModuleId det) const
{
  //
  // number of reconstructed hits
  //
  if (!GetPoints(det)) { return 0; }
  else {
    return (GetPoints(det)->GetEntriesFast());
  }
}

//______________________________________________
TClonesArray* PairAnalysisEvent::GetHits(ECbmModuleId det) const
{
  //
  // get hits array for certain detector
  //
  //TODO: add much straw hits
  switch (det) {
    case ECbmModuleId::kMvd: return fMvdHits;
    case ECbmModuleId::kSts: return fStsHits;
    case ECbmModuleId::kMuch: return fMuchHits;  //pixel
    case ECbmModuleId::kTrd: return fTrdHits;
    case ECbmModuleId::kRich: return fRichHits;
    case ECbmModuleId::kTof: return fTofHits;
    default: return 0x0;
  }
}

//______________________________________________
TClonesArray* PairAnalysisEvent::GetHitMatches(ECbmModuleId det) const
{
  //
  // get hit matches array for certain detector
  //
  //TODO: add much straw hits
  switch (det) {
    case ECbmModuleId::kMvd: return fMvdHitMatches;
    case ECbmModuleId::kSts: return fStsHitMatches;
    case ECbmModuleId::kMuch: return fMuchHitMatches;  //pixel
    case ECbmModuleId::kTrd: return fTrdHitMatches;
    case ECbmModuleId::kRich: return fRichHitMatches;
    case ECbmModuleId::kTof: return fTofHitMatches;
    default: return 0x0;
  }
}

//______________________________________________
TClonesArray* PairAnalysisEvent::GetPoints(ECbmModuleId det) const
{
  //
  // get mc points array for certain detector
  //
  switch (det) {
    case ECbmModuleId::kMvd: return fMvdPoints;
    case ECbmModuleId::kSts: return fStsPoints;
    case ECbmModuleId::kMuch: return fMuchPoints;
    case ECbmModuleId::kTrd: return fTrdPoints;
    case ECbmModuleId::kRich: return fRichPoints;
    case ECbmModuleId::kTof: return fTofPoints;
    default: return 0x0;
  }
}

//______________________________________________
TClonesArray* PairAnalysisEvent::GetCluster(ECbmModuleId det) const
{
  //
  // get cluster array for certain detector
  //
  switch (det) {
    // case ECbmModuleId::kMvd: return fMvdCluster;
    // case ECbmModuleId::kSts: return fStsCluster;
    // case ECbmModuleId::kMuch:return fMuchCluster; //pixel
    case ECbmModuleId::kTrd: return fTrdCluster;
    // case ECbmModuleId::kRich:return fRichCluster;
    // case ECbmModuleId::kTof: return fTofCluster;
    default: return 0x0;
  }
}

//______________________________________________
void PairAnalysisEvent::Clear(Option_t* /*opt*/)
{
  //
  // clear arrays
  //
  fTracks->Clear("C");
  // fGlobalTracks->Delete();   //global tracks
  // fTrdTracks->Delete();      //TRD tracks
  // fStsTracks->Delete();      //STS tracks
  // fMCTracks->Delete();       //mc tracks
  // fStsMatches->Delete();     //STS matches
}

//______________________________________________
Int_t PairAnalysisEvent::GetMvdMatchingIndex(CbmStsTrack* track) const
{
  //
  // calculate the standalone mvd mc matching
  //
  Int_t idx = -1;
  if (!track || !fMvdHitMatches) return idx;

  CbmTrackMatchNew* trackMatch = new CbmTrackMatchNew();

  Int_t nofMvdHits = track->GetNofMvdHits();
  for (Int_t iHit = 0; iHit < nofMvdHits; iHit++) {
    const CbmMatch* hitMatch = static_cast<CbmMatch*>(fMvdHitMatches->At(track->GetMvdHitIndex(iHit)));
    if (!hitMatch) continue;
    Int_t nofLinks = hitMatch->GetNofLinks();
    for (Int_t iLink = 0; iLink < nofLinks; iLink++) {
      const CbmLink& link      = hitMatch->GetLink(iLink);
      const FairMCPoint* point = static_cast<const FairMCPoint*>(fMvdPoints->At(link.GetIndex()));
      if (NULL == point) continue;
      trackMatch->AddLink(CbmLink(1., point->GetTrackID(), link.GetEntry(), link.GetFile()));
    }
  }
  if (trackMatch->GetNofLinks()) { idx = trackMatch->GetMatchedLink().GetIndex(); }

  //delete surplus stuff
  delete trackMatch;

  return idx;
}
