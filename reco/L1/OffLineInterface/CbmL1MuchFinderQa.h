/** @file CbmL1MuchFinderQa.h
  * @copyright Copyright (C) 2007-2009 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Sergey Gorbunov [committer] **/

#ifndef CBM_L1_MuchFinderQa_h
#define CBM_L1_MuchFinderQa_h

#include "CbmStsKFTrackFitter.h"

#include "FairTask.h"

#include "TH1.h"
#include "TLorentzVector.h"


class TClonesArray;
class TProfile;

class CbmL1MuchFinderQa : public FairTask {
public:
  /** Constructor **/
  CbmL1MuchFinderQa(const char* name = "CbmL1MuchFinderQa", Int_t iVerbose = 1);

  /** Destructor **/
  ~CbmL1MuchFinderQa();

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
  TClonesArray* fMuchTracks;  //! Much tracks
  TClonesArray* fSTSTrackMatch;

  CbmVertex* fPrimVtx;
  CbmStsKFTrackFitter fStsFitter;

  TDirectory* histodir;

  void Write();
  void writedir2current(TObject* obj);

  TProfile *fhPerfSignal, *fhPerfAll, *fhGhost;
  TH1F *histPull_dx[40], *histPull_dy[40], *histPull_tx[40], *histPull_ty[40], *histPull_qp[40], *histChi2[40];

public:
  ClassDef(CbmL1MuchFinderQa, 1);
};


#endif
