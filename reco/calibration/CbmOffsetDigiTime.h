// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmOffsetDigiTime                            -----
// -----               Created 13.02.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMOFFSETDIGITIME_H
#define CBMOFFSETDIGITIME_H

/// CbmRoot (+externals) headers

/// FairRoot headers
#include "FairTask.h"

/// Fairsoft (Root, Boost, ...) headers
#include "TClonesArray.h"

/// C/C++ headers
#include <map>
#include <set>
#include <vector>

template<class TDigi>
class CbmOffsetDigiTime : public FairTask {
public:
  CbmOffsetDigiTime(TString sDigiBranchName, TString sDigiCalBranchName = "", TString sDigiCalBranchDescr = "");

  CbmOffsetDigiTime(const CbmOffsetDigiTime&) = delete;
  CbmOffsetDigiTime operator=(const CbmOffsetDigiTime&) = delete;

  ~CbmOffsetDigiTime();


  /** Initiliazation of task at the beginning of a run. Inherited from FairTask. **/
  virtual InitStatus Init();

  /** ReInitiliazation of task when the runID changes. Inherited from FairTask. **/
  virtual InitStatus ReInit();


  /** Executed for each event. Inherited from FairTask. **/
  virtual void Exec(Option_t*);

  /** Load the parameter container from the runtime database. Inherited from FairTask. **/
  virtual void SetParContainers();

  /** Finish task called at the end of the run. Inherited from FairTask. **/
  virtual void Finish();

  /// Task settings
  void SetWriteOutputFlag(Bool_t bFlagIn) { fbWriteOutput = bFlagIn; }
  void AddOffsetPoint(UInt_t uIndexTS, Double_t dOffset) { fmOffsets[uIndexTS] = dOffset; }
  void AddAddressToOffset(UInt_t uDigiAddress) { fsAddrToOffset.insert(uDigiAddress); }

private:
  /// Control flags
  Bool_t fbWriteOutput =
    kTRUE;  //! If ON the output vector of digi is written to disk by FairRoot, otherwise just made available for higher stages

  /// Statistics
  uint64_t fulTsCounter = 0;  //! TS counter, not same as TS index!

  /// Input/Output vectors (default)
  std::vector<TDigi> const* fvDigiIn = nullptr;  //! Input vector of Digis, recovered from framework for input
  std::vector<TDigi>* fvDigiOut      = nullptr;  //! Output vector of Digis, propagated to framework for output

  /// Input/Output Arrays (backup)
  TClonesArray* fArrayDigiIn  = nullptr;  //! Input array of Digis, recovered from framework for input
  TClonesArray* fArrayDigiOut = nullptr;  //! Output array of Digis, propagated to framework for output


  /// List of offsets with their boundaries
  std::map<UInt_t, Double_t> fmOffsets                = {};   //! Index is the first TS where the offset is valid.
  Double_t fdCurrentOffset                            = 0.0;  //! Current offset in [ns]
  std::map<UInt_t, Double_t>::iterator fmitNextOffset = fmOffsets.end();

  /// List of addresses to which the offset should be applied
  std::set<UInt_t> fsAddrToOffset = {};  //! Addresses for which the digi time is offset, if empty applied to all.

  /// Names for framework objects search
  TString fsDigiBranchName     = "";
  TString fsDigiBranchNameCal  = "";
  TString fsDigiBranchDescrCal = "";

  ClassDef(CbmOffsetDigiTime, 0);
};

/// Include implementation of templated methods
#include "CbmOffsetDigiTime.tmpl"

#endif  // CBMOFFSETDIGITIME_H
