/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsMcData.cxx
/// \brief  Data structure for internal tracking MC-information (implementation)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#include "CaToolsMcData.h"

#include <utility>  // for std::move

using namespace ca::tools;

// ---------------------------------------------------------------------------------------------------------------------
//
McData::McData() {}

// ---------------------------------------------------------------------------------------------------------------------
//
McData::McData(const McData& /*other*/) {}

// ---------------------------------------------------------------------------------------------------------------------
//
McData::McData(McData&& other) noexcept { this->Swap(other); }

// ---------------------------------------------------------------------------------------------------------------------
//
McData& McData::operator=(const McData& other)
{
  if (this != &other) { McData(other).Swap(*this); }
  return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
//
McData& McData::operator=(McData&& other) noexcept
{
  if (this != &other) {
    McData tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}

// ---------------------------------------------------------------------------------------------------------------------
//
void McData::Swap(McData& /*other*/) noexcept {}
