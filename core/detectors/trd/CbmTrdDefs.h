/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Florian Uhlig [committer] */

/*
 * Purpose: Reference class for global definitions used within the CbmTrd project
 * -----
 */

#ifndef CBMTRDDEFS_H
#define CBMTRDDEFS_H

#include "Rtypes.h"

enum class eCbmTrdModuleTypes : Int_t
{
  kHighChDensitySmallR = 1,
  kLowChDensitySmallR  = 3,
  kHighChDensityLargeR = 5,
  kLowChDensityLargeR  = 7,
  kMcbmModule =
    8  // FIXME moduleType 8 has multiple definitions, check if non mCbm definitions are really needed. - PR 03/25/2020
    ,
  kNmoduleTypes = 5  // REMARK this number has to be updated by hand!
};                   ///< Enum for moduleTypes of the rectangular TrdModules as used in geometry files.


#endif
