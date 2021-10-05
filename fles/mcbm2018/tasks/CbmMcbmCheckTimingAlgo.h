/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Andreas Redelbach */

#ifndef CBMMCBMCHECKTIMINGALGO_H
#define CBMMCBMCHECKTIMINGALGO_H

#include "CbmModuleList.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"
#include "CbmTsEventHeader.h"

#include "TString.h"

#include <vector>

class TH1;
class TH2;
class CbmDigiManager;

class CheckTimingDetector {
public:
  CheckTimingDetector() { ; }
  CheckTimingDetector(ECbmModuleId detIdIn, std::string sNameIn)
  {
    detId = detIdIn;
    sName = sNameIn;
  }

  /// Settings
  ECbmModuleId detId     = ECbmModuleId::kNotExist;
  std::string sName      = "Invalid";
  Double_t dTimeRangeBeg = -1000.0;
  Double_t dTimeRangeEnd = 1000.0;
  UInt_t uRangeNbBins    = 320;
  UInt_t uChargeCutMin =
    0;  /// Charge cut used for example to reject/select pulser, no effect if equal, select if min < max, reject if max < min
  UInt_t uChargeCutMax =
    0;  /// Charge cut used for example to reject/select pulser, no effect if equal, select if min < max, reject if max < min

  /// Book-keeping variables
  Double_t dPrevTime      = 0.;
  Int_t iPrevRefFirstDigi = 0;
};

class CbmMcbmCheckTimingAlgo {
public:
  CbmMcbmCheckTimingAlgo();

  CbmMcbmCheckTimingAlgo(const CbmMcbmCheckTimingAlgo&) = delete;
  CbmMcbmCheckTimingAlgo operator=(const CbmMcbmCheckTimingAlgo&) = delete;

  /** Constructor with parameters (Optional) **/
  //  CbmMcbmCheckTimingAlgo(Int_t verbose);


  /** Destructor **/
  ~CbmMcbmCheckTimingAlgo();


  /** Initiliazation of task at the beginning of a run **/
  Bool_t Init();

  /** ReInitiliazation of task when the runID changes **/
  Bool_t ReInit();


  /** Executed for each event. **/
  void ProcessTs();

  /** Load the parameter container from the runtime database **/
  void SetParContainers();

  /** Finish task called at the end of the run **/
  void Finish();

  inline void SetOutFilename(TString sNameIn) { fOutFileName = sNameIn; }
  void WriteHistos();

  void SetReferenceDetector(ECbmModuleId refDetIn, std::string sNameIn, Double_t dTimeRangeBegIn = -1000.0,
                            Double_t dTimeRangeEndIn = 1000.0, UInt_t uRangeNbBinsIn = 320, UInt_t uChargeCutMinIn = 0,
                            UInt_t uChargeCutMaxIn = 0);
  void AddCheckDetector(ECbmModuleId detIn, std::string sNameIn, Double_t dTimeRangeBegIn = -1000.0,
                        Double_t dTimeRangeEndIn = 1000.0, UInt_t uRangeNbBinsIn = 320, UInt_t uChargeCutMinIn = 0,
                        UInt_t uChargeCutMaxIn = 0);
  void RemoveCheckDetector(ECbmModuleId detIn);

  void SetTrdPeakWidthNs(Double_t val = 120.) { fTrdPeakWidthNs = val; }
  void SetTrd2dPeakWidthNs(Double_t val = 120.) { fTrd2dPeakWidthNs = val; }
  void SetStsPeakWidthNs(Double_t val = 30.) { fStsPeakWidthNs = val; }
  void SetMuchPeakWidthNs(Double_t val = 100.) { fMuchPeakWidthNs = val; }
  void SetTofPeakWidthNs(Double_t val = 20.) { fTofPeakWidthNs = val; }
  void SetRichPeakWidthNs(Double_t val = 40.) { fRichPeakWidthNs = val; }
  void SetPsdPeakWidthNs(Double_t val = 20.) { fPsdPeakWidthNs = val; }

private:
  void CheckDataPresence(CheckTimingDetector detToCheck);
  void CreateHistos();

  template<class DigiRef>
  void CheckInterSystemOffset();
  template<class Digi>
  void FillTimeOffsetHistos(const Double_t dRefTime, const Double_t dRefCharge, UInt_t uDetIdx);


  /** Input array from previous already existing data level **/
  CbmDigiManager* fDigiMan = nullptr;  //!

  /** T0 is not included in CbmDigiManager, so add it explicitly here **/
  const std::vector<CbmTofDigi>* fpT0DigiVec = nullptr;  //!
  /** TRD-2D is not included in CbmDigiManager, so add it explicitly here **/
  const std::vector<CbmTrdDigi>* fpTrd2dDigiVec = nullptr;  //!

  /** @brief Pointer to the Timeslice start time used to write it to the output tree
      @remark since we hand this to the FairRootManager it also wants to delete it and we do not have to take care of deletion
   **/
  CbmTsEventHeader* fCbmTsEventHeader = nullptr;

  //
  UInt_t fuNbTs = 0;

  CheckTimingDetector fRefDet {CheckTimingDetector(ECbmModuleId::kT0, "T0")};
  std::vector<CheckTimingDetector> fvDets {
    CheckTimingDetector(ECbmModuleId::kSts, "Sts"),   CheckTimingDetector(ECbmModuleId::kMuch, "Much"),
    CheckTimingDetector(ECbmModuleId::kTrd, "Trd"),   CheckTimingDetector(ECbmModuleId::kTof, "Tof"),
    CheckTimingDetector(ECbmModuleId::kRich, "Rich"), CheckTimingDetector(ECbmModuleId::kPsd, "Psd")};

  /// vectors storing histograms for each detector under investigation
  std::vector<TH1*> fvhDetSelfDiff           = {};
  std::vector<TH1*> fvhDetToRefDiff          = {};
  std::vector<TH2*> fvhDetToRefDiffRefCharge = {};
  std::vector<TH2*> fvhDetToRefDiffDetCharge = {};
  std::vector<TH2*> fvhDetToRefDiffEvo       = {};
  std::vector<TH2*> fvhDetToRefDiffEvoLong   = {};

  /** Name of the histogram output file **/
  TString fOutFileName = "data/HistosCheckTiming.root";
  Double_t DetPeakPosSingle;
  Double_t DetAverageSingle;
  Double_t fTrdPeakWidthNs   = 120.;
  Double_t fTrd2dPeakWidthNs = 120.;
  Double_t fStsPeakWidthNs   = 30.;
  Double_t fMuchPeakWidthNs  = 100.;
  Double_t fTofPeakWidthNs   = 20.;
  Double_t fRichPeakWidthNs  = 40.;
  Double_t fPsdPeakWidthNs   = 20.;

  ClassDefNV(CbmMcbmCheckTimingAlgo, 1);
};

#endif
