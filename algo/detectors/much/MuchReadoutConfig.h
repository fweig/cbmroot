/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----            MuchReadoutConfig header file                       -----
// -----            Created 22/02/22  by P.-A. Loizeau                 -----
// -----            Modified 07/12/18  by A Kumar                      -----
// -------------------------------------------------------------------------

#ifndef ALGO_DETECTORS_MUCH_MUCHREADOUTCONFIG_H
#define ALGO_DETECTORS_MUCH_MUCHREADOUTCONFIG_H

#include <cstdint>
#include <map>
#include <vector>

namespace cbm::algo
{
  class MuchReadoutConfig {

  public:
    /** @brief Constructor **/
    MuchReadoutConfig();

    /** @brief Destructor **/
    virtual ~MuchReadoutConfig();

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
     ** @return Vector of MUCH addresses, indexed via channel number
     */
    std::vector<uint32_t> Map(uint16_t equipId, uint16_t elink);

    uint16_t GetNrOfCrobs() { return numComp * numCrobPerComp; }
    uint16_t GetNrOfFebs() { return GetNrOfCrobs() * kuNbFebsPerCrob; }
    uint16_t GetNrOfFebsInGemA() { return fuFebsInGemA; }
    uint16_t GetNrOfFebsInGemB() { return fuFebsInGemB; }

    int32_t GetFebId(uint16_t);

    int8_t GetPadXA(uint8_t febid, uint8_t channelid);
    int8_t GetPadYA(uint8_t febid, uint8_t channelid);
    int8_t GetPadXB(uint8_t febid, uint8_t channelid);
    int8_t GetPadYB(uint8_t febid, uint8_t channelid);
    double GetRealX(int16_t);
    double GetRealPadSize(int16_t);

    double GetRealX(int16_t Channel, int16_t Sector);
    double GetRealPadSize(int16_t Channel, int16_t Sector);

    //RPC Module Related Functions
    uint16_t GetNrOfFebsInRpc() { return fuFebsInRpc; }
    int8_t GetPadXRpc(uint8_t febid, uint8_t channelid);
    int8_t GetPadYRpc(uint8_t febid, uint8_t channelid);

    bool IsCrobActive(uint16_t uDpbIdx, uint16_t uCrobIdx);
    bool IsFebActive(uint16_t uFebInSystIdx);
    bool IsFebActive(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);
    bool IsFebPulser(uint16_t uFebInSystIdx);
    bool IsFebPulser(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);
    double GetFebAdcGain(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);
    double GetFebAdcOffset(uint16_t uDpbIdx, uint16_t uCrobIdx, uint16_t uFebIdx);

  private:
    // --- MUCH readout map
    // --- Map index: (equipment, elink, channel), map value: (MUCH address)
    std::map<uint16_t, std::vector<std::vector<uint32_t>>> fReadoutMap = {};

    /** @brief Initialisation of readout map **/
    void Init();

    /** @brief Maps component index, Fed Id and channel number to Much Address **/
    uint32_t CreateMuchAddress(uint32_t dpbidx, int32_t iFebId, uint32_t usChan);

    /// Constants
    static const uint16_t numCrobPerComp   = 1;    // Number of CROBs possible per DPB
    static const uint16_t numElinksPerCrob = 42;   // Number of elinks in each CROB ?
    static const uint16_t kuNbFebsPerCrob  = 9;    // Number of FEBs  connected to each CROB for mMuch 2019
    static const uint16_t kuNbAsicsPerFeb  = 1;    // Number of ASICs connected in each FEB for MUCH
    static const uint16_t numChanPerAsic   = 128;  // Number of channels in each ASIC

    /// Variables
    uint16_t numComp = 6;  // Total number of MUCH DPBs in system

    std::vector<int16_t> fiCrobActiveFlag;    // Array to hold the active flag for all CROBs, [ NbDpb * kuNbCrobPerDpb ]
    uint16_t fuFebsInGemA = 0;                // Number of FEBs connected in GEM Module A
    uint16_t fuFebsInGemB = 0;                // Number of FEBs connected in GEM Module B
    uint16_t fuFebsInRpc  = 0;                // Number of FEBs connected in RPC Module
    std::vector<int16_t> fnFebsIdsArrayGemA;  // Array to hold FEB IDs connected to GEM Module A
    std::vector<int16_t> fnFebsIdsArrayGemB;  // Array to hold FEB IDs connected to GEM Module B
    std::vector<int16_t> fnFebsIdsArrayRpc;   // Array to hold FEB IDs connected to RPC Module
    std::vector<int16_t> fChannelsToPadXA;  // Array which stores the corresponding x position of PAD of entire module A
    std::vector<int16_t> fChannelsToPadYA;  // Array which stores the corresponding y position of PAD of entire module A
    std::vector<int16_t> fChannelsToPadXB;  // Array which stores the corresponding x position of PAD of entire module B
    std::vector<int16_t> fChannelsToPadYB;  // Array which stores the corresponding y position of PAD of entire module B
    std::vector<int16_t> fChannelsToPadXRpc;  // Array which stores the corresponding x position of PAD of RPC module
    std::vector<int16_t> fChannelsToPadYRpc;  // Array which stores the corresponding y position of PAD of RPC module
    std::vector<double> fRealX;               // Array which stores the Real X (starting 18.733 cm) position of PAD
    std::vector<double> fRealPadSize;  // Array which stores the Real Progressive size of each padX (starting .327 cm )
  };

} /* namespace cbm::algo */

#endif  // ALGO_DETECTORS_MUCH_MUCHREADOUTCONFIG_H
