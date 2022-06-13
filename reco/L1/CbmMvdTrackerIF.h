/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmMvdTrackerIF.h
 * @brief  Input data and parameters interface from MVD subsystem used in L1 tracker (declaration)
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmMvdTrackerIF_h
#define CbmMvdTrackerIF_h 1

#include "L1TrackerInterfaceBase.h"
#include "FairTask.h"

#include <iostream>
#include <vector>


class CbmMvdDetector;
class CbmMvdStationPar;

/// Class CbmMvdTrackerIF is a CbmL1 subtask, which provides necessary methods for L1 tracker
/// to access the geometry and dataflow settings.
/// 
class CbmMvdTrackerIF: public FairTask, public L1TrackerInterfaceBase {
public:
  /// Default constructor
  CbmMvdTrackerIF();

  /// Destructor
  ~CbmMvdTrackerIF();

  /// FairTask: Init method
  InitStatus Init();

  /// FairTask: ReInit method
  InitStatus ReInit();

  /// Gets pointer to the instance of the CbmMvdTrackerIF
  static CbmMvdTrackerIF* Instance() { return fpInstance; }

  /// Gets actual number of tracking stations, provided by the current geometry setup
  int GetNtrackingStations() const;

  /// Gets time resolution for a tracking station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Time resolution [ns]
  double GetTimeResolution(int stationId) const;

  /// Gets z component of the tracking station position
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  double GetZ(int stationId) const;

  /// Gets max size of a tracking station along the X-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  double GetXmax(int stationId) const;
  
  /// Gets max size of a tracking station along the Y-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  double GetYmax(int stationId) const;

  /// Gets size of inner radius of a tracking station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  double GetRmin(int stationId) const;

  /// Gets size of outer radius of a tracking station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  double GetRmax(int stationId) const;

  /// Gets the tracking station thickness along the Z-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  double GetThickness(int stationId) const;

  /// Gets the tracking station radiation length
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  double GetRadLength(int stationId) const;

  /// Gets front strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  double GetStripsStereoAngleFront(int stationId) const; 

  /// Gets back strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  double GetStripsStereoAngleBack(int stationId) const; 

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsFront(int stationId) const;

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsBack(int stationId) const;

  /// Check if the detector provides time measurements
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  bool IsTimeInfoProvided(int stationId) const;
  
  /// FairTask: sets parameter containers up
  void SetParContainers();

  /// Copy and move constructers and assign operators are prohibited
  CbmMvdTrackerIF(const CbmMvdTrackerIF&)            = delete;
  CbmMvdTrackerIF(CbmMvdTrackerIF&&)                 = delete;
  CbmMvdTrackerIF& operator=(const CbmMvdTrackerIF&) = delete;
  CbmMvdTrackerIF& operator=(CbmMvdTrackerIF&&)      = delete;

private:
  static CbmMvdTrackerIF* fpInstance;  ///< Instance of the class

  const CbmMvdStationPar* fMvdStationPar {nullptr};  ///< Pointer to the Mvd station parameters 

  ClassDef(CbmMvdTrackerIF, 0);
};

#endif // CbmMvdTrackerIF
