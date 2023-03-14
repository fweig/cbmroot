/* Copyright (C) 2013-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Andrey Lebedev [committer] */

/** @file CbmStsAddress.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.05.2013
 **/

#ifndef CBMSTSADDRESS_H
#define CBMSTSADDRESS_H 1

#include "CbmDefs.h"  // for ECbmModuleId

#include <cassert>  // for assert
#include <cstdint>  // for uint32_t
#include <sstream>  // for string

#include <xpu/defines.h>  // for XPU_D

/** Enumerator for the hierarchy levels of the STS setup **/
enum EStsElementLevel
{
  kStsSystem,
  kStsUnit,
  kStsLadder,
  kStsHalfLadder,
  kStsModule,
  kStsSensor,
  kStsSide,
  kStsNofLevels
};


/** Namespace CbmStsAddress
 ** @brief Functions to encode or decode the address field of STS data.
 ** @author V.Friese <v.friese@gsi.de>
 **
 ** The current definition (version 1) of the address bit field for the STS is:
 **
 ** Level           Bits  Max. Elements  Bit Position
 ** System (kSTS)     4         16          0 -  3
 ** Unit              6         64          4 -  9
 ** Ladder            5         32         10 - 14
 ** HalfLadder        1          2         15
 ** Module            5         32         16 - 20
 ** Sensor            4         16         21 - 24
 ** Side              1          2         25
 ** Unused            2                    26 - 27
 ** Version           4         16         28 - 31
 **/
namespace CbmStsAddress
{

  inline constexpr uint32_t kCurrentVersion = 1;

  // --- These values are not to be changed if backward compatibility
  // --- shall be maintained.
  inline constexpr int32_t kVersionSize  = 4;   // Bits for version number
  inline constexpr int32_t kVersionShift = 28;  // First bit for version number
  inline constexpr int32_t kVersionMask  = (1 << kVersionSize) - 1;

  namespace Detail
  {
    // clang-format off
    // -----    Definition of address bit field   ------------------------------
    inline constexpr uint16_t kBits[kCurrentVersion + 1][kStsNofLevels] = {

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
    inline constexpr int32_t kShift[kCurrentVersion + 1][kStsNofLevels] = {
      {0, kShift[0][0] + kBits[0][0], kShift[0][1] + kBits[0][1], kShift[0][2] + kBits[0][2], kShift[0][3] + kBits[0][3],
      kShift[0][4] + kBits[0][4], kShift[0][5] + kBits[0][5]},

      {0, kShift[1][0] + kBits[1][0], kShift[1][1] + kBits[1][1], kShift[1][2] + kBits[1][2], kShift[1][3] + kBits[1][3],
      kShift[1][4] + kBits[1][4], kShift[1][5] + kBits[1][5]}};
    // -------------------------------------------------------------------------

    // -----    Bit masks  -----------------------------------------------------
    inline constexpr int32_t kMask[kCurrentVersion + 1][kStsNofLevels] = {
      {(1 << kBits[0][0]) - 1, (1 << kBits[0][1]) - 1, (1 << kBits[0][2]) - 1, (1 << kBits[0][3]) - 1,
      (1 << kBits[0][4]) - 1, (1 << kBits[0][5]) - 1, (1 << kBits[0][6]) - 1},

      {(1 << kBits[1][0]) - 1, (1 << kBits[1][1]) - 1, (1 << kBits[1][2]) - 1, (1 << kBits[1][3]) - 1,
      (1 << kBits[1][4]) - 1, (1 << kBits[1][5]) - 1, (1 << kBits[1][6]) - 1}};
    // -------------------------------------------------------------------------
    // clang-format on
  }  // Namespace Detail


  /** @brief Construct address
   ** @param unit         Unit index
   ** @param ladder       Ladder index in station
   ** @param halfladder   Halfladder index in ladder
   ** @param module       Module index within halfladder
   ** @param sensor       Sensor index within module
   ** @param side         Side (0=front, 1=back) of sensor
   ** @param channel      Channel number
   ** @return Unique element address
   **/
  int32_t GetAddress(uint32_t unit = 0, uint32_t ladder = 0, uint32_t halfladder = 0, uint32_t module = 0,
                     uint32_t sensor = 0, uint32_t side = 0, uint32_t version = kCurrentVersion);


  /** @brief Construct address
   ** @param elementIds   Array of element indices in their mother volumes
   ** @return Unique element address
   **/
  int32_t GetAddress(uint32_t* elementId, uint32_t version);


  /** @brief Construct the address of an element from the address of a
   ** descendant element.
   ** @param address Address of descendant element
   ** @param level   Desired hierarchy level
   ** @return Address of element at desired hierarchy level
   **
   ** This strips of the address information of all hierarchy levels
   ** below the desired one.
   **/
  int32_t GetMotherAddress(int32_t address, int32_t level);


  /** @brief Get the index of an element
   ** @param address Unique element address
   ** @param level Hierarchy level
   ** @return Element index
   **/
  uint32_t GetElementId(int32_t address, int32_t level);


  /** @brief Get system Id (should be ECbmModuleId::kSts)
   ** @param address Unique element address
   **/
  ECbmModuleId GetSystemId(int32_t address);


  /** @brief Extract version number
   ** @param address Unique element address
   ** @value Version number
   **
   ** The version is encoded in the last 4 bits (28 to 31).
   ** The maximal number of versions is 16.
   **/
  uint32_t GetVersion(int32_t address);


  /** @brief Set the index of an element, leaving the other element levels
   ** untouched
   ** @param address Unique element address
   ** @param level   Hierarchy level
   ** @param newId   New element index
   ** @return New address
   **/
  int32_t SetElementId(int32_t address, int32_t level, uint32_t newId);

  /** @brief Add version and system to compressed address that's stored in a digi
   ** @param digiAddress Compressed address from digi
   ** @return Full address
   **/
  XPU_D inline int32_t UnpackDigiAddress(int32_t digiAddress)
  {
    using namespace Detail;
    return digiAddress << kShift[1][kStsUnit] | ToIntegralType(ECbmModuleId::kSts) << kShift[1][kStsSystem]
           | 1u << kVersionShift;
  }

  /** @brief Strip address to contain only unit, (half)ladder and module.
   ** @param address Full address
   ** @return 17 bit address that can be stored in a Digi
   **/
  XPU_D inline int32_t PackDigiAddress(int32_t address)
  {
    using namespace Detail;
    constexpr int32_t kDMask = kMask[1][kStsUnit] << kShift[1][kStsUnit] | kMask[1][kStsLadder] << kShift[1][kStsLadder]
                               | kMask[1][kStsHalfLadder] << kShift[1][kStsHalfLadder]
                               | kMask[1][kStsModule] << kShift[1][kStsModule];

    int32_t ret = (address & kDMask) >> kShift[1][kStsUnit];

    // Check that no bits were set, that are stripped by this function.
    assert(address == UnpackDigiAddress(ret));

    return ret;
  }

  /** @brief String output
   ** @param address Unique element address
   **/
  std::string ToString(int32_t address);

}  // Namespace CbmStsAddress


#endif  // CBMSTSADDRESS_H
