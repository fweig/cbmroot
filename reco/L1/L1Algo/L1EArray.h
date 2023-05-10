/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   L1EArray.h
/// @brief  Implementation of L1EArray class
/// @since  02.05.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#ifndef L1EArray_h
#define L1EArray_h 1

#include <array>

/// @brief Class of arrays, which can be accessed by an enum class entry as an index
/// @note  The enum-array must contain an entry kEND, which represents the number of the enumeration entries and
///        is used, as an array size
/// @tparam  E  The enum class type
/// @tparam  T  Type of data in the underlying array
template<class E, class T>
class L1EArray : public std::array<T, static_cast<std::size_t>(E::kEND)> {
  using U = typename std::underlying_type<E>::type;  ///< Underlying type of enumeration
public:
  /// @brief Mutable access operator, indexed by enum entry
  T& operator[](const E& entry)
  {
    return std::array<T, static_cast<std::size_t>(E::kEND)>::operator[](static_cast<U>(entry));
  }

  /// @brief Mutable access operator, indexed by underlying index type
  T& operator[](U index) { return std::array<T, static_cast<std::size_t>(E::kEND)>::operator[](index); }

  /// @brief Constant access operator, indexed by enum entry
  const T& operator[](const E& entry) const
  {
    return std::array<T, static_cast<std::size_t>(E::kEND)>::operator[](static_cast<U>(entry));
  }

  /// @brief Constant access operator, indexed by underlying index type
  const T& operator[](U index) const { return std::array<T, static_cast<std::size_t>(E::kEND)>::operator[](index); }
};

#endif  // L1EArray_h
