/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

// -------------------------------------------------------------------------
// -----            T0ReadoutConfig header file                       -----
// -----            Created 22/02/22  by P.-A. Loizeau                 -----
// -----            Modified 07/12/18  by A Kumar                      -----
// -------------------------------------------------------------------------

#ifndef ALGO_DETECTORS_T0_T0READOUTCONFIG_H
#define ALGO_DETECTORS_T0_T0READOUTCONFIG_H

#include <cstdint>
#include <map>
#include <vector>

namespace cbm::algo
{
  class T0ReadoutConfig {

  public:
    /** @brief Constructor **/
    T0ReadoutConfig();

    /** @brief Destructor **/
    virtual ~T0ReadoutConfig();

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
    // --- T0 readout map
    // --- Map index: (equipment, elink, channel), map value: (TOF address)
    std::map<uint16_t, std::vector<std::vector<uint32_t>>> fReadoutMap = {};

    /** @brief Initialisation of readout map **/
    void Init();

    /// Constants
    /// Taken from mBmonCriPar.par
    static const uint16_t numComp        = 4;   // Total number of TOF DPBs in system
    static const uint32_t numFebsPerComp = 10;  // Number of FEEs which are connected to one GDPB
    static const uint32_t numAsicsPerFeb = 8;   // Number of ASICs connected in each FEB for TOF
    static const uint32_t numChanPerAsic = 4;   // Number of channels in each ASIC
    static const uint32_t numCrob        = 8;   // Total number of Gbtx links

    /// Taken from CbmMcbm2018TofPar.h
    static const uint32_t numFebsPerCrob   = 5;  // Number of FEBs  connected to each CROB for mTof 2019
    static const uint32_t numElinksPerCrob = numAsicsPerFeb * numFebsPerCrob;

    // Module Identifier connected to Gbtx link, has to match geometry
    const int32_t moduleId[numCrob] = {0, -1, 0, -1, 0, -1, 0, -1};

    // type of Rpcs connected to Gbtx link
    const int32_t rpcType[numCrob] = {99, -1, 99, -1, 99, -1, 99, -1};

    // side of Rpcs connected to Gbtx link, i.e. 0 or 1
    const int32_t rpcSide[numCrob] = {0, 0, 0, 0, 0, 0, 0, 0};

    std::vector<int32_t> fviRpcChUId = {};  // UID/address for each channel, build from type, side and module

    void BuildChannelsUidMap();
    void BuildChannelsUidMapT0(uint32_t& uCh, uint32_t uGbtx);
    void BuildChannelsUidMapT0_2022(uint32_t& uCh, uint32_t uGbtx);
  };

} /* namespace cbm::algo */

#endif  //ALGO_DETECTORS_T0_T0READOUTCONFIG_H
