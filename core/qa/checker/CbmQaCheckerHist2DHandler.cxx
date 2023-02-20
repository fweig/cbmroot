/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerHist2DHandler.h
/// @brief  Handler class for 2D-histograms (implementation)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  21.02.2023

#include "CbmQaCheckerHist2DHandler.h"

using cbm::qa::checker::Hist2DHandler;

// ---------------------------------------------------------------------------------------------------------------------
//
Hist2DHandler::Hist2DHandler(int iObject, int iFile, int iDataset) : Hist1DHandler(iObject, iFile, iDataset) {}
