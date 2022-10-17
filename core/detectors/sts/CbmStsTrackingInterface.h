/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmStsTrackingInterface.h
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (declaration)
 * @since  27.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmStsTrackingInterface_h
#define CbmStsTrackingInterface_h 1

#include "CbmPixelHit.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsSetup.h"
#include "CbmStsStation.h"
#include "CbmTrackingDetectorInterfaceBase.h"

#include "FairTask.h"

#include "TMath.h"

#include <iostream>


/// Class CbmStsTrackingInterface is a CbmL1 subtask, which provides necessary methods for L1 tracker
/// to access the geometry and dataflow settings.
///
class CbmStsTrackingInterface : public FairTask, public CbmTrackingDetectorInterfaceBase {
public:
  /// Default constructor
  CbmStsTrackingInterface();

  /// Destructor
  ~CbmStsTrackingInterface();

  /// FairTask: Init method
  InitStatus Init();

  /// FairTask: ReInit method
  InitStatus ReInit();

  /// Gets pointer to the instance of the CbmStsTrackingInterface class
  static CbmStsTrackingInterface* Instance() { return fpInstance; }

  /// Gets name of this subsystem
  std::string GetDetectorName() const { return "STS"; }

  /// Gets actual number of the tracking stations, provided by the current geometry setup
  int GetNtrackingStations() const { return CbmStsSetup::Instance()->GetNofStations(); }

  /// Gets station radiation length
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  double GetRadLength(int stationId) const { return GetStsStation(stationId)->GetRadLength(); }

  /// Gets size of outer radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  double GetRmax(int stationId) const
  {
    return GetXmax(stationId) > GetYmax(stationId) ? GetXmax(stationId) : GetYmax(stationId);
  }

  /// Gets size of inner radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  double GetRmin(int /*stationId*/) const { return 0.; }

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsBack(int stationId) const
  {
    return GetStsStation(stationId)->GetSensorPitch(0) / TMath::Sqrt(12.);
  }

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsFront(int stationId) const
  {
    return GetStsStation(stationId)->GetSensorPitch(0) / TMath::Sqrt(12.);
  }

  /// Gets back strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  double GetStripsStereoAngleBack(int stationId) const;

  /// Gets front strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  double GetStripsStereoAngleFront(int stationId) const;

  /// Gets station thickness along the Z-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  double GetThickness(int stationId) const { return GetStsStation(stationId)->GetSensorD(); }

  /// Gets time resolution for a station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Time resolution [ns]
  double GetTimeResolution(int /*stationId*/) const { return 5.; }

  /// Gets a tracking station of a CbmPixelHit
  /// \param  hit  A pointer to CbmPixelHit
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(const CbmPixelHit* hit) const { return GetTrackingStationIndex(hit->GetAddress()); }

  /// Gets a tracking station by the address of element
  /// \param  address  Unique element address
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(int address) const { return CbmStsSetup::Instance()->GetStationNumber(address); }

  /// Gets max size of a station along the X-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  double GetXmax(int stationId) const { return GetStsStation(stationId)->GetXmax(); }

  /// Gets max size of a station along the Y-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  double GetYmax(int stationId) const { return GetStsStation(stationId)->GetYmax(); }

  /// Gets z component of the station position
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  double GetZ(int stationId) const { return GetStsStation(stationId)->GetZ(); }

  /// Check if station provides time measurements
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  bool IsTimeInfoProvided(int /*stationId*/) const { return true; }

  /// FairTask: sets parameter containers up
  void SetParContainers();

  /// Copy and move constructors and assign operators are forbidden
  CbmStsTrackingInterface(const CbmStsTrackingInterface&) = delete;
  CbmStsTrackingInterface(CbmStsTrackingInterface&&)      = delete;
  CbmStsTrackingInterface& operator=(const CbmStsTrackingInterface&) = delete;
  CbmStsTrackingInterface& operator=(CbmStsTrackingInterface&&) = delete;

private:
  /// Gets pointer to the STS station object
  /// \param  stationId  Tracking staton ID
  /// \return Pointer to the particular CbmStsStation object
  __attribute__((always_inline)) CbmStsStation* GetStsStation(int stationId) const
  {
    return CbmStsSetup::Instance()->GetStation(stationId);
  }

  static CbmStsTrackingInterface* fpInstance;

  CbmStsParSetSensor* fStsParSetSensor {nullptr};          ///<
  CbmStsParSetSensorCond* fStsParSetSensorCond {nullptr};  ///<
  CbmStsParSetModule* fStsParSetModule {nullptr};          ///<

  ClassDef(CbmStsTrackingInterface, 0);
};

#endif  // CbmStsTrackingInterface
