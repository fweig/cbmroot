/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_UTIL_SERIALIZABLEENUM_H
#define CBM_ALGO_BASE_UTIL_SERIALIZABLEENUM_H

#include <algorithm>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <string_view>

#include <fmt/format.h>

namespace cbm::algo
{
  /**
   * @brief Helper class to serialize enums to strings and back.
   *
   * @tparam T The enum type.
   *
   * To use this class, you need to specialize EnumIsSerializable<T> and EnumStringMap<T>.
   * Used to indicate that an enum can be converted to / from a string.
   *
   * Example:
   * @code{.cpp}
   * enum class Detector {
   *  STS,
   *  TOF,
   * };
   *
   * // Set to true to indicate that the enum is serializable.
   * template<>
   * struct EnumIsSerializable<Detector> : std::true_type {};
   *
   * // Create dictionary to convert enum to string and back.
   * template<>
   * const EnumDict_t<Detector> EnumDict<Detector> = {
   *  {"sts", Detector::STS},
   *  {"tof", Detector::TOF},
   * };
   *
   * // Use it like this:
   * L_(info) << ToString(Detector::STS); // Prints "sts"
   *
   * std::optional<Detector> d = FromString<Detector>("tof"); // *d == Detector::TOF
   * std::optional<Detector> d2 = FromString<Detector>("invalid"); // d2 == std::nullopt
   * @endcode
   */
  template<typename T>
  struct EnumIsSerializable : std::false_type {};

  template<typename T>
  using EnumDict_t = std::set<std::pair<std::string_view, T>>;

  template<typename T>
  inline const EnumDict_t<T> EnumDict;

  template<typename T, typename = std::enable_if_t<EnumIsSerializable<T>::value>>
  std::optional<T> FromString(std::string_view str)
  {
    const auto& set = EnumDict<T>;
    auto it         = std::find_if(set.begin(), set.end(), [str](const auto& pair) { return pair.first == str; });
    if (it == set.end()) return std::nullopt;
    return it->second;
  }

  template<typename T, typename = std::enable_if_t<EnumIsSerializable<T>::value>>
  std::string_view ToString(T t)
  {
    const auto& set = EnumDict<T>;
    auto it         = std::find_if(set.begin(), set.end(), [t](const auto& pair) { return pair.second == t; });
    if (it == set.end()) throw std::runtime_error(fmt::format("Entry {} for enum missing!", static_cast<int>(t)));
    return it->first;
  }
}  // namespace cbm::algo

// Stream operators for enums
// Placed in global namespace to be found by ADL e.g. for std::ostream_iterator
namespace std
{
  template<typename T, typename = std::enable_if_t<cbm::algo::EnumIsSerializable<T>::value>>
  std::ostream& operator<<(std::ostream& os, T t)
  {
    os << cbm::algo::ToString(t);
    return os;
  }

  template<typename T, typename = std::enable_if_t<cbm::algo::EnumIsSerializable<T>::value>>
  std::istream& operator>>(std::istream& is, T& t)
  {
    std::string str;
    is >> str;
    auto maybet = cbm::algo::FromString<T>(str);

    if (!maybet) throw std::invalid_argument("Could not parse " + str + " as Enum");
    t = *maybet;

    return is;
  }
}  // namespace std

#endif  //CBM_ALGO_BASE_UTIL_SERIALIZABLEENUM_H
