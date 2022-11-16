/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                    CbmGlobalTrack header file                 -----
// -----                  Created 01/12/05  by V. Friese               -----
// -----                  Modified 04/06/09  by A. Lebedev             -----
// -------------------------------------------------------------------------

/**  CbmGlobalTrack.h
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Data class for Global CBM track. Data level RECO.
 ** It consists of local tracks in STS, MUCH and TRD and a RICH ring.
 **
 **/

#ifndef CBMGLOBALTRACK_H_
#define CBMGLOBALTRACK_H_ 1

#include "CbmTrackParam.h"  // for CbmTrackParam

#include <FairTrackParam.h>  // for FairTrackParam

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double32_t
#include <TObject.h>     // for TObject

#include <cstdint>

class CbmGlobalTrack : public TObject {

public:
  /** Default constructor **/
  CbmGlobalTrack();


  /** Destructor **/
  virtual ~CbmGlobalTrack();


  /** Accessors **/
  int32_t GetStsTrackIndex() const { return fStsTrack; }
  int32_t GetTrdTrackIndex() const { return fTrdTrack; }
  int32_t GetMuchTrackIndex() const { return fMuchTrack; }
  int32_t GetRichRingIndex() const { return fRichRing; }
  int32_t GetTofHitIndex() const { return fTofHit; }
  int32_t GetTofTrackIndex() const { return fTofTrack; }
  const FairTrackParam* GetParamFirst() const { return &fParamFirst; }
  const FairTrackParam* GetParamLast() const { return &fParamLast; }
  const CbmTrackParam* GetParamVertex() const { return &fParamPrimaryVertex; }
  int32_t GetPidHypo() const { return fPidHypo; }
  double GetChi2() const { return fChi2; }
  int32_t GetNDF() const { return fNDF; }
  int32_t GetFlag() const { return fFlag; }
  double GetLength() const { return fLength; }


  /** Modifiers **/
  void SetStsTrackIndex(int32_t iSts) { fStsTrack = iSts; }
  void SetTrdTrackIndex(int32_t iTrd) { fTrdTrack = iTrd; }
  void SetMuchTrackIndex(int32_t iMuch) { fMuchTrack = iMuch; }
  void SetRichRingIndex(int32_t iRing) { fRichRing = iRing; }
  void SetTofHitIndex(int32_t iTofHit) { fTofHit = iTofHit; }
  void SetTofTrackIndex(int32_t iTofTrack) { fTofTrack = iTofTrack; }
  void SetParamFirst(const FairTrackParam* parFirst) { fParamFirst = *parFirst; }
  void SetParamLast(const FairTrackParam* parLast) { fParamLast = *parLast; }
  void SetParamPrimaryVertex(const FairTrackParam* parPV) { fParamPrimaryVertex.Set(*parPV); }
  void SetPidHypo(int32_t iPid) { fPidHypo = iPid; }
  void SetChi2(double chi2) { fChi2 = chi2; }
  void SetNDF(int32_t ndf) { fNDF = ndf; }
  void SetFlag(int32_t iFlag) { fFlag = iFlag; }
  void SetLength(double length) { fLength = length; }


  /** Output to screen **/
  virtual void Print(Option_t* opt = "") const;


private:
  /** Indices of local StsTrack, TrdTrack, MuchTrack, RichRing and TofHit **/
  int32_t fStsTrack  = -1;
  int32_t fTrdTrack  = -1;
  int32_t fMuchTrack = -1;
  int32_t fRichRing  = -1;
  int32_t fTofHit    = -1;
  int32_t fTofTrack  = -1;

  /** Global track parameters at first and last plane **/
  FairTrackParam fParamFirst;
  FairTrackParam fParamLast;
  CbmTrackParam fParamPrimaryVertex;

  /** PID hypothesis used for global track fit **/
  int32_t fPidHypo = 0;

  /** Chi2 of global track fit **/
  double fChi2 = 0.;

  /** NDF of global track fit **/
  int32_t fNDF = 0;

  /** Quality flag **/
  int32_t fFlag = 0;

  /** Track length **/
  double fLength = 0.;

  ClassDef(CbmGlobalTrack, 3);
};


#endif
