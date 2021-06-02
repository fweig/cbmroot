/** @file CbmRichTrbDigi.h
  * @copyright Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBMRICHTRBDIGI_H
#define CBMRICHTRBDIGI_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for UInt_t, Double_t, Bool_t
#include <TObject.h>     // for TObject

class CbmRichTrbDigi : public TObject {
public:
  CbmRichTrbDigi();
  CbmRichTrbDigi(UInt_t TDCid, Bool_t hasLedge, Bool_t hasTedge, UInt_t Lch, UInt_t Tch, Double_t Ltimestamp,
                 Double_t Ttimestamp);
  virtual ~CbmRichTrbDigi();

  UInt_t GetTDCid() { return fTDCid; }

  Bool_t GetHasLeadingEdge() { return fHasLeadingEdge; }
  Bool_t GetHasTrailingEdge() { return fHasTrailingEdge; }

  UInt_t GetLeadingEdgeChannel() { return fLeadingEdgeChannel; }
  UInt_t GetTrailingEdgeChannel() { return fTrailingEdgeChannel; }

  Double_t GetLeadingEdgeTimeStamp() { return fLeadingEdgeTimestamp; }
  Double_t GetTrailingEdgeTimeStamp() { return fTrailingEdgeTimestamp; }

  Double_t GetTOT() { return fTrailingEdgeTimestamp - fLeadingEdgeTimestamp; }

protected:
  UInt_t fTDCid;

  Bool_t fHasLeadingEdge;
  Bool_t fHasTrailingEdge;

  UInt_t fLeadingEdgeChannel;
  UInt_t fTrailingEdgeChannel;

  Double_t fLeadingEdgeTimestamp;
  Double_t fTrailingEdgeTimestamp;

  ClassDef(CbmRichTrbDigi, 1)
};

#endif  // CBMRICHTRBDIGI_H
