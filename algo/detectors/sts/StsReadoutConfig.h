/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef ALGO_DETECTORS_STS_STSREADOUTCONFIG_H
#define ALGO_DETECTORS_STS_STSREADOUTCONFIG_H

#include <map>
#include <sstream>
#include <utility>
#include <vector>

namespace cbm::algo
{


  /** @class StsReadoutConfig
   ** @author Volker Friese <v.friese@gsi.de>
   ** @since 3 March 2022
   ** @brief Provides the hardware-to-software address mapping for the CBM-STS
   **
   ** The hardware address as provided in the raw data stream is specified in terms of the
   ** equipment identifier (specific to one FLES component) and the elink number with in
   ** component. This is to be translated into the module address and the ASIC number within
   ** the module.
   ** The mapping of the two address spaces is hard-coded in this class.
   **/

  class StsReadoutConfig {

  public:
    /** @brief Constructor **/
    StsReadoutConfig();


    /** @brief Destructor **/
    virtual ~StsReadoutConfig();


    /** @brief Equipment in the configuration
     ** @return Vector of equipment IDs
     **/
    std::vector<uint16_t> GetEquipmentIds();


    /** @brief Number of elinks of a component
     ** @param Equipment ID
     ** @return Number of elinks
     **/
    size_t GetNumElinks(uint16_t equipmentId);


    /** @brief API: Mapping from component and elink to address and ASIC number
     ** @param equipId     Equipment identifier (component)
     ** @param elink       Elink number within component
     ** @return (module address, ASIC number within module)
     */
    std::pair<int32_t, uint16_t> Map(uint16_t equipId, uint16_t elink);


    /** @brief Debug output of readout map **/
    std::string PrintReadoutMap();


  private:
    // --- STS readout map
    // --- Map index: (equipment, elink), map value: (module address, ASIC number in module)
    std::map<uint16_t, std::vector<std::pair<int32_t, uint16_t>>> fReadoutMap = {};  //!


    /** @brief Initialisation of readout map **/
    void Init();
  };

} /* namespace cbm::algo */

#endif /* ALGO_DETECTORS_STS_STSREADOUTCONFIG_H_ */
