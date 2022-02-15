/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1BaseStationInfo.cxx
 * @bried Realization of L1BaseStationInfo class large methods
 * @since 18.01.2022
 *
 * The class is implemented to connect concrete experimental setup (CBM or BMN in CbmL1 
 * or BmnL1) with general L1Tracking algorithm. Each derived class must contain general
 * algorithms sutable for the particular station type.
 * 
 ***********************************************************************************************************/

// FairRoot
#include <FairLogger.h>

// L1Algo core
#include "L1BaseStationInfo.h"
#include "L1Def.h"
#include "L1Parameters.h"

// C++ STL
#include <iomanip>
#include <utility>

//
// CONSTRUCTORS AND DESTRUCTOR
//

//----------------------------------------------------------------------------------------------------------------------//
//
L1BaseStationInfo::L1BaseStationInfo() noexcept
{
  LOG(debug) << "L1BaseStationInfo: Default constructor called for " << this << '\n';  // Temporary
}

//----------------------------------------------------------------------------------------------------------------------//
//
L1BaseStationInfo::L1BaseStationInfo(L1DetectorID detectorID, int stationID) noexcept
  : fDetectorID(detectorID)
  , fStationID(stationID)
{
  LOG(debug) << "L1BaseStationInfo: Constructor (detectorID, stationID) called for " << this << '\n';  // Temporary
  fInitFlags[keDetectorID] = true;
  fInitFlags[keStationID]  = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
L1BaseStationInfo::~L1BaseStationInfo() noexcept
{
  LOG(debug) << "L1BaseStationInfo: Destructor called for " << this << '\n';  // Temporary
}

//----------------------------------------------------------------------------------------------------------------------//
//
L1BaseStationInfo::L1BaseStationInfo(const L1BaseStationInfo& other) noexcept
  : fDetectorID(other.fDetectorID)
  , fStationID(other.fStationID)
  , fXmax(other.fXmax)
  , fYmax(other.fYmax)
  , fZPos(other.fZPos)
  , fL1Station(other.fL1Station)
  , fInitFlags(other.fInitFlags)
{
  LOG(debug) << "L1BaseStationInfo: Copy constructor called: " << &other << " was copied into " << this;
}

//----------------------------------------------------------------------------------------------------------------------//
//
L1BaseStationInfo::L1BaseStationInfo(L1BaseStationInfo&& other) noexcept
{
  LOG(debug) << "L1BaseStationInfo: Move constructor called: " << &other << " was moved into " << this;
  this->Swap(other);
}

//----------------------------------------------------------------------------------------------------------------------//
//
L1BaseStationInfo& L1BaseStationInfo::operator=(const L1BaseStationInfo& other) noexcept
{
  LOG(debug) << "L1BaseStationInfo: Copy operator= called for " << &other << " was copied into" << this;
  if (this != &other) { L1BaseStationInfo(other).Swap(*this); }
  return *this;
}

//----------------------------------------------------------------------------------------------------------------------//
//
L1BaseStationInfo& L1BaseStationInfo::operator=(L1BaseStationInfo&& other) noexcept
{
  LOG(debug) << "L1BaseStationInfo: Move operator= called for " << &other << " was copied into" << this;
  if (this != &other) {
    L1BaseStationInfo tmp(std::move(other));
    this->Swap(tmp);
  }
  return *this;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::Swap(L1BaseStationInfo& other) noexcept
{
  std::swap(fDetectorID, other.fDetectorID);
  std::swap(fStationID, other.fStationID);
  std::swap(fXmax, other.fXmax);
  std::swap(fYmax, other.fYmax);
  std::swap(fZPos, other.fZPos);
  std::swap(fL1Station, other.fL1Station);
  std::swap(fInitFlags, other.fInitFlags);
}

//
// BASIC METHODS
//

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::Print(int verbosity) const
{
  if (verbosity == 0) {
    LOG(info) << "L1BaseStationInfo object: {stationID, detectorID, address} = {" << fStationID << ", "
              << static_cast<int>(fDetectorID) << ", " << this << '}';
  }
  else if (verbosity > 0) {
    LOG(info) << "L1BaseStationInfo object: at " << this;
    LOG(info) << "\tStation ID:              " << fStationID;
    LOG(info) << "\tDetector ID:             " << static_cast<int>(fDetectorID);
    fL1Station.Print(verbosity - 1);
    LOG(info) << "\tAdditional fields:";
    LOG(info) << "\t\tXmax:                    " << fXmax;
    LOG(info) << "\t\tYmax:                    " << fYmax;
  }
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::Reset()
{
  L1BaseStationInfo other;
  std::swap(*this, other);
}


//
// GETTERS
//

//----------------------------------------------------------------------------------------------------------------------//
//
const L1Station& L1BaseStationInfo::GetL1Station() const
{
  bool isStationInitialized = IsInitialized();
  if (!isStationInitialized) {
    LOG(error)
      << "L1BaseStationInfo::GetL1Station: attempt to get an L1Staion object from uninitialized L1BaseStation with "
      << "stationID = " << fStationID << " and detectorID = " << static_cast<int>(fDetectorID);
    assert((!isStationInitialized));
  }
  return fL1Station;
}


//
// SETTERS
//

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetStationID(int inID)
{
  if (!fInitFlags[keStationID]) {
    fStationID              = inID;
    fInitFlags[keStationID] = true;
  }
  else {
    LOG(warn) << "L1BaseStationInfo::SetStationID: Attempt of station ID redifinition";
  }
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetDetectorID(L1DetectorID inID)
{
  if (!fInitFlags[keDetectorID]) {
    fDetectorID              = inID;
    fInitFlags[keDetectorID] = true;
  }
  else {
    LOG(warn) << "L1BaseStationInfo::SetDetectorID: Attempt of detector ID redifinition";
  }
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetStationType(int inType)
{
  if (!fInitFlags[keType]) {
    fL1Station.type    = inType;
    fInitFlags[keType] = true;
  }
  else {
    LOG(warn) << "L1BaseStationInfo::SetStationType: Attempt of station type redifinition";
  }
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetTimeInfo(int inTimeInfo)
{
  fL1Station.timeInfo    = inTimeInfo;
  fInitFlags[keTimeInfo] = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetZ(double inZ)
{
  fL1Station.z    = inZ;  // setting simd vector of single-precision floats, which is passed to high performanced L1Algo
  fZPos           = inZ;  // setting precised value to use in field approximation etc
  fInitFlags[keZ] = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetRmin(double inRmin)
{
  fL1Station.Rmin    = inRmin;
  fInitFlags[keRmin] = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetRmax(double inRmax)
{
  fL1Station.Rmax    = inRmax;
  fInitFlags[keRmax] = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetMaterial(double inThickness, double inRL)
{
#ifndef L1_NO_ASSERT  // check for zero denominator
  L1_ASSERT(inRL, "Attempt of entering zero inRL (radiational length) value");
#endif
  fL1Station.materialInfo.thick       = inThickness;
  fL1Station.materialInfo.RL          = inRL;
  fL1Station.materialInfo.RadThick    = fL1Station.materialInfo.thick / fL1Station.materialInfo.RL;
  fL1Station.materialInfo.logRadThick = log(fL1Station.materialInfo.RadThick);
  fInitFlags[keMaterialInfoThick]     = true;
  fInitFlags[keMaterialInfoRL]        = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetFieldSlice(const double* Cx, const double* Cy, const double* Cz)
{
  if (fInitFlags[keFieldSlice]) {
    LOG(warn) << "L1BaseStationInfo::SetFieldSlice: Attempt to redifine field slice for station with detectorID = "
              << static_cast<int>(fDetectorID) << " and stationID = " << fStationID << ". Redifinition ignored";
    return;
  }

  for (int idx = 0; idx < L1Parameters::kMaxNFieldApproxCoefficients; ++idx) {
    fL1Station.fieldSlice.cx[idx] = Cx[idx];
    fL1Station.fieldSlice.cy[idx] = Cy[idx];
    fL1Station.fieldSlice.cz[idx] = Cz[idx];
  }

  fInitFlags[keFieldSlice] = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetFieldSlice(const std::function<void(const double (&xyz)[3], double (&B)[3])>& getFieldValue)
{
  if (fInitFlags[keFieldSlice]) {
    LOG(warn) << "L1BaseStationInfo::SetFieldSlice: Attempt to redifine field slice for station with detectorID = "
              << static_cast<int>(fDetectorID) << " and stationID = " << fStationID << ". Redifinition ignored";
    return;
  }

#ifndef L1_NO_ASSERT  // check for zero denominator
  L1_ASSERT(fInitFlags[keZ], "Attempt to set magnetic field slice before setting z position of the station");
  L1_ASSERT(fInitFlags[keXmax], "Attempt to set magnetic field slice before setting Xmax size of the station");
  L1_ASSERT(fInitFlags[keYmax], "Attempt to set magnetic field slice before setting Ymax size of the station");
#endif
  // TODO: Change names of variables according to convention (S.Zh.)
  constexpr int M = L1Parameters::kMaxFieldApproxPolynomialOrder;
  constexpr int N = L1Parameters::kMaxNFieldApproxCoefficients;
  constexpr int D = 3;  ///> number of dimensions

  // SLE initialization
  double A[N][N + D] = {};  // augmented matrix
  double dx          = (fXmax / N / 2 < 1.) ? fXmax / N / 4. : 1.;
  double dy          = (fYmax / N / 2 < 1.) ? fYmax / N / 4. : 1.;

  for (double x = -fXmax; x <= fXmax; x += dx) {
    for (double y = -fYmax; y <= fYmax; y += dy) {
      double r = sqrt(fabs(x * x / fXmax / fXmax + y / fYmax * y / fYmax));
      if (r > 1.) { continue; }
      double p[D] = {x, y, fZPos};
      double B[D] = {};
      getFieldValue(p, B);

      double m[N] = {1};
      m[0]        = 1;
      for (int i = 1; i <= M; ++i) {
        int k = (i - 1) * i / 2;
        int l = i * (i + 1) / 2;
        for (int j = 0; j < i; ++j) {
          m[l + j] = x * m[k + j];
        }
        m[l + i] = y * m[k + i - 1];
      }

      double w = 1. / (r * r + 1);
      for (int i = 0; i < N; ++i) {
        // fill the left part of the matrix
        for (int j = 0; j < N; ++j) {
          A[i][j] += w * m[i] * m[j];
        }
        // fill the right part of the matrix
        for (int j = 0; j < D; ++j) {
          A[i][N + j] += w * B[j] * m[i];
        }
      }
    }
  }

  // SLE solution (Gaussian elimination)
  //
  for (int kCol = 0; kCol < N - 1; ++kCol) {
    for (int jRow = kCol + 1; jRow < N; ++jRow) {
      double factor = A[jRow][kCol] / A[kCol][kCol];
      for (int iCol = kCol; iCol < N + D; ++iCol) {
        A[jRow][iCol] -= factor * A[kCol][iCol];
      }
    }
  }
  for (int kCol = N - 1; kCol > 0; --kCol) {
    for (int jRow = kCol - 1; jRow >= 0; --jRow) {
      double factor = A[jRow][kCol] / A[kCol][kCol];
      for (int iCol = kCol; iCol < N + D; ++iCol) {
        A[jRow][iCol] -= factor * A[kCol][iCol];
      }
    }
  }

  for (int j = 0; j < N; ++j) {
    fL1Station.fieldSlice.cx[j] = A[j][N + 0] / A[j][j];
    fL1Station.fieldSlice.cy[j] = A[j][N + 1] / A[j][j];
    fL1Station.fieldSlice.cz[j] = A[j][N + 2] / A[j][j];
  }

  fInitFlags[keFieldSlice] = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetFrontBackStripsGeometry(double frontPhi, double frontSigma, double backPhi, double backSigma)
{
  //----- Original code from L1Algo ---------------------------------------------------------------------//
  double cFront = cos(frontPhi);
  double sFront = sin(frontPhi);
  double cBack  = cos(backPhi);
  double sBack  = sin(backPhi);

  // NOTE: Here additional double variables are used to save the precission

  fL1Station.frontInfo.cos_phi = cFront;
  fL1Station.frontInfo.sin_phi = sFront;
  fL1Station.frontInfo.sigma2  = frontSigma * frontSigma;

  fL1Station.backInfo.cos_phi = cBack;
  fL1Station.backInfo.sin_phi = sBack;
  fL1Station.backInfo.sigma2  = backSigma * backSigma;

  //if (fabs(b_phi - f_phi) < .1) {
  //  double th  = b_phi - f_phi;
  //  double det = cos(th);
  //  det *= det;
  //  fL1Station.XYInfo.C00 = (s_b * s_b * f_sigma * f_sigma + s_f * s_f * b_sigma * b_sigma) / det;
  //  fL1Station.XYInfo.C10 = -(s_b * c_b * f_sigma * f_sigma + s_f * c_f * b_sigma * b_sigma) / det;
  //  fL1Station.XYInfo.C11 = (c_b * c_b * f_sigma * f_sigma + c_f * c_f * b_sigma * b_sigma) / det;
  //} else {
  //  double det = c_f * s_b - s_f * c_b;
  //  det *= det;
  //  fL1Station.XYInfo.C00 = (s_b * s_b * f_sigma * f_sigma + s_f * s_f * b_sigma * b_sigma) / det;
  //  fL1Station.XYInfo.C10 = -(s_b * c_b * f_sigma * f_sigma + s_f * c_f * b_sigma * b_sigma) / det;
  //  fL1Station.XYInfo.C11 = (c_b * c_b * f_sigma * f_sigma + c_f * c_f * b_sigma * b_sigma) / det;
  //}

  double det = (fabs(backPhi - frontPhi) < 0.1) ? cos(backPhi - frontPhi) : (cFront * sBack - sFront * cBack);
  det *= det;
  fL1Station.XYInfo.C00 = (sBack * sBack * frontSigma * frontSigma + sFront * sFront * backSigma * backSigma) / det;
  fL1Station.XYInfo.C10 = -(sBack * cBack * frontSigma * frontSigma + sFront * cFront * backSigma * backSigma) / det;
  fL1Station.XYInfo.C11 = (cBack * cBack * frontSigma * frontSigma + cFront * cFront * backSigma * backSigma) / det;

  fL1Station.xInfo.cos_phi = -sFront / (cFront * sBack - cBack * sFront);
  fL1Station.xInfo.sin_phi = sBack / (cFront * sBack - cBack * sFront);
  fL1Station.xInfo.sigma2  = fL1Station.XYInfo.C00;

  fL1Station.yInfo.cos_phi = cBack / (cBack * sFront - cFront * sBack);
  fL1Station.yInfo.sin_phi = -cFront / (cBack * sFront - cFront * sBack);
  fL1Station.yInfo.sigma2  = fL1Station.XYInfo.C11;
  //-----------------------------------------------------------------------------------------------------//

  fInitFlags[keStripsFrontPhi]   = true;
  fInitFlags[keStripsFrontSigma] = true;
  fInitFlags[keStripsBackPhi]    = true;
  fInitFlags[keStripsBackSigma]  = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetXmax(double aSize)
{
  fXmax              = aSize;
  fInitFlags[keXmax] = true;
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetYmax(double aSize)
{
  fYmax              = aSize;
  fInitFlags[keYmax] = true;
}
