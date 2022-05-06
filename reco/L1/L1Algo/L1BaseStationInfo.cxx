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
#include "L1Assert.h"
#include "L1BaseStationInfo.h"
#include "L1Def.h"
#include "L1Parameters.h"
// C++ STL
#include <iomanip>
#include <sstream>
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
  fInitController.SetFlag(InitKey::keDetectorID);
  fInitController.SetFlag(InitKey::keStationID);
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
  , fInitController(other.fInitController)
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
  this->Swap(other);
}


//
// GETTERS
//
//----------------------------------------------------------------------------------------------------------------------//
//
const L1Station& L1BaseStationInfo::GetL1Station() const
{
  std::stringstream aStream;
  aStream << "L1BaseStationInfo::GetL1Station: attempt to get an L1Staion object from uninitialized L1BaseStation with "
          << "stationID = " << fStationID << " and detectorID = " << static_cast<int>(fDetectorID);
  L1MASSERT(0, fInitController.IsFinalized(), aStream.str().c_str());
  return fL1Station;
}

//
// SETTERS
//

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetDetectorID(L1DetectorID inID)
{
  if (!fInitController.GetFlag(InitKey::keDetectorID)) {
    fDetectorID = inID;
    fInitController.SetFlag(InitKey::keDetectorID);
  }
  else {
    LOG(warn) << "L1BaseStationInfo::SetDetectorID: Attempt of detector ID redifinition";
  }
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetRmax(double inRmax)
{
  fL1Station.Rmax = inRmax;
  fInitController.SetFlag(InitKey::keRmax);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetRmin(double inRmin)
{
  fL1Station.Rmin = inRmin;
  fInitController.SetFlag(InitKey::keRmin);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetFieldSlice(const double* Cx, const double* Cy, const double* Cz)
{
  if (fInitController.GetFlag(InitKey::keFieldSlice)) {
    LOG(warn) << "L1BaseStationInfo::SetFieldSlice: Attempt to redifine field slice for station with detectorID = "
              << static_cast<int>(fDetectorID) << " and stationID = " << fStationID << ". Redifinition ignored";
    return;
  }

  for (int idx = 0; idx < L1Parameters::kMaxNFieldApproxCoefficients; ++idx) {
    fL1Station.fieldSlice.cx[idx] = Cx[idx];
    fL1Station.fieldSlice.cy[idx] = Cy[idx];
    fL1Station.fieldSlice.cz[idx] = Cz[idx];
  }

  fInitController.SetFlag(InitKey::keFieldSlice);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetFieldSlice(const std::function<void(const double (&xyz)[3], double (&B)[3])>& getFieldValue)
{
  if (fInitController.GetFlag(InitKey::keFieldSlice)) {
    LOG(warn) << "L1BaseStationInfo::SetFieldSlice: Attempt to redifine field slice for station with detectorID = "
              << static_cast<int>(fDetectorID) << " and stationID = " << fStationID << ". Redifinition ignored";
    return;
  }

  L1MASSERT(0, fInitController.GetFlag(InitKey::keZ),
            "Attempt to set magnetic field slice before setting z position of the station");
  L1MASSERT(0, fInitController.GetFlag(InitKey::keXmax),
            "Attempt to set magnetic field slice before Xmax size of the station");
  L1MASSERT(0, fInitController.GetFlag(InitKey::keYmax),
            "Attempt to set magnetic field slice before Ymax size of the station");
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

  fInitController.SetFlag(InitKey::keFieldSlice);
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

  fInitController.SetFlag(InitKey::keStripsFrontPhi);
  fInitController.SetFlag(InitKey::keStripsFrontSigma);
  fInitController.SetFlag(InitKey::keStripsBackPhi);
  fInitController.SetFlag(InitKey::keStripsBackSigma);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetMaterial(double inThickness, double inRL)
{
  L1MASSERT(0, inRL, "Attempt of entering zero inRL (radiational length) value");

  fL1Station.materialInfo.thick       = inThickness;
  fL1Station.materialInfo.RL          = inRL;
  fL1Station.materialInfo.RadThick    = fL1Station.materialInfo.thick / fL1Station.materialInfo.RL;
  fL1Station.materialInfo.logRadThick = log(fL1Station.materialInfo.RadThick);
  fInitController.SetFlag(InitKey::keMaterialInfoThick);
  fInitController.SetFlag(InitKey::keMaterialInfoRL);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetStationID(int inID)
{
  if (!fInitController.GetFlag(InitKey::keStationID)) {
    fStationID = inID;
    fInitController.SetFlag(InitKey::keStationID);
  }
  else {
    LOG(warn) << "L1BaseStationInfo::SetStationID: Attempt of station ID redifinition";
  }
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetStationType(int inType)
{
  if (!fInitController.GetFlag(InitKey::keType)) {
    fL1Station.type = inType;
    fInitController.SetFlag(InitKey::keType);
  }
  else {
    LOG(warn) << "L1BaseStationInfo::SetStationType: Attempt of station type redifinition";
  }
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetXmax(double aSize)
{
  fXmax = aSize;
  fInitController.SetFlag(InitKey::keXmax);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetYmax(double aSize)
{
  fYmax = aSize;
  fInitController.SetFlag(InitKey::keYmax);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetTimeInfo(int inTimeInfo)
{
  fL1Station.timeInfo = inTimeInfo;
  fInitController.SetFlag(InitKey::keTimeInfo);
}

//----------------------------------------------------------------------------------------------------------------------//
//
void L1BaseStationInfo::SetZ(double inZ)
{
  fL1Station.z = inZ;  // setting simd vector of single-precision floats, which is passed to high performanced L1Algo
  fZPos        = inZ;  // setting precised value to use in field approximation etc
  fInitController.SetFlag(InitKey::keZ);
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
  std::swap(fInitController, other.fInitController);
}

//----------------------------------------------------------------------------------------------------------------------//
//
std::string L1BaseStationInfo::ToString(int verbosityLevel, int indentLevel) const
{
  std::stringstream aStream {};
  constexpr char indentChar = '\t';
  std::string indent(indentLevel, indentChar);

  if (verbosityLevel == 0) {
    aStream << indent << "L1BaseStationInfo object: {stationID, detectorID, z, address} = {" << fStationID << ", "
            << static_cast<int>(fDetectorID) << ", " << fZPos << ", " << this << '}';
  }
  else if (verbosityLevel > 0) {
    aStream << indent << "L1BaseStationInfo object: at " << this << '\n';
    aStream << indent << indentChar << "Station ID:              " << fStationID << '\n';
    aStream << indent << indentChar << "Detector ID:             " << static_cast<int>(fDetectorID) << '\n';
    aStream << indent << indentChar << "L1Station object:" << '\n';
    aStream << fL1Station.ToString(verbosityLevel - 1, indentLevel + 1) << '\n';
    aStream << indent << indentChar << "Additional fields:\n";
    aStream << indent << indentChar << indentChar << "Xmax:                    " << fXmax << '\n';
    aStream << indent << indentChar << indentChar << "Ymax:                    " << fYmax << '\n';
  }
  return aStream.str();
}
