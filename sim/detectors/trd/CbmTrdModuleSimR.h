/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Etienne Bechtel, Florian Uhlig [committer] */

#ifndef CBMTRDMODULESIMR_H
#define CBMTRDMODULESIMR_H

#include "CbmTrdCheckUtil.h"
#include "CbmTrdModuleSim.h"
#include "CbmTrdRawToDigiR.h"

class TRandom3;
class TFile;
class TH2D;
class TH1I;
class TH1D;
class CbmTrdParSetAsic;
class CbmTimeSlice;

/**
  * \brief Simulation module implementation for rectangular pad geometry 
  **/
class CbmTrdModuleSimR : public CbmTrdModuleSim {
public:
  CbmTrdModuleSimR(Int_t mod, Int_t ly, Int_t rot);
  virtual ~CbmTrdModuleSimR() { ; }
  void GetCounters(Int_t& nEl, Int_t& nLattice, Int_t& nOverThr) const
  {
    nEl      = nofElectrons;
    nLattice = nofLatticeHits;
    nOverThr = nofPointsAboveThreshold;
  }
  Int_t FlushBuffer(ULong64_t time = 0);
  Bool_t MakeDigi(CbmTrdPoint* p, Double_t time, Bool_t TR);
  Bool_t MakeRaw(/*CbmTrdPoint *p*/) { return kTRUE; }

  //seter functions
  void SetAsicPar(CbmTrdParSetAsic* p = NULL);
  void SetNCluster(Int_t nCluster) { fnClusterConst = nCluster; }
  void SetNoiseLevel(Double_t sigma_keV);
  void SetDistributionPoints(Int_t points);
  void SetSpadicResponse(Double_t calibration, Double_t tau);
  void SetPulsePars(Int_t mode);
  void SetPulseMode(Bool_t pulsed);
  void SetGamma(Double_t gamma) { fGamma = gamma; }
  void SetTriggerThreshold(Double_t minCharge) { fMinimumChargeTH = minCharge; }
  void SetPadPlaneScanArea(Int_t row);
  void ResetCounters()
  {
    nofElectrons            = 0;
    nofLatticeHits          = 0;
    nofPointsAboveThreshold = 0;
  }
  void SetMessageConverter(CbmTrdRawToDigiR* conv) { fMessageConverter = conv; }
  void GetModuleType(CbmTrdRawToDigiR* conv) { fMessageConverter = conv; }
  void SetQA(CbmTrdCheckUtil* qa) { fQA = qa; }

private:
  CbmTrdModuleSimR& operator=(const CbmTrdModuleSimR&);
  CbmTrdModuleSimR(const CbmTrdModuleSimR&);

  //pulsed mode
  void AddDigitoPulseBuffer(Int_t address, Double_t reldrift, Double_t charge, Double_t chargeTR, Double_t time,
                            Int_t trigger, Int_t epoints, Int_t ipoint);
  std::vector<Double_t> MakePulse(Double_t charge, std::vector<Double_t> pulse, Int_t address);
  void AddToPulse(Int_t address, Double_t charge, Double_t reldrift, std::vector<Double_t> pulse);
  Bool_t CheckMulti(Int_t address, std::vector<Double_t> pulse);
  Int_t CheckTrigger(std::vector<Double_t> pulse);
  Double_t CalcResponse(Double_t t);
  void ProcessPulseBuffer(Int_t address, Bool_t FNcall, Bool_t MultiCall, Bool_t down, Bool_t up);
  Int_t GetMultiBin(std::vector<Double_t> pulse);

  //vintage EB
  void AddDigi(Int_t address, Double_t charge, Double_t chargeTR, Double_t time, Int_t trigger);

  //non pulsed TB mode
  void AddDigitoBuffer(Int_t address, Double_t charge, Double_t chargeTR, Double_t time, Int_t trigger);
  void ProcessBuffer(Int_t address);

  //Buffer managment
  void CheckBuffer(Bool_t EB);
  void CleanUp(Bool_t EB);

  //general tools
  Bool_t DistributeCharge(Double_t pointin[3], Double_t pointout[3], Double_t delta[3], Double_t pos[3], Int_t ipoints);
  Double_t AddDrifttime(Double_t x, Double_t z);
  Double_t AddDrifttime(Int_t x);
  Double_t AddNoise(Double_t charge);
  Double_t GetStep(Double_t dist, Int_t roll);
  std::pair<Int_t, std::vector<Double_t>> GetTotalSteps(Double_t In[3], Double_t Out[3], Double_t dist);
  Int_t AddNoiseADC();
  Int_t AddCrosstalk(Double_t address, Int_t i, Int_t sec, Int_t row, Int_t col, Int_t ncols);
  Double_t CalcPRF(Double_t x, Double_t W, Double_t h);
  void CheckTime(Int_t address);
  void NoiseTime(ULong64_t eventTime);
  void SetDist(Int_t dist) { fDistributionMode = dist; }
  std::vector<Double_t> AddCorrelatedNoise(std::vector<Double_t> pulse);

  //general MC data usage - distributing MC charge over the pad plane
  void ScanPadPlane(const Double_t* local_point, Double_t reldrift, Double_t clusterELoss, Double_t clusterELossTR,
                    Int_t epoints, Int_t ipoint);


  //spadic response parameters
  Double_t fCalibration = 35. / 1.1107 / 0.8 * 1.5;  // calibrating pulse height to MIP
  Double_t fEReco = 0.;     // calibrating energy reconstruction to the amount and position of used samples of the pulse
  Double_t fTau   = 120.0;  // peaking time of the pulse
  Double_t fTriggerSlope = 12.0;  // trigger setting of the pulse
  Int_t fRecoMode        = 2;     // mode for reconstruction samples

  //general globals
  Double_t fSigma_noise_keV;
  Double_t fMinimumChargeTH;
  Double_t fCurrentTime;
  Double_t fAddress;
  Double_t fLastEventTime;
  Double_t fEventTime;
  Double_t fLastTime;
  Double_t fCollectTime;
  Int_t fnClusterConst;
  Int_t fnScanRowConst;

  Bool_t fPulseSwitch;
  Bool_t fPrintPulse;
  Bool_t fTimeShift;
  Bool_t fAddCrosstalk;
  Bool_t fClipping;

  Int_t fepoints;
  Int_t fAdcNoise;
  Int_t fDistributionMode;
  Double_t fCrosstalkLevel;

  Int_t fLastPoint = 0;
  Int_t fLastEvent = 0;
  Int_t frecostart = 2;
  Int_t frecostop  = 5;

  Int_t fClipLevel         = 500;
  Int_t fPresamples        = 2;
  Int_t fShapingOrder      = 1;
  Int_t fMaxBin            = 2 + fPresamples;
  Int_t fMinBin            = 0 + fPresamples;
  Double_t fGamma          = 0.;
  Double_t fMinDrift       = 12.5;
  CbmTimeSlice* fTimeSlice = NULL;  ///< link to CBM time slice


  //counters
  Int_t nofElectrons;
  Int_t nofLatticeHits;
  Int_t nofPointsAboveThreshold;

  Double_t fDriftStart = 0;

  std::map<Int_t, std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>> fAnalogBuffer;
  std::map<Int_t, std::pair<std::vector<Double_t>, CbmMatch*>> fPulseBuffer;
  std::map<Int_t, std::pair<Double_t, Int_t>> fMultiBuffer;
  std::map<Int_t, Double_t> fTimeBuffer;
  std::map<Int_t, Double_t> fShiftQA;
  std::map<Int_t, std::vector<std::map<TString, Int_t>>> fLinkQA;
  std::map<Int_t, Double_t> fMCQA;
  std::map<Int_t, std::vector<std::vector<Int_t>>> fMCBuffer;
  Float_t fQAPosition[3] = {0., 0., 0.};
  Float_t fQAPos_out[3]  = {0., 0., 0.};

  CbmTrdRawToDigiR* fMessageConverter = NULL;
  TH2D* fDriftTime                    = NULL;
  CbmTrdCheckUtil* fQA                = NULL;
  Bool_t fDebug                       = true;

  ClassDef(CbmTrdModuleSimR,
           1)  // Simulation module implementation for rectangular pad geometry
};

#endif
