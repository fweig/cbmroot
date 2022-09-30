/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsMcDataManager.cxx
/// \brief  Manager class for handling McData structure (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsMcDataManager.h"

#include <cassert>
#include <utility>

#include "CaToolsPerformance.h"

using namespace ca::tools;

// ---------------------------------------------------------------------------------------------------------------------
//
bool McDataManager::SendMcData(Performance* pPerformance)
{
  assert(pPerformance);
  pPerformance->ReceiveMcData(std::move(fMcData));
  return true;
}
