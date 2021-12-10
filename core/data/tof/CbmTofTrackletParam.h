/* Copyright (C) 2015-2021 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer], Florian Uhlig */

/**
 * \file CbmTofTrackletParam.h
 * \author Norbert Herrmann <n.herrmann@gsi.de>
 * \date 2020
 * \brief Data class for track parameters.
 **/

#ifndef CBMTOFTRACKLETPARAM_H_
#define CBMTOFTRACKLETPARAM_H_

#include <Rtypes.h>    // for ClassDef
#include <TObject.h>   // for TObject
#include <TVector3.h>  // for TVector3

#include <cstdint>
#include <sstream>  // for operator<<, basic_ostream, stringstream, cha...
#include <vector>   // for vector

#include <cmath>  // for abs, sqrt

/**
 * built by on 
 * \class CbmLitTrackletParam
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 * \brief Data class for track parameters.
 **/
class CbmTofTrackletParam : public TObject {
public:
  /**
    * \brief Constructor.
    */
  CbmTofTrackletParam()
    : fX(0.)
    , fY(0.)
    , fZ(0.)
    , fT(0.)
    , fTx(0.)
    , fTy(0.)
    , fTt(0.)
    , fQp(0.)
    , fLz(0.)
    , fChiSq(0.)
    , fCovMatrix(15, 0.)
  {
  }

  /**
    * \brief Destructor.
    */
  virtual ~CbmTofTrackletParam() {}

  /* Getters */
  double GetX() const { return fX; }
  double GetY() const { return fY; }
  double GetZ() const { return fZ; }
  double GetT() const { return fT; }
  double GetLz() const { return fLz; }
  double GetTx() const { return fTx; }
  double GetTy() const { return fTy; }
  double GetTt() const { return fTt; }
  double GetQp() const { return fQp; }
  double GetChiSq() const { return fChiSq; }
  double GetCovariance(int index) const { return fCovMatrix[index]; }
  const std::vector<double>& GetCovMatrix() const { return fCovMatrix; }

  /* Setters */
  void SetX(double x) { fX = x; }
  void SetY(double y) { fY = y; }
  void SetZ(double z) { fZ = z; }
  void SetT(double t) { fT = t; }
  void SetLz(double lz) { fLz = lz; }
  void SetTx(double tx) { fTx = tx; }
  void SetTy(double ty) { fTy = ty; }
  void SetTt(double tt) { fTt = tt; }
  void SetQp(double qp) { fQp = qp; }
  void SetChiSq(double v) { fChiSq = v; }
  void SetCovMatrix(const std::vector<double>& C) { fCovMatrix.assign(C.begin(), C.end()); }
  void SetCovariance(int index, double cov) { fCovMatrix[index] = cov; }

  /**
    * \brief Return direction cosines.
    * \param[out] nx Output direction cosine for OX axis.
    * \param[out] ny Output direction cosine for OY axis.
    * \param[out] nz Output direction cosine for OZ axis.
    */
  void GetDirCos(double& nx, double& ny, double& nz) const
  {
    double p      = (std::abs(fQp) != 0.) ? 1. / std::abs(fQp) : 1.e20;
    double pz     = std::sqrt(p * p / (fTx * fTx + fTy * fTy + 1));
    double px     = fTx * pz;
    double py     = fTy * pz;
    TVector3 unit = TVector3(px, py, pz).Unit();
    nx            = unit.X();
    ny            = unit.Y();
    nz            = unit.Z();
  }

  /**
    * \brief Return state vector as vector.
    * \return State vector as vector.
    */
  std::vector<double> GetStateVector() const
  {
    std::vector<double> state(5, 0.);
    state[0] = GetX();
    state[1] = GetY();
    state[2] = GetTx();
    state[3] = GetTy();
    state[4] = GetQp();
    return state;
  }

  /**
    * \brief Set parameters from vector.
    * \param[in] x State vector.
    */
  void SetStateVector(const std::vector<double>& x)
  {
    SetX(x[0]);
    SetY(x[1]);
    SetTx(x[2]);
    SetTy(x[3]);
    SetQp(x[4]);
  }

  /**
    * \brief Return string representation of class.
    * \return String representation of class.
    */
  std::string ToString() const
  {
    std::stringstream ss;
    ss << "TofTrackletParam: pos=(" << fX << "," << fY << "," << fZ << ") tx=" << fTx << " ty=" << fTy
       << " qp=" << fQp;  // << std::endl;
                          // ss << "cov: ";
    // for (int32_t i = 0; i < 15; i++) ss << fCovMatrix[i] << " ";
    // ss << endl;
    ss.precision(3);
    ss << " cov: x=" << fCovMatrix[0] << " y=" << fCovMatrix[5] << " tx=" << fCovMatrix[9] << " ty=" << fCovMatrix[12]
       << " q/p=" << fCovMatrix[14];
    return ss.str();
  }

  double GetZr(double R) const;

  double GetZy(double Y) const
  {
    if (fTy != 0.) { return (Y - fY) / fTy + fZ; }
    return 0.;
  }


private:
  double fX, fY, fZ, fT;  // X, Y, Z coordinates in [cm]
  double fTx, fTy, fTt;   // Slopes: tx=dx/dz, ty=dy/dz
  double fQp;             // Q/p: Q is a charge (+/-1), p is momentum in [GeV/c]
  double fLz;             // tracklength in z - direction
  double fChiSq;
  /* Covariance matrix.
    * Upper triangle symmetric matrix.
    * a[0,0..4], a[1,1..4], a[2,2..4], a[3,3..4], a[4,4] */
  std::vector<double> fCovMatrix;

  ClassDef(CbmTofTrackletParam, 1);
};

#endif /*CBMTOFTRACKLETPARAM_H_*/
