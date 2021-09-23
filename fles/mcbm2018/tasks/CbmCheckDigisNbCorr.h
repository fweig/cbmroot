/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef CBMCHECKDIGISNBCORR_H
#define CBMCHECKDIGISNBCORR_H

#include "CbmTofDigi.h"

#include "FairTask.h"

#include "TString.h"

#include <vector>

class TClonesArray;
class TH1;
class TH2;
class TProfile;
class CbmDigiManager;

class CbmCheckDigisNbCorr : public FairTask {
public:
  /** Default constructor **/
  CbmCheckDigisNbCorr();

  CbmCheckDigisNbCorr(const CbmCheckDigisNbCorr&) = delete;
  CbmCheckDigisNbCorr operator=(const CbmCheckDigisNbCorr&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmCheckDigisNbCorr(Int_t verbose);


  /** Destructor **/
  ~CbmCheckDigisNbCorr();


  /** Initiliazation of task at the beginning of a run **/
  virtual InitStatus Init();

  /** ReInitiliazation of task when the runID changes **/
  virtual InitStatus ReInit();


  /** Executed for each event. **/
  virtual void Exec(Option_t*);

  /** Load the parameter container from the runtime database **/
  virtual void SetParContainers();

  /** Finish task called at the end of the run **/
  virtual void Finish();

  void SetStsOffset(Double_t dval = 1600) { fdStsOffset = dval; }

  void SetMuchOffset(Double_t dval = 1600) { fdMuchOffset = dval; }

  void SetTofOffset(Double_t dval = 50) { fdTofOffset = dval; }

  void SetRichOffset(Double_t dval = 1200) { fdRichOffset = dval; }

  inline void SetTimeInterval(Double_t dInterNs, Double_t dTsInNs)
  {
    fdBinWidthNs = dInterNs;
    fdTsLengthNs = dTsInNs;
  }

  inline void SetT0PulserTotLimits(UInt_t uMin, UInt_t uMax)
  {
    fuMinTotPulserT0 = uMin;
    fuMaxTotPulserT0 = uMax;
  }

  inline void SetOutFilename(TString sNameIn) { fOutFileName = sNameIn; }

private:
  void CalcNrBins();
  void CreateHistos();
  void WriteHistos();


  /** Input array from previous already existing data level **/
  TClonesArray* fTsMetaData = nullptr;
  CbmDigiManager* fDigiMan  = nullptr;  //! Interface to digi data
  /** T0 is not included in CbmDigiManager, so add it explicitly here **/
  const std::vector<CbmTofDigi>* fT0DigiVec = nullptr;  //!
  TClonesArray* fT0DigiArr {nullptr};                   //!

  /// User settings: Data correction parameters
  UInt_t fuMinTotPulserT0;
  UInt_t fuMaxTotPulserT0;

  //
  Int_t fNrTs = 0;

  Int_t fNrOfT0Errors   = 0;
  Int_t fNrOfT0Digis    = 0;
  Int_t fNrOfStsErrors  = 0;
  Int_t fNrOfStsDigis   = 0;
  Int_t fNrOfMuchErrors = 0;
  Int_t fNrOfMuchDigis  = 0;
  Int_t fNrOfTofErrors  = 0;
  Int_t fNrOfTofDigis   = 0;
  Int_t fNrOfRichErrors = 0;
  Int_t fNrOfRichDigis  = 0;

  Double_t fdStsOffset  = 0.0;
  Double_t fdMuchOffset = 0.0;
  Double_t fdTofOffset  = 0.0;
  Double_t fdRichOffset = 0.0;

  Double_t fdTsLengthNs    = 100 * 102e3;
  Double_t fdBinWidthNs    = 10e3;
  Int_t fiBinNb            = fdTsLengthNs / fdBinWidthNs;
  Double_t fdTsStartOffset = 0;

  std::vector<UInt_t> fvuNbDigisPerBinT0;
  std::vector<UInt_t> fvuNbDigisPerBinSts;
  std::vector<UInt_t> fvuNbDigisPerBinMuch;
  std::vector<UInt_t> fvuNbDigisPerBinTof;
  std::vector<UInt_t> fvuNbDigisPerBinRich;

  TH2* fT0StsCorr  = nullptr;
  TH2* fT0MuchCorr = nullptr;
  TH2* fT0TofCorr  = nullptr;
  TH2* fT0RichCorr = nullptr;

  TH2* fStsMuchCorr = nullptr;
  TH2* fStsTofCorr  = nullptr;
  TH2* fStsRichCorr = nullptr;

  TH2* fMuchTofCorr  = nullptr;
  TH2* fMuchRichCorr = nullptr;

  TH2* fTofRichCorr = nullptr;

  TProfile* fT0StsCorrProf  = nullptr;
  TProfile* fT0MuchCorrProf = nullptr;
  TProfile* fT0TofCorrProf  = nullptr;
  TProfile* fT0RichCorrProf = nullptr;

  TProfile* fStsMuchCorrProf = nullptr;
  TProfile* fStsTofCorrProf  = nullptr;
  TProfile* fStsRichCorrProf = nullptr;

  TProfile* fMuchTofCorrProf  = nullptr;
  TProfile* fMuchRichCorrProf = nullptr;

  TProfile* fTofRichCorrProf = nullptr;

  static const UInt_t kuMaxNbStsDpbs = 2;
  std::vector<std::vector<UInt_t>> fvuNbDigisPerBinStsDpb;
  TH2* fT0StsDpbCorr[kuMaxNbStsDpbs];
  TH2* fStsMuchDpbCorr[kuMaxNbStsDpbs];
  TH2* fStsTofDpbCorr[kuMaxNbStsDpbs];
  TH2* fStsRichDpbCorr[kuMaxNbStsDpbs];
  TProfile* fT0StsDpbCorrProf[kuMaxNbStsDpbs];
  TProfile* fStsMuchDpbCorrProf[kuMaxNbStsDpbs];
  TProfile* fStsTofDpbCorrProf[kuMaxNbStsDpbs];
  TProfile* fStsRichDpbCorrProf[kuMaxNbStsDpbs];
  /*
    static const UInt_t kuMaxNbMuchAsics = 24;
    TH2*      fT0MuchAsicCorr[kuMaxNbMuchAsics];
    TProfile* fT0MuchAsicCorrProf[kuMaxNbMuchAsics];
*/
  TString fOutFileName {"HistosCheckDigisNbCorr.root"};

  ClassDef(CbmCheckDigisNbCorr, 1);
};

#endif
