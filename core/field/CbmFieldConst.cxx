/* Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mohammad Al-Turany, Denis Bertini [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                    CbmFieldConst source file                  -----
// -----                Created 06/01/04  by M. Al/Turany              -----
// -----                Redesign 13/02/06  by V. Friese                -----
// -------------------------------------------------------------------------
#include "CbmFieldConst.h"

#include "CbmFieldPar.h"  // for CbmFieldPar

#include <FairField.h>  // for FairField

#include <TString.h>  // for operator<<

#include <iomanip>   // for operator<<, setw
#include <iostream>  // for operator<<, basic_ostream, endl, ostream

using std::cerr;
using std::cout;
using std::endl;
using std::setw;


// -----   Default constructor   -------------------------------------------
CbmFieldConst::CbmFieldConst()
  : FairField()
  , fXmin(0.)
  , fXmax(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fBx(0.)
  , fBy(0.)
  , fBz(0.)
{
  fType = 0;
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmFieldConst::CbmFieldConst(const char* name, Double_t xMin, Double_t xMax, Double_t yMin, Double_t yMax,
                             Double_t zMin, Double_t zMax, Double_t bX, Double_t bY, Double_t bZ)
  : FairField(name)
  , fXmin(xMin)
  , fXmax(xMax)
  , fYmin(yMin)
  , fYmax(yMax)
  , fZmin(zMin)
  , fZmax(zMax)
  , fBx(bX)
  , fBy(bY)
  , fBz(bZ)
{
  fType = 0;
}
// -------------------------------------------------------------------------


// --------   Constructor from CbmFieldPar   -------------------------------
CbmFieldConst::CbmFieldConst(CbmFieldPar* fieldPar)
  : FairField()
  , fXmin(0.)
  , fXmax(0.)
  , fYmin(0.)
  , fYmax(0.)
  , fZmin(0.)
  , fZmax(0.)
  , fBx(0.)
  , fBy(0.)
  , fBz(0.)
{
  if (!fieldPar) {
    cerr << "-W- CbmFieldConst::CbmFieldConst: empty parameter container!" << endl;
    fType = 0;
  }
  else {
    fXmin = fieldPar->GetXmin();
    fXmax = fieldPar->GetXmax();
    fYmin = fieldPar->GetYmin();
    fYmax = fieldPar->GetYmax();
    fZmin = fieldPar->GetZmin();
    fZmax = fieldPar->GetZmax();
    fBx   = fieldPar->GetBx();
    fBy   = fieldPar->GetBy();
    fBz   = fieldPar->GetBz();
    fType = fieldPar->GetType();
  }
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmFieldConst::~CbmFieldConst() {}
// -------------------------------------------------------------------------


// -----   Set field region   ----------------------------------------------
void CbmFieldConst::SetFieldRegion(Double_t xMin, Double_t xMax, Double_t yMin, Double_t yMax, Double_t zMin,
                                   Double_t zMax)
{
  fXmin = xMin;
  fXmax = xMax;
  fYmin = yMin;
  fYmax = yMax;
  fZmin = zMin;
  fZmax = zMax;
}
// -------------------------------------------------------------------------


// -----   Set field values   ----------------------------------------------
void CbmFieldConst::SetField(Double_t bX, Double_t bY, Double_t bZ)
{
  fBx = bX;
  fBy = bY;
  fBz = bZ;
}
// -------------------------------------------------------------------------


// -----   Get x component of field   --------------------------------------
Double_t CbmFieldConst::GetBx(Double_t x, Double_t y, Double_t z)
{
  if (x < fXmin || x > fXmax || y < fYmin || y > fYmax || z < fZmin || z > fZmax) return 0.;
  return fBx;
}
// -------------------------------------------------------------------------


// -----   Get y component of field   --------------------------------------
Double_t CbmFieldConst::GetBy(Double_t x, Double_t y, Double_t z)
{
  if (x < fXmin || x > fXmax || y < fYmin || y > fYmax || z < fZmin || z > fZmax) return 0.;
  return fBy;
}
// -------------------------------------------------------------------------


// -----   Get z component of field   --------------------------------------
Double_t CbmFieldConst::GetBz(Double_t x, Double_t y, Double_t z)
{
  if (x < fXmin || x > fXmax || y < fYmin || y > fYmax || z < fZmin || z > fZmax) return 0.;
  return fBz;
}
// -------------------------------------------------------------------------


// -----   Screen output   -------------------------------------------------
void CbmFieldConst::Print(Option_t*) const
{
  cout << "======================================================" << endl;
  cout << "----  " << fTitle << " : " << fName << endl;
  cout << "----" << endl;
  cout << "----  Field type    : constant" << endl;
  cout << "----" << endl;
  cout << "----  Field regions : " << endl;
  cout << "----        x = " << setw(4) << fXmin << " to " << setw(4) << fXmax << " cm" << endl;
  cout << "----        y = " << setw(4) << fYmin << " to " << setw(4) << fYmax << " cm" << endl;
  cout << "----        z = " << setw(4) << fZmin << " to " << setw(4) << fZmax << " cm" << endl;
  cout.precision(4);
  cout << "----  B = ( " << fBx << ", " << fBy << ", " << fBz << " ) kG" << endl;
  cout << "======================================================" << endl;
}
// -------------------------------------------------------------------------


ClassImp(CbmFieldConst)
