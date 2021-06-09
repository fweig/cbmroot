/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                    CbmTrackParam header file                 -----
// -----                  Created 05/02/16  by T. Ablyazimov          -----
// -------------------------------------------------------------------------

/**  CbmTrackParam.h
 *@author T.Ablyazimov <t.ablyazimov@gsi.de>
 **
 ** Data class for Global CBM track parameters. Data level RECO.
 ** It is derived from the FairTrackParam class and extends its data interfaces.
 **
 **/

#ifndef CBMLTRACKPARAM_H_
#define CBMLTRACKPARAM_H_ 1

#include <FairTrackParam.h>  // for FairTrackParam

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t

class CbmTrackParam : public FairTrackParam {
public:
  CbmTrackParam() : fPx(0), fPy(0), fPz(0), fDpx(0), fDpy(0), fDpz(0), fTime(0.), fDTime(0.) {}
  void Set(const FairTrackParam& ftp, Double_t time = 0., Double_t timeError = 0.);
  void SetTime(Double_t time, Double_t timeError = 0.)
  {
    fTime  = time;
    fDTime = timeError;
  }
  Double_t GetPx() const { return fPx; }
  Double_t GetPy() const { return fPy; }
  Double_t GetPz() const { return fPz; }
  Double_t GetDpx() const { return fDpx; }
  Double_t GetDpy() const { return fDpy; }
  Double_t GetDpz() const { return fDpz; }
  Double_t GetTime() const { return fTime; }
  Double_t GetDTime() const { return fDTime; }

private:
  Double_t fPx;
  Double_t fPy;
  Double_t fPz;
  Double_t fDpx;
  Double_t fDpy;
  Double_t fDpz;
  Double_t fTime;
  Double_t fDTime;
  ClassDef(CbmTrackParam, 2);
};


#endif
