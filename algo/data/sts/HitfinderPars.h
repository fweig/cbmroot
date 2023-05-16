/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_DATA_STS_HITFINDERPARS_H
#define CBM_ALGO_DATA_STS_HITFINDERPARS_H

#include "LandauTable.h"
#include "Prelude.h"
#include "config/Property.h"

namespace cbm::algo::sts
{
  struct HitfinderPars {
    struct Asic {
      int nAdc;
      float dynamicRange;
      float threshold;
      float timeResolution;
      float deadTime;
      float noise;
      float zeroNoiseRate;

      XPU_D float AdcToCharge(unsigned short adc) const
      {
        return threshold + dynamicRange / float(nAdc) * (float(adc) + 0.5f);
      }

      static constexpr auto Properties = std::make_tuple(
        config::Property(&Asic::nAdc, "nAdc"), config::Property(&Asic::dynamicRange, "dynamicRange"),
        config::Property(&Asic::threshold, "threshold"), config::Property(&Asic::timeResolution, "timeResolution"),
        config::Property(&Asic::deadTime, "deadTime"), config::Property(&Asic::noise, "noise"),
        config::Property(&Asic::zeroNoiseRate, "zeroNoiseRate"));
    };

    struct ModuleTransform {
      // Rotation + translation matrix to transform
      // local module coordinates into global coordinate system.
      // No need for fancy math types here. These values are just copied
      // and moved to the GPU.
      // TODO: thats a lie, should use glm::mat3x4
      std::array<float, 9> rotation;  // 3x3 matrix
      std::array<float, 3> translation;

      static constexpr auto Properties = std::make_tuple(
        config::Property(&ModuleTransform::rotation, "rotation", "Rotation matrix", YAML::Flow),
        config::Property(&ModuleTransform::translation, "translation", "Translation vector", YAML::Flow));
    };

    struct Module {
      int32_t address;
      float dY;
      float pitch;
      float stereoF;
      float stereoB;
      float lorentzF;
      float lorentzB;
      ModuleTransform localToGlobal;

      static constexpr auto Properties = std::make_tuple(
        config::Property(&Module::address, "address", "Hardware Address", YAML::Hex),
        config::Property(&Module::dY, "dY"), config::Property(&Module::pitch, "pitch"),
        config::Property(&Module::stereoF, "stereoF"), config::Property(&Module::stereoB, "stereoB"),
        config::Property(&Module::lorentzF, "lorentzF"), config::Property(&Module::lorentzB, "lorentzB"),
        config::Property(&Module::localToGlobal, "localToGlobal"));
    };

    Asic asic;
    int nChannels;
    std::vector<Module> modules;
    LandauTable landauTable;  // Landau table for hitfinder, read from a seperate file

    static constexpr auto Properties = std::make_tuple(
      config::Property(&HitfinderPars::asic, "asic",
                       "Asic definitions. Currently assumes same parameters for all asics."),
      config::Property(&HitfinderPars::nChannels, "nChannels",
                       "Total number of channels per module. Hitfinder assumes nChannels / 2 channels per side."),
      config::Property(&HitfinderPars::modules, "modules"));
  };
}  // namespace cbm::algo::sts

#endif
