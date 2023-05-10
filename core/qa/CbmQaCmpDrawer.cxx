/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCmpDrawer.cxx
/// @brief  Class for a canvas with a comparison of multiple graphs or histograms (implementation)
/// @since  22.04.2023
/// @author Sergei Zharko <s.zharko@gsi.de>

#include "CbmQaCmpDrawer.h"

#include "TH1F.h"
#include "TProfile.h"

templateClassImp(CbmQaCmpDrawer);

template class CbmQaCmpDrawer<TH1F>;
template class CbmQaCmpDrawer<TProfile>;
template class CbmQaCmpDrawer<TH2F>;
template class CbmQaCmpDrawer<TProfile2D>;
