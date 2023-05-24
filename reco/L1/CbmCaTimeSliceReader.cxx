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
#include "CbmMuchTrack.h"
#include "CbmStsTrack.h"
#include "CbmTofTrack.h"
#include "CbmTrdTrack.h"

#include "FairRootManager.h"
#include "Logger.h"

#include <algorithm>
#include <numeric>

#include "L1Constants.h"
#include "L1InputData.h"
#include "L1Parameters.h"

using ca::tools::HitRecord;
using cbm::ca::TimeSliceReader;
using L1Constants::clrs::kCL;   // clear log
using L1Constants::clrs::kGNb;  // green bold log
using L1Constants::clrs::kRDb;  // red bold log

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::Clear()
{
  // Detector used
  fvbUseDet.fill(false);

  // Input branches
  fpBrTimeSlice = nullptr;
  fpParameters  = nullptr;

  fvpBrHits.fill(nullptr);

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

  for (int iDet = 0; iDet < static_cast<int>(L1DetectorID::kEND); ++iDet) {
    if (fvbUseDet[iDet] && !fvpBrHits[iDet]) {
      throw std::logic_error(std::string(kDetName[iDet]) + " hits branch is not found");
    }
  }

  if (fpvTracks) {
    if (ECbmCaTrackingMode::kSTS == fTrackingMode) {
      if (!fpBrRecoTracks) { throw std::logic_error("StsTrack branch is unavailable"); }
    }
    else if (ECbmCaTrackingMode::kMCBM == fTrackingMode) {
      if (!fpBrRecoTracks) { throw std::logic_error("GlobalTrack branch is unavailable"); }
      if (fvbUseDet[L1DetectorID::kSts] && !fpBrStsTracks) { throw std::logic_error("StsTrack branch is not found"); }
      if (fvbUseDet[L1DetectorID::kMuch] && !fpBrMuchTracks) {
        throw std::logic_error("MuchTrack branch is not found");
      }
      if (fvbUseDet[L1DetectorID::kTrd] && !fpBrRecoTracks) { throw std::logic_error("TrdTrack branch is not found"); }
      if (fvbUseDet[L1DetectorID::kTof] && !fpBrRecoTracks) { throw std::logic_error("TofTrack branch is not found"); }
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
bool TimeSliceReader::InitRun()
try {
  LOG(info) << "TimeSliceReader: initializing run ... ";

  // Init tracking detector interfaces
  fvpDetInterface[L1DetectorID::kMvd]  = CbmMvdTrackingInterface::Instance();
  fvpDetInterface[L1DetectorID::kSts]  = CbmStsTrackingInterface::Instance();
  fvpDetInterface[L1DetectorID::kMuch] = CbmMuchTrackingInterface::Instance();
  fvpDetInterface[L1DetectorID::kTrd]  = CbmTrdTrackingInterface::Instance();
  fvpDetInterface[L1DetectorID::kTof]  = CbmTofTrackingInterface::Instance();

  // ** Init data branches **

  auto* pFairManager = FairRootManager::Instance();
  LOG_IF(fatal, !pFairManager) << "TimeSliceReader: FairRootManager was not defined";

  fpBrTimeSlice = dynamic_cast<CbmTimeSlice*>(pFairManager->GetObject("TimeSlice."));

  // Init branches
  auto InitHitBranch = [&](L1DetectorID detID, const char* branchName) {
    if (fvbUseDet[detID]) { fvpBrHits[detID] = dynamic_cast<TClonesArray*>(pFairManager->GetObject(branchName)); }
  };

  InitHitBranch(L1DetectorID::kMvd, "MvdHit");
  InitHitBranch(L1DetectorID::kSts, "StsHit");
  InitHitBranch(L1DetectorID::kMuch, "MuchPixelHit");
  InitHitBranch(L1DetectorID::kTrd, "TrdHit");
  InitHitBranch(L1DetectorID::kTof, "TofHit");

  // Init track branches

  if (fpvTracks) {
    switch (fTrackingMode) {
      case ECbmCaTrackingMode::kSTS:
        fpBrRecoTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsTrack"));
        break;
      case ECbmCaTrackingMode::kMCBM:
        fpBrRecoTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("GlobalTrack"));
        if (fvbUseDet[L1DetectorID::kSts]) {
          fpBrStsTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsTrack"));
        }
        if (fvbUseDet[L1DetectorID::kMuch]) {
          fpBrMuchTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("MuchTrack"));
        }
        if (fvbUseDet[L1DetectorID::kTrd]) {
          fpBrTrdTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TrdTrack"));
        }
        if (fvbUseDet[L1DetectorID::kTof]) {
          fpBrTofTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TofTrack"));
        }
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
    case ECbmCaTrackingMode::kSTS:
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
          int iHext = pInputTrack->GetMvdHitIndex(iH);
          int iHint = fvvHitExtToIntIndexMap[L1DetectorID::kMvd][iHext];
          track.Hits.push_back(iHint);
        }  // iH
        for (int iH = 0; iH < pInputTrack->GetNofStsHits(); ++iH) {
          int iHext = pInputTrack->GetStsHitIndex(iH);
          int iHint = fvvHitExtToIntIndexMap[L1DetectorID::kSts][iHext];
          track.Hits.push_back(iHint);
        }  // iH
      }    // iT
      break;

    case ECbmCaTrackingMode::kMCBM:
      //LOG(fatal) << "Sorry, mCBM mode has not been implemented yet. Stay tuned :)";
      // Fill tracks from GlobalTrack branch
      for (int iT = 0; iT < nTracks; ++iT) {
        auto* pInputTrack = static_cast<CbmGlobalTrack*>(fpBrRecoTracks->At(iT));
        auto& track       = (*fpvTracks)[iT];
        CbmKFMath::CopyTrackParam2TC(pInputTrack->GetParamFirst(), track.T, track.C);
        CbmKFMath::CopyTrackParam2TC(pInputTrack->GetParamLast(), track.TLast, track.CLast);
        track.chi2 = pInputTrack->GetChi2();
        track.NDF  = pInputTrack->GetNDF();

        // ** Fill information from local tracks **
        // STS tracks (+ MVD)
        if (fvbUseDet[L1DetectorID::kSts]) {
          int iStsTrkId = pInputTrack->GetStsTrackIndex();
          if (iStsTrkId > -1) {
            auto* pStsTrack = static_cast<CbmStsTrack*>(fpBrStsTracks->At(iStsTrkId));
            if (fvbUseDet[L1DetectorID::kMvd]) {
              for (int iH = 0; iH < pStsTrack->GetNofMvdHits(); ++iH) {
                int iHext = pStsTrack->GetMvdHitIndex(iH);
                int iHint = fvvHitExtToIntIndexMap[L1DetectorID::kMvd][iHext];
                track.Hits.push_back(iHint);
              }
            }
            for (int iH = 0; iH < pStsTrack->GetNofStsHits(); ++iH) {
              int iHext = pStsTrack->GetStsHitIndex(iH);
              int iHint = fvvHitExtToIntIndexMap[L1DetectorID::kSts][iHext];
              track.Hits.push_back(iHint);
            }
          }
        }

        // MUCH tracks
        if (fvbUseDet[L1DetectorID::kMuch]) {
          int iMuchTrkId = pInputTrack->GetMuchTrackIndex();
          if (iMuchTrkId > -1) {
            auto* pMuchTrack = static_cast<CbmMuchTrack*>(fpBrMuchTracks->At(iMuchTrkId));
            for (int iH = 0; iH < pMuchTrack->GetNofHits(); ++iH) {
              int iHext = pMuchTrack->GetHitIndex(iH);
              int iHint = fvvHitExtToIntIndexMap[L1DetectorID::kMuch][iHext];
              track.Hits.push_back(iHint);
            }
          }
        }

        // TRD tracks
        if (fvbUseDet[L1DetectorID::kTrd]) {
          int iTrdTrkId = pInputTrack->GetTrdTrackIndex();
          if (iTrdTrkId > -1) {
            const auto* pTrdTrack = static_cast<const CbmTrdTrack*>(fpBrTrdTracks->At(iTrdTrkId));
            for (int iH = 0; iH < pTrdTrack->GetNofHits(); ++iH) {
              int iHext = pTrdTrack->GetHitIndex(iH);
              int iHint = fvvHitExtToIntIndexMap[L1DetectorID::kTrd][iHext];
              track.Hits.push_back(iHint);
            }  // iH
          }
        }

        // TOF tracks
        if (fvbUseDet[L1DetectorID::kTof]) {
          int iTofTrkId = pInputTrack->GetTofTrackIndex();
          if (iTofTrkId > -1) {
            const auto* pTofTrack = static_cast<const CbmTofTrack*>(fpBrTofTracks->At(iTofTrkId));
            for (int iH = 0; iH < pTofTrack->GetNofHits(); ++iH) {
              int iHext = pTofTrack->GetHitIndex(iH);
              int iHint = fvvHitExtToIntIndexMap[L1DetectorID::kTof][iHext];
              track.Hits.push_back(iHint);
            }  // iH
          }    // if iTofTrkId > -1
        }      // if fvbUseDet[L1DetectorID::kTof]
      }        // iT
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
void TimeSliceReader::SortQaHits()
{
  int nStationsActive = fpParameters->GetNstationsActive();
  L1Vector<CbmL1HitDebugInfo> vNewHits(fpvQaHits->size());
  std::vector<int> vHitFstIndexes(nStationsActive + 1, 0);
  std::vector<int> vNofHitsStored(nStationsActive, 0);

  // Count number of hits in each station (NOTE: we could use here boarders from the IO data manager, but we would keep
  // these two procedures independent)
  std::for_each(fpvQaHits->begin(), fpvQaHits->end(), [&](const auto& h) { ++vHitFstIndexes[h.GetStationId() + 1]; });
  for (int iSt = 0; iSt < nStationsActive; ++iSt) {
    vHitFstIndexes[iSt + 1] += vHitFstIndexes[iSt];
  }

  for (const auto& hit : (*fpvQaHits)) {
    int iSt                                                 = hit.GetStationId();
    vNewHits[vHitFstIndexes[iSt] + (vNofHitsStored[iSt]++)] = hit;
  }

  std::swap(vNewHits, (*fpvQaHits));
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::ReadHits()
{
  fNofHits     = 0;
  fNofHitKeys  = 0;
  fFirstHitKey = 0;

  // TODO: Address case with CbmEvent != nullptr
  for (int iDet = 0; iDet < static_cast<int>(L1DetectorID::kEND); ++iDet) {
    if (fvbUseDet[iDet]) { fvNofHitsTotal[iDet] = fvpBrHits[iDet]->GetEntriesFast(); }
  }

  int nHitsTot = std::accumulate(fvNofHitsTotal.begin(), fvNofHitsTotal.end(), 0);

  // Resize the containers
  if (fpvHitIds) {
    fpvHitIds->clear();
    fpvHitIds->reserve(nHitsTot);
  }
  if (fpvQaHits) {
    fpvQaHits->clear();
    fpvQaHits->reserve(nHitsTot);
  }
  if (fpIODataManager) { fpIODataManager->ResetInputData(nHitsTot); }

  std::fill(fvHitFirstIndexDet.begin(), fvHitFirstIndexDet.end(), 0);

  // Read hits for different detectors
  fvNofHitsUsed[L1DetectorID::kMvd]  = ReadHitsForDetector<L1DetectorID::kMvd>();
  fvNofHitsUsed[L1DetectorID::kSts]  = ReadHitsForDetector<L1DetectorID::kSts>();
  fvNofHitsUsed[L1DetectorID::kMuch] = ReadHitsForDetector<L1DetectorID::kMuch>();
  fvNofHitsUsed[L1DetectorID::kTrd]  = ReadHitsForDetector<L1DetectorID::kTrd>();
  fvNofHitsUsed[L1DetectorID::kTof]  = ReadHitsForDetector<L1DetectorID::kTof>();

  // Save first hit index for different detector subsystems
  for (uint32_t iDet = 0; iDet < fvNofHitsUsed.size(); ++iDet) {
    fvHitFirstIndexDet[iDet + 1] = fvHitFirstIndexDet[iDet] + fvNofHitsUsed[iDet];
  }

  fNofHits = std::accumulate(fvNofHitsUsed.cbegin(), fvNofHitsUsed.cend(), 0);

  // Update number of hit keys in input data object
  if (fpIODataManager) { fpIODataManager->SetNhitKeys(fNofHitKeys); }

  // Sort debug hits
  if (fpvQaHits) { this->SortQaHits(); }

  // Update maps of ext->int hit indexes
  // NOTE: fvpHitIds must be initialized, if we want to read tracks from the file
  if (fpvHitIds) {
    auto ResetIndexMap = [&, idx = 0](auto& v) mutable { v.reset(fvNofHitsTotal[idx++]); };
    std::for_each(fvvHitExtToIntIndexMap.begin(), fvvHitExtToIntIndexMap.end(), ResetIndexMap);

    for (int iH = 0; iH < fNofHits; ++iH) {
      const auto& hit                               = (*fpvQaHits)[iH];
      fvvHitExtToIntIndexMap[hit.Det][hit.ExtIndex] = iH;
    }
  }
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
    CbmL1HitDebugInfo aHitQa;
    aHitQa.Det      = hitRecord.fDet;
    aHitQa.ExtIndex = hitRecord.fExtId;
    aHitQa.IntIndex = static_cast<int>(fpvQaHits->size());
    aHitQa.iStation = hitRecord.fStaId;
    aHitQa.x        = hitRecord.fX;
    aHitQa.y        = hitRecord.fY;
    aHitQa.dx       = hitRecord.fDx;
    aHitQa.dy       = hitRecord.fDy;
    aHitQa.dxy      = hitRecord.fDxy;
    aHitQa.time     = hitRecord.fT;
    fpvQaHits->push_back(aHitQa);
  }
}
