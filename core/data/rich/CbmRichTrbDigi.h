/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef CBMRICHTRBDIGI_H
#define CBMRICHTRBDIGI_H

#include <Rtypes.h>      // for ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>

class CbmRichTrbDigi : public TObject {
public:
  CbmRichTrbDigi();
  CbmRichTrbDigi(uint32_t TDCid, bool hasLedge, bool hasTedge, uint32_t Lch, uint32_t Tch, double Ltimestamp,
                 double Ttimestamp);
  virtual ~CbmRichTrbDigi();

  uint32_t GetTDCid() { return fTDCid; }

  bool GetHasLeadingEdge() { return fHasLeadingEdge; }
  bool GetHasTrailingEdge() { return fHasTrailingEdge; }

  uint32_t GetLeadingEdgeChannel() { return fLeadingEdgeChannel; }
  uint32_t GetTrailingEdgeChannel() { return fTrailingEdgeChannel; }

  double GetLeadingEdgeTimeStamp() { return fLeadingEdgeTimestamp; }
  double GetTrailingEdgeTimeStamp() { return fTrailingEdgeTimestamp; }

  double GetTOT() { return fTrailingEdgeTimestamp - fLeadingEdgeTimestamp; }

protected:
  uint32_t fTDCid;

  bool fHasLeadingEdge;
  bool fHasTrailingEdge;

  uint32_t fLeadingEdgeChannel;
  uint32_t fTrailingEdgeChannel;

  double fLeadingEdgeTimestamp;
  double fTrailingEdgeTimestamp;

  ClassDef(CbmRichTrbDigi, 1)
};

#endif  // CBMRICHTRBDIGI_H
