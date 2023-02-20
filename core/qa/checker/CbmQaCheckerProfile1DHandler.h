/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file   CbmQaCheckerProfile1DHandler.h
/// @brief  Handler class for 1D-profiles (declaration)
/// @author S. Zharko <s.zharko@gsi.de>
/// @since  19.02.2023

#ifndef CbmQaCheckerProfile1DHandler_h
#define CbmQaCheckerProfile1DHandler_h 1

#include "CbmQaCheckerHist1DHandler.h"

namespace cbm::qa::checker
{
  /// @brief  Specification of the handler for TProfile class
  ///
  class Profile1DHandler : public Hist1DHandler {
  public:
    /// @brief Constructor
    /// @param iObject  Index of object
    /// @param iFile    Index of file
    /// @param iDataset Index of dataset
    Profile1DHandler(int iObject, int iFile, int iDataset);

    /// @brief Destructor
    ~Profile1DHandler() = default;

    /// @brief Creates object comparison canvas
    void CreateCanvases();
  };
}  // namespace cbm::qa::checker

#endif  // CbmQaCheckerProfile1DHandler_h
