/* Copyright (C) 2010-2023 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Valentina Akishina, Maksym Zyzak, Sergei Zharko */

#ifndef _CbmL1Hit_h_
#define _CbmL1Hit_h_

#include <iomanip>
#include <sstream>
#include <string>

#include "L1Vector.h"

// TODO: SZh: Complete the rule of five
// TODO: SZh: Make variables private
// TODO: SZh: Move class to ca::tools (ca::tools::Hit)

///
/// Identificator for cbm hits with their detector and index in cbm arrays
///
class CbmL1HitId {
public:
  CbmL1HitId() = default;
  CbmL1HitId(int det, int index) : detId(det), hitId(index) {};

  /// @brief String representation of class object
  /// @param header   If true, header will be printed
  std::string ToString(bool header = false) const
  {
    std::stringstream msg;
    if (header) {
      msg << std::setw(8) << std::setfill(' ') << "det. id" << ' ';
      msg << std::setw(8) << std::setfill(' ') << "ext. id";
    }
    else {
      msg << std::setw(8) << std::setfill(' ') << detId << ' ';
      msg << std::setw(8) << std::setfill(' ') << hitId;
    }
    return msg.str();
  }

  int detId = -1;  ///< detector ID
  int hitId = -1;  ///< index of hit in the TClonesArray array
};


///
/// a helper class for performance evaluation that contains useful info about cbm hits with hit-mcpoint match information
///
class CbmL1HitDebugInfo {  // TODO: SZh 21.09.2022: Replace instances of this class with L1Hit
public:
  /// @brief Gets detector type
  /// 0 - MVD
  /// 1 - STS
  /// 2 - MuCh
  /// 3 - TRD
  /// 4 - TOF
  // TODO: SZh 02.03.2023: Replace det. ID with L1DetectorID
  int GetDetectorType() const { return Det; }

  /// @brief Gets distance from z-axis [cm]
  double GetR() const { return std::sqrt(x * x + y * y); }

  /// @brief Gets index of the hit in the external container
  int GetExternalHitId() const { return ExtIndex; }

  /// @brief Gets index of matched MC point
  int GetMCPointIndex() const { return fPointID; }

  /// @brief Gets global index of active tracking station
  int GetStationId() const { return iStation; }

  /// @brief Gets time measurement [ns]
  double GetT() const { return time; }

  /// @brief Gets x component of position [cm]
  double GetX() const { return x; }

  /// @brief Gets y component of position [cm]
  double GetY() const { return y; }

  /// @brief Sets index of matched MC point
  /// @param pointID  Internal index of MC point
  void SetMCPointIndex(int pointID) { fPointID = pointID; }


  /// @brief String representation of the object
  /// @param verbose  Verbosity level
  /// @param header   If true, header will be printed
  std::string ToString(int verbose = 0, bool header = false) const
  {
    using std::setfill;
    using std::setw;
    std::stringstream msg;
    if (header) {
      msg << setw(8) << setfill(' ') << "ext. ID" << ' ';
      msg << setw(8) << setfill(' ') << "int. ID" << ' ';
      msg << setw(8) << setfill(' ') << "st. ID" << ' ';
      msg << setw(8) << setfill(' ') << "Det. ID" << ' ';
      msg << setw(8) << setfill(' ') << "MC p. ID" << ' ';
      msg << setw(14) << setfill(' ') << "x [cm]" << ' ';
      msg << setw(14) << setfill(' ') << "y [cm]" << ' ';
      msg << setw(14) << setfill(' ') << "time [ns]" << ' ';
      if (verbose > 0) {
        msg << setw(14) << setfill(' ') << "dx [cm]" << ' ';
        msg << setw(14) << setfill(' ') << "dy [cm]" << ' ';
        msg << setw(14) << setfill(' ') << "dxy [cm2]" << ' ';
        msg << setw(14) << setfill(' ') << "dt [ns]" << ' ';
      }
    }
    else {
      msg << setw(8) << setfill(' ') << ExtIndex << ' ';
      msg << setw(8) << setfill(' ') << IntIndex << ' ';
      msg << setw(8) << setfill(' ') << iStation << ' ';
      msg << setw(8) << setfill(' ') << Det << ' ';
      msg << setw(8) << setfill(' ') << GetMCPointIndex() << ' ';
      msg << setw(14) << setfill(' ') << x << ' ';
      msg << setw(14) << setfill(' ') << y << ' ';
      msg << setw(14) << setfill(' ') << time << ' ';
      if (verbose > 0) {
        msg << setw(14) << setfill(' ') << dx << ' ';
        msg << setw(14) << setfill(' ') << dy << ' ';
        msg << setw(14) << setfill(' ') << dxy << ' ';
        msg << setw(14) << setfill(' ') << dt << ' ';
      }
    }
    return msg.str();
  }

  // TODO: SZh 2.03.2023: make the variables private
  int ExtIndex;                                                ///< index of hit in the external branch
  int IntIndex;                                                ///< index of hit in the internal array
  int iStation;                                                ///< index of station in active stations array
  int Det;                                                     ///< detector subsystem ID
  double x;                                                    ///< x coordinate of position [cm]
  double y;                                                    ///< y coordinate of position [cm]
  double time;                                                 ///< hit time [ns]
  double dx;                                                   ///< x coordinate error [cm]
  double dy;                                                   ///< y coordinate error [cm]
  double dt;                                                   ///< time error [ns]
  double dxy;                                                  ///< covariance between x and y [cm2]
  int fPointID = -1;                                           ///< index of matched MC point
};

#endif
