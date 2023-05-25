/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#include "Types.h"

#include <algorithm>
#include <set>
#include <stdexcept>

#include <fmt/format.h>

using namespace cbm::algo;

namespace
{
  static const std::set<std::pair<std::string_view, Detector>> kDetectorString {
    {"sts", Detector::STS},
    {"tof", Detector::TOF},
  };

  static const std::set<std::pair<std::string_view, Step>> kRecoStepString {
    {"unpack", Step::Unpack},
    {"digitrigger", Step::DigiTrigger},
    {"localreco", Step::LocalReco},
  };

  static const std::set<std::pair<std::string_view, RecoData>> kRecoDataString {
    {"digi", RecoData::Digi},
    {"cluster", RecoData::Cluster},
    {"hit", RecoData::Hit},
  };

  template<typename T>
  std::optional<T> EnumFromString(std::string_view str, std::set<std::pair<std::string_view, T>> set)
  {
    auto it = std::find_if(set.begin(), set.end(), [str](const auto& pair) { return pair.first == str; });
    if (it == set.end()) return std::nullopt;
    return it->second;
  }

  template<typename T>
  std::string_view EnumToString(T t, std::set<std::pair<std::string_view, T>> set)
  {
    auto it = std::find_if(set.begin(), set.end(), [t](const auto& pair) { return pair.second == t; });
    if (it == set.end()) throw std::runtime_error(fmt::format("Entry {} for enum missing!", static_cast<int>(t)));
    return it->first;
  }
}  // namespace

template<>
std::optional<Step> cbm::algo::FromString(std::string_view str)
{
  return EnumFromString(str, kRecoStepString);
}

template<>
std::string_view cbm::algo::ToString(Step step)
{
  return EnumToString(step, kRecoStepString);
}

template<>
std::optional<Detector> cbm::algo::FromString(std::string_view str)
{
  return EnumFromString(str, kDetectorString);
}

template<>
std::string_view cbm::algo::ToString(Detector det)
{
  return EnumToString(det, kDetectorString);
}

template<>
std::optional<RecoData> cbm::algo::FromString(std::string_view str)
{
  return EnumFromString(str, kRecoDataString);
}

template<>
std::string_view cbm::algo::ToString(RecoData data)
{
  return EnumToString(data, kRecoDataString);
}
