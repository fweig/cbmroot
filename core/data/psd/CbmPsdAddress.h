/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, David Emschermann [committer] */

/** \file CbmPsdAddress.h
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \date 09.10.2019
 **/

/** \class CbmPsdAddress
 ** \brief CBM PSD interface class to the unique address
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \version 1.0
 **
 ** CbmPsdAddress is the class for the concrete interfaces to the unique address, 
 ** which is encoded in a 32-bit field (int32_t), for the PSD detector elements.
 **
 **                                     3         2         1         0   Shift  Bits  Values
 ** Current definition:                10987654321098765432109876543210
 ** System ID (ECbmModuleId::kPsd=8) on bits  0- 3   00000000000000000000000000001111    << 0     4      15
 ** Module ID          on bits  4- 9   00000000000000000000001111110000    << 4     6      63
 ** Section ID         on bits 10-14   00000000000000000111110000000000    <<10     5      31 // to be reduced to 15
 ** Empty              on bits 15-31   11111111111111111000000000000000    <<16    17  2^17-1
 **
 **/

#ifndef CBMPSDADDRESS_H
#define CBMPSDADDRESS_H 1

#include "CbmDefs.h"  // for ECbmModuleId::kPsd

#include <cassert>  // for assert
#include <cstdint>

class CbmPsdAddress {
public:
  /**
   * \brief Return address from system ID, module, Section.
   * \param[in] moduleId Module ID.
   * \param[in] SectionId Section ID.
   * \return Address from system ID, module, Section.
   **/
  static uint32_t GetAddress(int32_t moduleId, int32_t sectionId)
  {
    assert(!(moduleId < 0 || moduleId > fgkModuleIdLength || sectionId < 0 || sectionId > fgkSectionIdLength));
    return (ToIntegralType(ECbmModuleId::kPsd) << fgkSystemIdShift) | (moduleId << fgkModuleIdShift)
           | (sectionId << fgkSectionIdShift);
  }

  /**
   * \brief Return System identifier from address.
   * \param[in] address Unique channel address.
   * \return System identifier from address.
   **/
  static uint32_t GetSystemId(uint32_t address)
  {
    return (address & (fgkSystemIdLength << fgkSystemIdShift)) >> fgkSystemIdShift;
  }

  /**
   * \brief Return module ID from address.
   * \param[in] address Unique channel address.
   * \return Module ID from address.
   **/
  static uint32_t GetModuleId(uint32_t address)
  {
    return (address & (fgkModuleIdLength << fgkModuleIdShift)) >> fgkModuleIdShift;
  }

  /**
   * \brief Return sector ID from address.
   * \param[in] address Unique channel address.
   * \return Sector ID from address.
   **/
  static uint32_t GetSectionId(uint32_t address)
  {
    return (address & (fgkSectionIdLength << fgkSectionIdShift)) >> fgkSectionIdShift;
  }

  /**
    * \brief Set new module ID for address.
    * \param address Current address.
    * \param newModuleId New value for module ID.
    * \return New address with modified module ID.
    */
  static uint32_t SetModuleId(uint32_t address, int32_t newModuleId)
  {
    assert(!(newModuleId < 0 || newModuleId > fgkModuleIdLength));
    return GetAddress(newModuleId, GetSectionId(address));
  }

  /**
    * \brief Set new section ID for address.
    * \param address Current address.
    * \param newSectionId New value for section ID.
    * \return New address with modified section ID.
    */
  static uint32_t SetSectionId(uint32_t address, int32_t newSectionId)
  {
    assert(!(newSectionId < 0 || newSectionId > fgkSectionIdLength));
    return GetAddress(GetModuleId(address), newSectionId);
  }


private:
  // Length of the index of the corresponding volume
  static const int32_t fgkSystemIdLength  = 15;  // 2^4 - 1
  static const int32_t fgkModuleIdLength  = 63;  // 2^6 - 1
  static const int32_t fgkSectionIdLength = 31;  // 2^5 - 1

  // Number of a start bit for each volume
  static const int32_t fgkSystemIdShift  = 0;
  static const int32_t fgkModuleIdShift  = 4;
  static const int32_t fgkSectionIdShift = 10;
};

#endif
