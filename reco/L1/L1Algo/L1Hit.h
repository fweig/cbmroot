/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina, Igor Kulakov, Sergey Gorbunov [committer], Maksym Zyzak */

/// @file L1Hit.h
/// @author Sergey Gorbunov
/// @author Igor Kulakov
/// @author Valentina Akishina
/// @author Maksym Zyzak
/// @date 2007-2022

#ifndef L1Hit_h
#define L1Hit_h

#include <boost/serialization/access.hpp>

#include "L1Constants.h"

using L1HitIndex_t   = unsigned /*short*/ int;  ///< Index of L1Hit
using L1StripIndex_t = unsigned /*short*/ int;  ///< Index of the station strip

///
/// L1Hit class describes a generic hit for CA tracker
/// Note: U is a transverse coordinate of the hit in the axis perpendicular to the front strip
/// Note: V is a transverse coordinate of the hit in the axis perpendicular to the back strip
///
class /*alignas(L1Constants::misc::kAlignment)*/ L1Hit {
  friend class boost::serialization::access;

public:
  L1StripIndex_t f {0};  ///< front hit key index
  L1StripIndex_t b {0};  ///< back hit key index
  /// NOTE: For STS f and b correspond to the indexes of the front and back clusters of the hit in a dataset. For other
  ///       tracking detectors (MVD, MuCh, TRD, TOF) f == b and corresponds to the index of the hit. Indexes f and b
  ///       do not intersect between different detector stations.

  float u  = 0.f;  ///< measured U coordinate [cm]
  float v  = 0.f;  ///< measured V coordinate [cm]
  float t  = 0.f;  ///< measured time [ns]
  float z  = 0.f;  ///< fixed Z coordinate [cm]
  float du2 = 0.f;  ///< measured uncertainty of U coordinate [cm]
  float dv2 = 0.f;  ///< measured uncertainty of V coordinate [cm]
  float dt2 = 0.f;  ///< measured uncertainty of time [ns]
  int ID   = 0;    ///< index of hit before hits sorting
  int iSt  = 0;    ///< index of station in the active stations array
  // TODO: Test speed penalty of using iSt index

private:
  /// Serialization method, used to save L1Hit objects into binary or text file in a defined order
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& f;
    ar& b;
    ar& u;
    ar& v;
    ar& t;
    ar& z;
    ar& du2;
    ar& dv2;
    ar& dt2;
    ar& ID;
    ar& iSt;
  }
};

#endif
