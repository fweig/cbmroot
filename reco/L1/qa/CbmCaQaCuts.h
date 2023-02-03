/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

#ifndef CbmCaQaCuts_h
#define CbmCaQaCuts_h 1

namespace cbm::ca::qa::cuts
{
  constexpr double kMinP = 0.05;  ///< minimal momentum [Gev/c]

  // Max difference between hit and station z
  constexpr double kMaxDzStHitSts = 1.;  ///< Max distance for STS
};                                       // namespace cbm::ca::qa::cuts

#endif  // CbmCaQaCuts_h
