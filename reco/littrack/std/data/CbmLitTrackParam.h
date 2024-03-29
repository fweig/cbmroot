/* Copyright (C) 2008-2017 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Timur Ablyazimov */

/**
 * \file CbmLitTrackParam.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 * \brief Data class for track parameters.
 **/

#ifndef CBMLITTRACKPARAM_H_
#define CBMLITTRACKPARAM_H_

#include "base/CbmLitFloat.h"

#include "TVector3.h"

#include <sstream>
#include <string>
#include <vector>

#include <cmath>

using std::endl;
using std::string;
using std::stringstream;
using std::vector;

/**
 * \class CbmLitTrackParam
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 * \brief Data class for track parameters.
 **/
class CbmLitTrackParam {
public:
  static litfloat fSpeedOfLight;

public:
  /**
    * \brief Constructor.
    */
  CbmLitTrackParam() : fX(0.), fY(0.), fZ(0.), fTx(0.), fTy(0.), fQp(0.), fTime(0.), fCovMatrix(21, 0.) {}

  /**
    * \brief Destructor.
    */
  virtual ~CbmLitTrackParam() {}

  /* Getters */
  litfloat GetX() const { return fX; }
  litfloat GetY() const { return fY; }
  litfloat GetZ() const { return fZ; }
  litfloat GetTx() const { return fTx; }
  litfloat GetTy() const { return fTy; }
  litfloat GetQp() const { return fQp; }
  litfloat GetTime() const { return fTime; }
  litfloat GetCovariance(int index) const { return fCovMatrix[index]; }
  const vector<litfloat>& GetCovMatrix() const { return fCovMatrix; }

  /* Setters */
  void SetX(litfloat x) { fX = x; }
  void SetY(litfloat y) { fY = y; }
  void SetZ(litfloat z) { fZ = z; }
  void SetTx(litfloat tx) { fTx = tx; }
  void SetTy(litfloat ty) { fTy = ty; }
  void SetQp(litfloat qp) { fQp = qp; }
  void SetTime(litfloat t) { fTime = t; }
  void SetCovMatrix(const vector<litfloat>& C) { fCovMatrix.assign(C.begin(), C.end()); }
  void SetCovariance(int index, litfloat cov) { fCovMatrix[index] = cov; }

  /**
    * \brief Return direction cosines.
    * \param[out] nx Output direction cosine for OX axis.
    * \param[out] ny Output direction cosine for OY axis.
    * \param[out] nz Output direction cosine for OZ axis.
    */
  void GetDirCos(litfloat& nx, litfloat& ny, litfloat& nz) const
  {
    litfloat p    = (std::abs(fQp) != 0.) ? 1. / std::abs(fQp) : 1.e20;
    litfloat pz   = std::sqrt(p * p / (fTx * fTx + fTy * fTy + 1));
    litfloat px   = fTx * pz;
    litfloat py   = fTy * pz;
    TVector3 unit = TVector3(px, py, pz).Unit();
    nx            = unit.X();
    ny            = unit.Y();
    nz            = unit.Z();
  }

  /**
    * \brief Return state vector as vector.
    * \return State vector as vector.
    */
  vector<litfloat> GetStateVector() const
  {
    vector<litfloat> state(6, 0.);
    state[0] = GetX();
    state[1] = GetY();
    state[2] = GetTx();
    state[3] = GetTy();
    state[4] = GetQp();
    state[5] = GetTime();
    return state;
  }

  /**
    * \brief Set parameters from vector.
    * \param[in] x State vector.
    */
  void SetStateVector(const vector<litfloat>& x)
  {
    SetX(x[0]);
    SetY(x[1]);
    SetTx(x[2]);
    SetTy(x[3]);
    SetQp(x[4]);
    SetTime(x[5]);
  }

  /**
    * \brief Return string representation of class.
    * \return String representation of class.
    */
  string ToString() const
  {
    stringstream ss;
    ss << "TrackParam: pos=(" << fX << "," << fY << "," << fZ << ") tx=" << fTx << " ty=" << fTy << " qp=" << fQp
       << ", fTime=" << fTime;  // << std::endl;
                                // ss << "cov: ";
    // for (Int_t i = 0; i < 15; i++) ss << fCovMatrix[i] << " ";
    // ss << endl;
    ss.precision(3);
    ss << " cov: x=" << fCovMatrix[0] << " y=" << fCovMatrix[6] << " tx=" << fCovMatrix[11] << " ty=" << fCovMatrix[15]
       << " q/p=" << fCovMatrix[18] << ", time=" << fCovMatrix[20] << endl;
    return ss.str();
  }

private:
  litfloat fX, fY, fZ;  // X, Y, Z coordinates in [cm]
  litfloat fTx, fTy;    // Slopes: tx=dx/dz, ty=dy/dz
  litfloat fQp;         // Q/p: Q is a charge (+/-1), p is momentum in [GeV/c]
  litfloat fTime;

  /* Covariance matrix.
    * Upper triangle symmetric matrix.
    * a[0,0..4], a[1,1..4], a[2,2..4], a[3,3..4], a[4,4] */
  vector<litfloat> fCovMatrix;
};

#endif /*CBMLITTRACKPARAM_H_*/
