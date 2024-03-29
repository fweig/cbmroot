/* Copyright (C) 2014-2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexander Zinchenko [committer] */

/** CbmMuchFindVectorsQa.h
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **
 ** Task class for MUCH vector finder QA.
 **
 **/

#ifndef CBMMUCHFINDVECTORSQA_H_
#define CBMMUCHFINDVECTORSQA_H_ 1

#include "CbmMuchGeoScheme.h"

#include "FairTask.h"

#include <set>
#include <vector>

class CbmMuchTrack;
class TClonesArray;
class TH1D;
class TH2D;

class CbmMuchFindVectorsQa : public FairTask {
public:
  /** Default (standard) constructor **/
  CbmMuchFindVectorsQa();

  /** Destructor **/
  virtual ~CbmMuchFindVectorsQa();

  /** Initialisation **/
  virtual InitStatus Init();

  /** Task execution **/
  virtual void Exec(Option_t* opt);

  /** Finish at the end **/
  virtual void Finish();

  /** SetParContainers **/
  virtual void SetParContainers();

  /** Accessors **/
  //Int_t GetNofTracks()           { return fNofTracks; };

private:
  CbmMuchGeoScheme* fGeoScheme;  // Geometry scheme
  Int_t fStatFirst;              // First straw station No.
  Int_t fNstat;                  // Number of straw stations
  Int_t fNdoubl[10];             // Number of double layers in stations
  Double_t fZpos[7][10];         // Z-pos. of double layers in stations

  TClonesArray* fVectors;
  TClonesArray* fMCTracks;
  TClonesArray* fPoints;
  TClonesArray* fHits;
  TClonesArray* fHitsGem;
  TClonesArray* fDigis;
  TClonesArray* fDigisGem;
  TClonesArray* fDigiMatches;
  TClonesArray* fDigiMatchesGem;
  TClonesArray* fClusters;

  TH1D** fhNvec;
  TH1D** fhNdoubl;
  TH1D** fhNhits;
  TH1D** fhNhitsOk;
  TH1D** fhChi2;
  TH1D** fhNgood;
  TH1D** fhNghost;
  TH1D** fhChi2ok;
  TH1D** fhChi2bad;
  TH1D** fhDx;
  TH1D** fhDy;
  TH1D** fhDtx;
  TH1D** fhDty;
  TH1D** fhIds;
  TH2D** fhIdVsEv;
  TH1D** fhDtxAll;
  TH1D** fhDtyAll;
  TH1D** fhDtxOk;
  TH1D** fhDtyOk;
  TH1D* fhDtube[7][10];
  TH2D* fhDtube2[7][10];
  TH1D** fhShort;
  TH2D** fhOverlap;
  TH1D* fhSim;
  TH1D* fhRec;
  TH1D** fhChi2mat;
  TH1D** fhMatchMult;
  TH1D** fhOccup;
  TH1D* fhMCFit[7][10];
  TH1D* fhZXY[2];  //! transient histos
  TH1D* fhEvents;  //! transient histo
  TH2D** fhDx12;
  TH2D** fhDx23;
  TH2D** fhDy12;
  TH2D** fhDy23;

  void BookHistos();
  Bool_t CheckMatch(CbmMuchTrack* vec);
  Bool_t CheckMatchGem(CbmMuchTrack* vec);
  void CheckShorts(TClonesArray* hitArray);
  void CheckEffic();

  CbmMuchFindVectorsQa(const CbmMuchFindVectorsQa&);
  CbmMuchFindVectorsQa& operator=(const CbmMuchFindVectorsQa&);

  ClassDef(CbmMuchFindVectorsQa, 1)
};

#endif
