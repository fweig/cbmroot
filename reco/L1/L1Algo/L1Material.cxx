/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#include "L1Material.h"

#include <Logger.h>

#include <iomanip>
#include <sstream>
#include <vector>

#include "L1Utils.h"


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
float L1Material::GetRadThickScal(float x, float y) const
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
fvec L1Material::GetRadThickVec(fvec x, fvec y) const
{
  fvec r;
  for (size_t i = 0; i < fvec::size(); i++)
    r[i] = GetRadThickScal(x[i], y[i]);
  return r;
}


//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Material::CheckConsistency() const
{
  /* (i) Check, if the object was initialized */
  if (fNbins < 1 || std::isnan(fRmax)) { throw std::logic_error("L1Material: class was not initialized"); }

  /* (ii) Check if the thickness values correct (non-negative) */
  bool isThicknessOk = true;
  for (int i = 0; i < int(fTable.size()); ++i) {
    if (fTable[i] < 0) {
      isThicknessOk = false;
      LOG(error) << "L1Material: found illegal negative station thickness value " << fTable[i]
                 << " at iBinX = " << (i % fNbins) << ", iBin = " << (i / fNbins);
    }
  }
  if (!isThicknessOk) {
    throw std::logic_error("L1Material: incorrect station thickness values found in the thickness map");
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
//
void L1Material::SetBins(int nBins, float stationSize)
{
  fNbins = nBins;
  fRmax  = stationSize;

  if (fNbins < 1) {
    std::stringstream aStream;
    aStream << "L1Material: object cannot be initialized with non-positive nBins = " << nBins;
    throw std::logic_error(aStream.str());
  }

  if (stationSize < 0) {
    std::stringstream aStream;
    aStream << "L1Material: object cannot be initialized with non-positive stationStation = " << stationSize << " [cm]";
    throw std::logic_error(aStream.str());
  }

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

void L1Material::Repare()
{
  // correction of the material map: fill empty bins
  // we assume that bins with radiation thickness == 0. are lacking statistics

  const float cut = 1.e-8;
  const int n     = GetNbins();
  std::vector<float> oldMap(n * n, 0.);

  bool repeat = 1;

  while (repeat) {  // until there are empty bins

    oldMap.assign(oldMap.size(), 0.);
    for (int iy = 0; iy < n; ++iy) {
      for (int ix = 0; ix < n; ++ix) {
        oldMap[iy * n + ix] = GetRadThickBin(ix, iy);
      }
    }

    repeat = 0;
    for (int iy = 0; iy < n; ++iy) {
      for (int ix = 0; ix < n; ++ix) {
        if (oldMap[iy * n + ix] >= cut) continue;

        double sum  = 0.;
        double sumw = 0.;
        // look left
        for (int i = ix - 1; i >= 0; --i) {
          float v = oldMap[iy * n + i];
          if (v >= cut) {
            double w = 1. / (ix - i);
            sum += w * v;
            sumw += w;
            break;
          }
        }
        // look right
        for (int i = ix + 1; i < n; ++i) {
          float v = oldMap[iy * n + i];
          if (v >= cut) {
            double w = 1. / (i - ix);
            sum += w * v;
            sumw += w;
            break;
          }
        }
        // look down
        for (int i = iy - 1; i >= 0; --i) {
          float v = oldMap[i * n + ix];
          if (v >= cut) {
            double w = 1. / (iy - i);
            sum += w * v;
            sumw += w;
            break;
          }
        }
        // look up
        for (int i = iy + 1; i < n; ++i) {
          float v = oldMap[i * n + ix];
          if (v >= cut) {
            double w = 1. / (i - iy);
            sum += w * v;
            sumw += w;
            break;
          }
        }
        if (sumw > 1.e-8) {
          SetRadThickBin(ix, iy, sum / sumw);
          repeat = true;
        }
      }
    }
  }
}
