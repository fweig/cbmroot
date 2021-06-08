/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Pascal Raisig */

#ifndef CBMTRDPARSPADIC_H
#define CBMTRDPARSPADIC_H

#define NSPADICCH 32

#include "CbmTrdParAsic.h"  // for CbmTrdParAsic

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, UInt_t

#include <map>     // fMapAsicChannelToElink
#include <vector>  // fVecSpadicChannels

#include <stdint.h>  // for uint64_t, uint8_t, uint16_t

class FairParamList;

/** \brief Definition of SPADIC parameters **/
class CbmTrdParSpadic : public CbmTrdParAsic {
public:
  CbmTrdParSpadic(Int_t address = 0, Int_t FebGrouping = -1, Double_t x = 0, Double_t y = 0, Double_t z = 0,
                  std::uint64_t compId = 0);
  virtual ~CbmTrdParSpadic() { ; }

  virtual void LoadParams(
    FairParamList*
      inList);  ///< Loads the specific params for the spadic placed at fAddress, e.g. channel calibration parameters

  virtual Int_t GetNchannels() const { return NSPADICCH; };

  virtual Double_t GetSizeX() const { return fgSizeX; }
  virtual Double_t GetSizeY() const { return fgSizeY; }
  virtual Double_t GetSizeZ() const { return fgSizeZ; }

  static std::uint64_t CreateComponentId(
    Int_t criId, Int_t crobId, Int_t nThCrobOnModule,
    Int_t
      eLinkId);  ///< Create the componentId from a given criId, crobId, eLinkId and the nThCrobOnModule count, according to the scheme, defined by ECbmTrdComponentIdDecoding.

  std::vector<UInt_t> GetSpadicChannelVec() { return fVecSpadicChannels; }
  ///< Return the vector with the corresponding spadic channel numbers sorted from channel 00..31 in pad plane coordinates

  static Int_t GetNasicsOnModule(
    Int_t moduleType);  ///< Returns the number of asics on a given moduleType defined in eCbmTrdModuleTypes
  static Int_t GetNasicsPerCrob(
    Int_t moduleType);  ///< Returns the number of asics per Crob on a given moduleType defined in eCbmTrdModuleTypes
  static std::uint16_t GetCriId(
    std::uint64_t
      componentId);  ///< Extracts the CriId from a given componentId - Remark when the par files are created from geometries the CriId is set to the unique module number
  static std::uint8_t GetCrobId(std::uint64_t componentId);  ///< Extracts the CrobId from a given componentId
  static std::uint8_t GetCrobNumber(
    std::uint64_t componentId);  ///< Extracts the CrobNumber (nTh Crob on the module) from a given componentId
  static std::uint8_t GetElinkId(
    std::uint64_t componentId,
    Int_t
      channelId);  ///< eLinkId for the given asicAddress and channelId (in the asic coordinates, i.e. 00..31). Remark: no check of a correct componentId is performed

  std::uint16_t GetCriId();
  std::uint8_t GetCrobId();
  std::uint8_t GetCrobNumber();
  std::uint8_t GetElinkId(
    Int_t
      channelId);  ///< eLinkId for the current asic par set and the given channelId (in the asic coordinates, i.e. 00..31).
  UInt_t GetElinkNr(Int_t moduleChannel, UInt_t nChannelsPerRow);
  ///< Return the number of the elink (counting started in channel order from bottom left to right) correlated to module wide channel number passed as argument, e.g. 000...767 for the mcbm module type
  UInt_t GetAddressOnModule() const
  {
    return fAddress % 1000;
  }  ///< Returns the number of the asic on the module counted from top left
  Int_t GetAsicChAddress(const Int_t asicChannel);
  ///< Returns the nth asic Channel in asic coordinates in single asic padplane coordinates. Spadic channels are not mapped from 00 to 31 in padplane coordinates, this function returns the padplane channelnumber in the system of one asic(not in the channel map of a full module !)


private:
  static Double_t fgSizeX;  ///< SPADIC half size in x [cm]
  static Double_t fgSizeY;  ///< SPADIC half size in y [cm]
  static Double_t fgSizeZ;  ///< SPADIC half size in z [cm]

  const std::vector<UInt_t> fVecSpadicChannels    = {23, 7,  22, 6,  21, 19, 5,  20, 18, 4,  3,  17, 16, 2, 1,  0,
                                                  31, 30, 29, 15, 14, 28, 27, 13, 11, 26, 12, 10, 25, 9, 24, 8};
  std::map<UInt_t, UInt_t> fMapAsicChannelToElink = {};
  void FillAsicChannelToElinkMap(
    std::map<UInt_t, UInt_t>* map);  ///< Write the eLink to asicChannel mapping to the passed map

  ClassDef(CbmTrdParSpadic, 3)  // Definition of SPADIC ASIC parameters
};

#endif
