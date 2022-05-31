/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackerInterfaceSts.h
 * @brief  Input data and parameters interface from STS subsystem used in L1 tracker (declaration)
 * @since  27.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrackerInterfaceSts_h
#define CbmTrackerInterfaceSts_h 1

#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsSetup.h"
#include "L1TrackerInterfaceBase.h"
#include "FairTask.h"

#include <iostream>


/// Class CbmTrackerInterfaceSts is a CbmL1 subtask, which provides necessary methods for L1 tracker
/// to access the geometry and dataflow settings.
/// 
class CbmTrackerInterfaceSts: public FairTask, public L1TrackerInterfaceBase {
public:
  /// Default constructor
  CbmTrackerInterfaceSts();

  /// Destructor
  ~CbmTrackerInterfaceSts();

  /// FairTask: Init method
  InitStatus Init();

  /// FairTask: ReInit method
  InitStatus ReInit();

  /// Gets pointer to the instance of the CbmTrackerInterfaceSts class
  static CbmTrackerInterfaceSts* Instance() { return fpInstance; }

  /// Gets actual number of stations, provided by the current geometry setup
  int GetNstations() const;

  /// Gets time resolution for a station
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Time resolution [ns]
  double GetTimeResolution(int stationId) const;

  /// Gets z component of the station position
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  double GetZ(int stationId) const;

  /// Gets max size of a station along the X-axis
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  double GetXmax(int stationId) const;
  
  /// Gets max size of a station along the Y-axis
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  double GetYmax(int stationId) const;

  /// Gets size of inner radius of station
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  double GetRmin(int stationId) const;

  /// Gets size of outer radius of station
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  double GetRmax(int stationId) const;

  /// Gets station thickness along the Z-axis
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  double GetThickness(int stationId) const;

  /// Gets station radiation length
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  double GetRadLength(int stationId) const;

  /// Gets front strips stereo angle
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  double GetStripsStereoAngleFront(int stationId) const; 

  /// Gets back strips stereo angle
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  double GetStripsStereoAngleBack(int stationId) const; 

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsFront(int stationId) const;

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  double GetStripsSpatialRmsBack(int stationId) const;

  /// Check if station provides time measurements
  /// \param  stationId  Station ID in the setup 
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  bool IsTimeInfoProvided(int stationId) const;
  
  /// FairTask: sets parameter containers up
  void SetParContainers();

  /// Copy and move constructers and assign operators are forbidden
  CbmTrackerInterfaceSts(const CbmTrackerInterfaceSts&)            = delete;
  CbmTrackerInterfaceSts(CbmTrackerInterfaceSts&&)                 = delete;
  CbmTrackerInterfaceSts& operator=(const CbmTrackerInterfaceSts&) = delete;
  CbmTrackerInterfaceSts& operator=(CbmTrackerInterfaceSts&&)      = delete;

private:
  static CbmTrackerInterfaceSts* fpInstance;

  CbmStsParSetSensor*     fStsParSetSensor     {nullptr};  ///<
  CbmStsParSetSensorCond* fStsParSetSensorCond {nullptr};  ///<
  CbmStsParSetModule*     fStsParSetModule     {nullptr};  ///<

  ClassDef(CbmTrackerInterfaceSts, 0);


};

#endif // CbmTrackerInterfaceSts
