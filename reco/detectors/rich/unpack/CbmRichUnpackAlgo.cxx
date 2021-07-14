/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmRichUnpackAlgo.h"

#include <FairParGenericSet.h>
#include <FairTask.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>


CbmRichUnpackAlgo::CbmRichUnpackAlgo() : CbmRecoUnpackAlgo(fgkFlesSubsystemIdTrdR, "CbmRichUnpackAlgo") {}

CbmRichUnpackAlgo::~CbmRichUnpackAlgo() {}


ClassImp(CbmRichUnpackAlgo)
