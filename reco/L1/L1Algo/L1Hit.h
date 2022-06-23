/* Copyright (C) 2007-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina, Igor Kulakov, Sergey Gorbunov [committer], Maksym Zyzak */

/// @file L1Hit.h
/// @author Sergey Gorbunov
/// @author Igor Kulakov
/// @author Valentina Akishina
/// @author Maksym Zyzak
/// @date 2007-2021

#ifndef L1Hit_h
#define L1Hit_h

using L1HitIndex_t   = unsigned /*short*/ int;  ///< Index of L1Hit
using L1StripIndex_t = unsigned /*short*/ int;  ///< Index of the station strip

/// TODO: replace typedef with using (C++11) (S.Zharko)
/// TODO: change naming: THitI -> L1HitIndex_t, TStripI -> L1StripIndex_t (S.Zharko)


///
/// L1Hit class describes a generic hit for CA tracker
///
struct L1Hit {
  L1StripIndex_t f {0};  ///< front strip index
  L1StripIndex_t b {0};  ///< back strip index
  float u {0.f};         ///< measured U coordinate
  float v {0.f};         ///< measured V coordinate
  float t {0.f};         ///< measured time
  float z {0.f};         ///< fixed Z coordinate
  float du {0.f};        ///< mean squared error of u
  float dv {0.f};        ///< mean squared error of v
  float dt {0.f};        ///< mean squared error of t
  int ID {0};            ///< TODO: check if this ID is redundant
};

#endif
