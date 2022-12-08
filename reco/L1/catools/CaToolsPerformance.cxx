/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsPerformance.h
/// \brief  Tracking performance class (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsPerformance.h"

#include <utility>  // for std::move

using namespace ca::tools;

// ---------------------------------------------------------------------------------------------------------------------
//
void Performance::ReceiveMCData(MCData&& mcData) { fMCData = std::move(mcData); }

// ---------------------------------------------------------------------------------------------------------------------
//
