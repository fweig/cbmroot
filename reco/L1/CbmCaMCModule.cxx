/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmCaMCModule.cxx
/// \brief  CA Tracking performance interface for CBM (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CbmCaMCModule.h"

#include "CbmEvent.h"
#include "CbmL1Constants.h"
#include "CbmL1DetectorID.h"
#include "CbmL1Hit.h"
#include "CbmMCDataManager.h"
#include "CbmMCDataObject.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmStsHit.h"
#include "CbmTimeSlice.h"

#include "FairEventHeader.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "Logger.h"

#include "TDatabasePDG.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <boost/filesystem.hpp>

#include <algorithm>
#include <cassert>
#include <fstream>    // TODO: SZh 07.12.2022: For debug, should be removed!
#include <stdexcept>  // for std::logic_error

// *********************************
// ** Action definition functions **
// *********************************

// ---------------------------------------------------------------------------------------------------------------------
//
bool CbmCaMCModule::InitRun()
try {
  LOG(info) << "CA MC Module: initializing CA tracking Monte-Carlo module... ";

  auto fairManager = FairRootManager::Instance();
  assert(fairManager);

  auto mcManager = dynamic_cast<CbmMCDataManager*>(fairManager->GetObject("MCDataManager"));
  assert(mcManager);

  if (!fbLegacyEventMode) {
    fpTimeSlice   = dynamic_cast<CbmTimeSlice*>(fairManager->GetObject("TimeSlice."));
    fpMCEventList = dynamic_cast<CbmMCEventList*>(fairManager->GetObject("MCEventList."));
  }

  fpMCEventHeader = mcManager->GetObject("MCEventHeader.");
  fpMCTracks      = mcManager->InitBranch("MCTrack");

  fpMvdPoints  = nullptr;
  fpStsPoints  = nullptr;
  fpMuchPoints = nullptr;
  fpTrdPoints  = nullptr;
  fpTofPoints  = nullptr;

  fpMvdHitMatches  = nullptr;
  fpStsHitMatches  = nullptr;
  fpMuchHitMatches = nullptr;
  fpTrdHitMatches  = nullptr;
  fpTofHitMatches  = nullptr;

  fpStsClusterMatches = nullptr;
  fpStsHits           = nullptr;

  fFileEventIDs.clear();

  if (fbUseMvd) {
    LOG(info) << "CA MC Module: initializing branches for MVD";
    fpMvdPoints     = mcManager->InitBranch("MvdPoint");
    fpMvdHitMatches = dynamic_cast<TClonesArray*>(fairManager->GetObject("MvdHitMatch"));
  }

  if (fbUseSts) {
    LOG(info) << "CA MC Module: initializing branches for STS";
    fpStsPoints         = mcManager->InitBranch("StsPoint");
    fpStsHitMatches     = dynamic_cast<TClonesArray*>(fairManager->GetObject("StsHitMatch"));
    fpStsClusterMatches = dynamic_cast<TClonesArray*>(fairManager->GetObject("StsClusterMatch"));
    fpStsHits           = dynamic_cast<TClonesArray*>(fairManager->GetObject("StsHit"));
  }

  if (fbUseMuch) {
    LOG(info) << "CA MC Module: initializing branches for MuCh";
    fpMuchPoints     = mcManager->InitBranch("MuchPoint");
    fpMuchHitMatches = dynamic_cast<TClonesArray*>(fairManager->GetObject("MuchPixelHitMatch"));
  }

  if (fbUseTrd) {
    LOG(info) << "CA MC Module: initializing branches for TRD";
    fpTrdPoints     = mcManager->InitBranch("TrdPoint");
    fpTrdHitMatches = dynamic_cast<TClonesArray*>(fairManager->GetObject("TrdHitMatch"));
  }

  if (fbUseTof) {
    LOG(info) << "CA MC Module: initializing branches for TOF";
    fpTofPoints     = mcManager->InitBranch("TofPoint");
    fpTofHitMatches = dynamic_cast<TClonesArray*>(fairManager->GetObject("TofHitMatch"));
  }

  // Check initialization
  this->CheckInit();

  LOG(info) << "CA MC Module: initializing CA tracking Monte-Carlo module... \033[1;32mDone!\033[0m";

  return true;
}
catch (const std::logic_error& error) {
  LOG(info) << "CA MC Module: initializing CA tracking Monte-Carlo module... \033[1;31mFailed\033[0m\nReason: "
            << error.what();
  return false;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::InitEvent(CbmEvent* /*pEvent*/)
{
  // Fill a set of file and event indexes
  fFileEventIDs.clear();
  if (fbLegacyEventMode) {
    int iFile  = FairRunAna::Instance()->GetEventHeader()->GetInputFileId();
    int iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
    fFileEventIDs.insert({iFile, iEvent});
  }
  else {
    int nEvents = fpMCEventList->GetNofEvents();
    for (int iE = 0; iE < nEvents; ++iE) {
      int iFile  = fpMCEventList->GetFileIdByIndex(iE);
      int iEvent = fpMCEventList->GetEventIdByIndex(iE);
      fFileEventIDs.insert({iFile, iEvent});
    }
  }

  // Read data
  fpMCData->Clear();
  this->ReadMCTracks();
  this->ReadMCPoints();

  // Prepare tracks: set point indexes and redefine indexes from external to internal containers
  for (auto& aTrk : fpMCData->GetTrackContainer()) {
    aTrk.SortPointIndexes(
      [&](const int& iPl, const int& iPr) { return fpMCData->GetPoint(iPl).GetZ() < fpMCData->GetPoint(iPr).GetZ(); });
  }

  // ------ Match reconstructed and MC data
  this->MatchRecoAndMC();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::ProcessEvent(CbmEvent*) {}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::InitTrackInfo()
{
  // ----- Initialize stations arrangement and hit indexes
  fpMCData->InitTrackInfo(*fpvQaHits);

  // ----- Define reconstructable and additional flags
  for (auto& aTrk : fpMCData->GetTrackContainer()) {
    bool isRec = true;  // the track can be reconstructed

    // Cut on momentum
    isRec &= aTrk.GetP() > CbmL1Constants::MinRecoMom;

    // Cut on max number of points on station
    isRec &= aTrk.GetMaxNofPointsOnStation() <= 4;

    bool isAdd = isRec;  // is track additional

    // Cut on number of stations
    switch (fPerformanceMode) {
      case 1: isRec &= aTrk.GetNofConsStationsWithHit() >= CbmL1Constants::MinNStations; break;
      case 2: isRec &= aTrk.GetTotNofStationsWithHit() >= CbmL1Constants::MinNStations; break;
      case 3: isRec &= aTrk.GetNofConsStationsWithPoint() >= CbmL1Constants::MinNStations; break;
      default: LOG(fatal) << "CA MC Module: invalid performance mode " << fPerformanceMode;
    }

    if (aTrk.GetNofPoints() > 0) {
      isAdd &= aTrk.GetNofConsStationsWithHit() == aTrk.GetTotNofStationsWithHit();
      isAdd &= aTrk.GetNofConsStationsWithPoint() == aTrk.GetTotNofStationsWithHit();
      isAdd &= aTrk.GetTotNofStationsWithHit() == aTrk.GetTotNofStationsWithPoint();
      isAdd &= aTrk.GetNofConsStationsWithHit() >= 3;
      isAdd &= fpMCData->GetPoint(aTrk.GetPointIndexes()[0]).GetStationId() == 0;
      isAdd &= !isRec;
    }
    else {
      isAdd = false;
    }
    aTrk.SetFlagReconstructable(isRec);
    aTrk.SetFlagAdditional(isAdd);
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::Finish() {}


// **********************************
// **     Reco and MC matching     **
// **********************************

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::MatchRecoAndMC()
{
  this->MatchPointsAndHits();
  this->MatchRecoAndMCTracks();
  this->InitTrackInfo();
}


// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmCaMCModule::MatchHitWithMc<L1DetectorID::kMvd>(int iHitExt) const
{
  int iPoint = -1;
  if (fpMvdHitMatches) {
    const auto* hitMatch = dynamic_cast<CbmMatch*>(fpMvdHitMatches->At(iHitExt));
    assert(hitMatch);
    if (hitMatch->GetNofLinks() > 0 && hitMatch->GetLink(0).GetIndex() < fvNofPointsUsed[int(L1DetectorID::kMvd)]) {
      iPoint = hitMatch->GetLink(0).GetIndex();
    }
  }
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmCaMCModule::MatchHitWithMc<L1DetectorID::kSts>(int iHitExt) const
{
  int iPoint     = -1;
  const auto* sh = dynamic_cast<CbmStsHit*>(fpStsHits->At(iHitExt));

  // Match MC point
  if (fpStsClusterMatches) {
    const auto* clusterMatchF = static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetFrontClusterId()));
    const auto* clusterMatchB = static_cast<const CbmMatch*>(fpStsClusterMatches->At(sh->GetBackClusterId()));
    CbmMatch hitMatch;
    for (int iLinkF = 0; iLinkF < clusterMatchF->GetNofLinks(); ++iLinkF) {
      const auto& linkF = clusterMatchF->GetLink(iLinkF);
      for (int iLinkB = 0; iLinkB < clusterMatchB->GetNofLinks(); ++iLinkB) {
        const auto& linkB = clusterMatchB->GetLink(iLinkB);
        if (linkF == linkB) {
          hitMatch.AddLink(linkF);
          hitMatch.AddLink(linkB);
        }
      }
    }
    float bestWeight = 0.f;
    for (int iLink = 0; iLink < hitMatch.GetNofLinks(); ++iLink) {
      const CbmLink& link = hitMatch.GetLink(iLink);

      int iIndex = link.GetIndex();
      if (iIndex < 0) {
        iPoint = -1;
        break;
      }
      int iFile  = link.GetFile();
      int iEvent = link.GetEntry();

      if (fbLegacyEventMode) {
        iFile  = fFileEventIDs.begin()->first;
        iEvent = fFileEventIDs.begin()->second;

        // NOTE: Assertions below are temporary for testing consistency between link.GetFile() and
        //       fvFileEvent.begin()->first. It seems like this if-block is not needed, because
        //       assertions do not work out for a single file case (SZh)
        assert(iFile == link.GetFile());
        assert(iEvent == link.GetEntry());
      }

      if (link.GetWeight() > bestWeight) {
        bestWeight = link.GetWeight();
        iPoint     = fpMCData->FindInternalPointIndex(CalcGlobMCPointIndex(iIndex, L1DetectorID::kSts), iEvent, iFile);
        assert(iPoint != -1);
      }
    }
  }  // Match MC point
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmCaMCModule::MatchHitWithMc<L1DetectorID::kMuch>(int iHitExt) const
{
  int iPoint               = -1;
  const auto* hitMatchMuch = dynamic_cast<CbmMatch*>(fpMuchHitMatches->At(iHitExt));
  if (hitMatchMuch) {
    for (int iLink = 0; iLink < hitMatchMuch->GetNofLinks(); ++iLink) {
      if (hitMatchMuch->GetLink(iLink).GetIndex() < fvNofPointsUsed[int(L1DetectorID::kMuch)]) {
        int iMc = hitMatchMuch->GetLink(iLink).GetIndex();
        if (iMc < 0) {
          iPoint = -1;
          break;
        }
        int iFile  = hitMatchMuch->GetLink(iLink).GetFile();
        int iEvent = hitMatchMuch->GetLink(iLink).GetEntry();
        iPoint     = fpMCData->FindInternalPointIndex(CalcGlobMCPointIndex(iMc, L1DetectorID::kMuch), iEvent, iFile);
        assert(iPoint != -1);
      }
    }
  }
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmCaMCModule::MatchHitWithMc<L1DetectorID::kTrd>(int iHitExt) const
{
  int iPoint           = -1;
  const auto* hitMatch = dynamic_cast<const CbmMatch*>(fpTrdHitMatches->At(iHitExt));
  if (hitMatch) {
    int iMC = -1;
    if (hitMatch->GetNofLinks() > 0) {
      iMC = hitMatch->GetLink(0).GetIndex();
      assert(iMC >= 0 && iMC < fvNofPointsUsed[int(L1DetectorID::kTrd)]);

      int iMc = hitMatch->GetLink(0).GetIndex();
      if (iMc < 0) return iPoint;
      int iFile  = hitMatch->GetLink(0).GetFile();
      int iEvent = hitMatch->GetLink(0).GetEntry();

      iPoint = fpMCData->FindInternalPointIndex(CalcGlobMCPointIndex(iMc, L1DetectorID::kTrd), iEvent, iFile);
      assert(iPoint != -1);
    }
  }
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
int CbmCaMCModule::MatchHitWithMc<L1DetectorID::kTof>(int iHitExt) const
{
  int iPoint           = -1;
  const auto* hitMatch = dynamic_cast<const CbmMatch*>(fpTofHitMatches->At(iHitExt));
  if (hitMatch) {
    for (int iLink = 0; iLink < hitMatch->GetNofLinks(); ++iLink) {
      int iMc    = hitMatch->GetLink(iLink).GetIndex();
      int iFile  = hitMatch->GetLink(iLink).GetFile();
      int iEvent = hitMatch->GetLink(iLink).GetEntry();
      if (iMc < 0) return iPoint;

      assert(iMc >= 0 && iMc < fpTofPoints->Size(iFile, iEvent));
      int iPointPrelim = fpMCData->FindInternalPointIndex(CalcGlobMCPointIndex(iMc, L1DetectorID::kTof), iEvent, iFile);
      if (iPointPrelim == -1) { continue; }
      iPoint = iPointPrelim;
    }
  }
  return iPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::MatchPointsAndHits()
{
  // Clear point indexes of each hit
  // NOTE: at the moment only the best point is stored to the hit, but we keep the possibility for keeping many
  //       points for possible future module extension.
  for (auto& hit : (*fpvQaHits)) {
    hit.mcPointIds.clear();
  }

  // Match hit with point for different detectors
  auto DoMatch = [&](int iH, int iP) constexpr
  {
    if (iP > -1) {
      auto& hit = (*fpvQaHits)[iH];
      hit.mcPointIds.push_back_no_warning(iP);
      fpMCData->GetPoint(iP).AddHitID(iH);
    }
  };

  int iH = 0;
  if (fbUseMvd) {
    for (iH = (*fpvFstHitId)[int(L1DetectorID::kMvd)]; iH < (*fpvFstHitId)[int(L1DetectorID::kMvd) + 1]; ++iH) {
      DoMatch(iH, MatchHitWithMc<L1DetectorID::kMvd>((*fpvQaHits)[iH].ExtIndex));
    }
  }
  if (fbUseSts) {
    for (iH = (*fpvFstHitId)[int(L1DetectorID::kSts)]; iH < (*fpvFstHitId)[int(L1DetectorID::kSts) + 1]; ++iH) {
      DoMatch(iH, MatchHitWithMc<L1DetectorID::kSts>((*fpvQaHits)[iH].ExtIndex));
    }
  }
  if (fbUseSts) {
    for (iH = (*fpvFstHitId)[int(L1DetectorID::kMuch)]; iH < (*fpvFstHitId)[int(L1DetectorID::kMuch) + 1]; ++iH) {
      DoMatch(iH, MatchHitWithMc<L1DetectorID::kMuch>((*fpvQaHits)[iH].ExtIndex));
    }
  }
  if (fbUseTrd) {
    for (iH = (*fpvFstHitId)[int(L1DetectorID::kTrd)]; iH < (*fpvFstHitId)[int(L1DetectorID::kTrd) + 1]; ++iH) {
      DoMatch(iH, MatchHitWithMc<L1DetectorID::kTrd>((*fpvQaHits)[iH].ExtIndex));
    }
  }
  if (fbUseTof) {
    for (iH = (*fpvFstHitId)[int(L1DetectorID::kTof)]; iH < (*fpvFstHitId)[int(L1DetectorID::kTof) + 1]; ++iH) {
      DoMatch(iH, MatchHitWithMc<L1DetectorID::kTof>((*fpvQaHits)[iH].ExtIndex));
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::MatchRecoAndMCTracks()
{
  for (int iTre = 0; iTre < int(fpvRecoTracks->size()); ++iTre) {
    auto& trkRe = (*fpvRecoTracks)[iTre];

    // ----- Count number of hits from each MC track belonging to this reconstructed track
    auto& mNofHitsVsMCTrkID = trkRe.hitMap;
    //mNofHitsVsMCTrkID.clear();
    for (int iH : trkRe.Hits) {
      for (int iP : (*fpvQaHits)[iH].mcPointIds) {
        assert(iP > -1);  // Should never be triggered
        int iTmc = fpMCData->GetPoint(iP).GetTrackId();
        if (mNofHitsVsMCTrkID.find(iTmc) == mNofHitsVsMCTrkID.end()) { mNofHitsVsMCTrkID[iTmc] = 1; }
        else {
          mNofHitsVsMCTrkID[iTmc] += 1;
        }
      }  // loop over global point ids stored for hit: end
    }    // loop over hit ids stored for a reconstructed track: end


    // ----- Calculate track max purity
    // NOTE: Maximal purity is a maximum fraction of hits from a single MC track. A reconstructed track can be matched
    //       to several MC tracks, because it uses hits from different particle. Purity shows, how fully the true track
    //       was reconstructed.
    int nHitsTrkRe   = trkRe.Hits.size();  // number of hits in a given reconstructed track
    double maxPurity = 0.;                 // [0, 1]
    for (auto& item : mNofHitsVsMCTrkID) {
      int iTmc       = item.first;
      int nHitsTrkMc = item.second;

      if (iTmc < 0) { continue; }

      if (double(nHitsTrkMc) > double(nHitsTrkRe) * maxPurity) { maxPurity = double(nHitsTrkMc) / double(nHitsTrkRe); }

      ca::tools::MCTrack& trkMc = fpMCData->GetTrack(iTmc);

      // Match reconstructed and MC tracks, if purity with this MC track passes the threshold
      if (double(nHitsTrkMc) >= CbmL1Constants::MinPurity * double(nHitsTrkRe)) {
        trkMc.AddRecoTrackIndex(iTre);
        trkRe.AddMCTrackIndex(iTmc);
      }
      // If purity is low, but some hits of a given MC track are presented in the reconstructed track
      else {
        trkMc.AddTouchTrackIndex(iTre);
      }

      // Update max purity of the reconstructed track
      trkRe.SetMaxPurity(maxPurity);
    }  // loop over hit map: end
    assert(trkRe.GetNofMCTracks() < 2);

  }  // loop over reconstructed tracks: end
}


// ***********************
// **     Accessors     **
// ***********************

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::SetDetector(L1DetectorID detID, bool flag)
{
  switch (detID) {
    case L1DetectorID::kMvd: fbUseMvd = flag; break;
    case L1DetectorID::kSts: fbUseSts = flag; break;
    case L1DetectorID::kMuch: fbUseMuch = flag; break;
    case L1DetectorID::kTrd: fbUseTrd = flag; break;
    case L1DetectorID::kTof: fbUseTof = flag; break;
  }
}

// *******************************
// **     Utility functions     **
// *******************************

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::CheckInit() const
{
  // Check parameters
  if (!fpParameters.get()) { throw std::logic_error("Tracking parameters object was not defined"); }

  // Check output data containers
  if (!fpMCData) { throw std::logic_error("MC data object was not registered"); }
  if (!fpvRecoTracks) { throw std::logic_error("Reconstructed track container was not registered"); }
  if (!fpvHitIds) { throw std::logic_error("Hit index container was not registered"); }
  if (!fpvQaHits) { throw std::logic_error("QA hit container was not registered"); }
  if (!fpvFstHitId) { throw std::logic_error("Array of first hit indexes in each detector was not registered"); }

  // Check event list
  if (!fbLegacyEventMode && !fpMCEventList) { throw std::logic_error("MC event list was not found"); }
  if (!fbLegacyEventMode && !fpTimeSlice) { throw std::logic_error("Time slice was not found"); }

  // Tracks branch
  if (!fpMCTracks) { throw std::logic_error("MC tracks branch is unavailable"); }

  // Event header
  if (!fpMCEventHeader) { throw std::logic_error("MC event header is unavailable"); }

  // Check detectors initialization
  if (fbUseMvd) {
    if (!fpMvdPoints) { throw std::logic_error("MC points unavailable for MVD"); }
    if (!fpMvdHitMatches) { throw std::logic_error("Hit matches unavailable for MVD"); }
  }

  if (fbUseSts) {
    if (!fpStsPoints) { throw std::logic_error("MC points unavailable for STS"); }
    if (!fpStsHitMatches) { throw std::logic_error("Hit matches unavailable for STS"); }
    if (!fpStsClusterMatches) { throw std::logic_error("Cluster matches unavailable for STS"); }
    if (!fpStsHits) { throw std::logic_error("Hits unavailable for STS"); }
  }

  if (fbUseMuch) {
    if (!fpMuchPoints) { throw std::logic_error("MC points unavailable for MuCh"); }
    if (!fpMuchHitMatches) { throw std::logic_error("Hit matches unavailable for MuCh"); }
  }

  if (fbUseTrd) {
    if (!fpTrdPoints) { throw std::logic_error("MC points unavailable for TRD"); }
    if (!fpTrdHitMatches) { throw std::logic_error("Hit matches unavailable for TRD"); }
  }

  if (fbUseTof) {
    if (!fpTofPoints) { throw std::logic_error("MC points unavailable for TOF"); }
    if (!fpTofHitMatches) { throw std::logic_error("Hit matches unavailable for TOF"); }
  }
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<>
void CbmCaMCModule::ReadMCPointsForDetector<L1DetectorID::kTof>(CbmMCDataArray* pPoints)
{
  fvNofPointsUsed[int(L1DetectorID::kTof)] = 0;
  for (const auto& key : fFileEventIDs) {
    int iFile  = key.first;
    int iEvent = key.second;

    // Prepare TODO
    int nTofStationsGeo = fpParameters->GetNstationsGeometry(L1DetectorID::kTof);
    std::vector<std::vector<int>> vTofPointToTrack(nTofStationsGeo);      // TODO
    std::vector<std::vector<float>> vTofPointToTrackDZ(nTofStationsGeo);  // TODO
    for (int iStLocGeo = 0; iStLocGeo < nTofStationsGeo; ++iStLocGeo) {
      vTofPointToTrack[iStLocGeo].resize(fpMCData->GetNofTracks(), -1);
      vTofPointToTrackDZ[iStLocGeo].resize(fpMCData->GetNofTracks(), 1.e+5f);
    }

    // Array of flags, if a given TOF point is matched
    std::vector<char> vbTofPointMatched(pPoints->Size(iFile, iEvent), 0);

    // Check whether a particular TOF point is matched to a hit
    auto* pTofHits = dynamic_cast<TClonesArray*>(FairRootManager::Instance()->GetObject("TofHit"));
    for (int iHit = 0; iHit < pTofHits->GetEntriesFast(); ++iHit) {
      auto* pHitMatch = L1_DYNAMIC_CAST<CbmMatch*>(fpTofHitMatches->At(iHit));
      LOG_IF(fatal, !pHitMatch) << "CA MC Module: hit match was not found for TOF hit " << iHit;
      for (int iLink = 0; iLink < pHitMatch->GetNofLinks(); ++iLink) {
        const auto& link = pHitMatch->GetLink(iLink);
        if (link.GetFile() != iFile) { continue; }
        if (link.GetEntry() != iEvent) { continue; }
        int iP = link.GetIndex();
        if (iP < 0) { continue; }
        vbTofPointMatched[iP] = 1;
      }
    }

    int nPointsEvent = pPoints->Size(iFile, iEvent);
    // loop over all TOF points in event and ffvNofPointsUsed[int(L1DetectorID::kMvd)]ile
    for (int iP = 0; iP < nPointsEvent; ++iP) {
      if (!vbTofPointMatched[iP]) { continue; }  // iP was not matched

      auto* pExtPoint = L1_DYNAMIC_CAST<CbmTofPoint*>(pPoints->Get(iFile, iEvent, iP));
      LOG_IF(fatal, !pExtPoint) << "NO POINT: TOF: file, event, index = " << iFile << ' ' << iEvent << ' ' << iP;

      if (pExtPoint->GetTrackID() < 0) { continue; }  // Point does not have a track

      // Select station index for a point
      double zPos     = pExtPoint->GetZ();
      float bestDist  = 1000.;  // arbitrary large length [cm]
      int iStSelected = -1;     // local geometry index of TOF station

      for (int iStLocGeo = 0; iStLocGeo < nTofStationsGeo; ++iStLocGeo) {
        int iStActive = fpParameters->GetStationIndexActive(iStLocGeo, L1DetectorID::kTof);
        if (iStActive < 0) { continue; }  // station is not used in tracking
        const L1Station& station = fpParameters->GetStation(iStActive);
        if (fabs(zPos - station.fZ[0]) < bestDist) {
          bestDist    = fabs(zPos - station.fZ[0]);
          iStSelected = iStLocGeo;
        }
      }

      int iTrack = fpMCData->FindInternalTrackIndex(pExtPoint->GetTrackID(), iEvent, iFile);
      assert(iTrack > -1);
      if (iStSelected != -1) {
        int iStActive            = fpParameters->GetStationIndexActive(iStSelected, L1DetectorID::kTof);
        const L1Station& station = fpParameters->GetStation(iStActive);
        if (fabs(station.fZ[0] - zPos) < vTofPointToTrackDZ[iStSelected][iTrack]) {
          vTofPointToTrack[iStSelected][iTrack]   = iP;
          vTofPointToTrackDZ[iStSelected][iTrack] = fabs(station.fZ[0] - zPos);
        }
      }
    }  // loop over all TOF points in event and file: end

    for (int iStLocGeo = 0; iStLocGeo < nTofStationsGeo; ++iStLocGeo) {
      int iStActive = fpParameters->GetStationIndexActive(iStLocGeo, L1DetectorID::kTof);
      if (iStActive < 0) { continue; }
      for (int iTrk = 0; iTrk < (int) vTofPointToTrack[iStLocGeo].size(); ++iTrk) {
        if (vTofPointToTrack[iStLocGeo][iTrk] < 0) { continue; }
        ca::tools::MCPoint aPoint;
        if (FillMCPoint<L1DetectorID::kTof>(vTofPointToTrack[iStLocGeo][iTrk], iEvent, iFile, aPoint)) {
          aPoint.SetStationId(iStActive);
          aPoint.SetExternalId(CalcGlobMCPointIndex(vTofPointToTrack[iStLocGeo][iTrk], L1DetectorID::kTof));
          int iPInt = fpMCData->GetNofPoints();  // assign an index of point in internal container
          if (aPoint.GetTrackId() > -1) { fpMCData->GetTrack(aPoint.GetTrackId()).AddPointIndex(iPInt); }
          fpMCData->AddPoint(aPoint);
          ++fvNofPointsUsed[int(L1DetectorID::kTof)];
        }
      }  // loop over tracks: end
    }    // loop over geometry stations: end
  }      // loop over key: end
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::ReadMCPoints()
{
  int nPointsEstimated = 5 * fpMCData->GetNofTracks() * fpParameters->GetNstationsActive();
  fpMCData->ReserveNofPoints(nPointsEstimated);

  // ----- Initialise the number of points
  std::fill(fvNofPointsOrig.begin(), fvNofPointsOrig.end(), 0);

  for (const auto& key : fFileEventIDs) {
    int iFile  = key.first;
    int iEvent = key.second;
    if (fbUseMvd) { fvNofPointsOrig[int(L1DetectorID::kMvd)] += fpMvdPoints->Size(iFile, iEvent); }
    if (fbUseSts) { fvNofPointsOrig[int(L1DetectorID::kSts)] += fpStsPoints->Size(iFile, iEvent); }
    if (fbUseMuch) { fvNofPointsOrig[int(L1DetectorID::kMuch)] += fpMuchPoints->Size(iFile, iEvent); }
    if (fbUseTrd) { fvNofPointsOrig[int(L1DetectorID::kTrd)] += fpTrdPoints->Size(iFile, iEvent); }
    if (fbUseTof) { fvNofPointsOrig[int(L1DetectorID::kTof)] += fpTofPoints->Size(iFile, iEvent); }
  }

  // ----- Read MC points in MVD, STS, MuCh, TRD and TOF
  if (fbUseMvd) { this->ReadMCPointsForDetector<L1DetectorID::kMvd>(fpMvdPoints); }
  if (fbUseSts) { this->ReadMCPointsForDetector<L1DetectorID::kSts>(fpStsPoints); }
  if (fbUseMuch) { this->ReadMCPointsForDetector<L1DetectorID::kMuch>(fpMuchPoints); }
  if (fbUseTrd) { this->ReadMCPointsForDetector<L1DetectorID::kTrd>(fpTrdPoints); }
  if (fbUseTof) { this->ReadMCPointsForDetector<L1DetectorID::kTof>(fpTofPoints); }
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::ReadMCTracks()
{
  // ----- Total number of tracks
  int nTracksTot = 0;
  for (const auto& key : fFileEventIDs) {
    nTracksTot += fpMCTracks->Size(key.first, key.second);  /// iFile, iEvent
  }
  fpMCData->ReserveNofTracks(nTracksTot);

  // ----- Loop over MC events
  for (const auto& key : fFileEventIDs) {
    int iFile       = key.first;
    int iEvent      = key.second;
    auto pEvtHeader = dynamic_cast<FairMCEventHeader*>(fpMCEventHeader->Get(iFile, iEvent));
    if (!pEvtHeader) {
      LOG(fatal) << "CbmCaMCModule: event header is not found for file " << iFile << " and event " << iEvent;
    }
    double eventTime = fpMCEventList->GetEventTime(iEvent, iFile);

    // ----- Loop over MC tracks
    int nTracks = fpMCTracks->Size(iFile, iEvent);
    for (int iTrkExt = 0; iTrkExt < nTracks; ++iTrkExt) {
      CbmMCTrack* pExtMCTrk = dynamic_cast<CbmMCTrack*>(fpMCTracks->Get(iFile, iEvent, iTrkExt));
      if (!pExtMCTrk) {
        LOG(warn) << "CbmCaMCModule: track with (iF, iE, iT) = " << iFile << ' ' << iEvent << ' ' << iTrkExt
                  << " not found";
      }
      // Create a CbmL1MCTrack
      ca::tools::MCTrack aTrk;

      aTrk.SetId(fpMCData->GetNofTracks());  // assign current number of tracks read so far as an ID of a new track
      aTrk.SetExternalId(iTrkExt);           // external index of track is its index from CbmMCTrack objects container
      aTrk.SetEventId(iEvent);
      aTrk.SetFileId(iFile);

      aTrk.SetStartT(pExtMCTrk->GetStartT() + eventTime);
      aTrk.SetStartX(pExtMCTrk->GetStartX());
      aTrk.SetStartY(pExtMCTrk->GetStartY());
      aTrk.SetStartZ(pExtMCTrk->GetStartZ());

      aTrk.SetPx(pExtMCTrk->GetPx());
      aTrk.SetPy(pExtMCTrk->GetPy());
      aTrk.SetPz(pExtMCTrk->GetPz());

      aTrk.SetFlagSignal(aTrk.IsPrimary() && (aTrk.GetStartZ() > (pEvtHeader->GetZ() + 1.e-10)));

      // In CbmMCTrack mass is defined from ROOT PDG data base. If track is left from ion, and its pdg is not registered
      // in the data base, its mass is calculated as A times proton mass.
      aTrk.SetMass(pExtMCTrk->GetMass());

      // The charge in CbmMCTrack is similarly to mass defined from ROOT PDG data base. The value of charge there is
      // given in the units of 1/3e (absolute value of d-quark charge). In ca::tools::MCTrack we recalculate this
      // value to the units of e.
      aTrk.SetCharge(pExtMCTrk->GetCharge() / 3.);

      // Set index of mother track. We assume, that mother track was recorded into the internal array before
      int extMotherId = pExtMCTrk->GetMotherId();
      if (extMotherId < 0) {
        // This is a primary track, and it's mother ID equals -1 or -2. This value is taken also as an internal mother
        // ID.
        aTrk.SetMotherId(extMotherId);
      }
      else {
        // This is a secondary track, mother ID should be recalculated for the internal track array.
        int motherId = fpMCData->FindInternalTrackIndex(extMotherId, iEvent, iFile);
        if (motherId == -1) { motherId = -3; }  // Mother is neutral particle, which is rejected
        aTrk.SetMotherId(motherId);
      }

      aTrk.SetProcessId(pExtMCTrk->GetGeantProcessId());
      aTrk.SetPdgCode(pExtMCTrk->GetPdgCode());

      fpMCData->AddTrack(aTrk);
    }  // Loop over MC tracks: end
  }    // Loop over MC events: end
}
