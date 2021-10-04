/* Copyright (C) 2014-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig */

// ----------------------------------------------------------------------------
// -----                    CbmMvdDigitizer header file                   -----
// -----                   Created by C. Dritsa (2009)                    -----
// -----                   Maintained by M.Deveaux (m.deveaux(att)gsi.de) -----
// -----                   Update by P.Sitzmann (p.sitzmann(att)gsi.de    -----
// ----------------------------------------------------------------------------

#ifndef CBMMVDDIGITIZER_H
#define CBMMVDDIGITIZER_H 1

#include "CbmDefs.h"
#include "CbmDigitize.h"

#include "TStopwatch.h"
#include "TString.h"

#include <utility>
#include <vector>

class CbmMvdDetector;
class CbmMvdPileupManager;
class CbmMvdDigi;
class CbmMatch;

class CbmMvdDigitizer : public CbmDigitize<CbmMvdDigi> {

public:
  /** Default constructor **/
  CbmMvdDigitizer();


  /** Standard constructor 
  *@param name  Task name
  *@param mode  0 = MAPS, 1 = Ideal
  **/
  CbmMvdDigitizer(const char* name, Int_t mode = 0, Int_t iVerbose = 1);


  /** Destructor **/
  ~CbmMvdDigitizer();

  void Exec(Option_t* opt);

  /** @brief Detector system ID
   ** @return kMvd
   **/
  ECbmModuleId GetSystemId() const { return ECbmModuleId::kMvd; }

  void SetPileUp(Int_t pileUp) { fNPileup = pileUp; }
  void SetDeltaEvents(Int_t deltaEvents) { fNDeltaElect = deltaEvents; }
  void SetBgFileName(TString fileName) { fBgFileName = fileName; }
  void SetDeltaName(TString fileName) { fDeltaFileName = fileName; }
  void SetBgBufferSize(Int_t nBuffer) { fBgBufferSize = nBuffer; }
  void SetDeltaBufferSize(Int_t nBuffer) { fDeltaBufferSize = nBuffer; }
  void SetMisalignment(Float_t misalignment[3])
  {
    for (Int_t i = 0; i < 3; i++)
      epsilon[i] = misalignment[i];
  }  // set the misalignment in cm

  void BuildEvent();
  void ShowDebugHistograms() { fShowDebugHistos = kTRUE; }

  void SetProduceNoise() { fNoiseSensors = kTRUE; };


  /** @brief Clear data arrays **/
  virtual void ResetArrays();


private:
  /** Hit producer mode (0 = MAPS, 1 = Ideal) **/
  Int_t fMode;

  Bool_t fShowDebugHistos;
  Bool_t fNoiseSensors;

  CbmMvdDetector* fDetector;

  TClonesArray* fInputPoints;
  TClonesArray* fMcPileUp;

  TClonesArray* fTmpMatch;  //! Temporary TClonesArray to absorb from MvdDetector
  TClonesArray* fTmpDigi;   //! Temporary TClonesArray to absorb from MvdDetector

  std::vector<CbmMvdDigi*> fDigiVect;  //! Temporary storage for CbmDaq
  std::vector<CbmMatch*> fMatchVect;   //! Temporary storage for CbmDaq

  std::pair<Float_t, Int_t> fPerformanceDigi;

  UInt_t fDigiPluginNr;

  Double_t fFakeRate;  // Fake hit rate
  Int_t fNPileup;      // Number of pile-up background events
  Int_t fNDeltaElect;  // Number of delta electron events
  Int_t fDeltaBufferSize;
  Int_t fBgBufferSize;
  Float_t epsilon[3];

  TString fInputBranchName;  // Name of input branch (MvdPoint)
  TString fBgFileName;       // Name of background (pileup) file
  TString fDeltaFileName;    // Name of the file containing delta electrons


  TStopwatch fTimer;  ///< ROOT timer

  /** Pileup manager **/
  CbmMvdPileupManager* fPileupManager;
  CbmMvdPileupManager* fDeltaManager;


  // -----   Private methods   ---------------------------------------------
  /** Intialisation **/
  virtual InitStatus Init();


  /** Reinitialisation **/
  virtual InitStatus ReInit();


  /** Virtual method Finish **/
  virtual void Finish();


  /** Register the output arrays to the IOManager **/
  void Register();

  void GetMvdGeometry();


  /** Clear the hit arrays **/
  void Reset();


  /** Print digitisation parameters **/
  void PrintParameters();

private:
  CbmMvdDigitizer(const CbmMvdDigitizer&);
  CbmMvdDigitizer operator=(const CbmMvdDigitizer&);

  ClassDef(CbmMvdDigitizer, 1);
};


#endif
