/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerHist2DHandler.h
/// @brief  Handler class for 2D-histograms (declaration)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  21.02.2023

#ifndef CbmQaCheckerHist2DHandler_h
#define CbmQaCheckerHist2DHandler_h 1

#include "CbmQaCheckerHist1DHandler.h"

namespace cbm::qa::checker
{
  /// @brief  Specification of the handler for TProfile class
  ///
  class Hist2DHandler : public Hist1DHandler {
  public:
    /// @brief Constructor
    /// @param iObject  Index of object
    /// @param iFile    Index of file
    /// @param iDataset Index of dataset
    Hist2DHandler(int iObject, int iFile, int iDataset);

    /// @brief Destructor
    ~Hist2DHandler() = default;

    /// @brief Creates object comparison canvas
    void CreateCanvases() {};
  };
}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerProfile1DHandler_h
