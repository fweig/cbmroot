/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1MaterialInfo.h"

#include <iomanip>
#include <sstream>
#include <vector>

/************************
 * L1MaterialInfo class *
 ************************/

std::string L1MaterialInfo::ToString(int indentLevel) const
{
  std::stringstream aStream {};
  // TODO: possibly it is better to place indentChar into L1Parameters (S.Zharko)
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);
  aStream << indent << "Station thickness (X) [??]:  " << std::setw(12) << std::setfill(' ') << thick[0] << '\n';
  aStream << indent << "Radiation length (X0) [??]:  " << std::setw(12) << std::setfill(' ') << RL[0] << '\n';
  aStream << indent << "X/X0:                        " << std::setw(12) << std::setfill(' ') << RadThick[0] << '\n';
  aStream << indent << "log(X/X0):                   " << std::setw(12) << std::setfill(' ') << logRadThick[0];
  return aStream.str();
}

/********************
 * L1Material class *
 ********************/

L1Material::L1Material() {}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Material::~L1Material() noexcept {}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Material::L1Material(const L1Material& other)
  : fNbins(other.fNbins)
  , fRmax(other.fRmax)
  , fFactor(other.fFactor)
  , fTable(other.fTable)
{
}

//------------------------------------------------------------------------------------------------------------------------------------
//

L1Material& L1Material::operator=(const L1Material& other)
{
  if (this != &other) {
    fNbins  = other.fNbins;
    fRmax   = other.fRmax;
    fFactor = other.fFactor;
    fTable.clear();
    fTable.resize(other.fTable.size());
    for (size_t i = 0; i < other.fTable.size(); ++i) {
      fTable[i] = other.fTable[i];
    }
  }
  return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Material::L1Material(L1Material&& other) noexcept { this->Swap(other); }

//------------------------------------------------------------------------------------------------------------------------------------
//
L1Material& L1Material::operator=(L1Material&& other) noexcept
{
  if (this != &other) {
    L1Material tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
float L1Material::GetRadThick(float x, float y) const
{
  x     = (x < fRmax && x >= -fRmax) ? x : 0;
  y     = (y < fRmax && y >= -fRmax) ? y : 0;
  int i = static_cast<int>((x + fRmax) * fFactor);
  int j = static_cast<int>((y + fRmax) * fFactor);
  i     = (i < fNbins && i >= 0) ? i : fNbins / 2;
  j     = (j < fNbins && j >= 0) ? j : fNbins / 2;
  return fTable[i + j * fNbins];
}

//------------------------------------------------------------------------------------------------------------------------------------
//
fvec L1Material::GetRadThick(fvec x, fvec y) const
{
  fvec r;
  for (int i = 0; i < fvecLen; i++)
    r[i] = GetRadThick(x[i], y[i]);
  return r;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Material::SetBins(int nBins, float stationSize)
{
  fNbins  = nBins;
  fRmax   = stationSize;
  fFactor = 0.5 * fNbins / fRmax;

  fTable.resize(fNbins * fNbins);
}

//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Material::Swap(L1Material& other) noexcept
{
  std::swap(fNbins, other.fNbins);
  std::swap(fRmax, other.fRmax);
  std::swap(fFactor, other.fFactor);
  std::swap(fTable, other.fTable);  // Probably can cause segmentation violation (did not understand)
}
