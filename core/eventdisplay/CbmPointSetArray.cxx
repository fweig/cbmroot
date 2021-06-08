/* Copyright (C) 2005-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: J. Brandt, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----               CbmPointSetArray source file                    -----
// -----                Created 18/07/05  by J. Brandt                 -----
// -----               Following class TEvePointSetArray               -----
// -------------------------------------------------------------------------
#include "CbmPointSetArray.h"

#include <Logger.h>  // for LOG, Logger

#include <Rtypes.h>             // for ClassImp
#include <TColor.h>             // for TColor
#include <TEvePointSet.h>       // for TEvePointSet, TEvePointSetArray
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TString.h>            // for Form

////////////////////////////////////////////////////////////////////////////////
/// Constructor.

CbmPointSetArray::CbmPointSetArray(const char* name, const char* title)
  : TEvePointSetArray(name, title)
  , fColorMode(1)
  , fMarkerMode(1)
  , fTime(nullptr)
  , fCluSize(nullptr)
  , fToT(nullptr)
  , fIndex(nullptr)
  , fNPoints(0)
{
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor.

CbmPointSetArray::~CbmPointSetArray()
{
  delete[] fTime;
  fTime = 0;
  delete[] fToT;
  fToT = 0;
  delete[] fCluSize;
  fCluSize = 0;
  delete[] fIndex;
  fIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// Init Arrays for physical Quantities.

void CbmPointSetArray::InitValues(Int_t npoints)
{
  fNPoints = npoints;
  fTime    = new Double_t[npoints];
  fToT     = new Double_t[npoints];
  fCluSize = new Int_t[npoints];
  fIndex   = new Int_t[npoints];
}

////////////////////////////////////////////////////////////////////////////////
/// FillQuantities.

void CbmPointSetArray::FillValues(Int_t id, Double_t time, Double_t tot, Int_t cluSize, Int_t index)
{
  fTime[id]    = time;
  fToT[id]     = tot;
  fCluSize[id] = cluSize;
  fIndex[id]   = index;
}

////////////////////////////////////////////////////////////////////////////////
/// Apply ColorMode to bins.

void CbmPointSetArray::ApplyColorMode()
{
  //parameters needed for color-calculation
  Double_t binTime = 12.;    // ns    max. length of one particle passing through detector
  Double_t binCol  = 35.;    // green ->allows for visual distinction of a track-time
  Double_t eveTime = 50;     // 50ns Event Length
  Double_t binToT  = 20;     // a.u.  max. ToT of hit in arbitray units (aka calibrated to mean of 5)
  TColor::SetPalette(1, 0);  //rainbow color palette with 50 colors from purple to red
  Int_t nCol = TColor::GetNumberOfColors();

  LOG(info) << "ApplyColorMode " << fColorMode << " with " << nCol << " colors";

  for (Int_t id = 0; id < fNPoints; id++) {
    switch (fColorMode) {
      case 1:  //according to hit-time
        // time until 3.5ns (=binTime) gets colors of spectrum from purple (=0) to green (=binCol) -> visual color gradient
        if (fTime[id] < binTime) {
          this->GetBin(fIndex[id])->SetMainColor(TColor::GetColorPalette(fTime[id] * binCol / binTime));
        }
        // time until end of event (=eveTime) gets color-spectrum from green to red (=49)
        else {
          this->GetBin(fIndex[id])
            ->SetMainColor(
              TColor::GetColorPalette(binCol + (fTime[id] - binTime) * (nCol - binCol - 1) / (eveTime - binTime)));
        }
        LOG(info) << "Color for id " << id << ", ind " << fIndex[id] << ", t " << fTime[id] << ": "
                  << this->GetBin(fIndex[id])->GetMainColor();

        break;
      case 2:  //according to Tot of hit
        // color calculated to represent ToT. high ToT -> yellow and red . low ToT -> purple and blue. max.ToT = binToT
        this->GetBin(fIndex[id])->SetMainColor(TColor::GetColorPalette(fToT[id] * (nCol - 1) / binToT));
        break;
      case 3:  //according to index of bin
        this->GetBin(fIndex[id])->SetMainColor(TColor::GetColorPalette(fIndex[id] - 1));
        break;
      case 4:  //all points with color red
        this->GetBin(fIndex[id])->SetMainColor(TColor::GetColorPalette(49));
        break;
      default:  //default with index coloring
        this->GetBin(fIndex[id])->SetMainColor(TColor::GetColorPalette(fIndex[id] - 1));
        return;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////
/// Apply MarkerMode to bins.

void CbmPointSetArray::ApplyMarkerMode()
{
  for (Int_t id = 0; id < fNPoints; id++) {
    switch (fMarkerMode) {
      case 1:  //according to cluSize in visually appealing manner
        this->GetBin(fIndex[id])->SetMarkerSize(1.25 + (fCluSize[id] - 1) * 0.5);
        break;
      case 2:  //according to CluSize with real dimesion on detector
        this->GetBin(fIndex[id])->SetMarkerSize(4 * fCluSize[id]);
        break;
      case 3:  //const. for all bins
        this->GetBin(fIndex[id])->SetMarkerSize(1.5);
        break;
      default: this->GetBin(fIndex[id])->SetMarkerSize(1.5); return;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////
/// Apply BBox-Titles to all bins.

void CbmPointSetArray::ApplyTitles()
{
  for (Int_t id = 0; id < fNPoints; id++) {
    this->GetBin(fIndex[id])
      ->SetTitle(Form("PointId = %d\nTime = %2.2fns\nClusterSize = %d\nToT = %2.2f[a.u.]", id, fTime[id], fCluSize[id],
                      fToT[id]));
    this->GetBin(fIndex[id])->ComputeBBox();
  }
}

ClassImp(CbmPointSetArray)
