/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CbmL1DetectorID.h
/// \brief  Implementation of L1DetectorID enum class for CBM
/// \author S.Zharko
/// \data   01.12.2022

#ifndef CbmL1DetectorID_h
#define CbmL1DetectorID_h 1

/// Enumeration for the detector subsystems used in L1 tracking
/// It is important for the subsystems to be specified in the actual order. The order is used
/// for the L1Station array filling.
/// Note: L1DetectorID has a forward declaration in L1InitManager.h and L1BaseStationInfo.h
enum class L1DetectorID
{
  kMvd,
  kSts,
  kMuch,
  kTrd,
  kTof
};

/// @brief Enumeration for different tracking running modes
enum class ECbmTrackingMode
{
  kSTS,  ///< Local tracking in CBM (STS + MVD), results stored to the StsTrack branch
  kMCBM  ///< Global tracking in mCBM (STS, MuCh, TRD, TOF), results stored to GlobalTrack branch
};


#endif  // CbmL1DetectorID_h
