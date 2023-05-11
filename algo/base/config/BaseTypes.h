/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_CONFIG_BASETYPES_H
#define CBM_ALGO_BASE_CONFIG_BASETYPES_H

#include <string>
#include <tuple>
#include <vector>

#include "Prelude.h"

namespace cbm::algo::config
{

  using BaseTypes = std::tuple<bool, u8, i8, u16, i16, u32, i32, u64, i64, f32, f64, std::string>;

  template<typename T, typename Tuple>
  struct has_type;

  template<typename T, typename... Us>
  struct has_type<T, std::tuple<Us...>> : std::disjunction<std::is_same<T, Us>...> {
  };

  template<typename T>
  constexpr bool IsBaseType = has_type<T, BaseTypes>::value;

  template<typename T>
  constexpr bool IsEnum = std::is_enum_v<T>;

  template<typename T, typename = std::enable_if_t<IsBaseType<T>>>
  constexpr std::string_view BaseTypeToStr()
  {
    if constexpr (std::is_same_v<bool, T>) { return "bool"; }
    else if constexpr (std::is_same_v<u8, T>) {
      return "u8";
    }
    else if constexpr (std::is_same_v<i8, T>) {
      return "i8";
    }
    else if constexpr (std::is_same_v<u16, T>) {
      return "u16";
    }
    else if constexpr (std::is_same_v<i16, T>) {
      return "i16";
    }
    else if constexpr (std::is_same_v<u32, T>) {
      return "u32";
    }
    else if constexpr (std::is_same_v<i32, T>) {
      return "i32";
    }
    else if constexpr (std::is_same_v<float, T>) {
      return "float";
    }
    else if constexpr (std::is_same_v<std::string, T>) {
      return "string";
    }
    else {
      return "unknown";
    }
  }

  template<typename>
  struct is_std_vector : std::false_type {
  };

  template<typename T, typename A>
  struct is_std_vector<std::vector<T, A>> : std::true_type {
  };

  template<typename T>
  constexpr bool IsVector = is_std_vector<T>::value;

  template<typename>
  struct is_std_array : std::false_type {
  };

  template<typename T, std::size_t N>
  struct is_std_array<std::array<T, N>> : std::true_type {
  };

  template<typename T>
  constexpr bool IsArray = is_std_array<T>::value;

}  // namespace cbm::algo::config

#endif  // CBM_ALGO_BASE_CONFIG_BASETYPES_H
