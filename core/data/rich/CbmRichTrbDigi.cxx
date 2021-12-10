/* Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmRichTrbDigi.h"

#include <TObject.h>  // for TObject

CbmRichTrbDigi::CbmRichTrbDigi()
  : TObject()
  , fTDCid(0)
  , fHasLeadingEdge(false)
  , fHasTrailingEdge(false)
  , fLeadingEdgeChannel(0)
  , fTrailingEdgeChannel(0)
  , fLeadingEdgeTimestamp(0.)
  , fTrailingEdgeTimestamp(0.)
{
}

CbmRichTrbDigi::CbmRichTrbDigi(uint32_t TDCid, bool hasLedge, bool hasTedge, uint32_t Lch, uint32_t Tch,
                               double Ltimestamp, double Ttimestamp)
  : TObject()
  , fTDCid(TDCid)
  , fHasLeadingEdge(hasLedge)
  , fHasTrailingEdge(hasTedge)
  , fLeadingEdgeChannel(Lch)
  , fTrailingEdgeChannel(Tch)
  , fLeadingEdgeTimestamp(Ltimestamp)
  , fTrailingEdgeTimestamp(Ttimestamp)
{
}

CbmRichTrbDigi::~CbmRichTrbDigi() {}

ClassImp(CbmRichTrbDigi)
