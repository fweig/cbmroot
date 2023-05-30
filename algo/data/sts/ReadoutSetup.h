/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Felix Weiglhofer */
#ifndef CBM_ALGO_DATA_STS_READOUT_H
#define CBM_ALGO_DATA_STS_READOUT_H

#include <map>
#include <string>
#include <vector>

#include "Prelude.h"
#include "config/Property.h"

namespace cbm::algo::sts
{

  /**
   * @brief Readout setup / Hardware cabling for STS
   * Used to create the hardware mapping for the STS unpacker.
   */
  struct ReadoutSetup {

    struct Module {
      i32 address;
      i32 type;

      static constexpr auto Properties = std::make_tuple(
        config::Property(&Module::address, "address", "HW address of module", YAML::Hex),
        config::Property(&Module::type, "type",
                         "Type 0 had the connector at the right side, type 1 at the left side. For type 0, the mapping "
                         "of FEB to module side as above applies, for type 1, it has to be inverted."));
    };

    struct Component {
      u16 equipmentId;
      std::vector<std::vector<i16>> feb2module;
      std::vector<std::vector<i16>> feb2moduleSide;
      std::vector<std::vector<bool>> febIsPulser;

      static constexpr auto Properties = std::make_tuple(
        config::Property(&Component::equipmentId, "equipmentId",
                         "Equipment ID of component. Written to the data stream (MicrosliceDescriptor).", YAML::Hex),
        config::Property(&Component::feb2module, "feb2module",
                         "Mapping of FEB within CROB to module index (-1 = inactive)"),
        config::Property(&Component::feb2moduleSide, "feb2moduleSide",
                         "Mapping of FEB within CROB to module side (0 = left, 1 = right)"),
        config::Property(&Component::febIsPulser, "febIsPulser", "Flag if FEB is pulser (true) or not (false)"));
    };

    struct Elink {
      i16 toFeb;
      u32 toAsicFebA;
      u32 toAsicFebB;

      static constexpr auto Properties = std::make_tuple(
        config::Property(&Elink::toFeb, "toFeb", "Mapping of elink to FEB within CROB (-1 = inactive)"),
        config::Property(&Elink::toAsicFebA, "toAsicFebA", "Mapping of eLink to ASIC for FEB Type A", YAML::Hex),
        config::Property(&Elink::toAsicFebB, "toAsicFebB", "Mapping of eLink to ASIC for FEB Type B", YAML::Hex));
    };

    u16 numAsicsPerFeb;
    std::vector<Module> modules;
    std::vector<Component> components;
    std::vector<Elink> elinks;

    static constexpr auto Properties =
      std::make_tuple(config::Property(&ReadoutSetup::numAsicsPerFeb, "numAsicsPerFeb", "Number of ASICs per FEB"),
                      config::Property(&ReadoutSetup::modules, "modules", "Modules", {}, YAML::Flow),
                      config::Property(&ReadoutSetup::components, "components", "Components", {}, YAML::Flow),
                      config::Property(&ReadoutSetup::elinks, "elinks", "Elinks", {}, YAML::Flow));
  };

  /** @class ReadoutMapping
  ** @author Volker Friese <v.friese@gsi.de>
  ** @since 3 March 2022
  ** @brief Provides the hardware-to-software address mapping for the CBM-STS
  **
  ** The hardware address as provided in the raw data stream is specified in terms of the
  ** equipment identifier (specific to one FLES component) and the elink number with in
  ** component. This is to be translated into the module address and the ASIC number within
  ** the module.
  **/
  class ReadoutMapping {

  public:
    struct Entry {
      i32 moduleAddress;
      u16 asicNumber;
      bool pulser;
    };

    /** @brief Empty mapping **/
    ReadoutMapping() = default;

    /** @brief Constructor **/
    ReadoutMapping(const ReadoutSetup&);

    /** @brief Equipment in the configuration
   ** @return Vector of equipment IDs
      **/
    std::vector<u16> GetEquipmentIds();

    /** @brief Number of elinks of a component
   ** @param Equipment ID
      ** @return Number of elinks
      **/
    size_t GetNumElinks(u16 equipmentId);

    /** @brief Total number of elinks for STS
     ** @return Number of elinks
     **/
    size_t GetNumElinks();

    /** @brief API: Mapping from component and elink to address / ASIC number + pulser flag
      ** @param equipId     Equipment identifier (component)
      ** @param elink       Elink number within component
      ** @return (module address, ASIC number within module)
      */
    Entry Map(u16 equipId, u16 elink);

    /** @brief Debug output of readout map **/
    std::string PrintReadoutMap();

  private:
    // --- STS readout map
    // --- Map index: (equipment, elink)
    std::map<u16, std::vector<Entry>> fReadoutMapping = {};  //!

    /** @brief Initialisation of readout map **/
    void Init(const ReadoutSetup&);
  };

}  // namespace cbm::algo::sts

#endif  // CBM_ALGO_DATA_STS_READOUT_H
