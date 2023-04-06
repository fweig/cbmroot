/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaConstants.h
/// @brief  List of constant expressions, common in QA task
/// @since  04.04.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

namespace CbmQaConstants
{

  // Unit
  namespace unit
  {
    constexpr double cm = 1.;
    constexpr double m  = cm * 1.e+2;
    constexpr double mm = cm * 1.e-1;
    constexpr double um = cm * 1.e-4;
    constexpr double nm = cm * 1.e-7;
  }  // namespace unit

}  // namespace CbmQaConstants