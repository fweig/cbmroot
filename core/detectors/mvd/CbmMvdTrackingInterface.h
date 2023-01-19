/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMvdTrackingInterface.h
 * @brief  Input data and parameters interface from MVD subsystem used in L1 tracker (declaration)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmMvdTrackingInterface_h
#define CbmMvdTrackingInterface_h 1

#include "CbmMvdDetectorId.h"                  // for CbmMvdDetectorId
#include "CbmMvdHit.h"                         // for CbmMvdHit
#include "CbmMvdStationPar.h"                  // for CbmMvdStationPar
#include "CbmPixelHit.h"                       // for CbmPixelHit
#include "CbmTrackingDetectorInterfaceBase.h"  // for CbmTrackingDetectorInt...

#include <FairTask.h>                          // for InitStatus, FairTask

#include <Rtypes.h>                            // for ClassDef
#include <TMath.h>                             // for Pi

#include <algorithm>                           // for min, max
#include <string>                              // for string

class TBuffer;
class TClass;
class TMemberInspector;

/// Class CbmMvdTrackingInterface is a CbmL1 subtask, which provides necessary methods for L1 tracker
/// to access the geometry and dataflow settings.
///
class CbmMvdTrackingInterface : public FairTask, public CbmTrackingDetectorInterfaceBase, public CbmMvdDetectorId {
public:
  /// Default constructor
  CbmMvdTrackingInterface();

  /// Destructor
  ~CbmMvdTrackingInterface();

  /// FairTask: Init method
  InitStatus Init();

  /// FairTask: ReInit method
  InitStatus ReInit();

  /// Gets pointer to the instance of the CbmMvdTrackingInterface
  static CbmMvdTrackingInterface* Instance() { return fpInstance; }

  /// Gets name of this subsystem
  std::string GetDetectorName() const { return "MVD"; }

  /// Gets actual number of tracking stations, provided by the current geometry setup
  int GetNtrackingStations() const { return fMvdStationPar->GetStationCount(); }

  /// Gets the tracking station radiation length
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  double GetRadLength(int stationId) const
  {
    return fMvdStationPar->GetZThickness(stationId) / (10. * fMvdStationPar->GetZRadThickness(stationId));
  }

  /// Gets size of outer radius of a tracking station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  double GetRmax(int stationId) const
  {
    return std::max(fMvdStationPar->GetHeight(stationId), fMvdStationPar->GetWidth(stationId));
  }

  /// Gets size of inner radius of a tracking station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  double GetRmin(int stationId) const
  {
    return std::min(fMvdStationPar->GetBeamHeight(stationId), fMvdStationPar->GetBeamWidth(stationId));
  }

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsBack(int stationId) const { return fMvdStationPar->GetYRes(stationId) / 10000.; }

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsFront(int stationId) const { return fMvdStationPar->GetXRes(stationId) / 10000.; }

  /// Gets back strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  double GetStripsStereoAngleBack(int /*stationId*/) const { return TMath::Pi() / 2.; }

  /// Gets front strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  double GetStripsStereoAngleFront(int /*stationId*/) const { return 0.; }

  /// Gets the tracking station thickness along the Z-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  double GetThickness(int stationId) const { return fMvdStationPar->GetZThickness(stationId); }

  /// Gets a tracking station of a CbmPixelHit
  /// \param  hit  A pointer to CbmPixelHit
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(const CbmPixelHit* hit) const
  {
    auto hitMvd = [&] {
      if constexpr (kUseDynamicCast) { return dynamic_cast<const CbmMvdHit*>(hit); }
      else {
        return static_cast<const CbmMvdHit*>(hit);
      }
    }();
    return hitMvd->GetStationNr();
  }

  /// Gets a tracking station by the address of element (detectorID in terms of MVD)
  /// \param  detectorId  Unique element address (detectorID in terms of MVD)
  /// \return Local index of the tracking station
  int GetTrackingStationIndex(int detectorId) const { return StationNr(detectorId); }

  /// Gets max size of a tracking station along the X-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  double GetXmax(int stationId) const { return this->GetRmax(stationId); }

  /// Gets max size of a tracking station along the Y-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  double GetYmax(int stationId) const { return this->GetRmax(stationId); }

  /// Gets z component of the tracking station position
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  double GetZ(int stationId) const { return fMvdStationPar->GetZPosition(stationId); }

  /// Check if the detector provides time measurements
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  bool IsTimeInfoProvided(int /*stationId*/) const { return false; }

  /// FairTask: sets parameter containers up
  void SetParContainers();

  /// Copy and move constructers and assign operators are prohibited
  CbmMvdTrackingInterface(const CbmMvdTrackingInterface&) = delete;
  CbmMvdTrackingInterface(CbmMvdTrackingInterface&&)      = delete;
  CbmMvdTrackingInterface& operator=(const CbmMvdTrackingInterface&) = delete;
  CbmMvdTrackingInterface& operator=(CbmMvdTrackingInterface&&) = delete;

private:
  static CbmMvdTrackingInterface* fpInstance;  ///< Instance of the class

  const CbmMvdStationPar* fMvdStationPar {nullptr};  ///< Pointer to the Mvd station parameters

  ClassDef(CbmMvdTrackingInterface, 0);
};

#endif  // CbmMvdTrackingInterface
