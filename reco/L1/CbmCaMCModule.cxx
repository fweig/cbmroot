/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmCaMCModule.cxx
/// \brief  CA Tracking performance interface for CBM (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CbmCaMCModule.h"

#include "CbmEvent.h"
#include "CbmL1.h"  // for L1DetectorID
#include "CbmMCDataManager.h"
#include "CbmMCDataObject.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmTimeSlice.h"

#include "FairEventHeader.h"
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "Logger.h"

#include "TDatabasePDG.h"
#include "TLorentzVector.h"
#include "TVector3.h"

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

  LOG(info) << "Detector subsystems used:";
  LOG(info) << "\tMVD:  " << (fbUseMvd ? "ON" : "OFF");
  LOG(info) << "\tSTS:  " << (fbUseSts ? "ON" : "OFF");
  LOG(info) << "\tMuCh: " << (fbUseMuch ? "ON" : "OFF");
  LOG(info) << "\tTRD:  " << (fbUseTrd ? "ON" : "OFF");
  LOG(info) << "\tTOF:  " << (fbUseTof ? "ON" : "OFF");

  return true;
}
catch (const std::logic_error& error) {
  LOG(info) << "CA MC Module: initializing CA tracking Monte-Carlo module... \033[1;31mFailed\033[0m\nReason: "
            << error.what();
  return false;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::InitEvent(CbmEvent* pEvent)
{
  std::cout << "\033[1;32mCALL CbmCAMCModule::InitEvent\033[0m\n";

  // ------ Fill a set of file and event indexes
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

  // ------ Read data
  fMCData.Clear();
  this->ReadMCTracks();
  this->ReadMCPoints();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void CbmCaMCModule::Finish() { std::cout << "\033[1;32mFinishing performance\033[0m\n"; }


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
  // ----- Check parameters
  if (!fpParameters) { throw std::logic_error("Tracking parameters object was not defined"); }

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
      vTofPointToTrack[iStLocGeo].resize(fMCData.GetNofTracks(), -1);
      vTofPointToTrackDZ[iStLocGeo].resize(fMCData.GetNofTracks(), 1.e+5f);
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
    int counter      = 0;
    int counter1     = 0;
    // loop over all TOF points in event and file
    for (int iP = 0; iP < nPointsEvent; ++iP) {
      if (!vbTofPointMatched[iP]) {
        counter1++;
        continue;
      }

      auto* pExtPoint = L1_DYNAMIC_CAST<CbmTofPoint*>(pPoints->Get(iFile, iEvent, iP));
      LOG_IF(fatal, !pExtPoint) << "NO POINT: TOF: file, event, index = " << iFile << ' ' << iEvent << ' ' << iP
                                << '\n';
      if (!pExtPoint) { continue; }

      if (pExtPoint->GetTrackID() < 0) { continue; }

      // Cut on time-slice time
      if (!fbLegacyEventMode) {
        double pointT = pExtPoint->GetTime() + fpMCEventList->GetEventTime(iEvent, iFile);
        double startT = fpTimeSlice->GetStartTime();
        double endT   = fpTimeSlice->GetEndTime();
        if ((startT > 0. && pointT < startT) || (endT > 0. && pointT > endT)) { continue; }
      }

      // Select station index for a point
      double zPos     = pExtPoint->GetZ();
      float bestDist  = 1000.;  // arbitrary large length [cm]
      int iStSelected = -1;     // local geometry index of TOF station
      for (int iStLocGeo = 0; iStLocGeo < nTofStationsGeo; ++iStLocGeo) {
        int iStActive = fpParameters->GetStationIndexActive(iStLocGeo, L1DetectorID::kTof);
        if (iStActive < 0) { continue; }
        const L1Station& station = fpParameters->GetStation(iStActive);
        if (fabs(zPos - station.fZ[0]) < bestDist) {
          bestDist    = fabs(zPos - station.fZ[0]);
          iStSelected = iStLocGeo;
        }
      }

      int iTrack = fMCData.FindGlobalTrackIndex(pExtPoint->GetTrackID(), iEvent, iFile);
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
          aPoint.SetStationID(iStActive);
          int iGlobP = this->CalcGlobMCPointIndex(vTofPointToTrack[iStLocGeo][iTrk], L1DetectorID::kTof);
          fMCData.AddPoint(aPoint, iGlobP, iEvent, iFile);
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
  int nPointsEstimated = 5 * fMCData.GetNofTracks() * fpParameters->GetNstationsActive();
  fMCData.ReserveNofPoints(nPointsEstimated);

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


  // ******* DEBUG: START
  static int nCallsDBG = 0;
  if (nCallsDBG < 1) {
    auto openMode = nCallsDBG ? std::ios_base::app : std::ios_base::out;
    std::ofstream out("mc_points_dbg_new.txt", openMode);
    out << "N of points orig:\n";
    out << "\tMVD: " << fvNofPointsOrig[0] << '\n';
    out << "\tSTS: " << fvNofPointsOrig[1] << '\n';
    out << "\tMuCh: " << fvNofPointsOrig[2] << '\n';
    out << "\tTRD: " << fvNofPointsOrig[3] << '\n';
    out << "\tTOF: " << fvNofPointsOrig[4] << '\n';
    out << "N of points used:\n";
    out << "\tMVD: " << fvNofPointsUsed[0] << '\n';
    out << "\tSTS: " << fvNofPointsUsed[1] << '\n';
    out << "\tMuCh: " << fvNofPointsUsed[2] << '\n';
    out << "\tTRD: " << fvNofPointsUsed[3] << '\n';
    out << "\tTOF: " << fvNofPointsUsed[4] << '\n';
    out << "\ttotal: " << std::accumulate(fvNofPointsUsed.begin(), fvNofPointsUsed.end(), 0) << '\n';
    out << "POINTS:\n";

    out << fMCData.GetPoint(0).ToString(2, true) << '\n';
    for (const auto& point : fMCData.GetPointContainer()) {
      out << point.ToString(2) << '\n';
    }
    out.close();
    ++nCallsDBG;
  }
  // ******* DEBUG: END
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
  fMCData.ReserveNofTracks(nTracksTot);

  // ----- Loop over MC events
  for (const auto& key : fFileEventIDs) {
    int iFile       = key.first;
    int iEvent      = key.second;
    auto pEvtHeader = dynamic_cast<FairMCEventHeader*>(fpMCEventHeader->Get(iFile, iEvent));
    if (!pEvtHeader) {
      LOG(fatal) << "CbmCaMCModule: event header is not found for file " << iFile << " and event " << iEvent;
    }

    // ----- Loop over MC tracks
    int nTracks = fpMCTracks->Size(iFile, iEvent);
    for (int iTrkExt = 0; iTrkExt < nTracks; ++iTrkExt) {
      CbmMCTrack* pExtMCTrk = dynamic_cast<CbmMCTrack*>(fpMCTracks->Get(iFile, iEvent, iTrkExt));
      if (!pExtMCTrk) {
        LOG(warn) << "CbmCaMCModule: track with (iF, iE, iT) = " << iFile << ' ' << iEvent << ' ' << iTrkExt
                  << " not found";
      }
      TVector3 vtx;
      TLorentzVector mom4;
      pExtMCTrk->GetStartVertex(vtx);
      pExtMCTrk->Get4Momentum(mom4);
      int pdg         = pExtMCTrk->GetPdgCode();
      unsigned procID = pExtMCTrk->GetGeantProcessId();
      int motherID    = pExtMCTrk->GetMotherId();
      double charge   = 0.;  // in [e]
      double mass     = 0.;  // in [GeV/c2]

      // Initialize charge and mass from PDG
      auto pPdgBase = TDatabasePDG::Instance()->GetParticle(pdg);
      if (pPdgBase) {
        charge = pPdgBase->Charge() / 3.;
        mass   = pPdgBase->Mass();
      }

      // Create a CbmL1MCTrack
      int iTrkInt = fMCData.GetNofTracks();  // index of track in the MCData container
      CbmL1MCTrack aTrk(mass, charge, vtx, mom4, iTrkInt, motherID, pdg, procID);
      aTrk.time     = pExtMCTrk->GetStartT();
      aTrk.iFile    = iFile;
      aTrk.iEvent   = iEvent;
      aTrk.isSignal = aTrk.IsPrimary() && (aTrk.z > pEvtHeader->GetZ() + 1.e-10);
      fMCData.AddTrack(aTrk, iTrkExt, iEvent, iFile);
    }  // Loop over MC tracks: end
  }    // Loop over MC events: end
}
