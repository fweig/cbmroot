/* Copyright (C) 2006-2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: v.friese@gsi.de, Denis Bertini [committer] */

// -------------------------------------------------------------------------
// -----                  CbmTrackMergerIdeal header file              -----
// -----                  Created 01/12/05  by V. Friese               -----
// -------------------------------------------------------------------------


/** CbmTrackMergerIdeal
 *@author v.friese@gsi.de
 **
 ** Ideal merging of STD and TRD local tracks. Uses the MCTrack index
 ** obtained from the StsTrackMatch and TrdTrackMatch classes.
 **/


#ifndef CBMTRACKMERGERIDEAL_H
#define CBMTRACKMERGERIDEAL_H 1


#include "CbmTrackMerger.h"


class TClonesArray;


class CbmTrackMergerIdeal : public CbmTrackMerger {

public:
  /** Default constructor **/
  CbmTrackMergerIdeal();


  /** Destructor **/
  virtual ~CbmTrackMergerIdeal();


  /** Intialisation **/
  virtual void Init();


  /** Do the track merging **/
  virtual Int_t DoMerge(TClonesArray* stsTracks, TClonesArray* trdTracks, TClonesArray* glbTracks);


private:
  /** Arrays of StsTrackMatch and TrdTrackMatch **/
  TClonesArray* fStsMatch;
  TClonesArray* fTrdMatch;

  CbmTrackMergerIdeal(const CbmTrackMergerIdeal&);
  CbmTrackMergerIdeal& operator=(const CbmTrackMergerIdeal&);

  ClassDef(CbmTrackMergerIdeal, 1);
};

#endif
