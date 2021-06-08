/* Copyright (C) 2013-2020 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Andrey Lebedev [committer] */

/** @file CbmStsAddress.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.05.2013
 **/

#include "CbmStsAddress.h"

#include "CbmDefs.h"  // for kSts

#include <Logger.h>  // for Logger, LOG

#include <cassert>  // for assert
#include <sstream>  // for operator<<, basic_ostream, stringstream

namespace CbmStsAddress
{

  // -----    Definition of address bit field   ------------------------------
  const UShort_t kBits[kCurrentVersion + 1][kStsNofLevels] = {

    // Version 0 (until 23 August 2017)
    {
      4,  // system
      4,  // unit / station
      4,  // ladder
      1,  // half-ladder
      3,  // module
      2,  // sensor
      1   // side
    },

    // Version 1 (current, since 23 August 2017)
    {
      4,  // system
      6,  // unit
      5,  // ladder
      1,  // half-ladder
      5,  // module
      4,  // sensor
      1   // side
    }

  };
  // -------------------------------------------------------------------------


  // -----    Bit shifts -----------------------------------------------------
  const Int_t kShift[kCurrentVersion + 1][kStsNofLevels] = {
    {0, kShift[0][0] + kBits[0][0], kShift[0][1] + kBits[0][1], kShift[0][2] + kBits[0][2], kShift[0][3] + kBits[0][3],
     kShift[0][4] + kBits[0][4], kShift[0][5] + kBits[0][5]},

    {0, kShift[1][0] + kBits[1][0], kShift[1][1] + kBits[1][1], kShift[1][2] + kBits[1][2], kShift[1][3] + kBits[1][3],
     kShift[1][4] + kBits[1][4], kShift[1][5] + kBits[1][5]}};
  // -------------------------------------------------------------------------


  // -----    Bit masks  -----------------------------------------------------
  const Int_t kMask[kCurrentVersion + 1][kStsNofLevels] = {
    {(1 << kBits[0][0]) - 1, (1 << kBits[0][1]) - 1, (1 << kBits[0][2]) - 1, (1 << kBits[0][3]) - 1,
     (1 << kBits[0][4]) - 1, (1 << kBits[0][5]) - 1, (1 << kBits[0][6]) - 1},

    {(1 << kBits[1][0]) - 1, (1 << kBits[1][1]) - 1, (1 << kBits[1][2]) - 1, (1 << kBits[1][3]) - 1,
     (1 << kBits[1][4]) - 1, (1 << kBits[1][5]) - 1, (1 << kBits[1][6]) - 1}};
  // -------------------------------------------------------------------------


}  // Namespace CbmStsAddress


// -----   Construct address from element Ids   ------------------------------
Int_t CbmStsAddress::GetAddress(UInt_t unit, UInt_t ladder, UInt_t halfladder, UInt_t module, UInt_t sensor,
                                UInt_t side, UInt_t version)
{

  assert(version <= kCurrentVersion);

  // Catch overrun of allowed ranges
  UInt_t maxUnit = (1 << kBits[version][kStsUnit]) - 1;
  if (unit > maxUnit) {
    LOG(error) << "Unit Id " << unit << " exceeds maximum " << maxUnit;
    return 0;
  }
  UInt_t maxLadder = (1 << kBits[version][kStsLadder]) - 1;
  if (ladder > maxLadder) {
    LOG(error) << "Ladder Id " << ladder << " exceeds maximum " << maxLadder;
    return 0;
  }
  UInt_t maxHalfLadder = (1 << kBits[version][kStsHalfLadder]) - 1;
  if (halfladder > maxHalfLadder) {
    LOG(error) << "HalfLadder Id " << halfladder << " exceeds maximum " << maxHalfLadder;
    return 0;
  }
  UInt_t maxModule = (1 << kBits[version][kStsModule]) - 1;
  if (module > maxModule) {
    LOG(error) << "Module Id " << module << " exceeds maximum " << maxModule;
    return 0;
  }
  UInt_t maxSensor = (1 << kBits[version][kStsSensor]) - 1;
  if (sensor > maxSensor) {
    LOG(error) << "Sensor Id " << sensor << " exceeds maximum " << maxSensor;
    return 0;
  }
  UInt_t maxSide = (1 << kBits[version][kStsSide]) - 1;
  if (side > maxSide) {
    LOG(error) << "Side Id " << side << " exceeds maximum " << maxSide;
    return 0;
  }

  return ToIntegralType(ECbmModuleId::kSts) << kShift[version][kStsSystem] | unit << kShift[version][kStsUnit]
         | ladder << kShift[version][kStsLadder] | halfladder << kShift[version][kStsHalfLadder]
         | module << kShift[version][kStsModule] | sensor << kShift[version][kStsSensor]
         | side << kShift[version][kStsSide] | version << kVersionShift;
}
// ---------------------------------------------------------------------------


// -----   Construct address from array of element Ids   ----------------------
Int_t CbmStsAddress::GetAddress(UInt_t* elementId, UInt_t version)
{

  assert(version <= kCurrentVersion);

  Int_t address = ToIntegralType(ECbmModuleId::kSts) << kShift[version][kStsSystem];
  for (Int_t level = 1; level < kStsNofLevels; level++) {
    UInt_t maxId = (1 << kBits[version][level]) - 1;
    if (elementId[level] > maxId) {
      LOG(error) << "Id " << elementId[level] << " for STS level " << level << " exceeds maximum " << maxId;
      return 0;
    }
    address = address | (elementId[level] << kShift[version][level]);
  }
  address = address | (version << kVersionShift);

  return address;
}
// ---------------------------------------------------------------------------


// -----   Construct address from address of descendant element   ------------
Int_t CbmStsAddress::GetMotherAddress(Int_t address, Int_t level)
{
  assert(level >= kStsSystem && level < kStsNofLevels);
  if (level == kStsNofLevels - 1) return address;
  UInt_t version  = GetVersion(address);
  Int_t motherAdd = (address & ((1 << kShift[version][level + 1]) - 1));
  motherAdd       = motherAdd | (version << kVersionShift);
  return motherAdd;
}
// ---------------------------------------------------------------------------


// -----   Get the index of an element   -------------------------------------
UInt_t CbmStsAddress::GetElementId(Int_t address, Int_t level)
{
  assert(level >= kStsSystem && level < kStsNofLevels);
  UInt_t version = GetVersion(address);
  return (address & (kMask[version][level] << kShift[version][level])) >> kShift[version][level];
}
// ---------------------------------------------------------------------------


// -----   Get System ID   ---------------------------------------------------
ECbmModuleId CbmStsAddress::GetSystemId(Int_t address)
{
  return static_cast<ECbmModuleId>(GetElementId(address, kStsSystem));
  //  return GetElementId(address, kStsSystem);
}
// ---------------------------------------------------------------------------


// -----   Get the version number from the address   -------------------------
UInt_t CbmStsAddress::GetVersion(Int_t address)
{
  return UInt_t((address & (kVersionMask << kVersionShift)) >> kVersionShift);
}
// ---------------------------------------------------------------------------


// -----  Construct address by changing the index of an element   ------------
Int_t CbmStsAddress::SetElementId(Int_t address, Int_t level, UInt_t newId)
{
  assert(level >= kStsSystem && level < kStsNofLevels);
  UInt_t version = GetVersion(address);
  UInt_t maxId   = (1 << kBits[version][level]) - 1;
  if (newId > maxId) {
    LOG(fatal) << "Id " << newId << " for STS level " << level << " exceeds maximum " << maxId;
    return 0;
  }
  return (address & (~(kMask[version][level] << kShift[version][level]))) | (newId << kShift[version][level]);
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
std::string CbmStsAddress::ToString(Int_t address)
{
  std::stringstream ss;

  ss << "StsAddress: address " << address << " (version " << GetVersion(address) << ")"
     << ": system " << GetElementId(address, kStsSystem) << ", unit " << GetElementId(address, kStsUnit) << ", ladder "
     << GetElementId(address, kStsLadder) << ", half-ladder " << GetElementId(address, kStsHalfLadder) << ", module "
     << GetElementId(address, kStsModule) << ", sensor " << GetElementId(address, kStsSensor) << ", side "
     << GetElementId(address, kStsSide);
  return ss.str();
}
// -------------------------------------------------------------------------
