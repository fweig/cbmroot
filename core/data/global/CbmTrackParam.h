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

class CbmTrackParam : public FairTrackParam {
public:
  CbmTrackParam() : fPx(0), fPy(0), fPz(0), fDpx(0), fDpy(0), fDpz(0), fTime(0.), fDTime(0.) {}
  void Set(const FairTrackParam& ftp, double time = 0., double timeError = 0.);
  void SetTime(double time, double timeError = 0.)
  {
    fTime  = time;
    fDTime = timeError;
  }
  double GetPx() const { return fPx; }
  double GetPy() const { return fPy; }
  double GetPz() const { return fPz; }
  double GetDpx() const { return fDpx; }
  double GetDpy() const { return fDpy; }
  double GetDpz() const { return fDpz; }
  double GetTime() const { return fTime; }
  double GetDTime() const { return fDTime; }

private:
  double fPx;
  double fPy;
  double fPz;
  double fDpx;
  double fDpy;
  double fDpz;
  double fTime;
  double fDTime;
  ClassDef(CbmTrackParam, 2);
};


#endif
