/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmDefs.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <stdexcept>  // for out_of_range

// operator ++ for ECbmModuleId for convenient usage in loops
// This operator is tuned for ECbmModuleID. It takes into account non
// continuous values for the enum. Since the detectorID which is stored
// in the generated output has only 4 bit the maximum number of detectors
// can be 16 (0-15). To avoid that the enum class has to be changed again
// the values 11-15 are reserved for future detectors.
// The ids of the passive modules are only relevant at run time so they can
// be shifted easily
// The operator takes care about the non continuous values for the enum
// When it reaches the last detector it automatically continuous with the
// first passive module
ECbmModuleId& operator++(ECbmModuleId& e)
{
  if (e == ECbmModuleId::kLastModule) { throw std::out_of_range("for ECbmModuleId& operator ++ (ECbmModuleId&)"); }
  else if (e == ECbmModuleId::kNofSystems) {  // jump over gap between end of detectors and begin of passives
    e = ECbmModuleId::kMagnet;
  }
  else {
    e = ECbmModuleId(static_cast<std::underlying_type<ECbmModuleId>::type>(e) + 1);
  }
  return e;
}

// operator << for convenient output to std::ostream.
// Converts the enum value to a string which is put in the stream
std::ostream& operator<<(std::ostream& strm, const ECbmModuleId& modId)
{
  strm << std::to_string(ToIntegralType(modId));
  return strm;
}

// conversion functions between ECbmModuleId and std::string
static const std::array<std::pair<ECbmModuleId, std::string>, 22> ModIdStrMap = {
  {{ECbmModuleId::kRef, "Ref"},
   {ECbmModuleId::kMvd, "Mvd"},
   {ECbmModuleId::kSts, "Sts"},
   {ECbmModuleId::kRich, "Rich"},
   {ECbmModuleId::kMuch, "Much"},
   {ECbmModuleId::kTrd, "Trd"},
   {ECbmModuleId::kTof, "Tof"},
   {ECbmModuleId::kEcal, "Ecal"},
   {ECbmModuleId::kPsd, "Psd"},
   {ECbmModuleId::kHodo, "Hodo"},
   {ECbmModuleId::kDummyDet, "DummyDet"},
   {ECbmModuleId::kT0, "T0"},
   {ECbmModuleId::kTrd2d, "Trd2d"},
   {ECbmModuleId::kNofSystems, "NofSystems"},
   {ECbmModuleId::kMagnet, "Magnet"},
   {ECbmModuleId::kTarget, "Target"},
   {ECbmModuleId::kPipe, "Pipe"},
   {ECbmModuleId::kShield, "Shield"},
   {ECbmModuleId::kPlatform, "Platform"},
   {ECbmModuleId::kCave, "Cave"},
   {ECbmModuleId::kLastModule, "LastModule"},
   {ECbmModuleId::kNotExist, "NotExist"}}};

std::string ToString(ECbmModuleId modId)
{
  auto result = std::find_if(ModIdStrMap.begin(), ModIdStrMap.end(),
                             [modId](std::pair<ECbmModuleId, std::string> p) { return p.first == modId; });
  if (result == std::end(ModIdStrMap)) { return "NotExist"; }
  return result->second;
};

ECbmModuleId ToCbmModuleId(std::string modIdStr)
{
  auto result = std::find_if(ModIdStrMap.begin(), ModIdStrMap.end(),
                             [modIdStr](std::pair<ECbmModuleId, std::string> p) { return p.second == modIdStr; });
  if (result == std::end(ModIdStrMap)) { return ECbmModuleId::kNotExist; }
  return result->first;
};

ECbmModuleId ToCbmModuleIdCaseInsensitive(std::string modIdStr)
{
  auto result =
    std::find_if(ModIdStrMap.begin(), ModIdStrMap.end(), [modIdStr](std::pair<ECbmModuleId, std::string> p) {
      return p.second.size() == modIdStr.size()
             && std::equal(p.second.begin(), p.second.end(), modIdStr.begin(),
                           [](unsigned char a, unsigned char b) { return std::tolower(a) == std::tolower(b); });
    });
  if (result == std::end(ModIdStrMap)) { return ECbmModuleId::kNotExist; }
  return result->first;
};

// operator << for convenient output to std::ostream.
// Converts the enum value to a string which is put in the stream
std::ostream& operator<<(std::ostream& strm, const ECbmDataType& dataType)
{
  strm << std::to_string(ToIntegralType(dataType));
  return strm;
}
