/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// \file   CaToolsMCPoint.cxx
/// \brief  Internal class describing a MC point for CA tracking QA and performance (implementation)
/// \date   18.11.2022
/// \author S.Zharko <s.zharko@gsi.de>


#include "CaToolsMCPoint.h"

#include <iomanip>
#include <sstream>

using namespace ca::tools;

// ---------------------------------------------------------------------------------------------------------------------
//
std::string MCPoint::ToString(int verbose, bool printHeader) const
{
  if (verbose < 1) { return std::string(); }

  std::stringstream msg;
  if (printHeader) {
    if (verbose > 0) {
      msg << std::setw(10) << std::setfill(' ') << "track ID" << ' ';
      msg << std::setw(10) << std::setfill(' ') << "mother ID" << '|';
      msg << std::setw(10) << std::setfill(' ') << "station ID" << '|';
      msg << std::setw(10) << std::setfill(' ') << "PDG" << ' ';
      msg << std::setw(10) << std::setfill(' ') << "m [GeV/c2]" << ' ';
      msg << std::setw(10) << std::setfill(' ') << "q [e]" << '|';
      msg << std::setw(14) << std::setfill(' ') << "t [ns]" << ' ';
      msg << std::setw(14) << std::setfill(' ') << "x [cm]" << ' ';
      msg << std::setw(14) << std::setfill(' ') << "y [cm]" << ' ';
      msg << std::setw(14) << std::setfill(' ') << "z [cm]" << '|';
      if (verbose > 1) {
        msg << std::setw(14) << std::setfill(' ') << "zIn [cm]" << ' ';
        msg << std::setw(14) << std::setfill(' ') << "zOut [cm]" << '|';
        msg << std::setw(14) << std::setfill(' ') << "p [GeV/c]" << '|';
        msg << std::setw(10) << std::setfill(' ') << "point ID" << ' ';
        msg << std::setw(10) << std::setfill(' ') << "point ID (ext)" << ' ';
        msg << std::setw(10) << std::setfill(' ') << "event ID" << ' ';
        msg << std::setw(10) << std::setfill(' ') << "file ID" << ' ';
      }
    }
  }
  else {
    if (verbose > 0) {
      msg << std::setw(10) << std::setfill(' ') << fTrackId << ' ';
      msg << std::setw(10) << std::setfill(' ') << fMotherId << '|';
      msg << std::setw(10) << std::setfill(' ') << fStationId << '|';
      msg << std::setw(10) << std::setfill(' ') << fPdgCode << ' ';
      msg << std::setw(10) << std::setfill(' ') << fMass << ' ';
      msg << std::setw(10) << std::setfill(' ') << fCharge << '|';
      msg << std::setw(14) << std::setfill(' ') << fTime << ' ';
      msg << std::setw(14) << std::setfill(' ') << fPos[0] << ' ';
      msg << std::setw(14) << std::setfill(' ') << fPos[1] << ' ';
      msg << std::setw(14) << std::setfill(' ') << fPos[2] << '|';
      if (verbose > 1) {
        msg << std::setw(14) << std::setfill(' ') << fPosIn[2] << ' ';
        msg << std::setw(14) << std::setfill(' ') << fPosOut[2] << '|';
        msg << std::setw(14) << std::setfill(' ') << this->GetP() << '|';
        msg << std::setw(10) << std::setfill(' ') << fId << ' ';
        msg << std::setw(10) << std::setfill(' ') << fLinkKey.fIndex << ' ';
        msg << std::setw(10) << std::setfill(' ') << fLinkKey.fEvent << ' ';
        msg << std::setw(10) << std::setfill(' ') << fLinkKey.fFile << ' ';
      }
    }
  }
  return msg.str();
}
