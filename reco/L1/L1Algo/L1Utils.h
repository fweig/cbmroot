/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file  L1Utils.h
 * @brief File contains some general purpose functions for L1Algo
 * @since 12.01.2022
 ***********************************************************************************************************/
#ifndef L1Utils_h
#define L1Utils_h 1

#include <type_traits>

#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

#include <cmath>

#if defined(__CLING__) && defined(__arm64__)
#include "vectors/PSEUDO_F32vec4.h"
#else
#include "vectors/P4_F32vec4.h"
#endif

/// Class contains some utility functions for L1Algo
struct L1Utils {

  /// NaN value for float
  static constexpr float kNaN {std::numeric_limits<float>::signaling_NaN()};

  /// Comparison method for floats
  /// \param  lhs  Left floating point to compare
  /// \param  rhs  Right floating point to compare
  /// \return      Comparison result: true - equals within epsilon
  template<typename T, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = nullptr>
  static bool CmpFloats(T lhs, T rhs)
  {
    return fabs(lhs - rhs) < 2. * std::numeric_limits<T>::epsilon() * fabs(lhs + rhs)
           || fabs(lhs - rhs) < std::numeric_limits<T>::min();
  }


  static __attribute__((always_inline)) void CheckSimdVectorEquality(fvec v, const char* name)
  {
    if (!v.IsHorizontallyEqual()) {
      std::stringstream msg;
      msg << name << " SIMD vector is inconsistent, not all of the words are equal each other: " << v;
      throw std::logic_error(msg.str());
    }
  }

  /// Hash for unordered_map with enum class keys
  struct EnumClassHash {
    template<typename T>
    int operator()(T t) const
    {
      return static_cast<int>(t);
    }
  };

  /// Template function, which sets a value to an element of the map with a particular key
  /// \param key    Key of the element to be modified
  /// \param value  New value of the element under the selected key
  /// \param aMap   A reference to the map, which element is to be modified
  template<class Key, class T, class Hash = std::hash<Key>>
  static void SetSingleValueToMap(Key key, T value, std::unordered_map<Key, T, Hash>& aMap)
  {
    aMap[key] = value;
  }

  /// Template function, which sets a value to ALL elements of the map
  /// \param value  New value of the element under the selected key
  /// \param aMap   A reference to the map, which element is to be modified
  template<class Key, class T, class Hash = std::hash<Key>>
  static void SetSameValueToMap(T value, std::unordered_map<Key, T, Hash>& aMap)
  {
    for (auto it = aMap.begin(); it != aMap.end(); ++it) {
      it->second = value;
    }
  }

  /// Template function, which resets the elements of one map with the values defined in another map
  /// \param inMap  A constant reference to the map containing new parameters
  /// \param aMap   A reference to the map, which is going to be modified
  template<class Key, class T, class Hash = std::hash<Key>>
  static void SetMappedValuesToMap(const std::unordered_map<Key, T, Hash>& inMap,
                                   std::unordered_map<Key, T, Hash>& aMap)
  {
    for (auto it = aMap.begin(); it != aMap.end(); ++it) {
      if (inMap.find(it->first) != inMap.end()) { it->second = inMap.at(it->first); }
    }
  }

  /// Template function to represent mapped contents into std::string
  /// NOTE: operator<< must be defined for value of the map
  template<class Key, class T, class Hash = std::hash<Key>>
  static std::string RepresentMapWithString(const std::unordered_map<Key, T, Hash>& aMap, int entryWidth = 15)
  {
    std::stringstream token;
    for (auto it = aMap.begin(); it != aMap.end(); ++it) {
      token << std::setw(entryWidth) << std::setfill(' ') << it->second << ' ';
    }
    return token.str();
  }
};

#endif  // L1Utils_h
