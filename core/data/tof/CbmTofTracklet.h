/** @file CbmTofTracklet.h
 ** @author nh <N.Herrmann@gsi.de>
 ** @date 25.07.2015
 **/

#ifndef CBMTOFTRACKLET_H
#define CBMTOFTRACKLET_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t, Option_t
#include <TMath.h>       // for Sqrt, ATan, Power
#include <TObject.h>     // for TObject

#include <FairTrackParam.h>  // for FairTrackParam

#include <vector>  // for vector, __vector_base<>::value_type

#include "CbmTofHit.h"            // for CbmTofHit
#include "CbmTofTrackletParam.h"  // for CbmTofTrackletParam

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
  Int_t GetPidHypo() const { return fPidHypo; }

  /**  Number of TOF hits **/
  Int_t GetNofHits() const { return fTofHit.size(); }

  /**  Index of TOF hit **/
  Int_t GetHitIndex(Int_t ind) const { return fTofHit[ind]; }

  Double_t GetT0() const { return fT0; }
  Double_t GetTt() const { return fTt; }
  Double_t GetT0Err() const { return fT0Err; }
  Double_t GetTtErr() const { return fTtErr; }
  Double_t GetT0TtCov() const { return fT0TtCov; }
  Double_t GetTheta() const {
    return TMath::ATan(TMath::Sqrt(TMath::Power(fTrackPar.GetTy(), 2)
                                   + TMath::Power(fTrackPar.GetTx(), 2)));
  }
  Double_t GetPhi() const {
    return TMath::ATan(fTrackPar.GetTy() / fTrackPar.GetTx());
  }
  Int_t GetStationHitIndex(Int_t iSm) const {
    for (Int_t i = 0; i < (Int_t) fTofHit.size(); i++)
      if (fTofDet[i] == iSm) return fTofHit[i];
    return -1;
  }

  Int_t GetTofHitIndex(Int_t ind) const { return fTofHit[ind]; }
  inline void SetTofHitIndex(Int_t ind, Int_t i) { fTofHit[ind] = i; }
  CbmTofHit* GetTofHitPointer(Int_t ind) { return &fhit[ind]; }
  Int_t GetTofDetIndex(Int_t ind) const { return fTofDet[ind]; }

  const std::vector<Int_t>& GetTofHitInd() const { return fTofHit; }

  const Double_t*
  GetPoint(Int_t n);  // interface to event display: CbmEvDisTracks
  const Double_t*
  GetFitPoint(Int_t n);  // interface to event display: CbmEvDisTracks

  /**  Error of track x coordinate at TOF  **/
  Double_t GetTrackDx() const {
    return TMath::Sqrt(fTrackPar.GetCovariance(0));
  }

  /**  Error of track x coordinate at TOF  **/
  Double_t GetTrackDy() const {
    return TMath::Sqrt(fTrackPar.GetCovariance(5));
  }

  /**  Track length from primary vertex to TOF **/
  Double_t GetTrackLength() const { return fTrackLength; }

  /**  Track parameters at TOF **/
  CbmTofTrackletParam* GetTrackParameter() { return &fTrackPar; }

  /**  Track x position at TOF  **/
  Double_t GetTrackX() const { return fTrackPar.GetX(); }

  /**  Track y position at TOF  **/
  Double_t GetTrackY() const { return fTrackPar.GetY(); }

  /**  Track x slope  **/
  Double_t GetTrackTx() const { return fTrackPar.GetTx(); }

  /**  Track y slope  **/
  Double_t GetTrackTy() const { return fTrackPar.GetTy(); }

  /** Normalized distance from hit to track **/
  Double_t GetDistance() const { return fDistance; }
  Double_t GetTime() const { return fTime; }
  Double_t UpdateTt();
  Double_t UpdateT0();
  Double_t GetTex(CbmTofHit* pHit);

  Int_t
  GetFirstInd(Int_t iSmType);  // return closest Hit to target except in iSmType
  Double_t GetZ0x();           // return intercept with z-axis
  Double_t GetZ0y();           // return intercept with z-axis
  Double_t GetR0();            // return transverse distance at z=0
  Double_t GetFitX(Double_t Z);  // get x value of fit function at position z
  Double_t GetFitY(Double_t Z);  // get y value of fit function at position z
  Double_t GetFitT(Double_t R);  // get time of fit function at distance R
  Double_t GetRefVel(UInt_t N);  // get reference velocity from first N hits

  Double_t GetChiSq() const { return fChiSq; }
  Int_t GetNDF() const { return fNDF; }

  const FairTrackParam* GetParamFirst() const { return &fParamFirst; }
  const FairTrackParam* GetParamLast() const { return &fParamLast; }

  virtual void GetFairTrackParamLast();

  virtual Double_t GetMatChi2(Int_t iSm);

  /** Set TOF hit index **/
  inline void SetTofHitIndex(Int_t tofHitIndex, Int_t iDet, CbmTofHit* pHit) {
    fTofHit.resize(1);
    fTofHit[0] = tofHitIndex;
    fTofDet.resize(1);
    fTofDet[0] = iDet;
    fhit.resize(1);
    fhit[0] = CbmTofHit(*pHit);
    fMatChi.resize(1);
  }

  inline void SetTofHitIndex(Int_t tofHitIndex,
                             Int_t iDet,
                             CbmTofHit* pHit,
                             Double_t chi2) {
    fTofHit.resize(1);
    fTofHit[0] = tofHitIndex;
    fTofDet.resize(1);
    fTofDet[0] = iDet;
    fhit.resize(1);
    fhit[0] = CbmTofHit(*pHit);
    fMatChi.resize(1);
    fMatChi[0] = chi2;
  }

  inline void SetTofHitInd(const std::vector<Int_t>& tofHitInd) {
    fTofHit = tofHitInd;
  }


  inline void AddTofHitIndex(Int_t tofHitIndex, Int_t iDet, CbmTofHit* pHit) {
    fTofHit.resize(fTofHit.size() + 1);
    fTofHit[fTofHit.size() - 1] = tofHitIndex;
    fTofDet.resize(fTofHit.size());
    fTofDet[fTofHit.size() - 1] = iDet;
    fhit.resize(fTofHit.size());
    fhit[fTofHit.size() - 1] = CbmTofHit(*pHit);
    fMatChi.resize(fTofHit.size());
  }

  inline void AddTofHitIndex(Int_t tofHitIndex,
                             Int_t iDet,
                             CbmTofHit* pHit,
                             Double_t chi2) {
    fTofHit.resize(fTofHit.size() + 1);
    fTofHit[fTofHit.size() - 1] = tofHitIndex;
    fTofDet.resize(fTofHit.size());
    fTofDet[fTofHit.size() - 1] = iDet;
    fhit.resize(fTofHit.size());
    fhit[fTofHit.size() - 1] = CbmTofHit(*pHit);
    fMatChi.resize(fTofHit.size());
    fMatChi[fTofHit.size() - 1] = chi2;
  }

  inline void ReplaceTofHitIndex(Int_t tofHitIndex,
                                 Int_t iDet,
                                 CbmTofHit* pHit,
                                 Double_t chi2) {
    for (Int_t iHit = 0; iHit < (Int_t) fTofHit.size(); iHit++) {
      if (iDet == fTofDet[iHit]) {
        fTofHit[iHit] = tofHitIndex;
        fhit[iHit]    = CbmTofHit(*pHit);
        fMatChi[iHit] = chi2;
        break;
      }
    }
  }

  inline void RemoveTofHitIndex(Int_t /*tofHitIndex*/,
                                Int_t iDet,
                                CbmTofHit* /*pHit*/,
                                Double_t /*chi2*/) {
    for (Int_t iHit = 0; iHit < (Int_t) fTofHit.size(); iHit++) {
      if (iDet == fTofDet[iHit]) {
        fTofHit.erase(fTofHit.begin() + iHit);
        fhit.erase(fhit.begin() + iHit);
        fMatChi.erase(fMatChi.begin() + iHit);
        break;
      }
    }
  }

  virtual Double_t GetXdif(Int_t iSmType, CbmTofHit* pHit);
  virtual Double_t GetYdif(Int_t iSmType, CbmTofHit* pHit);
  virtual Double_t GetTdif(Int_t iSmType, CbmTofHit* pHit);
  virtual Double_t Dist3D(CbmTofHit* pHit0, CbmTofHit* pHit1);
  virtual Bool_t ContainsAddr(Int_t iAddr);
  virtual Int_t HitIndexOfAddr(Int_t iAddr);
  virtual CbmTofHit* HitPointerOfAddr(Int_t iAddr);

  /** Set track parameter **/
  inline void SetTrackParameter(CbmTofTrackletParam* par) { fTrackPar = *par; }

  /** Set track length **/
  inline void SetTrackLength(Double_t trackLength) {
    fTrackLength = trackLength;
  }

  /** Set PID hypothesis for track extrapolation to TOF **/
  inline void SetPidHypo(Int_t pid) { fPidHypo = pid; }

  /** Set normalized distance from hit to track **/
  inline void SetDistance(Double_t distance) { fDistance = distance; }
  inline void SetTime(Double_t val) { fTime = val; }
  inline void SetTt(Double_t val) { fTt = val; }
  inline void SetT0(Double_t val) { fT0 = val; }
  inline void SetT0Err(Double_t val) { fT0Err = val; }
  inline void SetTtErr(Double_t val) { fTtErr = val; }
  inline void SetT0TtCov(Double_t val) { fT0TtCov = val; }

  inline void SetChiSq(Double_t chiSq) { fChiSq = chiSq; }
  inline void SetNDF(Int_t ndf) { fNDF = ndf; }
  inline void SetParamFirst(const FairTrackParam* par) { fParamFirst = *par; }
  inline void SetParamLast(const FairTrackParam* par) { fParamLast = *par; }

  void SetParamLast(const CbmTofTrackletParam* par);
  //	void LoadParamLast();

  void Clear(Option_t* option = "");
  CbmTofTracklet(const CbmTofTracklet&); /**   Copy Constructor   **/

private:
  Double_t fTrackLength;  // Track length from primary vertex to TOF [cm]
  Int_t fPidHypo;         // PID hypothesis used for track extrapolation
  Double_t fDistance;     // Normalized distance from hit to track
  Double_t fTime;         // Reference time of reference hit
  Double_t fTt;           // slope dT/dr
  Double_t fT0;           // Time at origin
  Double_t fT0Err;        // Error on Time at origin
  Double_t fTtErr;        // Error on slope dT/dr
  Double_t fT0TtCov;      // Covariance od fT0 and fTt
  Double_t fChiSq;        // Chi2 of fit
  Int_t fNDF;             // # of degrees of freedom
  CbmTofTrackletParam fTrackPar;  //  Track parameters at z of TofHit
  FairTrackParam fParamFirst;  //  Track parameters at first and last fitted hit
  FairTrackParam fParamLast;   //
  std::vector<Int_t> fTofHit;  // Index of TofHit
  std::vector<Int_t> fTofDet;  // DetLayer of TofHit
  std::vector<Double_t> fMatChi;  // Matching Chi2 of TofHit
  std::vector<CbmTofHit> fhit;    // vector of TofHit objects
  Double_t fP[4];                 // transient (transfer) space point to Eve

  CbmTofTracklet&
  operator=(const CbmTofTracklet&); /**   Assignment operator   **/

  ClassDef(CbmTofTracklet, 3);
};

#endif /* CBMTOFTRACKLET_H */
