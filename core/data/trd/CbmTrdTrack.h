/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Matus Kalisky, Florian Uhlig, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                      CbmTrdTrack header file                  -----
// -----                  Created 11/07/05  by M. Kalisky              -----
// -----                  Modified 04/06/09 by A. Lebedev               -----
// -------------------------------------------------------------------------

/**  CbmTrdTrack.h
 *@author M.Kalisky <m.kalisky@gsi.de>
 ** 
 ** TRD local track. Holds lists of CbmTrdHits and the fitted
 ** track parameters. The fit parameters are of type FairTrackParam
 ** and can only be accessed and modified via this class.
 **/
#ifndef CBMTRDTRACK_H_
#define CBMTRDTRACK_H_ 1

#include "CbmTrack.h"  // for CbmTrack

#include <Rtypes.h>      // for ClassDef

class CbmTrdTrack : public CbmTrack {
public:
  /** Default constructor **/
  CbmTrdTrack();

  /** Destructor **/
  virtual ~CbmTrdTrack();

  /** Accessors  **/
  double GetPidWkn() const { return fPidWkn; }
  double GetPidANN() const { return fPidANN; }
  double GetELoss() const { return fELoss; }
  double GetPidLikeEL() const { return fPidLikeEL; }
  double GetPidLikePI() const { return fPidLikePI; }
  double GetPidLikeKA() const { return fPidLikeKA; }
  double GetPidLikePR() const { return fPidLikePR; }
  double GetPidLikeMU() const { return fPidLikeMU; }

  /** Modifiers  **/
  void SetPidWkn(double pid) { fPidWkn = pid; }
  void SetPidANN(double pid) { fPidANN = pid; }
  void SetELoss(double eLoss) { fELoss = eLoss; }
  void SetPidLikeEL(double value) { fPidLikeEL = value; }
  void SetPidLikePI(double value) { fPidLikePI = value; }
  void SetPidLikeKA(double value) { fPidLikeKA = value; }
  void SetPidLikePR(double value) { fPidLikePR = value; }
  void SetPidLikeMU(double value) { fPidLikeMU = value; }

private:
  /** PID value based on Wkn method **/
  double fPidWkn;

  /** PID value based on ANN method **/
  double fPidANN;

  /** PID values based on Likelihood method **/
  double fPidLikeEL;
  double fPidLikePI;
  double fPidLikeKA;
  double fPidLikePR;
  double fPidLikeMU;

  /** Total energy loss in TRD **/
  double fELoss;

  ClassDef(CbmTrdTrack, 4);
};

#endif
