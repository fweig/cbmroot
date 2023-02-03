/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTofTrackingInterface.h
 * @brief  Input data and parameters interface from TOF subsystem used in L1 tracker (declaration)
 * @since  23.06.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTofTrackingInterface_h
#define CbmTofTrackingInterface_h 1

#include "CbmPixelHit.h"
#include "CbmTofAddress.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofHit.h"
#include "CbmTrackingDetectorInterfaceBase.h"

#include "FairTask.h"

#include "TMath.h"

#include <iostream>
#include <vector>

class CbmTofDigiPar;

/// Class CbmTofTrackingInterface is a CbmTrackingDetetorInterfaceInit subtask, which provides necessary methods for L1 tracker
/// to access the geometry and dataflow settings.
///
class CbmTofTrackingInterface : public FairTask, public CbmTrackingDetectorInterfaceBase {
public:
  /// Default constructor
  CbmTofTrackingInterface();

  /// Destructor
  ~CbmTofTrackingInterface();

  /// FairTask: Init method
  InitStatus Init();

  /// FairTask: ReInit method
  InitStatus ReInit();

  /// Gets pointer to the instance of the CbmTofTrackingInterface
  static CbmTofTrackingInterface* Instance() { return fpInstance; }

  /// Gets name of this subsystem
  std::string GetDetectorName() const { return "TOF"; }

  /// Gets actual number of tracking stations, provided by the current geometry setup
  int GetNtrackingStations() const { return fDigiBdfPar->GetNbTrackingStations(); }

  /// Gets station radiation length
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  double GetRadLength(int /*stationId*/) const { return 2.; }

  /// Gets size of outer radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  double GetRmax(int /*stationId*/) const { return 150.; }

  /// Gets size of inner radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  double GetRmin(int /*stationId*/) const { return 0.; }

  /// Gets back strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  double GetStripsStereoAngleBack(int /*stationId*/) const { return 0.5 * TMath::Pi(); }

  /// Gets front strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  double GetStripsStereoAngleFront(int /*stationId*/) const { return 0.; }

  /// Gets station thickness along the Z-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  double GetThickness(int /*stationId*/) const { return 10.; }

  /// Gets a tracking station of a ToF hit
  /// \param  hit  A pointer to ToF hit
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(const CbmPixelHit* hit) const
  {
    int iSt = fDigiBdfPar->GetTrackingStation(CbmTofAddress::GetSmType(hit->GetAddress()),
                                              CbmTofAddress::GetSmId(hit->GetAddress()),
                                              CbmTofAddress::GetRpcId(hit->GetAddress()));

    if (fIfMissingHits) {
      // Recalculate station index for inconsistent hits
      if (hit->GetX() > 20. && hit->GetZ() > 270. && 1 == iSt) { iSt = 2; }
    }
    return iSt;
  }

  /// Gets a tracking station by the address of element
  /// \param  address  Unique element address
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(int /*address*/) const
  {
    // int iSt = fDigiBdfPar->GetTrackingStation(CbmTofAddress::GetSmType(address),
    //                                           CbmTofAddress::GetSmId(address),
    //                                           CbmTofAddress::GetRpcId(address));
    // NOTE: Implement, when the "mcbm_beam_2021_07_surveyed" parameters will be fixed
    // TODO: Invesitgate problem in mcbm_beam_2021_07_surveyed
    return -1;  // iSt;
  }


  /// Gets max size of a station along the X-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  double GetXmax(int /*stationId*/) const { return 20.; }

  /// Gets max size of a station along the Y-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  double GetYmax(int /*stationId*/) const { return 20.; }

  /// Gets z component of the station position
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  double GetZ(int stationId) const { return fTofStationZ[stationId]; }

  /// Check if station provides time measurements
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  bool IsTimeInfoProvided(int /*stationId*/) const { return true; }

  /// FairTask: sets parameter containers up
  void SetParContainers();

  /// Copy and move constructers and assign operators are prohibited
  CbmTofTrackingInterface(const CbmTofTrackingInterface&) = delete;
  CbmTofTrackingInterface(CbmTofTrackingInterface&&)      = delete;
  CbmTofTrackingInterface& operator=(const CbmTofTrackingInterface&) = delete;
  CbmTofTrackingInterface& operator=(CbmTofTrackingInterface&&) = delete;

  // This is a temporary hack for incorrect parameters at "mcbm_beam_2021_07_surveyed" setup. In the TOF parameters
  // file there is a mismatch in indeces of tracking stations of hits. At the moment,  we introduced a method, which
  // runs CbmTofTrackingInterface::ReInit and recalculates z-component positions  for this particular case. The method
  // should be invoked inside CbmL1::Init() function ONLY for the "mcbm_beam_2021_07_surveyed" setup until the
  // corresponding parameter file is not fixed.
  void FixHitsStationsMismatch();

private:
  static CbmTofTrackingInterface* fpInstance;  ///< Instance of the class

  CbmTofDigiPar* fDigiPar {nullptr};
  CbmTofDigiBdfPar* fDigiBdfPar {nullptr};

  std::vector<float> fTofStationZ {};  ///< ToF stations position z-component [cm]

  bool fIfMissingHits {false};  ///< Temporary flag, which is used to fix hits stations indexes mismatch

  ClassDef(CbmTofTrackingInterface, 0);
};

#endif  // CbmTofTrackingInterface
