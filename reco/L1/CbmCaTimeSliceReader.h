/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTimeSliceReader.h
/// @brief  Time-slice/event reader for CA tracker in CBM (header)
/// @since  24.02.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#ifndef CbmCaTimeSliceReader_h
#define CbmCaTimeSliceReader_h 1

#include "CbmCaMCModule.h"
#include "CbmL1.h"  // TMP: for CbmL1HitStore
#include "CbmL1DetectorID.h"
#include "CbmL1Hit.h"
#include "CbmL1Track.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchTrackingInterface.h"
#include "CbmMvdHit.h"
#include "CbmMvdTrackingInterface.h"
#include "CbmPixelHit.h"
#include "CbmStsHit.h"
#include "CbmStsTrackingInterface.h"
#include "CbmTofHit.h"
#include "CbmTofTrackingInterface.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrackingInterface.h"

#include "TClonesArray.h"

#include "CaToolsHitRecord.h"
#include "L1Constants.h"
#include "L1Vector.h"


class CbmTimeSlice;
class L1IODataManager;
class L1Parameters;

namespace cbm::ca
{
  using ::ca::tools::HitRecord;


  /// @brief A reader of time slice for CA tracker
  ///
  /// The class reads reconstructed hits and reconstructed tracks (optionally) and fills the CA tracking internal
  /// data structures.
  ///
  class TimeSliceReader {
  public:
    /// @brief Constructor from parameters
    /// @param mode  Tracking mode
    TimeSliceReader() = default;

    /// @brief Destructor
    ~TimeSliceReader() = default;

    /// @brief Copy constructor
    TimeSliceReader(const TimeSliceReader&) = delete;

    /// @brief Move constructor
    TimeSliceReader(TimeSliceReader&&) = delete;

    /// @brief Copy assignment operator
    TimeSliceReader& operator=(const TimeSliceReader&) = delete;

    /// @brief Move assignment operator
    TimeSliceReader& operator=(TimeSliceReader&&) = delete;

    /// @brief Clears class content
    void Clear();

    /// @brief Gets reference to container of first hit indexes in a detector subsystem
    /// @return Ref. to the container
    const auto& GetHitFirstIndexDet() const { return fvHitFirstIndexDet; }

    /// @brief Gets number of hits stored for a given detector
    /// @param iDet  Detector ID
    /// @return Number of hits
    int GetNofHits(L1DetectorID iDet) const
    {
      return fvHitFirstIndexDet[int(iDet) + 1] - fvHitFirstIndexDet[int(iDet)];
    }

    /// @brief Gets CBM tracking mode
    ECbmCaTrackingMode GetTrackingMode() const { return fTrackingMode; }

    /// @brief Run initializer function
    /// @return Success flag
    ///
    /// Initializes data branches and provides necessary checks in the beginning of the run
    bool InitRun();

    /// @brief Reads time slice
    ///
    /// Reads hits and tracks (optionally) from time slice
    void InitTimeSlice();

    /// @brief Registers hit debug info container
    /// @param vQaHits  Reference to Qa hit container
    /// @note If no container is registered, all related routines are omitted
    void RegisterQaHitContainer(L1Vector<CbmL1HitDebugInfo>& vQaHits) { fpvQaHits = &vQaHits; }

    /// @brief Registers hit index container
    /// @param vHitIds  Reference to hits indexes container
    /// @note If no container is registered, all related routines are omitted
    void RegisterHitIndexContainer(L1Vector<CbmL1HitId>& vHitIds) { fpvHitIds = &vHitIds; }

    /// @brief Registers CA parameters object
    /// @param pParameters  A shared pointer to the parameters object
    void RegisterParameters(std::shared_ptr<L1Parameters>& pParameters) { fpParameters = pParameters; }

    /// @brief Registers the CA IO data manager instance
    /// @param pIODataManager  Shared pointer to the IO data manager instance
    /// @note If no container is registered, all related routines are omitted
    void RegisterIODataManager(std::shared_ptr<L1IODataManager>& ioDataManager);

    /// @brief Register the reconstructed tracks container
    /// @param vTracks  Reference to reconstructed tracks container
    /// @note If no container is registered, all related routines are omitted
    void RegisterTracksContainer(L1Vector<CbmL1Track>& vTracks) { fpvTracks = &vTracks; }

    /// @brief Sets used detector subsystems
    /// @param  detID  Id of detector
    /// @param  flag   Flag: true - detector is used
    /// @note Should be called before this->Init()
    void SetDetector(L1DetectorID detID, bool flag = true) { fvbUseDet[detID] = flag; }

    /// @brief  Sets the tracking mode
    /// @param  mode Tracking mode (from ECbmTrackingMode)
    void SetTrackingMode(ECbmCaTrackingMode mode) { fTrackingMode = mode; }


  private:
    /// @brief Check class initialization
    /// @note The function throws std::logic_error, if initialization is incomplete
    void CheckInit() const;

    /// @brief Reads hits
    void ReadHits();

    /// @brief Reads reconstructed tracks
    void ReadRecoTracks();

    /// @brief Reads hits for a given detector subsystem
    /// @tparam Detector ID
    /// @return  Number of stored hits
    /// @note The function modifies fNofHitKey and fFirstHitKey counters
    template<L1DetectorID DetID>
    int ReadHitsForDetector();

    /// @brief Sorts QA hit objects by stations
    void SortQaHits();

    /// @brief Saves hit to data structures
    /// @param hitRecord  Filled hit record
    ///
    /// Stores recorded hit information into registered hit containers
    void StoreHitRecord(const HitRecord& hitRecord);

    bool fbReadTracks = true;   ///< flag to read reconstructed tracks from reco.root

    /// @brief Pointers to the tracking detector interfaces for each subsystem
    CbmCaDetIdArr_t<const CbmTrackingDetectorInterfaceBase*> fvpDetInterface = {{nullptr}};

    // Input data branches
    CbmTimeSlice* fpBrTimeSlice              = nullptr;      ///< Pointer to the TS object
    CbmCaDetIdArr_t<TClonesArray*> fvpBrHits = {{nullptr}};  ///< Input branch for hits

    // Branches for reconstructed tracks. The input at the moment (as for 27.02.2023) depends on the selected
    // tracking mode. For simulations in CBM, the CA tracking is used only in STS + MVD detectors. In this case
    // the reconstructed tracks are saved to the "StsTrack" branch as CbmStsTrack objects. For mCBM, the tracks from
    // CA are saved as global tracks, and the local ones are used to keep indexes of hits in different subsystems
    TClonesArray* fpBrRecoTracks = nullptr;  ///< Input branch for reconstructed tracks ("GlobalTrack", "StsTrack")
    TClonesArray* fpBrStsTracks  = nullptr;  ///< Input branch for reconstructed STS tracks ("StsTrack")
    TClonesArray* fpBrMuchTracks = nullptr;  ///< Input branch for reconstructed MuCh tracks ("MuchTrack")
    TClonesArray* fpBrTrdTracks  = nullptr;  ///< Input branch for reconstructed TRD tracks ("TrdTrack")
    TClonesArray* fpBrTofTracks  = nullptr;  ///< Input branch for reconstructed TOF tracks ("TofTrack")

    // Pointers to output data containers
    L1Vector<CbmL1HitId>* fpvHitIds                  = nullptr;  ///< Pointer to array of hit index objects
    L1Vector<CbmL1HitDebugInfo>* fpvQaHits           = nullptr;  ///< Pointer to array of debug hits
    L1Vector<CbmL1Track>* fpvTracks                  = nullptr;  ///< Pointer to array of reconstructed tracks
    std::shared_ptr<L1IODataManager> fpIODataManager = nullptr;  ///< Pointer to input data manager
    std::shared_ptr<L1Parameters> fpParameters       = nullptr;  ///< Pointer to tracking parameters object

    // Maps of hit indexes: ext -> int
    CbmCaDetIdArr_t<L1Vector<int>> fvvHitExtToIntIndexMap;  ///< Hit index map ext -> int

    CbmCaDetIdArr_t<int> fvNofHitsTotal = {{0}};      ///< Total hit number in detector
    CbmCaDetIdArr_t<int> fvNofHitsUsed  = {{0}};      ///< Number of used hits in detector
    CbmCaDetIdArr_t<bool> fvbUseDet     = {{false}};  ///< Flag: is detector subsystem used

    ECbmCaTrackingMode fTrackingMode = ECbmCaTrackingMode::kSTS;  ///< Tracking mode

    // Variables for storing cache
    int fNofHits     = 0;  ///< Stored number of hits
    int fNofHitKeys  = 0;  ///< Recorded number of hit keys
    int fFirstHitKey = 0;  ///< First index of hit key for the detector subsystem

    std::array<int, L1Constants::size::kMaxNdetectors + 1> fvHitFirstIndexDet = {{0}};  ///< First hit index in detector
  };
}  // namespace cbm::ca


// **************************************************
// ** Inline and template function implementations **
// **************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<L1DetectorID DetID>
int cbm::ca::TimeSliceReader::ReadHitsForDetector()
{
  if (!fvbUseDet[DetID]) { return 0; }  // Detector is entirelly not used

  const auto* pDetInterface = fvpDetInterface[DetID];
  int nHitsTot              = fvpBrHits[DetID]->GetEntriesFast();  // total number of hits provided by hit finder
  int nHitsStored           = 0;                                   // number of hits used in tracking

  fFirstHitKey = fNofHitKeys;

  for (int iHext = 0; iHext < nHitsTot; ++iHext) {
    HitRecord hitRecord;

    CbmPixelHit* pPixelHit = nullptr;  // Pointer to hit object
    int iStGeom            = -1;       // Geometry station number

    // Fill out detector specific data
    if constexpr (L1DetectorID::kMvd == DetID) {
      CbmMvdHit* pMvdHit = static_cast<CbmMvdHit*>(fvpBrHits[DetID]->At(iHext));
      pPixelHit          = static_cast<CbmPixelHit*>(pMvdHit);
      iStGeom            = pDetInterface->GetTrackingStationIndex(pMvdHit->GetStationNr());
    }
    else if constexpr (L1DetectorID::kSts == DetID) {
      CbmStsHit* pStsHit = static_cast<CbmStsHit*>(fvpBrHits[DetID]->At(iHext));
      pPixelHit          = static_cast<CbmPixelHit*>(pStsHit);
      iStGeom            = pDetInterface->GetTrackingStationIndex(pStsHit->GetAddress());
      hitRecord.fStripF  = fFirstHitKey + pStsHit->GetFrontClusterId();
      hitRecord.fStripB  = fFirstHitKey + pStsHit->GetBackClusterId();
      hitRecord.fDu      = pStsHit->GetDu();
      hitRecord.fDv      = pStsHit->GetDv();
    }
    else if constexpr (L1DetectorID::kMuch == DetID) {
      CbmMuchPixelHit* pMuchHit = static_cast<CbmMuchPixelHit*>(fvpBrHits[DetID]->At(iHext));
      pPixelHit                 = static_cast<CbmPixelHit*>(pMuchHit);
      iStGeom                   = pDetInterface->GetTrackingStationIndex(pMuchHit->GetAddress());
    }
    else if constexpr (L1DetectorID::kTrd == DetID) {
      CbmTrdHit* pTrdHit = static_cast<CbmTrdHit*>(fvpBrHits[DetID]->At(iHext));
      pPixelHit          = static_cast<CbmPixelHit*>(pTrdHit);
      iStGeom            = pDetInterface->GetTrackingStationIndex(pTrdHit->GetAddress());
    }
    else if constexpr (L1DetectorID::kTof == DetID) {
      CbmTofHit* pTofHit = static_cast<CbmTofHit*>(fvpBrHits[DetID]->At(iHext));
      pPixelHit          = static_cast<CbmPixelHit*>(pTofHit);
      // NOTE: In TOF we can take station index only from hit, because the function needs information on x and z
      //       of the hit in case of "beam_mcbm_2021_07_surveyed" (missingHits flag = true).
      // TODO: Investigate this case or apply the hack to the TOF level
      iStGeom = pDetInterface->GetTrackingStationIndex(pTofHit);

      // *** Additional cuts for TOF ***
      // Skip T0 hits
      if (5 == CbmTofAddress::GetSmType(pTofHit->GetAddress())) { continue; }

      // FIXME: Figure it out, if this cut is still needed (introduced a year ago for mCBM)
      if (ECbmCaTrackingMode::kMCBM == fTrackingMode && pTofHit->GetZ() > 400) { continue; }
    }

    int iStActive = fpParameters->GetStationIndexActive(iStGeom, DetID);
    if (iStActive == -1) { continue; }  // Cut off inactive stations

    // Fill out data common for all the detectors
    hitRecord.fStaId      = iStActive;
    hitRecord.fX          = pPixelHit->GetX();
    hitRecord.fY          = pPixelHit->GetY();
    hitRecord.fZ          = pPixelHit->GetZ();
    hitRecord.fDx         = pPixelHit->GetDx();
    hitRecord.fDy         = pPixelHit->GetDy();
    hitRecord.fDxy        = pPixelHit->GetDxy();
    hitRecord.fT          = pPixelHit->GetTime();
    hitRecord.fDt         = pPixelHit->GetTimeError();
    hitRecord.fDet        = static_cast<int>(DetID);
    hitRecord.fDataStream = (static_cast<int64_t>(hitRecord.fDet) << 60) | pPixelHit->GetAddress();
    float phiF            = pDetInterface->GetStripsStereoAngleFront(iStGeom);
    float phiB            = pDetInterface->GetStripsStereoAngleBack(iStGeom);
    hitRecord.fU          = hitRecord.fX * cos(phiF) + hitRecord.fY * sin(phiF);
    hitRecord.fV          = hitRecord.fX * cos(phiB) + hitRecord.fY * sin(phiB);
    hitRecord.fExtId      = iHext;

    // Update number of hit keys
    if constexpr (L1DetectorID::kSts == DetID) {
      if (fNofHitKeys <= hitRecord.fStripF) { fNofHitKeys += hitRecord.fStripF; }
      if (fNofHitKeys <= hitRecord.fStripB) { fNofHitKeys += hitRecord.fStripB; }
    }
    else {
      hitRecord.fDu     = hitRecord.fDx;
      hitRecord.fDv     = hitRecord.fDy;
      hitRecord.fStripF = fFirstHitKey + iHext;
      hitRecord.fStripB = hitRecord.fStripF;
      if (fNofHitKeys <= hitRecord.fStripF) { fNofHitKeys += hitRecord.fStripF; }
    }

    // Save hit to data structures
    this->StoreHitRecord(hitRecord);
    ++nHitsStored;
  }  // iH

  return nHitsStored;
}

#endif  // CbmCaTimeSliceReader_h
