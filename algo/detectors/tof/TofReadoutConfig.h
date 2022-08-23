/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#ifndef ALGO_DETECTORS_TOF_TOFREADOUTCONFIG_H
#define ALGO_DETECTORS_TOF_TOFREADOUTCONFIG_H

#include <cstdint>
#include <map>
#include <vector>

namespace cbm::algo
{
  class TofReadoutConfig {

  public:
    /** @brief Constructor **/
    TofReadoutConfig();

    /** @brief Destructor **/
    virtual ~TofReadoutConfig();

    /** @brief Equipment in the configuration
     ** @return Vector of equipment IDs
     **/
    std::vector<uint16_t> GetEquipmentIds();

    /** @brief Number of elinks of a component
     ** @param Equipment ID
     ** @return Number of elinks
     **/
    size_t GetNumElinks(uint16_t equipmentId);

    /** @brief API: Mapping from component and elink to addresses per channel
     ** @param equipId     Equipment identifier (component)
     ** @param elink       Elink number within component
     ** @return Vector of TOF addresses, indexed via channel number
     */
    std::vector<uint32_t> Map(uint16_t equipId, uint16_t elink);

  private:
    // --- TOF readout map
    // --- Map index: (equipment, elink, channel), map value: (TOF address)
    std::map<uint16_t, std::vector<std::vector<uint32_t>>> fReadoutMap = {};

    /** @brief Initialisation of readout map **/
    void Init();

    /// Mapping to eLink to ASIC number within DPB. Mapping is the same for each DPB.
    int32_t ElinkIdxToGet4Idx(uint32_t elink);
    int32_t Get4ChanToPadiChan(uint32_t channelInFee);

    /// Constants

    /// Taken from mTofCriParNickel_withBmon.par
    static const uint16_t numComp        = 8;   // Total number of TOF DPBs in system
    static const uint32_t numFebsPerComp = 10;  // Number of FEEs which are connected to one GDPB
    static const uint32_t numAsicsPerFeb = 8;   // Number of ASICs connected in each FEB for TOF
    static const uint32_t numChanPerAsic = 4;   // Number of channels in each ASIC
    static const uint32_t numCrob        = 16;  // Total number of Gbtx links

    // Module Identifier connected to Gbtx link, has to match geometry
    const int32_t moduleId[numCrob] = {0, 0, 1, 1, 0, 0, 2, 2, 3, 3, 4, 4, 0 - 1, 0, 0};

    // type of Rpcs connected to Gbtx link
    const int32_t rpcType[numCrob] = {0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 9, 9, 6, 9};

    // side of Rpcs connected to Gbtx link, i.e. 0 or 1
    const int32_t rpcSide[numCrob] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 8, 0};

    // number of Rpcs connected to Gbtx link, i.e. 3 or 5
    const int32_t numRpc[numCrob] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

    //// Taken from CbmMcbm2018TofPar.h  (fixed numCrobPerComp)
    static const uint32_t numCrobPerComp   = 2;  // Number of CROBs possible per DPB
    static const uint32_t numFebsPerCrob   = 5;  // Number of FEBs  connected to each CROB for mTof 2019
    static const uint32_t numChanPerFeb    = numChanPerAsic * numAsicsPerFeb;
    static const uint32_t numElinksPerCrob = numAsicsPerFeb * numFebsPerCrob;
    static const uint32_t numElinksPerComp = numElinksPerCrob * numCrobPerComp;

    // Mapping to eLink to ASIC number within CROB. Mapping is the same for each CROB.
    const uint32_t elink2Asic[numElinksPerCrob] = {27, 2,  7,  3,  31, 26, 30, 1,  33, 37, 32, 13, 9,  14,
                                                   10, 15, 17, 21, 16, 35, 34, 38, 25, 24, 0,  6,  20, 23,
                                                   18, 22, 28, 4,  29, 5,  19, 36, 39, 8,  12, 11};
    // Mapping in Readout chain PCBs
    const uint32_t asic2PadI[numChanPerFeb] = {
      3,  2,  1,  0,  7,  6,  5,  4,  11, 10, 9,  8,  15, 14, 13, 12,
      19, 18, 17, 16, 23, 22, 21, 20, 27, 26, 25, 24, 31, 30, 29, 28};  //! Map from GET4 channel to PADI channel

    std::vector<int32_t> fviRpcChUId = {};  // UID/address for each channel, build from type, side and module

    void BuildChannelsUidMap();
    void BuildChannelsUidMapCbm(uint32_t& uCh, uint32_t uGbtx);
    void BuildChannelsUidMapStar(uint32_t& uCh, uint32_t uGbtx);
    void BuildChannelsUidMapCern(uint32_t& uCh, uint32_t uGbtx);
    void BuildChannelsUidMapCera(uint32_t& uCh, uint32_t uGbtx);
    void BuildChannelsUidMapStar2(uint32_t& uCh, uint32_t uGbtx);
    void BuildChannelsUidMapBuc(uint32_t& uCh, uint32_t uGbtx);
  };

} /* namespace cbm::algo */

#endif  //ALGO_DETECTORS_TOF_TOFREADOUTCONFIG_H
