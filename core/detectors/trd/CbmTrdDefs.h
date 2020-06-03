/*
 * File: /CbmTrdDefs.h
 * Created Date: Tuesday March 3rd 2020
 * Author: Pascal Raisig -- praisig@ikf.uni-frankfurt.de
 * -----
 * Last Modified: Wednesday March 25th 2020 12:57:01
 * Modified By: Pascal Raisig
 * -----
 * Purpose: Reference class for global definitions used within the CbmTrd project
 * -----
 */

#ifndef CBMTRDDEFS_H
#define CBMTRDDEFS_H

#include "Rtypes.h"

enum class eCbmTrdModuleTypes : Int_t
{
      kHighChDensitySmallR  = 1
    , kLowChDensitySmallR   = 3
    , kHighChDensityLargeR  = 5
    , kLowChDensityLargeR   = 7
    , kMcbmModule           = 8     // FIXME moduleType 8 has multiple definitions, check if non mCbm definitions are really needed. - PR 03/25/2020
    , kNmoduleTypes         = 5     // REMARK this number has to be updated by hand!
};  ///< Enum for moduleTypes of the rectangular TrdModules as used in geometry files. 


#endif
