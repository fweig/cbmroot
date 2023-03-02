/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmCaTimeSliceReader.h
/// @brief  Time-slice/event reader for CA tracker in CBM (header)
/// @since  24.02.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#ifndef CbmCaTimeSliceReader_h
#define CbmCaTimeSliceReader_h 1

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

#include "L1Vector.h"


class CbmTimeSlice;
class L1IODataManager;
class L1Parameters;

namespace cbm::ca
{
  /// @brief A reader of time slice for CA tracker
  ///
  /// The class reads reconstructed hits and reconstructed tracks (optionally) and fills the CA tracking internal
  /// data structures.
  ///
  class TimeSliceReader {
    /// @brief A structure to store hits information from different detectors in a uniform manner
    ///
    struct HitRecord {
      double fX           = 0.;  ///< x component of hit position [cm]
      double fY           = 0.;  ///< y component of hit position [cm]
      double fDx          = 0.;  ///< error of x component of hit position [cm]
      double fDy          = 0.;  ///< error of y component of hit position [cm]
      double fDxy         = 0.;  ///< correlation between x and y components [cm]
      double fU           = 0.;  ///< hit position in direction of front strips [cm]
      double fV           = 0.;  ///< hit position in direction of back strips [cm]
      double fDu          = 0.;  ///< hit position error in direction of front strips [cm]
      double fDv          = 0.;  ///< hit position error in direction of back strips [cm]
      double fZ           = 0.;  ///< z component of hit position [cm]
      double fT           = 0.;  ///< time of hit [ns]
      double fDt          = 0.;  ///< time error of hit [ns]
      int64_t fDataStream = -1;  ///< Global index of detector module
      int fExtId          = -1;  ///< external index of hit
      int fStaId          = -1;  ///< index of active tracking station
      int fStripF         = -1;  ///< index of front strip
      int fStripB         = -1;  ///< index of back strip
      int fDet            = -1;  ///< detector ID
    };

  public:
    /// @brief Constructor from parameters
    /// @param mode  Tracking mode
    TimeSliceReader(ECbmTrackingMode mode);

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

    /// @brief Check class initialization
    /// @note The function throws std::logic_error, if initialization is incomplete
    void CheckInit() const;

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

    /// @brief Run initializer function
    /// @return Success flag
    ///
    /// Initializes data branches and provides necessary checks in the beginning of the run
    bool InitRun();

    /// @brief Reads time slice
    ///
    /// Reads hits and tracks (optionally) from time slice
    void InitTimeSlice();

    /// @brief Reads hits
    void ReadHits();

    /// @brief Reads reconstructed tracks
    void ReadRecoTracks();

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
    void SetDetector(L1DetectorID detID, bool flag = true);

  private:
    /// @brief Reads hits for a given detector subsystem
    /// @tparam Detector ID
    /// @param  pBrHits  Pointer to input hit branch
    /// @return  Number of stored hits
    /// @note The function modifies fNofHitKey and fFirstHitKey counters
    template<L1DetectorID DetID>
    int ReadHitsForDetector(const TClonesArray* pBrHits);

    /// @brief Sorts QA hit objects by stations
    void SortQaHits();

    /// @brief Saves hit to data structures
    /// @param hitRecord  Filled hit record
    ///
    /// Stores recorded hit information into registered hit containers
    void StoreHitRecord(const HitRecord& hitRecord);

    // Flags for detector subsystems being used
    bool fbUseMvd     = false;  ///< is MVD used
    bool fbUseSts     = false;  ///< is STS used
    bool fbUseMuch    = false;  ///< is MuCh used
    bool fbUseTrd     = false;  ///< is TRD used
    bool fbUseTof     = false;  ///< is TOF used
    bool fbReadTracks = true;   ///< flag to read reconstructed tracks from reco.root

    // Pointers to the tracking detector interfaces
    CbmTrackingDetectorInterfaceBase* fpMvdInterface  = nullptr;
    CbmTrackingDetectorInterfaceBase* fpStsInterface  = nullptr;
    CbmTrackingDetectorInterfaceBase* fpMuchInterface = nullptr;
    CbmTrackingDetectorInterfaceBase* fpTrdInterface  = nullptr;
    CbmTrackingDetectorInterfaceBase* fpTofInterface  = nullptr;

    // Input data branches
    CbmTimeSlice* fpBrTimeSlice = nullptr;  ///< Pointer to the TS object

    TClonesArray* fpBrMvdHits  = nullptr;  ///< Input branch for MVD hits ("MvdHit")
    TClonesArray* fpBrStsHits  = nullptr;  ///< Input branch for STS hits ("StsHit")
    TClonesArray* fpBrMuchHits = nullptr;  ///< Input branch for MuCh hits ("MuchPixelHit")
    TClonesArray* fpBrTrdHits  = nullptr;  ///< Input branch for TRD hits ("TrdHit")
    TClonesArray* fpBrTofHits  = nullptr;  ///< Input branch for TOF hits ("TofHit")

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
    L1Vector<int> vHitMvdIds {"CbmCaTimeSliceReader::vHitMvdIds"};
    L1Vector<int> vHitStsIds {"CbmCaTimeSliceReader::vHitStsIds"};
    L1Vector<int> vHitMuchIds {"CbmCaTimeSliceReader::vHitMuchIds"};
    L1Vector<int> vHitTrdIds {"CbmCaTimeSliceReader::vHitTrdIds"};
    L1Vector<int> vHitTofIds {"CbmCaTimeSliceReader::vHitTofIds"};

    // Additional
    ECbmTrackingMode fTrackingMode;  ///< Tracking mode

    // Variables for storing cache
    int fNofHits     = 0;  ///< Stored number of hits
    int fNofHitKeys  = 0;  ///< Recorded number of hit keys
    int fFirstHitKey = 0;  ///< First index of hit key for the detector subsystem

    std::array<int, L1Constants::size::kMaxNdetectors + 1> fvHitFirstIndexDet = {
      0};  ///< First index of hit in detector
  };
}  // namespace cbm::ca


// **************************************************
// ** Inline and template function implementations **
// **************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<L1DetectorID DetID>
int cbm::ca::TimeSliceReader::ReadHitsForDetector(const TClonesArray* pBrHits)
{
  int nHitsTot    = pBrHits->GetEntriesFast();  // total number of hits stored in a branch
  int nHitsStored = 0;                          // number of hits stored

  fFirstHitKey = fNofHitKeys;

  for (int iH = 0; iH < nHitsTot; ++iH) {
    HitRecord hitRecord;  // Record of hits information

    CbmPixelHit* pPixelHit = nullptr;  // Pointer to hit object
    float phiF             = 0.;       // Stereo angle of front strips
    float phiB             = 0.;       // Stereo angle of back strips
    int iStGeom            = -1;       // Geometry station number

    // Fill out detector specific data
    if constexpr (L1DetectorID::kMvd == DetID) {
      CbmMvdHit* pMvdHit = static_cast<CbmMvdHit*>(pBrHits->At(iH));
      iStGeom            = fpMvdInterface->GetTrackingStationIndex(pMvdHit->GetStationNr());
      phiF               = fpMvdInterface->GetStripsStereoAngleFront(iStGeom);
      phiB               = fpMvdInterface->GetStripsStereoAngleBack(iStGeom);
      pPixelHit          = static_cast<CbmPixelHit*>(pMvdHit);
      hitRecord.fDu      = pMvdHit->GetDx();
      hitRecord.fDv      = pMvdHit->GetDy();
    }
    else if constexpr (L1DetectorID::kSts == DetID) {
      CbmStsHit* pStsHit = static_cast<CbmStsHit*>(pBrHits->At(iH));
      iStGeom            = fpStsInterface->GetTrackingStationIndex(pStsHit->GetAddress());
      phiF               = fpStsInterface->GetStripsStereoAngleFront(iStGeom);
      phiB               = fpStsInterface->GetStripsStereoAngleBack(iStGeom);
      pPixelHit          = static_cast<CbmPixelHit*>(pStsHit);
      hitRecord.fStripF  = fFirstHitKey + pStsHit->GetFrontClusterId();
      hitRecord.fStripB  = fFirstHitKey + pStsHit->GetBackClusterId();
      hitRecord.fDu      = pStsHit->GetDu();
      hitRecord.fDv      = pStsHit->GetDv();
    }
    else if constexpr (L1DetectorID::kMuch == DetID) {
      CbmMuchPixelHit* pMuchHit = static_cast<CbmMuchPixelHit*>(pBrHits->At(iH));
      iStGeom                   = fpMuchInterface->GetTrackingStationIndex(pMuchHit->GetAddress());
      phiF                      = fpMuchInterface->GetStripsStereoAngleFront(iStGeom);
      phiB                      = fpMuchInterface->GetStripsStereoAngleBack(iStGeom);
      pPixelHit                 = static_cast<CbmPixelHit*>(pMuchHit);
      hitRecord.fDu             = pMuchHit->GetDx();
      hitRecord.fDv             = pMuchHit->GetDy();
    }
    else if constexpr (L1DetectorID::kTrd == DetID) {
      CbmTrdHit* pTrdHit = static_cast<CbmTrdHit*>(pBrHits->At(iH));
      iStGeom            = fpTrdInterface->GetTrackingStationIndex(pTrdHit->GetAddress());
      phiF               = fpTrdInterface->GetStripsStereoAngleFront(iStGeom);
      phiB               = fpTrdInterface->GetStripsStereoAngleBack(iStGeom);
      pPixelHit          = static_cast<CbmPixelHit*>(pTrdHit);
      hitRecord.fDu      = pTrdHit->GetDx();
      hitRecord.fDv      = pTrdHit->GetDy();
    }
    else if constexpr (L1DetectorID::kTof == DetID) {
      CbmTofHit* pTofHit = static_cast<CbmTofHit*>(pBrHits->At(iH));
      iStGeom            = fpTofInterface->GetTrackingStationIndex(pTofHit->GetAddress());
      phiF               = fpTofInterface->GetStripsStereoAngleFront(iStGeom);
      phiB               = fpTofInterface->GetStripsStereoAngleBack(iStGeom);
      pPixelHit          = static_cast<CbmPixelHit*>(pTofHit);
      hitRecord.fDu      = pTofHit->GetDx();
      hitRecord.fDv      = pTofHit->GetDy();

      // *** Additional cuts for TOF ***
      // Skip T0 hits
      if (5 == CbmTofAddress::GetSmType(pTofHit->GetAddress())) { continue; }

      // FIXME: Figure it out, if this cut is still needed (introduced a year ago for mCBM)
      if (ECbmTrackingMode::kMCBM == fTrackingMode && pTofHit->GetZ() > 400) { continue; }
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
    hitRecord.fU          = hitRecord.fX * cos(phiF) + hitRecord.fY * sin(phiF);
    hitRecord.fV          = hitRecord.fX * cos(phiB) + hitRecord.fY * sin(phiB);
    hitRecord.fExtId      = iH;

    // Update number of hit keys
    if constexpr (L1DetectorID::kSts == DetID) {
      if (fNofHitKeys <= hitRecord.fStripF) { fNofHitKeys += hitRecord.fStripF; }
      if (fNofHitKeys <= hitRecord.fStripB) { fNofHitKeys += hitRecord.fStripB; }
    }
    else {
      hitRecord.fStripF = fFirstHitKey + iH;
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
