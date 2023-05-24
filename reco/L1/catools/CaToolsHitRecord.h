/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmCaHitRecord.h
/// \brief  A header for cbm::ca::HitRecord structure
/// \since  15.05.2023
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CbmCaHitRecord_h
#define CbmCaHitRecord_h 1

#include <cstdint>

namespace ca::tools
{
  /// @brief A helper structure to store hits information from different detectors in a uniform manner
  ///
  /// @note  This structure is to be used ONLY at the TS/event initialization stage to fill hit arrays of different
  ///        types.
  ///
  struct HitRecord {
    double fX           = -1.;  ///< x component of hit position [cm]
    double fY           = -1.;  ///< y component of hit position [cm]
    double fDx          = -1.;  ///< error of x component of hit position [cm]
    double fDy          = -1.;  ///< error of y component of hit position [cm]
    double fDxy         = -1.;  ///< correlation between x and y components [cm]
    double fU           = -1.;  ///< hit position in direction of front strips [cm]
    double fV           = -1.;  ///< hit position in direction of back strips [cm]
    double fDu          = -1.;  ///< hit position error in direction of front strips [cm]
    double fDv          = -1.;  ///< hit position error in direction of back strips [cm]
    double fZ           = -1.;  ///< z component of hit position [cm]
    double fT           = -1.;  ///< time of hit [ns]
    double fDt          = -1.;  ///< time error of hit [ns]
    int64_t fDataStream = -1;   ///< Global index of detector module
    int fPointId        = -1;   ///< index of MC point
    int fExtId          = -2;   ///< external index of hit
    int fStaId          = -2;   ///< index of active tracking station
    int fStripF         = -2;   ///< index of front strip
    int fStripB         = -2;   ///< index of back strip
    int fDet            = -2;   ///< detector ID
  };
}  // namespace ca::tools

#endif  // CbmCaHitRecord_h
