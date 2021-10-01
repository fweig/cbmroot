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
class CbmQaCanvas;
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

  /* Set number of TRD stations */
  void SetNumberStations(Int_t nStations) { fNoTrdStations = nStations; }

  /** Set number of layers per station **/
  void SetLayersPerStations(Int_t nLayers) { fNoTrdPerStation = nLayers; }

private:
  TFolder fOutFolder;             /// output folder with histos and canvases
  TFolder* histFolder = nullptr;  /// subfolder for histograms

  CbmDigiManager* fDigiMan = nullptr;

  /* Data branches*/
  TClonesArray* fTrdHitCollection   = nullptr;
  TClonesArray* fTrdPointCollection = nullptr;
  TClonesArray* fMCTrackArray       = nullptr;

  /** Number of TRD stations **/
  Int_t fNoTrdStations = 4;

  /** Number of layers per station **/
  Int_t fNoTrdPerStation = 1;

  /* Write test histograms */
  void WriteHistograms();

  /* Test histograms*/
  std::vector<TH1F*> fvhHitPullsX;
  std::vector<TH1F*> fvhHitPullsY;

  std::vector<TH1F*> fvhedEcut;
  std::vector<TH1F*> fvhedEall;
  std::vector<TH1F*> fvhpidEcut;
  std::vector<TH1F*> fvhpidEall;

  /* Canvases */
  std::vector<CbmQaCanvas*> fvdECanvas;
  std::vector<CbmQaCanvas*> fvPullCanvas;

  /* Momentum cuts for energy distributions */
  Float_t fMomCutLower = 1.;
  Float_t fMomCutUpper = 7.;

  ClassDef(CbmTrdHitProducerQa, 3)
};

#endif
