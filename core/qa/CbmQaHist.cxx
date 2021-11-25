/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/// \file   CbmQaHist.cxx
/// \brief  Implementation of the CbmQaHist class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   22.11.2020

#include "CbmQaHist.h"

templateClassImp(CbmQaHist);

// create definitions of specific CbmQaHist classes,
// otherwise they will not be linked by the ROOT linker

template class CbmQaHist<TH1F>;
template class CbmQaHist<TH1D>;
template class CbmQaHist<TH1I>;
template class CbmQaHist<TProfile>;
template class CbmQaHist<TProfile2D>;
