/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_BASE_TYPES_H
#define CBM_BASE_TYPES_H

#include <iostream>
#include <optional>
#include <string_view>

namespace cbm::algo
{

  template<typename T>
  struct EnumIsSerializable : std::false_type {
  };

  template<typename T, typename = std::enable_if_t<EnumIsSerializable<T>::value>>
  std::optional<T> FromString(std::string_view str);

  template<typename T, typename = std::enable_if_t<EnumIsSerializable<T>::value>>
  std::string_view ToString(T t);

  enum class Detector
  {
    STS,
    TOF,
  };

  template<>
  struct EnumIsSerializable<Detector> : std::true_type {
  };

  enum class Step
  {
    Unpack,
    DigiTrigger,
    LocalReco,
  };

  template<>
  struct EnumIsSerializable<Step> : std::true_type {
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


}  // namespace cbm::algo

// Stream operators for enums
// Must be placed in std namespace to be found by ADL e.g. for std::ostream_iterator
namespace std
{
  template<typename T, typename = enable_if_t<cbm::algo::EnumIsSerializable<T>::value>>
  std::ostream& operator<<(std::ostream& os, T t)
  {
    os << cbm::algo::ToString(t);
    return os;
  }

  template<typename T, typename = enable_if_t<cbm::algo::EnumIsSerializable<T>::value>>
  std::istream& operator>>(std::istream& is, T& t)
  {
    std::string str;
    is >> str;
    auto maybet = cbm::algo::FromString<T>(str);

    if (!maybet) throw std::invalid_argument("Could not parse " + str);
    t = *maybet;

    return is;
  }
}  // namespace std

#endif
