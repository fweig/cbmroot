/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Dominik Smith [committer] */

#ifndef ALGO_DETECTORS_TRD2D_TRD2DREADOUTCONFIG_H
#define ALGO_DETECTORS_TRD2D_TRD2DREADOUTCONFIG_H

#include <map>
#include <sstream>
#include <utility>
#include <vector>

#include "UnpackTrd2d.h"

namespace cbm::algo
{


  /** @class Trd2dReadoutConfig
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 3 March 2022
   ** @brief Provides the hardware-to-software address mapping for the CBM-TRD2D
   **
   ** The hardware address as provided in the raw data stream is specified in terms of the
   ** equipment identifier (specific to one FLES component) and the elink number with in
   ** component. This is to be translated into the module address and the ASIC number within
   ** the module.
   ** The mapping of the two address spaces is hard-coded in this class.
   **/

  class Trd2dReadoutConfig {

  public:
    /** @brief Constructor **/
    Trd2dReadoutConfig();


    /** @brief Destructor **/
    virtual ~Trd2dReadoutConfig();


    /** @brief Equipment in the configuration
     ** @return Vector of equipment IDs
     **/
    std::vector<uint16_t> GetEquipmentIds();


    /** @brief Number of ASICS of a component
     ** @param Equipment ID
     ** @return Number of ASICS
     **/
    size_t GetNumAsics(uint16_t equipmentId);


    /** @brief Number of channels of a component - ASIC pair
     ** @param Equipment ID
     ** @param ASIC ID
     ** @return Number of channels
     **/
    size_t GetNumChans(uint16_t equipmentId, uint16_t asicId);

    //// TO DO: Check uint sizes (for asic Id etc).

    /** @brief API: Mapping from component to pair (module id, crob id)
     ** @param equipId     Equipment identifier (component)
     ** @return pair (module id, crob id)
     */
    std::pair<uint16_t, uint8_t> CompMap(uint16_t equipId);


    /** @brief API: Mapping from component, asic and channel to tuple (pad address, R pairing flag, daq offset)
     ** @param equipId     Equipment identifier (component)
     ** @param asic        ASIC number within component
     ** @param channel     Channel number within CROB
     ** @return tuple (pad address, R pairing flag, daq offset)
     */
    std::tuple<int32_t, bool, uint64_t> ChanMap(uint16_t equipId, uint16_t asic, uint16_t chan);


    /** @brief Debug output of readout map **/
    std::string PrintReadoutMap();

    /** @brief Initialisation of readout map **/
    void InitComponentMap(const std::map<uint32_t, uint16_t[NCROBMOD]>& crob_map);

    /** @brief Initialisation of channel map **/
    void InitChannelMap(
      const std::map<size_t, std::map<size_t, std::map<size_t, std::tuple<int32_t, bool, uint64_t>>>>& channelMap);

  private:
    // --- TRD2D readout map
    // --- Map index: (equipment), map value: (module id, crob id)
    std::map<uint16_t, std::pair<uint16_t, uint8_t>> fReadoutMap = {};  //!

    // --- TRD2D channel map
    // --- Map index: (equipment, asic, chan), map value: (pad address, R pairing flag, daq offset)
    std::map<uint16_t, std::vector<std::vector<std::tuple<int32_t, bool, uint64_t>>>> fChannelMap = {};  //!
  };

} /* namespace cbm::algo */

#endif /* ALGO_DETECTORS_TRD2D_TRD2DREADOUTCONFIG_H_ */
