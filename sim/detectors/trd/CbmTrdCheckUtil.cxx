/* Copyright (C) 2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Etienne Bechtel [committer] */

// Includes from TRD
#include "CbmTrdCheckUtil.h"

#include "CbmTrdDigi.h"

#include "TMath.h"
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TProfile3D.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "assert.h"

//_________________________________________________________________________________
CbmTrdCheckUtil::CbmTrdCheckUtil() : TObject(), f1D(), f2D(), f3D(), fProfile1D(), fProfile2D(), fProfile3D() {}


void CbmTrdCheckUtil::CreateHist(std::string name, Int_t xbins, Double_t xlow, Double_t xhigh, Int_t ybins,
                                 Double_t ylow, Double_t yhigh)
{
  if (ybins == 0 && f1D[name]) return;
  if (f2D[name]) return;
  if (ybins == 0) f1D[name] = new TH1D(name.data(), name.data(), xbins, xlow, xhigh);
  else
    f2D[name] = new TH2D(name.data(), name.data(), xbins, xlow, xhigh, ybins, ylow, yhigh);
}

void CbmTrdCheckUtil::CreateHist3D(std::string name, Int_t xbins, Double_t xlow, Double_t xhigh, Int_t ybins,
                                   Double_t ylow, Double_t yhigh, Int_t zbins, Double_t zlow, Double_t zhigh)
{
  if (f3D[name]) return;
  f3D[name] = new TH3D(name.data(), name.data(), xbins, xlow, xhigh, ybins, ylow, yhigh, zbins, zlow, zhigh);
}

void CbmTrdCheckUtil::CreateProfile(std::string name, Int_t xbins, Double_t xlow, Double_t xhigh, Int_t ybins,
                                    Double_t ylow, Double_t yhigh)
{
  if (ybins == 0 && fProfile1D[name]) return;
  if (fProfile2D[name]) return;
  if (ybins == 0) fProfile1D[name] = new TProfile(name.data(), name.data(), xbins, xlow, xhigh);
  else
    fProfile2D[name] = new TProfile2D(name.data(), name.data(), xbins, xlow, xhigh, ybins, ylow, yhigh);
}

void CbmTrdCheckUtil::CreateProfile3D(std::string name, Int_t xbins, Double_t xlow, Double_t xhigh, Int_t ybins,
                                      Double_t ylow, Double_t yhigh, Int_t zbins, Double_t zlow, Double_t zhigh)
{
  if (fProfile3D[name]) return;
  fProfile3D[name] =
    new TProfile3D(name.data(), name.data(), xbins, xlow, xhigh, ybins, ylow, yhigh, zbins, zlow, zhigh);
}

void CbmTrdCheckUtil::Fill(std::string name, Double_t x, Double_t y)
{
  if (y == 9999.) {
    if (!f1D[name]) return;
    f1D[name]->Fill(x);
  }
  else {
    if (!f2D[name]) return;
    f2D[name]->Fill(x, y);
  }
}

void CbmTrdCheckUtil::Fill3D(std::string name, Double_t x, Double_t y, Double_t z)
{
  if (!f3D[name]) return;
  f3D[name]->Fill(x, y, z);
}

// void CbmTrdCheckUtil::FillProfile(std::string name,Double_t x, Double_t y){
//   if(y == 9999.)   {if(!fProfile1D[name]) return;fProfile1D[name]->Fill(x);}
//   else             {if(!fProfile2D[name]) return;fProfile2D[name]->Fill(x,y);}
// }

void CbmTrdCheckUtil::FillProfile(std::string name, Double_t x, Double_t y, Double_t z)
{
  if (z == 9999.) {
    if (!fProfile1D[name]) return;
    fProfile1D[name]->Fill(x, y);
  }
  else {
    if (!fProfile2D[name]) return;
    fProfile2D[name]->Fill(x, y, z);
  }
}

void CbmTrdCheckUtil::FillProfile3D(std::string name, Double_t x, Double_t y, Double_t z, Double_t w)
{
  if (!fProfile3D[name]) return;
  fProfile3D[name]->Fill(x, y, z, w);
}

void CbmTrdCheckUtil::Fill(std::string name, Double_t x, Double_t y, Double_t z)
{
  if (!f2D[name]) return;
  f2D[name]->Fill(x, y, z);
}

void CbmTrdCheckUtil::FillW(std::string name, Double_t x, Double_t w)
{
  if (!f1D[name]) return;
  f1D[name]->Fill(x, w);
}
