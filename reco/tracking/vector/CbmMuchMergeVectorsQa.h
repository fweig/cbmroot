/* Copyright (C) 2014-2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexander Zinchenko [committer] */

/** CbmMuchMergeVectorsQa.h
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **
 ** Task class for MUCH vector merger QA.
 **
 **/

#ifndef CBMMUCHMERGEVECTORSQA_H_
#define CBMMUCHMERGEVECTORSQA_H_ 1

#include "CbmMuchGeoScheme.h"

#include "FairTask.h"

#include <set>
#include <vector>

class CbmMuchTrack;
class TClonesArray;
class TH1D;
class TH2D;

class CbmMuchMergeVectorsQa : public FairTask {
public:
  /** Default (standard) constructor **/
  CbmMuchMergeVectorsQa();

  /** Destructor **/
  virtual ~CbmMuchMergeVectorsQa();

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
  Double_t fZpos[5][10];         // Z-pos. of double layers in stations

  TClonesArray* fTracks;
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

  TH1D* fhSim;
  TH1D* fhRec;
  TH1D* fhMatchMult;
  TH1D* fhMatchOver;
  TH1D* fhOverlap;
  TH1D** fhChi2mat;
  TH2D* fhSimRec;

  void BookHistos();
  Bool_t CheckMatch(CbmMuchTrack* vec);
  void CheckEffic();

  CbmMuchMergeVectorsQa(const CbmMuchMergeVectorsQa&);
  CbmMuchMergeVectorsQa& operator=(const CbmMuchMergeVectorsQa&);

  ClassDef(CbmMuchMergeVectorsQa, 1)
};

#endif
