/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/***************************************************************************************************
 * @file   CbmTrackingDetectorInterfaceBase.h
 * @brief  Base abstract class for tracking detector interface to L1
 * @since  31.05.2022
 * @author S.Zharko <s.zharko@gsi.de>
 ***************************************************************************************************/

#ifndef CbmTrackingDetectorInterfaceBase_h
#define CbmTrackingDetectorInterfaceBase_h 1

#include <string>

class CbmPixelHit;

/// Abstract class, which should be inherited by every detecting subsystem tracking interface class
///
class CbmTrackingDetectorInterfaceBase {
public:
  /// Virtual destructor
  virtual ~CbmTrackingDetectorInterfaceBase() {}

  /// Returns the name of the detector subsystem
  virtual std::string GetDetectorName() const = 0;

  /// Gets actual number of stations, provided by the current geometry setup
  virtual int GetNtrackingStations() const = 0;

  // TODO: SZh 17.10.2022: At the moment the radiation length and the station thickness are not used for the tracking
  //                       initialization as soon as the material budget maps are in use. Should we keep these
  //                       accessors here, or just remove them from the interfaces?

  /// Gets station radiation length
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Radiation length [cm]
  virtual double GetRadLength(int stationId) const = 0;

  /// Gets size of inner radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station inner radius [cm]
  virtual double GetRmin(int stationId) const = 0;

  /// Gets size of outer radius of station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station outer radius [cm]
  virtual double GetRmax(int stationId) const = 0;

  /// Gets front strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for front strips [rad]
  virtual double GetStripsStereoAngleFront(int stationId) const = 0;

  /// Gets back strips stereo angle
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Absolute stereo angle for back strips [rad]
  virtual double GetStripsStereoAngleBack(int stationId) const = 0;

  /// Gets spatial resolution (RMS) for front strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  virtual double GetStripsSpatialRmsFront(int stationId) const = 0;

  /// Gets spatial resolution (RMS) for back strips
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Spatial resolution (RMS) for front strips [cm]
  virtual double GetStripsSpatialRmsBack(int stationId) const = 0;

  /// Gets station thickness along the Z-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Station thickness [cm]
  virtual double GetThickness(int stationId) const = 0;

  /// Gets time resolution for a station
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Time resolution [ns]
  virtual double GetTimeResolution(int stationId) const = 0;

  /// Gets a tracking station of a CbmPixelHit
  /// \param  hit  A pointer to CbmPixelHit
  /// \return Local index of the tracking station
  virtual int GetTrackingStationIndex(const CbmPixelHit* hit) const = 0;

  /// Gets a tracking station by the address of element
  /// \param  address  Unique element address
  /// \return Local index of the tracking station
  virtual int GetTrackingStationIndex(int address) const = 0;

  /// Gets max size of a station along the X-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the X-axis [cm]
  virtual double GetXmax(int stationId) const = 0;

  /// Gets max size of a station along the Y-axis
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Size of station along the Y-axis [cm]
  virtual double GetYmax(int stationId) const = 0;

  /// Gets z component of the station position
  /// \param  stationId  Tracking station ID in the setup (NOTE: must be in range [0..GetNstations()-1])
  /// \return Z position of station [cm]
  virtual double GetZ(int stationId) const = 0;

  /// Check if station provides time measurements
  /// \param  stationId  Tracking station ID in the setup
  /// \return Flag: true - station provides time measurements, false - station does not provide time measurements
  virtual bool IsTimeInfoProvided(int stationId) const = 0;

  /// Technical flag: true - all downcasts are done with dynamic_cast followed by checks for nullptr (increases
  //  computing time, better for debug), false - all downcasts are done with static_cast without sanity checks
  //  (decreases computting time, but can cause bugs)

  static constexpr bool kUseDynamicCast {true};


  /// Checks detector interface: boundary conditions of the parameters
  bool Check() const;
};

#endif  // CbmTrackingDetectorInterfaceBase_h
