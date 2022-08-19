/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                    CbmTrdSetTracksPidLike header file          -----
// -----                  Created 25/02/2007  by F. Uhlig              -----
// -----                  Updated 31/08/2016  by J. Book               -----
// -------------------------------------------------------------------------


/** CbmTrdSetTracksPidLike
 **
 *@author F.Uhlig <F.Uhlig@gsi.de>
 **
 ** Task class for PID calculation in the TRD using the 
 ** Likelihood method.
 ** Input: TClonesArray of CbmTrdTrack
 ** Parameters: The parameters for this class are histograms 
 ** with the energy loss information for five particle species
 ** (electrons, pions, kaons, protons and muons) as a function 
 ** of momentum. Between these momenta a interpoaltion is done.
 **
 **/


#ifndef CBMTRDSETTRACKSPIDLIKE_H
#define CBMTRDSETTRACKSPIDLIKE_H 1

#include "CbmTrdParSetGas.h"

#include "FairTask.h"

class TClonesArray;

class CbmTrdSetTracksPidLike : public FairTask {

public:
  /** Default constructor **/
  CbmTrdSetTracksPidLike();


  /** Standard constructor 
   **
   *@param name   Name of class
   *@param title  Task title
   **/
  CbmTrdSetTracksPidLike(const char* name, const char* title = "FairTask");


  /** Destructor **/
  virtual ~CbmTrdSetTracksPidLike();


  /** Initialisation at beginning of each event **/
  virtual InitStatus Init();

  /** Task execution **/
  virtual void Exec(Option_t* opt);


  /** Finish at the end of each event **/
  virtual void Finish();


  /** SetParContainers **/
  virtual void SetParContainers();


  /** Accessors **/
  Int_t GetNofTracks() const { return fNofTracks; };

  /** Setters **/
  void SetInputFileName(const char* file) { fFileName = file; }
  void SetUseMCInfo(Bool_t use = kTRUE) { fMCinput = use; }
  void SetUseMomDependence(Bool_t use = kTRUE) { fMomDep = use; }

private:
  CbmTrdSetTracksPidLike& operator=(const CbmTrdSetTracksPidLike&);
  CbmTrdSetTracksPidLike(const CbmTrdSetTracksPidLike&);

  TString fFileName               = "";      // input file
  Bool_t fMCinput                 = kFALSE;  // use MC information for input histograms
  Bool_t fMomDep                  = kTRUE;   // use momentum dependence for input histograms
  TClonesArray* fTrackArray       = NULL;    /** Input array of TRD tracks */
  TClonesArray* fTrdHitArray      = NULL;    /** Input array of TRD Hits */
  TClonesArray* fglobalTrackArray = NULL;    /** Input array of global tracks */

  TObjArray* fHistdEdx = NULL;  // Prob. of dEdx for 5 particle species
  Int_t fNofTracks     = 0;     // Number of tracks successfully fitted

  CbmTrdParSetGas* fGasPar = NULL;  // Gas parameters, path to input reference spectra located here

  static const Int_t fgkNParts = 5;  // numer of different particle species
  enum EParticleType
  {
    kElectron = 0,
    kPion     = 1,
    kKaon     = 2,
    kProton   = 3,
    kMuon     = 4,
  };

  /** Read the histograms from file **/
  Bool_t ReadData();

  /** Calculate probability for particle (with momentum) and dEdx **/
  Double_t GetProbability(Int_t iType, Double_t mom, Double_t dedx) const;


  ClassDef(CbmTrdSetTracksPidLike, 2);
};

#endif
