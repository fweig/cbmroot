/* Copyright (C) 2006-2013 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Denis Bertini [committer] */

/** 
 *
 * @author S.Gorbunov
 * @version 0.0
 * @since 02.12.05
 * 
 */

#ifndef CBMKFTRACK_H
#define CBMKFTRACK_H

#include "CbmKFHit.h"
#include "CbmKFTrackInterface.h"

#include <vector>

class CbmKFHit;
class FairTrackParam;
class CbmStsTrack;

class CbmKFTrack : public CbmKFTrackInterface {

private:
  Double_t fT[6], fC[15], fMass, fChi2;
  Bool_t fIsElectron;
  Int_t fNDF;

public:
  std::vector<CbmKFHit*> fHits;

  CbmKFTrack();
  ~CbmKFTrack() {}

  CbmKFTrack(CbmKFTrackInterface& track) : fMass(0), fChi2(0), fIsElectron(0), fNDF(0), fHits() { SetTrack(track); }

  CbmKFTrack(FairTrackParam& track) : fMass(0), fChi2(0), fIsElectron(0), fNDF(0), fHits() { SetTrackParam(track); }

  CbmKFTrack(CbmStsTrack& track, bool first = 1) : fMass(0), fChi2(0), fIsElectron(0), fNDF(0), fHits()
  {
    SetStsTrack(track, first);
  }

  void SetTrack(CbmKFTrackInterface& track);
  void SetTrackParam(const FairTrackParam& track);
  void SetStsTrack(CbmStsTrack& track, bool first = 1);

  void GetTrackParam(FairTrackParam& track);
  void GetStsTrack(CbmStsTrack& track, bool first = 1);

  void SetPID(Int_t pidHypo);

  Double_t* GetTrack() override { return fT; }
  Double_t* GetCovMatrix() override { return fC; }
  Double_t& GetRefChi2() override { return fChi2; }
  Int_t& GetRefNDF() override { return fNDF; }
  Double_t GetMass() const override { return fMass; }
  Bool_t IsElectron() const override { return fIsElectron; }
  Int_t GetNOfHits() override { return fHits.size(); }
  CbmKFHit* GetHit(Int_t i) override { return fHits[i]; }

  ClassDefOverride(CbmKFTrack, 1)
};

#endif
