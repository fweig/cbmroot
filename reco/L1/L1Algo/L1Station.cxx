/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1Station.h"

#include <Logger.h>

#include <iomanip>
#include <sstream>

//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Station::CheckConsistency() const
{
  /*
   * Integer fields initialization checks
   */

  if (type < 0) {
    std::stringstream msg;
    msg << "L1Station: station type was not initialized (type = " << type << ", type > 0)";
    throw std::logic_error(msg.str());
  }

  if (timeInfo != 0 && timeInfo != 1) {
    std::stringstream msg;
    msg << "L1Station: illegal time information flag (timeInfo = " << timeInfo << ", "
        << "0 [time information is not used] or 1 [time information is used] expected)";
    throw std::logic_error(msg.str());
  }

  if (fieldStatus != 0 && fieldStatus != 1) {
    std::stringstream msg;
    msg << "L1Station: illegal field status flag (fieldStatus = " << fieldStatus << ", "
        << "0 [station is outside the field] or 1 [station is inside the field] expected";
    throw std::logic_error(msg.str());
  }

  /*
   * SIMD vector checks: all the words in a SIMD vector must be equal
   */

  L1Utils::CheckSimdVectorEquality(fZ, "L1Station::fZ");
  L1Utils::CheckSimdVectorEquality(Rmin, "L1Station::Rmin");
  L1Utils::CheckSimdVectorEquality(Rmax, "L1Station::Rmax");
  L1Utils::CheckSimdVectorEquality(dt, "L1Station::dt");

  /*
   * Inner and outer radia checks:
   *  (i)  both Rmin and Rmax must be >= 0
   *  (ii) Rmax cannot be smaller or equal to Rmin
   */

  if (Rmin[0] < 0 || L1Utils::CmpFloats(Rmin[0], Rmax[0]) || Rmax[0] < Rmin[0]) {
    std::stringstream msg;
    msg << "L1Station: " << this->ToString() << " has incorrect radia values: "
        << "Rmin = " << Rmin[0] << " [cm], Rmax = " << Rmax[0] << " [cm] (0 <= Rmin < Rmax expected)";
    throw std::logic_error(msg.str());
  }

  /*
   * Time resolution cannot be smaller then 0
   */

  if (dt[0] < 0) {
    std::stringstream msg;
    msg << "L1Station: " << this->ToString() << " has incorrect time resolution value: "
        << "dt = " << dt[0] << " ns (expected positive)";
    throw std::logic_error(msg.str());
  }

  /*
   * Check consistency of other members
   */

  //materialInfo.CheckConsistency();
  // TODO: Temporarily switched off, because Much has RL = 0, which is actually incorrect, but really is not used.
  //       One should provide average radiation length values for each Much layer (S.Zharko)
  fieldSlice.CheckConsistency();
  frontInfo.CheckConsistency();
  backInfo.CheckConsistency();
  xInfo.CheckConsistency();
  yInfo.CheckConsistency();
  XYInfo.CheckConsistency();

  // Check consistency of coordinate transformations
  for (float x = -1.f; x < 1.1f; x += 0.2) {
    for (float y = -1.f; y < 1.1f; y += 0.2) {
      auto [u, v]   = ConvXYtoUV<float>(x, y);
      auto [x1, y1] = ConvUVtoXY<float>(u, v);

      if (fabs(x1 - x) > 1.e-6 || fabs(y1 - y) > 1.e-6) {
        std::stringstream msg;
        msg << "L1Station: " << this->ToString() << " makes wrong coordinate convertion: "
            << "x,y " << x << "," << y << " -> u,v " << u << "," << v << " -> x,y " << x1 << "," << y1
            << " are not the same: dx,dy " << x1 - x << "," << y1 - y;
        throw std::logic_error(msg.str());
      }
    }
  }
}

// TODO: Improve log style (S.Zh.)
//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Station::Print(int verbosity) const { LOG(info) << ToString(verbosity); }

//------------------------------------------------------------------------------------------------------------------------------------
//
std::string L1Station::ToString(int verbosityLevel, int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  if (verbosityLevel == 0) { aStream << "station type = " << type << ", z = " << fZ[0] << " cm"; }
  else {
    aStream << '\n';
    aStream << indent << "z pos [cm]:   " << std::setw(12) << std::setfill(' ') << fZ[0] << '\n';
    aStream << indent << "z thick [cm]:   " << std::setw(12) << std::setfill(' ') << fZthick[0] << '\n';
    aStream << indent << "R_min [cm]:   " << std::setw(12) << std::setfill(' ') << Rmin[0] << '\n';
    aStream << indent << "R_max [cm]:   " << std::setw(12) << std::setfill(' ') << Rmax[0] << '\n';
    aStream << indent << "Station type: " << std::setw(12) << std::setfill(' ') << type << '\n';
    aStream << indent << "Is time used: " << std::setw(12) << std::setfill(' ') << timeInfo << '\n';
    aStream << indent << "Is in field:  " << std::setw(12) << std::setfill(' ') << fieldStatus << '\n';

    if (verbosityLevel > 3) {
      aStream << indent << "Field approcimation coefficients:\n";
      aStream << fieldSlice.ToString(indentLevel + 1) << '\n';
    }
    if (verbosityLevel > 2) {
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
      aStream << yInfo.ToString(indentLevel + 2) << '\n';
    }
  }
  return aStream.str();
}
