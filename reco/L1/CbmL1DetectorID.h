/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmL1DetectorID.h
/// @brief  Implementation of L1DetectorID enum class for CBM
/// @author S.Zharko
/// @since  01.12.2022

#ifndef CbmL1DetectorID_h
#define CbmL1DetectorID_h 1

#include <string>

#include "L1EArray.h"

/// Enumeration for the detector subsystems used in L1 tracking
/// It is important for the subsystems to be specified in the actual order. The order is used
/// for the L1Station array filling.
/// Note: L1DetectorID has a forward declaration in L1InitManager.h and L1BaseStationInfo.h
/// TODO: Probably, we need to move everything into a single CbmCaConst.h header
enum class L1DetectorID
{
  kMvd,
  kSts,
  kMuch,
  kTrd,
  kTof,
  kEND  ///< End of enumeration
};

/// @brief  Alias to array, indexed by L1DetectorID enum
/// @note   To be used only in CBM-specific code
template<typename T>
using CbmCaDetIdArr_t = L1EArray<L1DetectorID, T>;

namespace cbm::ca
{
  /// @brief Name of detector subsystems
  constexpr CbmCaDetIdArr_t<const char*> kDetName = {{"MVD", "STS", "MuCh", "TRD", "TOF"}};

}  // namespace cbm::ca

/// @brief Enumeration for different tracking running modes
enum class ECbmCaTrackingMode
{
  kSTS,  ///< Local tracking in CBM (STS + MVD), results stored to the StsTrack branch
  kMCBM  ///< Global tracking in mCBM (STS, MuCh, TRD, TOF), results stored to GlobalTrack branch
};


#endif  // CbmL1DetectorID_h
