/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko */

#include "L1UMeasurementInfo.h"

#include <iomanip>
#include <sstream>

std::string L1UMeasurementInfo::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly it is better to place the indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "cos(phi):    " << std::setw(12) << std::setfill(' ') << cos_phi[0] << '\n';
  aStream << indent << "sin(phi):    " << std::setw(12) << std::setfill(' ') << sin_phi[0] << '\n';
  aStream << indent << "sigma2 [??]: " << std::setw(12) << std::setfill(' ') << sigma2[0];
  return aStream.str();
}
