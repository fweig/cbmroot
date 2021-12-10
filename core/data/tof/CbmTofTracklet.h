/* Copyright (C) 2015-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer], Florian Uhlig */

/** @file CbmTofTracklet.h
 ** @author nh <N.Herrmann@gsi.de>
 ** @date 25.07.2015
 **/

#ifndef CBMTOFTRACKLET_H
#define CBMTOFTRACKLET_H 1

#include "CbmTofHit.h"            // for CbmTofHit
#include "CbmTofTrackletParam.h"  // for CbmTofTrackletParam

#include <FairTrackParam.h>  // for FairTrackParam

#include <Rtypes.h>   // for THashConsistencyHolder, ClassDef
#include <TObject.h>  // for TObject

#include <cstdint>
#include <vector>  // for vector, __vector_base<>::value_type

#include <cmath>

/** @class CbmTofTracklet
 ** @brief Provides information on attaching a TofHit to a TofTrack
 ** @author nh
 **/
class CbmTofTracklet : public TObject {

public:
  /**   Constructor   **/
  CbmTofTracklet();

  /** Constructor with parameters
	 ** @param trackIndex   Array index of global track
	 ** @param hitIndex     Array index of TOF hit
	 ** @param trackLength  Track length from primary vertex to TOF
	 ** @param trackPar     Parameters of track at TOF
	 ** @param pidHypo      PID hypothesis for track extrapolation
	 **/

  /**   Destructor   **/
  virtual ~CbmTofTracklet();

  virtual void PrintInfo();

  /**  PID hypothesis for track extrapolation to TOF **/
  int32_t GetPidHypo() const { return fPidHypo; }

  /**  Number of TOF hits **/
  int32_t GetNofHits() const { return fTofHit.size(); }

  /**  Index of TOF hit **/
  int32_t GetHitIndex(int32_t ind) const { return fTofHit[ind]; }

  double GetT0() const { return fT0; }
  double GetTt() const { return fTt; }
  double GetT0Err() const { return fT0Err; }
  double GetTtErr() const { return fTtErr; }
  double GetT0TtCov() const { return fT0TtCov; }
  double GetTheta() const { return atan(sqrt(pow(fTrackPar.GetTy(), 2) + pow(fTrackPar.GetTx(), 2))); }
  double GetPhi() const { return atan(fTrackPar.GetTy() / fTrackPar.GetTx()); }
  int32_t GetStationHitIndex(int32_t iSm) const
  {
    for (int32_t i = 0; i < (int32_t) fTofHit.size(); i++)
      if (fTofDet[i] == iSm) return fTofHit[i];
    return -1;
  }

  int32_t GetTofHitIndex(int32_t ind) const { return fTofHit[ind]; }
  inline void SetTofHitIndex(int32_t ind, int32_t i) { fTofHit[ind] = i; }
  CbmTofHit* GetTofHitPointer(int32_t ind) { return &fhit[ind]; }
  int32_t GetTofDetIndex(int32_t ind) const { return fTofDet[ind]; }

  const std::vector<int32_t>& GetTofHitInd() const { return fTofHit; }

  const double* GetPoint(int32_t n);     // interface to event display: CbmEvDisTracks
  const double* GetFitPoint(int32_t n);  // interface to event display: CbmEvDisTracks

  /**  Error of track x coordinate at TOF  **/
  double GetTrackDx() const { return sqrt(fTrackPar.GetCovariance(0)); }

  /**  Error of track x coordinate at TOF  **/
  double GetTrackDy() const { return sqrt(fTrackPar.GetCovariance(5)); }

  /**  Track length from primary vertex to TOF **/
  double GetTrackLength() const { return fTrackLength; }

  /**  Track parameters at TOF **/
  CbmTofTrackletParam* GetTrackParameter() { return &fTrackPar; }

  /**  Track x position at TOF  **/
  double GetTrackX() const { return fTrackPar.GetX(); }

  /**  Track y position at TOF  **/
  double GetTrackY() const { return fTrackPar.GetY(); }

  /**  Track x slope  **/
  double GetTrackTx() const { return fTrackPar.GetTx(); }

  /**  Track y slope  **/
  double GetTrackTy() const { return fTrackPar.GetTy(); }

  /** Normalized distance from hit to track **/
  double GetDistance() const { return fDistance; }
  double GetTime() const { return fTime; }
  double UpdateTt();
  double UpdateT0();
  double GetTex(CbmTofHit* pHit);

  int32_t GetFirstInd(int32_t iSmType);  // return closest Hit to target except in iSmType
  double GetZ0x();                       // return intercept with z-axis
  double GetZ0y();                       // return intercept with z-axis
  double GetR0();                        // return transverse distance at z=0
  double GetFitX(double Z);              // get x value of fit function at position z
  double GetFitY(double Z);              // get y value of fit function at position z
  double GetFitT(double R);              // get time of fit function at distance R
  double GetRefVel(uint32_t N);          // get reference velocity from first N hits

  double GetChiSq() const { return fChiSq; }
  int32_t GetNDF() const { return fNDF; }

  const FairTrackParam* GetParamFirst() const { return &fParamFirst; }
  const FairTrackParam* GetParamLast() const { return &fParamLast; }

  virtual void GetFairTrackParamLast();

  virtual double GetMatChi2(int32_t iSm);

  /** Set TOF hit index **/
  inline void SetTofHitIndex(int32_t tofHitIndex, int32_t iDet, CbmTofHit* pHit)
  {
    fTofHit.resize(1);
    fTofHit[0] = tofHitIndex;
    fTofDet.resize(1);
    fTofDet[0] = iDet;
    fhit.resize(1);
    fhit[0] = CbmTofHit(*pHit);
    fMatChi.resize(1);
  }

  inline void SetTofHitIndex(int32_t tofHitIndex, int32_t iDet, CbmTofHit* pHit, double chi2)
  {
    fTofHit.resize(1);
    fTofHit[0] = tofHitIndex;
    fTofDet.resize(1);
    fTofDet[0] = iDet;
    fhit.resize(1);
    fhit[0] = CbmTofHit(*pHit);
    fMatChi.resize(1);
    fMatChi[0] = chi2;
  }

  inline void SetTofHitInd(const std::vector<int32_t>& tofHitInd) { fTofHit = tofHitInd; }


  inline void AddTofHitIndex(int32_t tofHitIndex, int32_t iDet, CbmTofHit* pHit)
  {
    fTofHit.resize(fTofHit.size() + 1);
    fTofHit[fTofHit.size() - 1] = tofHitIndex;
    fTofDet.resize(fTofHit.size());
    fTofDet[fTofHit.size() - 1] = iDet;
    fhit.resize(fTofHit.size());
    fhit[fTofHit.size() - 1] = CbmTofHit(*pHit);
    fMatChi.resize(fTofHit.size());
  }

  inline void AddTofHitIndex(int32_t tofHitIndex, int32_t iDet, CbmTofHit* pHit, double chi2)
  {
    fTofHit.resize(fTofHit.size() + 1);
    fTofHit[fTofHit.size() - 1] = tofHitIndex;
    fTofDet.resize(fTofHit.size());
    fTofDet[fTofHit.size() - 1] = iDet;
    fhit.resize(fTofHit.size());
    fhit[fTofHit.size() - 1] = CbmTofHit(*pHit);
    fMatChi.resize(fTofHit.size());
    fMatChi[fTofHit.size() - 1] = chi2;
  }

  inline void ReplaceTofHitIndex(int32_t tofHitIndex, int32_t iDet, CbmTofHit* pHit, double chi2)
  {
    for (int32_t iHit = 0; iHit < (int32_t) fTofHit.size(); iHit++) {
      if (iDet == fTofDet[iHit]) {
        fTofHit[iHit] = tofHitIndex;
        fhit[iHit]    = CbmTofHit(*pHit);
        fMatChi[iHit] = chi2;
        break;
      }
    }
  }

  inline void RemoveTofHitIndex(int32_t /*tofHitIndex*/, int32_t iDet, CbmTofHit* /*pHit*/, double /*chi2*/)
  {
    for (int32_t iHit = 0; iHit < (int32_t) fTofHit.size(); iHit++) {
      if (iDet == fTofDet[iHit]) {
        fTofHit.erase(fTofHit.begin() + iHit);
        fhit.erase(fhit.begin() + iHit);
        fMatChi.erase(fMatChi.begin() + iHit);
        break;
      }
    }
  }

  virtual double GetXdif(int32_t iSmType, CbmTofHit* pHit);
  virtual double GetYdif(int32_t iSmType, CbmTofHit* pHit);
  virtual double GetTdif(int32_t iSmType, CbmTofHit* pHit);
  virtual double Dist3D(CbmTofHit* pHit0, CbmTofHit* pHit1);
  virtual bool ContainsAddr(int32_t iAddr);
  virtual int32_t HitIndexOfAddr(int32_t iAddr);
  virtual CbmTofHit* HitPointerOfAddr(int32_t iAddr);

  /** Set track parameter **/
  inline void SetTrackParameter(CbmTofTrackletParam* par) { fTrackPar = *par; }

  /** Set track length **/
  inline void SetTrackLength(double trackLength) { fTrackLength = trackLength; }

  /** Set PID hypothesis for track extrapolation to TOF **/
  inline void SetPidHypo(int32_t pid) { fPidHypo = pid; }

  /** Set normalized distance from hit to track **/
  inline void SetDistance(double distance) { fDistance = distance; }
  inline void SetTime(double val) { fTime = val; }
  inline void SetTt(double val) { fTt = val; }
  inline void SetT0(double val) { fT0 = val; }
  inline void SetT0Err(double val) { fT0Err = val; }
  inline void SetTtErr(double val) { fTtErr = val; }
  inline void SetT0TtCov(double val) { fT0TtCov = val; }

  inline void SetChiSq(double chiSq) { fChiSq = chiSq; }
  inline void SetNDF(int32_t ndf) { fNDF = ndf; }
  inline void SetParamFirst(const FairTrackParam* par) { fParamFirst = *par; }
  inline void SetParamLast(const FairTrackParam* par) { fParamLast = *par; }

  void SetParamLast(const CbmTofTrackletParam* par);
  //	void LoadParamLast();

  void Clear(Option_t* option = "");
  CbmTofTracklet(const CbmTofTracklet&); /**   Copy Constructor   **/

private:
  double fTrackLength;            // Track length from primary vertex to TOF [cm]
  int32_t fPidHypo;               // PID hypothesis used for track extrapolation
  double fDistance;               // Normalized distance from hit to track
  double fTime;                   // Reference time of reference hit
  double fTt;                     // slope dT/dr
  double fT0;                     // Time at origin
  double fT0Err;                  // Error on Time at origin
  double fTtErr;                  // Error on slope dT/dr
  double fT0TtCov;                // Covariance od fT0 and fTt
  double fChiSq;                  // Chi2 of fit
  int32_t fNDF;                   // # of degrees of freedom
  CbmTofTrackletParam fTrackPar;  //  Track parameters at z of TofHit
  FairTrackParam fParamFirst;     //  Track parameters at first and last fitted hit
  FairTrackParam fParamLast;      //
  std::vector<int32_t> fTofHit;   // Index of TofHit
  std::vector<int32_t> fTofDet;   // DetLayer of TofHit
  std::vector<double> fMatChi;    // Matching Chi2 of TofHit
  std::vector<CbmTofHit> fhit;    // vector of TofHit objects
  double fP[4];                   // transient (transfer) space point to Eve

  CbmTofTracklet& operator=(const CbmTofTracklet&); /**   Assignment operator   **/

  ClassDef(CbmTofTracklet, 3);
};

#endif /* CBMTOFTRACKLET_H */
