/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1Station.h"

#include <FairLogger.h>

#include <iomanip>
#include <sstream>

// TODO: Improve log style (S.Zh.)
void L1Station::Print(int verbosity) const
{
  LOG(info) << ToString(verbosity); 
}

std::string L1Station::ToString(int verbosityLevel, int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  if (verbosityLevel > 1) {
    // TODO: probably we can have verbosity level and address for each underlying object (S.Zharko)
    aStream << '\n' << indent << "Address: " << this << '\n';
  }
  if (verbosityLevel == 0) {
    aStream << indent << "- <z [cm], typeID> = " << std::setw(12) << std::setfill(' ') << z[0] << ", " << std::setw(4) << std::setfill(' ') << type;
  }
  else {
    aStream << indent << "Station type ID:  " << std::setw(12) << std::setfill(' ') << type << '\n';
    aStream << indent << "Is time info used:           " << std::setw(12) << std::setfill(' ') << timeInfo << '\n';
    aStream << indent << "Is station placed in field:  " << std::setw(12) << std::setfill(' ') << fieldStatus << '\n';
    aStream << indent << "z position [cm]:             " << std::setw(12) << std::setfill(' ') << z[0] << '\n';
    aStream << indent << "Rmin [cm]:                   " << std::setw(12) << std::setfill(' ') << Rmin[0] << '\n';
    aStream << indent << "Rmax [cm]:                   " << std::setw(12) << std::setfill(' ') << Rmax[0] << '\n';
    aStream << materialInfo.ToString(indentLevel) << '\n';
    if (verbosityLevel > 1) {
      aStream << indent << "Field approcimation coefficients:\n";
      aStream << fieldSlice.ToString(indentLevel + 1) << '\n';
    }
    aStream << indent << "Strips geometry:\n";
    aStream << indent << indentChar << "Front:\n";
    aStream << frontInfo.ToString(indentLevel + 2) << '\n';
    aStream << indent << indentChar << "Back:\n";
    aStream << backInfo.ToString(indentLevel + 2) << '\n';
    aStream << indent << indentChar << "XY cov matrix:\n";
    aStream << XYInfo.ToString(indentLevel + 2) << '\n';
    aStream << indent << indentChar << "X layer:\n";
    aStream << xInfo.ToString(indentLevel + 2) << '\n';
    aStream << indent << indentChar << "Y layer:\n";
    aStream << xInfo.ToString(indentLevel + 2) << '\n';
  }
  return aStream.str();
}
