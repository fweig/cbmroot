/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBM2019CHECKDIGISMUCH_H
#define CBMMCBM2019CHECKDIGISMUCH_H

#include "CbmDefs.h"

#include "FairTask.h"

#include "TString.h"

class TClonesArray;
class TH1;
class TH2;
class CbmDigiManager;

class CbmMcbm2019CheckDigisMuch : public FairTask {
public:
  CbmMcbm2019CheckDigisMuch();

  CbmMcbm2019CheckDigisMuch(const CbmMcbm2019CheckDigisMuch&) = delete;
  CbmMcbm2019CheckDigisMuch
  operator=(const CbmMcbm2019CheckDigisMuch&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmMcbm2019CheckDigisMuch(Int_t verbose);


  /** Destructor **/
  ~CbmMcbm2019CheckDigisMuch();


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

  inline void SetTimeWindow(UInt_t uTsJump,
                            Double_t dFirstTsOffset,
                            UInt_t uPrevTs     = 2,
                            UInt_t uPostTs     = 2,
                            Double_t dTsLength = 10240000.) {
    fuTsJump      = uTsJump;
    fdFirstTsOffs = dFirstTsOffset;
    fuStartTs     = uTsJump - uPrevTs;
    fuStopTs      = uTsJump + uPostTs;
    fdTsLength    = dTsLength;
    fdStartTime   = fuStartTs * dTsLength + dFirstTsOffset;
  }
  inline void SetDigiDistPlotStartTime(Double_t dStartTime) {
    fdDigiDistStart = dStartTime;
    fdDigiDistStop  = dStartTime + 500000;
  }

  inline void SetMuchPulseradcLimits(UInt_t uMin, UInt_t uMax) {
    fuMinAdcPulserMuch = uMin;
    fuMaxAdcPulserMuch = uMax;
  }

  inline void SetOutFilename(TString sNameIn) { fOutFileName = sNameIn; }

private:
  void CreateHistos();
  void WriteHistos();

  static const UInt_t kuMaxNbAsics  = 36;
  static const UInt_t kuNbChansAsic = 128;

  CbmDigiManager* fDigiMan   = nullptr;  //!
  UInt_t fNrTs               = 0;
  TH2* fSameChanDigisDistEvo = nullptr;

  UInt_t fuTsJump        = 3;
  Double_t fdFirstTsOffs = 0.0;
  UInt_t fuStartTs       = 0.0;
  UInt_t fuStopTs        = 0.0;
  Double_t fdTsLength    = 0.0;
  Double_t fdStartTime   = 0.0;

  Double_t fdDigiDistStart = 0.0;
  Double_t fdDigiDistStop  = 0.0;

  UInt_t fuMinAdcPulserMuch = 5;
  UInt_t fuMaxAdcPulserMuch = 15;
  TH2* fDigisPerAsicEvo     = nullptr;
  Double_t fdLastMuchDigi[kuMaxNbAsics][kuNbChansAsic];
  Double_t fdLastMuchDigiPulser[kuMaxNbAsics][kuNbChansAsic];

  TH2* fDigisPerChanEvo = nullptr;

  TString fOutFileName {"data/CheckDigisMuch.root"};

  ClassDef(CbmMcbm2019CheckDigisMuch, 1);
};

#endif  // CBMMCBM2019CHECKDIGISMUCH_H
