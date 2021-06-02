/** @file CbmTofTrackletParam.h
  * @copyright Copyright (C) 2015-2021 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer], Florian Uhlig **/

/**
 * \file CbmTofTrackletParam.h
 * \author Norbert Herrmann <n.herrmann@gsi.de>
 * \date 2020
 * \brief Data class for track parameters.
 **/

#ifndef CBMTOFTRACKLETPARAM_H_
#define CBMTOFTRACKLETPARAM_H_

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t
#include <TObject.h>     // for TObject
#include <TVector3.h>    // for TVector3

#include <sstream>  // for operator<<, basic_ostream, stringstream, cha...
#include <vector>   // for vector

#include <math.h>  // for abs, sqrt

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
  Double_t GetX() const { return fX; }
  Double_t GetY() const { return fY; }
  Double_t GetZ() const { return fZ; }
  Double_t GetT() const { return fT; }
  Double_t GetLz() const { return fLz; }
  Double_t GetTx() const { return fTx; }
  Double_t GetTy() const { return fTy; }
  Double_t GetTt() const { return fTt; }
  Double_t GetQp() const { return fQp; }
  Double_t GetChiSq() const { return fChiSq; }
  Double_t GetCovariance(int index) const { return fCovMatrix[index]; }
  const std::vector<Double_t>& GetCovMatrix() const { return fCovMatrix; }

  /* Setters */
  void SetX(Double_t x) { fX = x; }
  void SetY(Double_t y) { fY = y; }
  void SetZ(Double_t z) { fZ = z; }
  void SetT(Double_t t) { fT = t; }
  void SetLz(Double_t lz) { fLz = lz; }
  void SetTx(Double_t tx) { fTx = tx; }
  void SetTy(Double_t ty) { fTy = ty; }
  void SetTt(Double_t tt) { fTt = tt; }
  void SetQp(Double_t qp) { fQp = qp; }
  void SetChiSq(Double_t v) { fChiSq = v; }
  void SetCovMatrix(const std::vector<Double_t>& C) { fCovMatrix.assign(C.begin(), C.end()); }
  void SetCovariance(int index, Double_t cov) { fCovMatrix[index] = cov; }

  /**
    * \brief Return direction cosines.
    * \param[out] nx Output direction cosine for OX axis.
    * \param[out] ny Output direction cosine for OY axis.
    * \param[out] nz Output direction cosine for OZ axis.
    */
  void GetDirCos(Double_t& nx, Double_t& ny, Double_t& nz) const
  {
    Double_t p    = (std::abs(fQp) != 0.) ? 1. / std::abs(fQp) : 1.e20;
    Double_t pz   = std::sqrt(p * p / (fTx * fTx + fTy * fTy + 1));
    Double_t px   = fTx * pz;
    Double_t py   = fTy * pz;
    TVector3 unit = TVector3(px, py, pz).Unit();
    nx            = unit.X();
    ny            = unit.Y();
    nz            = unit.Z();
  }

  /**
    * \brief Return state vector as vector.
    * \return State vector as vector.
    */
  std::vector<Double_t> GetStateVector() const
  {
    std::vector<Double_t> state(5, 0.);
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
  void SetStateVector(const std::vector<Double_t>& x)
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
    // for (Int_t i = 0; i < 15; i++) ss << fCovMatrix[i] << " ";
    // ss << endl;
    ss.precision(3);
    ss << " cov: x=" << fCovMatrix[0] << " y=" << fCovMatrix[5] << " tx=" << fCovMatrix[9] << " ty=" << fCovMatrix[12]
       << " q/p=" << fCovMatrix[14];
    return ss.str();
  }

  Double_t GetZr(Double_t R) const;

  Double_t GetZy(Double_t Y) const
  {
    if (fTy != 0.) { return (Y - fY) / fTy + fZ; }
    return 0.;
  }


private:
  Double_t fX, fY, fZ, fT;  // X, Y, Z coordinates in [cm]
  Double_t fTx, fTy, fTt;   // Slopes: tx=dx/dz, ty=dy/dz
  Double_t fQp;             // Q/p: Q is a charge (+/-1), p is momentum in [GeV/c]
  Double_t fLz;             // tracklength in z - direction
  Double_t fChiSq;
  /* Covariance matrix.
    * Upper triangle symmetric matrix.
    * a[0,0..4], a[1,1..4], a[2,2..4], a[3,3..4], a[4,4] */
  std::vector<Double_t> fCovMatrix;

  ClassDef(CbmTofTrackletParam, 1);
};

#endif /*CBMTOFTRACKLETPARAM_H_*/
