/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMuchTrackingInterface.h
 * @brief  Input data and parameters interface from MuCh subsystem used in L1 tracker (declaration)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmMuchTrackingInterface_h
#define CbmMuchTrackingInterface_h 1

#include "CbmMuchGeoScheme.h"
#include "CbmMuchModuleGem.h"
#include "CbmMuchPad.h"
#include "CbmMuchStation.h"
#include "CbmPixelHit.h"
#include "CbmTrackingDetectorInterfaceBase.h"

#include "FairTask.h"

#include "TMath.h"
#include "TString.h"

#include <iostream>
#include <vector>

class CbmMuchLayer;

/// Class CbmMuchTrackingInterface is a CbmL1 subtask, which provides necessary methods for L1 tracker
/// to access the geometry and dataflow settings.
///
/// NOTE: For MuCh one tracking station is a MuCh layer!
///
class CbmMuchTrackingInterface : public FairTask, public CbmTrackingDetectorInterfaceBase {
public:
  /// Default constructor
  CbmMuchTrackingInterface();

  /// Destructor
  ~CbmMuchTrackingInterface();

  /// FairTask: Init method
  InitStatus Init();

  /// FairTask: ReInit method
  InitStatus ReInit();

  /// Gets pointer to the instance of the CbmMuchTrackingInterface
  static CbmMuchTrackingInterface* Instance() { return fpInstance; }

  /// Gets name of this subsystem
  std::string GetDetectorName() const { return "MuCh"; }

  /// Gets actual number of tracking stations, provided by the current geometry setup
  int GetNtrackingStations() const;

  /// Gets station radiation length
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  double GetRadLength(int /*stationId*/) const { return 0; }  // TODO: Replace with correct value

  /// Gets size of outer radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  double GetRmax(int /*stationId*/) const { return 100.; }

  /// Gets size of inner radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  double GetRmin(int /*stationId*/) const { return 10.; }

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsBack(int /*stationId*/) const { return 0.35; }

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsFront(int /*stationId*/) const { return 0.35; }

  /// Gets back strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  double GetStripsStereoAngleBack(int /*stationId*/) const { return TMath::Pi() / 2.; }

  /// Gets front strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  double GetStripsStereoAngleFront(int /*stationId*/) const { return 0.; }

  /// Gets station thickness along the Z-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  double GetThickness(int stationId) const { return GetMuchLayer(stationId)->GetDz(); };

  /// Gets time resolution for a station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Time resolution [ns]
  double GetTimeResolution(int /*stationId*/) const { return 3.9; }

  /// Gets a tracking station of a CbmPixelHit
  /// \param  hit  A pointer to CbmPixelHit
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(const CbmPixelHit* hit) const { return GetTrackingStationIndex(hit->GetAddress()); }

  /// Gets a tracking station by the address of element
  /// \param  address  Unique element address
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(int address) const
  {
    return CbmMuchGeoScheme::GetStationIndex(address) * 3 + CbmMuchGeoScheme::GetLayerIndex(address);
  }

  /// Gets max size of a station along the X-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  double GetXmax(int /*stationId*/) const { return 100.; }

  /// Gets max size of a station along the Y-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  double GetYmax(int /*stationId*/) const { return 100.; }

  /// Gets z component of the station position
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  double GetZ(int stationId) const { return GetMuchLayer(stationId)->GetZ(); }

  /// Check if station provides time measurements
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  bool IsTimeInfoProvided(int /*stationId*/) const { return true; }

  /// FairTask: sets parameter containers up
  void SetParContainers();

  /// Copy and move constructers and assign operators are prohibited
  CbmMuchTrackingInterface(const CbmMuchTrackingInterface&) = delete;
  CbmMuchTrackingInterface(CbmMuchTrackingInterface&&)      = delete;
  CbmMuchTrackingInterface& operator=(const CbmMuchTrackingInterface&) = delete;
  CbmMuchTrackingInterface& operator=(CbmMuchTrackingInterface&&) = delete;

private:
  /// Gets pointer to the TRD module
  /// \param  stationId  Tracking staton ID
  /// \return Pointer to the particular CbmTrdParModDigi object
  __attribute__((always_inline)) CbmMuchLayer* GetMuchLayer(int stationId) const
  {
    return fGeoScheme->GetLayer(GetMuchStationId(stationId), GetMuchLayerId(stationId));
  }

  /// Calculates MuCh layer ID from tracker station ID
  __attribute__((always_inline)) int GetMuchLayerId(int stationId) const { return stationId % 3; }

  /// Calculates MuCh station ID from tracker station ID
  __attribute__((always_inline)) int GetMuchStationId(int stationId) const { return stationId / 3; }


  static CbmMuchTrackingInterface* fpInstance;  ///< Instance of the class

  CbmMuchGeoScheme* fGeoScheme {nullptr};  ///< MuCh geometry scheme instance

  ClassDef(CbmMuchTrackingInterface, 0);
};

#endif  // CbmMuchTrackingInterface
