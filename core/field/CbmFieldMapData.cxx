/** @file CbmFieldMapData.cxx
  * @copyright Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig, Denis Bertini [committer] **/

// -------------------------------------------------------------------------
// -----                    CbmFieldMapData source file                -----
// -----                V. Friese                14/02/06              -----
// -------------------------------------------------------------------------
#include "CbmFieldMapData.h"

#include "CbmFieldMap.h"  // for CbmFieldMap

#include <TArrayF.h>  // for TArrayF
#include <TNamed.h>   // for TNamed

// -------------   Default constructor  ----------------------------------
CbmFieldMapData::CbmFieldMapData()
  : TNamed()
  , fType(1)
  , fXmin(0.)
  , fXmax(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fNx(0)
  , fNy(0)
  , fNz(0)
  , fBx(nullptr)
  , fBy(nullptr)
  , fBz(nullptr)
{
}
// ------------------------------------------------------------------------


// -------------   Standard constructor   ---------------------------------
CbmFieldMapData::CbmFieldMapData(const char* mapName)
  : TNamed(mapName, "CBM Field Map Data")
  , fType(1)
  , fXmin(0.)
  , fXmax(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fNx(0)
  , fNy(0)
  , fNz(0)
  , fBx(nullptr)
  , fBy(nullptr)
  , fBz(nullptr)
{
}
// ------------------------------------------------------------------------


// -----   Constructor from CbmFieldMap   ------------------------------
CbmFieldMapData::CbmFieldMapData(const char* name, const CbmFieldMap& map)
  : TNamed(name, "CBM Field Map Data")
  , fType(map.GetType())
  , fXmin(map.GetXmin())
  , fXmax(map.GetXmax())
  , fYmin(map.GetYmin())
  , fYmax(map.GetYmax())
  , fZmin(map.GetZmin())
  , fZmax(map.GetZmax())
  , fNx(map.GetNx())
  , fNy(map.GetNy())
  , fNz(map.GetNz())
  , fBx(new TArrayF(*(map.GetBx())))
  , fBy(new TArrayF(*(map.GetBy())))
  , fBz(new TArrayF(*(map.GetBz())))
{
  // Take out scaling factor and convert from kG to T
  if (map.GetScale() == 0.) { Fatal("CbmFieldMap", "Zero scaling factor"); }
  Double_t factor = map.GetScale() * 10.;
  Int_t index     = 0;
  for (Int_t ix = 0; ix < fNx; ix++) {
    for (Int_t iy = 0; iy < fNy; iy++) {
      for (Int_t iz = 0; iz < fNz; iz++) {
        index = ix * fNy * fNz + iy * fNz + iz;
        if (fBx) (*fBx)[index] = (*fBx)[index] / factor;
        if (fBy) (*fBy)[index] = (*fBy)[index] / factor;
        if (fBz) (*fBz)[index] = (*fBz)[index] / factor;
      }  // z loop
    }    // y loop
  }      // x loop
}
// ------------------------------------------------------------------------


// ------------   Destructor   --------------------------------------------
CbmFieldMapData::~CbmFieldMapData()
{
  if (fBx) delete fBx;
  if (fBy) delete fBy;
  if (fBz) delete fBz;
}
// ------------------------------------------------------------------------


ClassImp(CbmFieldMapData)
