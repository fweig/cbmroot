/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1XYMeasurementInfo.h"

#include <sstream>  // for stringstream

std::string L1XYMeasurementInfo::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly is better to be place indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "C00: " << std::setw(12) << std::setfill(' ') << C00 << '\n';
  aStream << indent << "C10: " << std::setw(12) << std::setfill(' ') << C10 << '\n';
  aStream << indent << "C11: " << std::setw(12) << std::setfill(' ') << C11;
  return aStream.str();
}
