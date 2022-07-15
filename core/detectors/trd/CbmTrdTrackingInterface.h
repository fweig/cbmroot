/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrdTrackingInterface.h
 * @brief  Input data and parameters interface from TRD subsystem used in L1 tracker (declaration)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrdTrackingInterface_h
#define CbmTrdTrackingInterface_h 1

#include "CbmPixelHit.h"
#include "CbmTrackingDetectorInterfaceBase.h"
#include "CbmTrdAddress.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParSetDigi.h"

#include "FairTask.h"

#include "TMath.h"

#include <iostream>
#include <vector>

/// Class CbmTrdTrackingInterface is a CbmL1 subtask, which provides necessary methods for L1 tracker
/// to access the geometry and dataflow settings.
///
/// NOTE: For TRD one tracking station is a TRD module!
///
class CbmTrdTrackingInterface : public FairTask, public CbmTrackingDetectorInterfaceBase {
public:
  /// Default constructor
  CbmTrdTrackingInterface();

  /// Destructor
  ~CbmTrdTrackingInterface();

  /// FairTask: Init method
  InitStatus Init();

  /// FairTask: ReInit method
  InitStatus ReInit();

  /// Gets pointer to the instance of the CbmTrdTrackingInterface
  static CbmTrdTrackingInterface* Instance() { return fpInstance; }

  /// Gets actual number of tracking stations, provided by the current geometry setup
  int GetNtrackingStations() const;

  /// Gets station radiation length
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  double GetRadLength(int /*stationId*/) const { return 1.6; }

  /// Gets size of outer radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  double GetRmax(int stationId) const { return 2. * this->GetXmax(stationId); }

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

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsBack(int /*stationId*/) const { return 0.15; }

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsFront(int /*stationId*/) const { return 0.15; }

  /// Gets station thickness along the Z-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  double GetThickness(int stationId) const { return 2. * GetTrdModulePar(stationId)->GetSizeZ(); }

  /// Gets time resolution for a station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Time resolution [ns]
  double GetTimeResolution(int /*stationId*/) const { return 10.; }

  /// Gets a tracking station of a CbmPixelHit
  /// \param  hit  A pointer to CbmPixelHit
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(const CbmPixelHit* hit) const { return GetTrackingStationIndex(hit->GetAddress()); }

  /// Gets a tracking station by the address
  /// \param  address  Unique element address
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(int address) const
  {
    return CbmTrdAddress::GetLayerId(address);
  }

  /// Gets max size of a station along the X-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  double GetXmax(int stationId) const { return GetTrdModulePar(stationId)->GetSizeX(); }

  /// Gets max size of a station along the Y-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  double GetYmax(int stationId) const { return GetTrdModulePar(stationId)->GetSizeY(); }

  /// Gets z component of the station position
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  double GetZ(int stationId) const { return GetTrdModulePar(stationId)->GetZ(); }

  /// Check if station provides time measurements
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  bool IsTimeInfoProvided(int /*stationId*/) const { return true; }

  /// FairTask: sets parameter containers up
  void SetParContainers();

  /// Copy and move constructers and assign operators are prohibited
  CbmTrdTrackingInterface(const CbmTrdTrackingInterface&) = delete;
  CbmTrdTrackingInterface(CbmTrdTrackingInterface&&)      = delete;
  CbmTrdTrackingInterface& operator=(const CbmTrdTrackingInterface&) = delete;
  CbmTrdTrackingInterface& operator=(CbmTrdTrackingInterface&&) = delete;

private:
  /// Gets pointer to the TRD module
  /// \param  moduleId  Id of the Trd module
  /// \return Pointer to the particular CbmTrdParModDigi object
  __attribute__((always_inline)) CbmTrdParModDigi* GetTrdModulePar(int moduleId) const
  {
    return static_cast<CbmTrdParModDigi*>(fTrdDigiPar->GetModulePar(fTrdDigiPar->GetModuleId(moduleId)));
  }

  static CbmTrdTrackingInterface* fpInstance;  ///< Instance of the class

  CbmTrdParSetDigi* fTrdDigiPar {nullptr};
  //CbmTrdParModDigi* fTrdModuleInfo {nullptr};

  ClassDef(CbmTrdTrackingInterface, 0);
};

#endif  // CbmTrdTrackingInterface
