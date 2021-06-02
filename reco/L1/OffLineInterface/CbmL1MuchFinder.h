/** @file CbmL1MuchFinder.h
  * @copyright Copyright (C) 2006-2009 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Denis Bertini [committer], Sergey Gorbunov **/

#ifndef CBM_L1_MuchFinder_h
#define CBM_L1_MuchFinder_h

#include "CbmStsKFTrackFitter.h"

#include "FairTask.h"

#include "TH1.h"
#include "TLorentzVector.h"


class TClonesArray;

class CbmL1MuchFinder : public FairTask {
public:
  /** Constructor **/
  CbmL1MuchFinder(const char* name = "CbmL1MuchFinder", Int_t iVerbose = 1);

  /** Destructor **/
  ~CbmL1MuchFinder();

  /// * FairTask methods

  /** Intialisation at begin of run. To be implemented in the derived class.
   *@value  Success   If not kSUCCESS, task will be set inactive.
   **/
  InitStatus Init();

  /** Reinitialisation.
   *@value  Success   If not kSUCCESS, task will be set inactive.
   **/
  InitStatus ReInit();

  /** Intialise parameter containers.
   **/
  void SetParContainers();

  void Exec(Option_t* option);

  /** Action after each event. **/
  void Finish();

private:
  TClonesArray* fMuchPoints;  //! Much MC points
  TClonesArray* fMuchHits;    //! Much Hits
  TClonesArray* fStsTracks;
  TClonesArray* fMCTracks;
  TClonesArray* fSTSTrackMatch;
  TClonesArray* fTrackCollection;  //! Much tracks

  CbmVertex* fPrimVtx;
  CbmStsKFTrackFitter fStsFitter;

  TDirectory* histodir;

  void Write();
  void writedir2current(TObject* obj);

  TH1F* fhNBranches;

public:
  ClassDef(CbmL1MuchFinder, 1);
};


#endif
