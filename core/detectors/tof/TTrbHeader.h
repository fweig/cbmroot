/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef TTRBHEADER_H_
#define TTRBHEADER_H_

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, UInt_t, Int_t, UShort_t, Bool_t
#include <TNamed.h>      // for TNamed

#include <vector>  // for vector

class TTrbHeader : public TNamed {
public:
  TTrbHeader();

  virtual ~TTrbHeader() {};

  virtual void Clear(Option_t* option = "");

  void SetTriggerPattern(UInt_t uVal) { fuTriggerPattern = uVal; }
  void SetTriggerType(UInt_t uVal) { fuTriggerType = uVal; }
  void SetTimeInSpill(Double_t dVal) { fdTimeInSpill = dVal; }
  void SetTimeInRun(Double_t dVal) { fdTimeInRun = dVal; }
  void SetSpillIndex(Int_t iVal) { fiSpillIndex = iVal; }
  void SetEventDAQDate(UInt_t uVal) { fuEventDAQDate = uVal; }
  void SetEventDAQTime(UInt_t uVal) { fuEventDAQTime = uVal; }
  void SetTriggerIndex(Int_t iVal) { fiTriggerIndex = iVal; }
  void SetCTSBusyTime(Double_t dVal) { fdCTSBusyTime = dVal; }
  void SetCTSIdleTime(Double_t dVal) { fdCTSIdleTime = dVal; }
  void SetSubeventSizes(const std::vector<UShort_t>& vuVal);

  UInt_t GetTriggerPattern() const { return fuTriggerPattern; }
  UInt_t GetTriggerType() const { return fuTriggerType; }
  Double_t GetTimeInSpill() const { return fdTimeInSpill; }
  Double_t GetTimeInRun() const { return fdTimeInRun; }
  Int_t GetSpillIndex() const { return fiSpillIndex; }
  UInt_t GetEventDAQDate() const { return fuEventDAQDate; }
  UInt_t GetEventDAQTime() const { return fuEventDAQTime; }
  Int_t GetTriggerIndex() const { return fiTriggerIndex; }
  Double_t GetCTSBusyTime() const { return fdCTSBusyTime; }
  Double_t GetCTSIdleTime() const { return fdCTSIdleTime; }
  UShort_t GetSubeventSize(Int_t iSubevent) const;
  Int_t GetNSubevents() const;

  Bool_t TriggerFired(Int_t iTrg);

private:
  UInt_t fuTriggerPattern;
  UInt_t fuTriggerType;
  Double_t fdTimeInSpill;  // in [s]
  Double_t fdTimeInRun;    // in [s]
  Int_t fiSpillIndex;
  UInt_t fuEventDAQDate;
  UInt_t fuEventDAQTime;
  Int_t fiTriggerIndex;
  Double_t fdCTSBusyTime;  // in [us]
  Double_t fdCTSIdleTime;  // in [us]

  std::vector<UShort_t> fuSubeventSizes;  // in bytes

  ClassDef(TTrbHeader, 3)
};
#endif
