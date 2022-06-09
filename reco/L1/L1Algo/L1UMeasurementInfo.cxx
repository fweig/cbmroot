/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1UMeasurementInfo.h"

#include <iomanip>
#include <sstream>

//----------------------------------------------------------------------------------------------------------------------
//
std::string L1UMeasurementInfo::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly it is better to place the indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "cos(phi):    " << std::setw(12) << std::setfill(' ') << cos_phi[0] << '\n';
  aStream << indent << "sin(phi):    " << std::setw(12) << std::setfill(' ') << sin_phi[0] << '\n';
  aStream << indent << "sigma2 [cm2]: " << std::setw(12) << std::setfill(' ') << sigma2[0];
  return aStream.str();
}

//----------------------------------------------------------------------------------------------------------------------
//
void L1UMeasurementInfo::CheckConsistency() const
{
  /* (i) Checks for the horizontal equality of SIMD vector elements */
  L1Utils::CheckSimdVectorEquality(cos_phi, "L1UMeasurementsInfo::cos_phi");
  L1Utils::CheckSimdVectorEquality(sin_phi, "L1UMeasurementsInfo::sin_phi");
  L1Utils::CheckSimdVectorEquality(sigma2, "L1UMeasurementsInfo::sigma2");
  
  /*(ii) Sigma2 possible values*/
  if (sigma2[0] < 0) {
    std::stringstream msg;
    msg << "L1UMeasurementsInfo: illegal value of sigma2: " << sigma2[0] << " [cm2] (sigma2 >= 0 excepted)";
    throw std::logic_error(msg.str());
  }
}
