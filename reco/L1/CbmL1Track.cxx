/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmL1Track.cxx
/// @brief  Ca Tracking track representation in CBM (implementation)
/// @since  04.04.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmL1Track.h"

#include <sstream>

// ---------------------------------------------------------------------------------------------------------------------
//
std::string CbmL1Track::ToString(int verbose, bool header) const
{
  using std::setfill;
  using std::setw;
  std::stringstream msg;
  if (header) {
    msg << setw(12) << setfill(' ') << "p [GeV/c]" << ' ';
    msg << setw(12) << setfill(' ') << "Tx" << ' ';
    msg << setw(8) << setfill(' ') << "N hits" << ' ';
    msg << setw(8) << setfill(' ') << "N MC tracks" << ' ';
    msg << setw(12) << setfill(' ') << "MC tr (old)" << ' ';
    msg << setw(12) << setfill(' ') << "MC tr (new)" << ' ';
    msg << setw(8) << setfill(' ') << "IsGhost" << ' ';
    msg << setw(8) << setfill(' ') << "Max.pur." << ' ';
    msg << '\n';
  }
  else {


    msg << setw(12) << setfill(' ') << GetP() << ' ';
    msg << setw(12) << setfill(' ') << GetTx() << ' ';
    msg << setw(8) << setfill(' ') << GetNofHits() << ' ';
    msg << setw(8) << setfill(' ') << GetNofMCTracks() << ' ';
    msg << setw(12) << setfill(' ') << GetMatchedMCTrackIndex() << ' ';
    msg << setw(12) << setfill(' ') << (mcTracks.size() ? mcTracks[0]->ID : -1) << ' ';
    msg << setw(8) << setfill(' ') << IsGhost() << ' ';
    msg << setw(8) << setfill(' ') << GetMaxPurity() << ' ';
    msg << '\n';
    if (verbose > 0) {
      msg << "\thits: ";
      for (int iH : GetHitIndexes()) {
        msg << iH << ' ';
      }
      msg << "\n\tmc tracks: (NEW)";
      for (int iT : GetMCTrackIndexes()) {
        msg << iT << ' ';
      }
      msg << "\n\tmc tracks: (Old)";
      for (auto* iT : mcTracks) {
        msg << iT->ID << ' ';
      }
    }
  }
  return msg.str();
}
