/* Copyright (C) 2006-2014 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Matus Kalisky, Denis Bertini [committer], Florian Uhlig, Dominik Smith */

// -----------------------------------------------------------------------
// -----                     CbmTrdHitProducerQa                     -----
// -----               Created 13/12/05  by M. Kalisky               -----
// -----------------------------------------------------------------------


/** CbmTrdHitProducerQa
 **
 *@author M.Kalisky <M.Kalisky@gsi.de>
 **
 ** Class for performance of TRD Hit Producer
 ** Reads all hits and computes Hit pools.
 ** More to come later.
 **/


#ifndef CBMTRDHITPRODUCERQA
#define CBMTRDHITPRODUCERQA 1

#include "FairTask.h"

#include <TFolder.h>

class CbmDigiManager;
class TClonesArray;
class TH1F;

class CbmTrdHitProducerQa : public FairTask {
public:
  /* Defaul constructor */
  CbmTrdHitProducerQa();

  /* Standard constructor */
  CbmTrdHitProducerQa(const char* name, const char* title = "FairTask");

  /* Destructor */
  virtual ~CbmTrdHitProducerQa();

  /* Initialisation */
  InitStatus Init();

  /* Execution */
  virtual void Exec(Option_t* option);

  /* Finish at the end of each event */
  virtual void Finish();

  /* Set the momentum cuts */
  void SetMomentumCuts(Float_t CutLower, Float_t CutHigher)
  {
    fMomCutLower = CutLower;
    fMomCutUpper = CutHigher;
  }

private:
  TFolder fOutFolder;   /// output folder with histos and canvases
  TFolder* histFolder;  /// subfolder for histograms

  CbmDigiManager* fDigiMan = nullptr;

  /* Data branches*/
  TClonesArray* fTrdHitCollection;
  TClonesArray* fTrdPointCollection;
  TClonesArray* fMCTrackArray;

  /** Number of TRD stations **/
  Int_t fNoTrdStations;

  /** Number of layers per station **/
  Int_t fNoTrdPerStation;

  /* Write test histograms */
  void WriteHistograms();

  /* Test histograms*/
  TH1F* fHitPullsX;  // = ((Hit - Point) / HitError)  in X
  TH1F* fHitPullsY;  // = ((Hit - Point) / HitError)  in Y

  TH1F* S1L1edEcut;   //
  TH1F* S1L1edEall;   //
  TH1F* S1L1pidEcut;  //
  TH1F* S1L1pidEall;  //

  TH1F* S3L4edEcut;   //
  TH1F* S3L4edEall;   //
  TH1F* S3L4pidEcut;  //
  TH1F* S3L4pidEall;  //

  /* Momentum cuts for energy distributions */
  Float_t fMomCutLower = 1.25;
  Float_t fMomCutUpper = 1.75;

  ClassDef(CbmTrdHitProducerQa, 3)
};

#endif
