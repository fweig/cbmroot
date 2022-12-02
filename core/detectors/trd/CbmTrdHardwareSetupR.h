/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Florian Uhlig [committer] */

/*
 * -----
 * Purpose: This class contains the hardware mapping for asics at a given beamtime and provides the functionalities to
 * write them into the CbmTrdParAsic containers for the corresponding geometry
 * -----
 */

#ifndef CBMTRDHARDWARESETUPR_H
#define CBMTRDHARDWARESETUPR_H

#include "CbmTrdParSetAsic.h"

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t
#include <TNamed.h>      // for TNamed
#include <TString.h>     // for TString

#include <map>     // for map
#include <vector>  // for vector

#include <stdint.h>  // for size_t

enum class ECbmTrdHardwareSetupVersion : Int_t
{
  kUndefined = 0,
  kMcbm2020  = 1,
  kMcbm2020b,
  kMcbm2021,
  kMcbm2022,
  kMcbm2022WithoutHybrid,  // for v22e setup
  kMcbm2022Only1D,         // for v22f setup
  kLabIkfOneSpadic,
  kDesy2019,
  kCbm2025
};  ///< Enum for hardware setup versions, they are for example correlated to the componentId setup.


class CbmTrdHardwareSetupR : public TNamed {
public:
  CbmTrdHardwareSetupR(/* args */);
  CbmTrdHardwareSetupR(const CbmTrdHardwareSetupR&);
  CbmTrdHardwareSetupR operator=(const CbmTrdHardwareSetupR&);
  ~CbmTrdHardwareSetupR();

  size_t GetComponentId(Int_t asicAddress,
                        ECbmTrdHardwareSetupVersion
                          hwSetup);  ///< Retrieve componentId of the asic add the passed address for the passed hwSetup
  size_t GetComponentId(
    Int_t
      asicAddress);  ///< Retrieve componentId of the asic add the passed address for the currently selected ComponentIdMap
  std::map<Int_t, size_t> GetComponentIdMap() { return fComponentIdMap; }

  void SetParameterFile(TString fileName) { fParameterFileName = fileName; }
  void SetComponentIdMap(std::map<Int_t, size_t> compMap) { fComponentIdMap = compMap; }

  /**
   * @brief Create a hardware to software asic addreess translator map, with hidden parameter loading.
   * 
   * @param isLoadedParameters 
   * @return std::map<size_t, Int_t> 
  */
  std::map<size_t, Int_t> CreateHwToSwAsicAddressTranslatorMap(bool isLoadedParameters);

  /**
   * @brief Create a hardware to software asic addreess translator map
   * 
   * @param moduleparsets par container for all asics on a module 
   * @return std::map<size_t, Int_t> 
  */
  std::map<size_t, Int_t> CreateHwToSwAsicAddressTranslatorMap(CbmTrdParSetAsic* moduleparsets);

  /**
   * @brief Create a Asic Channel Map, with hidden parameter loading.
   * 
   * @param isLoadedParameters 
   * @return std::map<Int_t, std::vector<Int_t>> 
  */
  std::map<Int_t, std::vector<Int_t>> CreateAsicChannelMap(bool isLoadedParameters);

  /**
   * @brief Create a Asic Channel Map
   * 
   * @param parset 
   * @return std::map<Int_t, std::vector<Int_t>> 
  */
  std::map<Int_t, std::vector<Int_t>> CreateAsicChannelMap(CbmTrdParSetAsic* parset);

  void SelectComponentIdMap(ECbmTrdHardwareSetupVersion hwSetup);
  void SelectComponentIdMap(TString geoTag);
  bool WriteComponentIdsToParams();

private:
  /* data */
  std::map<Int_t, size_t>
    fComponentIdMap;  ///< Container for the translation betweem software asicAddress and hardware asicAddress. First: CbmTrdParAsic::fAddress, Second CbmTrdParAsic::fComponentId

  TString fParameterFileName;  ///< Name of the parameter file correlated to the hardware setup

  ClassDef(CbmTrdHardwareSetupR, 1)  // Definition of actual hardware setup
};

#endif
