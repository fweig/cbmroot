/* Copyright (C) 2005-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                       CbmVertex source file                   -----
// -----                  Created 28/11/05  by V. Friese               -----
// -------------------------------------------------------------------------
#include "CbmVertex.h"

#include <Logger.h>  // for Logger, LOG

#include <TMatrixTSym.h>    // for TMatrixTSym
#include <TMatrixTUtils.h>  // for TMatrixTRow, TMatrixTRow_const
#include <TNamed.h>         // for TNamed

#include <iomanip>  // for operator<<, setprecision
#include <sstream>  // for operator<<, basic_ostream, stringstream
#include <string>   // for char_traits

using namespace std;


// -----   Default constructor   -------------------------------------------
CbmVertex::CbmVertex()
  : TNamed("Vertex", "Global")
  , fX(0.)
  , fY(0.)
  , fZ(0.)
  , fChi2(0.)
  , fNDF(0)
  , fNTracks(0)
  , fCovMatrix()
{
  for (int32_t i = 0; i < 6; i++)
    fCovMatrix[i] = 0;
}
// -------------------------------------------------------------------------

// -----   Constructor with name and title   -------------------------------
CbmVertex::CbmVertex(const char* name, const char* title)
  : TNamed(name, title)
  , fX(0.)
  , fY(0.)
  , fZ(0.)
  , fChi2(0.)
  , fNDF(0)
  , fNTracks(0)
  , fCovMatrix()
{
  for (int32_t i = 0; i < 6; i++)
    fCovMatrix[i] = 0;
}
// -------------------------------------------------------------------------


// -----   Constructor with all parameters   -------------------------------
CbmVertex::CbmVertex(const char* name, const char* title, double x, double y, double z, double chi2, int32_t ndf,
                     int32_t nTracks, const TMatrixFSym& covMat)
  : TNamed(name, title)
  , fX(x)
  , fY(y)
  , fZ(z)
  , fChi2(chi2)
  , fNDF(ndf)
  , fNTracks(nTracks)
  , fCovMatrix()
{
  if ((covMat.GetNrows() != 3) && (covMat.GetNcols() != 3)) {
    LOG(error) << "Wrong dimension of passed covariance matrix. Clear the "
                  "covariance matrix.";
    for (int32_t i = 0; i < 6; i++)
      fCovMatrix[i] = 0;
  }
  else {
    int32_t index = 0;
    for (int32_t i = 0; i < 3; i++) {
      for (int32_t j = i; j < 3; j++)
        fCovMatrix[index++] = covMat[i][j];
    }
  }
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmVertex::~CbmVertex() {}
// -------------------------------------------------------------------------


// -----   Public method Print   -------------------------------------------
void CbmVertex::Print(Option_t*) const
{
  double chi2ndf;
  if (fNDF) chi2ndf = fChi2 / double(fNDF);
  else
    chi2ndf = 0.;
  LOG(info) << "Vertex coord. (" << fX << "," << fY << "," << fZ << ") cm, "
            << "chi2/ndf = " << chi2ndf << ", " << fNTracks << " tracks used";
}
// -------------------------------------------------------------------------


// -----   Accessor to covariance matrix    --------------------------------
void CbmVertex::CovMatrix(TMatrixFSym& covMat) const
{
  int32_t index = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      covMat[i][j] = fCovMatrix[index];
      covMat[j][i] = fCovMatrix[index];
      index++;
    }
  }
}
// -------------------------------------------------------------------------


// -----   Accessor to covariance matrix elements   ------------------------
double CbmVertex::GetCovariance(int32_t i, int32_t j) const
{
  TMatrixFSym* mat = new TMatrixFSym(3);
  CovMatrix(*mat);
  double element = (*mat)[i][j];
  delete mat;
  return element;
}
// -------------------------------------------------------------------------


// -----   Public method SetVertex   ---------------------------------------
void CbmVertex::SetVertex(double x, double y, double z, double chi2, int32_t ndf, int32_t nTracks,
                          const TMatrixFSym& covMat)
{
  fX       = x;
  fY       = y;
  fZ       = z;
  fChi2    = chi2;
  fNDF     = ndf;
  fNTracks = nTracks;
  if ((covMat.GetNrows() != 3) && (covMat.GetNcols() != 3)) {
    LOG(error) << "Wrong dimension of passed covariance matrix. Clear the "
                  "covariance matrix.";
    for (int32_t i = 0; i < 6; i++)
      fCovMatrix[i] = 0;
  }
  else {
    int32_t index = 0;
    for (int32_t i = 0; i < 3; i++) {
      for (int32_t j = i; j < 3; j++)
        fCovMatrix[index++] = covMat[i][j];
    }
  }
}
// -------------------------------------------------------------------------


// -----   Public method Reset   -------------------------------------------
void CbmVertex::Reset()
{
  fX = fY = fZ = fChi2 = 0.;
  fNDF = fNTracks = 0;
  for (int32_t i = 0; i < 6; i++)
    fCovMatrix[i] = 0;
}
// -------------------------------------------------------------------------


// --- String output  ------------------------------------------------------
string CbmVertex::ToString() const
{

  double chi2ndf = (fNDF ? fChi2 / double(fNDF) : 0.);
  stringstream ss;
  ss << "Vertex: position (" << fixed << setprecision(4) << fX << ", " << fY << ", " << fZ
     << ") cm, chi2/ndf = " << chi2ndf << ", tracks used: " << fNTracks;
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmVertex)
