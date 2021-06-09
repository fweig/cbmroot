/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann, Florian Uhlig [committer] */

/** @file CbmTofCalibrator.h
 ** @author nh <N.Herrmann@gsi.de>
 ** @date 28.02.2020
 **/

#ifndef CBMTOFCALIBRATOR_H
#define CBMTOFCALIBRATOR_H 1

// TOF Classes and includes
class CbmTofDigi;
class CbmTofHit;
class CbmMatch;
class CbmEvent;
class CbmVertex;
// Geometry
class CbmTofGeoHandler;
class CbmTofDetectorId;
class CbmTofDigiPar;
class CbmTofDigiBdfPar;
class CbmTofCell;
class CbmTofFindTracks;
class CbmDigiManager;

#include "CbmDigiManager.h"
#include "CbmTofDigi.h"
#include "CbmTofEventClusterizer.h"
#include "CbmTofFindTracks.h"
#include "CbmTofHit.h"
#include "CbmTofTracklet.h"
#include "CbmTofTrackletParam.h"
#include "CbmTofTrackletTools.h"

#include "FairTrackParam.h"

#include "TH1.h"
#include "TH2.h"
#include "TMath.h"

#include <vector>

class TClonesArray;

/** @class CbmTofCalibrator
 ** @brief  contains filling and updating of calibration histos
 ** @author nh
 **/
class CbmTofCalibrator : public FairTask {

public:
  /**   Constructor   **/
  CbmTofCalibrator();

  /**   Destructor   **/
  virtual ~CbmTofCalibrator();

  InitStatus Init();
  Bool_t InitParameters();
  Bool_t CreateCalHist();
  void FillCalHist(CbmTofTracklet* pTrk, Int_t iOpt, CbmEvent* pEvent = NULL);
  Bool_t UpdateCalHist(Int_t iOpt);
  void ReadHist(TFile* fhFile);
  void WriteHist(TFile* fhFile);

  inline void SetR0Lim(Double_t dVal) { fdR0Lim = dVal; }
  inline void SetBeam(Bool_t bVal) { fbBeam = bVal; }

private:
  CbmDigiManager* fDigiMan;
  CbmTofEventClusterizer* fTofClusterizer;
  CbmTofFindTracks* fTofFindTracks;
  CbmTofTrackletTools* fTrackletTools;

  const std::vector<CbmTofDigi>* fTofCalDigiVec = nullptr;

  CbmTofDigiPar* fDigiPar;
  CbmTofDigiBdfPar* fDigiBdfPar;
  TClonesArray* fTofDigiMatchColl;  // TOF Digi Links
  TH1* fhCalR0;
  TH1* fhCalDX0;
  TH1* fhCalDY0;

  std::vector<TH2*> fhCalPosition;                        // [nbDet]
  std::vector<TH2*> fhCalPos;                             // [nbDet]
  std::vector<TH2*> fhCalTOff;                            // [nbDet]
  std::vector<TH2*> fhCalTot;                             // [nbDet]
  std::vector<std::vector<std::vector<TH2*>>> fhCalWalk;  // [nbDet][nbCh][nSide]

  std::vector<TH1*> fhCorPos;                             // [nbDet]
  std::vector<TH1*> fhCorTOff;                            // [nbDet]
  std::vector<TH1*> fhCorTot;                             // [nbDet]
  std::vector<TH1*> fhCorTotOff;                          // [nbDet]
  std::vector<TH1*> fhCorSvel;                            // [nbDet]
  std::vector<std::vector<std::vector<TH1*>>> fhCorWalk;  // [nbDet][nbCh][nSide]

  std::map<UInt_t, UInt_t> fDetIdIndexMap;

  Double_t fdR0Lim = 0.;
  Bool_t fbBeam    = kFALSE;

  CbmTofCalibrator(const CbmTofCalibrator&) = delete;
  CbmTofCalibrator operator=(const CbmTofCalibrator&) = delete;

  ClassDef(CbmTofCalibrator, 1);
};

#endif /* CBMTOFCALIBRATOR_H */
