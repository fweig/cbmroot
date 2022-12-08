/* Copyright (C) 2010-2017 Frankfurt Institute for Advanced Studies, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel,  Sergey Gorbunov, Igor Kulakov [committer], Maksym Zyzak */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  L1 Monte Carlo information
 *
 *====================================================================
 */

#ifndef CbmL1MCPoint_H
#define CbmL1MCPoint_H

#include <iomanip>
#include <sstream>
#include <string>

#include "L1Vector.h"

struct CbmL1MCPoint {

  CbmL1MCPoint() = default;

  static bool compareIDz(const CbmL1MCPoint& a, const CbmL1MCPoint& b)
  {
    return (a.ID < b.ID) || ((a.ID == b.ID) && (a.z < b.z));
  }

  static bool pcompareIDz(const CbmL1MCPoint* a, const CbmL1MCPoint* b)
  {
    return (a->ID < b->ID) || ((a->ID == b->ID) && (a->z < b->z));
  }

  double x      = 0.;
  double y      = 0.;
  double z      = 0.;
  double px     = 0.;
  double py     = 0.;
  double pz     = 0.;
  double xIn    = 0.;
  double yIn    = 0.;
  double zIn    = 0.;
  double pxIn   = 0.;
  double pyIn   = 0.;
  double pzIn   = 0.;
  double xOut   = 0.;
  double yOut   = 0.;
  double zOut   = 0.;
  double pxOut  = 0.;
  double pyOut  = 0.;
  double pzOut  = 0.;
  double p      = 0.;
  double q      = 0.;
  double mass   = 0.;
  double time   = 0.;
  int pdg       = 0;
  int ID        = 0;
  int mother_ID = 0;
  int iStation  = 0;
  int pointId   = -1;
  int file      = -1;
  int event     = -1;
  L1Vector<int> hitIds {"CbmL1MCPoint::hitIds"};  // indices of CbmL1Hits in L1->vStsHits array

  /// Temporary log function for debugging
  std::string ToString(int verbose, bool printHeader = false) const
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
          msg << std::setw(10) << std::setfill(' ') << "event ID" << ' ';
          msg << std::setw(10) << std::setfill(' ') << "file ID" << ' ';
        }
      }
    }
    else {
      if (verbose > 0) {
        msg << std::setw(10) << std::setfill(' ') << ID << ' ';
        msg << std::setw(10) << std::setfill(' ') << mother_ID << '|';
        msg << std::setw(10) << std::setfill(' ') << iStation << '|';
        msg << std::setw(10) << std::setfill(' ') << pdg << ' ';
        msg << std::setw(10) << std::setfill(' ') << mass << ' ';
        msg << std::setw(10) << std::setfill(' ') << q << '|';
        msg << std::setw(14) << std::setfill(' ') << time << ' ';
        msg << std::setw(14) << std::setfill(' ') << x << ' ';
        msg << std::setw(14) << std::setfill(' ') << y << ' ';
        msg << std::setw(14) << std::setfill(' ') << z << '|';
        if (verbose > 1) {
          msg << std::setw(14) << std::setfill(' ') << zIn << ' ';
          msg << std::setw(14) << std::setfill(' ') << zOut << '|';
          msg << std::setw(14) << std::setfill(' ') << p << '|';
          msg << std::setw(10) << std::setfill(' ') << pointId << ' ';
          msg << std::setw(10) << std::setfill(' ') << event << ' ';
          msg << std::setw(10) << std::setfill(' ') << file << ' ';
        }
      }
    }
    return msg.str();
  }
};

#endif
