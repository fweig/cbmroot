/* Copyright (C) 2022-2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaMCModule.h
/// @brief  CA Tracking performance interface for CBM (header)
/// @since  23.09.2022
/// @author S.Zharko <s.zharko@gsi.de>

#ifndef CbmCaMCModule_h
#define CbmCaMCModule_h 1

#include "CbmL1DetectorID.h"
#include "CbmL1Hit.h"
#include "CbmL1Track.h"
#include "CbmLink.h"
#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMuchPoint.h"
#include "CbmMvdPoint.h"
#include "CbmStsPoint.h"
#include "CbmTimeSlice.h"
#include "CbmTofPoint.h"
#include "CbmTrdPoint.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"

#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "CaToolsMCData.h"
#include "CaToolsMCPoint.h"
#include "L1Constants.h"
#include "L1InputData.h"
#include "L1Parameters.h"
#include "L1Undef.h"

class CbmEvent;
class CbmMCDataObject;
class CbmL1HitDebugInfo;
class CbmL1Track;

enum class L1DetectorID;

namespace cbm::ca
{
  /// @brief Class CbmCaPerformance is an interface to communicate between
  ///
  class MCModule {


  public:
    /// @brief Constructor
    /// @param verbosity  Verbosity level
    /// @param perfMode   Performance mode (defines cut on number of consecutive stations with hit or point)
    MCModule(int verb = 0, int perfMode = 3) : fVerbose(verb), fPerformanceMode(perfMode) {}

    /// @brief Destructor
    ~MCModule() = default;

    /// @brief Copy constructor
    MCModule(const MCModule&) = delete;

    /// @brief Move constructor
    MCModule(MCModule&&) = delete;

    /// @brief Copy assignment operator
    MCModule& operator=(const MCModule&) = delete;

    /// @brief Move assignment operator
    MCModule& operator=(MCModule&&) = delete;


    /// @brief Defines performance action in the end of the run
    void Finish();

    /// @brief Gets a pointer to MC data object
    const ::ca::tools::MCData* GetMCData() const { return fpMCData; }

    /// @brief Defines performance action in the beginning of each event or time slice
    /// @note This function should be called before hits initialization
    /// @param  pEvent Pointer to a current CbmEvent
    void InitEvent(CbmEvent* pEvent);

    /// @brief Defines action on the module in the beginning of the run
    /// @return Success flag
    bool InitRun();

    /// @brief Matches hit with MC point
    /// @tparam  DetId Detector ID
    /// @param   iHitExt  External index of hit
    /// @return           MC-point index in fvMCPoints array
    ///
    /// This method finds a match for a given hit or matches for hits clusters (in case of STS), finds the best
    /// link in the match and returns the corresponding global ID of the MC points.
    template<L1DetectorID DetId>
    int MatchHitWithMc(int iHitExt) const;

    /// @brief Match reconstructed and MC data
    ///
    /// Runs matching of hits with points and reconstructed tracks with MC ones. Reconstructed tracks are updated with
    /// true information.
    void MatchRecoAndMC();

    /// @brief Processes event
    ///
    /// Fills histograms and tables, should be called after the tracking done
    void ProcessEvent(CbmEvent* pEvent);

    /// @brief Sets first hit indexes container in different detectors
    /// @param source Array of indexes
    void RegisterFirstHitIndexes(const std::array<int, L1Constants::size::kMaxNdetectors + 1>& source)
    {
      fpvFstHitId = &source;
    }

    /// @brief Sets used detector subsystems
    /// @param  detID  Id of detector
    /// @param  flag   Flag: true - detector is used
    /// @note Should be called before this->Init()
    void SetDetector(L1DetectorID detID, bool flag) { fvbUseDet[detID] = flag; }

    /// @brief Sets legacy event mode:
    /// @param flag Flag:
    ///              - true:  runs on events base,
    ///              - false: runs on time-slice base
    void SetLegacyEventMode(bool flag) { fbLegacyEventMode = flag; }

    /// @brief Registers MC data object
    /// @param mcData  Instance of MC data
    void RegisterMCData(::ca::tools::MCData& mcData) { fpMCData = &mcData; }

    /// @brief Registers reconstructed track container
    /// @param vRecoTrack Reference to reconstructed track container
    void RegisterRecoTrackContainer(L1Vector<CbmL1Track>& vRecoTracks) { fpvRecoTracks = &vRecoTracks; }

    /// @brief Registers hit index container
    /// @param vHitIds  Reference to hit index container
    void RegisterHitIndexContainer(L1Vector<CbmL1HitId>& vHitIds) { fpvHitIds = &vHitIds; }

    /// @brief Registers CA parameters object
    /// @param pParameters  A shared pointer to the parameters object
    void RegisterParameters(std::shared_ptr<L1Parameters>& pParameters) { fpParameters = pParameters; }

    /// @brief Registers debug hit container
    /// @param vQaHits  Reference to debug hit container
    void RegisterQaHitContainer(L1Vector<CbmL1HitDebugInfo>& vQaHits) { fpvQaHits = &vQaHits; }

  private:
    /// @brief Check class initialization
    /// @note The function throws std::logic_error, if initialization is incomplete
    void CheckInit() const;

    /// @brief  Initializes MC track
    ///
    /// Initializes information about arrangement of points and hits of MC tracks within stations and the status
    /// of track ability to be reconstructed, calculates max number of points and hits on a station, number of
    /// consecutive stations containing a hit or point and number of stations and points with hits.
    void InitTrackInfo();

    /// @brief  Match sets of MC points and reconstructed hits for a given detector
    /// @tparam  DetID  Index of the detector
    ///
    /// Writes indexes of MC points to each hit and indexes of hits to each MC point.
    template<L1DetectorID DetID>
    void MatchPointsAndHits();

    /// @brief Matches reconstructed tracks with MC tracks
    ///
    /// In the procedure, the maps of associated MC track indexes to corresponding number of hits are filled out and the
    /// max purity is calculated for each reconstructed track in the TS/event. If the value of purity for a given MC track
    /// is larger then a threshold, the corresponding track index is saved to the reconstructed track object, in the same
    /// time the index of the reconstructed track object is saved to this MC track object. If purity for the MC track does
    /// not pass the threshold, its index will not be accounted as a matched track, and the index of reconstructed track
    /// will be added as an index of "touched" track.
    void MatchRecoAndMCTracks();

    /// @brief Reads MC tracks from external trees and saves them to MCDataObject
    void ReadMCTracks();

    /// @brief Reads MC points from external trees and saves them to MCDataObject
    void ReadMCPoints();

    /// @brief Reads MC points in particular detector
    template<L1DetectorID DetID>
    void ReadMCPointsForDetector();

    /// @brief Fills a single detector-specific MC point
    /// @tparam     DetID      Detector subsystem ID
    /// @param[in]  iExtId     Index of point in the external points container
    /// @param[in]  iEvent     EventID of point in the external points container
    /// @param[in]  iFile      FileID of point int the external points container
    /// @param[out] intMCPoint Reference to the internal tracking MC point object (ca::tools::MCData)
    /// @return true   Point is correct and is to be saved to the MCData object
    /// @return false  Point is incorrect and will be ignored
    template<L1DetectorID DetID>
    std::optional<::ca::tools::MCPoint> FillMCPoint(int iExtId, int iEvent, int iFile);

    // ------ Flags
    CbmCaDetIdArr_t<bool> fvbUseDet = {{false}};  ///< Flag: is detector subsystem used
    bool fbLegacyEventMode          = false;  ///< if tracking uses events instead of time-slices (back compatibility)
    int fVerbose                    = 0;      ///< Verbosity level
    int fPerformanceMode            = -1;     ///< Mode of performance

    std::shared_ptr<L1Parameters> fpParameters = nullptr;  ///< Pointer to tracking parameters object
    // ------ Input data branches
    const CbmTimeSlice* fpTimeSlice  = nullptr;  ///< Current time slice
    CbmMCEventList* fpMCEventList    = nullptr;  ///< MC event list
    CbmMCDataObject* fpMCEventHeader = nullptr;  ///< MC event header
    CbmMCDataArray* fpMCTracks       = nullptr;  ///< MC tracks input

    CbmCaDetIdArr_t<CbmMCDataArray*> fvpBrPoints   = {{nullptr}};  ///< Array of points vs. detector
    CbmCaDetIdArr_t<TClonesArray*> fvpBrHitMatches = {{nullptr}};  ///< Array of hit match branches vs. detector

    TClonesArray* fpStsHits           = nullptr;  ///< Array of STS hits (currently needed for matching)
    TClonesArray* fpStsClusterMatches = nullptr;  ///< Array of STS cluster matches

    // Matching information
    std::set<std::pair<int, int>> fFileEventIDs;  ///< Set of file and event indexes: first - iFile, second - iEvent

    // ----- Internal MC data
    ::ca::tools::MCData* fpMCData = nullptr;  ///< MC information (hits and tracks) instance

    // ----- Internal reconstructed data
    L1Vector<CbmL1Track>* fpvRecoTracks    = nullptr;  ///< Pointer to reconstructed track container
    L1Vector<CbmL1HitId>* fpvHitIds        = nullptr;  ///< Pointer to hit index container
    L1Vector<CbmL1HitDebugInfo>* fpvQaHits = nullptr;  ///< Pointer to QA hit container


    /// @brief Pointer to array of first hit indexes in the detector subsystem
    ///
    /// This array must be initialized in the run initialization function.
    const std::array<int, L1Constants::size::kMaxNdetectors + 1>* fpvFstHitId = nullptr;
  };
}  // namespace cbm::ca


// **********************************************
// **     Template function implementation     **
// **********************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<L1DetectorID DetID>
std::optional<::ca::tools::MCPoint> cbm::ca::MCModule::FillMCPoint(int iExtId, int iEvent, int iFile)
{
  auto oPoint = std::make_optional<::ca::tools::MCPoint>();

  // ----- Get positions, momenta, time and track ID
  TVector3 posIn;             // Position at entrance to station [cm]
  TVector3 posOut;            // Position at exist of station [cm]
  TVector3 momIn;             // 3-momentum at entrance to station [GeV/c]
  TVector3 momOut;            // 3-momentum at exit of station [GeV/c]
  double time = undef::kD;    // Time of MC point (after beginning of the event) [ns]
  int iTmcExt = undef::kI32;  // Track ID in the external container

  auto* pBrPoints = fvpBrPoints[DetID];

  if constexpr (L1DetectorID::kMvd == DetID) {
    auto* pExtPoint = dynamic_cast<CbmMvdPoint*>(pBrPoints->Get(iFile, iEvent, iExtId));
    if (!pExtPoint) {
      LOG(warn) << "CbmCaMCModule: MVD MC point with iExtId = " << iExtId << ", iEvent = " << iEvent
                << ", iFile = " << iFile << " does not exist";
      return std::nullopt;
    }
    pExtPoint->Position(posIn);
    pExtPoint->PositionOut(posOut);
    pExtPoint->Momentum(momIn);
    pExtPoint->MomentumOut(momOut);
    time    = pExtPoint->GetTime();
    iTmcExt = pExtPoint->GetTrackID();
  }
  // STS
  else if constexpr (L1DetectorID::kSts == DetID) {
    auto* pExtPoint = dynamic_cast<CbmStsPoint*>(pBrPoints->Get(iFile, iEvent, iExtId));
    if (!pExtPoint) {
      LOG(warn) << "CbmCaMCModule: STS MC point with iExtId = " << iExtId << ", iEvent = " << iEvent
                << ", iFile = " << iFile << " does not exist";
      return std::nullopt;
    }
    pExtPoint->Position(posIn);
    pExtPoint->PositionOut(posOut);
    pExtPoint->Momentum(momIn);
    pExtPoint->MomentumOut(momOut);
    time    = pExtPoint->GetTime();
    iTmcExt = pExtPoint->GetTrackID();
  }
  // MuCh
  else if constexpr (L1DetectorID::kMuch == DetID) {
    auto* pExtPoint = dynamic_cast<CbmMuchPoint*>(pBrPoints->Get(iFile, iEvent, iExtId));
    if (!pExtPoint) {
      LOG(warn) << "CbmCaMCModule: MuCh MC point with iExtId = " << iExtId << ", iEvent = " << iEvent
                << ", iFile = " << iFile << " does not exist";
      return std::nullopt;
    }
    pExtPoint->Position(posIn);
    pExtPoint->PositionOut(posOut);
    pExtPoint->Momentum(momIn);
    pExtPoint->Momentum(momOut);
    time    = pExtPoint->GetTime();
    iTmcExt = pExtPoint->GetTrackID();
  }
  // TRD
  else if constexpr (L1DetectorID::kTrd == DetID) {
    auto* pExtPoint = dynamic_cast<CbmTrdPoint*>(pBrPoints->Get(iFile, iEvent, iExtId));
    if (!pExtPoint) {
      LOG(warn) << "CbmCaMCModule: TRD MC point with iExtId = " << iExtId << ", iEvent = " << iEvent
                << ", iFile = " << iFile << " does not exist";
      return std::nullopt;
    }
    pExtPoint->Position(posIn);
    pExtPoint->PositionOut(posOut);
    pExtPoint->Momentum(momIn);
    pExtPoint->MomentumOut(momOut);
    time    = pExtPoint->GetTime();
    iTmcExt = pExtPoint->GetTrackID();
  }
  // TOF
  else if constexpr (L1DetectorID::kTof == DetID) {
    auto* pExtPoint = dynamic_cast<CbmTofPoint*>(pBrPoints->Get(iFile, iEvent, iExtId));
    if (!pExtPoint) {
      LOG(warn) << "CbmCaMCModule: TOF MC point with iExtId = " << iExtId << ", iEvent = " << iEvent
                << ", iFile = " << iFile << " does not exist";
      return std::nullopt;
    }
    pExtPoint->Position(posIn);
    pExtPoint->Position(posOut);
    pExtPoint->Momentum(momIn);
    pExtPoint->Momentum(momOut);
    time    = pExtPoint->GetTime();
    iTmcExt = pExtPoint->GetTrackID();
  }
  if (iTmcExt < 0) {
    LOG(warn) << "CbmCaMCModule: For MC point with iExtId = " << iExtId << ", iEvent = " << iEvent
              << ", iFile = " << iFile << " MC track is undefined (has ID = " << iTmcExt << ')';
    return std::nullopt;
  }
  TVector3 posMid = 0.5 * (posIn + posOut);
  TVector3 momMid = 0.5 * (momIn + momOut);

  // // ----- Get station index
  int stationID    = undef::kI32;  // Index of station in the array of active tracking stations
  int nStationsGeo = fpParameters->GetNstationsGeometry(DetID);
  // MVD, STS TODO: Try to extend for MuCh and TRD
  if constexpr (L1DetectorID::kMvd == DetID || L1DetectorID::kSts == DetID) {
    // Determine active station global index
    double bestDist = 1.e+20;
    for (int iStLocGeo = 0; iStLocGeo < nStationsGeo; ++iStLocGeo) {
      int iStActive = fpParameters->GetStationIndexActive(iStLocGeo, DetID);
      if (iStActive < 0) { continue; }
      const L1Station& station = fpParameters->GetStation(iStActive);
      double dist              = posIn.Z() - station.fZ[0];
      if (fabs(dist) < fabs(bestDist)) {
        bestDist  = dist;
        stationID = iStActive;
      }
    }
  }
  // MuCh, TRD
  else if constexpr (L1DetectorID::kMuch == DetID || L1DetectorID::kTrd == DetID) {
    // Offset for MuCh - 2.5 cm, for TRD - 20. cm. TODO: Where did these values came from?
    double offset = (L1DetectorID::kMuch == DetID) ? 2.5 : 20.;
    for (int iStLocGeo = 0; iStLocGeo < nStationsGeo; ++iStLocGeo) {
      int iStActive = fpParameters->GetStationIndexActive(iStLocGeo, DetID);
      if (iStActive < 0) { continue; }
      const L1Station& station = fpParameters->GetStation(iStActive);
      if (posMid.Z() > station.fZ[0] - offset) { stationID = iStActive; }
    }
  }

  // Update point time with event time
  time += fpMCEventList->GetEventTime(iEvent, iFile);

  // ----- Reject MC points falling out of the time slice
  // STS, MuCh, TRD, TOF
  if constexpr (DetID != L1DetectorID::kMvd) {
    double startT = fpTimeSlice->GetStartTime();
    double endT   = fpTimeSlice->GetEndTime();

    if ((startT > 0. && time < startT) || (endT > 0. && time > endT)) {
      LOG(warn) << "CbmCaMCModule: MC point with iExtId = " << iExtId << ", iEvent = " << iEvent
                << ", iFile = " << iFile << " and det id " << int(DetID) << " fell out of the TS duration [" << startT
                << ", " << endT << "] with measured time = " << time << " [ns]";
      return std::nullopt;
    }
  }

  // ----- Fill MC point
  oPoint->SetExternalId(fpMCData->GetPointGlobExtIndex(DetID, iExtId));
  oPoint->SetEventId(iEvent);
  oPoint->SetFileId(iFile);
  oPoint->SetTime(time);
  oPoint->SetX(posMid.X());
  oPoint->SetY(posMid.Y());
  oPoint->SetZ(posMid.Z());
  oPoint->SetXIn(posIn.X());
  oPoint->SetYIn(posIn.Y());
  oPoint->SetZIn(posIn.Z());
  oPoint->SetXOut(posOut.X());
  oPoint->SetYOut(posOut.Y());
  oPoint->SetZOut(posOut.Z());
  oPoint->SetPx(momMid.X());
  oPoint->SetPy(momMid.Y());
  oPoint->SetPz(momMid.Z());
  oPoint->SetPxIn(momIn.X());
  oPoint->SetPyIn(momIn.Y());
  oPoint->SetPzIn(momIn.Z());
  oPoint->SetPxOut(momOut.X());
  oPoint->SetPyOut(momOut.Y());
  oPoint->SetPzOut(momOut.Z());

  // Select current number of points as a local id of points
  oPoint->SetId(fpMCData->GetNofPoints());

  // Match MC track and point to each other
  int iTmcInt = fpMCData->FindInternalTrackIndex(iTmcExt, iEvent, iFile);

  oPoint->SetTrackId(iTmcInt);
  if (iTmcInt > -1) { fpMCData->GetTrack(iTmcInt).AddPointIndex(oPoint->GetId()); }

  oPoint->SetStationId(stationID);
  oPoint->SetDetectorId(DetID);

  auto* pExtTrk = L1_DYNAMIC_CAST<CbmMCTrack*>(fpMCTracks->Get(iFile, iEvent, iTmcExt));
  if (!pExtTrk) {
    LOG(warn) << "CbmCaMCModule: MC track with iTmcExt = " << iTmcExt << ", iEvent = " << iEvent
              << ", iFile = " << iFile << " MC track is undefined (nullptr)";
    return std::nullopt;
  }
  oPoint->SetPdgCode(pExtTrk->GetPdgCode());
  oPoint->SetMotherId(pExtTrk->GetMotherId());

  auto* pPdgDB = TDatabasePDG::Instance()->GetParticle(oPoint->GetPdgCode());
  oPoint->SetMass(pPdgDB ? pPdgDB->Mass() : 0.);  /// TODO: Set from track
  oPoint->SetCharge(pPdgDB ? pPdgDB->Charge() / 3. : 0.);

  return oPoint;
}

// ---------------------------------------------------------------------------------------------------------------------
//
template<L1DetectorID DetID>
void cbm::ca::MCModule::MatchPointsAndHits()
{
  if (!fvbUseDet[DetID]) { return; }

  for (int iH = (*fpvFstHitId)[static_cast<int>(DetID)]; iH < (*fpvFstHitId)[static_cast<int>(DetID) + 1]; ++iH) {
    auto& hit = (*fpvQaHits)[iH];
    int iP    = MatchHitWithMc<DetID>(hit.ExtIndex);
    if (iP > -1) {
      hit.SetMCPointIndex(iP);
      fpMCData->GetPoint(iP).AddHitID(iH);
    }
  }
}
// ---------------------------------------------------------------------------------------------------------------------
// NOTE: template is used, because another template function FillMCPoint is used inside
template<L1DetectorID DetID>
void cbm::ca::MCModule::ReadMCPointsForDetector()
{
  for (const auto& [iFile, iEvent] : fFileEventIDs) {
    int nPointsEvent = fvpBrPoints[DetID]->Size(iFile, iEvent);
    for (int iP = 0; iP < nPointsEvent; ++iP) {
      std::optional<::ca::tools::MCPoint> oPoint = FillMCPoint<DetID>(iP, iEvent, iFile);
      if (oPoint) { fpMCData->AddPoint(*oPoint); }
    }  // iP: end
  }    // key: end
}


#endif  // CbmCaMCModule_h
