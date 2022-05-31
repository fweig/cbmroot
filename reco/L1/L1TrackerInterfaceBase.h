/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   L1TrackerInterfaceBase.h
 * @brief  Base abstract class for tracking detector interface to L1
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef L1TrackerInterfaceBase_h
#define L1TrackerInterfaceBase_h 1

/// Abstract class, which should be inherited by every detecting subsystem interface class
///
class L1TrackerInterfaceBase {
public:
  /// Virtual destructor
  virtual ~L1TrackerInterfaceBase() {}

  /// Gets actual number of stations, provided by the current geometry setup
  virtual int GetNstations() const = 0;

  /// Gets time resolution for a station
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Time resolution [ns]
  virtual double GetTimeResolution(int stationId) const = 0;

  /// Gets z component of the station position
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  virtual double GetZ(int stationId) const = 0;

  /// Gets max size of a station along the X-axis
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  virtual double GetXmax(int stationId) const = 0;
  
  /// Gets max size of a station along the Y-axis
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  virtual double GetYmax(int stationId) const = 0;

  /// Gets size of inner radius of station
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  virtual double GetRmin(int stationId) const = 0;

  /// Gets size of outer radius of station
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  virtual double GetRmax(int stationId) const = 0;

  /// Gets station thickness along the Z-axis
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  virtual double GetThickness(int stationId) const = 0;

  /// Gets station radiation length
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  virtual double GetRadLength(int stationId) const = 0;

  /// Gets front strips stereo angle
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  virtual double GetStripsStereoAngleFront(int stationId) const = 0; 

  /// Gets back strips stereo angle
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  virtual double GetStripsStereoAngleBack(int stationId) const = 0; 

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  virtual double GetStripsSpatialRmsFront(int stationId) const = 0;

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  virtual double GetStripsSpatialRmsBack(int stationId) const = 0;

  /// Check if station provides time measurements
  /// \param  stationId  Station ID in the setup 
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  virtual bool IsTimeInfoProvided(int stationId) const = 0;
};

#endif // L1TrackerInterfaceBase_h
