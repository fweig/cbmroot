/* Copyright (C) 2016-2021 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

/********************************************************************************
 *    Copyright (C) 2016 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                      CbmRecoTracks header file                -----
// -----                Created 12/02/16  by T. Ablyazimov             -----
// -------------------------------------------------------------------------


/** CbmRecoTracks
 * @author T. Ablyazimov
 * @since 12.02.16
 *   CBM event display object
 **
 **/

#ifndef CBMRECOTRACKS_H
#define CBMRECOTRACKS_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Option_t
//#include <TEveTrackPropagator.h> // IWYU pragma: keep needed by cint
#include <FairTask.h>  // for FairTask, InitStatus

#include <TClonesArray.h>  // IWYU pragma: keep needed by cliong
#include <TEveVector.h>    // for TEveVector
#include <TString.h>       // for TString

class CbmPixelHit;
class CbmStsTrack;
class CbmTrack;
class FairEventManager;
class TEveTrack;
class TEveTrackList;
class TObjArray;
class TParticle;
class TEveTrackPropagator;

class CbmRecoTracks : public FairTask {

public:
  /** Default constructor **/
  CbmRecoTracks();


  /** Standard constructor
    *@param name        Name of task
    *@param iVerbose    Verbosity level
    **/
  CbmRecoTracks(const char* name, Int_t iVerbose = 1);

  /** Destructor **/
  virtual ~CbmRecoTracks();

  /** Set verbosity level. For this task and all of the subtasks. **/
  void SetVerbose(Int_t iVerbose) { fVerbose = iVerbose; }
  /** Executed task **/
  virtual void Exec(Option_t* option);
  virtual InitStatus Init();
  virtual void SetParContainers();

  /** Action after each event**/
  virtual void Finish();
  void Reset();
  TEveTrackList* GetTrGroup(TParticle* P);

protected:
  void HandlePixelHit(TEveTrack* eveTrack, Int_t& n, const CbmPixelHit* hit, TEveVector* pMom);
  void HandleTrack(TEveTrack* eveTrack, Int_t& n, const CbmTrack* recoTrack);
  void HandleStsTrack(TEveTrack* eveTrack, Int_t& n, const CbmStsTrack* stsTrack);

  TClonesArray* fGlobalTracks;
  TClonesArray* fMvdHits;
  TClonesArray* fStsHits;
  TClonesArray* fStsTracks;
  TClonesArray* fRichRings;
  TClonesArray* fRichHits;
  TClonesArray* fMuchPixelHits;
  TClonesArray* fMuchTracks;
  TClonesArray* fTrdHits;
  TClonesArray* fTrdTracks;
  TClonesArray* fTofHits;
  TEveTrackPropagator* fTrPr;
  FairEventManager* fEventManager;  //!
  TObjArray* fEveTrList;
  TString fEvent;          //!
  TEveTrackList* fTrList;  //!
  //TEveElementList *fTrackCont;

  Double_t MinEnergyLimit;
  Double_t MaxEnergyLimit;
  Double_t PEnergy;

private:
  CbmRecoTracks(const CbmRecoTracks&);
  CbmRecoTracks& operator=(const CbmRecoTracks&);

  ClassDef(CbmRecoTracks, 1);
};


#endif
