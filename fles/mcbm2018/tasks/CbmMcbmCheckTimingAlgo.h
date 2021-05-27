/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMMCBMCHECKTIMINGALGO_H
#define CBMMCBMCHECKTIMINGALGO_H

#include "CbmModuleList.h"
#include "CbmTofDigi.h"

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

  ClassDefNV(CbmMcbmCheckTimingAlgo, 1);
};

#endif
