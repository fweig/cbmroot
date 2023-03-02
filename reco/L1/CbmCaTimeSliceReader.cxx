/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTimeSliceReader.cxx
/// @brief  Time-slice/event reader for CA tracker in CBM (implementation)
/// @since  24.02.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmCaTimeSliceReader.h"

#include "CbmGlobalTrack.h"
#include "CbmKFMath.h"  // for CopyTrackParam2TC
#include "CbmStsTrack.h"

#include "FairRootManager.h"
#include "Logger.h"

#include "L1Constants.h"
#include "L1InputData.h"
#include "L1Parameters.h"

using cbm::ca::TimeSliceReader;
using L1Constants::clrs::kCL;   // clear log
using L1Constants::clrs::kGNb;  // green bold log
using L1Constants::clrs::kRDb;  // red bold log

// ---------------------------------------------------------------------------------------------------------------------
//
TimeSliceReader::TimeSliceReader(ECbmTrackingMode mode) : fTrackingMode(mode) {}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::Clear()
{
  // Detector used
  fbUseMvd  = false;
  fbUseSts  = false;
  fbUseMuch = false;
  fbUseTrd  = false;
  fbUseTof  = false;

  // Input branches
  fpBrTimeSlice = nullptr;
  fpParameters  = nullptr;

  fpBrMvdHits  = nullptr;
  fpBrStsHits  = nullptr;
  fpBrMuchHits = nullptr;
  fpBrTrdHits  = nullptr;
  fpBrTofHits  = nullptr;

  fpBrRecoTracks = nullptr;
  fpBrStsTracks  = nullptr;
  fpBrMuchTracks = nullptr;
  fpBrTrdTracks  = nullptr;
  fpBrTofTracks  = nullptr;

  // Pointers to output containers
  fpvHitIds       = nullptr;
  fpvQaHits       = nullptr;
  fpIODataManager = nullptr;
  fpvTracks       = nullptr;

  fNofHits     = 0;
  fNofHitKeys  = 0;
  fFirstHitKey = 0;

  std::fill(fvHitFirstIndexDet.begin(), fvHitFirstIndexDet.end(), 0);
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::CheckInit() const
{
  // Check parameters
  if (!fpParameters.get()) { throw std::logic_error("Tracking parameters object was not defined"); }
  if (!fpvHitIds) { throw std::logic_error("Hit index container was not defined"); }

  if (!fpBrTimeSlice) { throw std::logic_error("Time slice branch is unavailable"); }

  if (fbUseMvd && !fpBrMvdHits) { throw std::logic_error("MVD hits branch is unavailable"); }
  if (fbUseSts && !fpBrStsHits) { throw std::logic_error("STS hits branch is unavailable"); }
  if (fbUseMuch && !fpBrMuchHits) { throw std::logic_error("MuCh hits branch is unavailable"); }
  if (fbUseTrd && !fpBrTrdHits) { throw std::logic_error("TRD hits branch is unavailable"); }
  if (fbUseTof && !fpBrTofHits) { throw std::logic_error("TOF hits branch is unavailable"); }

  if (fpvTracks) {
    if (ECbmTrackingMode::kSTS == fTrackingMode) {
      if (!fpBrRecoTracks) { throw std::logic_error("StsTrack branch is unavailable"); }
    }
    else if (ECbmTrackingMode::kMCBM == fTrackingMode) {
      if (!fpBrRecoTracks) { throw std::logic_error("GlobalTrack branch is unavailable"); }
      if (fbUseSts && !fpBrStsTracks) { throw std::logic_error("StsTrack branch is unavailable"); }
      if (fbUseMuch && !fpBrMuchTracks) { throw std::logic_error("MuchTrack branch is unavailable"); }
      if (fbUseTrd && !fpBrRecoTracks) { throw std::logic_error("TrdTrack branch is unavailable"); }
      if (fbUseTof && !fpBrRecoTracks) { throw std::logic_error("TofTrack branch is unavailable"); }
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool TimeSliceReader::InitRun()
try {
  LOG(info) << "TimeSliceReader: initializing run ... ";

  // Init tracking detector interfaces
  fpMvdInterface  = CbmMvdTrackingInterface::Instance();
  fpStsInterface  = CbmStsTrackingInterface::Instance();
  fpMuchInterface = CbmMuchTrackingInterface::Instance();
  fpTrdInterface  = CbmTrdTrackingInterface::Instance();
  fpTofInterface  = CbmTofTrackingInterface::Instance();

  // ** Init data branches **

  auto* pFairManager = FairRootManager::Instance();
  LOG_IF(fatal, !pFairManager) << "TimeSliceReader: FairRootManager was not defined";

  fpBrTimeSlice = dynamic_cast<CbmTimeSlice*>(pFairManager->GetObject("TimeSlice."));

  // Init hit branches
  if (fbUseMvd) { fpBrMvdHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("MvdHit")); }
  if (fbUseSts) { fpBrStsHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsHit")); }
  if (fbUseMuch) { fpBrMuchHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("MuchPixelHit")); }
  if (fbUseTrd) { fpBrTrdHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TrdHit")); }
  if (fbUseTof) { fpBrTofHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TofHit")); }

  // Init track branches
  if (fpvTracks) {
    switch (fTrackingMode) {
      case ECbmTrackingMode::kSTS:
        fpBrRecoTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsTrack"));
        break;
      case ECbmTrackingMode::kMCBM:
        fpBrRecoTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("GlobalTrack"));
        if (fbUseSts) { fpBrStsTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsTrack")); }
        if (fbUseMuch) { fpBrMuchTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("MuchTrack")); }
        if (fbUseTrd) { fpBrTrdTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TrdTrack")); }
        if (fbUseTof) { fpBrTofTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TofTrack")); }
        break;
    }
  }

  // Check initialization
  this->CheckInit();

  LOG(info) << "TimeSliceReader: initializing run ... " << kGNb << "done" << kCL;
  return true;
}
catch (const std::logic_error& error) {
  LOG(info) << "TimeSliceReader: initializing run ... " << kRDb << "failed" << kCL << "\nReason: " << error.what();
  return false;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::InitTimeSlice()
{
  this->ReadHits();
  this->ReadRecoTracks();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::ReadHits()
{
  fNofHits     = 0;
  fNofHitKeys  = 0;
  fFirstHitKey = 0;

  // Get total number of hits
  int nHitsTot = 0;
  if (fbUseMvd) { nHitsTot += fpBrMvdHits->GetEntriesFast(); }
  if (fbUseSts) { nHitsTot += fpBrStsHits->GetEntriesFast(); }
  if (fbUseMuch) { nHitsTot += fpBrMuchHits->GetEntriesFast(); }
  if (fbUseTrd) { nHitsTot += fpBrTrdHits->GetEntriesFast(); }
  if (fbUseTof) { nHitsTot += fpBrTofHits->GetEntriesFast(); }

  // Resize the containers
  if (fpvHitIds) {
    fpvHitIds->clear();
    fpvHitIds->reserve(nHitsTot);
  }
  if (fpvQaHits) {
    fpvQaHits->clear();
    fpvQaHits->reserve(nHitsTot);
  }
  if (fpIODataManager) {
    fpIODataManager->ResetInputData();
    fpIODataManager->ReserveNhits(nHitsTot);
  }

  std::fill(fvHitFirstIndexDet.begin(), fvHitFirstIndexDet.end(), 0);

  // Save number of hits stored
  std::array<int, L1Constants::size::kMaxNdetectors> vNofHits = {0};

  // Read hits for different detectors
  if (fbUseMvd) { vNofHits[int(L1DetectorID::kMvd)] += ReadHitsForDetector<L1DetectorID::kMvd>(fpBrMvdHits); }
  if (fbUseSts) { vNofHits[int(L1DetectorID::kSts)] += ReadHitsForDetector<L1DetectorID::kSts>(fpBrStsHits); }
  if (fbUseMuch) { vNofHits[int(L1DetectorID::kMuch)] += ReadHitsForDetector<L1DetectorID::kMuch>(fpBrMuchHits); }
  if (fbUseTrd) { vNofHits[int(L1DetectorID::kTrd)] += ReadHitsForDetector<L1DetectorID::kTrd>(fpBrTrdHits); }
  if (fbUseTof) { vNofHits[int(L1DetectorID::kTof)] += ReadHitsForDetector<L1DetectorID::kTof>(fpBrTofHits); }

  // Save first hit index for different detector subsystems
  for (uint32_t iDet = 0; iDet < vNofHits.size(); ++iDet) {
    fvHitFirstIndexDet[iDet + 1] = fvHitFirstIndexDet[iDet] + vNofHits[iDet];
    fNofHits += vNofHits[iDet];
  }

  // Update maps of ext->int hit indexes
  // NOTE: fvpHitIds must be initialized, if we want to read tracks from the file
  if (fpvHitIds) {
    auto UpdateHitIndexMap = [&](L1Vector<int> & vIds, L1DetectorID detID) constexpr
    {
      vIds.reset(vNofHits[int(detID)]);
      for (int iH = fvHitFirstIndexDet[int(detID)]; iH < fvHitFirstIndexDet[int(detID) + 1]; ++iH) {
        vIds[(*fpvHitIds)[iH].hitId] = iH;
      }
    };
    if (fbUseMvd) { UpdateHitIndexMap(vHitMvdIds, L1DetectorID::kMvd); }
    if (fbUseSts) { UpdateHitIndexMap(vHitStsIds, L1DetectorID::kSts); }
    if (fbUseMuch) { UpdateHitIndexMap(vHitMuchIds, L1DetectorID::kMuch); }
    if (fbUseTrd) { UpdateHitIndexMap(vHitTrdIds, L1DetectorID::kTrd); }
    if (fbUseTof) { UpdateHitIndexMap(vHitTofIds, L1DetectorID::kTof); }
  }
  // Update number of hit keys in input data object
  if (fpIODataManager) { fpIODataManager->SetNhitKeys(fNofHitKeys); }

  // Sort debug hits
  if (fpvQaHits) { this->SortQaHits(); }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::ReadRecoTracks()
{
  if (!fpvTracks) {
    LOG(warn) << "TimeSliceReader: reconstructed tracks should not be read in this regime";
    return;
  }

  assert(fpBrRecoTracks);
  int nTracks = fpBrRecoTracks->GetEntriesFast();
  fpvTracks->reset(nTracks);
  switch (fTrackingMode) {
    case ECbmTrackingMode::kSTS:
      // Fill tracks from StsTrack branch
      for (int iT = 0; iT < nTracks; ++iT) {
        auto* pInputTrack = static_cast<CbmStsTrack*>(fpBrRecoTracks->At(iT));
        auto& track       = (*fpvTracks)[iT];
        CbmKFMath::CopyTrackParam2TC(pInputTrack->GetParamFirst(), track.T, track.C);
        CbmKFMath::CopyTrackParam2TC(pInputTrack->GetParamLast(), track.TLast, track.CLast);
        track.chi2      = pInputTrack->GetChiSq();
        track.NDF       = pInputTrack->GetNDF();
        track.Tpv[6]    = pInputTrack->GetStartTime();
        track.Cpv[20]   = pInputTrack->GetStartTimeError();
        track.T[6]      = pInputTrack->GetFirstHitTime();
        track.C[20]     = pInputTrack->GetFirstHitTimeError();
        track.TLast[6]  = pInputTrack->GetLastHitTime();
        track.CLast[20] = pInputTrack->GetLastHitTimeError();
        track.Hits.clear();
        track.Hits.reserve(pInputTrack->GetNofHits());
        for (int iH = 0; iH < pInputTrack->GetNofMvdHits(); ++iH) {
          int iHint = vHitMvdIds[pInputTrack->GetMvdHitIndex(iH)];
          track.Hits.push_back(iHint);  // !!!!
        }                               // iH
        for (int iH = 0; iH < pInputTrack->GetNofStsHits(); ++iH) {
          int iHint = vHitStsIds[pInputTrack->GetStsHitIndex(iH)];
          track.Hits.push_back(iHint);
        }  // iH
      }    // iT
      break;

    case ECbmTrackingMode::kMCBM:
      LOG(fatal) << "Sorry, mCBM mode has not been implemented yet. Stay tuned :)";
      //  TODO: fill Global track here
      break;
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::RegisterIODataManager(std::shared_ptr<L1IODataManager>& pIODataManager)
{
  LOG_IF(fatal, !pIODataManager.get()) << "TimeSliceReader: passed null pointer as a L1IODataManager instance";
  fpIODataManager = pIODataManager;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::SetDetector(L1DetectorID detID, bool flag)
{
  switch (detID) {
    case L1DetectorID::kMvd: fbUseMvd = flag; break;
    case L1DetectorID::kSts: fbUseSts = flag; break;
    case L1DetectorID::kMuch: fbUseMuch = flag; break;
    case L1DetectorID::kTrd: fbUseTrd = flag; break;
    case L1DetectorID::kTof: fbUseTof = flag; break;
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::SortQaHits()
{
  int nStationsActive = fpParameters->GetNstationsActive();
  L1Vector<CbmL1HitDebugInfo> vNewHits(fpvQaHits->size());
  std::vector<int> vHitFirstIndexes(nStationsActive + 1, 0);
  std::vector<int> vNofHitsStored(nStationsActive, 0);

  // Count number of hits in each station (NOTE: we could use here boarders from the IO data manager, but we would keep
  // these two procedures independent)
  for (const auto& hit : (*fpvQaHits)) {
    ++vHitFirstIndexes[hit.GetStationId() + 1];
  }

  for (int iSt = 0; iSt < nStationsActive; ++iSt) {
    vHitFirstIndexes[iSt + 1] += vHitFirstIndexes[iSt];
  }

  for (const auto& hit : (*fpvQaHits)) {
    int iSt                                                   = hit.GetStationId();
    vNewHits[vHitFirstIndexes[iSt] + (vNofHitsStored[iSt]++)] = hit;
  }

  std::swap(vNewHits, (*fpvQaHits));
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::StoreHitRecord(const HitRecord& hitRecord)
{
  // Save the algo hit
  if (fpIODataManager.get()) {
    L1Hit aHit;
    aHit.iSt = hitRecord.fStaId;
    aHit.f   = hitRecord.fStripF;
    aHit.b   = hitRecord.fStripB;
    aHit.ID  = static_cast<int>(fpIODataManager->GetNofHits());
    aHit.z   = hitRecord.fZ;
    aHit.u   = hitRecord.fU;
    aHit.v   = hitRecord.fV;
    aHit.t   = hitRecord.fT;
    aHit.dt2 = hitRecord.fDt * hitRecord.fDt;
    aHit.du2 = hitRecord.fDu * hitRecord.fDu;
    aHit.dv2 = hitRecord.fDv * hitRecord.fDv;
    fpIODataManager->PushBackHit(aHit, hitRecord.fDataStream);
  }

  // Save hit ID information
  if (fpvHitIds) { fpvHitIds->emplace_back(hitRecord.fDet, hitRecord.fExtId); }

  // Save debug information
  if (fpvQaHits) {
    CbmL1HitDebugInfo hitDbg;
    hitDbg.Det      = hitRecord.fDet;
    hitDbg.ExtIndex = hitRecord.fExtId;
    hitDbg.iStation = hitRecord.fStaId;
    hitDbg.x        = hitRecord.fX;
    hitDbg.y        = hitRecord.fY;
    hitDbg.dx       = hitRecord.fDx;
    hitDbg.dy       = hitRecord.fDy;
    hitDbg.dxy      = hitRecord.fDxy;
    hitDbg.time     = hitRecord.fT;
    fpvQaHits->push_back(hitDbg);
  }
}
