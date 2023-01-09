/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Valentina Akishina, Maksym Zyzak */

#ifndef _CbmL1Hit_h_
#define _CbmL1Hit_h_

#include <iomanip>
#include <sstream>
#include <string>

#include "L1Vector.h"

/// TODO: SZh: Complete the rule of five
/// TODO: SZh: Make variables private
/// TODO; SZh: Move class to ca::tools (ca::tools::Hit)

///
/// Identificator for cbm hits with their detector and index in cbm arrays
///
class CbmL1HitId {
public:
  CbmL1HitId() = default;
  CbmL1HitId(int det, int index) : detId(det), hitId(index) {};

  int detId {0};  ///< detector ID (mvd/sts/etc)
  int hitId {0};  ///< index of hit in the TClonesArray array
};


///
/// a helper class for performance evaluation that contains useful info about cbm hits with hit-mcpoint match information
///
class CbmL1HitDebugInfo {  // TODO: SZh 21.09.2022: Replace instances of this class with L1Hit
public:
  /// Gets detector type
  /// 0 - MVD
  /// 1 - STS
  /// 2 - MuCh
  /// 3 - TRD
  /// 4 - TOF
  // TODO: SZh 02.03.2023: Replace det. ID with L1DetectorID
  int GetDetectorType() const { return Det; }

  /// Gets distance from z-axis [cm]
  float GetR() const { return std::sqrt(x * x + y * y); }

  /// Gets index of the hit in the external container
  int GetExternalHitId() const { return ExtIndex; }

  /// Gets MC point index container
  const auto& GetMCPointIndexes() const { return mcPointIds; }

  /// Gets index of matched MC point
  int GetMCPointIndex() const { return mcPointIds.size() ? mcPointIds[0] : -1; }

  /// Gets global index of active tracking station
  int GetStationId() const { return iStation; }

  /// Gets time measurement [ns]
  float GetT() const { return time; }

  /// Gets x component of position [cm]
  float GetX() const { return x; }

  /// Gets y component of position [cm]
  float GetY() const { return y; }

  /// @brief String representation of the object
  /// @param verbose  Verbosity level
  /// @param header   If true, header will be printed
  std::string ToString(int verbose = 10, bool header = false) const
  {
    using std::setfill;
    using std::setw;
    std::stringstream msg;
    if (header) {
      msg << setw(8) << setfill(' ') << "ext. ID" << ' ';
      msg << setw(8) << setfill(' ') << "st. ID" << ' ';
      msg << setw(12) << setfill(' ') << "x [cm]" << ' ';
      msg << setw(12) << setfill(' ') << "y [cm]" << ' ';
      msg << setw(12) << setfill(' ') << "time [ns]" << ' ';
      msg << setw(8) << setfill(' ') << "Det. ID" << ' ';
    }
    else {
      msg << setw(8) << setfill(' ') << ExtIndex << ' ';
      msg << setw(8) << setfill(' ') << iStation << ' ';
      msg << setw(12) << setfill(' ') << x << ' ';
      msg << setw(12) << setfill(' ') << y << ' ';
      msg << setw(12) << setfill(' ') << time << ' ';
      msg << setw(8) << setfill(' ') << Det << ' ';
      if (verbose > 0) {
        msg << "\n\tMC point indexes: ";
        for (int iP : mcPointIds) {
          msg << iP << ' ';
        }
      }
    }
    return msg.str();
  }

  int ExtIndex;                                                ///< index of hit in the external branch
  int iStation;                                                ///< index of station in active stations array
  double x;                                                    ///< x coordinate of position [cm]
  double y;                                                    ///< y coordinate of position [cm]
  double time;                                                 ///< hit time [ns]
  double dx;                                                   ///< x coordinate error [cm]
  double dy;                                                   ///< y coordinate error [cm]
  double dt;                                                   ///< time error [ns]
  double dxy;                                                  ///< covariance between x and y [cm2]
  int Det;                                                     ///< detector subsystem ID
  L1Vector<int> mcPointIds {"CbmL1HitDebugInfo::mcPointIds"};  ///< indices of CbmL1MCPoint in L1->fvMCPoints array
};

#endif
