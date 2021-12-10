/* Copyright (C) 2013-2020 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Kryshen [committer], Florian Uhlig */

#include "CbmMuchAddress.h"

#include "CbmDefs.h"  // for kMuch

#include <iomanip>  // for setw, __iom_t6
#include <ios>      // for right


// -----    Definition of the address field   -------------------------------
const int32_t CbmMuchAddress::fgkBits[] = {fgkSystemBits,  // system = kMUCH
                                           3,              // station
                                           2,              // layer
                                           1,              // layerside
                                           5,              // module
                                           8,              // sector
                                           9};             // channel
// -------------------------------------------------------------------------

// -----    Initialisation of bit shifts -----------------------------------
const int32_t CbmMuchAddress::fgkShift[] = {0,
                                            CbmMuchAddress::fgkShift[0] + CbmMuchAddress::fgkBits[0],
                                            CbmMuchAddress::fgkShift[1] + CbmMuchAddress::fgkBits[1],
                                            CbmMuchAddress::fgkShift[2] + CbmMuchAddress::fgkBits[2],
                                            CbmMuchAddress::fgkShift[3] + CbmMuchAddress::fgkBits[3],
                                            CbmMuchAddress::fgkShift[4] + CbmMuchAddress::fgkBits[4],
                                            CbmMuchAddress::fgkShift[5] + CbmMuchAddress::fgkBits[5]};
// -------------------------------------------------------------------------


// -----    Initialisation of bit masks  -----------------------------------
const int32_t CbmMuchAddress::fgkMask[] = {(1 << fgkBits[0]) - 1, (1 << fgkBits[1]) - 1, (1 << fgkBits[2]) - 1,
                                           (1 << fgkBits[3]) - 1, (1 << fgkBits[4]) - 1, (1 << fgkBits[5]) - 1,
                                           (1 << fgkBits[6]) - 1};
// -------------------------------------------------------------------------


// -----  Unique element address   -----------------------------------------
uint32_t CbmMuchAddress::GetAddress(int32_t station, int32_t layer, int32_t layerside, int32_t module, int32_t sector,
                                    int32_t channel)
{

  // Catch overrunning of allowed ranges
  if (station >= (1 << fgkBits[kMuchStation])) {
    LOG(error) << "Station Id " << station << " exceeds maximum (" << (1 << fgkBits[kMuchStation]) - 1 << ")";
    return 0;
  }
  if (layer >= (1 << fgkBits[kMuchLayer])) {
    LOG(error) << "Layer Id " << layer << " exceeds maximum (" << (1 << fgkBits[kMuchLayer]) - 1 << ")";
    return 0;
  }
  if (layerside >= (1 << fgkBits[kMuchLayerSide])) {
    LOG(error) << "LayerSide Id " << layerside << " exceeds maximum (" << (1 << fgkBits[kMuchLayerSide]) - 1 << ")";
    return 0;
  }
  if (module >= (1 << fgkBits[kMuchModule])) {
    LOG(error) << "Module Id " << module << " exceeds maximum (" << (1 << fgkBits[kMuchModule]) - 1 << ")";
    return 0;
  }
  if (sector >= (1 << fgkBits[kMuchSector])) {
    LOG(error) << "Sector Id " << sector << " exceeds maximum (" << (1 << fgkBits[kMuchSector]) - 1 << ")";
    return 0;
  }
  if (channel >= (1 << fgkBits[kMuchChannel])) {
    LOG(error) << "Channel Id " << channel << " exceeds maximum (" << (1 << fgkBits[kMuchChannel]) - 1 << ")";
    return 0;
  }

  return ToIntegralType(ECbmModuleId::kMuch) << fgkShift[kMuchSystem] | station << fgkShift[kMuchStation]
         | layer << fgkShift[kMuchLayer] | layerside << fgkShift[kMuchLayerSide] | module << fgkShift[kMuchModule]
         | sector << fgkShift[kMuchSector] | channel << fgkShift[kMuchChannel];
}
// -------------------------------------------------------------------------


// -----  Unique element address   -----------------------------------------
uint32_t CbmMuchAddress::GetAddress(int32_t* elementId)
{

  uint32_t address = ToIntegralType(ECbmModuleId::kMuch) << fgkShift[kMuchSystem];
  for (int32_t level = 1; level < kMuchNofLevels; level++) {
    if (elementId[level] >= (1 << fgkBits[level])) {
      LOG(error) << "Id " << elementId[level] << " for MUCH level " << level << " exceeds maximum ("
                 << (1 << fgkBits[level]) - 1 << ")";
      return 0;
    }
    address = address | (elementId[level] << fgkShift[level]);
  }

  return address;
}
// -------------------------------------------------------------------------


// -----   Print info   ----------------------------------------------------
void CbmMuchAddress::Print()
{
  LOG(info) << "Number of MUCH levels is " << kMuchNofLevels;
  for (int32_t level = 0; level < kMuchNofLevels; level++)
    LOG(info) << "Level " << std::setw(2) << std::right << level << ": bits " << std::setw(2) << fgkBits[level]
              << ", max. range " << std::setw(6) << fgkMask[level];
}
// -------------------------------------------------------------------------


ClassImp(CbmMuchAddress)
