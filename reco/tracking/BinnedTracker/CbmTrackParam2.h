/** @file CbmTrackParam2.h
  * @copyright Copyright (C) 2018 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [orginator] **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef CBM_TRACK_PARAM2
#define CBM_TRACK_PARAM2

#include "FairTrackParam.h"
//#include "TMatrixD.h"
#include <type_traits>

#include <algorithm>
#include <iostream>
#include <iterator>

/*static inline std::ostream& operator<< (std::ostream& os, const TMatrixD& matrix)
{
    for (int i = 0; i < matrix.GetNrows(); ++i)
    {
        for (int j = 0; j < matrix.GetNcols(); ++j)
            os << matrix(i, j) << " ";
        
        os << std::endl;
    }
    
    return os;
}*/

/*class CbmTrackParam2
{
public:
   CbmTrackParam2() : fZ(0), fVector(5, 1), fCovMatrix(5, 5) {}
   
   Double_t GetX() const { return fVector(0, 0); }
   void SetX(Double_t v) { fVector(0, 0) = v; }
   Double_t GetY() const { return fVector(1, 0); }
   void SetY(Double_t v) { fVector(1, 0) = v; }
   Double_t GetZ() const { return fZ; }
   void SetZ(Double_t v) { fZ = v; }
   Double_t GetTx() const { return fVector(2, 0); }
   void SetTx(Double_t v) { fVector(2, 0) = v; }
   Double_t GetTy() const { return fVector(3, 0); }
   void SetTy(Double_t v) { fVector(3, 0) = v; }
   Double_t GetTime() const { return fVector(4, 0); }
   void SetTime(Double_t time) { fVector(4, 0) = time; }
   
   TMatrixD& GetVector() { return fVector; }
   const TMatrixD& GetVector() const { return fVector; }
   
   
   //void CovMatrix(Double_t cov[]) const { std::copy(std::begin(fCovMatrix), std::end(fCovMatrix), cov); }
   //void SetCovMatrix(Double_t cov[]) { std::copy(cov, cov + 21, std::begin(fCovMatrix)); }
   //Double_t GetCovariance(Int_t i, Int_t j) const { return fCovMatrix[i * (13 - i) / 2 + j - i]; }
   //void SetCovariance(Int_t i, Int_t j, Double_t val) { fCovMatrix[i * (13 - i) / 2 + j - i] = val; }
   Double_t GetCov(int i, int j) const { return fCovMatrix(i, j); }
   void SetCov(int i, int j, Double_t v) { fCovMatrix(i, j) = v; fCovMatrix(j, i) = v; }
   
   TMatrixD& GetCovMatrix() { return fCovMatrix; }
   const TMatrixD& GetCovMatrix() const { return fCovMatrix; }
  
private:
    Double_t fZ;
    TMatrixD fVector;
    TMatrixD fCovMatrix;
};*/

class CbmTrackParam2 : public FairTrackParam {
public:
  Double_t GetTime() const { return GetQp(); }
  void SetTime(Double_t time) { SetQp(time); }

  Double_t GetCovXX() const { return GetCovariance(0, 0); }
  void SetCovXX(Double_t v) { SetCovariance(0, 0, v); }
  Double_t GetCovYY() const { return GetCovariance(1, 1); }
  void SetCovYY(Double_t v) { SetCovariance(1, 1, v); }
  Double_t GetCovXY() const { return GetCovariance(0, 1); }
  void SetCovXY(Double_t v) { SetCovariance(0, 1, v); }
  Double_t GetCovTxTx() const { return GetCovariance(2, 2); }
  void SetCovTxTx(Double_t v) { SetCovariance(2, 2, v); }
  Double_t GetCovTyTy() const { return GetCovariance(3, 3); }
  void SetCovTyTy(Double_t v) { SetCovariance(3, 3, v); }
  Double_t GetCovTT() const { return GetCovariance(4, 4); }
  void SetCovTT(Double_t v) { SetCovariance(4, 4, v); }

private:
  Double_t GetCovariance(Int_t i, Int_t j) const { return FairTrackParam::GetCovariance(i, j); }
  void SetCovariance(Int_t i, Int_t j, Double_t val) { FairTrackParam::SetCovariance(i, j, val); }

private:
  ClassDef(CbmTrackParam2, 1);
};

#endif  //CBM_TRACK_PARAM2
