/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CaToolsPerformance.h
/// \brief  Tracking performance class (header)
/// \since  23.09.2022
/// \author S.Zharko <s.zharko@gsi.de>

#ifndef CaToolsPerformance_h
#define CaToolsPerformance_h 1

#include "CaToolsMcData.h"

class L1Parameters;

namespace ca
{
  namespace tools
  {
    /// Class ca::tools::Performance defines CA tracking internal performance measurement
    ///
    class Performance {
    public:
      // *****************************************
      // **     Constructors and destructor     **
      // *****************************************


      /// Default constructor
      Performance() = default;

      /// Destructor
      ~Performance() = default;

      /// Copy constructor
      Performance(const Performance& other) = delete;

      /// Move constructor
      Performance(Performance&& other) = delete;

      /// Copy assignment operator
      Performance& operator=(const Performance& other) = delete;

      /// Move assignment operator
      Performance& operator=(Performance&& other) = delete;


      // ***********************
      // **     Accessors     **
      // ***********************

      /// Receives MC data object from McDataManager
      void ReceiveMcData(McData&& mcData);


    private:
      const L1Parameters* fpParameters = nullptr;  ///< Instance of the tracking core class parameters
      McData fMcData                   = {};       ///< Object of MC data
    };
  }  // namespace tools
}  // namespace ca

#endif  // CaToolsPerformance_h
