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
  fpvDbgHits      = nullptr;
  fpIODataManager = nullptr;
  fpvTracks       = nullptr;

  // Other
  fpParameters = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::InitRun()
{
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
  LOG_IF(fatal, !fpBrTimeSlice) << "TimeSliceReader: time slice was not defined";

  // Init hit branches
  if (fbUseMvd) {
    fpBrMvdHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("MvdHit"));
    LOG_IF(fatal, !fpBrMvdHits) << "TimeSliceReader: MVD hits were not found";
  }

  if (fbUseSts) {
    fpBrStsHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsHit"));
    LOG_IF(fatal, !fpBrStsHits) << "TimeSliceReader: STS hits were not found";
  }

  if (fbUseMuch) {
    fpBrMuchHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("MuchPixelHit"));
    LOG_IF(fatal, !fpBrMuchHits) << "TimeSliceReader: MuCh hits were not found";
  }

  if (fbUseTrd) {
    fpBrTrdHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TrdHit"));
    LOG_IF(fatal, !fpBrTrdHits) << "TimeSliceReader: TRD hits were not found";
  }

  if (fbUseTof) {
    fpBrTofHits = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TofHit"));
    LOG_IF(fatal, !fpBrTofHits) << "TimeSliceReader: TOF hits were not found";
  }

  // Init track branches
  if (fpvTracks) {
    switch (fTrackingMode) {
      case ECbmTrackingMode::kSTS:
        fpBrRecoTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsTrack"));
        LOG_IF(fatal, !fpBrRecoTracks) << "TimeSliceReader: StsTrack not found";
        break;
      case ECbmTrackingMode::kMCBM:
        fpBrRecoTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("GlobalTrack"));
        LOG_IF(fatal, !fpBrRecoTracks) << "TimeSliceReader: GlobalTrack not found";
        if (fbUseSts) {
          fpBrStsTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("StsTrack"));
          LOG_IF(fatal, !fpBrStsTracks) << "TimeSliceReader: StsTrack not found";
        }
        if (fbUseMuch) {
          fpBrMuchTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("MuchTrack"));
          LOG_IF(fatal, !fpBrMuchTracks) << "TimeSliceReader: MuchTrack not found";
        }
        if (fbUseTrd) {
          fpBrTrdTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TrdTrack"));
          LOG_IF(fatal, !fpBrTrdTracks) << "TimeSliceReader: TrdTrack not found";
        }
        if (fbUseTof) {
          fpBrTofTracks = dynamic_cast<TClonesArray*>(pFairManager->GetObject("TofTrack"));
          LOG_IF(fatal, !fpBrTofTracks) << "TimeSliceReader: TofTrack not found";
        }
        break;
    }
  }

  LOG(info) << "TimeSliceReader: initializing run ... " << kGNb << "done" << kCL;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::InitTimeSlice() { LOG(info) << "TimeSliceReader: initializing time slice"; }

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
  fpvHitIds->clear();
  fpvHitIds->reserve(nHitsTot);
  fpvDbgHits->clear();
  fpvDbgHits->reserve(nHitsTot);
  fpIODataManager->ResetInputData();
  fpIODataManager->ReserveNhits(nHitsTot);

  // Read hits for different detectors
  if (fbUseMvd) { fNofHits += ReadHitsForDetector<L1DetectorID::kMvd>(fpBrMvdHits); }
  if (fbUseSts) { fNofHits += ReadHitsForDetector<L1DetectorID::kSts>(fpBrStsHits); }
  if (fbUseMuch) { fNofHits += ReadHitsForDetector<L1DetectorID::kMuch>(fpBrMuchHits); }
  if (fbUseTrd) { fNofHits += ReadHitsForDetector<L1DetectorID::kTrd>(fpBrTrdHits); }
  if (fbUseTof) { fNofHits += ReadHitsForDetector<L1DetectorID::kTof>(fpBrTofHits); }

  fpIODataManager->SetNhitKeys(fNofHitKeys);
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
          track.Hits.push_back(-pInputTrack->GetMvdHitIndex(iH) - 1);  // !!!!
        }                                                              // iH
        for (int iH = 0; iH < pInputTrack->GetNofStsHits(); ++iH) {
          track.Hits.push_back(pInputTrack->GetStsHitIndex(iH));
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
void TimeSliceReader::RegisterHitDebugInfoContainer(L1Vector<CbmL1HitDebugInfo>& vDbgHits) { fpvDbgHits = &vDbgHits; }

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::RegisterHitIndexContainer(L1Vector<CbmL1HitId>& vHitIds) { fpvHitIds = &vHitIds; }

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::RegisterIODataManager(std::shared_ptr<L1IODataManager>& pIODataManager)
{
  LOG_IF(fatal, !pIODataManager.get()) << "TimeSliceReader: passed null pointer as a L1IODataManager instance";
  fpIODataManager = pIODataManager;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::RegisterTracksContainer(L1Vector<CbmL1Track>& vTracks) { fpvTracks = &vTracks; }

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
void TimeSliceReader::SetParameters(const L1Parameters* pParameters)
{
  LOG_IF(fatal, !pParameters) << "TimeSliceReader: passed null pointer as L1IODataManager instance";
  fpParameters = pParameters;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void TimeSliceReader::StoreHitRecord(const HitRecord& hitRecord)
{
  int iHitGlob = static_cast<int>(fpIODataManager->GetNofHits());  // Current hit global index

  // Save the algo hit
  if (fpIODataManager.get()) {
    L1Hit aHit;
    aHit.iSt = hitRecord.fStaId;
    aHit.f   = hitRecord.fStripF;
    aHit.b   = hitRecord.fStripB;
    aHit.ID  = -1;
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
  if (fpvDbgHits) {
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
    fpvDbgHits->push_back(hitDbg);
  }
}
