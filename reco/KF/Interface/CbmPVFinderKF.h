/* Copyright (C) 2006-2016 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Denis Bertini [committer], Volker Friese */

/** CbmPVFinderKF
 *@author S.Gorbunov
 **
 **/


#ifndef CBMKFPVFINDERKF_H
#define CBMKFPVFINDERKF_H 1


#include "CbmPrimaryVertexFinder.h"


class CbmPVFinderKF : public CbmPrimaryVertexFinder {

public:
  /** Default constructor **/
  CbmPVFinderKF() {};


  /** Destructor **/
  ~CbmPVFinderKF() {};


  /** Execution of PV finding.
   *@param tracks   TClonesArray of CbmStsTracks
   *@param vertex   Primary vertex (output)
   *@param event    Pointer to event object
   **/
  virtual Int_t FindPrimaryVertex(TClonesArray* tracks, CbmVertex* vertex);


  /** Execution of PV finding.
   ** @param event    Pointer to event object
   ** @param tracks   TClonesArray of CbmStsTracks
   **/
  virtual Int_t FindEventVertex(CbmEvent* event, TClonesArray* tracks);


  ClassDef(CbmPVFinderKF, 1);
};

#endif
