/*
 * File: /CbmTrdHardwareSetupR.h
 * Created Date: Thursday March 5th 2020
 * Author: Pascal Raisig -- praisig@ikf.uni-frankfurt.de
 * -----
 * Last Modified: Saturday March 21st 2020 14:30:16
 * Modified By: Pascal Raisig
 * -----
 * Purpose: This class contains the hardware mapping for asics at a given beamtime and provides the functionalities to
 * write them into the CbmTrdParAsic containers for the corresponding geometry
 * -----
 */

#ifndef CBMTRDHARDWARESETUPR_H
#define CBMTRDHARDWARESETUPR_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t
#include <TNamed.h>      // for TNamed
#include <TString.h>     // for TString

#include <stdint.h>      // for uint64_t
#include <map>           // for map
#include <vector>        // for vector

enum class ECbmTrdHardwareSetupVersion : Int_t
{
    kUndefined = 0
    , kMcbm2020  = 1
    , kLabIkfOneSpadic
    , kCbm2025
}; ///< Enum for hardware setup versions, they are for example correlated to the componentId setup.


class CbmTrdHardwareSetupR : public TNamed
{
public:
    CbmTrdHardwareSetupR(/* args */);
    CbmTrdHardwareSetupR(const CbmTrdHardwareSetupR&);
    CbmTrdHardwareSetupR operator=(const CbmTrdHardwareSetupR&);
    ~CbmTrdHardwareSetupR();

    std::uint64_t GetComponentId(Int_t asicAddress, ECbmTrdHardwareSetupVersion hwSetup); ///< Retrieve componentId of the asic add the passed address for the passed hwSetup 
    std::uint64_t GetComponentId(Int_t asicAddress); ///< Retrieve componentId of the asic add the passed address for the currently selected ComponentIdMap
    std::map<Int_t, std::uint64_t> GetComponentIdMap()              {return fComponentIdMap;}

    void SetParameterFile(TString fileName)                 {fParameterFileName = fileName;}
    void SetComponentIdMap(std::map<Int_t, std::uint64_t> compMap)  {fComponentIdMap = compMap;}
    
    std::map<std::uint64_t, Int_t> CreateHwToSwAsicAddressTranslatorMap(bool isLoadedParameters);
    std::map<Int_t, std::vector<Int_t>> CreateAsicChannelMap(bool isLoadedParameters);
    void SelectComponentIdMap(ECbmTrdHardwareSetupVersion hwSetup);
    bool WriteComponentIdsToParams();

private:
    /* data */
    std::map<Int_t, uint64_t> fComponentIdMap; ///< Container for the translation betweem software asicAddress and hardware asicAddress. First: CbmTrdParAsic::fAddress, Second CbmTrdParAsic::fComponentId
    
    TString fParameterFileName;                   ///< Name of the parameter file correlated to the hardware setup

    ClassDef(CbmTrdHardwareSetupR, 1) // Definition of actual hardware setup
};

#endif
