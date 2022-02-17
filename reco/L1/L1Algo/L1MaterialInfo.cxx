/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1MaterialInfo.h"

#include <iomanip>
#include <sstream>
#include <vector>

std::string L1MaterialInfo::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly it is better to place indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "Thickness (X) [??]:          " << std::setw(12) << std::setfill(' ') << thick[0] << '\n';
  aStream << indent << "Radiational length (X0) [??]:" << std::setw(12) << std::setfill(' ') << RL[0] << '\n';
  aStream << indent << "X/X0:                        " << std::setw(12) << std::setfill(' ') << RadThick[0] << '\n';
  aStream << indent << "log(X/X0):                   " << std::setw(12) << std::setfill(' ') << logRadThick[0];
  return aStream.str();
}
