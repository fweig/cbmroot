/* Copyright (C) 2016-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file  L1AlgoFunctions.h
 * @brief File contains some general purpose functions
 * @since 12.01.2022
 ***********************************************************************************************************/

#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

/// Template function, which sets a value to an element of the map with a particular key
/// \param key    Key of the element to be modified
/// \param value  New value of the element under the selected key
/// \param aMap   A reference to the map, which element is to be modified
template<class Key, class T, class Hash = std::hash<Key>>
void SetSingleValueToMap(Key key, T value, std::unordered_map<Key, T, Hash>& aMap)
{
  aMap[key] = value;
}

/// Template function, which sets a value to ALL elements of the map
/// \param value  New value of the element under the selected key
/// \param aMap   A reference to the map, which element is to be modified
template<class Key, class T, class Hash = std::hash<Key>>
void SetSameValueToMap(T value, std::unordered_map<Key, T, Hash>& aMap)
{
  for (auto it = aMap.begin(); it != aMap.end(); ++it) {
    it->second = value;
  }
}

/// Template function, which resets the elements of one map with the values defined in another map
/// \param inMap  A constant reference to the map containing new parameters
/// \param aMap   A reference to the map, which is going to be modified
template<class Key, class T, class Hash = std::hash<Key>>
void SetMappedValuesToMap(const std::unordered_map<Key, T, Hash>& inMap, std::unordered_map<Key, T, Hash>& aMap)
{
  for (auto it = aMap.begin(); it != aMap.end(); ++it) {
    if (inMap.find(it->first) != inMap.end()) { it->second = inMap.at(it->first); }
  }
}

/// Template function to represent mapped contents into std::string
/// NOTE: operator<< must be defined for value of the map
template<class Key, class T, class Hash = std::hash<Key>>
std::string RepresentMapWithString(const std::unordered_map<Key, T, Hash>& aMap, int entryWidth = 15)
{
  std::stringstream token;
  for (auto it = aMap.begin(); it != aMap.end(); ++it) {
    token << std::setw(entryWidth) << std::setfill(' ') << it->second << ' ';
  }
  return token.str();
}
