/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_BASE_TYPES_H
#define CBM_BASE_TYPES_H

#include "util/SerializableEnum.h"

namespace cbm::algo
{

  enum class Detector
  {
    STS,
    TOF,
  };

  template<>
  struct EnumIsSerializable<Detector> : std::true_type {};

  template<>
  const EnumDict_t<Detector> EnumDict<Detector> = {
    {"STS", Detector::STS},
    {"TOF", Detector::TOF},
  };

  enum class Step
  {
    Unpack,
    DigiTrigger,
    LocalReco,
  };

  template<>
  struct EnumIsSerializable<Step> : std::true_type {};

  template<>
  const EnumDict_t<Step> EnumDict<Step> = {
    {"Unpack", Step::Unpack},
    {"Digitrigger", Step::DigiTrigger},
    {"Localreco", Step::LocalReco},
  };

  enum class RecoData
  {
    Digi,
    Cluster,
    Hit,
  };

  template<>
  struct EnumIsSerializable<RecoData> : std::true_type {
  };

  template<>
  const EnumDict_t<RecoData> EnumDict<RecoData> = {
    {"Digi", RecoData::Digi},
    {"Cluster", RecoData::Cluster},
    {"Hit", RecoData::Hit},
  };

}  // namespace cbm::algo


#endif
