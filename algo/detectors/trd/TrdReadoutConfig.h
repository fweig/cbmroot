/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef ALGO_DETECTORS_TRD_TRDREADOUTCONFIG_H
#define ALGO_DETECTORS_TRD_TRDREADOUTCONFIG_H

#include <map>
#include <sstream>
#include <utility>
#include <vector>

namespace cbm::algo
{


  /** @class TrdReadoutConfig
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 3 March 2022
   ** @brief Provides the hardware-to-software address mapping for the CBM-TRD
   **
   ** The hardware address as provided in the raw data stream is specified in terms of the
   ** equipment identifier (specific to one FLES component) and the elink number with in
   ** component. This is to be translated into the module address and the ASIC number within
   ** the module.
   ** The mapping of the two address spaces is hard-coded in this class.
   **/

  class TrdReadoutConfig {

  public:
    /** @brief Constructor **/
    TrdReadoutConfig();


    /** @brief Destructor **/
    virtual ~TrdReadoutConfig();


    /** @brief Equipment in the configuration
     ** @return Vector of equipment IDs
     **/
    std::vector<uint16_t> GetEquipmentIds();


    /** @brief Number of CROBS of a component
     ** @param Equipment ID
     ** @return Number of CROBS
     **/
    size_t GetNumCrobs(uint16_t equipmentId);


    /** @brief Number of elinks of a component - CROB pair
     ** @param Equipment ID
     ** @param CROB ID
     ** @return Number of elinks
     **/
    size_t GetNumElinks(uint16_t equipmentId, uint16_t crobId);


    /** @brief API: Mapping from component, crob and elink to pair (ASIC address, channel addresses)
     ** @param equipId     Equipment identifier (component)
     ** @param crob        CROB number within component
     ** @param elink       Elink number within CROB
     ** @return pair (ASIC address, channel addresses )
     */
    std::pair<int32_t, std::vector<uint32_t>> Map(uint16_t equipId, uint16_t crob, uint16_t elink);


    /** @brief Debug output of readout map **/
    std::string PrintReadoutMap();


    /** @brief Initialisation of readout map **/
    void Init(const std::map<size_t, std::map<size_t, std::map<size_t, size_t>>>& addressMap,
              std::map<size_t, std::map<size_t, std::map<size_t, std::map<size_t, size_t>>>>& channelMap);

  private:
    // --- TRD readout map
    // --- Map index: (equipment, crob, elink), map value: (ASIC address)
    std::map<uint16_t, std::vector<std::vector<uint16_t>>> fReadoutMap = {};  //!

    // --- TRD channel map
    // --- Map index: (equipment, crob, elink, chan), map value: (channel address)
    std::map<uint16_t, std::vector<std::vector<std::vector<uint32_t>>>> fChannelMap = {};  //!
  };

} /* namespace cbm::algo */

#endif /* ALGO_DETECTORS_TRD_TRDREADOUTCONFIG_H_ */
