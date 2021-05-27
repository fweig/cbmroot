#ifndef CBMTRDRAWTODIGI_H
#define CBMTRDRAWTODIGI_H

#include "CbmTrdCheckUtil.h"
#include "CbmTrdDigi.h"
//#include "CbmSpadicRawMessage22.h"

#include <FairRootManager.h>

#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <map>
#include <string>
#include <vector>

class CbmTrdRawToDigiR : public TObject {


public:
  /** @brief default Constructor with messages
   **/
  CbmTrdRawToDigiR();

  CbmTrdRawToDigiR(std::string readfile);

  /** @brief Constructor with messages and preset reconstruction mode
   **/
  CbmTrdRawToDigiR(Double_t cal, Double_t tau, Int_t mode);

  /** @brief Constructor with messages and selection mask
   **/
  CbmTrdRawToDigiR(Double_t cal, Double_t tau, std::vector<Int_t> mask);

  /** @brief Destructor **/
  virtual ~CbmTrdRawToDigiR() { ; }

  //  CbmTrdDigi* MakeDigi(CbmSpadicRawMessage22* raw);
  CbmTrdDigi* MakeDigi(std::vector<Int_t> samples, Int_t channel, Int_t uniqueModuleId, ULong64_t time,
                       Bool_t FN = false);

  Float_t GetTimeShift(std::vector<Int_t> samples);
  Double_t GetCharge(std::vector<Int_t> samples, Int_t shift = -1);

  void SetCalibration(Double_t cal) { fCalibration = cal; }
  void SetTau(Double_t tau) { fTau = tau; }
  void SetRecoMode(Int_t mode) { fRecoMode = mode; }
  void SetShapingOrder(Int_t order) { fShapingOrder = order; }
  void SetRecoMask(std::vector<Int_t> mask) { fSampleMask = mask; }
  void SetSetter(Bool_t set) { fSet = set; }
  void SetReadFile(std::string file) { fReadFile = file; }
  void SetWriteFile(std::string file) { fWriteFile = file; }
  void SetMaxBin(Int_t bin) { fMaxBin = bin; }
  void SetMinBin(Int_t bin) { fMinBin = bin; }
  void SetPresamples(Int_t pre) { fPresamples = pre; }
  void SetLookup(Int_t mode) { fLookUp = mode; }
  void SetPars(Int_t mode, Double_t cal, Double_t tau, std::vector<Int_t> mask);
  void Init();
  void FillLookUps(std::string write = "");
  void WriteMaps(std::string file = "") { (void) file; }  //dummy for now
  void ReadMaps(std::string file = "");
  void SetDebug(Bool_t debug) { fDebug = debug; }
  void SetQA(CbmTrdCheckUtil* qa) { fQA = qa; }


  Bool_t GetSetter() { return fSet; }

private:
  CbmTrdRawToDigiR(const CbmTrdRawToDigiR&);
  CbmTrdRawToDigiR operator=(const CbmTrdRawToDigiR&);

  Double_t fCalibration = 35. / 1.1107 / 0.8;  // calibrating pulse height to MIP
  Double_t fEReco = 0.;   // calibrating energy reconstruction to the amount and position of used samples of the pulse
  Double_t fTau   = 120;  // peaking time of the pulse
  Int_t fRecoMode = 1;    // pre defined mode for reconstruction samples
  Int_t fShapingOrder  = 1;                // shaping order in analytic response calculation
  Int_t fDynamicRange  = 500;              // maximum of ADC range to identiefy time shift
  Int_t fHighBin       = 3;                // additional sample position for lookup table to identiefy time shift
  Int_t fPresamples    = 2;                // additional sample position for lookup table to identiefy time shift
  Int_t fMaxBin        = 2 + fPresamples;  // expected maximum position for the pulse
  Int_t fMinBin        = 1 + fPresamples;  // additional sample position for lookup table
  Float_t fExtrapolate = .2;               // percentage extrapolation range for analytical solution
  std::vector<Int_t> fSampleMask;          // reconstruction mask
  Double_t CalcResponse(Double_t t);

  Int_t fLookUp = 3;

  Bool_t fDebug          = false;  // Debug switch to output calculation time of the lookup
  Bool_t fSet            = false;  // Boolean for module initialisation in simulation
  std::string fReadFile  = "";     // filepath to existing lookup table
  std::string fWriteFile = "";     // filepath to write newly calculated lookup table into

  CbmTrdCheckUtil* fQA = NULL;

  std::map<Int_t, std::map<Int_t, Float_t>> fElookupSmall;
  std::map<Int_t, std::map<Int_t, std::map<Int_t, Int_t>>> fElookupAsym;
  std::map<Int_t, std::map<Int_t, Int_t>> fElookupA;
  std::map<Int_t, std::map<Int_t, std::map<Int_t, Int_t>>> fElookupBig;
  std::map<Int_t, Float_t> fElookup;

  ClassDef(CbmTrdRawToDigiR, 1);
};

#endif
