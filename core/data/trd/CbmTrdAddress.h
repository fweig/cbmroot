/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, Florian Uhlig [committer] */

/**
 * \file CbmTrdAddress.h
 * \author F. Uhlig <f.uhlig@gsi.de>
 * \date 25/05/09
 * \brief Helper class to convert unique channel ID back and forth.
 *
 * Modified 06/05/2013 by Andrey Lebedev <andrey.lebedev@gsi.de>
 *
 *                                 3         2         1          shift length
 * Current definition:            10987654321098765432109876543210
 * System ID (kTRD=5) on bits 0-3 00000000000000000000000000001111 <<0  15
 * Layer ID on bits 4-7           00000000000000000000000011110000 <<4  15
 * Module ID on bits 8-14         00000000000000000111111100000000 <<8  127
 * Sector ID on bits 15-16        00000000000000011000000000000000 <<15 3
 * Row ID on bits 17-23           00000000111111100000000000000000 <<17 127
 * Column ID on bits 24-31        11111111000000000000000000000000 <<24 255
 *
 **/

#ifndef CBMTRDDETECTORID_H
#define CBMTRDDETECTORID_H 1

#include "CbmDefs.h"  // for kTrd

#include <cassert>  // for assert
#include <cstdint>


class CbmTrdAddress {
public:
  /**
   * \brief Return address from system ID, layer, module, sector, column and row IDs.
   * \param[in] layerId Layer ID.
   * \param[in] moduleId Module ID.
   * \param[in] sectorId Sector ID.
   * \param[in] rowId Row ID.
   * \param[in] columnId Column ID.
   * \return Address from system ID, layer, module, sector, row and column IDs.
   **/
  static uint32_t GetAddress(int32_t layerId, int32_t moduleId, int32_t sectorId, int32_t rowId, int32_t columnId)
  {
    assert(!(layerId < 0 || layerId > fgkLayerIdLength || moduleId < 0 || moduleId > fgkModuleIdLength || sectorId < 0
             || sectorId > fgkSectorIdLength || rowId < 0 || rowId > fgkRowIdLength || columnId < 0
             || columnId > fgkColumnIdLength));
    return (ToIntegralType(ECbmModuleId::kTrd) << fgkSystemIdShift) | (layerId << fgkLayerIdShift)
           | (moduleId << fgkModuleIdShift) | (sectorId << fgkSectorIdShift) | (rowId << fgkRowIdShift)
           | (columnId << fgkColumnIdShift);
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
   * \brief Return layer ID from address.
   * \param[in] address Unique channel address.
   * \return Layer ID from address.
   **/
  static uint32_t GetLayerId(uint32_t address)
  {
    return (address & (fgkLayerIdLength << fgkLayerIdShift)) >> fgkLayerIdShift;
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
  static uint32_t GetSectorId(uint32_t address)
  {
    return (address & (fgkSectorIdLength << fgkSectorIdShift)) >> fgkSectorIdShift;
  }

  /**
   * \brief Return row ID from address.
   * \param[in] address Unique channel address.
   * \return Row ID from address.
   **/
  static uint32_t GetRowId(uint32_t address) { return (address & (fgkRowIdLength << fgkRowIdShift)) >> fgkRowIdShift; }

  /**
   * \brief Return column ID from address.
   * \param[in] address Unique channel address.
   * \return Column ID from address.
   **/
  static uint32_t GetColumnId(uint32_t address)
  {
    return (address & (fgkColumnIdLength << fgkColumnIdShift)) >> fgkColumnIdShift;
  }

  /**
   * \brief Return unique module ID from address.
   * \param[in] address Unique channel address.
   * \return Unique module ID from address.
   **/
  static uint32_t GetModuleAddress(uint32_t address)
  {
    return (address & (fgkModuleAddressLength << fgkModuleAddressStart)) >> fgkModuleAddressStart;
  }

  /**
   * \brief Return unique sector ID from address.
   * \param[in] address Unique channel address.
   * \return Unique sector ID from address.
   **/
  static uint32_t GetSectorAddress(uint32_t address)
  {
    return (address & (fgkSectorAddressLength << fgkSectorAddressStart)) >> fgkSectorAddressStart;
  }

  /**
    * \brief Set new layer ID for address.
    * \param address Current address.
    * \param newLayerId New value for layer ID.
    * \return New address with modified layer ID.
    */
  static uint32_t SetLayerId(uint32_t address, int32_t newLayerId)
  {
    assert(!(newLayerId < 0 || newLayerId > fgkLayerIdLength));
    return GetAddress(newLayerId, GetModuleId(address), GetSectorId(address), GetRowId(address), GetColumnId(address));
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
    return GetAddress(GetLayerId(address), newModuleId, GetSectorId(address), GetRowId(address), GetColumnId(address));
  }

  /**
    * \brief Set new sector ID for address.
    * \param address Current address.
    * \param newSectorId New value for sector ID.
    * \return New address with modified sector ID.
    */
  static uint32_t SetSectorId(uint32_t address, int32_t newSectorId)
  {
    assert(!(newSectorId < 0 || newSectorId > fgkSectorIdLength));
    return GetAddress(GetLayerId(address), GetModuleId(address), newSectorId, GetRowId(address), GetColumnId(address));
  }

  /**
    * \brief Set new row ID for address.
    * \param address Current address.
    * \param newRowId New value for row ID.
    * \return New address with modified row ID.
    */
  static uint32_t SetRowId(uint32_t address, int32_t newRowId)
  {
    assert(!(newRowId < 0 || newRowId > fgkRowIdLength));
    return GetAddress(GetLayerId(address), GetModuleId(address), GetSectorId(address), newRowId, GetColumnId(address));
  }

  /**
    * \brief Set new column ID for address.
    * \param address Current address.
    * \param newColumnId New value for column ID.
    * \return New address with modified column ID.
    */
  static uint32_t SetColumnId(uint32_t address, int32_t newColumnId)
  {
    assert(!(newColumnId < 0 || newColumnId > fgkColumnIdLength));
    return GetAddress(GetLayerId(address), GetModuleId(address), GetSectorId(address), GetRowId(address), newColumnId);
  }

private:
  // Length of the index of the corresponding volume
  static const int32_t fgkSystemIdLength = 15;   // 2^4 - 1
  static const int32_t fgkLayerIdLength  = 15;   // 2^4 - 1
  static const int32_t fgkModuleIdLength = 127;  // 2^7 - 1
  static const int32_t fgkSectorIdLength = 3;    // 2^2 - 1
  static const int32_t fgkRowIdLength    = 127;  // 2^7 - 1
  static const int32_t fgkColumnIdLength = 255;  // 2^8 - 1
  // Number of a start bit for each volume
  static const int32_t fgkSystemIdShift = 0;
  static const int32_t fgkLayerIdShift  = 4;
  static const int32_t fgkModuleIdShift = 8;
  static const int32_t fgkSectorIdShift = 15;
  static const int32_t fgkRowIdShift    = 17;
  static const int32_t fgkColumnIdShift = 24;
  // Start bit and length for unique module address.
  // For unique module address use all bits up to sector ID.
  static const int32_t fgkModuleAddressStart  = 0;
  static const int32_t fgkModuleAddressLength = 32767;  // 2^15 - 1
  // Start bit and length for unique sector address.
  // For unique sector address use all bits up to column ID.
  static const int32_t fgkSectorAddressStart  = 0;
  static const int32_t fgkSectorAddressLength = 131071;  // 2^17 - 1
};

#endif
