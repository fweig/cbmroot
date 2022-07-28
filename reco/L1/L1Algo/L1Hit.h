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

using L1HitIndex_t   = unsigned /*short*/ int;  ///< Index of L1Hit
using L1StripIndex_t = unsigned /*short*/ int;  ///< Index of the station strip

///
/// L1Hit class describes a generic hit for CA tracker
/// Note: U is a transverse coordinate of the hit in the axis perpendicular to the front strip
/// Note: V is a transverse coordinate of the hit in the axis perpendicular to the back strip
///
class L1Hit {
public:
  L1StripIndex_t f {0};  ///< front strip index
  L1StripIndex_t b {0};  ///< back strip index
  float u {0.f};         ///< measured U coordinate [cm]
  float v {0.f};         ///< measured V coordinate [cm]
  float t {0.f};         ///< measured time
  float z {0.f};         ///< fixed Z coordinate
  float du {0.f};        ///< measured uncertainty of U coordinate [cm]
  float dv {0.f};        ///< measured uncertainty of V coordinate [cm]
  float dt {0.f};        ///< measured uncertainty of time [ns]
  int ID {0};            ///< TODO: check if this ID is redundant
};

#endif
