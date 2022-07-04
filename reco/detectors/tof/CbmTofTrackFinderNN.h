/* Copyright (C) 2015-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer], Pierre-Alain Loizeau */

/**
nh, adapt from
 * \file CbmTrdTrackFinderIdeal.h
**/

#ifndef CBMTOFTRACKFINDERNN_H
#define CBMTOFTRACKFINDERNN_H

#include "CbmTofHit.h"
#include "CbmTofTrackFinder.h"
#include "CbmTofTrackFitter.h"
#include "CbmTofTracklet.h"

#include <map>
#include <vector>

#include "LKFMinuit.h"

class TClonesArray;

class CbmTofTrackFinderNN : public CbmTofTrackFinder {
public:
  /**
    * \brief Constructor.
    */
  CbmTofTrackFinderNN();

  /**
    * \brief Destructor
    */
  virtual ~CbmTofTrackFinderNN();

  /**
    * \brief Inherited from CbmTofTrackFinder.
    */
  void Init();

  Int_t DoFind(TClonesArray* fTofHits, TClonesArray* fTofTracks);

  void TrklSeed(Int_t iHit);
  Int_t HitUsed(Int_t iHit);

  /*
   void RemoveMultipleAssignedHits(
			 TClonesArray* fTofHits,
			 Int_t         iDet
			 );
*/

  void UpdateTrackList(Int_t iTrk);
  void UpdateTrackList(CbmTofTracklet* pTrk);

  inline void SetFitter(CbmTofTrackFitter* Fitter) { fFitter = Fitter; }
  inline void SetMaxTofTimeDifference(Double_t val) { fMaxTofTimeDifference = val; }
  inline void SetTxLIM(Double_t val) { fTxLIM = val; }
  inline void SetTyLIM(Double_t val) { fTyLIM = val; }
  inline void SetTxMean(Double_t val) { fTxMean = val; }
  inline void SetTyMean(Double_t val) { fTyMean = val; }
  inline void SetSIGLIM(Double_t val) { fSIGLIM = val; }
  inline void SetSIGLIMMOD(Double_t val) { fSIGLIMMOD = val; }
  inline void SetChiMaxAccept(Double_t val) { fChiMaxAccept = val; }
  inline void SetPosYMaxScal(Double_t val) { fPosYMaxScal = val; }

  inline Double_t GetTxLIM() { return fTxLIM; }
  inline Double_t GetTyLIM() { return fTyLIM; }
  inline Double_t GetTxMean() { return fTxMean; }
  inline Double_t GetTyMean() { return fTyMean; }
  inline Double_t GetSIGLIM() { return fSIGLIM; }
  inline Double_t GetSIGLIMMOD() { return fSIGLIMMOD; }
  inline Double_t GetChiMaxAccept() { return fChiMaxAccept; }

  static void Line3Dfit(CbmTofTracklet* pTrk);
  static void Line3Dfit(CbmTofTracklet* pTrk, Int_t iAddr);
  Bool_t Active(CbmTofTracklet* pTrk);

  void PrintStatus(char* cComm);
  void AddVertex();
  inline void SetAddVertex(int ival) { fiAddVertex = ival; }
  inline void SetVtxNbTrksMin(int ival) { fiVtxNbTrksMin = ival; }

  //Copy constructor
  CbmTofTrackFinderNN(const CbmTofTrackFinderNN& finder);
  //assignment operator
  CbmTofTrackFinderNN& operator=(const CbmTofTrackFinderNN& fSource);

private:
  TClonesArray* fHits;
  TClonesArray* fOutTracks;
  Int_t fiNtrks;                  // Number of Tracks
  CbmTofTrackFitter* fFitter;     // Pointer to TrackFitter concrete class
  CbmTofFindTracks* fFindTracks;  // Pointer to Task
  CbmTofDigiPar* fDigiPar;
  Double_t fMaxTofTimeDifference;
  Double_t fTxLIM;
  Double_t fTyLIM;
  Double_t fTxMean;
  Double_t fTyMean;
  Double_t fSIGLIM;
  Double_t fSIGLIMMOD;
  Double_t fChiMaxAccept;
  Double_t fPosYMaxScal;
  static LKFMinuit fMinuit;

  //intermediate storage variables
  std::vector<CbmTofTracklet*> fTracks;  // Tracklets to which hit is assigned
  //std::vector<std::map <CbmTofTracklet *, Int_t> > fvTrkMap;  // Tracklets to which hit is assigned
  std::vector<std::vector<CbmTofTracklet*>> fvTrkVec;  // Tracklets to which hit is assigned
  int fiAddVertex;
  int fiVtxNbTrksMin;
  ClassDef(CbmTofTrackFinderNN, 1);
};

#endif
