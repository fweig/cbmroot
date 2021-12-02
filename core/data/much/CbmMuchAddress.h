/* Copyright (C) 2013-2020 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Kryshen [committer] */

#ifndef CBMMUCHADDRESS_H
#define CBMMUCHADDRESS_H 1

#include "CbmAddress.h"  // for CbmAddress

#include <Rtypes.h>  // for ClassDef

#include <cstdint>

/** Enumerator for MUCH element levels
 ** If this is changed, the initialisation of the static members
 ** of CbmMuchAddress must be changed accordingly!
 **/
enum MuchElementLevel
{
  kMuchSystem,     //!< System = MUCH
  kMuchStation,    //!< Station
  kMuchLayer,      //!< Layer
  kMuchLayerSide,  //!< LayerSide
  kMuchModule,     //!< Module
  kMuchSector,     //!< Sector
  kMuchChannel,    //!< Channel
  kMuchNofLevels   //!< Number of MUCH levels
};


/** @class CbmMuchAddress
 ** @brief Interface class to unique address for the MUCH
 ** @author E.Kryshen <e.kryshen@gsi.de>
 ** @version 1.0
 **
 **Created as modification of CbmStsAddress class
 **
 ** The CbmMuchAddress interprets and modifies the unique address
 ** for the MUCH by the proper bit operation on the address bit field.
 **
 ** The current definition of the bit field for the MUCH is:
 **
 **                                               3         2         1
 ** Level           Bits  Max. Elements  Shift   10987654321098765432109876543210 \n
 ** System (kMUCH)    4         16          0    0000000000000000000000000000xxxx \n
 ** Station           3          8          4    0000000000000000000000000xxx0000 \n
 ** Layer             2          4          7    00000000000000000000000xx0000000 \n
 ** LayerSide         1          2          9    0000000000000000000000x000000000 \n
 ** Module            5         32         10    00000000000000000xxxxx0000000000 \n
 ** Sector            8        256         15    000000000xxxxxxxx000000000000000 \n
 ** Channel           9        512         23    xxxxxxxxx00000000000000000000000 \n
 **/
class CbmMuchAddress : public CbmAddress {

public:
  /** Construct address
     ** @param station      Station index
     ** @param layer        Layer index in station
     ** @param side         Side index in layer
     ** @param module       Module index within layerside
     ** @param sector       Sector index within module
     ** @param channel      Channel number
     ** @return Unique element address
     **/
  static uint32_t GetAddress(int32_t station = 0, int32_t layer = 0, int32_t side = 0, int32_t module = 0,
                             int32_t sector = 0, int32_t channel = 0);


  static uint32_t GetAddress(int32_t* elementIds);


  /** Get the number of hierarchy levels
     ** For use in macros which do not include this header file.
     ** @return       Number of hierarchy levels
     **/
  static int32_t GetNofLevels() { return kMuchNofLevels; }


  /** Get the number of bits for a given hierarchy level
     ** @param level  Requested element level
     ** @return       Number of bits in address field
     **/
  static int32_t GetNofBits(int32_t level)
  {
    if (level < 0 || level >= kMuchNofLevels) return 0;
    return fgkBits[level];
  }


  /** Get the index of an element
     ** @param address Unique element address
     ** @param level Hierarchy level
     ** @return Element index
     **/
  static int32_t GetElementId(uint32_t address, int32_t level)
  {
    if (level < 0 || level >= kMuchNofLevels) return -1;
    return (address & (fgkMask[level] << fgkShift[level])) >> fgkShift[level];
  }

  /** Derivatives */
  static int32_t GetSystemIndex(int32_t address) { return GetElementId(address, kMuchSystem); }
  static int32_t GetStationIndex(int32_t address) { return GetElementId(address, kMuchStation); }
  static int32_t GetLayerIndex(int32_t address) { return GetElementId(address, kMuchLayer); }
  static int32_t GetLayerSideIndex(int32_t address) { return GetElementId(address, kMuchLayerSide); }
  static int32_t GetModuleIndex(int32_t address) { return GetElementId(address, kMuchModule); }
  static int32_t GetSectorIndex(int32_t address) { return GetElementId(address, kMuchSector); }
  static int32_t GetChannelIndex(int32_t address) { return GetElementId(address, kMuchChannel); }

  static int32_t GetElementAddress(int32_t address, int32_t level)
  {
    int32_t mask = (1 << (CbmMuchAddress::fgkShift[level] + CbmMuchAddress::fgkBits[level])) - 1;
    return address & mask;
  }

  /** Print information on the bit field **/
  static void Print();


  /** Set the index of an element
     ** leaving the other element levels untouched
     ** @param address Unique element address
     ** @param level   Hierarchy level
     ** @param newId   New element index
     ** @return New address
     **/
  static uint32_t SetElementId(uint32_t address, int32_t level, int32_t newId);

private:
  /** Number of bits for the different levels **/
  static const int32_t fgkBits[kMuchNofLevels];

  /** Bit shifts for the different levels **/
  static const int32_t fgkShift[kMuchNofLevels];

  /** Bit masks for the different levels **/
  static const int32_t fgkMask[kMuchNofLevels];


  ClassDef(CbmMuchAddress, 1);
};


#endif /* CBMMUCHADDRESS_H  */
