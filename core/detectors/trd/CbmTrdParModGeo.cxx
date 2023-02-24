/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci, Florian Uhlig [committer] */

#include "CbmTrdParModGeo.h"

#include <Logger.h>

#include <TGeoBBox.h>          // for TGeoBBox
#include <TGeoMatrix.h>        // for TGeoHMatrix
#include <TGeoPhysicalNode.h>  // for TGeoPhysicalNode

#include <string.h>  // for memcpy, memset

//___________________________________________________________________
CbmTrdParModGeo::CbmTrdParModGeo(const char* name, const char* title) : CbmTrdParMod(name, title), fNode(nullptr)
{
  // AB : Do not initialize the physical node (alignment aware) at this point as the alignment is not applied yet
  // fNode = new TGeoPhysicalNode(title);
}

//___________________________________________________________________
CbmTrdParModGeo::~CbmTrdParModGeo() { delete fNode; }

//___________________________________________________________________
Double_t CbmTrdParModGeo::GetDX() const { return ((TGeoBBox*) fNode->GetShape())->GetDX(); }

//___________________________________________________________________
Double_t CbmTrdParModGeo::GetDY() const { return ((TGeoBBox*) fNode->GetShape())->GetDY(); }

//___________________________________________________________________
Double_t CbmTrdParModGeo::GetDZ() const { return ((TGeoBBox*) fNode->GetShape())->GetDZ(); }

//_______________________________________________________________________________
void CbmTrdParModGeo::LocalToMaster(Double_t in[3], Double_t out[3]) const
{
  if (!fNode) return;
  fNode->GetMatrix()->LocalToMaster(in, out);
}

//___________________________________________________________________
void CbmTrdParModGeo::GetXYZ(Double_t xyz[3]) const
{
  memset(xyz, 0, 3 * sizeof(Double_t));
  Double_t gxyz[3];
  fNode->GetMatrix()->LocalToMaster(xyz, gxyz);
  memcpy(xyz, gxyz, 3 * sizeof(Double_t));
}

//___________________________________________________________________
bool CbmTrdParModGeo::SetNode()
{
  if (fNode) {
    LOG(warn) << "CbmTrdParModGeo::SetNode: Aligned info already loaded. Updating";
    delete fNode;
  }
  fNode = new TGeoPhysicalNode(GetTitle());
  if (!fNode) return false;

  LOG(debug) << GetName() << " : " << GetTitle();
  // fNode->GetMatrix()->Print();

  return true;
}

ClassImp(CbmTrdParModGeo)
