/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1XYMeasurementInfo.h"

#include <iomanip>
#include <sstream>  // for stringstream

#include "L1Utils.h"

void L1XYMeasurementInfo::CheckConsistency() const
{
  /* (i) Checks for the horizontal equality of SIMD vector elements */
  L1Utils::CheckSimdVectorEquality(C00, "L1XYMeasurementsInfo::C00");
  L1Utils::CheckSimdVectorEquality(C10, "L1XYMeasurementsInfo::C10");
  L1Utils::CheckSimdVectorEquality(C11, "L1XYMeasurementsInfo::C11");
}

std::string L1XYMeasurementInfo::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly is better to be place indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "C00:         " << std::setw(12) << std::setfill(' ') << C00[0] << '\n';
  aStream << indent << "C10:         " << std::setw(12) << std::setfill(' ') << C10[0] << '\n';
  aStream << indent << "C11:         " << std::setw(12) << std::setfill(' ') << C11[0];
  return aStream.str();
}
