/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#ifndef L1ObjectInitController_h
#define L1ObjectInitController_h 1

/// @file   L1ObjectInitController.h
/// @author Sergei Zharko
/// @date   22.02.2022

#include "FairLogger.h"

#include <bitset>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

/// L1ObjectInitController is a class, which provides flags system
/// and functionality needed for L1 algorithm objects initialization
///
// TODO: Possible improvement: introduce another template parameter, which represents a local enum class...
template<int NumberOfFlags, class InitKeyEnum>
class L1ObjectInitController {
public:
  /// Gets an initialization status of the flag placed at bitIndex
  /// \param bitIndex    index of bit
  bool GetFlag(InitKeyEnum bitKey) const
  {
#ifndef FAST_CODE
    int bitIndex = static_cast<int>(bitKey);
    if (bitIndex >= NumberOfFlags || bitIndex < 0) {
      LOG(fatal) << "L1OnjectInitController::GetFlagStatus: attempt of flag access with index = " << bitIndex
                 << " outside the range [0, " << NumberOfFlags << ']';
      assert((!(bitIndex >= NumberOfFlags || bitIndex < 0)));
    }
#endif  // FAST_CODE
    return fInitFlags[static_cast<int>(bitKey)];
  }

  /// Checks, if the object is finalized, i.e. all its fields were setup
  bool IsFinalized() const { return fInitFlags.size() == fInitFlags.count(); }

  /// Sets an initialization status of the flag placed at bitIndex
  /// \param bitIndex    index of bit
  /// \param newStatus   flag value (true is default)
  void SetFlag(InitKeyEnum bitKey, bool newStatus = true)
  {
#ifndef FAST_CODE
    int bitIndex = static_cast<int>(bitKey);
    if (bitIndex >= NumberOfFlags || bitIndex < 0) {
      LOG(fatal) << "L1OnjectInitController::GetFlagStatus: attempt of flag access with index = " << bitIndex
                 << " outside the range [0, " << NumberOfFlags << ']';
      assert((!(bitIndex >= NumberOfFlags || bitIndex < 0)));
    }
#endif  // FAST_CODE
    fInitFlags[static_cast<int>(bitKey)] = newStatus;
  }

  /// String representation of initialization flags contents
  /// \param indentLevel   number of indent charachets int output
  std::string ToString(int indentLevel = 0) const
  {
    std::stringstream aStream {};
    constexpr char indentChar = '\t';
    std::string indent(indentLevel, indentChar);
    aStream << indent << "L1ObjectInitController: flag values";
    aStream << '\n' << indent << "index: ";
    for (int idx = 0; idx < NumberOfFlags; ++idx) {
      aStream << std::setw(3) << std::setfill(' ') << idx << ' ';
    }
    aStream << '\n' << indent << "value: ";
    for (int idx = 0; idx < NumberOfFlags; ++idx) {
      aStream << "  " << static_cast<int>(fInitFlags[idx]) << ' ';
    }
    return aStream.str();
  }

private:
  std::bitset<NumberOfFlags> fInitFlags {};  ///< object of flags sets
};

#endif  // L1ObjectInitController_h
